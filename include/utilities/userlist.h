#ifndef USERLIST
#define USERLIST

#include "../server.h"

/**
 * @brief comparator function for user
 *
 * @param user1 user1 data
 * @param user2 user2 data
 * @return int
 */
int userComparator(const void *user1, const void *user2);

/**
 * @brief printer function for user
 *
 * @param user user data
 * @param fp
 */
void userPrinter(void *user, void *fp);

/**
 * @brief deleter function for user
 *
 * @param user user data
 */
void userDeleter(void *user);
/**
 * @brief 2 ** power
 *
 * @param degree
 */
uint32_t twotopower(int degree);
/**
 * @brief update bit vector for enroll
 *
 * @param user
 */
void update_enroll(user_t *user, int course_index);

/**
 * @brief printer for userlist in course
 *
 * @param courseindex which course
 *
 * @param type enroll/ waitlist
 * @param delimiter SEMICOLON/ARROW
 */
void userPrinter1(int courseindex, char *type, int delimiter);
/**
 * @brief update bit vector for waitlist
 *
 * @param user
 */
void update_wait(user_t *user, int course_index);
/**
 * @brief printer for userlist in course
 *
 * @param courseindex which course
 *
 * @param type enroll/ waitlist
 * @param delimiter SEMICOLON/ARROW
 * @param output string to output to, need to alloc before
 */
void SuserPrinter1(int courseindex, char *type, int delimiter, char *output);

/**
 * @brief update user enrollment
 * removing that class from user
 *
 * @param user
 * @param course_index
 */
void rm_update_enroll(user_t *user, int course_index);

/**
 * @brief update user waitlist
 * removing that class from user
 *
 * @param user
 * @param course_index
 */
void rm_update_wait(user_t *user, int course_index);

#endif