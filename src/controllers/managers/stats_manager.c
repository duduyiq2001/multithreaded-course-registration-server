#include "controllers/managers/stats_manager.h"
#include "server.h"
#include "pthread.h"
#include "string.h"
#include "debug.h"
void update_stats(char *type, int amount)
{
    pthread_mutex_lock(&stats_access);
    if (strcmp(type, "client") == 0)
    {
        curStats.clientCnt += amount;
    }
    if (strcmp(type, "thread") == 0)
    {
        curStats.threadCnt += amount;
    }
    if (strcmp(type, "adds") == 0)
    {
        curStats.totalAdds += amount;
    }
    if (strcmp(type, "client") == 0)
    {
        curStats.totalDrops += amount;
    }
    pthread_mutex_unlock(&stats_access);
}

/**
 * @brief used only after killing all threads
 * so no locking is needed
 *
 */
void print_stats()
{
#ifdef DEBUG
    fprintf(stderr, "enter stats");
#endif
    fprintf(stderr, "%d %d %d %d\n", curStats.clientCnt, curStats.threadCnt, curStats.totalAdds, curStats.totalDrops);
}