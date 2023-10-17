/*
 * WARNING!
 * Before copying code to this file, make sure it's as perfect
 * as you can make it to be, since no debug logs should be
 * recorded here.
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <jansson.h>
#include <global.h>
#include <log.h>

/*
 * description
 * Handler for the 'remove' action command.
 *
 * Clean up the path string received by the user
 * 1. Remove initial './' part of string if it exists
 * 2. Add '/' to the end of the path string if file path is a directory
 * 3. Append string to the CWD(Current Working Directory, without svn root)
 */
void _remove(struct command_info* c)
{
	int i = 0;
	struct stat fs;
	char rpath[MAX_PATH_LEN] = {0};

	if (c == NULL) return;

	d("starting 'remove' command handler");

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

				// check if 'rpath' exists
				{
					size_t index;
					json_t *value;

					json_array_foreach (c->j_group, index, value) {
						if (_strfcmp(json_string_value(value), rpath) == 0) {
							d("found matching string! removing from array (%s)", value);
							if (json_array_remove(c->j_group, index) == -1) {
								errout("failed to remove index from array");
							}
							break;
						}
					}
				}

			} else {
				errout("unable to get file stats, critical error (%s)", c->act_arg[i]);
			}
		} else {
			errout("path received does not exist (%s)", c->act_arg[i]);
		}
	}
}
