/*
 * WARNING!
 * Before copying code to this file, make sure it's as perfect
 * as you can make it to be, since no debug logs should be
 * recorded here.
 */

#include <log.h>

/*
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

	printf("%s\n%s", log, _RESET);

	return;
}

/*
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

	fprintf(stderr, "%s\n%s", log, _RESET);

	return;
}

/*
 * description
 * For printing important information to user.
 */
void alert(const char* s, ...)
{
	va_list list;
	time_t t = time(NULL);
	struct tm mt = *localtime(&t);
	char log[MAX_LOG_SIZE] = {0};
	int p = 0;

	p = snprintf(log, sizeof(log), "%s[%04d/%02d/%02d][%02d:%02d:%02d] ",
			_F_BRIGHT_GREEN,
			mt.tm_year+1900, mt.tm_mon+1, mt.tm_mday,
			mt.tm_hour, mt.tm_min, mt.tm_sec);

	va_start(list, s);
	vsnprintf(log+p, sizeof(log)-p, s, list);
	va_end(list);

	printf("%s\n%s", log, _RESET);

	return;
}

/*
 * description
 * Logs general debug logs for functionality checks to 'stdout'
 * Requires MODE_DEBUG define on compile (Check Makefile)
 *
 * note
 * String length of function is hardcoded to 16, increase as needed
 * Integer length of code line is hardcoded to 6, increase as needed
 * Must have the 'MODE_DEBUG' defined active!
 */
void debug_log(const char* _f, int l, const char* s, ...)
{
#if !defined(MODE_DEBUG)
	return;
#endif // !defined(MODE_DEBUG)

	va_list list;
	time_t t = time(NULL);
	struct tm mt = *localtime(&t);
	char log[MAX_LOG_SIZE] = {0};
	int p = 0;
	char f[17] = {0};
	int i;

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
 * description
 * Logs general debug logs for functionality checks to 'stdout'
 * Requires MODE_DEBUG define on compile (Check Makefile)
 *
 * note
 * String length of function is hardcoded to 16, increase as needed
 * Integer length of code line is hardcoded to 6, increase as needed
 * Must have the 'MODE_DEBUG' defined active!
 * This will use a large buffer for output, so use ONLY when necessary
 */
void debug_l_log(const char* _f, int l, const char* s, ...)
{
#if !defined(MODE_DEBUG)
	return;
#endif // !defined(MODE_DEBUG)

	va_list list;
	time_t t = time(NULL);
	struct tm mt = *localtime(&t);
	char log[MAX_LARGE_LOG_SIZE] = {0};
	int p = 0;
	char f[17] = {0};
	int i;

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
 * description
 * Stringify the JSON object and outputs the result
 */
void dump_json(const char* f, int l, json_t *obj)
{
	char *s = NULL;

	if (obj == NULL) return;

	s = json_dumps(obj, JSON_INDENT(1));
	debug_l_log(f, l, "\n%s", s);

	free(s);
}
