/*************************************************************************
	> File Name: udp.h
	> Author: 
	> Mail: 
	> Created Time: Sat 13 Feb 2021 12:17:53 PM CST
 ************************************************************************/

#ifndef _UDP_H
#define _UDP_H

int socket_create_udp(int port);
int socket_udp();
int udp_accept(int fd, struct User *user);
int udp_connetc(struct sockaddr_in *client);

#endif
