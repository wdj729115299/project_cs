#ifndef _CLIENT_H
#define _CLIENT_H

#define CONNECT_PORT    5000

#define SEND_BUF_LEN        512

#define READ_BUF_LEN    1024
#define WRITE_BUF_LEN   1024

struct packet{
    unsigned int msg_len;
    char data[SEND_BUF_LEN];
};

#endif

