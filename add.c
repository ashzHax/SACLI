#include <jansson.h>
#include <global.h>
#include <log.h>

/*
 * description
 * Handler for the 'add' action command
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
		errout("command_info is NULL");
		return_value = EXIT_ADD_ERROR;
		goto EXIT;
	}

	d("starting 'add' command handler");

	for (i=0; i<c->act_arg_cnt; i++) {
		if (get_clean_path(c, c->act_arg[i], 
		final_path, sizeof(final_path)) == 0)
		{
			errout("failed to find valid file, ignoring [%s]", c->act_arg[i]);
			return_value = EXIT_ADD_ERROR_INVALID_PATH;
			continue;
		}
		d("found valid file [%s]", final_path);

		// check if 'final_path' has duplicates
		is_same = 0;
		json_array_foreach (c->json_files, index, value) {
			if (_strfcmp(json_string_value(value), final_path) == 0) {
				errout(
				"already have file [%s] inside group [%s], skipping",
				final_path, c->grp_name);

				is_same = 1;
				return_value = EXIT_ADD_ERROR_DUPLICATE_PATH;
				break;
			}
		}

		if (is_same == 0) {
			out("adding [%s] to group [%s]", final_path, c->grp_name);
			json_array_append_new(c->json_files, json_string(final_path));
		}
	}

EXIT:
	return return_value;
}
