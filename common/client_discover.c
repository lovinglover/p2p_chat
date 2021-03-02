/*************************************************************************
	> File Name: client_discover.c
	> Author: 
	> Mail: 
	> Created Time: Sun 14 Feb 2021 08:09:44 PM CST
 ************************************************************************/

#include "head.h"
extern char *config;
extern char name[20];
extern char msg[512];

void *client_discover(void *arg) {
    char file[512] = {0};
    FILE *fp = NULL;
    struct Msg request;
    struct sockaddr_in client;
    int sockfd;

    if ((sockfd = socket_udp()) < 0) {
        perror("socket_udp");
        exit(EXIT_FAILURE);
    }

    request.type = CHAT_SYN;
    strcpy(request.from, name);
    strcpy(request.msg, msg);
    get_value(config, "iplist", file);

    if ((fp = fopen(file, "r")) == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    DBG(YELLOW"<IP Discover> starting\n"NONE);
    client.sin_family = AF_INET;
    client.sin_port = htons(8888);
    while (!feof(fp)) {
        char ip[20] = {0};
        fscanf(fp, "%s", ip);
        if (!strlen(ip)) continue;

        client.sin_addr.s_addr = inet_addr(ip);
        sendto(sockfd, (void *)&request, sizeof(struct Msg), 0, (struct sockaddr *)&client, sizeof(struct sockaddr_in));
        DBG("<Discover send to %s> : I'm %s, msg is %s\n", ip, request.from, request.msg);
    }
    DBG(YELLOW"<IP Discover> end\n"NONE);
}
