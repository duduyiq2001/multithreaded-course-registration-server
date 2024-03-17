#include "server.h"
#include "protocol.h"
#include "utilities/userlist.h"
#include "parsers/course_parser.h"
#include "utilities/server_runners.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

stats_t curStats;
course_t courseArray[32];

list_t userList;

// single lock for log file
pthread_mutex_t logfile_lock;

// mutex suite for userlist
pthread_mutex_t userlist_access;
pthread_mutex_t userlist_readcount_access;
int userlist_readers;

// mutex suite for stats

pthread_mutex_t stats_access;

// mutex suite for courses
pthread_mutex_t course_access[32];

// logfile

FILE *logFile;

// num of course;

int num_of_courses;

// conditional flag for exiting process!

volatile sig_atomic_t sigint_flag;
void sigint_handler(int sig)
{

    int olderrno = errno;
    sigint_flag = 1;
    errno = olderrno;
}
void install_handler()
{
    struct sigaction myaction = {{0}};

    myaction.sa_handler = sigint_handler;

    if (sigaction(SIGINT, &myaction, NULL) == -1)
    {
        printf("signal handler failed to install\n");
    }
}
int main(int argc, char *argv[])
{
    int opt;
    while ((opt = getopt(argc, argv, "h")) != -1)
    {
        switch (opt)
        {
        case 'h':
            fprintf(stderr, USAGE_MSG);
            exit(EXIT_FAILURE);
        }
    }

    // 3 positional arguments necessary
    if (argc != 4)
    {
        fprintf(stderr, USAGE_MSG);
        exit(EXIT_FAILURE);
    }
    unsigned int port_number = atoi(argv[1]);
    char *poll_filename = argv[2];
    char *log_filename = argv[3];

    // INSERT CODE HERE

    /**
     * @brief initialize resources:
     * 1. userlist ,course_list, number of courses, log file, stats
     *
     *
     *
     *
     */

    // initialize user list
    userList.head = NULL;
    userList.length = 0;
    userList.comparator = &userComparator;
    userList.deleter = &userDeleter;
    userList.printer = &userPrinter;

    // course array already initialized
    // initialize the num of courses to -1
    num_of_courses = -1;

    // open poll file, log file
    FILE *pollFile = fopen(poll_filename, "r");

    // checking if the file is opened successfully
    if (pollFile == NULL)
    {

        // errno = ESERV;
        fprintf(stderr, "poll file null");
        exit(1);
    }

    logFile = fopen(log_filename, "w");

    // checking if the file is opened successfully
    if (logFile == NULL)
    {

        // errno = ESERV;
        fprintf(stderr, "poll file null");
        exit(1);
    }

    // initializing curStats

    curStats.clientCnt = 0;
    curStats.threadCnt = 0;
    curStats.totalAdds = 0;
    curStats.totalDrops = 0;

    /**
     * @brief Parsing the poll file to get the list of courses
     *
     *
     *
     *
     */
    num_of_courses = course_parser(courseArray, pollFile);
    fclose(pollFile);
    // if error then we exit
    if (num_of_courses == -1)
    {
        fprintf(stderr, "poll file invalid");
        exit(1);
    }

    /**
     * @brief before runnning server we initialize all locks
     *
     */

    pthread_mutex_init(&logfile_lock, NULL);
    pthread_mutex_init(&stats_access, NULL);
    pthread_mutex_init(&userlist_access, NULL);
    pthread_mutex_init(&userlist_readcount_access, NULL);
    for (int i = 0; i < 32; i++)
    {
        pthread_mutex_init(&course_access[i], NULL);
    }
    // initialize reader count to 0
    userlist_readers = 0;

    // initialize sig int flag
    sigint_flag = 0;
    // install handler
    // Prepare the signal set
    sigset_t set;
    sigfillset(&set);        // Initialize set to full, including all signals
    sigdelset(&set, SIGINT); // Remove SIGINT from the set, allowing it to be delivered

    // Block all signals except SIGINT
    if (sigprocmask(SIG_SETMASK, &set, NULL) < 0)
    {
        fprintf(stderr, "set mask eror");
        return 1;
    }
    install_handler();
    // signal(SIGINT, sigint_handler);

    // declare server initialized
    printf("Server initialized with %d courses.\n", num_of_courses);
    printf("Currently listening on port %d.\n", port_number);

    /**
     * @brief starting runner server
     *
     */
    run_server((int)port_number);

    return 0;
}
