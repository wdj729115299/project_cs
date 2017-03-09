#include <stdlib.h>
#include <stdio.h>

void handle_sigchld( int sig )
{
	
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

	signal(SIGCHLD, handle_sigchld);

	//server_conf_init();

	ret = server_socket_init(tunable_listen_address, tunable_listen_port, &liten_fd);
	if( ret < 0){
		fprintf(stderr, "server socket init failed");
		exit(EXIT_FAILURE);
	}

	int connfd;
	struct sockaddr_in addr;

	while(1){
		ret = server_accept(listen_fd, &connfd, &addr, 0);
        if( ret < 0){
            exit(EXIT_FAILURE);
        }
        pid = fork();
        if(pid < 0){
            printf("%s line %d %d: fork error\n", __FILE__, __LINE__, __FUNCTION__);
            exit(EXIT_FAILURE);
        }else if(pid > 0){
            close(connfd);
        }else{
            
        }
	}
	
}
