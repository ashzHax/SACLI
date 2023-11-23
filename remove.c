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
 * Handler for the 'remove' action command.
 */
int _remove(struct command_info* c)
{
	int i = 0;
	char final_path[MAX_PATH_LEN] = {0};
	size_t index;
	json_t *value;
	int return_value = EXIT_NORMAL;
	int flag = 0; // to check if anything was done

	if (c == NULL) {
		d("critical error, 'c' is NULL");
		return_value = EXIT_ADD_FAILED;
		goto EXIT;
	}

	d("starting 'remove' command handler");

	for (i=0; i<c->act_arg_cnt; i++) {
		if (get_clean_path(c, c->act_arg[i], final_path, sizeof(final_path)) < 0) {
			errout("failed to find valid file [%s]", c->act_arg[i]);
			// use received string as path anyway to check if path exists in array
			_strcpy(c->act_arg[i], final_path, sizeof(final_path));
		}
		d("finalized file path (%s)", final_path);

		// find and delete target
		flag = 0;
		json_array_foreach (c->j_files, index, value) {
			if (_strfcmp(json_string_value(value), final_path) == 0) {
				out("removing file from group [%s] -> [%s]", c->group_name, final_path);
				if (json_array_remove(c->j_files, index) == -1) {
					errout("failed to remove index from array");
				}
				flag = 1;
				break;
			}
		}

		if (flag == 0) {
			errout("file [%s] not found inside group [%s]", final_path, c->group_name);
		}
	}

EXIT:
	return return_value;
}
