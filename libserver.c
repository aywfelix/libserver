#include "libserver.h"
int main()
{
	lib_write_log(LOGPATH, "the server start...\n");
	struct st_sysserv sysserv;
	readconfig(&sysserv);
	source_init(&sysserv);
	sysserv.serv.serv_fd = lib_tcpsrv_init(sysserv.server.serv_ip, sysserv.server.serv_port);
	if(sysserv.serv.serv_fd < 0)
	{
		return -1;
	}
	lib_set_nonblock(sysserv.server.serv_fd);
	struct event listen_ev;
	struct st_dispatch dispatch;
	dispatch->base = event_base_new();
    event_set(&listen_ev,sysserv.server.serv_fd,EV_READ|EV_PERSIST, accept_cb, &sysserv);
	event_base_set(dispatch->base, &listen_ev);
	event_add(&listen_ev,NULL);
	event_dispatch(dispatch->base);
    return 0;
}


void readconfig(struct st_sysserv *sys_serv)
{
	char tmpbuf[50];
	memset(tmpbuf,0,sizeof(tmpbuf));
	lib_file_readcfg("server","ip","127.0.0.1",sys_serv->server.serv_ip, 24, CONFFILE);
    memset(tmpbuf,0, sizeof(tmpbuf));
	lib_file_readcfg("server","port","9999",tmpbuf, 4, CONFFILE);
	sys_serv->server.serv_port = atoi(tmpbuf);
	memset(tmpbuf, 0, sizeof(tmpbuf));
	lib_file_readcfg("server","thrd_num","10", tmpbuf, 2, CONFFILE);
	sys_serv->thrd_num = atoi(tmpbuf);
		
}

void accept_cb(int fd, short event, void *arg)
{
	struct st_client *client;
    struct st_sysserv *sysserv = (struct st_sysserv*)arg;
	client->cli_fd = lib_tcpsrv_accept(sysserv->server.serv_fd, &(client->cli_addr));
	if(client->cli_fd < 0)
		return -1;
	else
		lib_write_log(LOGFILE, "[%s]recv new connection{%s:%d}\n", lib_time_now(), inet_ntoa(client->cli_addr), ntohs(client->cli_addr.port));
	//保存客户端信息
	
}

int resource_init(struct st_server *server)
{
	return 0;
}
