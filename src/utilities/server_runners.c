
#include "utilities/server_runners.h"
#include "controllers/managers/stats_manager.h"
#include "auth/auth.h"
#include <pthread.h>
#include <signal.h>
#include <errno.h>
#include "protocol/rmessage.h"
#include "protocol/wmessage.h"
#include "utilities/multiplex.h"
#include "routing/router.h"
#include "controllers/managers/courselist_manager.h"
#include "controllers/managers/userlist_manager.h"
#include "debug.h"
/**
 * @brief pthread join on all threads
 *
 */
void wait_threads()
{
    node_t *current = userList.head;
    for (int i = 0; i < userList.length; i++)
    {
        pthread_t tid = ((user_t *)current->data)->tid;
        void *res;
        // making sure tid is valid
        if (tid != -1 && tid != 0)
        {
            // Send SIGINT to the specific thread
            pthread_kill(tid, SIGINT);
            if (pthread_join(tid, &res) != 0)
            {
                fprintf(stderr, "Failed to join thread %ld", tid);
                return;
            }
        }
    }
    return;
}

/**
 * @brief init function for the server to run on certain port
 *
 * @param server_port
 * @return int
 */
int server_init(int server_port)
{
    int sockfd;
    struct sockaddr_in servaddr;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("socket creation failed...\n");
        exit(EXIT_FAILURE);
    }
    else
        printf("Socket successfully created\n");

    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(server_port);

    int opt = 1;

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, (char *)&opt, sizeof(opt)) < 0)
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA *)&servaddr, sizeof(servaddr))) != 0)
    {
        printf("socket bind failed\n");
        exit(EXIT_FAILURE);
    }
    else
        printf("Socket successfully binded\n");

    // Now server is ready to listen and verification
    if ((listen(sockfd, 1)) != 0)
    {
        printf("Listen failed\n");
        exit(EXIT_FAILURE);
    }
    else
        printf("Server listening on port: %d.. Waiting for connection\n", server_port);

    return sockfd;
}

/**
 * @brief process each client
 * update tid
 * wait in a loop for user input
 * @param client user object for client
 * @return void*
 */
void *process_client(void *user)
{

    // deference
    user_t *client = (user_t *)user;
    // update thread tid
    client->tid = pthread_self();
    enum msg_types m_type;
    char *message = NULL;

    // read from client
    while (1)
    {
        int waitresult;
        /**
         * @brief waiting for client to be ready
         * block if not
         */
        waitresult = wait_for_client(client->socket_fd);
        if (waitresult == 0)
        {
            // wait error
            send_error(client->socket_fd, ESERV);
            // detach thread
            pthread_detach(pthread_self());
            return NULL;
        }
        // siguint occurs
        if (waitresult == 2)
        {
#ifdef DEBUG
            printf("client thread ends");
            fflush(stdout);
#endif
            // close connection
            close(client->socket_fd);
            return NULL;
        }
        int readresult;
        /**
         * @brief read  message from client
         *
         */
        readresult = read_client(client->socket_fd, &m_type, &message);
        if (readresult == 0)
        {
            fprintf(stderr, "msg read error");
            free(message);
            send_error(client->socket_fd, ESERV);
            // detach thread
            pthread_detach(pthread_self());
            // read error
            return NULL;
        }
        // sigint happens
        if (readresult == 2)
        {
            // close connection
            close(client->socket_fd);
            free(message);
            // read error
            return NULL;
        }

        /**
         * @brief routing the request
         *
         */
        int iflogout = route(client, &m_type, message);
        // clear message
        if (message != NULL)
        {
            free(message);
            message = NULL;
        }
        // checking the flag
        if (sigint_flag)
        {
            // close client connection
            close(client->socket_fd);
            // exiting thread
            return NULL;
        }
        // check if user logs out
        if (iflogout)
        {
            // then detach
            pthread_detach(pthread_self());
            return NULL;
        }
    }

    return NULL;
}

/**
 * @brief runner function for server
 * listen to client connections in a loop
 * if a connection is accepted, wait for a login request
 * authenticate the login request
 * if successful, send ok
 *      update active userlist
 * ir error, send EUSRLOGIN
 * @param server_port
 */
void run_server(int server_port)
{
    int listen_fd = server_init(server_port); // Initiate server and start listening on specified port
    // initialize resources
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    // buffer to hold client name
    char *name_buffer = NULL;

    pthread_t tid;

    while (1)
    {
        //         // check conditional flag for exiting
        //         if (sigint_flag)
        //         {
        //             // wait for all threads to terminate
        //             wait_threads();
        //             // close connection
        //             close(listen_fd);
        //             // PRINT AND EXIT
        //             course_print();
        //             userlist_print();
        //             print_stats();
        //             fflush(stderr);
        // #ifdef DEBUG
        //             fprintf(stderr, "print finished");
        // #endif
        //             // free all resources
        //             DeleteList(&userList);
        //             for (int i = 0; i < num_of_courses; i++)
        //             {
        //                 DeleteList(&(courseArray[i].enrollment));
        //                 DeleteList(&(courseArray[i].waitlist));
        //             }
        //             return;
        //         }
        // Wait and Accept the connection from client
        printf("Wait for new client connection\n");
        int *client_fd = malloc(sizeof(int));
        *client_fd = accept(listen_fd, (SA *)&client_addr, &client_addr_len);
        // check conditional flag for exiting
        if (sigint_flag)
        {
            // wait for all threads to terminate
            wait_threads();
            // close connection
            close(listen_fd);
            // PRINT AND EXIT
            course_print();
            userlist_print();
            print_stats();

            fflush(stderr);

            // free all resources
            free(client_fd);
            DeleteList(&userList);

            for (int i = 0; i < num_of_courses; i++)
            {
                DeleteList1(&(courseArray[i].enrollment));

                DeleteList1(&(courseArray[i].waitlist));
            }
            return;
        }
        if (*client_fd < 0)
        {
            printf("server acccept failed\n");
            exit(EXIT_FAILURE);
        }
        else
        {
            printf("Client connetion accepted\n");
            // update client count
            update_stats("client", 1);
            /**
             * @brief authenticate client
             *
             */
            user_t *new_user = authenticate(client_fd, &name_buffer);

            // update thread count
            update_stats("thread", 1);

            // spawn connection if login successfull
            if (new_user != NULL)
            {
                pthread_create(&tid, NULL, process_client, (void *)new_user);
            }
        }
    }
    close(listen_fd);
    return;
}
