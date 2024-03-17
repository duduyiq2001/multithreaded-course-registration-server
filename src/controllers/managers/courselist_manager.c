#include "controllers/managers/courselist_manager.h"
#include "utilities/userlist.h"
#include "controllers/managers/logfile_manager.h"
#include <pthread.h>
/**
 * @brief
 * obtain course index lock
 * assume course index is valid
 * writer for userlist
 * if course is full then fail return 0
 * if user is already added then return 2
 * if succeed then return 1
 * @param user
 * @param course_index
 * @return int
 */
int add_user(user_t *user, int course_index)
{
    // grab lock
    pthread_mutex_lock(&course_access[course_index]);
    // check if course is full
    if (courseArray[course_index].enrollment.length >= courseArray[course_index].maxCap)
    {
        // release lock
        pthread_mutex_unlock(&course_access[course_index]);
        return 0;
    }
    // spots available
    node_t *current = courseArray[course_index].enrollment.head;
    for (int i = 0; i < courseArray[course_index].enrollment.length; i++)
    {
        if (current != NULL && current->data != NULL && strcmp(((user_t *)current->data)->username, user->username) == 0)
        {
            // release lock
            pthread_mutex_unlock(&course_access[course_index]);
            return 2;
        }

        current = current->next;
    }
    // insert new user
    InsertInOrder(&(courseArray[course_index]).enrollment, user);
#ifdef DEBUG
    course_print();
#endif
    // release lock
    pthread_mutex_unlock(&course_access[course_index]);

    // update bit array
    // acquire write lock for user
    pthread_mutex_lock(&userlist_access);

    update_enroll(user, course_index);
    // release write lock for user
    pthread_mutex_unlock(&userlist_access);
#ifdef DEBUG
    course_print();
#endif
    return 1;
}

/**
 * @brief no locks needed
 *
 */
// title, maxCap, enrolledCnt, <enrolled usernames>, <waitlist usernames>\n
void course_print()
{
    for (int i = 0; i < num_of_courses; i++)
    {
        fprintf(stdout, "%s, %d, %d, ", courseArray[i].title, courseArray[i].maxCap, courseArray[i].enrollment.length);
        userPrinter1(i, "enroll", SEMICOLON);
        fprintf(stdout, ", ");
        userPrinter1(i, "waitlist", SEMICOLON);
        fflush(stdout);
        fprintf(stdout, "\n");
    }
}
// before log out detach the thread.

/**
 * @brief locks required
 * dynamcally allocate a string to
 * hold all clist information
 * malloc 1
 *
 */
char *clist_print()
{
    int length = 0;
    // first malloc two times of buffer size
    char *response = (char *)malloc(sizeof(char) * BUFFER_SIZE * 2 + sizeof(char));
    response[0] = '\0';

    for (int i = 0; i < num_of_courses; i++)
    {
        char buffer[BUFFER_SIZE];
        /**
         * @brief acquire lock
         *
         */
        pthread_mutex_lock(&course_access[i]);
        sprintf(buffer, "Course %d - %s", i, courseArray[i].title);
        // if closed
        if (courseArray[i].enrollment.length >= courseArray[i].maxCap)
        {
            strcat(buffer, " (CLOSED)");
        }
        /**
         * @brief release lock
         *
         */
        pthread_mutex_unlock(&course_access[i]);

        // add new line
        strcat(buffer, "\n");
        // calc length
        length += strlen(buffer);
        // if length exceed cap
        // else concat
        strcat(response, buffer);
    }
    // malloc a string that's exact length
    char *res = (char *)malloc(sizeof(char) * strlen(response) + sizeof(char));
    strcpy(res, response);
    // free response
    free(response);
    return res;
}

