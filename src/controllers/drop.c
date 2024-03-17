#include "controllers/drop.h"
#include "controllers/managers/logfile_manager.h"
#include "protocol/wmessage.h"
#include "protocol.h"
#include "controllers/managers/courselist_manager.h"
#include "controllers/managers/stats_manager.h"
#include "controllers/waitadd.h"
/**
 * @brief
 * first atoi the course index
 * send back error if index is non valid
 *
 * if user not even enrolled, send back ECDENIED
 * log username NODROP course_index\n
 *
 * IF enrolled, send OK , log username DROP course_index enrolled\n
 * username WAITADD course_index enrolled if there is waitadd
 * @param user
 * @param course_index
 */
void drop(user_t *user, char *course_index)
{
    // get index
    int index = atoi(course_index);
    if (index > num_of_courses - 1)
    {
        // logging error
        write_drop(logFile, user, index, NOTFOUND);
        // sending course not found error
        send_error(user->socket_fd, ECNOTFOUND);
        return;
    }

    // course is valid
    int drop_result;
    drop_result = drop_user(user, index);
#ifdef DEBUG
    course_print();
#endif
    if (drop_result == 0)
    {
        // logging error
        write_drop(logFile, user, index, NEGATIVE);
        // course already full
        // return error
        if (send_error(user->socket_fd, ECDENIED) == 0)
        {
            fprintf(stderr, "senderror");
        };
    }

    else
    {
        // logging success
        write_drop(logFile, user, index, POSITIVE);
        // respond ok to client
        if (send_message(user->socket_fd, OK, "") == 0)
        {
            fprintf(stderr, "senderror");
        };
        update_stats("drops", 1);
        if (drop_result == 2)
        {
            // we do wait add
            waitadd(index);
        }
    }

#ifdef DEBUG
    // DeleteList(&userList);
    // course_print();

#endif
}