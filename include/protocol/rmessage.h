#ifndef RMESSAGE
#define RMESSAGE
#include "../protocol.h"
#include <unistd.h>
#include <stdlib.h>
/**
 * @brief read from client,
 * place msg_type , message content into param
 * 1 success, 0 error
 * @param socket_fd
 * @param msg_type
 * @param message
 * @return int
 */
int read_client(int socket_fd, enum msg_types *msg_type, char **message);

// read(socket_fd, *message, length);
/**
 * @brief recusively call read,
 * check errno
 */
int recur_read(int socket_fd, char *message, int length);

#endif