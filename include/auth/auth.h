
#ifndef AUTHEN
#define AUTHEN
#include <getopt.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <server.h>

#define BUFFER_SIZE 1024
#define SA struct sockaddr

/**
 * @brief authenticate the client by WAITing on
 * login request
 * if success return user
 * if error return NULL
 * note: userlist update will be afterwards
 * @param clientfd_ptr
 * @return
 */
user_t *authenticate(void *clientfd_ptr, char **username);
/**
 * @brief update user_t when logout
 *
 * @param user
 */
void logout(user_t *user);

#endif