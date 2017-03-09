#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>

int server_socket_init(const char *listen_address, unsigned short listen_port, int *fd)
{
	int ret = 0;
	int s;
	struct sockaddr_in servaddr;

	memset(&servaddr, 0, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(listen_port);

	if( listen_address == NULL ){
		servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	inet_pton(AF_INET, listen_address, &servaddr.sin_addr);

	s = socket(PF_INET, SOCK_STREAM, 0);
	if( s < 0 ){
		ret = errno;
		printf("create socket failed\n");
		return ret;
	}

	int on = 1;
	ret = setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	if(ret < 0){
		ret = errno;
		printf("setsockopt reuseaddr failed\n");
		return ret;
	}

	ret = bind(s, (struct sockaddr*)&servaddr, sizeof(servaddr));
	if( ret < 0 ){
		ret = errno;
		printf("bind failed\n");
		return ret;
	}

	ret = listen(s, SOMAXCONN);
	if( ret < 0 ){
		ret = errno;
		printf("listen failed\n");
		return ret;
	}

	*fd = s;

	return 0;
}

int accept_timeout(int fd, struct sockaddr_in *addr, int wait_seconds)
{
	int ret;
	socklen_t addr_len = sizeof(struct sockaddr_in);

	if( wait_seconds > 0 ){
		fd_set accept_fdset;
        struct timeval timeout;
        FD_ZERO(&accept_fdset);
        FD_SET(fd, &accept_fdset);
        timeout.tv_sec = wait_seconds;
        timeout.tv_usec = 0;

        do{
            ret = select(fd + 1, &accept_fdset, NULL, NULL, &timeout);
        }while(ret < 0 && errno == EINTR );
        if( ret < 0 ){
            return -1;
        }
        if( ret == 0){
            return 0;
        }
	}

	if( addr != NULL){
		ret = accept(fd, (struct sockaddr*)addr, &addr_len);
	}else{
		ret = accept( fd, NULL, NULL );
	}

	if( ret < 0 ){
		ret = errno;
		printf("accept error\n");
	}

	return ret;
}

int server_accept(int fd, int *connfd, struct sockaddr_in *addr, int timeout)
{
	int ret = 0;

	ret = accept_timeout(fd, addr, timeout);
	if(ret < 0){
        printf("%s line %d %d:accept_timeout failed\n", __FILE__, __LINE__, __FUNCTION__);
        return ret;
	}
    if(ret == 0 ){
        return 0;
    }
	*connfd = ret;

	return ret;
}
