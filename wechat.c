/*************************************************************************
	> File Name: wechat.c
	> Author: 
	> Mail: 
	> Created Time: Sat 13 Feb 2021 03:15:24 PM CST
 ************************************************************************/

#include "head.h"
const char *config = "./chat.conf";
char name[20], msg[512];
int port, subfd;
struct User *users;

void init(int argc, char **argv) {
    int opt;
    while ((opt = getopt(argc, argv, "p:n:m:")) != -1) {
        switch (opt) {
            case 'p': port = atoi(optarg); break;
            case 'm': strcpy(msg, optarg); break;
            case 'n': strcpy(name, optarg); break;
            default: {
                fprintf(stderr, "Usage : %s -n name -p port -m msg\n", argv[0]);
                exit(EXIT_FAILURE);
            }
        }
    }

    if (!port) {
        char tmp[20];
        get_value(config, "port", tmp);
        port = atoi(tmp);
    }
    if (!strlen(name)) get_value(config, "name", name);
    if (!strlen(msg)) get_value(config, "msg", msg);
    DBG(BLUE "<starting...>" NONE "name = %s, port = %d, msg = %s\n", name, port, msg);
}

void logout(int signum) {
    struct Msg logout_msg;
    logout_msg.type = CHAT_FIN;
    strcpy(logout_msg.from, name);
    strcpy(logout_msg.msg, "Bye!");
    for (int i = 0; i < MAX_USER; i++) {
        if (users[i].flag & FL_ONLINE) {
            send(users[i].fd, &logout_msg, sizeof(struct Msg), 0);
        }
    }
    DBG(RED"Logout from chat system...\n"NONE);
    exit(EXIT_SUCCESS);
}

int main(int argc, char **argv) {
    init(argc, argv);
    signal(SIGINT, logout);
    int listener, epollfd, newfd;
    pthread_t discover_tid, heart_beat_tid, reactor_tid, send_tid;
    
    if ((listener = socket_create_udp(port)) < 0) {
        perror("socket_create_udp");
        exit(EXIT_FAILURE);
    }
    DBG(BLUE"<starting...>"NONE "listener fd=%d ok\n", listener);

    if ((epollfd = epoll_create(1)) < 0) {
        perror("epoll_create");
        exit(EXIT_FAILURE);
    }
    if ((subfd = epoll_create(1)) < 0) {
        perror("epoll_create");
        exit(EXIT_FAILURE);
    }

	pthread_create(&discover_tid, NULL, client_discover, NULL);
    pthread_create(&heart_beat_tid, NULL, heart_beat, NULL);
    pthread_create(&reactor_tid, NULL, reactor, NULL);
    pthread_create(&send_tid, NULL, send_chat, NULL);
    
    struct epoll_event ev, events[MAX_N];
    ev.events = EPOLLIN;
    ev.data.fd = listener;
    users = (struct User *)calloc(MAX_USER, sizeof(struct User));

    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listener, &ev) < 0) {
        perror("epoll_ctl");
        exit(EXIT_FAILURE);
    }

    while (1) {
        int nfds = epoll_wait(epollfd, events, MAX_N, -1);
        DBG(RED"<Main Reactor>"NONE" : Got events...\n");
        if (nfds < 0) {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < nfds; i++) {
            struct User new_user;
            if ((newfd = udp_accept(listener, &new_user)) < 0) {
                fprintf(stderr, "error in udp_accept\n");
                continue;
            }
            add_to_sub_reactor(&new_user);
            DBG(RED"<Main Reactor>"NONE" : Adding to sub reactor\n");
        }
    }
    return 0;
}
