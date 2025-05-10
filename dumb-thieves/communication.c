#include "communication.h"
#include "utils.h"
#include <pthread.h>
#include <unistd.h>
#include <time.h>

void send_message(Process* process, Message* msg, int dest) {
    MPI_Send(msg, sizeof(Message), MPI_BYTE, dest, 0, MPI_COMM_WORLD);
    printf("[P%d] (clock: %d) SENT %s to P%d\n",
           process->rank, msg->lamport_clock, msg_type_to_string(msg->type), dest);
}

void receive_message(Process* process, Message* msg, int* source) {
    MPI_Status status;
    MPI_Recv(msg, sizeof(Message), MPI_BYTE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    *source = status.MPI_SOURCE;
    // process->lamport_clock = max(process->lamport_clock, msg->lamport_clock) + 1;
    printf("[P%d] (clock: %d) RECEIVED %s from P%d\n",
           process->rank, process->lamport_clock, msg_type_to_string(msg->type), *source);
}

void broadcast_message(Process* process, Message* msg, int num_processes) {
    for (int i = 0; i < num_processes; i++) {
        if (i != process->rank) {
            send_message(process, msg, i);
        }
    }
}

void wait_for_acks(Process* process, int min_ack_num) {
    struct timespec req = {0, 10 * 1000 * 1000}; // 10ms
    while (process->ack_count < min_ack_num) {
        nanosleep(&req, NULL);
    }
}

void leave_critical_sections(Process* process) {
    Request req;
    
    // house
    while (!is_queue_empty(&process->house_queue)) {
        dequeue(&process->house_queue, &req);
        
        Message ack = {
            .type = MSG_ACK,
            .rank = process->rank,
            .lamport_clock = ++process->lamport_clock,
            .house_ID = -1
        };
        send_message(process, &ack, req.rank);
    }
    
    // paser
    while (!is_queue_empty(&process->fence_queue)) {
        dequeue(&process->fence_queue, &req);
        
        Message ack = {
            .type = MSG_ACK,
            .rank = process->rank,
            .lamport_clock = ++process->lamport_clock,
            .house_ID = -1
        };
        send_message(process, &ack, req.rank);
    }
}

void* listener_thread(void* arg) {
    Process* process = arg;
    Message msg;
    int source;

    while (true) {
        receive_message(process, &msg, &source);

        // Aktualizacja zegara Lamporta
        process->lamport_clock = max(process->lamport_clock, msg.lamport_clock) + 1;

        if (msg.type == MSG_ACK) {
            process->ack_count += 1;
            printf("[P%d] (clock: %d) My ACK: %d\n", process->rank, process->lamport_clock, process->ack_count);
        }

        else if (msg.type == MSG_REQ_HOUSE || msg.type == MSG_REQ_FENCE) {
            // Określamy docelową kolejkę
            Queue* target_queue = (msg.type == MSG_REQ_HOUSE) ? &process->house_queue : &process->fence_queue;

            // Sprawdzamy czy proces też ubiega się o ten zasób
            bool waiting = false;
            bool using = false;
            int my_clock = process->lamport_clock;

            if (msg.type == MSG_REQ_HOUSE) {
                waiting = process->state == WAITING_FOR_HOUSE;
                using = process->state == ROBBING_HOUSE;
            }
            else if (msg.type == MSG_REQ_FENCE) {
                waiting = process->state == WAITING_FOR_FENCE;
                using = process->state == HAS_FENCE;
            }

            if (!waiting && !using) {
                // i dont have it, i dont want it
                process->lamport_clock += 1;
                Message ack = {
                    .type = MSG_ACK,
                    .rank = process->rank,
                    .lamport_clock = process->lamport_clock
                };
                send_message(process, &ack, msg.rank);
            }
            else {
                // i have it or i want it
                bool i_have_priority =
                    (my_clock < msg.lamport_clock) ||
                    (my_clock == msg.lamport_clock && process->rank < msg.rank);

                if (i_have_priority) {
                    // im better, you have to wait
                    Request req = {
                        .rank = msg.rank,
                        .lamport_clock = msg.lamport_clock,
                        .house_ID = msg.house_ID
                    };
                    enqueue(target_queue, req);
                }
                else {
                    // you're better, go first
                    process->lamport_clock += 1;
                    Message ack = {
                        .type = MSG_ACK,
                        .rank = process->rank,
                        .lamport_clock = process->lamport_clock
                    };
                    send_message(process, &ack, msg.rank);
                }
            }
        }
    }
}


const char* msg_type_to_string(Message_type type) {
    switch (type) {
        case MSG_ACK: return "MSG_ACK";
        case MSG_REQ_HOUSE: return "MSG_REQ_HOUSE";
        case MSG_REQ_FENCE: return "MSG_REQ_FENCE";
        default: return "UNKNOWN";
    }
}
