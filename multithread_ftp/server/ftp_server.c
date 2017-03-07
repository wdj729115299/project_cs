#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "ftp_server.h"

static int sendn(int sd, void *buf, size_t count)
{
    int left = count;
    char *ptr = (char*)buf;

    while(left > 0){
        int n = send(sd, buf, left, 0);
        if( n < 0 ){
            if(errno == EINTR)
                continue;
            return -1;
        }else if( n == 0){
            continue;
        }else{
            left -= n;
            ptr += n;
        }
    }

    return count;
}

void *ftp_main_loop(void *arg)
{
    int data_len;
    int length;
    FILE *fp;
    int ret;
    char g_ftp_read_buf[1024];
    char g_ftp_write_buf[1024];
    unsigned long total_bytes = 0;
    struct packet pkt;
    
    int client_sd = *((int*) arg);
    while(1){
        memset(g_ftp_read_buf, 0, g_read_buf_len);
        memset(g_ftp_write_buf, 0, g_write_buf_len);
        memset(&pkt, 0, sizeof(pkt));
            
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
                length = fread(pkt.data, sizeof(char), sizeof(pkt.data), fp);
                if(length < 0){
                    printf("fread failed.");
                    break;
                }
                if(length == 0)
                    continue;
                pkt.msg_len = htonl(length);
                if(sendn(client_sd, (void*)&pkt, length + 4) < 0){
                    printf("send file failed.\n");
                    break;
                }
                total_bytes += length;
                printf("send %lu bytes\n", total_bytes);
                memset(&pkt, 0, sizeof(struct packet));
           }
           fclose(fp);
           close(client_sd);
        }
    }
}