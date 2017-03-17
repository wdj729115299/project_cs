#ifndef __PRIV_SOCK_H__
#define __PRIV_SOCK_H__

/*FTP服务进程向nobody进程请求的命令*/
#define PRIV_SOCK_GET_DATA_SOCK		1
#define PRIV_SOCK_PASV_ACTIVE		2
#define PRIV_SOCK_PASV_LISTEN		3
#define PRIV_SOCK_PASV_ACCETP		4

/*nobody进程对FTP服务进程的应答*/
#define PRIV_SOCK_RESULT_OK			1
#define PRIV_SOCK_RESULT_BAD		2

#endif
