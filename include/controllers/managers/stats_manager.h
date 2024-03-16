#ifndef STAT_MAN
#define STAT_MAN
/**
 * @brief update stats
 *
 * @param type field to change
 * @param amount stat += amount
 */
void update_stats(char *type, int amount);

/**
 * @brief used only after killing all threads
 * so no locking is needed
 *
 */
void print_stats();

#endif