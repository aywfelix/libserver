/*
 * Filename:     lib_file.c
 *                
 * Version:       0.1
 * Author:        LaiJia <laijia2008@126.com>
 * Created at:    Tue Oct 14 20:07:33 2014
 *                
 * Description:    文件处理相关函数
 *
 */

#include "lib_file.h"
#define BUFFER_SIZE 1024
/********************************* 
 * Function Name: lib_file_isexist
 * Function description: find file is exist
 * Author: laijia
 * Input parameter: file full path 
 *
 * Return value: 
 *
 **********************************/
int lib_file_isexist(const char *strfilepath)
{
	if(NULL == strfilepath || strcmp(strfilepath,"")==0)
		return -1;
	if(access(strfilepath,0) == 0)
		return 0;
	else
		return -2;
}
/********************************* 
 * Function Name: lib_file_create
 * Function description: create the file
 * Author: laijia
 * Input parameter: file full path 
 *
 * Return value: 
 *
 **********************************/
int lib_file_create(const char *strfilepath)
{

	if(NULL == strfilepath || strcmp(strfilepath,"") == 0)
		return -1;
	if(access(strfilepath,0) == 0)
		return -2;
	else
	{
		FILE *fp = fopen(strfilepath,"wb");
		if(NULL == fp)
			return -3;

		fclose(fp);
	}
	return 0;
	
}
/********************************* 
 * Function Name: lib_file_delete
 * Function description:  delete file
 * Author: laijia
 * Input parameter: file full path
 *
 * Return value: 
 *
 **********************************/
int lib_file_delete(const char *strfilepath)
{
	if(NULL == strfilepath || strcmp(strfilepath,"") == 0)
		return -1;
	else if(lib_file_isexist(strfilepath) == 0)
	{
		remove(strfilepath);
		return 0;
	}
	return -2;
		
}
/********************************* 
 * Function Name: lib_file_rename
 * Function description: rename the file
 * Author: laijia
 * Input parameter:
 * strfileold --file old name
 * strfilenew --file new name
 * Return value: 
 *
 **********************************/
int lib_file_rename(const char *strfileold, const char *strfilenew)
{
	int ret = -1;
   	if(NULL != strfileold && NULL != strfilenew && strcmp(strfileold,"") != 0 && strcmp(strfilenew, "") != 0)
	{
		ret = rename(strfileold, strfilenew);
		if(ret == 0)
		{
			return 0;
		}
		
	}
	return ret;
}

//get the file size
long lib_file_filesize(FILE *fd)
{
	long curpos,length;
	curpos = ftell(fd);
	fseek(fd, 0L,SEEK_END);
	length = ftell(fd);
	fseek(fd,curpos, SEEK_SET);

	return length;
}

/********************************* 
 * Function Name: lib_file_copy
 * Function description: file copy
 * Author: laijia
 * Input parameter: 
 *    strfilesrc :file source path
 *    strfiledest :file dest path
 * Return value: 
 *
 **********************************/
int lib_file_copy(const char *strfilesrc, const char *strfiledest)
{
	FILE *fd_from = NULL;
	FILE *fd_to = NULL;
	char tmpbuf[BUFFER_SIZE];
	int read_num = 0,write_num = 0;
	long filesize = 0;
	if((NULL != strfilesrc) && (NULL != strfiledest) && (strcmp(strfilesrc,"") != 0) && (strcmp(strfiledest, "") != 0))
	{
		fd_from = fopen(strfilesrc,"rb");
		fd_to = fopen(strfiledest,"wb");
		if(fd_from == NULL || fd_to == NULL)
		{
			return -1; //open file failure
		}
        filesize = lib_file_filesize(fd_from);
		printf("read the file size is %ld\n",filesize);
  		while(!feof(fd_from))
		{
			if((read_num = fread(tmpbuf,sizeof(char), BUFFER_SIZE, fd_from)) < 0)
			{
				fprintf(stderr,"read error\n");
				return -1;
			}
			if(BUFFER_SIZE > filesize)
			{
				if((write_num = fwrite(tmpbuf,sizeof(char),filesize,fd_to)) != filesize)
				{
					fprintf(stderr,"write error\n");
					return -1;
				}
			}
			else
			{
				if((write_num = fwrite(tmpbuf, sizeof(char),BUFFER_SIZE,fd_to)) != BUFFER_SIZE)
				{
					fprintf(stderr,"write error\n");
					return -1;
  				}

				filesize = filesize - BUFFER_SIZE;
			}

			bzero(tmpbuf,BUFFER_SIZE);
		}
		
	}
	fclose(fd_from);
	fclose(fd_to);
	return 0;
}

