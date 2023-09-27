/*
 * WARNING!
 * Before copying code to this file, make sure it's as perfect
 * as you can make it to be, since no debug logs should be
 * recorded here.
 */

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <jansson.h>
#include <global.h>
#include <log.h>

static void write_group(struct command_info *c, json_t *group)
{
	json_t *all = NULL;

	// TODO: add param null check

	all = json_object_get(c->j_config, "groups");
	// TODO: need group name from options!
	json_object_set(all, c->group_name, group);

	d("dumping the root object before dumping to file");
#if defined(MODE_DEBUG)
	{
		char *test = NULL;

		test = json_dumps(c->j_config, JSON_INDENT(4));
		d("\n%s", test);

		free(test);
	}
#endif // MODE_DEBUG

	if(json_dump_file(c->j_config, c->config_path, JSON_INDENT(4)) < 0) {
		d("file dump error!!!"); // error!
	} else {
		d("file dump success");
	}
}

#if 0
static json_t* get_group(struct command_info *c)
{
	//char group[MAX_GROUP_NAME_LEN] = {0};
	json_t *all = NULL;
	json_t *group = NULL;

	all = json_object_get(c->j_config, "groups");

	// TODO: need group name from options!
	group = json_object_get(all, "default");

	return group;
}
#endif

/*
 * description
 * Handler for the 'add' action command.
 * TODO: need to add to actual svn (on svm commit)
 */
void add(struct command_info* c)
{
	int i = 0;
	json_t *group = NULL;
	struct stat fs;
	char rpath[MAX_PATH_LEN] = {0};
	int is_same = 0;

	if (c == NULL) return;
	
	// TODO: need to get actual group from either the -g option or default
	//group = get_group(c);
	group = c->j_group;

	for (i=0; i<c->act_arg_cnt; i++) {
		d("[add] checking if path exists: [%s]", c->act_arg[i]);
		if (access(c->act_arg[i], F_OK) == 0) { // file exists
			d("files exists! [%s]", c->act_arg[i]);
			if (stat(c->act_arg[i], &fs) == 0) { // got file stat
				d("got stats! [%s]", c->act_arg[i]);

				// now to clean up the path received by the user.
				// 1. remove initial './' reveiced, if it exists
				// 2. add '/' to the end if we received a directory
				// 3. then append to the root path
				
				if (c->act_arg[i][0] == '.' && c->act_arg[i][1] == '/') {
					snprintf(rpath, sizeof(rpath), "%s%s", c->cwd, c->act_arg[i]+2);
				} else {
					snprintf(rpath, sizeof(rpath), "%s%s", c->cwd, c->act_arg[i]);
				}
				d("file path final: [%s]", rpath);

				if (S_ISDIR(fs.st_mode)) { // is directory
					if (rpath[strlen(rpath)-1] != '/') {
						rpath[strlen(rpath)] = '/';
					}
				}

				// ? remove root path (i think i accidently deleted this part


				// check dup
				{
					size_t index;
					json_t *value;
					json_array_foreach(group, index, value) {
						d("checking! [%d][%s] ?= [%s]", index, json_string_value(value), rpath);
						if(_strfcmp(json_string_value(value), rpath) == 0) { // if same
							is_same = 1;
							break;
						}
					}
				}

				if(is_same) {
					d("naaaaaaaa you ain't going anywhere bruh");
				} else {
					d("appending! [%s]", rpath);
					json_array_append_new(group, json_string(rpath));
				}
			}
		}
	}

	write_group(c, group);
}
