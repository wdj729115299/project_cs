#include "sockutil.h"

/**
 * writen - 发送固定字节数
 * @fd: 文件描述符
 * @buf: 发送缓冲区
 * @count: 要读取的字节数
 * 成功返回count，失败返回-1
 */
ssize_t writen(int fd, const void *buf, size_t count)
{
	size_t nleft = count; //剩余的需要写入的字节数。
	ssize_t nwritten; //成功写入的字节数。
	char *bufp = (char*) buf; //将缓冲的指针强制转换为字符类型的指针。
	/*
	 * 如果剩余需要写入的字节数大于0则继续
	 * */
	while (nleft > 0)
	{
		/*
		 * ssize_t write(int fd, const void *buf, size_t count);
		 * fd为需要写入的文件描述符，buf为字符缓存区，count为需要写入的字节数。
		 *
		 * */
		if ((nwritten = write(fd, bufp, nleft)) < 0)
		{
			/*
			 * 如果是被信号中断的继续读
			 * */
			if (errno == EINTR)
				continue;
			return -1;
		} else if (nwritten == 0)
			continue;
		//字符指针推移已经写入成功大小的字节数。
		bufp += nwritten;
		//剩余的字节数。
		nleft -= nwritten;
	}
	return count;
}

/**
 * readn - 读取固定字节数
 * @fd: 文件描述符
 * @buf: 接收缓冲区
 * @count: 要读取的字节数
 * 成功返回count，失败返回-1，读到EOF返回<count
 */
//ssize_t 在x64下为long  在x86下为int
ssize_t readn(int fd, void *buf, size_t count)
{
	//size_t 在x64下为 unsigned long 类型， 在x86下为 unsigned int 类型
	size_t nleft = count; //将count接过来 ，个数
	ssize_t nread;
	char *bufp = (char*) buf; //将空指针类型转换为char类型指针。
	while (nleft > 0)
	{
		/*ssize_t read(int fd, void *buf, size_t count);
		 * 从文件描述符fd中读取count字节存到buf中
		 * 返回读取字节数的个数。
		 * */
		if ((nread = read(fd, bufp, nleft)) < 0)
		{
			/*
			 * 如果是被信号中断的继续读
			 * */
			if (errno == EINTR)
				continue;
			return -1;
		}
		/*
		 * 如果输入的读取个数为0，那么返回的读取个数为0
		 * 不执行任何操作。
		 * nleft为剩余的需要读取的字节个数。
		 * 如果为0，说明读到文件尾，
		 *
		 * */
		else if (nread == 0)
			return count - nleft;
		bufp += nread; //将字符指针向前推进已成功读取字符数的大小单位。
		nleft -= nread; //剩余的个数减去已经成功读取的字节数。
	}
	return count;
}

/**
 * recv_peek - 仅仅查看套接字缓冲区数据，但不移除数据
 * @sockfd: 套接字
 * @buf: 接收缓冲区
 * @len: 长度
 * 成功返回>=0，失败返回-1
 */
ssize_t recv_peek(int sockfd, void *buf, size_t len)
{
	while (1)
	{
		/*
		 * ssize_t recv(int sockfd, void *buf, size_t len, int flags);
		 * sockfd 套接字
		 * len 需要读取的长度
		 * MSG_PEEK只从队列中查看，但不取出。
		 * 返回接受到的字节的长度，失败返回-1，接受到关闭信号返回0；
		 * */
		int ret = recv(sockfd, buf, len, MSG_PEEK);
		/*
		 * 如果被信号中断了，继续
		 * */
		if (ret == -1 && errno == EINTR)
			continue;
		return ret;
	}
}

/**
 * readline - 按行读取数据
 * @sockfd: 套接字
 * @buf: 接收缓冲区
 * @maxline: 每行最大长度
 * 成功返回>=0，失败返回-1
 */
ssize_t readline(int sockfd, void *buf, size_t maxline)
{
	int ret;
	int nread;
	char *bufp = buf;
	int nleft = maxline;
	while (1)
	{
		ret = recv_peek(sockfd, bufp, nleft);
		if (ret < 0)
			return ret;
		else if (ret == 0)
			return ret;

		nread = ret;
		int i;
		for (i=0; i<nread; i++)
		{
			if (bufp[i] == '\n')
			{
				ret = readn(sockfd, bufp, i+1);
				if (ret != i+1)
					ERR_EXIT("readn failed\n");

				return ret;
			}
		}

		if (nread > nleft)
			ERR_EXIT("nread > nleft\n");

		nleft -= nread;
		ret = readn(sockfd, bufp, nread);
		if (ret != nread)
			ERR_EXIT("ret != nread");

		bufp += nread;
	}

	return -1;
}