int lib_file_getfilesize(const char *strfilepath)
{
	struct stat fd_stat;
	if(stat(strfilepath, &fd_stat) == -1)
	{
		return -1;
	}
	return (int)fd_stat.st_size;
}
/********************************* 
* Function Name: lib_file_writecfg
* Function description: fullfill the simple to write configure file
* Author: laijia
* Input parameter: 
*    strfilepath : the file path
*    strsection : the section //配置项
*    strkey : the key //关键字
*    value                 
* Return value: 
*
**********************************/
int lib_file_writecfg(const char *strfilepath,const char *strsection, const char *strkey, const char *value)
{
	FILE *fp = NULL;
	char tempbuf[100],secbuf[100];
	if(NULL == strfilepath || strcmp(strfilepath, "") == 0)
	{
		return -1;
	}
	else
	{
		if(lib_file_isexist(strfilepath) != 0 )
		{
			lib_file_create(strfilepath);

			if( (fp = fopen(strfilepath,"wb")) == NULL)
			{
				fclose(fp);
			   	fprintf(stderr,"open wrong\n");
				return -1;
			}
  			snprintf(tempbuf,100,"[%s]\n", strsection);
			fputs(tempbuf,fp);
			bzero(tempbuf,BUFFER_SIZE);
	 		sprintf(tempbuf,"%s=%s\n",strkey,value);
			fputs(tempbuf,fp);
			fclose(fp);
			return 0;
		}//create the configure file
        else
		{
			fp = fopen(strfilepath,"rw+");
			while(!feof(fp))
			{
				fgets(secbuf,100, fp);
				if(strstr(secbuf,strsection) != NULL) //the section is exist
				{
					while(!feof(fp))
					{
   						fgets(secbuf,100, fp);
						if(strstr(secbuf,strkey) != NULL)
						{
							return -2; //the key is exist
						}
						else
						{
							sprintf(tempbuf,"%s=%s\n",strkey,value);
							fputs(tempbuf,fp);
							fclose(fp);
							return 0;   	
						}
				
					}
				}
				else
				{
					snprintf(tempbuf,100,"[%s]\n", strsection);
					fputs(tempbuf,fp);
					bzero(tempbuf,sizeof(tempbuf));
					sprintf(tempbuf,"%s=%s\n",strkey,value);
					fputs(tempbuf,fp);
					fclose(fp);
					return 0;
		
				}
   	     	}
			fclose(fp);
			return 0;
		}
	}
}
/********************************* 
* Function Name: lib_file_readcfg
* Function description: fullfill read confugure file to get the value
* Author: laijia
* Input parameter: 
*
* Return value: 
*
**********************************/
int lib_file_readcfg(const char *strfilepath,const char *strsection, const char *strkey, char *value)
{
	char tmpbuf[100];
	FILE *fp = NULL;
	if(NULL == strfilepath || strcmp(strfilepath,"") == 0)
	{
		return -1;
	}

	if((fp = fopen(strfilepath, "rb")) == NULL)
	{
		return -2;
	}

	while(!feof(fp))
	{
		fgets(tmpbuf,100,fp);
		if(strstr(tmpbuf, strsection) != NULL)
		{

			while(!feof(fp))
			{
				fgets(tmpbuf, 100, fp);
  				if(strstr(tmpbuf,strkey) == tmpbuf && (tmpbuf[strlen(strkey)] == '='))
				{
					strcpy(value, tmpbuf + strlen(strkey) + 1);
				}
                fclose(fp);
                return 0;	
			}
					
		}
	}
	return -3;
}


void  lib_file_appendstring ( char** pDest, const char* pSrc )
{
	int		iEnd;
	char	*pString ;

	pString = *pDest;
	/* if not adding a string, return */
	if ( pSrc == (char*)NULL || strlen ( pSrc ) == 0 )
	{
		return ;
	}
	
	/* check src */
	if ( pString == (char*)NULL )
	{
		pString = (char*) malloc ( strlen(pSrc)+10 );/* allocate and copy */
		iEnd = 0;
	}
	else 
	{
		iEnd = strlen(pString );
		pString = malloc(iEnd+strlen(pSrc)+10);
		memcpy(pString,*pDest,iEnd);
		pString[iEnd]=0;
		free(*pDest);
		*pDest = NULL;
	}
    
	if(pString !=NULL)
	{
		memcpy(pString+iEnd,pSrc,strlen(pSrc));
		pString[strlen(pSrc)+iEnd]=0;
		*pDest = pString;
	}
	return ;
}

