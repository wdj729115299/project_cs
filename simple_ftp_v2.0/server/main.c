#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <netinet/in.h>
#include <string.h>
#include "tunable.h"
#include "session.h"

static void server_conf_init()
{
	tunable_listen_address = TUNABLE_LISTEN_ADDRESS;
	tunable_listen_port = TUNABLE_LISTEN_PORT;
    tunable_idle_session_timeout = TUNABLE_IDLE_SESSION_TIMEOUT;
	
}

int main(int argc, char *argv[])
{
	int listen_fd;
	int ret;
    pid_t pid;
	
	if( getuid() != 0 ){
		fprintf(stderr, "ftp_v2.0 server:must be started as root\n");
		exit(EXIT_FAILURE);
	}

	signal(SIGCHLD, SIG_IGN);

	server_conf_init();
    server_session_init();

	ret = server_socket_init(tunable_listen_address, tunable_listen_port, &listen_fd);
	if( ret < 0){
		fprintf(stderr, "server socket init failed");
		exit(EXIT_FAILURE);
	}

	int connfd;
	struct sockaddr_in addr;

	while(1){
		ret = server_accept(listen_fd, &connfd, &addr, 0);
        if( ret == 0){
            continue;
        }
        if( ret < 0){
            exit(EXIT_FAILURE);
        }
		printf("accept something.\n");
        pid = fork();
        if(pid < 0){
            printf("%s line %d %d: fork error\n", __FILE__, __LINE__, __FUNCTION__);
            exit(EXIT_FAILURE);
        }else if(pid > 0){		//父进程返回子进程ID
        	session_table_add(&addr, 0);
			//父进程关闭连接socket
            close(connfd);
        }else{					//子进程返回0
        	//子进程关闭监听socket
            close(listen_fd);

            //让session中的进程忽略handle_sigchld处理函数
			signal(SIGCHLD, SIG_IGN);
            
            begin_session(connfd);
        }
	}
	
}
