#include "lib_public.h"

char* lib_time_now(char *outtime, int flag)
{
	time_t now_t;
	struct tm *now_tm;
	if(NULL == outtime)
	   return NULL;
	now_t = time(NULL);
	now_tm = localtime(&now_t);

	if(0 == flag)
		strftime(outtime, 20, "%Y-%m-%d %H:%M:%S", now_tm);
	else
		strftime(outtime, 15, "%Y%m%d%H%M%S", now_tm);
	return outtime;
}
// Description :	计算CRC,多项式为G(x)＝x16＋x12＋x5＋1, 按半子节计算
//	* input : 		*check_start_acp:	待校验数据的起始指针
//	*			check_len_ai：		校验的长度（以字节为单位）

unsigned short lib_crc_check(unsigned char *start, unsigned int len)
{
	unsigned short crcreturn = 0;
	unsigned char da;
	unsigned int crc_ta[16] = {
		0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
		0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef
	};

 	while(len--){
		da = ((unsigned char)(crcreturn / 256)) / 16;
		crcreturn <<= 4;
		crcreturn ^= crc_ta[da ^ (*start / 16)];
		da=((unsigned char)(crcreturn / 256)) / 16;
		crcreturn <<= 4;
		crcreturn ^= crc_ta[da ^ (*start & 0x0f)];
		start++;
	}

	return(crcreturn); 
}

void lib_pwd(char *filepath, int len)
{
	getcwd(filepath, len);
}

void lib_write_log(const char *logpath, char *format,...)
{
	FILE *fp;
	va_list args;
	fp = fopen(logpath, "a+");
	va_start(args, format);
	vfprintf(fp, format, args);
	va_end(args);
	fflush(fp);
	fclose(fp);
}

void lib_exec_cmd(const char *cmd, char *result)
{
	FILE *fp;
	fp = popen(cmd, "r");
	fread(result, sizeof(char), strlen(result)-1, fp);
   	pclose(fp);
}
void lib_get_record(char *out, char *in, int num, char *esc)
{
	int i, len = 0, esc_len = 0;
	char *head, *tail;

	esc_len = strlen(esc);
	head = in;
	
	for(i = 1; (tail = strstr(head, esc)) && i != num; i++)	head = tail + esc_len;
	len = tail - head;
	if (tail == NULL)	return -1;
	else	memcpy(out, head, len);
	out[len] = '\0';
}

int lib_test_net(const char *ipaddr)
{
	int ret = -1;
	char cmd[50]="ping ";
	strcat(cmd, ipaddr);
	strcat(cmd, " -c 1 > /dev/null");
	if(!system(cmd))
	   ret = 0;
	return ret;
}


