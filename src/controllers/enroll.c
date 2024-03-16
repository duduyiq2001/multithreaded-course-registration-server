#include "controllers/enroll.h"
#include "protocol/wmessage.h"
#include "controllers/managers/stats_manager.h"
#include "controllers/managers/courselist_manager.h"
#include "controllers/managers/logfile_manager.h"
#include "controllers/managers/userlist_manager.h"
/**
 * @brief
 * first atoi the course index
 * send back error if index is non valid
 * enroll if the user is not already enrolled and
 * the course is not full
 *
 * @param user
 * @param course_index
 */
void enroll(user_t *user, char *course_index)
{
    // get index
    int index = atoi(course_index);
    if (index > num_of_courses - 1)
    {
        // sending course not found error
        send_error(user->socket_fd, ECNOTFOUND);
    }

    // course is valid
    int enroll_result;
    enroll_result = add_user(user, index);
#ifdef DEBUG
    course_print();
#endif
    if (enroll_result == 0)
    {
        // logging error
        write_log(logFile, "NOTFOUND_E", user->username);
        // course already full
        // return error
        if (send_error(user->socket_fd, ECDENIED) == 0)
        {
            fprintf(stderr, "senderror");
        };
    }

    if (enroll_result == 1)
    {
        // logging success
        write_log(logFile, "ENROLL", user->username);
        // course added
        // update stats
        update_stats("adds", 1);
        // respond ok to client
        if (send_message(user->socket_fd, OK, "") == 0)
        {
            fprintf(stderr, "senderror");
        };
    }

    if (enroll_result == 2)
    {
        // user already enrolled
        // logging error
        write_log(logFile, "NOENROLL", user->username);
        // course already full
        // return error
        if (send_error(user->socket_fd, ECDENIED) == 0)
        {
            fprintf(stderr, "senderror");
            fflush(stderr);
        };
    }
#ifdef DEBUG
    // DeleteList(&userList);
    // course_print();

#endif
}