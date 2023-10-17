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
 * Find the x position where the substring is found in string
 *
 * parameter
 * nth = starts from 1
 *
 * return
 * The position where x starts
 */
char* find_substring(int nth, const char* s, const char* ss)
{
	int pos = 0;
	size_t slen = _strlen(s);
	size_t sslen = _strlen(ss);
	int i=0;
	int j=0;
	int is_same;
	int same_cnt = 0;

	if(nth < 0 || s == NULL || ss == NULL) return NULL;

	for(i=0; i<slen; i++) {
		d("initial check [%c] != [%c]", s[i], ss[0]);
		if(s[i] == ss[0]) {
			is_same = 1;
			for(j=0; j<sslen; j++) {
				d("final check [%c] != [%c]", s[i+j], ss[j]);
				if(s[i+j] != ss[j]) {
					is_same = 0;
					break;
				}
			}
			if(is_same) {
				d("SAME!");
				same_cnt++;
				if(same_cnt >= nth) {
					pos = i;
				}
			}
		}
	}

	return (char*)(s+pos);
}
int cnt_total_substring(const char* s, const char* ss)
{
	size_t slen = _strlen(s);
	size_t sslen = _strlen(ss);
	int i=0;
	int j=0;
	int is_same;
	int same_cnt = 0;

	if(s == NULL || ss == NULL) return -1;

	for(i=0; i<slen; i++) {
		//d("initial check [%c] != [%c]", s[i], ss[0]);
		if(s[i] == ss[0]) {
			is_same = 1;
			for(j=0; j<sslen; j++) {
				//d("final check [%c] != [%c]", s[i+j], ss[j]);
				if(s[i+j] != ss[j]) {
					is_same = 0;
					break;
				}
			}
			if(is_same) {
				//d("SAME!");
				same_cnt++;
			}
		}
	}

	return same_cnt;
}

/*
 * description
 * Get finalized, clean path string from user-inputted pathing
 *
 * TODO: need handle for "./test/.././././././feeds.conf"
 * TODO: move to global.c
 */
#include <sys/stat.h>
char* get_finalized_path(struct command_info* c, const char *ori, char *final, size_t final_sz)
{
	struct stat fs;
	int cnt;
	int i;
	//int j;
	char **div = NULL;
	int last = 0;
	int idx = 0;
	int *idx_arr = NULL;
	int farr = 0;

	d("starting --> [%s]", ori);

	if (stat(ori, &fs) == 0) {
		if (_strncmp(ori, "./", 2) == 0) { // has './' at the start of the string
			snprintf(final, final_sz, "%s%s", c->cwd, (char*)(ori+2));
		} else {
			snprintf(final, final_sz, "%s%s", c->cwd, ori);
		}

		// check if file path is a directory
		if (S_ISDIR(fs.st_mode)) {
			if (final[strlen(final)-1] != '/') {
				final[strlen(final)] = '/';
				final[strlen(final)+1] = '\0';
			}
		}

		// testing, needs to be in a loop
		//d("test ------> [%s]", find_substring(0, final, "../"));
		cnt = cnt_total_substring(final, "/") + 1; // for the file
		d("total CNT [%d]", cnt);

		div = (char**)malloc(cnt * sizeof(char*));
		idx_arr = (int*)malloc(cnt * sizeof(int));
		if(div) {
			for(i=0; i<cnt; i++) {
				div[i] = (char*) malloc(MAX_FILENAME_LEN * sizeof(char));
				if(!div[i]) {
					// TODO make a function killer here
					// this is a critical error!
				}
			}
		} else {
			return NULL;
		}

		d("alloc good! final[%s]", final);

		last = 0;
		idx = 0;
		farr = 0;
		for(i=0; i<_strlen(final); i++) {
			// differ by '/'
			// if path is /home/testing/blash/text.txt
			// '/' 'home/' 'testing/' 'blash/' 'text.txt'
			d("check [%c]=='/'", final[i]);
			if(final[i] == '/' || final[i+1] == '\0') {
				d("copying data!!!");
				_strcpy(&final[last], div[idx++], i-last+1);
				d("test");
				last = i+1;
				d("test");
				if(_strfcmp(div[idx-1], "../") == 0) {
					// append index to array
					if(idx-2 < 0) {
						d("unable to go further back than that (some idiot who wants to make the path go beyond root");
					} else {
						idx_arr[farr++] = idx-2;
						idx_arr[farr++] = idx-1;
					}
					d("remove target set [%s] <- [%s]", div[idx-2], div[idx-1]);
				}
				d("test");
			}
		}

		d("test");

		/// need a better algorithm to fix puzzle
		// ?
		char master[MAX_PATH_LEN] = {0};
		int t = 0;
		int j;
		int skip = 0;
		for(i=0; i<idx; i++) {
			skip = 0;
			for (j=0; j<farr; j++) {
				if(i == idx_arr[j]) {
					skip =1;
				}
			}

			if(skip) continue;

			d("DIV -> [%s]", div[i]);
			t += snprintf(master+t, sizeof(master)-t, "%s", div[i]);
		}
		d("real final? if i don't die [%s]", master);
		snprintf(final, final_sz, "%s", master);

		// now just need handle for path strings that goes past the svn root

		for(i=0; i<cnt; i++) {
			free(div[i]);
		}
		free(div);
		if(idx_arr) {
			free(idx_arr);
		}

	} else { // failed to get file info
		return NULL;
	}

	return final; // TODO: hmmmm how to handle this?
}

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
int _add(struct command_info* c)
{
	int return_value = EXIT_NORMAL;
	int i = 0;
	char final_path[MAX_PATH_LEN] = {0};
	int is_same = 0;
	size_t index;
	json_t *value;

	if (c == NULL) {
		d("Critical error, 'c' is NULL");
		return_value = EXIT_ADD_FAILED;
		goto EXIT;
	}

	d("starting 'add' command handler");

	for (i=0; i<c->act_arg_cnt; i++) {
		if (access(c->act_arg[i], F_OK) != 0) {
			d("unable to find file, checking SVN schedule data (%s)", c->act_arg[i]);

			if (get_file_svn_schedule(c->act_arg[i]) != SVN_SCHED_DELETE) {
				d("not in delete schedule, so file doesn't exist (%s)", c->act_arg[i]);
				errout("path received does not exist (%s)", c->act_arg[i]);
				continue;
			}
		}
		d("file exists! (%s)", c->act_arg[i]);

		if (get_finalized_path(c, c->act_arg[i], final_path, sizeof(final_path)) == NULL) {
			d("Critical error, failed to get finalized path");
			continue;
		}
		d("finalized file path (%s)", final_path);

		// check if 'final_path' has duplicates
		is_same = 0;
		json_array_foreach (c->j_group, index, value) {
			if (_strfcmp(json_string_value(value), final_path) == 0) {
				errout("found duplicate file path, skipping (%s)", final_path);
				is_same = 1;
				break;
			}
		}

		if (is_same == 0) {
			d("appending new file path to array (%s)", final_path);
			json_array_append_new(c->j_group, json_string(final_path));
		}
	}

EXIT:
	return return_value;
}
