#include <jansson.h>
#include <global.h>
#include <log.h>

/*
 * description
 * Handler for the 'remove' action command
 */
int _remove(struct command_info* c)
{
	int return_value = EXIT_NORMAL;

	int i = 0;
	char final_path[MAX_PATH_LEN] = {0};
	int flag = 0; // to check if any changes were made

	size_t index;
	json_t *value;

	if (c == NULL) {
		errout("command_info is NULL");
		return_value = EXIT_REMOVE_ERROR;
		goto EXIT;
	}

	d("starting 'remove' command handler");

	for (i=0; i<c->act_arg_cnt; i++) {
		if (get_clean_path(c, c->act_arg[i],
		final_path, sizeof(final_path)) == NULL)
		{
			errout(
			"failed to find valid file, checking group array using given string [%s]",
			c->act_arg[i]);

			// use user received string as path to check if path exists in array
			_strcpy(c->act_arg[i], final_path, sizeof(final_path));
		}
		d("using (in)valid file [%s]", final_path);

		// find file and if found, delete from JSON array
		flag = 0;
		json_array_foreach (c->json_files, index, value) {
			// found file
			if (_strfcmp(json_string_value(value), final_path) == 0) {
				out("removing [%s] from group [%s]", final_path, c->grp_name);

				// remove from array
				if (json_array_remove(c->json_files, index) == -1) {
					errout("failed to remove index from array");
					return_value = EXIT_REMOVE_ERROR_JSON_REMOVE_FAIL;
					goto EXIT;
				}

				flag = 1;
				break;
			}
		}

		// path was not in the JSON array
		if (flag == 0) {
			errout(
			"path [%s] was not found inside group [%s]",
			final_path, c->grp_name);

			return_value = EXIT_REMOVE_ERROR_FILE_NOT_FOUND;
			goto EXIT;
		}
	}

EXIT:
	return return_value;
}
