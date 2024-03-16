#ifndef ROUTE
#define ROUTE
#include "../server.h"
#include "../protocol.h"
/**
 * @brief router routes each request to each handler(controller)
 * each controller/ handler will take charge of logging and messaging
 * return 1 if continue session
 * return 0 if logout
 * @param user
 * @param type
 * @param message
 */
int route(user_t *user, enum msg_types *type, char *message);

#endif