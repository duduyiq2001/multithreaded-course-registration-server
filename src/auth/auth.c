
#include "auth/auth.h"
#include "protocol/rmessage.h"
#include "protocol/wmessage.h"
#include "utilities/multiplex.h"
#include "controllers/managers/userlist_manager.h"
#include "controllers/managers/logfile_manager.h"
#include "controllers/managers/stats_manager.h"
/**
 * @brief authenticate the client by WAITing on
 * login request
 * handle client communication
 * if success return the user
 * if not return NULL
 * @param clientfd_ptr
 * @return int
 */
user_t *authenticate(void *clientfd_ptr, char **username)
{
    // dereference client
    int client_fd = *(int *)clientfd_ptr;

    enum msg_types m_type;

    /**
     * @brief waiting for client to be ready
     * block if not
     */
    if (wait_for_client(client_fd) == 0)
    {
        // wait error
        send_error(client_fd, ESERV);
        return NULL;
    }

    /**
     * @brief read login message from client
     *
     */
    if (read_client(client_fd, &m_type, username) == 0)
    {
        fprintf(stderr, "msg read error");
        send_error(client_fd, ESERV);
        // read error
        return NULL;
    }
    // checking if message is valid

    if (m_type != LOGIN)
    {
        fprintf(stderr, "no login request");
        send_error(client_fd, ESERV);
        return NULL;
    }

    // check active users
    user_t *new_user = find_user(*username);
    if (new_user != NULL && new_user->socket_fd != -1)
    {
        // send back error
        // user already logged in
        send_error(client_fd, EUSRLGDIN);
        // logging error
        write_log(logFile, "REJECT", new_user->username);
        return NULL;
    }

    // logging in an existing user
    if (new_user != NULL)
    {
        // logging
        write_log(logFile, "RECONNECTED", new_user->username);
        // resetting client fd
        new_user->socket_fd = client_fd;
        // respond ok to client

        send_message(client_fd, OK, "");

        return login_user(*username, client_fd);
    }
    else
    { // register user in session

        // logging
        write_log(logFile, "CONNECTED", *username);
        // respond ok to client

        send_message(client_fd, OK, "");
        return insert_user(*username, client_fd);
    }
}

/**
 * @brief update user_t when logout
 * set socket, tid to -1
 *
 * @param user
 */
void logout(user_t *user)
{
    // // respond ok after detach
    send_message(user->socket_fd, OK, "");
    // setting user
    user->socket_fd = -1;
    user->tid = -1;
    // writing to log
    write_log(logFile, "LOGOUT", user->username);

    return;
}