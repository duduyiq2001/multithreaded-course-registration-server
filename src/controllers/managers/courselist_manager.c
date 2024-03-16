#include "controllers/managers/courselist_manager.h"
#include "utilities/userlist.h"
#include "controllers/managers/logfile_manager.h"
#include <pthread.h>
/**
 * @brief
 * obtain course index lock
 * assume course index is valid
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

        return 0;
    }
    // spots available
    node_t *current = courseArray[course_index].enrollment.head;
    for (int i = 0; i < courseArray[course_index].enrollment.length; i++)
    {
        if (current != NULL && current->data != NULL && strcmp(((user_t *)current->data)->username, user->username) == 0)
        {

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
        fprintf(stderr, "%s, %d, %d, ", courseArray[i].title, courseArray[i].maxCap, courseArray[i].enrollment.length);
        userPrinter1(i, "enroll");
        fprintf(stderr, ", ");
        userPrinter1(i, "waitlist");
        fflush(stderr);
        fprintf(stderr, "\n");
    }
}
// before log out detach the thread.