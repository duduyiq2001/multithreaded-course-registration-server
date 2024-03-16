
#ifndef MULTIPLEX
#define MULTIPLEX
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
#define BUFFER_SIZE 1024
#define SA struct sockaddr

/**
 * @brief use select to wait for client fd to be ready
 *
 * would block if not
 * @param client_fd
 */
int wait_for_client(int client_fd);

#endif