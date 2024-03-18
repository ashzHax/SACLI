#include <jansson.h>
#include <global.h>
#include <log.h>

/*
 * description
 * For handling 'svn st' resulting paths, and getting only the path
 * without the status indicating characters
 */
static char* svn_st_cut(char* s, size_t ss)
{
	int i;
	char c[MAX_PATH_LEN] = {0};

	if (s == NULL || ss <= 0) return NULL;

	// find where the path starts
	for (i=3; s[i]==' '; i++) {};

	snprintf(c, sizeof(c), "%s", &(s[i]));
	snprintf(s, ss, "%s", c);

	return s;
}

/*
 * description
 * Handler for the 'revert' action command.
 */
int _auto(struct command_info* c)
{
	int return_value = EXIT_NORMAL;

	FILE *_file = NULL;
	char path[MAX_PATH_LEN] = {0};
	char cmd[MAX_CMD_LEN + MAX_PATH_LEN] = {0};

	FILE *p = NULL;
	char target[MAX_PATH_LEN] = {0};
	char final_path[MAX_PATH_LEN] = {0};

	int flag = 0;

	size_t index;
	json_t *value;

	if (c == NULL) {
		errout("command_info is NULL");
		return_value = EXIT_AUTO_ERROR;
		goto EXIT;
	}

	d("starting 'auto' command handler");

	// read auto target list file
	_file = fopen(c->auto_path, "r");
	if (_file == NULL) {
		errout("failed to open file [%s]", c->auto_path);
		errout("if file does not exist, run 'svm target'");
		return_value = EXIT_AUTO_ERROR_FILE_OPEN_FAIL;
		goto EXIT;
	}

	// run 'svn st' on every entry in the auto target list file
	while (fgets(path, sizeof(path), _file) != NULL) {
		snprintf(cmd, sizeof(cmd), "svn st %s", path);
		rm_whitespace(cmd);

		p = popen(cmd, "r");
		if (p == NULL) {
			continue;
		}

		// read the result of the command
		while(fgets(target, sizeof(target), p) != NULL) {
			rm_whitespace(target);
			svn_st_cut(target, sizeof(target));
			d("rm_whitespace() & svn_st_cut() result [%s]", target);

			if (get_clean_path(c, target, final_path, sizeof(final_path)) == NULL) {
				errout("failed to find valid file, ignoring [%s]", target);
				// no error on this case, in error causes are normally excusable
				continue;
			}
			d("found valid file [%s]", final_path);
			
			// now the same routine as 'add'
			// check if 'final_path' has duplicates
			flag = 0;
			json_array_foreach (c->json_files, index, value) {
				if (_strfcmp(json_string_value(value), final_path) == 0) {
					errout(
					"already have file [%s] inside group [%s], skipping",
					final_path, c->grp_name);
					flag = 1;
					break;
				}
			}

			// if the 'final_path' does not exist within the group, add it
			if (flag == 0) {
				out("adding [%s] to group [%s]", final_path, c->grp_name);
				json_array_append_new(c->json_files, json_string(final_path));
			}
		}
		pclose(p);
	}
	fclose(_file);

EXIT:
	return return_value;
}
