#include "controllers/managers/userlist_manager.h"
#include "utilities/userlist.h"

/**
 * @brief find a user from the userlist
 *  return NULL if not found
 *  return user if found
 *  READER function
 * @param username
 * @return user_t*
 */
user_t *find_user(char *username)
{
    // acquiring reader lock
    // Reader wants to enter
    pthread_mutex_lock(&userlist_readcount_access);
    // increment reader count
    userlist_readers++;
    // if it's the first reader
    if (userlist_readers == 1)
    {
        pthread_mutex_lock(&userlist_access); // First reader locks the resource
    }
    // release count lock
    pthread_mutex_unlock(&userlist_readcount_access);

    /**
     * @brief reading begins
     *
     */
    node_t *current = userList.head;
    for (int i = 0; i < userList.length; i++)
    {
        if (current != NULL && current->data != NULL && strcmp(((user_t *)current->data)->username, username) == 0)
        {
            // reading ends
            // update read count or unlock resource
            pthread_mutex_lock(&userlist_readcount_access);
            // decrement users
            userlist_readers--;
            if (userlist_readers == 0)
            {
                pthread_mutex_unlock(&userlist_access); // Last reader unlocks the resource
            }
            pthread_mutex_unlock(&userlist_readcount_access);
            return (user_t *)current->data;
        }
        current = current->next;
    }

    // reading ends
    // update read count or unlock resource
    pthread_mutex_lock(&userlist_readcount_access);
    // decrement users
    userlist_readers--;
    if (userlist_readers == 0)
    {
        pthread_mutex_unlock(&userlist_access); // Last reader unlocks the resource
    }
    pthread_mutex_unlock(&userlist_readcount_access);
    return NULL;
}

/**
 * @brief insert user into userlist
 * writer function
 * @param username
 * @return 0 if fail
 * 1 success
 * 1 malloc
 */
user_t *insert_user(char *username, int socketfd)
{
    // malloc a user
    user_t *new_user = malloc(sizeof(user_t));
    // initialize user resource
    new_user->tid = 0;
    new_user->username = username;
    new_user->enrolled = 0;
    new_user->waitlisted = 0;
    new_user->socket_fd = socketfd;

    // acquire lock
    // Writer wants to enter
    pthread_mutex_lock(&userlist_access); // Lock the resource

    // insert into list
    InsertInOrder(&userList, new_user);
    // release lock

    pthread_mutex_unlock(&userlist_access);
    return new_user;
}

/**
 * @brief delete user from list
 *
 * @param username
 * @return int
 * 0 if fail
 * 1 success
 * writer function
 */
int delete_user(char *username)
{
    // acquiring writer lock
    // Writer wants to enter
    pthread_mutex_lock(&userlist_access); // Lock the resource

    /**
     * @brief begin writing
     *
     */
    node_t *current = userList.head;
    node_t *prev = NULL;
    for (int i = 0; i < userList.length; i++)
    {
        if (current != NULL && current->data != NULL && strcmp(((user_t *)current->data)->username, username) == 0)
        {
            // free internal resource
            (*userList.deleter)(current->data);
            if (prev != NULL)
            {
                // reconnect nodes
                prev->next = current->next;
            }
            free(current);
            // writing ends
            // release lock

            pthread_mutex_unlock(&userlist_access);
            return 1;
        }
        // update current and prev
        current = current->next;
        prev = current;
    }
    // writing ends
    // release lock

    pthread_mutex_unlock(&userlist_access);
    return 0;
}

/**
 * @brief logging in a user by setting the socket_fd from -1
 *  return NULL if not found
 *  writer function
 * @param userame
 * @param socketfd
 * @return user_t*
 */
user_t *login_user(char *username, int socketfd)
{
    // acquiring writer lock
    // Writer wants to enter
    pthread_mutex_lock(&userlist_access); // Lock the resource

    node_t *current = userList.head;
    for (int i = 0; i < userList.length; i++)
    {
        if (current != NULL && current->data != NULL && strcmp(((user_t *)current->data)->username, username) == 0)
        {
            // resetting socketfd
            ((user_t *)current->data)->socket_fd = socketfd;

            // writing ends
            // release lock

            pthread_mutex_unlock(&userlist_access);
            return (user_t *)current->data;
        }
        current = current->next;
    }
    // writing ends
    // release lock

    pthread_mutex_unlock(&userlist_access);
    return NULL;
}

/**
 * @brief no lock needed
 *
 */
void userlist_print()
{
    PrintLinkedList(&userList, stderr);
}

/**
 * @brief build study list strings
 * allocate string inside
 * need free outside
 * user reader lock required
 * malloc 1
 * return NULL if no enroll
 * @param user
 * @return char*
 */
char *GetStudyList(user_t *user)
{
    char *response = (char *)malloc(sizeof(char) * BUFFER_SIZE * 3);
    // initialize with empty
    response[0] = '\0';
    for (int i = 0; i < num_of_courses; i++)
    {
        char buffer[BUFFER_SIZE];
        /**
         * @brief grab reader lock
         *
         */
        // Reader wants to enter
        pthread_mutex_lock(&userlist_readcount_access);
        // increment reader count
        userlist_readers++;
        // if it's the first reader
        if (userlist_readers == 1)
        {
            pthread_mutex_lock(&userlist_access); // First reader locks the resource
        }
        // release count lock
        pthread_mutex_unlock(&userlist_readcount_access);
        /**
         *
         *
         */
        if ((user->enrolled & twotopower(i)) != 0)
        {
            sprintf(buffer, "Course %d - %s\n", i, courseArray[i].title);
            // add to response
            strcat(response, buffer);
        }
        if ((user->waitlisted & twotopower(i)) != 0)
        {
            sprintf(buffer, "Course %d - %s (WAITING)\n", i, courseArray[i].title);
            strcat(response, buffer);
        }
        /**
         * @brief releasing reader lock
         *
         */
        // reading ends
        // update read count or unlock resource
        pthread_mutex_lock(&userlist_readcount_access);
        // decrement users
        userlist_readers--;
        if (userlist_readers == 0)
        {
            pthread_mutex_unlock(&userlist_access); // Last reader unlocks the resource
        }
        pthread_mutex_unlock(&userlist_readcount_access);
        /**
         *
         *
         */
    }
    // if not enrolled or waited in any
    if (strcmp(response, "") == 0)
    {
        free(response);
        return NULL;
    }
    // cpy to actual return string
    char *res = (char *)malloc(sizeof(char) * strlen(response) + sizeof(char));
    strcpy(res, response);
    // get rid of the response buffer
    free(response);
    return res;
}