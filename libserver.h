#include "lib_net.h"
#include "lib_thread.h"
#include "lib_log.h"
#include <event.h>

#define IPADDR "127.0.0.1"
#define PORT 3333
#define LOGPATH "./log/"
#define CONFPATH "./config/"
typedef struct st_client
{
	char name[20];
	struct sockaddr_in cli_addr;
	int cli_fd;
	pthread_t cli_pid;
	struct event *cli_event;
	struct base *cli_base;
}client;

typedef struct st_server
{
	int serv_fd;
	struct sockaddr_in s
    struct event *serv_event;
	struct base *serv_base
}server;

void readconfig(const char *config, );
