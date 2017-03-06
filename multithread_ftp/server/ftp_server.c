#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "ftp_server.h"

/*
static void ftp_data_init(void)
{
    g_ftp_read_buf = (char *)malloc(sizeof(char) * g_read_buf_len);
    if(g_ftp_read_buf == NULL){
        printf("no memory for read_buf");
        exit(0);
    }

    g_ftp_write_buf = (char *)malloc(sizeof(char) * g_write_buf_len);
    if(g_ftp_write_buf == NULL){
        printf("no memory for write buf");
        exit(0);
    }
}
*/

void *ftp_main_loop(void *arg)
{
    int data_len;
    int length;
    FILE *fp;
    int ret;
    char g_ftp_read_buf[1024];
    char g_ftp_write_buf[1024];
    char buf[SEND_BUF_LEN];
    unsigned long total_bytes = 0;
    
    //ftp_data_init();
    
    int client_sd = *((int*) arg);
    while(1){
        memset(g_ftp_read_buf, 0, g_read_buf_len);
        memset(g_ftp_write_buf, 0, g_write_buf_len);
        memset(buf, 0, SEND_BUF_LEN);
            
        data_len = recv(client_sd, g_ftp_read_buf, g_read_buf_len, 0);
        if(data_len > 0){
           if(g_ftp_read_buf[strlen(g_ftp_read_buf)-1] == '\n'){
                g_ftp_read_buf[strlen(g_ftp_read_buf)-1] = '\0';
            }
           fp = fopen(g_ftp_read_buf, "rb");
           if(fp){
                printf("%s %d", g_ftp_read_buf, strlen(g_ftp_read_buf));
                fflush(stdout);
                strcpy(g_ftp_write_buf, "sure,you can get it.\n");
           }else{
                printf("%d %s ",  strlen(g_ftp_read_buf), g_ftp_read_buf);
                strcpy(g_ftp_write_buf, "sorry,you can't get it.\n");
                perror("open failed");
           }
           while( !feof(fp)){
                length = fread(buf, sizeof(char), SEND_BUF_LEN - 1, fp);
                send(client_sd, buf, length, 0);
                total_bytes += length;
                printf("send %lu bytes\n", total_bytes);
                memset(buf, 0, SEND_BUF_LEN);
           }
           fclose(fp);
        }
    }
}