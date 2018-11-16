#include <sys/socket.h>
#include <strings.h>
#include <arpa/inet.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int sockfd;
    char line_to[16], line_in[16];
    struct sockaddr_in saddr, caddr;
    size_t clen = sizeof(caddr);
    if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
    }
    bzero(&saddr, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(51000);
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sockfd, (struct sockaddr*) &saddr, sizeof(saddr)) < 0) {
        perror("bind");
    }
    if (recvfrom(sockfd, line_in, 16, 0, &caddr, &clen) < 0) {
        perror("recvfrom");
    }
    puts(line_in);
    line_in[0] = '1';
    if (sendto(sockfd, line_in, 16, 0, &caddr, sizeof(caddr)) < 0) {
        perror("sendto");
    }
}
