#include "lib_shm.h"

int lib_shm_init(const char *path, int id, size_t size, int shmflag)
{
	int shmid = 0;
	shmid = shmget(ftok(path, id), size, shmflag);
	return shmid;
}

void lib_shm_mat(int shmid, void *shmaddr)
{
	shmaddr = shmat(shmid, 0, 0);
}
//forbidden to use the memory
int lib_shm_del(char *shmaddr)
{
	int ret = 0;
	ret = shmdt(shmaddr);
	return (ret == 0) ? 0 : -1;
}
// IPC_STAT        得到共享内存的状态
// IPC_SET         改变共享内存的状态
// IPC_RMID        删除共享内存
int lib_shm_ctl(int shmid, int cmd)
{
	int ret = 0;
	struct shmid_ds *buf;
	ret = shmctl(shmid, cmd, buf);

	return (ret == 0) ? 0 : -1;
}

int lib_shm_free(int shmid,  char *shmaddr)
{
	int ret = 0;
	ret = shmdt(shmaddr);
	if(ret != 0)
		return -1;
	ret = shmctl(shmid, IPC_RMID, NULL);
}

