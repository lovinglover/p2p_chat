/*************************************************************************
	> File Name: head.h
	> Author: 
	> Mail: 
	> Created Time: Sat 13 Feb 2021 12:17:05 PM CST
 ************************************************************************/

#ifndef _HEAD_H
#define _HEAD_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <ncursesw/curses.h>
#include <locale.h>

#include "tcp.h"
#include "color.h"
#include "Wechat.h"
#include "udp.h"
#include "common.h"
#include "reactor.h"
#include "client_discover.h"
#include "heart_beat.h"
#include "send_chat.h"

#ifdef _D
#define DBG(fmt, args...) printf(fmt, ##args)
#else
#define DBG(fmt, args...)
#endif

#endif
