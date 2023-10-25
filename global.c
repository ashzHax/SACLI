/*
 * WARNING!
 * Before copying code to this file, make sure it's as perfect
 * as you can make it to be, since no debug logs should be
 * recorded here.
 * Eh, who cares?
 */

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
	rm_whitespace(buf);

	if (_strfcmp(buf, "normal") == 0) {
		type = SVN_SCHED_NORMAL;
	} else if (_strfcmp(buf, "add") == 0) {
		type = SVN_SCHED_ADD;
	} else if (_strfcmp(buf, "delete") == 0) {
		type = SVN_SCHED_DELETE;
	}

	return type;
}

/*
 * description
 * Find the 'nth' substring, and return the pointer to the start of substring
 *
 * parameter
 * nth = starts from 0
 * s = string
 * ss = substring
 *
 * return
 * The position of 's' where 'nth' substring starts
 */
char* ss_find_nth(int nth, const char* s, const char* ss)
{
	int i = 0;
	size_t slen = 0;
	int j = 0;
	size_t sslen = 0;
	int flag = 0; // is substring the same as current position?
	int cnt = 0; // substring count

	if (nth < 0 || s == NULL || ss == NULL) return NULL;

	slen = _strlen(s);
	sslen = _strlen(ss);

	if (slen < sslen) return NULL;

	for (i=0; i<(int)slen; i++) {
		if (s[i] == ss[0]) {
			flag = 1; // reset

			for (j=0; j<(int)sslen; j++) {
				if(s[i+j] != ss[j]) {
					flag = 0; // is not the same
					break;
				}
			}

			if (flag) {
				cnt++;
				if (cnt >= nth) {
					return (char*)(s+i);
				}
			}
		}
	}

	return NULL;
}

/*
 * description
 * Get th total count of a substring found in string
 *
 * return
 * Number of 'ss' found in 's'
 * -1: error case
 */
int ss_cnt(const char* s, const char* ss)
{
	int i = 0;
	size_t slen = 0;
	int j = 0;
	size_t sslen = 0;
	int flag = 0; // is substring the same as current position?
	int cnt = 0; // substring count

	if (s == NULL || ss == NULL) return -1;

	slen = _strlen(s);
	sslen = _strlen(ss);

	for (i=0; i<slen; i++) {
		if (s[i] == ss[0]) {
			flag = 1; // reset

			for (j=0; j<sslen; j++) {
				if(s[i+j] != ss[j]) {
					flag = 0; // is not the same
					break;
				}
			}

			if (flag) cnt++;
		}
	}

	return cnt;
}

/*
 * description
 * Get finalized, clean path string from user-inputted pathing
 * Clean up the path string received by the user
 * 1. Remove initial './' part of string if it exists
 * 2. Add '/' to the end of the path string if file path is a directory
 * 3. Append string to the CWD(Current Working Directory, without svn root)
 *
 * parameter
 * p = path string pointer
 * f = finalized path string pointer
 * f_sz = size of 'f'
 * 
 * note
 * added a new more bytes to handle compile error of buffer overflow
 */
