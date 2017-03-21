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

/*接受命令（父<-子）*/
char priv_sock_get_cmd(int fd)
{
	char res;
	int ret = 0;
	ret = readn(fd, &res, sizeof(res));
	
	if (ret == 0)
	{
		printf("ftp process exit\n");
		exit(EXIT_SUCCESS);
	}
	
	if (ret != sizeof(res))
	{
		fprintf(stderr, "priv_sock_get_cmd error");
		exit(EXIT_FAILURE);
	}	
	return res;
}

/*发送结果（父->子）*/
void priv_sock_send_result(int fd, char res)
{
	int ret = 0;
	ret = writen(fd, &res, sizeof(res));
	if (ret != sizeof(res))
	{
		fprintf(stderr, "priv_sock_send_result error");
		exit(EXIT_FAILURE);
	}
}

/*发送文件描述符*/
void priv_sock_send_fd(int sock_fd, int fd)
{
	//send_fd(sock_fd, fd);
}

/*接收一个整数*/
int priv_sock_get_int(int fd)
{
	int the_int = 0;
	
	int ret = 0;
	ret = readn(fd, &the_int, sizeof(the_int));
	if (ret != sizeof(the_int))
	{
		fprintf(stderr, "priv_sock_get_int error");
		exit(EXIT_FAILURE);
	}	
	
	return the_int;
}

/*接收一个字符串*/
void priv_sock_recv_buf(int fd, char *buf, unsigned int len)
{
	unsigned int recv_len = (unsigned int)priv_sock_get_int(fd);
	if (recv_len > len)
	{
		fprintf(stderr, "priv_sock_recv_buf error\n");
		exit(EXIT_FAILURE);		
	}
	
	int ret = readn(fd, buf, recv_len);
	if (ret != recv_len)
	{
		fprintf(stderr, "priv_sock_recv_buf error\n");
		exit(EXIT_FAILURE);			
	}
	
}