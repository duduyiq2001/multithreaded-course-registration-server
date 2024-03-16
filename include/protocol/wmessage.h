#ifndef WMESSAGE
#define WMESSAGE
#include "../protocol.h"
#include <unistd.h>

/**
 * @brief send a message to client
 * return 1 upon success
 * 0 upon error
 *
 * @param socket_fd
 * @param msg_type
 * @param msg
 * @return int
 */
int send_message(int socket_fd, enum msg_types msg_type, char *msg);

/**
 * @brief send an error to client
 * return 1 upon success
 * 0 upon error
 * @param socket_fd
 * @param error_type
 * @return int
 */
int send_error(int socket_fd, enum msg_types error_type);

#endif