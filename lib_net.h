#include<sys/socket.h>
#include<sys/un.h>
#include<netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
//udp
int lib_udpsrv_init(const char *ipaddr, int port);
int lib_udpsrv_free(int sockfd);
int lib_udpclient_init(const char *ipaddr, int port, struct sockaddr_in *serv_addr);
int lib_udp_recv(int sock, void *recvbuf, int len, struct sockaddr *from, int *addrlen);
int lib_udp_snd(int sock, const void *sendbuf, int len, const struct sockaddr *to, int addrlen);
//tcp
int lib_tcpsrv_init(const char *ipaddr, int port);
int lib_tcpclient_init();
int lib_tcpclient_connet(int sock, const char *ipaddr, int port);
int lib_tcpsrv_accept(int listensock, struct sockaddr_in *cli_addr);
int lib_tcp_send(int sock, char *sendbuf, int len);
int lib_tcp_recv(int sock, char *recvbuf, int len, int timeout);
int lib_tcp_free(int sock);

int lib_set_nonblock(int sock);

