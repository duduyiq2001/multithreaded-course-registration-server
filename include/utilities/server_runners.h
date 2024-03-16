#ifndef SERVER_RUNNER
#define SERVER_RUNNER

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
#include "../server.h"
#define BUFFER_SIZE 1024
#define SA struct sockaddr
/**
 * @brief init function for the server to run on certain port
 *
 * @param server_port
 * @return int
 */
int server_init(int server_port);

/**
 * @brief process each client
 *
 * @param client the user_t object
 * @return void*
 */
void *process_client(void *user);

/**
 * @brief runner function for server
 *
 * @param server_port
 */
void run_server(int server_port);

#endif