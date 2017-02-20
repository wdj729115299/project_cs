/*
 *client main
 *
 */
 
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
    unsigned long server_ip;

    if(argc < 2){
        printf("please input server ip address.");
        return -1;
    }
   
    server_ip = inet_addr(argv[1]);
    if(server_ip == INADDR_NONE){
        printf("ip address error.");
        return -1;
    }

    memset(read_buf, 0, READ_BUF_LEN);
    memset(write_buf, 0, WRITE_BUF_LEN);

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
        scanf("%s", write_buf);
        ret = send(sd, write_buf, strlen(write_buf), 0);
        if(ret < 0){
            printf("send '%s' failed.", write_buf);
        }

        ret = recv(sd, read_buf, READ_BUF_LEN, 0);
        if(ret > 0){
            printf("%s", read_buf);
        }

        memset(read_buf, 0, READ_BUF_LEN);
        memset(write_buf, 0, WRITE_BUF_LEN);
    }
}
