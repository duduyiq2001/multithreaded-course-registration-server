#include "controllers/waitlist.h"
#include "protocol/wmessage.h"
#include "controllers/managers/stats_manager.h"
#include "controllers/managers/courselist_manager.h"
#include "controllers/managers/logfile_manager.h"
#include "controllers/managers/userlist_manager.h"
#include "protocol.h"
/**
 * @brief
 * first atoi the course index
 * send back error if index is non valid
 * waitlist if the user is not already waitlisted or waitlisted
 * @param user
 * @param course_index
 */
void waitlist(user_t *user, char *course_index)
{
    // get index
    int index = atoi(course_index);
    if (index > num_of_courses - 1)
    {
        // logging error
        write_wait(logFile, user, index, NOTFOUND);
        // sending course not found error
        send_error(user->socket_fd, ECNOTFOUND);
        return;
    }

    // course is valid
    int wait_result;
    wait_result = wait_user(user, index);
#ifdef DEBUG
    course_print();
#endif
    if (wait_result == 0)
    {
        // user already waited
        // logging error
        write_wait(logFile, user, index, NEGATIVE);
        // course not full
        // return error
        if (send_error(user->socket_fd, ECDENIED) == 0)
        {
            fprintf(stderr, "senderror");
        };
    }

    if (wait_result == 1)
    {
        // logging success
        write_wait(logFile, user, index, POSITIVE);
        // respond ok to client
        if (send_message(user->socket_fd, OK, "") == 0)
        {
            fprintf(stderr, "senderror");
        };
    }

    if (wait_result == 2)
    {
        // user already waited
        // logging error
        write_wait(logFile, user, index, NEGATIVE);
        // course already full
        // return error
        if (send_error(user->socket_fd, ECDENIED) == 0)
        {
            fprintf(stderr, "senderror");
            fflush(stderr);
        };
    }
}