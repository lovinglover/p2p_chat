/*************************************************************************
	> File Name: reactor.h
	> Author: 
	> Mail: 
	> Created Time: Sat 13 Feb 2021 04:18:33 PM CST
 ************************************************************************/

#ifndef _REACTOR_H
#define _REACTOR_H

void add_event_ptr(int fd, int event, struct User *user);
void add_to_sub_reactor(struct User *user);
void del_from_reactor(int fd);
void *reactor(void *arg);

#endif
