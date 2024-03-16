#ifndef ULIST_MAN
#define ULIST_MAN
#include "../../utilities/linkedlist.h"
#include "../../server.h"
#include <string.h>
#include <pthread.h>
/**
 * @brief find a user from the userlist
 *  return NULL if not found
 *  return user if found
 * @param username
 * @return user_t*
 */
user_t *find_user(char *username);

/**
 * @brief insert user into userlist
 *
 * @param username
 * @return newly inserted user
 *
 */
user_t *insert_user(char *username, int socketfd);

/**
 * @brief delete user from list
 *
 * @param username
 * @return int
 * 0 if fail
 * 1 success
 */
int delete_user(char *username);

/**
 * @brief logging in a user by setting the socket_fd from -1
 *  return NULL if not found
 * @param userame
 * @param socketfd
 * @return user_t*
 */
user_t *login_user(char *username, int socketfd);

/**
 * @brief no lock needed
 *
 */
void userlist_print();

#endif