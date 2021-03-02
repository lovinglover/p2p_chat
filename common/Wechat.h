/*************************************************************************
	> File Name: Wechat.h
	> Author: 
	> Mail: 
	> Created Time: Sat 13 Feb 2021 12:19:16 PM CST
 ************************************************************************/

#ifndef _WECHAT_H
#define _WECHAT_H

#define CHAT_SYN 0x001
#define CHAT_ACK 0x002
#define CHAT_FIN 0x004
#define CHAT_HEART 0x008
#define CHAT_MSG 0X010

#define MAX_N 10
#define MAX_USER 1000

struct User {
    char name[20];
    int flag;
    int fd;
    int cnt;
};

#define FL_SYNSENT 0x001
#define FL_ONLINE 0x002

struct Msg {
    int type;
    char from[20];
    char to[20];
    char msg[1024];
};

#endif
