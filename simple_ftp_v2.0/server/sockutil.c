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