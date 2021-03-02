/*************************************************************************
	> File Name: udp.c
	> Author: 
	> Mail: 
	> Created Time: Sat 13 Feb 2021 12:24:01 PM CST
 ************************************************************************/

#include "head.h"
extern int port;
extern char name[20];

int socket_create_udp(int port) {
    int listener;
    if ((listener = socket_udp()) < 0) {
        return -1;
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    int opt = 1;
    setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(listener, (struct sockaddr *)&server, sizeof(struct sockaddr_in)) < 0) {
        return -1;
    }

    return listener;
}

int socket_udp() {
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        return -1;
    }

    return sockfd;
}

int udp_connect(struct sockaddr_in *client) {
    int sockfd;
    if ((sockfd = socket_create_udp(port)) < 0) {
        return -1;
    }

    client->sin_port = htons(8888);
    if (connect(sockfd, (struct sockaddr *)client, sizeof(struct sockaddr)) < 0) {
        return -1;
    }
    return sockfd;
}

int udp_accept(int fd, struct User *user) {
    int new_fd, ret;
    struct sockaddr_in client;
    socklen_t len = sizeof(struct sockaddr_in);
    struct Msg request, response;
    bzero(&request, sizeof(struct Msg));
    bzero(&response, sizeof(struct Msg));

    ret = recvfrom(fd, (void *)&request, sizeof(struct Msg), 0, (struct sockaddr *)&client, &len);

    if (ret != sizeof(struct Msg)) {
        response.type = CHAT_FIN;
        strcpy(response.msg, "Login Request Error!");
        sendto(fd, (void *)&response, sizeof(struct Msg), 0, (struct sockaddr *)&client, len);
        DBG(BLUE"Size Error\n"NONE);
        return -1;
    }

    if (request.type & CHAT_FIN) {
        return -1;
    } else if ((request.type & CHAT_SYN) && (request.type & CHAT_ACK)) {
        DBG(GREEN"<Accepting>"NONE" : SYN ACK recrived from %s\n", request.from);
    } else if (request.type != CHAT_SYN) {
        response.type = CHAT_FIN;
        strcpy(response.msg, "Error Request!");
        sendto(fd, (void *)&response, sizeof(struct Msg), 0, (struct sockaddr *)&client, len);
        DBG(BLUE"type error!"NONE"\t[%s] : %s\n", request.from, request.msg);
        return -1;
    }

    DBG(GREEN"<Accepting>"NONE" [%s] : %s\n", request.from, request.msg);

    strcpy(user->name, request.from);
    user->flag = FL_ONLINE;
    user->cnt = 5;
    new_fd = udp_connect(&client);

    if (new_fd < 0) {
        perror("udp_connect");
        exit(EXIT_FAILURE);
    }

    user->fd = new_fd;
    char buff[512] = {0};
    sprintf(buff, "%s Login successfully\n", user->name);
    strcpy(response.msg, buff);
    strcpy(response.from, name);
    response.type = (CHAT_ACK | CHAT_SYN);
    send(new_fd, (void *)&response, sizeof(struct Msg), 0);
    return new_fd;
}
