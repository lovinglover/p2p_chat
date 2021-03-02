/*************************************************************************
	> File Name: send_chat.c
	> Author: 
	> Mail: 
	> Created Time: Sun 14 Feb 2021 10:58:43 PM CST
 ************************************************************************/

#include "head.h"

extern char name[20];
extern struct User *users;

void *send_chat(void *arg) {
    struct Msg msg;
    strcpy(msg.from, name);
    msg.type = CHAT_MSG;

    while (1) {
        memset(msg.msg, 0, sizeof(msg.msg));
        scanf("%[^\n]", msg.msg);
        getchar();
        for (int i = 0; i < MAX_USER; i++) {
            if (users[i].flag) {
                send(users[i].fd, &msg, sizeof(struct Msg), 0);
            }
        }
    }
} 
