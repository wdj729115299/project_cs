#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "client.h"

int main(int argc, char *argv[])
{
    int sd;
    int ret;
    socklen_t addr_len;
    struct sockaddr_in addr_in;
    char read_buf[READ_BUF_LEN];
    char write_buf[WRITE_BUF_LEN];
    char buf[SEND_BUF_LEN];
    int length;
    unsigned long server_ip;
    FILE *fp;
    unsigned long total_bytes = 0;

    if(argc < 2){
        printf("please input server ip address.");
        return -1;
    }
   
    server_ip = inet_addr(argv[1]);
    if(server_ip == INADDR_NONE){
        printf("ip address error.");
        return -1;
    }
    
    /*step1: create socket*/
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if(sd < 0){
        printf("create socket failed.");
        return;
    }

    /*step2:connect to server*/
    addr_in.sin_family = AF_INET;
    addr_in.sin_port = htons(CONNECT_PORT);
    addr_in.sin_addr.s_addr = server_ip;
    addr_len = sizeof(struct sockaddr_in);

    ret = connect(sd, (struct sockaddr*)&addr_in, addr_len);
    if(ret < 0){
        printf("connect to server failed.");   
        return ret;
    }

    while(1){
        
        memset(read_buf, 0, READ_BUF_LEN);
        memset(write_buf, 0, WRITE_BUF_LEN);
        memset(buf, 0, SEND_BUF_LEN);
        
        fgets(write_buf, WRITE_BUF_LEN, stdin);
        ret = send(sd, write_buf, strlen(write_buf), 0);
        if(ret < 0){
            printf("send '%s' failed.", write_buf);
        }

        fp = fopen("/home/bbb", "wb+");
        if(!fp){
            perror("open failed");
        }
       while((length = recv(sd, buf, SEND_BUF_LEN - 1, 0)) > 0){
            fwrite(buf, sizeof(char), length, fp);
            total_bytes += length;
            printf("recv %lu bytes\n", total_bytes);
            memset(buf, 0, SEND_BUF_LEN);
       }
       fclose(fp);
    }
    close(sd);
}
