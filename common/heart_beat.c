/*************************************************************************
	> File Name: heart_beat.c
	> Author: 
	> Mail: 
	> Created Time: Sun 14 Feb 2021 09:53:43 PM CST
 ************************************************************************/

#include "head.h"

extern struct User *users;

void *heart_beat(void *arg) {
    struct Msg heart;
    heart.type = CHAT_HEART;
    while (1) {
        for (int i = 0; i < MAX_USER; i++) {
            if (users[i].flag) {
                if (users[i].cnt <= 0) {
                    DBG(RED"<%s>"NONE" has no repaly for five times and will be removed by heart_beat...\n", users[i].name);
                    users[i].flag = 0;
                    del_from_reactor(users[i].fd);
                    close(users[i].fd);
                    DBG(RED"<%s>"NONE" is removed by heart_beat", users[i].name);
                }

                send(users[i].fd, (void *)&heart, sizeof(struct Msg), 0);
                DBG(BLUE"send heart_beat to %s\n"NONE, users[i].name);
                users[i].cnt--;
            }
        }
        sleep(60);
    }
}
