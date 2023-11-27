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
 * Handler for the 'commit' action command.
 */
int _commit(struct command_info* c)
{
	int return_value = EXIT_NORMAL;
	FILE *comment_file = NULL;
	char path[MAX_PATH_LEN*2] = {0}; // TODO: this bloated size makes no sense
	char path2[MAX_PATH_LEN*2] = {0}; // TODO: this bloated size makes no sense
	const char *tmp = NULL;
	char buf [256] = {0};
	char copy [256] = {0};
	size_t index;
	json_t *value;
	char in[8] = {0};
	char cmd [MAX_CMD_LEN*7]=  {0};
	FILE *files = NULL;
	char full_file_path[MAX_PATH_LEN] = {0};

	if (c == NULL) {
		d("critical error, 'c' is NULL");
		return_value = EXIT_COMMIT_FAILED;
		goto EXIT;
	}

	alert("commiting the following files:");
	snprintf(path2, sizeof(path2), "%s.c.%s.files", c->svn_root_path, c->group_name); 
	files = fopen(path2, "w");
	if (files == NULL) {
		d("failed to open commit files file");
		return_value = EXIT_COMMIT_FAILED;
		goto EXIT;
	}

	json_array_foreach (c->j_files, index, value) {
		tmp = json_string_value(value);
		snprintf(full_file_path, sizeof(full_file_path), "%s%s", c->svn_root_path, tmp);
		fputs(full_file_path, files);

		// add files
		if (get_file_svn_schedule(full_file_path) == SVN_SCHED_UNDEFINED) {
			snprintf(cmd, sizeof(cmd), "svn add %s", full_file_path); 
			out(cmd);
			system(cmd);
		}

		fputs("\n", files);
		out("-> [ %s ]", full_file_path);
	}

	fclose(files);

	alert("commit message will be the following:");
	
	tmp = json_string_value(c->j_comment);
	if (tmp == NULL || strlen(tmp) <= 0) {
		errout("no comment found! run \"svm comment\"");
		return_value = EXIT_COMMIT_FAILED;
		goto EXIT;
	} else {
		snprintf(path, sizeof(path), "%s", tmp); 
	}
	comment_file = fopen(path, "r");

	if (comment_file == NULL) {
		return_value = EXIT_COMMIT_FAILED;
		goto EXIT;
	}

	// dont need copy, since buf can be modified
	while(fgets(buf, sizeof(buf), comment_file) != NULL) {
		memset(copy, 0x0, sizeof(copy));
		strncpy(copy, buf, strlen(buf));
		out("%s", rm_whitespace(copy));
	}

	fclose(comment_file);

	alert("commence commit? (y/N)");
	
	fgets(in, sizeof(in), stdin);

	if (in[0] != 'y') {
		goto EXIT;
	}

	snprintf(cmd, sizeof(cmd), "svn commit -F %s --targets %s", path, path2); 
	out(cmd);
	system(cmd);

EXIT:
	return return_value;
}
