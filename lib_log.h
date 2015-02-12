#ifndef _LOG_H_
#define _LOG_H_

#include <errno.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#define LOG_DEBUG 0
#define LOG_MSG   1
#define LOG_WARN  2
#define LOG_ERR   3

#define CHECK_FMT(a,b) __attribute__((format(printf,a,b)))
#define NORETURN  __attribute__((noreturn))

void log_err(int eval, const char *fmt, ...) CHECK_FMT(2,3) NORETURN;
void log_warn(const char *fmt, ...) CHECK_FMT(1,2);
void log_errx(int eval, const char *fmt, ...) CHECK_FMT(2,3) NORETURN;
void log_warnx(const char *fmt, ...) CHECK_FMT(1,2);

#endif
