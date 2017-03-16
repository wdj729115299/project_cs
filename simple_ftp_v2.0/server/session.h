#ifndef _SESSION_H
#define _SESSION_H
#include <pthread.h>
#include <unistd.h>
#include "hash.h"

#define SESSION_TABLE_NUM   1
#define SESSION_BUCKET_NUM   65536 + 2
#define MAX_COMMAND_LINE    1024
#define MAX_COMMAND      32
#define MAX_ARG			 1024

typedef struct session_flow_info
{
    unsigned long sip;          //host byte order
    unsigned short sport;       //host byte order
    //unsigned char proto;      //default tcp
} session_flow_info_t;

typedef struct
{
   session_flow_info_t tuple;
   struct hlist_node   node;
}session_entry_t;

typedef struct
{
    struct hlist_head   head;
    unsigned long       bucket_num;
    //spinlock_t         lock;
}session_bucket_t;

typedef struct
{
    session_bucket_t    *buckets;
    unsigned long       hash_num;
    unsigned long       entry_num;
}session_table_t;

typedef struct
{
    uid_t uid;
    int ctrl_fd;

    char cmdline[MAX_COMMAND_LINE];
    char cmd[MAX_COMMAND];
    char arg[MAX_ARG];

    //数据连接
	struct sockaddr_in *port_addr;
	int pasv_listen_fd;
	int data_fd;
	int data_process;  /*当前有没处于传输数据的状态*/

    int parent_fd;
    int child_fd;
}session_t;

extern session_table_t* session_table_init();
extern void server_session_init();
extern void session_table_add_entry();
extern void session_table_add();
extern void begin_session( int );
#endif
