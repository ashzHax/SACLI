#include <jansson.h>
#include <global.h>
#include <log.h>

/*
 * description
 * Handler for the 'info' action command
 * Outputs a list of all groups
 */
int _info(struct command_info* c)
{
	int return_value = EXIT_NORMAL;

	const char *key;
	json_t *value;
	json_t *list;

	if (c == NULL) {
		errout("command_info is NULL");
		return_value = EXIT_INFO_ERROR;
		goto EXIT;
	}

	d("starting 'info' command handler");

	list = json_object_get(c->json_cfg, "groups");
	if (list == NULL) {
		errout("json_object_get returned NULL");
		return_value = EXIT_INFO_ERROR;
		goto EXIT;
	}

	out("list of groups:");
	json_object_foreach(list, key, value) {
		out("-> [%s]", key);
	}

EXIT:
	return return_value;
}
