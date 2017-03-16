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