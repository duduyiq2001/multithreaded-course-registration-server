#include "utilities/userlist.h"
#include "server.h"
#include <math.h>
/**
 * @brief comparator function for user
 *
 * @param user1 user1 data
 * @param user2 user2 data
 * @return int
 */
int userComparator(const void *user1, const void *user2)
{
    user_t *user1t = (user_t *)user1;
    user_t *user2t = (user_t *)user2;
    return strcmp(user1t->username, user2t->username);
}

/**
 * @brief printer function for user
 *
 * @param user user data
 * @param fp
 */
void userPrinter(void *user, void *fp)
{
    fprintf(stderr, "%s, %d, %d\n", ((user_t *)user)->username, ((user_t *)user)->enrolled, ((user_t *)user)->waitlisted);
}

/**
 * @brief deleter function for user
 *
 * @param user user data
 */
void userDeleter(void *user)
{
    user_t *usert = (user_t *)user;
    free(usert->username);
    free(usert);
    return;
}
/**
 * @brief 2 ** power
 *
 * @param degree
 */
int twotopower(int degree)
{
    int num = 1;
    for (int i = 0; i < degree; i++)
    {
        num = num * 2;
    }
    return num;
}

/**
 * @brief update bit vector for enroll
 *
 * @param user
 */
void update_enroll(user_t *user, int course_index)
{
    user->enrolled += twotopower(course_index);
}

/**
 * @brief printer for userlist in course
 *
 * @param courseindex which course
 * @param type enroll/ waitlist
 */
void userPrinter1(int courseindex, char *type)
{
    if (strcmp(type, "enroll") == 0)
    {
        int length = courseArray[courseindex].enrollment.length;
#ifdef DEBUG
        printf("length of users is %d", length);
        fflush(stdout);
#endif
        node_t *current = courseArray[courseindex].enrollment.head;
        for (int i = 0; i < length; i++)
        {
            if (i == length - 1)
            {
#ifdef DEBUG
                printf("username is being printed");
                fflush(stdout);
#endif
                fprintf(stderr, "%s", ((user_t *)current->data)->username);
                continue;
            }
            fprintf(stderr, "%s;", ((user_t *)current->data)->username);
            current = current->next;
        }
    }
    else
    {
        int length = courseArray[courseindex].waitlist.length;
        node_t *current = courseArray[courseindex].waitlist.head;
        for (int i = 0; i < length; i++)
        {
            if (i == length - 1)
            {
                fprintf(stderr, "%s", ((user_t *)current->data)->username);
            }
            fprintf(stderr, "%s;", ((user_t *)current->data)->username);
            current = current->next;
        }
    }
}