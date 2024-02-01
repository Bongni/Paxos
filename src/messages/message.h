#ifndef NETWORK_MESSAGE_H
#define NETWORK_MESSAGE_H

enum Command {
    RequestTicket = 1,
    Ok = 2,
    Propose = 3,
    Success = 4,
    Execute = 5
};

typedef struct {
    int sender;
    enum Command command;
    int ticket;
    int value;
} Message;

/*
    Serialize message
*/

unsigned char *serialize_int(unsigned char *buffer, int value);

unsigned char *serialize_message(unsigned char *buffer, Message *message);

/*
    Deserialize message
*/

unsigned char *deserialize_int(unsigned char *buffer, int *value);

unsigned char *deserialize_message(unsigned char *buffer, Message *message);

#endif