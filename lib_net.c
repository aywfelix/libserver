#include "lib_net.h"

int lib_udpsrv_init(const char *ipaddr, int port)
{
	int sock = -1, ret = 0;
	struct sockaddr_in serv_addr;
	if(ipaddr == NULL  || port <= 0)
		return -1;
	if((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
		return -2;
	//bzero(&serv_addr, sizeof(serv_addr));
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	//serv_addr.sin_addr.s_addr = inet_addr(ipaddr);
	//serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	inet_pton(AF_INET, ipaddr, &(serv_addr.sin_addr));
	serv_addr.sin_port = htons(port);

	if((ret = bind(sock, (struct sockaddr*)&serv_addr, sizeof(struct sockaddr))) < 0)
	{
		close(sock);
		return -3;
	}

	return sock;
}

int lib_udpsrv_free(int sock)
{
	if(sock <= 0)
		return -1;
    else
		return (close(sock) == 0)?0:-2;
}

int lib_udpclient_init(const char *ipaddr, int port, struct sockaddr_in *serv_addr)
{
	int ret = 0;
	int sock = -1;
	if(ipaddr == NULL || NULL == serv_addr || port <= 0)
		return -1;
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr->sin_family = AF_INET;
	serv_addr->sin_port = htons(port);
	if((ret = inet_pton(AF_INET, ipaddr, &(serv_addr->sin_addr)))<0)
		return -2;
	if((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
		return -3;
	else
		return sock;
}

int lib_udp_recv(int sock, void *recvbuf, int len, struct sockaddr *from, int *addrlen)
{
	int ret = 0;
	if(sock <= 0|| recvbuf == NULL || len <= 0 || from == NULL || addrlen== NULL)
		return -1;
	//int recvfrom(int s, void *buf, int len, unsigned int flags, struct sockaddr *from, int *fromlen);
	ret = recvfrom(sock, recvbuf, len, 0, from, addrlen);
	return (ret >0) ? ret : -2;
}

int lib_udp_snd(int sock, const void *sendbuf, int len, const struct sockaddr *to, int addrlen)
{
	if(sock <=0 || NULL == sendbuf || len <=0 || NULL == to || 0 == addrlen)
		return -1;
	//int sendto(int s, const void * msg, int len, unsigned int flags, const struct sockaddr * to, int tolen);
	int ret = sendto(sock, sendbuf, len, 0, to, addrlen);
	return (ret > 0) ? ret : -2;
}

//tcp

int lib_tcpsrv_init(const char *ipaddr, int port)
{
   	int ret = 0, recvbuf = 128*1024, sendbuf = 128*1024;
	int listen_sock = 0;
	struct sockaddr_in serv_addr;
	int readdr = 1;
	struct linger ling;
	ling.l_linger = 0;
	ling.l_onoff = 1;
	if(NULL == ipaddr || port <= 0)
		return -1;
	listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(listen_sock < 0)
		return -2;
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(ipaddr);
	serv_addr.sin_port = htons(port);

	setsockopt(listen_sock, SOL_SOCKET,SO_REUSEADDR, &readdr, sizeof(int));
	setsockopt(listen_sock, SOL_SOCKET,SO_RCVBUF, &recvbuf, sizeof(int));
	setsockopt(listen_sock, SOL_SOCKET, SO_SNDBUF, &sendbuf, sizeof(int));
    setsockopt(listen_sock, SOL_SOCKET, SO_LINGER, (const char *)&ling, sizeof(struct linger));

	ret = bind(listen_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if(ret < 0)
		return -3;
	ret = listen(listen_sock, 0);
	if(ret < 0)
		return -4;
	
	return listen_sock;
}

int lib_tcpclient_init()
{
	int sock = 0;
	struct linger ling;
	ling.l_linger = 0;
	ling.l_onoff = 1;
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0)
		return -1;
	setsockopt(sock, SOL_SOCKET, SO_LINGER, (const char *)&ling, sizeof(ling));
	return sock;
}

int lib_tcpclient_connet(int sock, const char *ipaddr, int port)
{
	int ret = 0;
	if(sock <=0 || NULL == ipaddr || port <=0)
		return -1;
	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_addr.s_addr = inet_addr(ipaddr);
	ret = connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

	return (ret < 0) ? ret : 0;
}

int lib_tcpsrv_accept(int listensock, struct sockaddr_in *cli_addr)
{
	int cli_sock = 0;
	int len = sizeof(cli_addr);
	cli_sock = accept(listensock, (struct sockaddr*)cli_addr, &len);
    //int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
	return (cli_sock < 0) ? -1 : cli_sock;
}

int lib_tcp_send(int sock, char *sendbuf, int len)
{
	if(sock <= 0 || NULL == sendbuf || len <=0)
		return -1;
	int ret =0;
	int data_left = len;
	int send_tol = 0;
	while(data_left > 0)
	{
		ret = send(sock, sendbuf + send_tol, data_left, 0);
        if(ret < 0)
		{
			if(errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)
			{
				usleep(100000);
				ret = 0;
			}
				
		}
		send_tol += ret;
		data_left = len - send_tol;
   	}
	return send_tol;
}

int lib_tcp_recv(int sock, char *recvbuf, int len, int timeout)
{
	if(sock <= 0 || NULL == recvbuf || len <=0 )
		return -1;
	int ret = 0;
	fd_set fds;
	struct timeval interval;
   	memset(recvbuf ,0 ,sizeof(recvbuf));
	FD_ZERO(&fds);
	FD_SET(sock, &fds);
    if(timeout < 0)
	{
		ret = select(sock+1, &fds, NULL,NULL,NULL);
		if(FD_ISSET(sock, &fds))
		{
			ret = recv(sock, recvbuf, len, 0);
		}
   	}
	else
	{
		interval.tv_sec = timeout;
		interval.tv_usec = 0;
		ret = select(sock+1, &fds, NULL, NULL, &interval);
		if(FD_ISSET(sock, &fds))
		{
			ret = recv(sock, recvbuf, len, 0);
		}
	}

	return (ret > 0)?ret :-2;
 }

int lib_tcp_free(int sock)
{
	int ret = 0;
	if(sock <= 0)
		return -1;
	ret = close(sock);
	return (ret==0) ? 0 : -1;
}

int lib_set_nonblock(int sock)
{
	int old_fd = fcntl(sock, F_GETFL);
        if(old_fd < 0)
          return -1;
        if(fcntl(sock, F_SETFL, old_fd|O_NONBLOCK) < 0)
          return -1;

        return old_fd;                 
}
