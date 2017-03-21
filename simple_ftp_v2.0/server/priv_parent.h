#ifndef __PRIV_PARENT_H__
#define __PRIV_PARENT_H__

#include "session.h"
#include "tunable.h"

/*FTP���������nobody�������������*/
#define PRIV_SOCK_GET_DATA_SOCK		1
#define PRIV_SOCK_PASV_ACTIVE		2
#define PRIV_SOCK_PASV_LISTEN		3
#define PRIV_SOCK_PASV_ACCETP		4

/*nobody���̶�FTP������̵�Ӧ��*/
#define PRIV_SOCK_RESULT_OK			1
#define PRIV_SOCK_RESULT_BAD		2

extern void handle_parent(session_t *);

#endif
