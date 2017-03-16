#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include "session.h"

/*初始化内部进程间通讯通道*/
void priv_sock_init(session_t *session)
{
    int sockfds[2];

    if(socketpair(AF_UNIX, SOCK_STREAM, 0, sockfds) < 0){
        printf("%s line %d %d:create socketpari failed\n", __FILE__, __LINE__, __FUNCTION__);
        exit(1);
    }

    session->parent_fd = sockfds[0];
    session->child_fd = sockfds[1];
}

void do_priv_sock_set_context(int *fd)
{
    if( *fd != -1 ){
        close(*fd);
        *fd = -1;
    }
}

void priv_sock_set_child_context(session_t *session)
{
    do_priv_sock_set_context(&session->parent_fd);
}

void priv_sock_set_parent_context(session_t *session)
{
    do_priv_sock_set_context(&session->child_fd);
}