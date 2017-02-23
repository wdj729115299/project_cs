#include <stdlib.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <getopt.h>
#include "server.h"

static const struct option long_options[] = {
    {"help", 0, NULL, 'h'},
    {"version", 0, NULL, 'v'},
    {"port", 0, NULL, 'p'},
    {"blog", 0, NULL, 'b'},
    {"rbuf", 0, NULL, 'r'},
    {"wbuf", 0, NULL, 'w'},
    {NULL, 0, NULL, 0}
};

static const char *short_options = "p:b:r:w:";

static int global_data_default_init()
{
   g_server_listen_port = SERVER_PORT;
   g_back_log = BACK_LOG;
   g_read_buf_len = READ_BUF_LEN;
   g_write_buf_len = WRITE_BUF_LEN;
}

static int socket_init()
{
    int server_sd;
    struct sockaddr_in server_addr;
    socklen_t addr_len;
    
    /*step1: create socket*/
    server_sd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_sd < 0){
        printf("create socket failed.");
        return -1;
    }

    /*step2: bind*/
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(g_server_listen_port);
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

    return server_sd;
}

static void server_echo_loop(int client_sd)
{
    int data_len;
    
    while(1){
        memset(read_buf, 0, g_read_buf_len);
        memset(write_buf, 0, g_write_buf_len);

        data_len = recv(client_sd, read_buf, g_read_buf_len, 0);
        if(data_len > 0){
            printf("%s", read_buf);
        }else{
            close(client_sd);
            break;
        }
        send(client_sd, read_buf, strlen(read_buf), 0);
    }
}

static void server_loop(int server_sd)
{
    int client_sd;
    struct sockaddr_in client_addr;
    pid_t pid;
    
    socklen_t addr_len = sizeof(client_addr);
    
    while(1){
        client_sd = accept(server_sd, (struct sockaddr*)&client_addr, &addr_len);
        if(client_sd > 0){
            pid = fork();
            if( pid < 0){
                printf("fork error.");
            }else if( pid > 0 ){
                continue;
            }else{
                server_echo_loop(client_sd);
            }
        }
    }
}

static void get_server_options(int argc, char *argv[],
                        const char *short_options, const struct option *long_options)
{
    int opt;
    
    while( (opt =  getopt_long (argc, argv, short_options, long_options, NULL)) != -1){
        switch(opt){
            case 'h':
                break;
            case 'v':
                break;
            case 'p':
                g_server_listen_port = atoi(optarg);
                break;
            case 'b':
                g_back_log = atoi(optarg);
                break;
            case 'r':
                g_read_buf_len = atoi(optarg);
                break;
            case 'w':
                g_write_buf_len = atoi(optarg);
                break;
            default:
                break;
        }
    }
}

static void global_memory_init()
{
    read_buf = (char*)malloc(sizeof(char) * g_read_buf_len);
    if(read_buf == NULL){
        printf("no memory for read_buf");
        exit(0);
    }
    memset(read_buf, 0, g_read_buf_len);

    write_buf = (char*)malloc(sizeof(char) * g_write_buf_len);
    if(write_buf == NULL){
        printf("no memory for write_buf");
        exit(0);
    }
}

int main(int argc, char *argv[])
{
    global_data_default_init();
    
    get_server_options(argc, argv, short_options, long_options);

    global_memory_init();
    
    int server_sd = socket_init();
    if(server_sd < 0){
        printf("init socket failed.");
        return -1;
    }

    server_loop(server_sd);
}

