#include <jansson.h>
#include <global.h>
#include <log.h>

/*
 * description
 * Handler for the 'comment' action command
 */
int _comment(struct command_info* c)
{
	int return_value = EXIT_NORMAL;

	char *path = NULL;
	int i = 0;

	size_t p_size = 0; // size of path string
	size_t c_size = 0; // size of comment string
	char *comment_buf = NULL;
	int pos = 0; // current position of the combined comment string
	FILE *comment_file = NULL;
	char *cmd = NULL;

	if (c == NULL) {
		errout("command_info is NULL");
		return_value = EXIT_COMMENT_ERROR;
		goto EXIT;
	}

	// dynamic allocation for the path, because it's too big otherwise
	// MAX_PATH_LEN + root path length + group name length
	p_size = sizeof(char) * 
		(_strlen(c->root_path) + _strlen(c->grp_name) + MAX_PATH_LEN);
	path = (char*)malloc(p_size);
	cmd = (char*)malloc(p_size);
	if (path == NULL || cmd == NULL) {
		errout("malloc() failed");
		return_value = EXIT_COMMENT_ERROR;
		goto EXIT;
	}

	// not very happy with the macro use here, but it's fast enough
	snprintf(path, p_size, "%s/"CMT_FILE, c->root_path, c->grp_name);

	// if the 'comment' handler was called with the comment itself, append to file
	if (c->act_arg_cnt > 0) {
		// the total size of all comments, based on MAX_CMNT_WORD_LEN
		// MAX_CMNT_WORD_LEN is the estimated length of a single comment word
		c_size = sizeof(char) * c->act_arg_cnt * MAX_CMNT_WORD_LEN;
		comment_buf = (char*)malloc(c_size);
		if (comment_buf == NULL) {
			errout("malloc() failed");
			return_value = EXIT_COMMENT_ERROR;
			goto EXIT;
		}

		// append argument words into a single string
		pos = 0;
		for (i=0; i<c->act_arg_cnt; i++) {
			pos += snprintf(comment_buf+pos, c_size-pos, "%s ", c->act_arg[i]);
		}
		rm_whitespace(comment_buf);
		d("finalzed comment string [%s]", comment_buf);

		// write to file (appending)
		comment_file = fopen(path, "a");
		if (comment_file) {
			fputs("\n", comment_file); // to properly divide newly added comments
			fputs(comment_buf, comment_file);
			fclose(comment_file);
		} else {
			errout("failed to open comment file [%s]", path);
			return_value = EXIT_COMMENT_ERROR_FOPEN_FAIL;
			goto EXIT;
		}
	} else {
		// only when there are no arguments, call the editor
		// if you are using svn (boomer) and not using vim (boomer technology)
		// you ain't a real human being
		// TODO: option to choose what editor the user wants to use
		snprintf(cmd, p_size, "vim %s", path); 

		d("running command: [%s]", cmd);
		system(cmd);
	}

EXIT:
	if (path) {
		free(path);
	}
	if (cmd) {
		free(cmd);
	}
	if (comment_buf) {
		free(comment_buf);
	}

	return return_value;
}
