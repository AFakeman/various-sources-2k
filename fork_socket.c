//
//  main.c
//  acos
//
//  Created by Anton Suslov on 26/05/2017.
//  Copyright © 2017 Anton Suslov. All rights reserved.
//

#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <errno.h>

#define PORT 8081
#define BACKLOG 5
#define BUF_SIZE 4096

int sockfd = 0;
int accepted;

void graceful_exit() {
    close(sockfd);
    close(accepted);
    exit(EXIT_FAILURE);
}

void process_sigint(int signal) {
    graceful_exit();
}

void run() {
    int port = PORT;
    int backlog = BACKLOG;
    ssize_t rd;
    char buffer[BUF_SIZE];
    struct sockaddr_in serv_addr;
    struct sockaddr_in client_addr;
    char *addr;
    pid_t forked;
    socklen_t client_len = sizeof(client_addr);
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket create");
        graceful_exit();
    }
    memset(&serv_addr, 0 , sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("Bind error");
        graceful_exit();
    }
    if (listen(sockfd, backlog) < 0) {
        perror("Listen");
        graceful_exit();
    }
    for(;;) {
        if ((accepted = accept(sockfd, (struct sockaddr *) &client_addr, &client_len)) < 0) {
            if (errno != EINTR) {
                perror("Accept");
                graceful_exit();
            }
            continue;
        }
        if ((forked = fork()) < 0) {
            perror("Fork");
            graceful_exit();
        }
        if (forked == 0) {
            if (dup2(accepted, 1) < 0) {
                graceful_exit();
            }
            if (dup2(accepted, 2) < 0) {
                graceful_exit();
            }
            if (system("date") < 0) {
                graceful_exit();
            }
            close(accepted);
            exit(EXIT_SUCCESS);
        } else {
            addr = inet_ntoa(client_addr.sin_addr);
            printf("%s\n", addr);
            wait(NULL);
            close(accepted);
        }
    }
}

int main(int argc, const char * argv[]) {
    signal(SIGTERM, process_sigint);
    signal(SIGINT, process_sigint);
    run();
    return 0;
}
