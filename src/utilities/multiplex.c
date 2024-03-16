#include "utilities/multiplex.h"
#include "server.h"
#include "errno.h"

/**
 * @brief use select to wait for client fd to be ready
 *
 * would block if not
 * return 1 success,
 * 0 error
 * 2 sigint
 * @param client_fd
 */
int wait_for_client(int client_fd)
{
    // initialize resources
    fd_set read_fds;
    int retval;

    // setting mask to 0s
    FD_ZERO(&read_fds);
    // set client fd
    FD_SET(client_fd, &read_fds);
    // wait using select
    retval = select(client_fd + 1, &read_fds, NULL, NULL, NULL);
    // checking errno
    if (errno == EINTR)
    {
#ifdef DEBUG
        printf("EINTR detected");
        fflush(stdout);
#endif
        // if indeed sigint
        if (sigint_flag)
        {
            return 2;
        }
        // if not resume
        return wait_for_client(client_fd);
    }

    if (retval != 1 && !FD_ISSET(client_fd, &read_fds))
    {
        fprintf(stderr, "Error with select() function\n");
        return 0;
    }
    return 1;
}