#include "routing/router.h"
#include "controllers/enroll.h"
#include "controllers/waitlist.h"
#include "auth/auth.h"
#include "controllers/clist.h"
#include "controllers/sched.h"
#include "controllers/drop.h"
/**
 * @brief router routes each request to each handler(controller)
 * each controller/ handler will take charge of logging and messaging
 * * return 0 if continue session
 * didn't use switch because too scared to forget a break statement in
 * each block lol
 * return 1 if logout
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
    if (*type == WAIT)
    {
        // waitlist user
        waitlist(user, message);
    }
    if (*type == LOGOUT)
    {
        // detach first
        pthread_detach(pthread_self());
        logout(user);
        return 1;
    }
    if (*type == CLIST)
    {
        clist(user);
    }
    if (*type == SCHED)
    {
        sched(user);
    }
    if (*type == DROP)
    {
        drop(user, message);
    }
    return 0;
}