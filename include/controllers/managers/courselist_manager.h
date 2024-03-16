#ifndef COURSE_MAN
#define COURSE_MAN
#include "../../server.h"

/**
 * @brief assume course index is valid
 * if course is full then fail return 0
 * if user is already added then return 2
 * if succeed then return 1
 * @param user
 * @param course_index
 * @return int
 */
int add_user(user_t *user, int course_index);

/**
 * @brief no locks needed
 *
 */
void course_print();

#endif