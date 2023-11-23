/*
 * WARNING!
 * Before copying code to this file, make sure it's as perfect
 * as you can make it to be, since no debug logs should be
 * recorded here.
 * Eh, who cares?
 */

#include <jansson.h>
#include <global.h>
#include <log.h>

/*
 * description
 * Handler for the 'comment' action command.
 */
int _comment(struct command_info* c)
{
	int return_value = EXIT_NORMAL;
	int i = 0;
	char *buf = NULL;
	int pos = 0; // position of string
	size_t size = 0;
	char path[MAX_PATH_LEN*2] = {0}; // TODO: this bloated size makes no sense
	char cmd [MAX_CMD_LEN*3]=  {0}; // TODO: this either

	if (c == NULL) {
		d("critical error, 'c' is NULL");
		return_value = EXIT_ADD_FAILED;
		goto EXIT;
	}

	// TODO: need a way to un-hard code this 'c', for crying out loud
	snprintf(path, sizeof(path), "%s.c.%s.comment", c->svn_root_path, c->group_name); 

	// if there is a comment 
	if (c->act_arg_cnt > 0) {
		size = sizeof(char) * c->act_arg_cnt * MAX_CMNT_WORD_LEN;
		buf = (char*)malloc(size);
		if (buf == NULL) {
			return_value = EXIT_ERROR;
			goto EXIT;
		}

		pos = 0;
		for (i=0; i<c->act_arg_cnt; i++) {
			pos += snprintf(buf+pos, size-pos, "%s ", c->act_arg[i]);
		}
		rm_whitespace(buf);

		d("final string: [%s]", buf);

		// write to file, then open file
		// save the file name to the "j_comment"
		FILE *f = fopen(path, "a");
		if(f) {
			fputs("\n", f);// TODO:  yeah....lets not do this
			fputs(buf, f);
			fclose(f);
		}
		free(buf);
	}

	// TODO: option to choose what editor u wanna use?
	snprintf(cmd, sizeof(cmd), "vim %s", path); 
	system(cmd);

	json_string_set(c->j_comment, path);
EXIT:
	return return_value;
}