/*******************************************************************
			读取文件内容进入内存
函数声明：int	ictp_f_readfile(char *filename,char *buf,int flag,int length)
函数说明：根据flag变量内容将文件内容读取到指定的内存中
参数说明：
	filename：要读取文件的名称，该名称包含文件的绝对路径
	buf		：保存读取出来的数据，函数内部分配空间，外部释放。
	flag	：读取方式	0：全部读取，length无效
						1：根据length指定的大小从文件起始出读取。
	length	：配合flag使用
返回结果：

*******************************************************************/
int  lib_file_readfile(char *filename,char **buf,int flag,int length)
{
	int		fd = 0;
	char	*line=NULL;
	int		readlen=0;
	int		total_read = 0;
	int		filesize = 0;

	if(*buf!=NULL)
	{
		strcpy(*buf,"");
	}

	filesize = (int) lib_file_getfilesize(filename);
	if(filesize <= 0 ||filesize < length)
		return 0;

	line = (char*)malloc(filesize+10);
	fd = open(filename,O_RDWR);
	if(fd < 0)
	{
		free(line);
		return	0;
	}
	
 	while(1)
 	{
		if(flag == 1)
		{
			readlen = read(fd,line+total_read,length-total_read);
			if(readlen>0)
			{
				total_read = total_read + readlen;
			}
			else break;
		}
		else
		{
			readlen = read(fd,line+total_read,filesize-total_read);
			if(readlen>0)
			{
				total_read = total_read + readlen;
			}
			else break;
		}
	}
    	
    close(fd);
	
	if(readlen<0)
	{
		free(line);
		return 0;
	}
	lib_file_appendstring(buf,line);
	free(line);
	return 1;
}

int	lib_file_getxmlnode(char *outbuffer, char *inbuffer, int i, char *delim,char *end)
{
	int			j = 0;
	int			k = 0;
	char		*token;
	char		*tmpbuf;
	char		*blanktoken;
	int			len = 0;
	int			delim_len =0 ;
	int			end_len = 0;
	int			blank_len = 0;
	int			search_flag = 0;

	strcpy ( outbuffer, "" );
	
	tmpbuf = ( char * ) malloc ( strlen(inbuffer) + 1 );
	if(tmpbuf == NULL)	return 0;

	blanktoken = (char*)malloc(strlen(delim) +10);

	strcpy( tmpbuf, inbuffer);
	len = strlen( tmpbuf );
	delim_len = strlen( delim );
	end_len = strlen(end);
	strcpy(blanktoken , delim);
	strcpy(blanktoken+strlen(delim)-1," />");
	blank_len = strlen(blanktoken);

	for(j = 0; j < len;j++)
	{
		if(	memcmp(tmpbuf+j,end,end_len)!=0 &&		
			memcmp(tmpbuf+j,blanktoken,blank_len)!=0)
			continue;
		
		k++;
		if(k!=i) continue;
		search_flag = 1;
		break;		
	}

	if(search_flag!=1)
	{
		strcpy(outbuffer,"");
		free(tmpbuf);
		free(blanktoken);
		return 0;
	}
	if(memcmp(tmpbuf+j,blanktoken,blank_len)==0)
	{
		free(tmpbuf);
		free(blanktoken);
		strcpy(outbuffer,"");
		return 1;
	}

	tmpbuf[j] = 0;
	
	//token = tmpbuf+j+delim_len;
	search_flag = 0;
	i = 0;
	len = strlen(tmpbuf);
	for(j = len;j>0;j--)
	{
		if(memcmp(tmpbuf+j,delim,delim_len)!=0)
		{
			i++;
			if(tmpbuf[j] == '\t'||tmpbuf[j]==0x0d||tmpbuf[j]==0x0a)
				tmpbuf[j] = ' ';
			continue;
		}
		search_flag = 1;
		break;
	}
	
	if(search_flag !=1)	
	{
		strcpy(outbuffer,"");
		free(tmpbuf);
		return 0;
	}
	token = tmpbuf+j+delim_len;
	memcpy(outbuffer,token,i-delim_len);
	outbuffer[i-delim_len]=0;
		
	free( tmpbuf );
	free(blanktoken);
	return	1;
}

