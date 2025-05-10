#include "communication.h"
#include "utils.h"

void send_message(Process* process, Message* msg, int dest) {
    increment_clock(process);
    msg->lamport_clock = process->lamport_clock;
    MPI_Send(msg, sizeof(Message), MPI_BYTE, dest, 0, MPI_COMM_WORLD);
    printf("[%d] Sent %d to %d (clock: %d)\n", 
           process->rank, msg->type, dest, msg->lamport_clock);
    fflush(stdout);
}

void receive_message(Process* process, Message* msg, int* source, int blocking) {
    MPI_Status status;
    MPI_Recv(msg, sizeof(Message), MPI_BYTE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

    *source = status.MPI_SOURCE;
    update_clock(process, msg->lamport_clock);
    printf("[%d] Received %d from %d (clock: %d)\n", 
           process->rank, msg->type, *source, process->lamport_clock);
    fflush(stdout);
}

void broadcast_message(Process* process, Message* msg, int processes_count) {
    for (int i = 0; i < processes_count; i++) {
        if (i != process->rank) {
            send_message(process, msg, i);
        }
    }
}

void wait_for_acks(Process* process, int min_ack_num, int processes_count) {
    Message msg;
    int source;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    while (process->ack_count < min_ack_num) {
        receive_message(process, &msg, &source, 0);

        if (msg.type == MSG_ACK) {
            process->ack_count++;
            printf("[%d] ACK: (%d / %d)\n", rank, process->ack_count, min_ack_num);
            fflush(stdout);
        }
        else if (msg.type == MSG_REQ_HOUSE || msg.type == MSG_REQ_PASER) {
            Request req = {
                .rank = msg.rank,
                .lamport_clock = msg.lamport_clock,
                .house_ID = msg.house_ID
            };
            
            Queue* target_queue;
            if (msg.type == MSG_REQ_HOUSE) {
                target_queue = &process->house_queue;
            } else {
                target_queue = &process->paser_queue;
            }
            
            enqueue(target_queue, req);
            
            Message ack = {
                .type = MSG_ACK,
                .rank = process->rank,
                .lamport_clock = ++process->lamport_clock
            };
            send_message(process, &ack, msg.rank);
        }
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
    while (!is_queue_empty(&process->paser_queue)) {
        dequeue(&process->paser_queue, &req);
        
        Message ack = {
            .type = MSG_ACK,
            .rank = process->rank,
            .lamport_clock = ++process->lamport_clock,
            .house_ID = -1
        };
        send_message(process, &ack, req.rank);
    }
}
