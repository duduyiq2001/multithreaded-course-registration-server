#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>
#include <string.h>

// These are the message types for the PetrV protocol
enum msg_types
{
    OK,
    LOGIN,
    LOGOUT,
    CLIST,
    SCHED,
    ENROLL,
    DROP,
    WAIT,
    EUSRLGDIN = 0xF0,
    ECDENIED,
    ECNOTFOUND,
    ECNOCOURSES,
    ESERV = 0xFF
};

// This is the struct describes the header of the PetrV protocol messages
typedef struct
{
    uint32_t msg_len; // this should include the null terminator
    uint8_t msg_type;
} petrV_header;

// Read a petr_header from the socket_fd. Places contents into the memory // referenced by h. Returns 0 upon success, -1 on error.
int rd_msgheader(int socket_fd, petrV_header *h);

// Write petr_header reference by h and the msgbuf to the socket_fd. // If the msg_len in h is 0, the msgbuf is ignored // Returns 0 upon success, -1 on error
int wr_msg(int socket_fd, petrV_header *h, char *msgbuf);

#endif