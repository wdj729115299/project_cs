#ifndef _SESSION_H
#define _SESSION_H
#include <pthread.h>
#include "hash.h"

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
    spinlock_t         lock;
}session_bucket_t;

typedef struct
{
    session_bucket_t    *buckets;
    unsigned long       hash_num;
    unsigned long       entry_num;
}session_table_t;

#define SESSION_TABLE_NUM   1
#define SESSION_BUCKET_NUM   65536 + 2

extern void session_table_init();
extern void server_session_init();
extern void session_table_add_entry();
extern void session_table_add();
#endif
