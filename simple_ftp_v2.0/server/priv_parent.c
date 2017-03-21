#include <linux/capability.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#include "priv_parent.h"
#include "sockutil.h"

int minimize_privilege()
{
	/*设置权限 让nobody进程可以绑定20端口*/
    struct __user_cap_header_struct  cap_header;
	struct __user_cap_data_struct  cap_data;
	
	memset(&cap_header, 0, sizeof(cap_header));
	memset(&cap_data, 0, sizeof(cap_data));
	
	cap_header.version = _LINUX_CAPABILITY_VERSION_1;
	cap_header.pid = 0;
	
	__u32 cap_mask = 0;
	cap_mask |= (1 << CAP_NET_BIND_SERVICE); //绑定系统端口
	cap_data.effective = cap_data.permitted = cap_mask;
	cap_data.inheritable = 0;
	
	capset(&cap_header, &cap_data);	
	
	return 0;
}

static void privop_pasv_get_data_sock(session_t *sess)
{
	/*接收port*/
	unsigned short port = (unsigned short)priv_sock_get_int(sess->parent_fd);
	/*接收ip*/
	char ip[16] = {0};
	priv_sock_recv_buf(sess->parent_fd, ip, sizeof(ip));
	
	/*创建数据连接*/
	int data_sockfd = -1;
	
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	//addr.sin_addr = inet_addr(ip)
	inet_pton(AF_INET, ip, &addr.sin_addr);
	
	
	/*绑定20端口*/
	//sckServer_init(tunable_listen_address, 20, &data_sockfd);
	data_sockfd = tcp_client(tunable_listen_address, 20);
	
	if (data_sockfd == -1)
	{ 
		priv_sock_send_result(sess->parent_fd, PRIV_SOCK_RESULT_BAD);
		return;
	}
	
	if (connect_timeout(data_sockfd, &addr, tunable_connect_timeout) < 0)
	{
		close(data_sockfd);
		priv_sock_send_result(sess->parent_fd, PRIV_SOCK_RESULT_BAD);
		return;
	}
	/*发送成功信息*/
	priv_sock_send_result(sess->parent_fd, PRIV_SOCK_RESULT_OK);
	/*传输文件描述符*/
	priv_sock_send_fd(sess->parent_fd, data_sockfd);

	close(data_sockfd);
}

static void privop_pasv_active(session_t *session)
{
}

static void privop_pasv_listen(session_t *session)
{
}

static void privop_pasv_accept(session_t *session)
{
}

void handle_parent(session_t *session)
{
    /*将父进程变成nobody进程*/
	struct passwd *pw = getpwnam("nobody");
	if (pw == NULL)
	{
		return;
	}
	if (setegid(pw->pw_gid) < 0)
	{
		ERR_EXIT("setegid");
	}
	if (seteuid(pw->pw_uid) < 0)
	{
		ERR_EXIT("seteuid");
	}
	
	minimize_privilege();

    char cmd;
    while(1){
        cmd = priv_sock_get_cmd(session->parent_fd);

        switch(cmd){
            case PRIV_SOCK_GET_DATA_SOCK:
        		privop_pasv_get_data_sock(session);
        		break;
        	case PRIV_SOCK_PASV_ACTIVE:
        		privop_pasv_active(session);
        		break;
        	case PRIV_SOCK_PASV_LISTEN:
        		privop_pasv_listen(session);
        		break;
        	case PRIV_SOCK_PASV_ACCETP:
        		privop_pasv_accept(session);
        		break;
        }
    }
}
