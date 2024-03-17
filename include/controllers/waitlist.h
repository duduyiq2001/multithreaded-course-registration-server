#ifndef WAITLIST_H
#define WAITLIST
#include "../server.h"
/**
 * @brief
 * first atoi the course index
 * send back error if index is non valid 
 * waitlist if the user is not already waitlisted or enrolled
 * @param user 
 * @param course_index 
 */
void waitlist(user_t *user, char* course_index);
#endif