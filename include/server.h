#ifndef SERVER_H
#define SERVER_H

#include <arpa/inet.h>
#include <getopt.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <semaphore.h>
#include <signal.h>
#include "./utilities/linkedlist.h"

#define BUFFER_SIZE 1024
#define SA struct sockaddr

#define USAGE_MSG "./bin/zotReg_server [-h] PORT_NUMBER COURSE_FILENAME LOG_FILENAME"                      \
                  "\n  -h                 Displays this help menu and returns EXIT_SUCCESS."               \
                  "\n  PORT_NUMBER        Port number to listen on."                                       \
                  "\n  COURSE_FILENAME    File to read course information from at the start of the server" \
                  "\n  LOG_FILENAME       File to output server actions into. Create/overwrite, if exists\n"

typedef struct
{
    int clientCnt;
    int threadCnt;
    int totalAdds;
    int totalDrops;
} stats_t;

extern stats_t curStats;

typedef struct
{
    char *username;
    int socket_fd;
    pthread_t tid;
    uint32_t enrolled;
    uint32_t waitlisted;
} user_t;

typedef struct
{
    char *title;
    int maxCap;
    list_t enrollment;
    list_t waitlist;
} course_t;

extern course_t courseArray[32];

extern list_t userList;

// single lock for log file
extern pthread_mutex_t logfile_lock;

// mutex suite for userlist
extern pthread_mutex_t userlist_access;
extern pthread_mutex_t userlist_readcount_access;
extern int userlist_readers;

// mutex suite for stats

extern pthread_mutex_t stats_access;

// mutex suite for courses
extern pthread_mutex_t course_access[32];

// logfile

extern FILE *logFile;

// num of course;

extern int num_of_courses;

// conditional flag for exiting process!

extern volatile sig_atomic_t sigint_flag;
#endif
