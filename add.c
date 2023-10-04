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
void add(struct command_info* c)
{
	int i = 0;
	struct stat fs;
	char rpath[MAX_PATH_LEN] = {0};
	int is_same = 0;

	if (c == NULL) return;

	d("starting 'add' command handler");

	for (i=0; i<c->act_arg_cnt; i++) {
		if (access(c->act_arg[i], F_OK) == 0) {
			d("file exists (%s)", c->act_arg[i]);

			if (stat(c->act_arg[i], &fs) == 0) {
				d("stats retrieved (%s)", c->act_arg[i]);
				
				if (c->act_arg[i][0] == '.' && c->act_arg[i][1] == '/') {
					snprintf(rpath, sizeof(rpath), "%s%s", c->cwd, c->act_arg[i]+2);
				} else {
					snprintf(rpath, sizeof(rpath), "%s%s", c->cwd, c->act_arg[i]);
				}

				// check if file path is a directory
				if (S_ISDIR(fs.st_mode)) {
					if (rpath[strlen(rpath)-1] != '/') {
						rpath[strlen(rpath)] = '/';
					}
				}
				d("finalized file path (%s)", rpath);

				// check if 'rpath' has duplicates
				{
					size_t index;
					json_t *value;

					json_array_foreach (c->j_group, index, value) {
						if (_strfcmp(json_string_value(value), rpath) == 0) {
							errout("found duplicate file path, skipping (%s)", value);
							is_same = 1;
							break;
						}
					}
				}

				if (is_same != 1) {
					d("appending new file path to array (%s)", rpath);
					json_array_append_new(c->j_group, json_string(rpath));
				}
			} else {
				errout("unable to get file stats, critical error (%s)", c->act_arg[i]);
			}
		} else {
			errout("path received does not exist (%s)", c->act_arg[i]);
		}
	}
}
