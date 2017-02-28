#include <stdlib.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <getopt.h>
#include <pthread.h>
#include "server.h"

static const struct option long_options[] = {
    {"help", 0, NULL, 'h'},
    {"version", 0, NULL, 'v'},
    {"port", 0, NULL, 'p'},
    {"blog", 0, NULL, 'b'},
    {"rbuf", 0, NULL, 'r'},
    {"fork", 0, NULL, 'f'},
    {"thread", 0, NULL, 't'},
    {"wbuf", 0, NULL, 'w'},
    {NULL, 0, NULL, 0}
};

static const char *short_options = "p:b:r:w:tf";
unsigned int g_read_buf_len;
unsigned int g_write_buf_len;

static int global_data_default_init()
{
   g_server_listen_port = SERVER_PORT;
   g_back_log = BACK_LOG;
   g_read_buf_len = READ_BUF_LEN;
   g_write_buf_len = WRITE_BUF_LEN;
   g_work_mode = MULTI_THREAD_MODE;
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

static void action_loop(int client_sd)
{
    pid_t fork_pid;        
    pthread_t thread_pid;
    
    if(g_work_mode == MULTI_PROCESS_MODE){
        fork_pid = fork();
        if( fork_pid < 0){
            printf("fork error.");
        }else if( fork_pid > 0 ){
            return;
        }else{
            ftp_main_loop((void*)&client_sd);
        }
    }else if(g_work_mode == MULTI_THREAD_MODE){

        pthread_create(&thread_pid, NULL, ftp_main_loop, &client_sd);
    }else{
        printf("wrok mode error.");
        exit(0);
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
           action_loop(client_sd);
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
            case 'f':
                g_work_mode = MULTI_PROCESS_MODE;
                break;
            case 't':
                g_work_mode = MULTI_THREAD_MODE;
                break;
            default:
                break;
        }
    }
}

int main(int argc, char *argv[])
{
    global_data_default_init();

    get_server_options(argc, argv, short_options, long_options);
    
    int server_sd = socket_init();
    if(server_sd < 0){
        printf("init socket failed.");
        return -1;
    }
    
    server_loop(server_sd);
}

