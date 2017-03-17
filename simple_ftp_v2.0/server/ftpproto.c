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

void ftp_reply(session_t *sess, int status, const char *text)
{
	char buf[1024] = {0};
	sprintf(buf, "%d %s\r\n", status, text);
	writen(sess->ctrl_fd, buf, strlen(buf));
}

static ftpcmd_t ctrl_cmds[] = {
    /*���ʿ�������*/
    {"USER", do_user},

    /*�����������*/
    {"PORT",  do_port },

    /*��������*/
	{"RETR",  do_retr },
};

session_t *p_sess;

void handle_alarm_timeout( int sig )
{
    shutdown(p_sess->ctrl_fd, SHUT_RD);
    ftp_reply(p_sess, FTP_IDLE_TIMEOUT, "Timeout");
    shutdown(p_sess->ctrl_fd, SHUT_WR);
    exit(1);
}

void start_cmdio_alarm()
{
    if(tunable_idle_session_timeout > 0){
        signal(SIGALRM, handle_alarm_timeout);
        alarm(tunable_idle_session_timeout);
    }
    
}

void handle_child(session_t *session)
{
    int ret = 0, i, size;
    ftp_reply(session->ctrl_fd, FTP_GREET, "( tinyftpd 1.0 )");

    while(1){
        memset(session->cmdline, 0, sizeof(session->cmdline));
        memset(session->cmd, 0, sizeof(session->cmd));
        memset(session->arg, 0, sizeof(session->arg));

        p_sess = session;
        start_cmdio_alarm();

        ret = readline(session->ctrl_fd, (void *)session->cmdline, sizeof(session->cmdline));
        if( ret <= 0){
            exit(1);
        }

        //ȥ��\r\n
        str_trim_crlf(session->cmdline)
        //����FTP����Ͳ���
		str_split(session->cmdline, session->cmd, session->arg, ' ');
        //������ת��Ϊ��д
		str_upper(session->cmd);

        size = sizeof(ctrl_cmds)/sizeof(ctrl_cmds[0]);
        for( i = 0; i < size; i++){
            if( strcmp(ctrl_cmds[i].cmd, session->cmd) == 0 ){
                if( ctrl_cmds[i].cmd_handler != NULL ){
                    ctrl_cmds[i].cmd_handler(session);
                }else{
                    /*δʵ������*/
					ftp_reply(session, FTP_COMMANDNOTIMPL, "Uncompletement command");	
                }
            }
            break;
        }
        if (i == size)
		{
			/*����ʶ����*/
			ftp_reply(session, FTP_BADCMD, "Bad command");			
		}
    }
}
