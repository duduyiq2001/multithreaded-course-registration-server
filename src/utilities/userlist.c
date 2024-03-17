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
uint32_t twotopower(int degree)
{
    // Ensure n is within the range of 0 to (sizeof(unsigned long) * 8 - 1)
    // to prevent shifting more bits than the size of unsigned long
    if (degree < 0 || degree >= sizeof(uint32_t) * 8)
    {
        printf("n is out of range. Must be between 0 and %lu.\n", sizeof(uint32_t) * 8 - 1);
        return 0;
    }

    return 1U << degree;
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
void userPrinter1(int courseindex, char *type, int delimiter)
{
    char *deltr = (delimiter == SEMICOLON) ? ";" : "->";
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
                fprintf(stdout, "%s", ((user_t *)current->data)->username);
                continue;
            }
            fprintf(stdout, "%s%s", ((user_t *)current->data)->username, deltr);
            current = current->next;
        }
    }
    /**
     * @brief For waitlist
     *
     */
    else
    {
        int length = courseArray[courseindex].waitlist.length;
        node_t *current = courseArray[courseindex].waitlist.head;
        for (int i = 0; i < length; i++)
        {
            if (i == length - 1)
            {
                fprintf(stdout, "%s", ((user_t *)current->data)->username);
                continue;
            }
            fprintf(stdout, "%s%s", ((user_t *)current->data)->username, deltr);
            current = current->next;
        }
    }
}

/**
 * @brief update bit vector for waitlist
 *
 * @param user
 */
void update_wait(user_t *user, int course_index)
{
    user->waitlisted += twotopower(course_index);
}
/**
 * @brief printer for userlist in course
 *
 * @param courseindex which course
 *
 * @param type enroll/ waitlist
 * @param delimiter SEMICOLON/ARROW
 * @param output string to output to, need to alloc before
 */
void SuserPrinter1(int courseindex, char *type, int delimiter, char *output)
{
    char *deltr = (delimiter == SEMICOLON) ? ";" : "->";
    output[0] = '\0';
    if (strcmp(type, "enroll") == 0)
    {
        int length = courseArray[courseindex].enrollment.length;
        // send back null string if no course
        if (length == 0)
        {
            return;
        }
#ifdef DEBUG
        printf("length of users is %d", length);
        fflush(stdout);
#endif
        node_t *current = courseArray[courseindex].enrollment.head;
        for (int i = 0; i < length; i++)
        {
            char buffer[BUFFER_SIZE];
            if (i == length - 1)
            {
                sprintf(buffer, "%s", ((user_t *)current->data)->username);
                continue;
            }
            sprintf(buffer, "%s%s", ((user_t *)current->data)->username, deltr);
            current = current->next;
            // append to output
            strcat(output, buffer);
        }
    }
    /**
     * @brief for waitlist
     *
     */
    else
    {
        int length = courseArray[courseindex].waitlist.length;
        // send back null string if no course
        if (length == 0)
        {
            output[0] = '\0';
            return;
        }
        node_t *current = courseArray[courseindex].waitlist.head;
        for (int i = 0; i < length; i++)
        {
            char buffer[BUFFER_SIZE];
            if (i == length - 1)
            {
                sprintf(buffer, "%s", ((user_t *)current->data)->username);
            }
            sprintf(buffer, "%s%s", ((user_t *)current->data)->username, deltr);
            current = current->next;
            // append to output
            strcat(output, buffer);
        }
    }
}

/**
 * @brief update user enrollment
 * removing that class from user
 *
 * @param user
 * @param course_index
 */
void rm_update_enroll(user_t *user, int course_index)
{
    user->enrolled -= twotopower(course_index);
}

/**
 * @brief update user waitlist
 * removing that class from user
 *
 * @param user
 * @param course_index
 */
void rm_update_wait(user_t *user, int course_index)
{
    user->waitlisted -= twotopower(course_index);
}
