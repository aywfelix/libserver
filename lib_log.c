#include "lib_log.h"

typedef void (*log_print_cb)(int serverity, const char *msg);
void warn_help(int severity, const char *errstr, const char *fmt, va_list ap);
void internal_log(int serverity, const char *msg);
void log_set_cb(log_print_cb log_cb);
void errexit(int errcode) NORETURN;
void log_snprintf(char *buf, int buflen, const char *fmt, ...);

static log_print_cb log_fn = NULL;

void errexit(int errcode)
{
	exit(errcode);
}

void log_err(int eval, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	warn_help(LOG_ERR, strerror(errno), fmt, ap);
	va_end(ap);
	errexit(eval);
}

void log_warn(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	warn_help(LOG_WARN, strerror(errno), fmt, ap);
	va_end(ap);
	
}

void log_errx(int eval, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	warn_help(LOG_ERR, NULL, fmt, ap);
	va_end(ap);
	errexit(eval);
}

void log_warnx(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	warn_help(LOG_WARN, NULL, fmt, ap);
	va_end(ap);
}

void warn_help(int severity, const char *errstr, const char *fmt, va_list ap)
{
	char buf[1024];
	int len;
	int buflen = sizeof(buf);
	if (fmt != NULL)
	{
		vsnprintf(buf, buflen, fmt, ap);
	}
	else
		buf[0] = '\0';
	if(errstr)
	{
		len = strlen(buf);
		if(len < sizeof(buf) - 3)
		{
			log_snprintf(buf + len, buflen - len, ": %s", errstr);
		}
	}
   
	internal_log(severity, buf);
}

void log_snprintf(char *buf, int buflen, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buf, buflen, fmt, ap);
	va_end(ap);
}

void log_set_cb(log_print_cb log_cb)
{
	log_fn = log_cb;
}
void internal_log(int serverity, const char *msg)
{
	const char *serverity_str;

	if(log_fn) //自己设置日志函数
	{
		log_fn(serverity, msg);
	}
	switch(serverity)
	{
	case LOG_ERR:
		serverity_str = "err";
		break;
	case LOG_WARN:
		serverity_str = "warn";
		break;
	default:
		serverity_str = "???";
		break;
	}
	//默认是标准输出，可以修改写日志
	fprintf(stderr, "[%s] %s\n", serverity_str, msg);
}

