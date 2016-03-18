#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <time.h>
#include <sys/resource.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netinet/tcp.h>

https://github.com/DevinLow/epoll_cloud.git


#define PORT 13688
#define LISTENNUM 4096
#define MAX_FDS 4096

int server_listen(int sockfd)
{
	if(-1==listen(sockfd, LISTENNUM))
	{
		return -1;
	}
	return 0;
}

int create_socket(void)
{
	int sockfd;
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0)
	{
		perror("socket");
		return -1;
	}
	return sockfd;
}
int server_bind(int sockfd)
{
	struct sockaddr_in serveraddr;
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_addr.s_addr = INADDR_ANY;
	serveraddr.sin_port = htons(PORT); 
	serveraddr.sin_family = AF_INET;
	if (-1 == bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr))) // SOCKET_ERROR
	{
		return -1;
	}
	return 0;

}
int server_setopt(int sockfd)
{
	int err;
	int val = 1;
	int bufsize = 32 * 1024;
	err = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
	if (0 != err)
	{
		return -1;
	}
	err = setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, (char*)(&bufsize), sizeof(int));
	if (0 != err)
	{
		return -1;
	}

	val = 2; // 2 minitues
	err = setsockopt(sockfd, IPPROTO_TCP, TCP_DEFER_ACCEPT, &val, sizeof(val));
	if (0 != err)
	{
		return -1;
	}
	return 0;
}
int server_epoll_create(int *epoll_fd)
{
	*epoll_fd = epoll_create(MAX_FDS); // 10240
	if (0 >= *epoll_fd)
	{
		return -1;
	}
	return 0;
}
int main(void)
{
	int sockfd;
	int epoll_fd;
	sockfd=create_socket();
	if(sockfd==-1)
	{
		exit(1);
	}
	if(-1==server_setopt(sockfd))
	{
		exit(1);
	}
	if(-1==server_bind(sockfd))
	{
		exit(1);
	}
	if(-1==server_listen(sockfd))
	{
		exit(1);
	}
	if(-1==server_epoll_create(&epoll_fd))
	{
		exit(1);
	}
	return 0;
}
