#ifndef _SERVER_H
#define _SERVER_H

#define SERVER_PORT     6000
#define READ_BUF_LEN    1024
#define WRITE_BUF_LEN   1024
#define BACK_LOG    5

static unsigned short g_server_listen_port;
static int g_back_log;
static int g_read_buf_len;
static int g_write_buf_len;

static char *read_buf;
static char *write_buf;

#endif

