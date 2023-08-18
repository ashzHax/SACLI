// by     : ashz
// reason : main code interface for handling arguments and running sub-systems
/* tab = 4 spaces */

#include <stdio.h>
#include "global.h"

#define EXIT_NORMAL					0
#define EXIT_ERROR					-1
#define EXIT_ARGUMENT_PARSE_FAIL	-2
#define EXIT_INVALID_ARGUMENT		-3

// modes
enum {
	INVALID = -1,
	ADD = 0,
	REMOVE,
	SHOW,
	COMMENT,
	COMMIT,
	EDIT,
	HELP,
	INFO,
	CLEAN,
	REVERT,
	ROLLBACK
};

// move to lib :/
inline BOOL cpy_string(char *s, char *d, size_t t)
{
	int i;

	if(s == NULL || d == NULL || t <= 0) {
		return 0;
	}

	for(i=0; i<t; i++) {
		if( s[i] == '\0' ) {
			break;
		}
		d[i] = s[i];
	}
	d[i] = '\0';

	return 1;
}

inline void convert_str_lower(char *s, size_t t)
{
	int i;

	if(s == NULL || t <= 0) {
		return;
	}

	for(i=0; i<t; i++) {
		if(s[i] >= 65 && s[i] <= 90) {
			s[i] += 32;
		}
	}
}

/* 
 * handmade strlen() 
 *
 * note
 * Personally, I think it is a very bad idea to modify the function arguments.
 * But for fast processing, declaring a "could be saved" variable is a waste of time.
 * 
 * return
 * length of "s" (till character NULL)
 * -1 on error
*/
size_t get_str_len(char *s)
{
	size_t len = 0;

	if(s == NULL) {
		return -1;
	}

	while(*(s++) != '\0') {
		len++;
	}
	return len;
}

// handmade strncmp, but a full compare of length.
// thinking about it, its more like Java isEqual()
unsigned char chk_str_diff(char *a, char *b)
{
	int i;
	int len;

	if(a == NULL || b == NULL) {
		return (unsigned char)-1;
	}

	len = get_str_len(a);

	if(len != get_str_len(b)) {
		return (unsigned char)-1;
	}

	for(i=0; i<(int)len; i++) {
		if(a[i] != b[i]) {
			return (unsigned char)len-i;
		}
	}

	return (unsigned char)0;
}

int option_parse(int argc, char** argv)
{
	int len;

#if DEBUG_MODE // print argument list
	int i = 0;

	for(i = 0; i < argc; i++) {
		printf("[%d] - argv[%s]\n", i, argv[i]);
	}
#endif // DEBUG_MODE

	len = get_str_len(argv[1]);

	printf("---> [%s():%d] ---> len:%d\n", __func__, __LINE__, len);

	if (chk_str_diff(argv[1], "add") == 0) { // if same
		printf("found ADD!!!!\n");
	}

	return 0;
}

int main(int argc, char** argv)
{
	int mode = MODE_INVALID;

	mode = option_parse(argc, argv);
	switch(mode) {
		case ADD:
		{
			break;
		}
	}

	printf("basic compile test for project initialization\n");
	return 0;
}
