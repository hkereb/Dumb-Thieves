#include <mpi.h>
#include <stdio.h>
#include "communication.h"
#include "utils.h"

void send_message(Message* msg, int dest){
    msg->lamport_clock += 1;
    MPI_Send(msg, sizeof(Message), MPI_BYTE, dest, 0, MPI_COMM_WORLD);
    printf("[Process %d] Sent message %d to %d (local time = %d)\n", msg->rank, msg->type, dest, msg->lamport_clock);
}

void receive_message(Message* msg, int* source) {
    MPI_Status status;
    MPI_Recv(msg, sizeof(Message), MPI_BYTE, 0, 0, MPI_COMM_WORLD, &status);
    update_clock(msg->lamport_clock);
    printf("[Process %d] Received message %d from %d (local time = %d)\n", msg->rank, msg->type, *source, lamport_clock);
}

void broadcast_message(Message* msg, int processes_count) {
    for (int i = 0; i < processes_count; i++) {
        if (i != msg->rank){
            send_message(msg, i);
        }
    }
}

void wait_for_acks(Process* process, int min_ack_num) {
    Message msg; 
    int source;

    while (process->ack_count < min_ack_num) {
        receive_message(&msg, &source);

        if (msg.lamport_clock > process->lamport_clock) {
            process->lamport_clock = msg.lamport_clock;
        }
        process->lamport_clock++;

        if (msg.type == MSG_ACK) {
            process->ack_count++;
        }
    }
}

void leave_pasers(Process* process) {
    Request req;
    
    while (!is_queue_empty(&process->house_queue)) {
        dequeue(&process->house_queue, &req);

        Message ack_msg;
        ack_msg.type = MSG_ACK;
        ack_msg.rank = process->rank;
        ack_msg.lamport_clock = ++process->lamport_clock;
        ack_msg.house_ID = req.house_ID;

        send_message(&ack_msg, req.rank);
    }

    while (!is_queue_empty(&process->fence_queue)) {
        dequeue(&process->fence_queue, &req);

        Message ack_msg;
        ack_msg.type = MSG_ACK;
        ack_msg.rank = process->rank;
        ack_msg.lamport_clock = ++process->lamport_clock;
        ack_msg.house_ID = req.house_ID;

        send_message(&ack_msg, req.rank);
    }
}

