/*
 * WARNING!
 * Before copying code to this file, make sure it's as perfect
 * as you can make it to be, since no debug logs should be
 * recorded here.
 */

#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <global.h>
#include <color.h>

/* 
 * personal strlen() 
 *
 * description
 * Returns the length of the character buffer 's'
 * Counts till NULL is found
 *
 * note
 * I think it is a very bad idea to modify the 
 * function parameter variables.
 * But for fast processing, declaring a 'safe variable'
 * is a waste of time.
 * 
 * return
 * length of 's' (till character NULL)
 * -1 on error
 */
size_t _strlen(const char *s)
{
	size_t len = 0;

	if (s == NULL) {
		return -1;
	}

	while (*(s++) != '\0') {
		len++;
	}
	return len;
}

/* 
 * ASCII string to lowercase ASCII string
 *
 * description
 * Changes the capitalization of all character in 's' to
 * lowercase letters.
 *
 * return
 * Returns the received string 's'
 * Returns NULL on error
 */
char* _strlow(char *s)
{
	int		i = 0;
	size_t	len = 0;

	if (s == NULL) {
		return NULL;
	}

	len = _strlen(s);

	if (len <= 0) {
		return NULL;
	}

	for (i=0; i<len; i++) {
		if (s[i] >= ASCII_ALPHA_UP_START && 
				s[i] <= ASCII_ALPHA_UP_END) {
			s[i] += ASCII_ALPHA_CASE_DIFF;
		}
	}

	return s;
}

/* 
 * personal strcpy()
 *
 * description
 * Copies 't' amount of characters from 's' to 'd'
 *
 * return
 * Returns the received string 's'
 * Returns NULL on error
 */
char* _strcpy(const char *s, char *d, size_t t)
{
	int i;

	if (s == NULL || d == NULL || t <= 0) {
		return NULL;
	}

	for (i=0; i<t; i++) {
		if (s[i] == '\0') {
			break;
		}
		d[i] = s[i];
	}
	d[i] = '\0';

	return d;
}

/* 
 * personal strncmp(), but better
 *
 * description
 * A full compare of string, even the length.
 * (Thinking about it, it's more like Java isEqual() method
 *
 * return
 * Returns the difference count (in length), or the (index+1) from
 * which the difference starts
 * -1: Invalid parameter values
 * 0 : If 'a' and 'b' are the same string
 * x : If 'a' and 'b' are NOT the same string
 */
int _strfcmp(const char *a, const char *b)
{
	int i;
	int a_len, b_len;

	if (a == NULL || b == NULL) {
		return -1;
	}

	a_len = _strlen(a);
	b_len = _strlen(b);

	if (a_len != b_len) {
		if (a_len > b_len) {
			return a_len - b_len;
		} else {
			return b_len - a_len;
		}
	}

	for (i=0; i<(int)a_len; i++) {
		if (a[i] != b[i]) {
			return i+1;
		}
	}

	return 0;
}

/* 
 * personal strncmp()
 *
 * description
 * A numbered compare of string.
 *
 * return
 * Returns the difference count (in length), or the (index+1) from
 * which the difference starts
 * -1: Invalid parameter values
 * 0 : If 'a' and 'b' are the same string
 * x : If 'a' and 'b' are NOT the same string
 */
int _strncmp(const char *a, const char *b, size_t t)
{
	int i;
	int a_len, b_len;

	if (a == NULL || b == NULL || t <= 0) {
		return -1;
	}

	a_len = _strlen(a);
	b_len = _strlen(b);

	// invalid amount of bytes to check 't' amount of characters
	if (a_len > b_len) {
		if (t > b_len) {
			return -1;
		}
	} else {
		if (t > a_len) {
			return -1;
		}
	}

	for (i=0; i<(int)t; i++) {
		if (a[i] != b[i]) {
			return i+1;
		}
	}

	return 0;
}

/*
 * Debug log function
 *
 * description
 * Logs general debug logs for functionality checks to 'stdout'
 * Requires MODE_DEBUG define on compile (Check Makefile)
 *
 * note
 * String length of function is hardcoded to 16, increase as needed
 * Integer length of code line is hardcoded to 6, increase as needed
 */
void debug_log(const char* _f, int l, const char* s, ...)
{
	va_list list;
	time_t t = time(NULL);
	struct tm mt = *localtime(&t);
	char log[MAX_LOG_SIZE] = {0};
	int p = 0;
	char f[17] = {0};
	int i;

#if !defined(MODE_DEBUG)
	return;
#endif // !defined(MODE_DEBUG)

	if (_strlen(_f) > 16) {
		for (i=0; i<13; i++) {
			f[i] = _f[i];
		}
		f[i++] = '.';
		f[i++] = '.';
		f[i++] = '.';
		f[i] = '\0';
	} else {
		_strcpy(_f, f, 16);
	}

	p = snprintf(log, sizeof(log), "%s[%04d/%02d/%02d][%02d:%02d:%02d][%16s():%-6d] ",
			_F_BRIGHT_BLACK,
			mt.tm_year+1900, mt.tm_mon+1, mt.tm_mday,
			mt.tm_hour, mt.tm_min, mt.tm_sec,
			f, l);

	va_start(list, s);
	vsnprintf(log+p, sizeof(log)-p, s, list);
	va_end(list);

	printf("%s\n%s", log, _RESET);

	return;
}

/*
 * Output log function
 *
 * description
 * For printing information to user.
 */
void out(const char* s, ...)
{
	va_list list;
	time_t t = time(NULL);
	struct tm mt = *localtime(&t);
	char log[MAX_LOG_SIZE] = {0};
	int p = 0;

	p = snprintf(log, sizeof(log), "%s[%04d/%02d/%02d][%02d:%02d:%02d] ",
			_F_BRIGHT_BLUE,
			mt.tm_year+1900, mt.tm_mon+1, mt.tm_mday,
			mt.tm_hour, mt.tm_min, mt.tm_sec);

	va_start(list, s);
	vsnprintf(log+p, sizeof(log)-p, s, list);
	va_end(list);

	printf("%s\n", log);

	return;
}

/*
 * Error output log function
 *
 * description
 * For printing error information to user.
 */
void errout(const char* s, ...)
{
	va_list list;
	time_t t = time(NULL);
	struct tm mt = *localtime(&t);
	char log[MAX_LOG_SIZE] = {0};
	int p = 0;

	p = snprintf(log, sizeof(log), "%s[%04d/%02d/%02d][%02d:%02d:%02d] ",
			_F_BRIGHT_RED,
			mt.tm_year+1900, mt.tm_mon+1, mt.tm_mday,
			mt.tm_hour, mt.tm_min, mt.tm_sec);

	va_start(list, s);
	vsnprintf(log+p, sizeof(log)-p, s, list);
	va_end(list);

	fprintf(stderr, "%s\n", log);

	return;
}
