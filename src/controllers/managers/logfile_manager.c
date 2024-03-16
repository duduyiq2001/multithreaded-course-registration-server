#include "controllers/managers/logfile_manager.h"
/**
 * @brief writing log specifying log type
 *
 * @param logfile
 * @param logtype
 * @param username
 */
void write_log(FILE *logfile, char *logtype, char *username)
{

    if (strcmp(logtype, "REJECT") == 0 || strcmp(logtype, "RECONNECTED") == 0 || strcmp(logtype, "CONNECTED") == 0)
    {
        // lock file
        pthread_mutex_lock(&logfile_lock);
        fprintf(logfile, "%s %s\n", logtype, username);
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
void write_wait(FILE *logfile, user_t *user)
{
    return;
}