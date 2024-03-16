#include "protocol/wmessage.h"

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
int send_message(int socket_fd, enum msg_types msg_type, char *msg)
{
    // construct header
    petrV_header header;
    header.msg_len = (uint32_t)strlen(msg);
    header.msg_type = (uint8_t)msg_type;
    // send message

    if (wr_msg(socket_fd, &header, msg) == 0)
    {
        // if success
        return 1;
    }

    // write error
    return 0;
}

/**
 * @brief send an error to client
 * return 1 upon success
 * 0 upon error
 * @param socket_fd
 * @param error_type
 * @return int
 */
int send_error(int socket_fd, enum msg_types error_type)
{
    // construct header
    petrV_header header;
    header.msg_len = 0;
    header.msg_type = (uint8_t)error_type;
    // send error
    if (wr_msg(socket_fd, &header, "") == 0)
    {
        // if successs
        return 1;
    }
    // 0 on error
    return 0;
}