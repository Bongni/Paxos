
set(NETWORK 
    network/network.h 
    network/network.c 
    network/queue.h 
    network/queue.c
)
add_library(network STATIC ${NETWORK})

add_executable(Paxos main.c)
