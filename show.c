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
 * Handler for the 'show' action command.
 */
int _show(struct command_info* c)
{
	int return_value = EXIT_NORMAL;
	size_t index;
	json_t *value;

	if (c == NULL) {
		d("critical error, 'c' is NULL");
		return_value = EXIT_ADD_FAILED;
		goto EXIT;
	}

	out("showing files inside group [%s]:", c->group_name);
	json_array_foreach (c->j_group, index, value) {
		out("    -> [%s]", json_string_value(value));
	}
EXIT:
	return return_value;
}
