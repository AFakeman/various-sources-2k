//
//  main.c
//  chat
//
//  Created by Anton Suslov on 23/05/2017.
//  Copyright © 2017 Anton Suslov. All rights reserved.
//

#include <stdio.h>
#include <poll.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MSG_SIZE 4096
#define POLL_SIZE 2

int main(int argc, const char * argv[]) {
    struct pollfd poll_struct[POLL_SIZE];
    int sockfd;
    int i;
    struct sockaddr_in client;
    struct sockaddr_in server;
    char message[MSG_SIZE];
    memset(&poll_struct, 0, sizeof(poll_struct));
    memset(&client, 0, sizeof(client));
    client.sin_port = htons(0);
    client.sin_addr.s_addr = htonl(INADDR_ANY);
    client.sin_family = AF_INET;
    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket");
        exit(EXIT_FAILURE);
    }
    poll_struct[0].fd = STDIN_FILENO;
    poll_struct[0].events = POLLIN;
    poll_struct[1].fd = sockfd;
    poll_struct[1].events = POLLIN;
    if (bind(sockfd, &client, sizeof(client)) < 0) {
        perror("Bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    if (inet_aton(argv[1], &server.sin_addr) < 0) {
        perror("Inet_aton");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));
    if (connect(sockfd, &server, sizeof(server))) {
        perror("Connect");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    for(;;) {
        if (poll(poll_struct, 2, -1) < 0) {
            perror("Poll:");
            close(sockfd);
            exit(EXIT_FAILURE);
        }
        if (poll_struct[0].revents & POLLIN) {
            if (fgets(message, sizeof(message), stdin) < 0) {
                perror("fgets");
            }
            if (message[0] == '\0') {
                printf("Bye!");
                exit(EXIT_SUCCESS);
            }
            if (write(sockfd, message, MSG_SIZE) < 0) {
                perror("Send");
            }
        }
        if (poll_struct[1].revents & POLLHUP) {
            printf("Bye!");
            exit(EXIT_SUCCESS);
        } else if (poll_struct[1].revents & POLLIN) {
            if (read(sockfd, message, MSG_SIZE) < 0) {
                perror("Receive");
            }
            fputs(message, stdout);
        }
    }
    return 0;
}
