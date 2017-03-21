#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include "session.h"
#include "hash.h"

session_table_t **g_session_table = NULL;

session_table_t* session_table_init()
{
	int i;
	
    session_table_t *session_table = (session_table_t*)malloc(sizeof(session_bucket_t) * SESSION_BUCKET_NUM);
    if(!session_table){
        printf("%s line %d %d: malloc failed\n", __FILE__, __LINE__, __FUNCTION__);
        exit(1);
    }
    session_table->hash_num = SESSION_BUCKET_NUM;
    //atomic_set(&session_table->entry_num, 0);
    session_table->entry_num = 0;
    
    session_table->buckets = 
                       (session_bucket_t*)malloc(sizeof(session_bucket_t) * SESSION_BUCKET_NUM);
    if(!session_table->buckets){
        printf("%s line %d %d: malloc failed\n", __FILE__, __LINE__, __FUNCTION__);
        exit(1);
    }

    session_bucket_t *buckets = session_table->buckets;

    for( i = 0; i < SESSION_BUCKET_NUM; i++){
        buckets[i].bucket_num = 0;
        INIT_HLIST_HEAD(&buckets[i].head);
        //spin_lock_init(&buckets[i].lock);
    }

    return session_table;
}

void server_session_init()
{
	int i;
	
    g_session_table = (session_table_t*)malloc(sizeof(session_table_t*) * SESSION_TABLE_NUM);
    if(!g_session_table){
        printf("%s line %d %d: malloc failed\n", __FILE__, __LINE__, __FUNCTION__);
        exit(1);
    }

    for( i = 0; i < SESSION_TABLE_NUM; i++){
        g_session_table[i] = session_table_init();
    }
}

void session_table_add_entry(session_flow_info_t *flow_info, session_table_t *session_table)
{
    unsigned int hash;
    session_bucket_t *bucket;
    session_entry_t *new_entry;
    
    hash = jhash_2words( flow_info->sip, flow_info->sport, 0);
    bucket = &session_table->buckets[hash % session_table->hash_num];
    //spin_lock(&bucket->lock);
    new_entry = (session_entry_t*)malloc(sizeof(session_entry_t));
    if(new_entry == NULL){
        printf("%s line %d %d: malloc failed\n", __FILE__, __LINE__, __FUNCTION__);
        //spin_unlock(&bucket->lock);
    }
    memset(new_entry, 0, sizeof(session_entry_t));
    memcpy(&new_entry->tuple, flow_info, sizeof(session_flow_info_t));
    INIT_HLIST_NODE(&new_entry->node);
    hlist_add_head(&new_entry->node, &bucket->head);
    bucket->bucket_num++;
    //spin_unlock(&bucket->lock);

    //atomic_inc(&session_table->entry_num);
    session_table->entry_num++;
}

void session_table_add(struct sockaddr_in *addr, int index)
{   
    session_flow_info_t flow_info;
    flow_info.sip = ntohl(addr->sin_addr.s_addr);
    flow_info.sport = ntohs(addr->sin_port);
    
    session_table_add_entry(&flow_info, g_session_table[index]);
}

void begin_session(int connfd)
{
    session_t *session = (session_t *)malloc(sizeof(session_t));
    if( session == NULL ){
        printf("%s line %d %d:accept_timeout failed\n", __FILE__, __LINE__, __FUNCTION__);
        exit(1);
    }

    priv_sock_init(session);
    
    session->ctrl_fd = connfd;

    pid_t pid;
    pid = fork();
    if( pid < 0 ){
        printf("%s line %d %d:fork failed\n", __FILE__, __LINE__, __FUNCTION__);
        exit(1);
    }else if( pid == 0 ){
        /*设置子进程环境*/
		priv_sock_set_child_context(session);		
		handle_child(session);
    }else{
        /*设置父进程环境*/
		priv_sock_set_parent_context(session);
		handle_parent(session);
    }
}