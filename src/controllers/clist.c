#include "controllers/clist.h"
#include "controllers/managers/logfile_manager.h"
#include "controllers/managers/courselist_manager.h"
#include "protocol/wmessage.h"

/**
 * @brief
 *
 * call the courselist clist print function
 * send the string to user
 * @param user
 * @param course_index
 */
void clist(user_t *user)
{
    // logging
    write_log(logFile, "CLIST", user->username);
    // build response
    char *response = clist_print();
    // send back to user
    send_message(user->socket_fd, CLIST, response);
    // free response
    free(response);
    return;
}