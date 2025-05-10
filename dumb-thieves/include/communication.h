//
// Created by hkere on 26/04/2025.
//

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "utils.h"

typedef enum Message_type {
    MSG_REQ_HOUSE,
    MSG_REQ_FENCE,
    MSG_ACK
} Message_type;

typedef struct Message {
    int type;
    int rank;
    int lamport_clock;
    int house_ID;
} Message;

void send_message(Process* process, Message* msg, int dest);
void receive_message(Process* process, Message* msg, int* source);
void broadcast_message(Process* process, Message* msg, int num_processes);
void wait_for_acks(Process* process, int min_ack_num);
void leave_critical_sections(Process* process);
void* listener_thread(void* arg);
const char* msg_type_to_string(Message_type type);


#endif //COMMUNICATION_H
