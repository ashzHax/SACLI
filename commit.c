#include <jansson.h>
#include <global.h>
#include <log.h>

/*
 * description
 * Handler for the 'commit' action command
 */
int _commit(struct command_info* c)
{
	int return_value = EXIT_NORMAL;

	// for the commit list file
	char *cl_path = NULL;
	size_t cl_path_size = 0;
	FILE *cl_path_fp = NULL;

	size_t index;
	json_t *value;

	const char *t = NULL;
	char *target = NULL;
	size_t target_size = 0;
	char cmd[MAX_CMD_LEN] = {0};

	// for the comment file
	char *co_path = NULL;
	size_t co_path_size = 0;
	FILE *co_path_fp = NULL;
	char co_read[256] = {0};

	// user confirmation
	char in[8] = {0};

	if (c == NULL) {
		errout("command_info is NULL");
		return_value = EXIT_COMMIT_ERROR;
		goto EXIT;
	}

	// get commit list file's path length
	cl_path_size = sizeof(char) * 
		(_strlen(c->root_path) + _strlen(c->grp_name) + 32);

	// create an dynamic string variable for the commit list file
	cl_path = (char*)malloc(cl_path_size);
	if (cl_path == NULL) {
		errout("malloc() failed");
		return_value = EXIT_COMMIT_ERROR;
		goto EXIT;
	}

	// create temporary commit list file's path
	snprintf(cl_path, cl_path_size, "%s/"COT_FILE, c->root_path, c->grp_name);

	target_size = sizeof(char) * (_strlen(c->root_path) + MAX_PATH_LEN);
	target = (char*)malloc(target_size);
	if (target == NULL) {
		errout("malloc() failed");
		return_value = EXIT_COMMIT_ERROR;
		goto EXIT;
	}

	cl_path_fp = fopen(cl_path, "w");
	if (cl_path_fp == NULL) {
		errout("failed to open file [%s]", cl_path);
		return_value = EXIT_COMMIT_ERROR_FILE_OPEN_FAIL;
		goto EXIT;
	}

	alert("commiting the following files:");

	// get files from JSON list, and append them to commit list file
	json_array_foreach (c->json_files, index, value) {
		t = json_string_value(value);
		snprintf(target, target_size, "%s/%s", c->root_path, t);

		// run 'svn add' command if file is not scheduled for anything
		// TODO: make this code run if option is given
		if (get_file_svn_schedule(target) == SVN_SCHED_UNDEFINED) {
			alert(" -> scheduling file to be added [%s]", target);

			snprintf(cmd, sizeof(cmd), "svn add %s > /dev/null", target); 
			d("running command: [%s]", cmd);
			system(cmd);
		}

		fputs(target, cl_path_fp);
		fputs("\n", cl_path_fp);

		out(" -> [%s]", target);
	}

	// commit list ready to be committed
	free(target);
	target = NULL;
	fclose(cl_path_fp);
	cl_path_fp = NULL;

	// get commit list file's path length
	co_path_size = sizeof(char) * 
		(_strlen(c->root_path) + _strlen(c->grp_name) + 32);

	// create an dynamic string variable for the commit list file
	co_path = (char*)malloc(co_path_size);
	if (co_path == NULL) {
		errout("malloc() failed");
		return_value = EXIT_COMMIT_ERROR;
		goto EXIT;
	}

	// get comment file
	snprintf(co_path, co_path_size, "%s/"CMT_FILE, c->root_path, c->grp_name);

	co_path_fp = fopen(co_path, "r");
	if (co_path_fp == NULL) {
		errout("failed to open file [%s]", co_path);
		return_value = EXIT_COMMIT_ERROR_FILE_OPEN_FAIL;
		goto EXIT;
	}

	alert("commit message will be as the following:");
	while(fgets(co_read, sizeof(co_read), co_path_fp) != NULL) {
		out("%s", rm_whitespace(co_read));
	}
	fclose(co_path_fp);
	co_path_fp = NULL;

	// get user confirmation before running commit
	alert("commence commit? (y/N)");

	fgets(in, sizeof(in), stdin);
	if (in[0] != 'y') {
		out("commit canceled! ending program [%s]", rm_whitespace(in));
		goto EXIT;
	}

	snprintf(cmd, sizeof(cmd), "svn commit -F %s --targets %s", co_path, cl_path); 
	d("running command: [%s]", cmd);
	system(cmd);

EXIT:
	if (cl_path) {
		free(cl_path);
	}
	if (target) {
		free(target);
	}
	if (cl_path_fp) {
		fclose(cl_path_fp);
	}
	if (co_path) {
		free(co_path);
	}
	if (co_path_fp) {
		fclose(co_path_fp);
	}

	return return_value;
}
