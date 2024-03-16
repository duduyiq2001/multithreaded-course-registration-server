#include "protocol/rmessage.h"
#include "errno.h"
#include "server.h"
/**
 * @brief read from client,
 * place msg_type , message content into param
 * malloc message, need free
 * 1 success, 0 error, 2 sigint
 * @param socket_fd
 * @param msg_type
 * @param message
 * @return int
 */
int read_client(int socket_fd, enum msg_types *msg_type, char **message)
{
    // construct header
    petrV_header header;

    if (rd_msgheader(socket_fd, &header) == 0)
    {
        // checking errno
        if (errno == EINTR)
        {
            // if indeed sigint
            if (sigint_flag)
            {
                return 2;
            }
            // if not resume
            return read_client(socket_fd, msg_type, message);
        }
        *msg_type = header.msg_type;
        // getting the len of the message
        int length = header.msg_len;
        // mallocing message
        *message = (char *)malloc(sizeof(char) * (length) + sizeof(char));
        // read message based on len
        return recur_read(socket_fd, *message, length);
    }
    // checking errno
    if (errno == EINTR)
    {
        // if indeed sigint
        if (sigint_flag)
        {
            return 2;
        }
        // if not resume
        return read_client(socket_fd, msg_type, message);
    }
    return 0;
}

/**
 * @brief recusively call read,
 * check errno
 *
 */
int recur_read(int socket_fd, char *message, int length)
{
    // read message based on len
    int received_size = read(socket_fd, message, length);

    // checking errno
    if (errno == EINTR)
    {
        // if indeed sigint
        if (sigint_flag)
        {
            // free message
            free(message);
            return 2;
        }
        // if not resume
        return recur_read(socket_fd, message, length);
    }
    // setting null terminator
    message[length] = '\0';

    // check if length is correct
    if (received_size != length)
    {
        return 0;
    }
    // sucess
    return 1;
}