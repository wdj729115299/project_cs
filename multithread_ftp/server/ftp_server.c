#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ftp_server.h"

static char *g_ftp_read_buf;
static char *g_ftp_write_buf;

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

void *ftp_main_loop(void *arg)
{
    int data_len;
    FILE *fp;
    int ret;
    
    ftp_data_init();
    
    int client_sd = *((int*) arg);
    while(1){
        memset(g_ftp_read_buf, 0, g_read_buf_len);
        memset(g_ftp_write_buf, 0, g_write_buf_len);
            
        data_len = recv(client_sd, g_ftp_read_buf, g_read_buf_len, 0);
        if(data_len > 0){
           fp = fopen(g_ftp_read_buf, "r");
           if(fp){
                printf("%s", g_ftp_read_buf);
                strcpy(g_ftp_write_buf, "sure,you can get it.\n");
           }else{
                strcpy(g_ftp_write_buf, "sorry,you can't get it.\n");
           }
           data_len = send(client_sd, g_ftp_write_buf, strlen(g_ftp_write_buf), 0);
           printf("%d\n", data_len);
        }
    }
}