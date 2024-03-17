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

/**
 * @brief locks required
 * dynamcally allocate a string to
 * hold all clist information
 * malloc 1
 */
char *clist_print();

//  @brief assume course index is valid
//  * if course is not full then fail return 0
//  * if user is already enrolled or waitlisted then return 2
//  * if succeed then return 1
//  * course index lock
//  * write lock for user
//  * @param user
//  * @param course_index
//  * @return int
int wait_user(user_t *user, int course_index);

/**
 * @brief remove a user from course
 *
 * @param course_index
 * @param enrollment integer specifying enroll or waitlist
 */
void rm_user(user_t *user, int course_index, int enrollment);
//  @brief assume course index is valid
//  * if user not enrolled in course retunr 0
//  * if succeed , no wait add then return 1
//  * with wait add return 2
//  * user read lock
//  * course index lock
//  * user write lock
//  * @param user
//  * @param course_index
//  * @return int
int drop_user(user_t *user, int course_index);
/**
 * @brief remove a user from course
 *
 * @param course_index
 */
user_t *rm_user_with_lock(int course_index);

#endif