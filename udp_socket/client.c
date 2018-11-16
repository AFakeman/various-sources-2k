#include <sys/socket.h>
#include <strings.h>
#include <arpa/inet.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int sockfd;
    char line_to[16] = "hello";
    char line_in[16];
    struct sockaddr_in saddr, caddr;
    if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
    }
    bzero(&caddr, sizeof(caddr));
    caddr.sin_family = AF_INET;
    caddr.sin_port = htons(0);
    caddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sockfd, (struct sockaddr*) &caddr, sizeof(caddr)) < 0) {
        perror("bind");
    }
    bzero(&saddr, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(51000);
    if (inet_aton(argv[1], &saddr.sin_addr) < 0) {
        perror("inet_aton");
    }
    if (sendto(sockfd, line_to, 16, 0, &saddr, sizeof(saddr)) < 0) {
        perror("sendto");
    }
    if (recvfrom(sockfd, line_in, 16, 0, NULL, NULL) < 0) {
        perror("recvfrom");
    }
    puts(line_in);
}
