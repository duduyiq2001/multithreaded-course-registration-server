#include "controllers/sched.h"
#include "controllers/managers/userlist_manager.h"
#include "controllers/managers/logfile_manager.h"
#include "protocol/wmessage.h"
#include "protocol.h"
/**
 * @brief
 *
 * call userlist printstudylist function
 * logging sched or no sched
 * send the string to user
 */
void sched(user_t *user)
{
    // getting enroll info from user
    char *response = GetStudyList(user);
    if (response == NULL)
    {
        // log error
        write_log(logFile, "NOSCHED", user->username);
        // send response to user
        send_error(user->socket_fd, ENOCOURSES);
        return;
    }
    // send studylist back
    send_message(user->socket_fd, SCHED, response);
    // free response
    free(response);
    // log success
    write_log(logFile, "SCHED", user->username);
    return;
}