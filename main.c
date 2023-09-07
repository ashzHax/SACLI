/* 
 * by     : ashz
 * reason : main code interface for handling arguments and running sub-systems
 * tab    : 4 spaces, 1 tab character
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <jansson.h>
#include <global.h>

struct command_info cmd;
#define CFG_FILE ".c.json"
#define get_cfg_path(r) r""CFG_FILE

/*
 * note
 * string list of command modes, for sifting user input
 * list order must match enum defined in global.h
 */
const char* mode_list[MODE_MAX] = {
	"add",
	"remove",
	"show",
	"comment",
	"commit",
	"edit",
	"help",
	"info",
	"clean",
	"revert",
	"rollback"
};

/*
 * description
 * Remove whitespaces at the end of string
 * (null, next line, spaces)
 */
static void remove_end_whitespaces(char *b)
{
	int i = 0;
	size_t len = 0;

	len = _strlen(b);

	for (i=len-1; i>=0; i++) {
		if (b[i] == '\0' || b[i] == '\n' || b[i] == ' ') {
			b[i] = '\0';
		} else  {
			return;
		}
	}
}

/*
 * description
 * Finds the action argument, then divide option and action
 * arguments
 */
static void option_parse(int argc, char** argv)
{
	int i = 0;
	int j = 0;
	char argt[MAX_ARG_LEN] = {0};
	int idx_action = 0;

	// debugging purposes
	for(i = 0; i < argc; i++) {
		d("argv[%d] = %s", i, argv[i]);
	}

	for (i=1; i<argc; i++) {
		// less efficient, since doing it this way makes 1024 bytes be written
		// all the time, but this function only runs once at init
		_strcpy(argv[i], argt, MAX_ARG_LEN); 
		_strlow(argt);
		
		d("checking [%s]...", argt);

		// finding action parameter
		for (j=0; j<MODE_MAX; j++) {
			if (_strfcmp(argt, mode_list[j]) == 0) {
				cmd.mode = j;
				break;
			}
		}

		if (cmd.mode != MODE_INVALID) {
			cmd.act_arg_cnt = argc - i - 1; // cannot go below 0 (in theory)
			idx_action = i;
			break;
		}
		cmd.opt_arg_cnt++;
	}
	d("idx_action = %d, act_arg_cnt=%d", idx_action, cmd.act_arg_cnt);

	cmd.opt_arg = (const char**)malloc(sizeof(const char*)*cmd.opt_arg_cnt);
	for (i=0; i<cmd.opt_arg_cnt; i++) {
		cmd.opt_arg[i] = argv[i+1];
		d("opt_args[%d] = argv[%d] = %s", i, i+1, argv[i+1]);
	}

	cmd.act_arg = (const char**)malloc(sizeof(const char*)*cmd.act_arg_cnt);
	for (i=0; i<cmd.act_arg_cnt; i++) {
		cmd.act_arg[i] = argv[idx_action+1+i];
		d("act_args[%d] = argv[%d] = %s", i, idx_action+1+i, argv[idx_action+1+i]);
	}
}

/*
 * description
 * Finds the root directory of the current SVN repository
 *
 * return
 * '-1' if the current path is not a valid SVN repository (program should exit)
 * '0' on normal
 */
static int find_root_dir(char **buf)
{
	FILE *p;

	*buf = (char*)malloc(sizeof(char)*MAX_PATH_LEN);

	if (buf == NULL) {
		return EXIT_ERROR;
	}

	p = popen("svn info 2> /dev/null | grep \"Working Copy Root Path\" | awk \'{print $5}\'", "r");

	if(p) {
		fgets(*buf, MAX_PATH_LEN, p);
		pclose(p);
	}

	if(_strlen(*buf) <= 0) {
		return EXIT_NOT_IN_SVN_REPO;
	}

	remove_end_whitespaces(*buf);

	return 0;
}

/*
 * description
 * Show the user how to use this amazing command
 */
static void help()
{
	out("usage: svm [options] [action] [...]");
}

/*
 * description
 * Creates the configuration file,
 * while filling it in with the basic values
 */
