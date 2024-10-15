
## Course Registration Server

A simple server written in C that manages and stores course registration information. Users can interact with the server by performing operations such as logging in, adding or dropping courses, waitlisting, and retrieving the current list of classes. The server is optimized for multithreading, allowing multiple users to maintain sessions simultaneously.

### Directory Structure

I structured my code slightly differently from others, following a structure typical of a web project. Here's the directory layout:

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


### Server Functionality

-   The server runner processes user requests, validates them, and passes them to the router.
-   The router directs requests to the appropriate controllers, which serve as the endpoints.
-   Controllers contain managers, representing database operations (an in-memory database in this case). **All locks are implemented at this level** to handle concurrency control effectively.
