//
// Created by hkere on 26/04/2025.
//

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "process.h"

typedef enum MessageType {
    MSG_REQ_HOUSE,
    MSG_REQ_FENCE,
    MSG_ACK
} MessageType;

typedef struct Message {
    int type;
    int rank;
    int lamport_clock;
    int house_ID;
} Message;

void send_message(Process* process, Message* msg, int dest);
void receive_message(Process* process, Message* msg, int* source);
void broadcast_message(Process* process, Message* msg, int num_processes);
void wait_for_ack(Process* process, int min_ack_num);
void* listener_thread(void* arg);

#endif //COMMUNICATION_H
