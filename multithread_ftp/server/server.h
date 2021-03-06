#ifndef _SERVER_H
#define _SERVER_H

#include "ftp_server.h"

#define SERVER_PORT     5000
#define READ_BUF_LEN    1024
#define WRITE_BUF_LEN   1024
#define BACK_LOG    5

enum work_mode
{
    MULTI_PROCESS_MODE,
    MULTI_THREAD_MODE,
    WORK_MODE_MAX
};

static unsigned short g_server_listen_port;
static int g_back_log;
static int g_work_mode;

#endif

