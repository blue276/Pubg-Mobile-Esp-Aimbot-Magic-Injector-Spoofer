#include "ReadProcessMemory.h"
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/mman.h>






enum
{
    REQ_MAPS=0x1551,
    REQ_EXIT,
    REQ_STOP,
    REQ_INIT,
};

