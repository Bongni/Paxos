#ifndef NETWORK_MESSAGE_H
#define NETWORK_MESSAGE_H

enum Command {
    RequestTicket,
    Ok,
    Propose,
    Success,
    Execute
};

typedef struct {
    int sender;
    enum Command command;
    int ticket;
    int value;
} Message;

#endif