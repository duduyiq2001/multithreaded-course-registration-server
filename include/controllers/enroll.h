#ifndef ENROLL_H
#define ENROLL_H
#include "../server.h"
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
void enroll(user_t *user, char *course_index);

#endif