static int create_file()
{
	FILE *f = NULL;
	char buf[MAX_PATH_LEN] = {0};
	json_t *new = NULL;
	json_t *groups = NULL;

	snprintf(buf, MAX_PATH_LEN, "%s/%s", cmd.root_path, CFG_FILE);
	f = fopen(buf, "w+");
	if (f == NULL) {
		return EXIT_UNABLE_TO_CREATE_FILE;
	} 

	new = json_object();
	groups = json_object();

	if (new != NULL && groups != NULL) {
		json_object_set(groups, "default", json_object());
		json_object_set(new, "groups", groups);
		json_dumpf(new, f, JSON_INDENT(4));
	}

	json_decref(new);
	fclose(f);

	return 0;
}

/*
 * description
 * The initiation function.
 * Not much, currently. But you wait.
 * I waited, and got a whole lot more than what I asked for.
 */
static int init()
{
	FILE *cfg_file = NULL;
	int r = 0; // return value
	json_error_t jerr; // jansson error handling
	char cfg_file_path[MAX_PATH_LEN] = {0};

	memset(&cmd, 0x0, sizeof(struct command_info));

	// get root path
	r = find_root_dir(&cmd.root_path);
	if (r < 0) {
		return r;
	}
	d("svn root path: [%s]", cmd.root_path);

	// get configuration file path
	snprintf(cfg_file_path, MAX_PATH_LEN, "%s/%s", cmd.root_path, CFG_FILE);
	d("configuration file path: [%s]", cfg_file_path);

	// check configuration file existence, if not, create file
	if (access(cfg_file_path, F_OK) != 0) {
		r = create_file();
		if (r < 0) {
			return r;
		}
	}

	// open configuration file
	cfg_file = fopen(cfg_file_path, "r");
	if (cfg_file == NULL) {
		return EXIT_UNABLE_TO_OPEN_FILE;
	}

	// load configuration file as JSON
	cmd.cfg_obj = json_loadf(cfg_file, (JSON_DECODE_ANY | JSON_DISABLE_EOF_CHECK), &jerr);

	// dump and output JSON object 
#if defined(MODE_DEBUG)
	{
		char *test = NULL;

		test = json_dumps(cmd.cfg_obj, JSON_INDENT(4));
		d("\n%s", test);

		free(test);
	}
#endif // MODE_DEBUG

	fclose(cfg_file);
	return r;
}

/*
 * description
 * The de-initiation function.
 * Very important if you don't want un-managed memory
 * lingering in your system.
 */
static void deinit()
{
	free(cmd.opt_arg);
	free(cmd.act_arg);

	free(cmd.root_path);

	d("de-initalized all variables");
}



/*
 * Received the 'add' action command, let's go.
 * 
 * description
 * Handler for the 'add' action command.
 *
 * TODO: need to check if arguments are valid
 */
void add(struct command_info* c)
{
#if 0 // for next commit
	int i = 0;

	// add to file (add to svn when doing actual commit
#if 0
	// basically, need json handler about now ^^;

	if (option_is_bad) {
		exit_error_all_the_shebang;
	}

	while(action_arg_cnt) {
		if(check_action_argument_path_valid(arg[i]) = good){
			add_path_to_group_array(group, arg);
		}
	}
#endif
	
	for (i=0; i<c->act_arg_cnt; i++) {
		if (access(c->act_arg[i], F_OK) == 0) {
			
		}
	}
#endif
}


/*
 * description
 * It's the most important function.
 * Thats it.
 */
int main(int argc, char** argv)
{
	int exit_code = EXIT_NORMAL;

	exit_code = init();
	if (exit_code < 0) {
		goto END_PROG;
	}

	option_parse(argc, argv);

	switch(cmd.mode) {
		case MODE_ADD:
		{
			add(&cmd);
			d("running \"ADD\" function");
			break;
		}
		default:
		{
			help();
			exit_code = EXIT_ACTION_NOT_FOUND;
			d("found no action argument, ending program");
		}
	}

END_PROG:
	deinit();

	switch(exit_code) {
		case EXIT_NOT_IN_SVN_REPO: {
			errout("You are not inside a SVN repository! [-.-]");
			break;
		}
		case EXIT_NORMAL: {
			out("Finished. [^.^][%d]", exit_code);
			break;
		}
		default: {
			out("Unknown exit code.");
		}
	}

	return exit_code;
}
