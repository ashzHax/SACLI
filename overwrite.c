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
 * Handler for the 'overwrite' action command.
 */
int _overwrite(struct command_info* c)
{
	int return_value = EXIT_NORMAL;
	const char *dest_name = NULL;
	const char *src_name = NULL;
	json_t *dest = NULL;
	json_t *src = NULL;
	json_t *group_list = NULL;
	json_t *t = NULL;
	json_t *t2 = NULL;
	int flag_create = 1;

	if (c == NULL) {
		d("critical error, 'c' is NULL");
		return_value = EXIT_OVERWRITE_FAILED;
		goto EXIT;
	}

	d("starting 'overwrite' command handler");
	
	if (c->act_arg_cnt == 0) {
		errout("not enough arguments to run overwrite");
		return_value = EXIT_OVERWRITE_NOT_ENOUGH_ARGS;
		goto EXIT;
	}
	if (c->act_arg_cnt > 2) {
		errout("too many arguments to run overwrite");
		return_value = EXIT_OVERWRITE_TOO_MANY_ARGS;
		goto EXIT;
	}

	d("received arguments [%s] <- [%s]", c->act_arg[0], c->act_arg[1]);
	dest_name = c->act_arg[0];
	src_name = c->act_arg[1];

	group_list = json_object_get(c->j_config, "groups");
	if (group_list == NULL) {
		d("critical error, 'group_list' is NULL");
		return_value = EXIT_OVERWRITE_FAILED;
		goto EXIT;
	}
	dest = json_object_get(group_list, dest_name);
	src = json_object_get(group_list, src_name);

	if (src == NULL) {
		errout("group '%s' does not exist", src);
		return_value = EXIT_OVERWRITE_FAILED;
		goto EXIT;
	}

	if (dest == NULL) {
		dest = json_object();
		//json_object_set_new(dest, "")
		//c->j_files = json_array();
		//c->j_comment = json_string("");
		//json_object_set(c->j_group, "files", c->j_files);
		//json_object_set(c->j_group, "comment", c->j_comment);
		//json_object_set(group_list, c->group_name, c->j_group);
	}
	
	t = json_object_get(src, "files");
	t2 = json_object_get(dest, "files");
	if (t == NULL) {
		errout("source group does not have files!");
		return_value = EXIT_OVERWRITE_FAILED;
		if (flag_create) json_decref(dest);
		goto EXIT;
	}
	if (t2) {
		json_decref(t2);
	}
	json_object_set_new(dest, "files", t);
	json_object_set_new(group_list, dest_name, dest);

	//if (json_dump_file(c->j_config, c->config_path, JSON_INDENT(1)) < 0) {
	//	errout("Failed to dump configuration data to file, critical error");
	//} else {
	//	d("Data dump successful");
	//}
EXIT:
	return return_value;
}
