/*************************************************************************
	> File Name: reactor.c
	> Author: 
	> Mail: 
	> Created Time: Sun 14 Feb 2021 07:57:16 PM CST
 ************************************************************************/

#include "head.h"
extern struct User *users;
extern int subfd;

void add_to_sub_reactor(struct User *user) {
    int fd = user->fd;
    users[fd] = *user;
    add_event_ptr(users[fd].fd, EPOLLIN | EPOLLET, &users[fd]);
    return ;
}

void add_event_ptr(int fd, int event, struct User *user) {
    struct epoll_event ev;
    ev.events = event;
    ev.data.ptr = (void *)user;
    epoll_ctl(subfd, EPOLL_CTL_ADD, fd, &ev);
    return ;
}

void del_from_reactor(int fd) {
    epoll_ctl(subfd, EPOLL_CTL_DEL, fd, NULL);
    return ;
}

void *reactor(void *arg) {
    struct epoll_event events[20];
    struct Msg msg;

    while (1) {
        int nfds = epoll_wait(subfd, events, 20, -1);
        if (nfds < 0) {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < nfds; i++) {
            int fd = ((struct User *)(events[i].data.ptr))->fd;
            recv(fd, &msg, sizeof(struct Msg), 0);
            users[fd].cnt = 5;

            DBG(L_RED"\nreceived msg:\ntype : %03x\nfrom : %s\nmsg : %s"NONE"\n", msg.type, msg.from, msg.msg);

            if (msg.type & CHAT_ACK && msg.type & CHAT_SYN) {
                continue;
            } else if (msg.type & CHAT_HEART) {
                DBG(RED"Receive heart beat from %s"NONE"\n", users[fd].name);
                msg.type = CHAT_ACK;
                send(fd, &msg, sizeof(struct Msg), 0);
            } else if (msg.type & CHAT_ACK) {
                DBG(RED"Receive heart ACK from %s"NONE"\n", users[fd].name);
            } else if (msg.type & CHAT_FIN) {
                DBG(BLUE"<%s>"NONE" is offline and will be removed by reactor\n", users[fd].name);
                del_from_reactor(fd);
                DBG(BLUE"<%s>"NONE" is removed by reactor\n", users[fd].name);
                users[fd].flag = 0;
                close(fd);
            } else if (msg.type & CHAT_MSG) {
                DBG("<%s> : %s\n", msg.from, msg.msg);
            } else {
                DBG(RED"chat type error in reactor\n"NONE);
            }
        }
    }
}
