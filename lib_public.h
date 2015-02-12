#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>

int lib_time_now(char *outtime, int flag);
unsigned short lib_crc_check(unsigned char *start, unsigned int len);
void lib_pwd(char *filepath, int len);
void lib_write_log(const char *logpath, char *format,...);
//field :代表记录
//sep ：分割符
//origin ： 原始字符串
// a\vb\vc\v field[0] = a field[1] = b ...  
int lib_separate_record(char **field,char *out, char sepsection, char seprecord,char *origin);
void lib_get_record(char *out, char *in, int num, char *esc);
//将返回执行的一条结果
void lib_exec_cmd(const char *cmd, char *result);
//利用ping测试网络
int lib_test_net(const char *ipaddr);












