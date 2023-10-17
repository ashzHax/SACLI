/*
 * WARNING!
 * Before copying code to this file, make sure it's as perfect
 * as you can make it to be, since no debug logs should be
 * recorded here.
 */

#include <stdio.h>
#include <global.h>

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

	if (s == NULL) return -1;

	while (*(s++) != '\0') len++;

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

	if (s == NULL) return NULL;

	len = _strlen(s);

	if (len <= 0) return NULL;

	for (i=0; i<len; i++) {
		if (s[i] >= ASCII_ALPHA_UP_START && s[i] <= ASCII_ALPHA_UP_END) {
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

	if (s == NULL || d == NULL || t <= 0) return NULL;

	for (i=0; i<t; i++) {
		if (s[i] == '\0') break;
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

	if (a == NULL || b == NULL) return -1;

	a_len = _strlen(a);
	b_len = _strlen(b);

	if (a_len != b_len) {
		if (a_len > b_len) return a_len - b_len;
		else return b_len - a_len;
	}

	for (i=0; i<(int)a_len; i++) {
		if (a[i] != b[i]) return i+1;
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

	if (a == NULL || b == NULL || t <= 0) return -1;

	a_len = _strlen(a);
	b_len = _strlen(b);

	// invalid amount of bytes to check 't' amount of characters
	if (a_len > b_len) {
		if (t > b_len) return -1;
	} else {
		if (t > a_len) return -1;
	}

	for (i=0; i<(int)t; i++) {
		if (a[i] != b[i]) return i+1;
	}

	return 0;
}

/*
 * description
 * Remove whitespaces at the end of string
 * Whitespaces include NULL character, next line, and spaces
 * Adds a NULL character to the end of the string after a
 * different character has been detected
 *
 * note
 * This will modify the received string, so be cautious
 * when using this function
 */
void rm_whitespace(char *s)
{
	int i = 0;

	if (s == NULL) return;

	for (i=_strlen(s)-1; i>=0; i++) {
		if (s[i] == '\0' || s[i] == '\n' || s[i] == ' ') {
			s[i] = '\0';
		} else {
			return;
		}
	}
}

/*
 * description
 * Adds a '/' to the end of the string
 * Checks if the 'max' length of string can handle 2 more strings
 * (NULL character and the '/' character
 *
 * return
 * The parameter buffer pointer is returned on success
 * NULL is returned on fail
 */
char* add_end_dir_slash(char* s, size_t max)
{
	int i = 0;
	size_t n = 0;

	if (s == NULL) return NULL;

	n = _strlen(s);

	if (max < n+2) return NULL;

	for (i=_strlen(s)-1; i>=0; i++) {
		if (s[i] == '\0' || s[i] == '\n' || s[i] == ' ') {
			s[i] = '\0';
		} else {
			s[i+1] = '/';
			s[i+2] = '\0';
			return s;
		}
	}

	return NULL; // should not run, critical error
}

/*
 * description
 * Get SVN file schedule for the file 's'
 *
 * return
 * The SVN schedule of target file
 */
int get_file_svn_schedule(const char *s)
{
	FILE *p = NULL; 
	int type = SVN_SCHED_UNDEFINED;
	char cmd[MAX_CMD_LEN] = {0};
	char buf[MAX_SCHED_LEN] = {0};

	snprintf(cmd, MAX_CMD_LEN, "svn info %s | grep Schedule: | awk '{print $2}'", s);

	p = popen(cmd, "r");
	if (p) {
		fgets(buf, sizeof(buf), p);
		pclose(p);
	}

	if (_strfcmp(buf, "normal") == 0) {
		type = SVN_SCHED_NORMAL;
	} else if (_strfcmp(buf, "add") == 0) {
		type = SVN_SCHED_ADD;
	} else if (_strfcmp(buf, "delete") == 0) {
		type = SVN_SCHED_DELETE;
	}

	return type;
}
