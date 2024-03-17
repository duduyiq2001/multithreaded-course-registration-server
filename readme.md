### I structured my code slightly differently from other people. I sort of followed a structure of a typical web project, here is the directory structure:

include
├── auth
│   └── auth.h
├── controllers
│   ├── clist.h
│   ├── drop.h
│   ├── enroll.h
│   ├── managers
│   │   ├── courselist_manager.h
│   │   ├── logfile_manager.h
│   │   ├── stats_manager.h
│   │   └── userlist_manager.h
│   ├── sched.h
│   ├── waitadd.h
│   └── waitlist.h
├── debug.h
├── parsers
│   └── course_parser.h
├── protocol
│   ├── rmessage.h
│   └── wmessage.h
├── protocol.h
├── routing
│   └── router.h
├── server.h
└── utilities
    ├── linkedlist.h
    ├── multiplex.h
    ├── server_runners.h
    └── userlist.h

### *The server runner processes the user's request, validate it and pass it to the router 
### *the router routes request to controllers, which is like the endpoints 
### *under controllers there's managers, I use this to represent database layer operations (in this case it's an in memory database). Therefore naturally, ALL THE LOCKS ARE IMPLEMENTED AT THIS LEVEL