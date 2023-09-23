#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <color.h>
#include <global.h>

#define MAX_LOG_SIZE 1024
#define MAX_LARGE_LOG_SIZE 1024*1024 // 1MB

void	debug_log(const char* f, int l, const char* s, ...);
void	debug_l_log(const char* _f, int l, const char* s, ...);
void	dump_json(const char* f, int l, json_t *obj);
#define	d(s, ...) debug_log(__func__, __LINE__, s, ##__VA_ARGS__)
#define	dl(s, ...) debug_l_log(__func__, __LINE__, s, ##__VA_ARGS__)
#define	dj(obj) dump_json(__func__, __LINE__, obj)

void	out(const char* s, ...);
void	errout(const char* s, ...);

#endif // __LOG_H__
