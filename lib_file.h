/*
 * Filename: lib_file.h
 *                
 * Version:       0.1
 * Author:        LaiJia <laijia2008@126.com>
 * Created at:    Tue Oct 14 20:05:17 2014
 *                
 * Description:   文件处理相关头文件   
 *
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
int lib_file_isexist(const char *strfilepath);
int lib_file_create(const char *strfilepath);
int lib_file_delete(const char *strfilepath);
int lib_file_rename(const char *strfileold, const char *strfilenew);
long lib_file_filesize(FILE *fd);
int lib_file_copy(const char *strfilesrc, const char *strfiledest);
int lib_file_getfilesize(const char *strfilepath);
int lib_file_writecfg(const char *strfilepath,const char *strsection, const char *strkey, const char *value);
int lib_file_readcfg(const char *strfilepath,const char *strsection, const char *strkey, char *value);
void  lib_file_appendstring ( char** pDest, const char* pSrc );
int  lib_file_readfile(char *filename,char **buf,int flag,int length);
int	lib_file_getxmlnode(char *outbuffer, char *inbuffer, int i, char *delim,char *end);











