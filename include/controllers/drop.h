#ifndef DROP_H
#define DROP_H
#include "../server.h"
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
void drop(user_t *user, char *course_index);

#endif