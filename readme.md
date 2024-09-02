### I structured my code slightly differently from other people. I sort of followed a structure of a typical web project, here is the directory structure:

include <br/>
├── auth <br/>
│   └── auth.h <br/>
├── controllers <br/>
│   ├── clist.h <br/>
│   ├── drop.h <br/>
│   ├── enroll.h <br/>
│   ├── managers <br/>
│   │   ├── courselist_manager.h <br/>
│   │   ├── logfile_manager.h <br/>
│   │   ├── stats_manager.h <br/>
│   │   └── userlist_manager.h <br/>
│   ├── sched.h <br/>
│   ├── waitadd.h <br/>
│   └── waitlist.h <br/>
├── debug.h <br/>
├── parsers <br/>
│   └── course_parser.h <br/>
├── protocol <br/>
│   ├── rmessage.h <br/>
│   └── wmessage.h <br/>
├── protocol.h <br/>
├── routing <br/>
│   └── router.h <br/>
├── server.h <br/>
└── utilities <br/>
    ├── linkedlist.h <br/>
    ├── multiplex.h <br/>
    ├── server_runners.h <br/>
    └── userlist.h <br/>


### *The server runner processes the user's request, validate it and pass it to the router 
### *the router routes request to controllers, which is like the endpoints 
### *under controllers there's managers, I use this to represent database layer operations (in this case it's an in memory database). Therefore naturally, ALL THE LOCKS ARE IMPLEMENTED AT THIS LEVEL