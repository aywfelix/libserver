#include <unistd.h>  //getpagesize(  )
#include <sys/ipc.h>
#include <sys/shm.h>

#define LEN 256
struct share_s
{
    int writer; //0可写1表示可读
	char text[LEN];
};
//flag 默认为 IPC_CREAT |IPC_EXCL |0666
int lib_shm_init(const char *path, int id, size_t size, int shmflag);
void lib_shm_mat(int shmid, void *shmaddr);
int lib_shm_del(char *shmaddr);
// IPC_STAT        得到共享内存的状态
// IPC_SET         改变共享内存的状态
// IPC_RMID        删除共享内存
int lib_shm_ctl(int shmid, int cmd);
int lib_shm_free(int shmid,void *shmaddr);
