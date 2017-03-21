#include "sockutil.h"

/**
 * writen - ���͹̶��ֽ���
 * @fd: �ļ�������
 * @buf: ���ͻ�����
 * @count: Ҫ��ȡ���ֽ���
 * �ɹ�����count��ʧ�ܷ���-1
 */
ssize_t writen(int fd, const void *buf, size_t count)
{
	size_t nleft = count; //ʣ�����Ҫд����ֽ�����
	ssize_t nwritten; //�ɹ�д����ֽ�����
	char *bufp = (char*) buf; //�������ָ��ǿ��ת��Ϊ�ַ����͵�ָ�롣
	/*
	 * ���ʣ����Ҫд����ֽ�������0�����
	 * */
	while (nleft > 0)
	{
		/*
		 * ssize_t write(int fd, const void *buf, size_t count);
		 * fdΪ��Ҫд����ļ���������bufΪ�ַ���������countΪ��Ҫд����ֽ�����
		 *
		 * */
		if ((nwritten = write(fd, bufp, nleft)) < 0)
		{
			/*
			 * ����Ǳ��ź��жϵļ�����
			 * */
			if (errno == EINTR)
				continue;
			return -1;
		} else if (nwritten == 0)
			continue;
		//�ַ�ָ�������Ѿ�д��ɹ���С���ֽ�����
		bufp += nwritten;
		//ʣ����ֽ�����
		nleft -= nwritten;
	}
	return count;
}

/**
 * readn - ��ȡ�̶��ֽ���
 * @fd: �ļ�������
 * @buf: ���ջ�����
 * @count: Ҫ��ȡ���ֽ���
 * �ɹ�����count��ʧ�ܷ���-1������EOF����<count
 */
//ssize_t ��x64��Ϊlong  ��x86��Ϊint
ssize_t readn(int fd, void *buf, size_t count)
{
	//size_t ��x64��Ϊ unsigned long ���ͣ� ��x86��Ϊ unsigned int ����
	size_t nleft = count; //��count�ӹ��� ������
	ssize_t nread;
	char *bufp = (char*) buf; //����ָ������ת��Ϊchar����ָ�롣
	while (nleft > 0)
	{
		/*ssize_t read(int fd, void *buf, size_t count);
		 * ���ļ�������fd�ж�ȡcount�ֽڴ浽buf��
		 * ���ض�ȡ�ֽ����ĸ�����
		 * */
		if ((nread = read(fd, bufp, nleft)) < 0)
		{
			/*
			 * ����Ǳ��ź��жϵļ�����
			 * */
			if (errno == EINTR)
				continue;
			return -1;
		}
		/*
		 * �������Ķ�ȡ����Ϊ0����ô���صĶ�ȡ����Ϊ0
		 * ��ִ���κβ�����
		 * nleftΪʣ�����Ҫ��ȡ���ֽڸ�����
		 * ���Ϊ0��˵�������ļ�β��
		 *
		 * */
		else if (nread == 0)
			return count - nleft;
		bufp += nread; //���ַ�ָ����ǰ�ƽ��ѳɹ���ȡ�ַ����Ĵ�С��λ��
		nleft -= nread; //ʣ��ĸ�����ȥ�Ѿ��ɹ���ȡ���ֽ�����
	}
	return count;
}

/**
 * recv_peek - �����鿴�׽��ֻ��������ݣ������Ƴ�����
 * @sockfd: �׽���
 * @buf: ���ջ�����
 * @len: ����
 * �ɹ�����>=0��ʧ�ܷ���-1
 */
ssize_t recv_peek(int sockfd, void *buf, size_t len)
{
	while (1)
	{
		/*
		 * ssize_t recv(int sockfd, void *buf, size_t len, int flags);
		 * sockfd �׽���
		 * len ��Ҫ��ȡ�ĳ���
		 * MSG_PEEKֻ�Ӷ����в鿴������ȡ����
		 * ���ؽ��ܵ����ֽڵĳ��ȣ�ʧ�ܷ���-1�����ܵ��ر��źŷ���0��
		 * */
		int ret = recv(sockfd, buf, len, MSG_PEEK);
		/*
		 * ������ź��ж��ˣ�����
		 * */
		if (ret == -1 && errno == EINTR)
			continue;
		return ret;
	}
}

/**
 * readline - ���ж�ȡ����
 * @sockfd: �׽���
 * @buf: ���ջ�����
 * @maxline: ÿ����󳤶�
 * �ɹ�����>=0��ʧ�ܷ���-1
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
 * ��������connect_timeout
 * �������ͷ��˽�������
 * ������
 *
 * ���أ�
 * */
int connect_timeout(int fd, struct sockaddr_in *addr,
		unsigned int wait_seconds)
{
	int ret;
	//��ȡsocket�ṹ��Ĵ�С��
	socklen_t addrlen = sizeof(struct sockaddr_in);
	//�������ĵȴ�ʱ�����0��ȡ��socket������״̬��0��ִ�С�
	if (wait_seconds > 0)
		activate_nonblock(fd);
	//����
	/*
	 * int connect(int sockfd, const struct sockaddr *addr,socklen_t addrlen);
	 *
	 * */
	ret = connect(fd, (struct sockaddr*) addr, addrlen);
	//EINPROGRESS ���ڴ���
	if (ret < 0 && errno == EINPROGRESS)
	{
		/*
		 * void FD_CLR(int fd, fd_set *set);
		 * int  FD_ISSET(int fd, fd_set *set);
		 * void FD_SET(int fd, fd_set *set);
		 * void FD_ZERO(fd_set *set);
		 * */
		//���ü�������
		fd_set connect_fdset;
		struct timeval timeout;
		//��ʼ������
		FD_ZERO(&connect_fdset);
		//��fd �ļ���������socket�����������
		FD_SET(fd, &connect_fdset);
		/*
		 * struct timeval {
		 *     long    tv_sec;         // seconds       ��
		 *     long    tv_usec;        // microseconds  ΢��
		 *     };
		 * */
		timeout.tv_sec = wait_seconds;
		timeout.tv_usec = 0;
		do
		{
			// һ�����ӽ��������׽��־Ϳ�д  ����connect_fdset������д������
			ret = select(fd + 1, NULL, &connect_fdset, NULL, &timeout);
		} while (ret < 0 && errno == EINTR);
		if (ret == 0)
		{
			ret = -1;
			/*
			 * #define ETIMEDOUT       110     // Connection timed out
             *  Tcp���������ӵġ��ڳ����б���Ϊ����tcp��⵽�Զ�socket���ٿ�
             *  ��ʱ(���ܷ���̽�������̽���û���յ�ACK����Ӧ��)��select��
             *  ����socket�ɶ���������recvʱ����-1��ͬʱ����errnoΪETIMEDOUT��
			 * */
			errno = ETIMEDOUT;
		} else if (ret < 0)
			return -1;
		else if (ret == 1)
		{
			/* ret����Ϊ1����ʾ�׽��ֿ�д�������������������һ�������ӽ����ɹ���һ�����׽��ֲ�������*/
			/* ��ʱ������Ϣ���ᱣ����errno�����У���ˣ���Ҫ����getsockopt����ȡ�� */
			int err;
			socklen_t socklen = sizeof(err);
			//��ȡsocket��״̬
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

