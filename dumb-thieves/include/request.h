#ifndef REQUEST_H
#define REQUEST_H

typedef struct Request {
    int rank;
    int lamport_clock;
    int house_ID;
} Request;

#endif