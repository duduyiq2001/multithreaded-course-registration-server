#include "controllers/managers/logfile_manager.h"
/**
 * @brief writing log specifying log type
 * used string to specify instead enum or macros
 * if I have more time I will change that
 * @param logfile
 * @param logtype
 * @param username
 */
void write_log(FILE *logfile, char *logtype, char *username)
{
    // username first
    if (strcmp(logtype, "REJECT") == 0 || strcmp(logtype, "RECONNECTED") == 0 || strcmp(logtype, "CONNECTED") == 0)
    {
        // lock file
        pthread_mutex_lock(&logfile_lock);
        fprintf(logfile, "%s %s\n", logtype, username);
        fflush(logFile);
        // unlock
        pthread_mutex_unlock(&logfile_lock);
    }
    // event type first
    if (strcmp(logtype, "LOGOUT") == 0 || strcmp(logtype, "CLIST") == 0 || strcmp(logtype, "NOSCHED") == 0 || strcmp(logtype, "SCHED") == 0)
    {
        // lock file
        pthread_mutex_lock(&logfile_lock);
        fprintf(logfile, "%s %s\n", username, logtype);
        fflush(logFile);
        // unlock
        pthread_mutex_unlock(&logfile_lock);
    }
    return;
}
/**
 * @brief write log for wait
 *
 * @param logfile
 * @param user
 */
void write_wait(FILE *logfile, user_t *user, int course_index, int positive)
{
    if (positive == 1)
    {
        fprintf(logFile, "%s WAIT %d %d\n", user->username, course_index, user->waitlisted);
        return;
    }
    if (positive == 0)
    {
        fprintf(logFile, "%s NOWAIT %d\n", user->username, course_index);
        return;
    }
    fprintf(logFile, "%s NOTFOUND_W %d\n", user->username, course_index);
    return;
}

/**
 * @brief write log for enroll
 *
 * @param logfile
 * @param user
 *
 */
void write_enroll(FILE *logfile, user_t *user, int course_index, int positive)
{
    if (positive == 1)
    {
        fprintf(logFile, "%s ENROLL %d %d\n", user->username, course_index, user->enrolled);
        return;
    }
    if (positive == 0)
    {
        fprintf(logFile, "%s NOENROLL %d\n", user->username, course_index);
        return;
    }
    fprintf(logFile, "%s NOTFOUND_E %d\n", user->username, course_index);

    return;
}

/**
 * @brief write log for drop
 *
 * @param logfile
 * @param user
 *
 */
void write_drop(FILE *logfile, user_t *user, int course_index, int positive)
{
    if (positive == 1)
    {
        fprintf(logFile, "%s DROP %d %d\n", user->username, course_index, user->enrolled);
        return;
    }
    if (positive == 0)
    {
        fprintf(logFile, "%s NODROP %d\n", user->username, course_index);
        return;
    }
    fprintf(logFile, "%s NOTFOUND_D %d\n", user->username, course_index);

    return;
}

/**
 * @brief write log for wait add
 *
 * @param logfile
 * @param user
 * @param course_index
 */
void write_waitadd(FILE *logfile, user_t *user, int course_index)
{
    fprintf(logFile, "%s WAITADD %d %d\n", user->username, course_index, user->enrolled);
}