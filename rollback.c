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
 * Handler for the 'revert' action command.
 */
int _rollback(struct command_info* c)
{
	int return_value = EXIT_NORMAL;
	char final_path[MAX_PATH_LEN] = {0};
	char cmd[MAX_CMD_LEN+(MAX_PATH_LEN*2)] = {0};
	int rev = 0;
	int i;
	time_t t = time(NULL);

	if (c == NULL) {
		d("critical error, 'c' is NULL");
		return_value = EXIT_ROLLBACK_FAILED;
		goto EXIT;
	}

	d("starting 'rollback' command handler");

	if (c->act_arg_cnt < 2) {
		return EXIT_ROLLBACK_NOT_ENOUGH_ARGS;
	}

	rev = atoi(c->act_arg[0]);
	// TODO: add a revision validicity checker here (of the highest revision)
	if (rev <= 0) {
		return EXIT_ERROR; // TODO: ALSO ADD A VALID ERROR DEFINE 
	}

	for (i=1; i<c->act_arg_cnt; i++) {
		if (get_clean_path(c, c->act_arg[i], final_path, sizeof(final_path)) < 0) {
			errout("failed to find valid file [%s]", c->act_arg[i]);
			continue;
		}
		d("finalized file path (%s)", final_path);
		
		// remove existing file
		snprintf(cmd, sizeof(cmd), "rm -rf %s%s", c->svn_root_path, final_path);
		d("running command: [%s]", cmd);
		system(cmd);

		// get old file
		snprintf(cmd, sizeof(cmd), "svn up -r %d %s%s", rev, c->svn_root_path, final_path);
		d("running command: [%s]", cmd);
		system(cmd);

		// copy to temporary location
		snprintf(cmd, sizeof(cmd), "cp %s%s %d.bk", c->svn_root_path, final_path, (int)t);
		d("running command: [%s]", cmd);
		system(cmd);

		// up to latest
		snprintf(cmd, sizeof(cmd), "svn up %s%s", c->svn_root_path, final_path);
		d("running command: [%s]", cmd);
		system(cmd);

		// move backup file to original location
		snprintf(cmd, sizeof(cmd), "mv %d.bk %s%s", (int)t, c->svn_root_path, final_path);
		d("running command: [%s]", cmd);
		system(cmd);
	}

	out("rollbacked target file ", c->group_name);
EXIT:
	return return_value;
}
