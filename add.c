/*
 * WARNING!
 * Before copying code to this file, make sure it's as perfect
 * as you can make it to be, since no debug logs should be
 * recorded here.
 */

#include <jansson.h>
#include <global.h>
#include <log.h>

/*
 * description
 * Handler for the 'add' action command.
 *
 * Clean up the path string received by the user
 * 1. Remove initial './' part of string if it exists
 * 2. Add '/' to the end of the path string if file path is a directory
 * 3. Append string to the CWD(Current Working Directory, without svn root)
 *
 * TODO: need to run 'svn add' to all files on commit
 */
int _add(struct command_info* c)
{
	int return_value = EXIT_NORMAL;
	int i = 0;
	char final_path[MAX_PATH_LEN] = {0};
	int is_same = 0;
	size_t index;
	json_t *value;

	if (c == NULL) {
		d("critical error, 'c' is NULL");
		return_value = EXIT_ADD_FAILED;
		goto EXIT;
	}

	d("starting 'add' command handler");

	for (i=0; i<c->act_arg_cnt; i++) {
		if (get_clean_path(c, c->act_arg[i], final_path, sizeof(final_path)) < 0) {
			errout("failed to find valid file [%s]", c->act_arg[i]);
			continue;
		}
		d("finalized file path (%s)", final_path);

		// check if 'final_path' has duplicates
		is_same = 0;
		json_array_foreach (c->j_group, index, value) {
			if (_strfcmp(json_string_value(value), final_path) == 0) {
				errout("found duplicate file path, skipping (%s)", final_path);
				is_same = 1;
				break;
			}
		}

		if (is_same == 0) {
			out("adding new file to group [%s] -> [%s]", c->group_name, final_path);
			json_array_append_new(c->j_group, json_string(final_path));
		}
	}

EXIT:
	return return_value;
}
