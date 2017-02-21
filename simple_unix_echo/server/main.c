#include <stdlib.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include "server.h"

int main(int argc, char *argv[])
{
    int server_sd, client_sd;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    socklen_t addr_len;
    int data_len;
    char read_buf[READ_BUF_LEN];
    char write_buf[WRITE_BUF_LEN];

    /*step1: create socket*/
    server_sd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_sd < 0){
        printf("create socket failed.");
        return -1;
    }

    /*step2: bind*/
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(SERVER_PORT);
    addr_len = sizeof(server_addr);

    if(bind(server_sd, (struct sockaddr*)&server_addr, addr_len) < 0){
        printf("bind socket failed.");
        return -1;
    }

    /*step3: listen*/
    if(listen(server_sd, 5) < 0){
        printf("listen socket failed.");
        return -1;
    }
    
    /*step4: accept*/
    addr_len = sizeof(client_addr);
    while(1){
        client_sd = accept(server_sd, (struct sockaddr*)&client_addr, &addr_len);
        if(client_sd > 0){
            while(1){
                memset(read_buf, 0, READ_BUF_LEN);
                memset(write_buf, 0, WRITE_BUF_LEN);
                
                data_len = recv(client_sd, read_buf, READ_BUF_LEN, 0);
                if(data_len > 0){
                    printf("%s", read_buf);
                }else{
                    close(client_sd);
                    break;
                }
                
                send(client_sd, read_buf, strlen(read_buf), 0);
            }
        }
    }
}
