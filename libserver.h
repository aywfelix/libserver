#include "lib_net.h"
#include "lib_thread.h"
#include "lib_log.h"
#include <event.h>

#define IPADDR "127.0.0.1"
#define PORT 3333
#define LOGFILE "./log/libserver.log"
#define CONFFILE "./config/libserver.ini"
typedef struct st_client
{
	char name[20];
	struct sockaddr_in cli_addr;
	int cli_fd;
	pthread_t cli_pid;
	struct event *cli_event;
	struct event_base *cli_base;	
};

typedef struct st_server
{
	int serv_fd;
	char serv_ip[24];
	int serv_port;
    struct event *serv_event;
	struct event_base *serv_base
};

typedef struct st_sysserv
{
	struct st_server server;
    struct st_client_info *client;
	int thrd_num;
	
};

//存储客户端信息
typedef struct st_client_info
{
	int cli_max_num;
	int cli_info_curnum;
	struct st_client_info *next;
	pthread_mutex_t mutex;
};


typedef struct st_dispatch
{
	struct event_base *base;
};

void readconfig(struct st_system *system);
void timeout_cb(int fd, short event, void *arg);
void accept_cb(int fd, short event, void *arg);

