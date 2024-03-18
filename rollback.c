#include <jansson.h>
#include <global.h>
#include <log.h>

/*
 * description
 * Handler for the 'rollback' action command
 */
int _rollback(struct command_info* c)
{
	int return_value = EXIT_NORMAL;

	int revision = 0;

	size_t size = 0;
	char *cmd = NULL;

	FILE *p = NULL;
	char result_s[32] = {0}; // initial result value
	int result_i = 0; // result value to integer

	int i = 0;
	char path[MAX_PATH_LEN] = {0};

	time_t t = time(NULL); // for temporary file backup

	if (c == NULL) {
		errout("command_info is NULL");
		return_value = EXIT_ROLLBACK_ERROR;
		goto EXIT;
	}

	d("starting 'rollback' command handler");

	if (c->act_arg_cnt < 2) {
		return_value = EXIT_ROLLBACK_NOT_ENOUGH_ARGS;
		goto EXIT;
	}

	size = sizeof(char) * (MAX_CMD_LEN + _strlen(c->root_path) + MAX_PATH_LEN);
	cmd = (char*)malloc(size);
	if (cmd == NULL) {
		errout("malloc() failed");
		return_value = EXIT_REVERT_ERROR;
		goto EXIT;
	}

	revision = atoi(c->act_arg[0]);

	// gets the repository root revision, and use that as max revision reference
	snprintf(cmd, size,
		"svn info %s/ | grep Revision | awk '{print $2}'", c->root_path);

	p = popen(cmd, "r");
	if (p == NULL) {
		errout("popen() failed");
		return_value = EXIT_ROLLBACK_ERROR;
		goto EXIT;
	}

	fgets(result_s, sizeof(result_s), p);
	pclose(p);

	result_i = atoi(result_s);

	if (revision > result_i || revision <= 0) {
		errout("invalid repository revision value");
		return_value = EXIT_ROLLBACK_ERROR_INVALID_REVISION;
		goto EXIT;
	}

	for (i=1; i<c->act_arg_cnt; i++) {
		// get rollback target file's full path
		if (get_clean_path(c, c->act_arg[i], path, sizeof(path)) == NULL) {
			errout("failed to find valid file [%s]", c->act_arg[i]);
			continue;
		}
		d("found valid path [%s]", path);
		
		// remove existing file
		snprintf(cmd, size, "rm -rf %s/%s", c->root_path, path);
		d("running command: [%s]", cmd);
		system(cmd);

		// get old file
		snprintf(cmd, size, "svn up -r %d %s/%s", revision, c->root_path, path);
		d("running command: [%s]", cmd);
		system(cmd);

		// copy to temporary location
		snprintf(cmd, size, "cp %s/%s %d.bk", c->root_path, path, (int)t);
		d("running command: [%s]", cmd);
		system(cmd);

		// up to latest
		snprintf(cmd, size, "svn up %s/%s", c->root_path, path);
		d("running command: [%s]", cmd);
		system(cmd);

		// move backup file to original location
		snprintf(cmd, size, "mv %d.bk %s/%s", (int)t, c->root_path, path);
		d("running command: [%s]", cmd);
		system(cmd);

		out("rolling back file [%s]", path);
	}

EXIT:
	if (cmd) {
		free(cmd);
	}

	return return_value;
}
