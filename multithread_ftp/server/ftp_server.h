#ifndef _FTP_SERVER_H
#define _FTP_SERVER_H

#define SEND_BUF_LEN    512 

extern void *ftp_main_loop(void *arg);

extern unsigned int g_read_buf_len;
extern unsigned int g_write_buf_len;

struct packet{
    unsigned int msg_len;
    char data[SEND_BUF_LEN];
};

#endif
