#ifndef CPARSER_H
#define CPARSER_H
#include "../server.h"
#include <stdlib.h>

/**
 * @brief parsing the course information from fp
 *
 * @param carray
 * @param fp
 * @return the number of courses if success, -1 on error
 */
int course_parser(course_t *carray, FILE *fp);

#endif