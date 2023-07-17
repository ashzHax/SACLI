// by     : ashz
// reason : main code interface for handling arguments and running sub-systems
/* tab = 4 spaces */

#include <stdio.h>
#include "global.h"

#define EXIT_NORMAL					0
#define EXIT_ERROR					-1
#define EXIT_ARGUMENT_PARSE_FAIL	-2
#define EXIT_INVALID_ARGUMENT		-3

enum {
	ADD,
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

int opt_parse(int argc, char** argv)
{
	int i = 0;
	for(i = 0; i < argc; i++) {
		printf("[%d] - argv[%s]\n", i, argv[i]);
	}

	return 0;
}

int main(int argc, char** argv)
{
	int ret = 0;

	ret = opt_parse(argc, argv);
	if(ret < 0) {
		return ret;
	}

	printf("basic compile test for project initialization\n");
	return 0;
}