int tcp_client(const char *address, unsigned short port)
{
	int sock;
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock < 0)
	{
		exit(1);
	}

	if (port > 0)
	{
		struct sockaddr_in localaddr;
		memset(&localaddr, 0, sizeof(localaddr));
		localaddr.sin_family = AF_INET;
		localaddr.sin_port = htons(port);
		inet_pton(AF_INET, address, &localaddr.sin_addr);		
		
		int on = 1;
		int ret = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
		if (ret < 0)
		{
			ret = errno;
			ERR_EXIT("setsockopt");
		}
		ret = bind(sock, (struct sockaddr*) &localaddr, sizeof(localaddr));
		if (ret < 0)
		{
			ret = errno;
			ERR_EXIT("bind");
		}		
	}
	
	return sock;
}

int activate_nonblock(int fd)
{
    return 0;
}

int deactivate_nonblock(int fd)
{
    return 0;
}

/*
 * 函数名：connect_timeout
 * 描述：客服端接受数据
 * 参数：
 *
 * 返回：
 * */
int connect_timeout(int fd, struct sockaddr_in *addr,
		unsigned int wait_seconds)
{
	int ret;
	//获取socket结构体的大小。
	socklen_t addrlen = sizeof(struct sockaddr_in);
	//如果传入的等待时间大于0就取消socket的阻塞状态，0则不执行。
	if (wait_seconds > 0)
		activate_nonblock(fd);
	//链接
	/*
	 * int connect(int sockfd, const struct sockaddr *addr,socklen_t addrlen);
	 *
	 * */
	ret = connect(fd, (struct sockaddr*) addr, addrlen);
	//EINPROGRESS 正在处理
	if (ret < 0 && errno == EINPROGRESS)
	{
		/*
		 * void FD_CLR(int fd, fd_set *set);
		 * int  FD_ISSET(int fd, fd_set *set);
		 * void FD_SET(int fd, fd_set *set);
		 * void FD_ZERO(fd_set *set);
		 * */
		//设置监听集合
		fd_set connect_fdset;
		struct timeval timeout;
		//初始化集合
		FD_ZERO(&connect_fdset);
		//把fd 文件描述符的socket加入监听集合
		FD_SET(fd, &connect_fdset);
		/*
		 * struct timeval {
		 *     long    tv_sec;         // seconds       秒
		 *     long    tv_usec;        // microseconds  微妙
		 *     };
		 * */
		timeout.tv_sec = wait_seconds;
		timeout.tv_usec = 0;
		do
		{
			// 一但连接建立，则套接字就可写  所以connect_fdset放在了写集合中
			ret = select(fd + 1, NULL, &connect_fdset, NULL, &timeout);
		} while (ret < 0 && errno == EINTR);
		if (ret == 0)
		{
			ret = -1;
			/*
			 * #define ETIMEDOUT       110     // Connection timed out
             *  Tcp是面向连接的。在程序中表现为，当tcp检测到对端socket不再可
             *  用时(不能发出探测包，或探测包没有收到ACK的响应包)，select会
             *  返回socket可读，并且在recv时返回-1，同时置上errno为ETIMEDOUT。
			 * */
			errno = ETIMEDOUT;
		} else if (ret < 0)
			return -1;
		else if (ret == 1)
		{
			/* ret返回为1（表示套接字可写），可能有两种情况，一种是连接建立成功，一种是套接字产生错误，*/
			/* 此时错误信息不会保存至errno变量中，因此，需要调用getsockopt来获取。 */
			int err;
			socklen_t socklen = sizeof(err);
			//获取socket的状态
			int sockoptret = getsockopt(fd, SOL_SOCKET, SO_ERROR, &err,
					&socklen);
			if (sockoptret == -1)
			{
				return -1;
			}
			if (err == 0)
			{
				ret = 0;
			} else
			{
				errno = err;
				ret = -1;
			}
		}
	}
	if (wait_seconds > 0)
	{
		deactivate_nonblock(fd);
	}
	return ret;
}

