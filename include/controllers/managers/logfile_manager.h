#ifndef LOG_MAN
#define LOG_MAN
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include "../../server.h"

/**
 * @brief writing log specifying log type
 *
 * @param logfile
 * @param logtype
 * @param username
 */
void write_log(FILE *logfile, char *logtype, char *username);
/**
 * @brief write log for wait
 *
 * @param logfile
 * @param user
 *
 */
void write_wait(FILE *logfile, user_t *user, int course_index, int positive);

/**
 * @brief write log for enroll
 *
 * @param logfile
 * @param user
 *
 */
void write_enroll(FILE *logfile, user_t *user, int course_index, int positive);

/**
 * @brief write log for drop
 *
 * @param logfile
 * @param user
 *
 */
void write_drop(FILE *logfile, user_t *user, int course_index, int positive);

/**
 * @brief write log for wait add
 *
 * @param logfile
 * @param user
 * @param course_index
 */
void write_waitadd(FILE *logfile, user_t *user, int course_index);
#endif