#include "session.h"
#include "ftpproto.h"

static void do_user(session_t *session)
{
}

static void do_port(session_t *session)
{
}

static void do_retr(session_t *session)
{
}

void ftp_relply(session_t *sess, int status, const char *text)
{
	char buf[1024] = {0};
	sprintf(buf, "%d %s\r\n", status, text);
	writen(sess->ctrl_fd, buf, strlen(buf));
}

static ftpcmd_t ctrl_cmds[] = {
    /*访问控制命令*/
    {"USER", do_user},

    /*传输参数命令*/
    {"PORT",  do_port },

    /*服务命令*/
	{"RETR",  do_retr },
};

void handle_parent(session_t *session)
{
}

void handle_child(session_t *session)
{
    int ret = 0;
    ftp_relply(session->ctrl_fd, FTP_GREET, "( tinyftpd 1.0 )");

    while(1){
        memset(session->cmdline, 0, sizeof(session->cmdline));
        memset(session->cmd, 0, sizeof(session->cmd));
        memset(session->arg, 0, sizeof(session->arg));
    }
}
