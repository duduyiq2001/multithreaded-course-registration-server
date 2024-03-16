#include "parsers/course_parser.h"
#include "server.h"
#include "utilities/userlist.h"
#include <string.h>

/**
 * @brief parsing the course information from fp
 *
 * @param carray
 * @param fp
 * @return the number of courses if success, -1 on error
 */
int course_parser(course_t *carray, FILE *fp)
{
    // counting total number of courses
    int count = 0;
    char buffer[BUFFER_SIZE];
    // loop through file
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        // split the string
        char *split_ptr = strchr(buffer, ';');
        // return error if ; does not exist
        if (split_ptr == NULL)
        {
            return -1;
        }
        // malloc the title
        carray[count].title = (char *)malloc(sizeof(char) * (split_ptr - buffer + 1));
        // copy the title
        strncpy(carray[count].title, buffer, split_ptr - buffer);
        // set null terminator
        carray[count].title[split_ptr - buffer] = '\0';
        // serialze max cap
        int maxCap = atoi(split_ptr + 1);
        // set max cap
        carray[count].maxCap = maxCap;
        // initialize waitlist and enrollist
        (carray[count].enrollment).head = NULL;
        (carray[count].enrollment).length = 0;
        (carray[count].enrollment).comparator = &userComparator;
        (carray[count].enrollment).printer = &userPrinter;
        (carray[count].enrollment).deleter = &userDeleter;
        // waitlist
        (carray[count].waitlist).head = NULL;
        (carray[count].waitlist).length = 0;
        (carray[count].waitlist).comparator = &userComparator;
        (carray[count].waitlist).printer = &userPrinter;
        (carray[count].waitlist).deleter = &userDeleter;

        // increment
        count += 1;
    }
    return count;
}
