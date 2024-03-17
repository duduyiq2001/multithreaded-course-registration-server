#include "controllers/managers/courselist_manager.h"
#include "controllers/managers/stats_manager.h"
#include "controllers/managers/logfile_manager.h"
/**
 * @brief take one from waitlist and add to enrollment
 *
 * @param course_index
 */
void waitadd(int course_index)
{
    // remove the first user from waitlist
    user_t *user = rm_user_with_lock(course_index);
    if (user == NULL)
    {
        fprintf(stderr, "error, waitlist empty");
        return;
    }
    // add user to enrollment
    add_user(user, course_index);
    // here we assume no fail
    // update total add
    update_stats("adds", 1);
    // log wait add
    write_waitadd(logFile, user, course_index);
    return;
}