int get_clean_path(struct command_info* c, const char *p, char *f, size_t f_sz)
{
	struct stat fs;
	char new_p1[MAX_PATH_LEN+4] = {0}; // new path buffer 1 
	int s_cnt = 0; // substring count
	char **new_p2 = NULL; // new path array 2
	int p_len = 0; // length of path
	int first = 0; // first character index of a file name
	int *back_list = NULL;
	int *del_list = NULL;
	int i = 0;
	int j = 0;
	int d = 0;
	int b = 0;
	int flag = 0;
	int ret = 0;

	if (c == NULL || p == NULL || f == NULL || f_sz < 0) return -1;

	// check if path is valid
	if (access(p, F_OK) != 0) {
		if (get_file_svn_schedule(p) == SVN_SCHED_UNDEFINED) {
			return -1;
		}
		_strcpy(p, f, f_sz); // file is registered for schedule in SVN
		return 0;
	}

	// to check if 'p' is a directory
	if (stat(p, &fs) != 0) return -1;

	// if 'p' is a directory, but does not have the end '/', add it
	s_cnt = 0;
	if (S_ISDIR(fs.st_mode) && p[_strlen(p)-1] != '/') {
		snprintf(new_p1, sizeof(new_p1), "%s%s/", c->cwd, p);
	} else {
		snprintf(new_p1, sizeof(new_p1), "%s%s", c->cwd, p);
		s_cnt = 1;
	}
	p_len = _strlen(new_p1);

	// split all paths by '/'
	s_cnt += ss_cnt(new_p1, "/"); 
	
	new_p2 = (char**)malloc(sizeof(char*) * s_cnt);
	if (new_p2 == NULL) {
		d("malloc failed()");
		ret = -1;
		goto ERR;
	}
	memset(new_p2, 0, sizeof(char*) * s_cnt);

	del_list = (int*)malloc(sizeof(int) * s_cnt);
	if (del_list == NULL) {
		d("malloc failed()");
		ret = -1;
		goto ERR;
	}
	memset(del_list, 0, sizeof(int) * s_cnt);

	back_list = (int*)malloc(sizeof(int) * s_cnt);
	if (back_list == NULL) {
		d("malloc failed()");
		ret = -1;
		goto ERR;
	}
	memset(back_list, 0, sizeof(int) * s_cnt);

	first = 0;
	b = 0;
	d = 0;
	for (i=0; i<s_cnt; i++) {
		new_p2[i] = (char*)malloc(sizeof(char) * MAX_FILENAME_LEN);
		if (new_p2[i] == NULL) {
			d("malloc failed()");
			ret = -1;
			goto ERR;
		}

		// want to target the NULL character at the end
		for (j=first; j<=p_len; j++) {
			if (new_p1[j] == '/') {
				_strcpy(&new_p1[first], new_p2[i], (j-first)+1);
				first = j+1;
				break;
			} else if (new_p1[j] == '\0') {
				_strcpy(&new_p1[first], new_p2[i], (j-first)+1);
				first = -1; // end allocation loop
				break;
			}
		}
		
		// end this loop, since we no longer need more files
		if (first < 0) break;

		// check what is within the file names
		if (_strfcmp(new_p2[i], "../") == 0) {
			if (i == 0) {
				d("unable to handle back pathing from root [%s]", new_p2[i]);
				ret = -1;
				goto ERR;
			}
			back_list[b++] = i;
		} else if (_strfcmp(new_p2[i], "./") == 0) {
			del_list[d++] = i;
		}
	}

	first = 0; // used as position pointer for 'f'
	for (i=0; i<s_cnt; i++) {
		flag = 0;

		for (j=0; j<d; j++) {
			if (i == del_list[j]) {
				flag = 1;
				break;
			}
		}
		if (flag) continue;

		for (j=0; j<b; j++) {
			// checking the front word as well as current
			if (i+1 == back_list[j] || i == back_list[j]) {
				flag = 1;
				break;
			}
		}
		if (flag) continue;

		first += snprintf(f+first, f_sz-first, "%s", new_p2[i]);
	}

	// see if it exists with SVN root path attached to it
	snprintf(new_p1, sizeof(new_p1), "%s%s", c->svn_root_path, f);
	d("root attached file path, for checks [%s]", new_p1);

	// check if final path is valid
	if (access(new_p1, F_OK) != 0) {
		if (get_file_svn_schedule(new_p1) == SVN_SCHED_UNDEFINED) {
			d("cleaned path does not exist, returning original [%s]", p);
			_strcpy(p, f, f_sz); // file is registered for schedule in SVN
			ret = 0;
			goto ERR;
		}
	}
ERR:
	if (new_p2 != NULL) {
		for (i=0; i<s_cnt; i++) {
			if (new_p2[i] != NULL) {
				free(new_p2[i]);
			}
		}
		free(new_p2);
	}

	if (del_list != NULL) {
		free(del_list);
	}

	if (back_list != NULL) {
		free(back_list);
	}

	return ret;
}
