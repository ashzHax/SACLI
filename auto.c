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

char* cut(char *c, size_t cs) {
	int i = 3; // TODO: might have to change if the there are multiple status
	char t[MAX_PATH_LEN] = {0};

	for (i=0;c[i]!='\n';i++) {};
	c[i] = '\0';
	for (i=3;c[i]==' ';i++) {};

	snprintf(t, sizeof(t), "%s", &(c[i]));
	snprintf(c, cs, "%s", t);

	return c;
}

/*
 * description
 * Handler for the 'revert' action command.
 */
int _auto(struct command_info* c)
{
	int return_value = EXIT_NORMAL;
	char cmd[MAX_CMD_LEN+MAX_PATH_LEN] = {0};
	FILE *fp = NULL;
	char path[MAX_PATH_LEN] = {0};
	FILE *p = NULL;
	char file[MAX_PATH_LEN] = {0};
	char final_file[MAX_PATH_LEN] = {0};
	int is_same = 0;
	size_t index;
	json_t *value;

	if (c == NULL) {
		d("critical error, 'c' is NULL");
		return_value = EXIT_AUTO_FAILED;
		goto EXIT;
	}

	d("starting 'auto' command handler");

	// read target file
	fp = fopen(c->auto_target_path, "r");
	if (fp == NULL) {
		return_value = EXIT_AUTO_FAILED;
		goto EXIT;
	}

	// run svn st based on that file
	while (fgets(path, sizeof(path), fp) != NULL) {
		snprintf(cmd, sizeof(cmd), "svn st %s", path);

		p = popen(cmd, "r");
		if (p == NULL) {
			continue;
		}
		
		while(fgets(file, sizeof(file), p) != NULL) {
			cut(file, sizeof(file));
			d("got file : [%s]", file);
			if (get_clean_path(c, file, final_file, sizeof(final_file)) < 0) {
				errout("failed to find valid file [%s]", file);
				continue;
			}
			d("finalized file path [%s]", final_file);
			
			// check if 'final_file' has duplicates
			is_same = 0;
			json_array_foreach (c->j_files, index, value) {
				if (_strfcmp(json_string_value(value), final_file) == 0) {
					errout("found duplicate file path, skipping (%s)", final_file);
					is_same = 1;
					break;
				}
			}

			if (is_same == 0) {
				out("adding new file to group [%s] -> [%s]", c->group_name, final_file);
				json_array_append_new(c->j_files, json_string(final_file));
			}
		}
		pclose(p);
	}
	fclose(fp);

	// add all M/D/A files to the group
	
EXIT:
	return return_value;
}
