
set(NETWORK 
    network/network.h 
    network/network.c 
    network/queue.h 
    network/queue.c
)
add_library(network STATIC ${NETWORK})

set(PAXOS
    paxos/client.h
    paxos/client.c
    paxos/server.h
    paxos/server.c
)
add_library(paxoslib STATIC ${PAXOS})

set(LIBS 
    network
    paxoslib

    pthread    
)

add_executable(Paxos main.c)
target_link_libraries(Paxos ${LIBS})
