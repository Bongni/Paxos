
set(LIB_SIMULATION
    networkSimulation/network.h 
    networkSimulation/network.c


    messages/queue.h 
    messages/queue.c
    messages/message.h


    paxosSimulation/client.h
    paxosSimulation/client.c
    paxosSimulation/server.h
    paxosSimulation/server.c


    helpers/timer.h
    helpers/timer.c
)
add_library(lib_simulation STATIC ${LIB_SIMULATION})

set(LIB 
    network/network.h 
    network/network.c 

    
    messages/queue.h 
    messages/queue.c
    messages/message.h


    paxos/client.h
    paxos/client.c
    paxos/server.h
    paxos/server.c


    helpers/timer.h
    helpers/timer.c
)
add_library(lib STATIC ${LIB})


add_executable(Paxos main.c)
target_link_libraries(Paxos pthread lib_simulation)


add_executable(Client main.c)
target_link_libraries(Client pthread lib)


add_executable(Server main.c)
target_link_libraries(Server pthread lib)
