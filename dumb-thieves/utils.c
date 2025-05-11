#include "utils.h"

int select_house(Process* process, int num_houses) {
    return  ((process->rank * (process->houses_visited_count + 1)) % num_houses + 1);
}

int max(int a, int b) {
    if (a > b) return a;

    return b;
}

const char* msg_type_to_string(MessageType type) {
    switch (type) {
        case MSG_ACK: return "MSG_ACK";
        case MSG_REQ_HOUSE: return "MSG_REQ_HOUSE";
        case MSG_REQ_FENCE: return "MSG_REQ_FENCE";
        default: return "UNKNOWN";
    }
}

const char* state_to_string(ProcessState state) {
    switch (state) {
        case RESTING: return "RESTING";
        case WAITING_FOR_HOUSE: return "WAITING_FOR_HOUSE";
        case ROBBING_HOUSE: return "ROBBING_HOUSE";
        case WAITING_FOR_FENCE: return "WAITING_FOR_FENCE";
        case HAS_FENCE: return "HAS_FENCE";
        default: return "UNKNOWN";
    }
}

void custom_printf(FILE* out, const char* fmt, ...) {
    va_list args;
    va_list args_copy;
    va_start(args, fmt);
    va_copy(args_copy, args);

    // terminal
    vprintf(fmt, args);

    // log file
    vfprintf(out, fmt, args_copy);
    
    va_end(args_copy);
    va_end(args);

    fflush(stdout);
    fflush(out);
}