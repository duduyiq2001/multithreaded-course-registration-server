#include "routing/router.h"
#include "controllers/enroll.h"
#include "auth/auth.h"
/**
 * @brief router routes each request to each handler(controller)
 * each controller/ handler will take charge of logging and messaging
 * * return 1 if continue session
 * return 0 if logout
 * @param user
 * @param type
 * @param message
 */
int route(user_t *user, enum msg_types *type, char *message)
{
    if (*type == ENROLL)
    {
        // enroll user
        enroll(user, message);
    }
    if (*type == LOGOUT)
    {
        logout(user);
        // logout user
        return 0;
    }
    return 1;
}