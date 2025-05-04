//
// Created by hkere on 26/04/2025.
//

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "process.h"

typedef enum Message_type {
    MSG_REQ_HOUSE,
    MSG_REQ_PASER,
    MSG_ACK
} Message_type;

typedef struct Message {
    Message_type type;
    int rank;
    int lamport_clock;
    int house_ID;
} Message;

void send_message(Message* msg, int dest);
void receive_message(Message* msg, int* source);
void broadcast_message(Message *msg, int processes_count);
void wait_for_acks(Process* process, int min_ack_num);
void leave_pasers(Process* process);
#endif //COMMUNICATION_H
