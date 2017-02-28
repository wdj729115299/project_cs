#ifndef _FTP_SERVER_H
#define _FTP_SERVER_H

extern void *ftp_main_loop(void *arg);

extern unsigned int g_read_buf_len;
extern unsigned int g_write_buf_len;
#endif
