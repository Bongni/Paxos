
set(LIB 
    network/network.h 
    network/network.c 
    network/queue.h 
    network/queue.c


    paxos/client.h
    paxos/client.c
    paxos/server.h
    paxos/server.c


    helpers/timer.h
    helpers/timer.c
)
add_library(lib STATIC ${LIB})


add_executable(Paxos main.c)
target_link_libraries(Paxos pthread lib)