//  @brief assume course index is valid
//  * if course is not full then fail return 0
//  * if user is already enrolled or waitlisted then return 2
//  * if succeed then return 1
//   writer for userlist
//  * @param user
//  * @param course_index
//  * @return int
int wait_user(user_t *user, int course_index)
{
    // grab lock
    pthread_mutex_lock(&course_access[course_index]);
    // check if course is not even full
    if (courseArray[course_index].enrollment.length < courseArray[course_index].maxCap)
    {
        // release lock
        pthread_mutex_unlock(&course_access[course_index]);
        return 0;
    }
    // if already in waitlist
    node_t *current_wait = courseArray[course_index].waitlist.head;
    for (int i = 0; i < courseArray[course_index].waitlist.length; i++)
    {
        if (current_wait != NULL && current_wait->data != NULL && strcmp(((user_t *)current_wait->data)->username, user->username) == 0)
        {
            // release lock
            pthread_mutex_unlock(&course_access[course_index]);
            return 2;
        }

        current_wait = current_wait->next;
    }
    // if already enrolled
    node_t *current = courseArray[course_index].enrollment.head;
    for (int i = 0; i < courseArray[course_index].enrollment.length; i++)
    {
        if (current != NULL && current->data != NULL && strcmp(((user_t *)current->data)->username, user->username) == 0)
        {
            // release lock
            pthread_mutex_unlock(&course_access[course_index]);
            return 2;
        }

        current = current->next;
    }

    // insert new user
    InsertAtTail(&(courseArray[course_index]).waitlist, user);
#ifdef DEBUG
    course_print();
#endif
    // release lock
    pthread_mutex_unlock(&course_access[course_index]);

    // update bit array
    // acquire write lock for user
    pthread_mutex_lock(&userlist_access);

    update_wait(user, course_index);
    // release write lock for user
    pthread_mutex_unlock(&userlist_access);
#ifdef DEBUG
    course_print();
#endif
    return 1;
}

/**
 * @brief remove a user from course
 *
 * @param course_index
 * @param enrollment integer specifying enroll or waitlist
 */
void rm_user(user_t *user, int course_index, int enrollment)
{
    node_t *current;
    int length;
    if (enrollment == ENROLLMENT_LIST)
    {
        current = courseArray[course_index].enrollment.head;
        length = courseArray[course_index].enrollment.length;
    }
    else
    {
        current = courseArray[course_index].waitlist.head;
        length = courseArray[course_index].waitlist.length;
    }
    node_t *prev = NULL;
    for (int i = 0; i < length; i++)
    {
        if (current != NULL && current->data != NULL && (user_t *)current->data == user)
        {

            if (prev != NULL)
            {
                // reconnect nodes
                prev->next = current->next;
                // decrement count
                if (enrollment == ENROLLMENT_LIST)
                {

                    courseArray[course_index].enrollment.length -= 1;
                }
                else
                {
                    courseArray[course_index].waitlist.head = NULL;
                    courseArray[course_index].waitlist.length -= 1;
                }
            }
            else
            {
                // set head to null
                if (enrollment == ENROLLMENT_LIST)
                {
                    courseArray[course_index].enrollment.head = NULL;
                    courseArray[course_index].enrollment.length = 0;
                }
                else
                {
                    courseArray[course_index].waitlist.head = NULL;
                    courseArray[course_index].waitlist.length = 0;
                }
            }
            return;
        }
        // update current and prev
        current = current->next;
        prev = current;
    }

    return;
}

//  @brief assume course index is valid
//  * if user not enrolled in course retunr 0
//  * if succeed , no wait add then return 1
//  * with wait add return 2
//  * user read lock
//  * course index lock
//  * user write lock
//  * @param user
//  * @param course_index
//  * @return int
int drop_user(user_t *user, int course_index)
{
    int ifwaitadd = 0;
    // if not enrolled
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
    if ((user->enrolled & twotopower(course_index)) == 0)
    {
        // not even enrolled
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
        return 0;
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
    // grab course index lock
    pthread_mutex_lock(&course_access[course_index]);
    // remove user
    rm_user(user, course_index, ENROLLMENT_LIST);

    if (courseArray[course_index].waitlist.length > 0)
    {
        // setting wait add to true
        ifwaitadd = 1;
    }
    // grab course index lock
    pthread_mutex_unlock(&course_access[course_index]);

    // update bit array

    // acquire write lock for user
    pthread_mutex_lock(&userlist_access);

    rm_update_enroll(user, course_index);
    // release write lock for user
    pthread_mutex_unlock(&userlist_access);

    if (ifwaitadd)
    {
        // calling wait add controller
        return 2;
    }
#ifdef DEBUG
    course_print();
#endif
    return 1;
}

/**
 * @brief remove a user from course
 *
 * @param course_index
 * @param enrollment integer specifying enroll or waitlist
 */
user_t *rm_user_with_lock(int course_index)
{
    user_t *user = NULL;
    // grab course index block
    pthread_mutex_lock(&course_access[course_index]);
    // pop head
    if (courseArray[course_index].waitlist.length > 0)
    {
        user = ((user_t *)((node_t *)courseArray[course_index].waitlist.head)->data);
        courseArray[course_index].waitlist.head = courseArray[course_index].waitlist.head->next;
        courseArray[course_index].waitlist.length -= 1;
    }
    // release lock
    pthread_mutex_unlock(&course_access[course_index]);
    return user;
}