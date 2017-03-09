#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include "client.h"

static int recvn(int sd, void *buf, size_t count)
{
    int left = count;
    char *ptr = (char *)buf;
    while(left > 0){
        int n = recv(sd, ptr, left, 0);
        if( n < 0 ){
            if(errno == EINTR){
                printf("recvn eintr.\n");
                continue;
            }
            return -1;
        }else if( n == 0 ){     //EOF
            return count -left;
        }else{
            left -= n;
            ptr += n;
        }
    }
    return count;
}

int main(int argc, char *argv[])
{
    int sd;
    int ret;
    socklen_t addr_len;
    struct sockaddr_in addr_in;
    char read_buf[READ_BUF_LEN];
    char write_buf[WRITE_BUF_LEN];
    struct packet pkt;
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
    
    memset(read_buf, 0, READ_BUF_LEN);
    memset(write_buf, 0, WRITE_BUF_LEN);
    memset(&pkt, 0, sizeof(pkt));
    
    fgets(write_buf, WRITE_BUF_LEN, stdin);
    ret = send(sd, write_buf, strlen(write_buf), 0);
    if(ret < 0){
        printf("send '%s' failed.", write_buf);
    }

    fp = fopen("/home/bbb", "wb+");
    if(!fp){
        perror("open failed");
    }
   while(1){
        ret = recvn(sd, &pkt.msg_len, sizeof(pkt.msg_len));
        printf("ret = %d\n", ret);
        if(ret <= 0){
            break;
        }

        pkt.msg_len = ntohl(pkt.msg_len);

        ret = recvn(sd, pkt.data, pkt.msg_len);
        if(ret <= 0){
            break;
        }
        
        if(fwrite(pkt.data, sizeof(char), pkt.msg_len, fp) < pkt.msg_len){
            printf("write file failed.\n");
            break;
        }
        total_bytes += pkt.msg_len;
        printf("recv %lu bytes\n", total_bytes);
        memset(&pkt, 0, sizeof(pkt));
   }
   fclose(fp);
   
   close(sd);
}
