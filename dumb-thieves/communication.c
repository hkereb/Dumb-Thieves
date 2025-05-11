#include "communication.h"
#include "utils.h"
#include <pthread.h>
#include <unistd.h>
#include <time.h>

void send_message(Process* process, Message* msg, int dest, FILE* fp) {
    MPI_Send(msg, sizeof(Message), MPI_BYTE, dest, 0, MPI_COMM_WORLD);
    custom_printf(fp, "[P%d] (clock: %d) SENT %s to P%d, while %s\n",
           process->rank, msg->lamport_clock, msg_type_to_string(msg->type), dest, state_to_string(process->state));
}

void receive_message(Process* process, Message* msg, int* source, FILE* fp) {
    MPI_Status status;
    MPI_Recv(msg, sizeof(Message), MPI_BYTE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    *source = status.MPI_SOURCE;

    update_clock_upon_recv(process, msg->lamport_clock);
    custom_printf(fp, "[P%d] (clock: %d) RECEIVED %s from P%d, while %s\n",
           process->rank, process->lamport_clock, msg_type_to_string(msg->type), *source, state_to_string(process->state));
}

void broadcast_message(Process* process, Message* msg, int num_processes, FILE* fp) {
    for (int i = 0; i < num_processes; i++) {
        if (i != process->rank) {
            send_message(process, msg, i, fp);
        }
    }
}

void wait_for_ack(Process* process, int min_ack_num) {
    struct timespec req = {0, 10 * 1000 * 1000}; // 10ms
    while (process->ack_count < min_ack_num) {
        nanosleep(&req, NULL);
    }
}

void send_ack(Process* process, int destination_rank, FILE* fp) {
    increment_clock_by_one(process);
    Message ack = {
        .type = MSG_ACK,
        .rank = process->rank,
        .lamport_clock = process->lamport_clock
    };
    send_message(process, &ack, destination_rank, fp);
}

void* listener_thread(void* arg) {
    ThreadArgs* thread_args = (ThreadArgs*)arg;
    Process* process = thread_args->process;
    FILE* fp = thread_args->fp;

    Message msg;
    int source;

    while (true) {
        receive_message(process, &msg, &source, fp);

        if (msg.type == MSG_ACK) {
            process->ack_count += 1;
            custom_printf(fp, "[P%d] (clock: %d) My ACK: %d, while %s\n", process->rank, process->lamport_clock, process->ack_count, state_to_string(process->state));
        }
        else if (msg.type == MSG_REQ_HOUSE || msg.type == MSG_REQ_FENCE) {
            // which queue will they go to
            Queue* target_queue = (msg.type == MSG_REQ_HOUSE) ? &process->house_queue : &process->fence_queue;

            // do I also want this resource?
            bool waiting = false;
            bool using = false;
            int my_clock = process->last_req_clock;

            if (msg.type == MSG_REQ_HOUSE) {
                waiting = (process->state == WAITING_FOR_HOUSE && process->house_ID == msg.house_ID);
                using = (process->state == ROBBING_HOUSE && process->house_ID == msg.house_ID);
            }
            else if (msg.type == MSG_REQ_FENCE) {
                waiting = process->state == WAITING_FOR_FENCE;
                using = process->state == HAS_FENCE;
            }

            if (!waiting && !using) {
                // i dont have it, i dont want it
                send_ack(process, source, fp);
            }
            else if (waiting) {
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
                    send_ack(process, source, fp);
                }
            }
            else {
                // im using it
                Request req = {
                    .rank = msg.rank,
                    .lamport_clock = msg.lamport_clock,
                    .house_ID = msg.house_ID
                };
                enqueue(target_queue, req);
            }
        }
    }
}
