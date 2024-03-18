#include <jansson.h>
#include <global.h>
#include <log.h>

/*
 * description
 * Handler for the 'show' action command
 * This command shows the files inside a target group
 */
int _show(struct command_info* c)
{
	int return_value = EXIT_NORMAL;

	size_t index;
	json_t *value;

	if (c == NULL) {
		errout("command_info is NULL");
		return_value = EXIT_SHOW_ERROR;
		goto EXIT;
	}

	out("list of files inside group [%s]", c->grp_name);
	json_array_foreach (c->json_files, index, value) {
		out("-> [%s]", json_string_value(value));
	}

EXIT:
	return return_value;
}
