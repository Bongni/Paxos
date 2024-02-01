#include "message.h"

unsigned char *serialize_int(unsigned char *buffer, int value) {
    buffer[0] = value >> 24;
    buffer[1] = value >> 16;
    buffer[2] = value >> 8;
    buffer[3] = value;

    return buffer + 4;
}

unsigned char *serialize_message(unsigned char *buffer, Message *message) {
    buffer = serialize_int(buffer, message->sender);
    buffer = serialize_int(buffer, message->command);
    buffer = serialize_int(buffer, message->ticket);
    buffer = serialize_int(buffer, message->value);

    return buffer;
}


unsigned char *deserialize_int(unsigned char *buffer, int *value) {
    *value = 0;
    *value += buffer[0] << 24;
    *value += buffer[1] << 16;
    *value += buffer[2] << 8;
    *value += buffer[3];

    return buffer + 4;
}

unsigned char *deserialize_message(unsigned char *buffer, Message *message) {
    buffer = deserialize_int(buffer, &message->sender);
    buffer = deserialize_int(buffer, (int *) &message->command);
    buffer = deserialize_int(buffer, &message->ticket);
    buffer = deserialize_int(buffer, &message->value);

    return buffer + 4;
}