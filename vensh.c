/* Libraries */
#include "base.h"
#include "errors.h"

/* Directives */
#if defined(__linux__)
	#define EXE_FILEPATH "/proc/self/exe"
#elif defined(__APPLE__)
	#include <mach-o/dyld.h>
#else
	#error "vensh: OS is unsupported"
#endif

/* Prototypes */
void timeout(int signum);
void prompt_locale(void);
int is_supported_locale(char* locale);
void loop(void);
char* readline(void);
char** splitline(char* line);
int execute(char** args);
int C(void);
int L(void);
int launch(char** args);
int cmd_cd(char** args);
int cmd_help(char** args);
int cmd_env(char** args);
int cmd_exit(char** args);
int cmd_setenv(char** args);
int set_env(char* envreference, char* envvalue);
int cmd_prependenv(char** args);
int prepend_env(char* envreference, char* envvalue);
int is_valid_reference(const char *name);
int cmd_unsetenv(char** args);
int cmd_unsetenv(char** args);
int cmd_usage(char** args);
int run_rc(char* filename);
int append_env_to_rc(char *envreference, char *envvalue);
char* slice(char* s, int i);

/* Constants */
char* LOCALELIST[] = {
	"FR",
	"EN"
};

char* CMDS[] = {
	"cd",
	"env",
	"exit",
	"help",
	"setenv",
	"prependenv",
	"unsetenv",
	"usage"
};

int (*FNS[]) (char**) = {
	&cmd_cd,
	&cmd_env,
	&cmd_exit,
	&cmd_help,
	&cmd_setenv,
	&cmd_prependenv,
	&cmd_unsetenv,
	&cmd_usage
};

extern char** environ;

char* CMDUSAGES_EN[] = {
	"vensh: Built-in command \"cd\" is used with 1 argument, as in \"cd <file path>\"",
	"vensh: Built-in command \"env\" is used with no argument, as in \"env\"",
	"vensh: Built-in command \"exit\" is used with no argument, as in \"exit\"",
	"vensh: Built-in command \"help\" is used with no argument, as in \"help\"",
	"vensh: Built-in command \"setenv\" is used with 2 arguments, as in \"setenv <environment variable name> <environment variable value>\"",
	"vensh: Built-in command \"prependenv\" is used with 2 arguments, as in \"prependenv <environment variable name> <environment variable value>\"",
	"vensh: Built-in command \"unsetenv\" is used with 1 argument, as in \"unsetenv <environment variable name>\"",
	"vensh: Built-in command \"usage\" is used with 1 argument, as in \"usage <built-in command name>\""
};

char* CMDUSAGES_FR[] = {
	"vensh: La commande intégrée \"cd\" s'utilise avec 1 argument, par exemple \"cd <chemin du fichier>\"",
	"vensh: La commande intégrée \"env\" s'utilise sans argument, par exemple \"env\"",
	"vensh: La commande intégrée \"exit\" s'utilise sans argument, par exemple \"exit\"",
	"vensh: La commande intégrée \"help\" s'utilise sans argument, par exemple \"help\"",
	"vensh: La commande intégrée \"setenv\" s'utilise avec 2 arguments, par exemple \"setenv <nom de la variable d'environnement> <valeur de la variable d'environnement>\"",
	"vensh: La commande intégrée \"prependenv\" s'utilise avec 2 arguments, par exemple \"prependenv <nom de la variable d'environnement> <valeur de la variable d'environnement>\"",
	"vensh: La commande intégrée \"unsetenv\" s'utilise avec 1 argument, par exemple \"unsetenv <nom de la variable d'environnement>\"",
	"vensh: La commande intégrée \"usage\" s'utilise avec 1 argument, par exemple \"usage <nom de la commande intégrée>\""
};

/* Functions */
int main(int argc, char** argv)
{
	char exe_filepath[PATH_MAX];

	#if defined(__linux__)
		ssize_t len = readlink(EXE_FILEPATH, exe_filepath, sizeof(exe_filepath) - 1);
		if (len == -1) {
			perror("readlink");
			exit(1);
		}
		exe_filepath[len] = '\0';

	#elif defined(__APPLE__)
		uint32_t exe_filepath_size = sizeof(exe_filepath);
		if (_NSGetExecutablePath(exe_filepath, &exe_filepath_size) != 0) {print_expected_filepath_length("shorter"); exit(1);}
		char real_path[PATH_MAX];
		if (realpath(exe_filepath, real_path) == NULL) {print_realpath_failure(exe_filepath); exit(1);}
		strncpy(exe_filepath, real_path, PATH_MAX);
	#endif

	// Extract the directory from the executable path
	char *exe_dir = dirname(exe_filepath);

	// Change the current working directory to the executable's directory
	if (chdir(exe_dir) != 0) {print_cd_failure(exe_dir); exit(1);}

	// Create runtime configuration if it does not exist.
	if (fopen("vensh.rc", "r") == NULL) fopen("vensh.rc", "w");

	// Get current working directory and set VENSHROOT, BACKUP_DIR, VENSHBIN, PATH, VENSHASSETS, VENSHLOGS, and VENSHBACKUPS.
	char cwd[PATH_MAX];
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
		char* rootfilepath = (char*) malloc(sizeof(cwd) + 1);
		if (rootfilepath == NULL) {print_malloc_failure(); exit(1);}
		sprintf(rootfilepath, "%s", cwd);
		set_env("VENSHROOT", rootfilepath);
		set_env("BACKUP_DIR", rootfilepath);

		char* binfilepath = (char*) malloc(sizeof(cwd) + strlen("/bin") + 1);
		if (binfilepath == NULL) {print_malloc_failure(); exit(1);}
		sprintf(binfilepath, "%s/bin", cwd);
		if (opendir(binfilepath) == NULL && mkdir(binfilepath, 0777) == -1) {print_mkdir_failure(binfilepath); exit(1);}
		set_env("VENSHBIN", binfilepath);
		prepend_env("PATH", binfilepath);

		char* assetsfilepath = (char*) malloc(sizeof(cwd) + strlen("/assets") + 1);
		if (assetsfilepath == NULL) {print_malloc_failure(); exit(1);}
		sprintf(assetsfilepath, "%s/assets", cwd);
		if (opendir(assetsfilepath) == NULL && mkdir(assetsfilepath, 0777) == -1) {print_mkdir_failure(assetsfilepath); exit(1);}
		set_env("VENSHASSETS", assetsfilepath);

		char* logsfilepath = (char*) malloc(sizeof(cwd) + strlen("/logs") + 1);
		if (logsfilepath == NULL) {print_malloc_failure(); exit(1);}
		sprintf(logsfilepath, "%s/logs", cwd);
		if (opendir(logsfilepath) == NULL && mkdir(logsfilepath, 0777) == -1) {print_mkdir_failure(logsfilepath); exit(1);}
		set_env("VENSHLOGS", logsfilepath);

		char* backupsfilepath = (char*) malloc(sizeof(cwd) + strlen("/backups") + 1);
		if (backupsfilepath == NULL) {print_malloc_failure(); exit(1);}
		sprintf(backupsfilepath, "%s/backups", cwd);
		if (opendir(backupsfilepath) == NULL && mkdir(backupsfilepath, 0777) == -1) {print_mkdir_failure(backupsfilepath); exit(1);}
		set_env("VENSHBACKUPS", backupsfilepath);

		free(binfilepath);
		free(assetsfilepath);
		free(logsfilepath);
		free(backupsfilepath);
	}

	// Load runtime configuration.
	run_rc("vensh.rc");

	// Set LOCALE.
	char *locale = getenv("LOCALE");
	if (locale == NULL) prompt_locale();
	else if (is_supported_locale(locale) == 0) {unsetenv("LOCALE"); set_env("LOCALE", "EN");}

	// Enable idle timeout mechanism.
	if (signal(SIGALRM, timeout) == SIG_ERR) {print_signal_failure("SIGALRM", "timeout"); exit(1);}
	alarm(IDLE_TIME_LIMIT);

	// Set SHELL.
	setenv("SHELL", strcat(getenv("VENSHROOT"), "/vensh"), 1);

	// Loop continuously.
	loop();

	return 0;
}

void timeout(int signum)
{
	print_expected_activity();
	exit(0);
}

void prompt_locale(void)
{
	char input[1024];
	int choice;
	printf("\n👋\n");
	printf("1 = English (EN)\n");
	printf("2 = Français (FR)\n");
	do {
		fgets(input, 1024, stdin);
		choice = atoi(input);
	} while (choice != 1 && choice != 2);
	unsetenv("LOCALE");
	if (choice == 1) {set_env("LOCALE", "EN"); append_env_to_rc("LOCALE", "EN");}
	else {set_env("LOCALE", "FR"); append_env_to_rc("LOCALE", "FR");}
}

int is_supported_locale(char* locale)
{
	for (int i = 0; i < L(); i++) {
		if (strcmp(locale, LOCALELIST[i]) == 0) return 1;
	}
	return 0;
}

// Read and execute input in a loop.
void loop(void)
{
	int runstatus;
	do {
		char cwd[PATH_MAX];
		if (getcwd(cwd, sizeof(cwd)) != NULL) {
			printf(COLOUR_BLUE "%s " COLOUR_RESET COLOUR_PINK ">w< " COLOUR_RESET, cwd);
		}
		char* line = readline();
		char** args = splitline(line);
		runstatus = execute(args);
		free(line);
		free(args);
	} while(runstatus);
}

// Read and return line from stdin.
char* readline(void)
{
	// Initialise line buffer size to 1024.
	int buffersize = 1024;
	int position = 0;
	int nextchar;

	char* buffer = malloc(sizeof(char) * buffersize);
	if (!buffer) {print_malloc_failure(); exit(1);}

	while (1) {
		nextchar = getchar();
		// If next character is EOF, replace it with null and return.
		if (nextchar == EOF || nextchar == '\n') {buffer[position] = '\0'; return buffer;}
		// Otherwise, "append" nextchar to buffer.
		else buffer[position] = nextchar;
		// Increment position.
		position++;
		// If position exceeds buffer size, double buffer size and reallocate memory.
		if (position >= buffersize) {
			buffersize += 1024;
			buffer = realloc(buffer, buffersize);
			if (!buffer) {print_malloc_failure(); exit(1);}
		}
	}
}

char** splitline(char* line)
{
	// Initialise token buffer size to 64.
	int buffersize = 64;
	int position = 0;
	char** tokens = malloc(buffersize * sizeof(char*));
	char* token;
	if (!tokens) {print_malloc_failure(); exit(1);}

	token = strtok(line, LINE_DELIMITER);
	while (token != NULL) {
		tokens[position] = token;
		// Increment position.
		position++;
		// If position exceeds buffer size, double buffer size and reallocate memory.
		if (position >= buffersize) {
			buffersize *= 2;
			tokens = realloc(tokens, buffersize * sizeof(char*));
			if (!tokens) {print_malloc_failure(); exit(1);}
		}
		token = strtok(NULL, LINE_DELIMITER);
	}

	tokens[position] = NULL;
	return tokens;
}

// Run function or launch program.
int execute(char** args)
{
	// Line := Delimited sequence of words.
	if (args[0] == NULL) return 1;
	// Iterate over built-in commands: if first word entered matches any command name, try to run corresponding function.
	for (int i = 0; i < C(); i++) if (strcmp(args[0], CMDS[i]) == 0) return (*FNS[i])(args);
	// Otherwise, try to launch corresponding program.
	launch(args);
	return 1;
}

// Return calculated number of built-in commands.
int C()
{
	return sizeof(CMDS) / sizeof(char*);
}

// Return calculated number of locales.
int L()
{
	return sizeof(LOCALELIST) / sizeof(char*);
}

// Launch program.
int launch(char** args)
{
	pid_t pid, wpid;
	int status;
	pid = fork();
	if (pid == 0) {
		if (execvp(args[0], args) == -1) print_launch_failure(args[0]);
		exit(1);
	}
	else {
		do {
			wpid = waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	return 1;
}

// Change directory.
int cmd_cd(char** args)
{
	int argc = 0;
	while (args[argc] != NULL) argc++;
	if (argc != 2) {print_expected_argc("1"); return 1;}
	if (chdir(args[1]) != 0) {print_cd_failure(args[1]);}
	return 1;
}

// Print list of built-in commands.
int cmd_help(char** args)
{
	if (strcmp(getenv("LOCALE"), "EN") == 0) printf("vensh: Les commandes intégrées sont ");
	else printf("vensh: Built-in commands are ");
	for (int i = 0; i < C()-1; i++) printf("\"%s\", ", CMDS[i]);
	printf("\"%s\"\n", CMDS[C()-1]);
	return 1;
}

// List environment variables.
// Credits: https://stackoverflow.com/a/12059006.
int cmd_env(char** args)
{
	char** s = environ;
	for (; *s; s++) printf("%s\n", *s);
	return 1;
}

int cmd_exit(char** args)
{
	return 0;
}

// Set environment variable.
int cmd_setenv(char** args)
{
	int argc = 0;
	while (args[argc] != NULL) argc++;
	if (argc != 2) {print_expected_argc("1"); return 1;}

	if (strchr(args[1], '=') == NULL) {print_expected("="); return 1;}
	char* equalposition = strchr(args[1], '=');
	int envreferencelength = equalposition - args[1];
	int envvaluelength = strlen(equalposition + 1);
	if (envreferencelength == 0 || envreferencelength > 255) {print_expected_envreferencelength(); return 1;}
	if (envvaluelength == 0 || envvaluelength > 1023) {print_expected_envvaluelength(); return 1;}

	char envreference[envreferencelength + 1];
	strncpy(envreference, args[1], envreferencelength);
	envreference[envreferencelength] = '\0';
	char* envvalue = equalposition + 1;
	if (is_valid_reference(envreference) == 0) {print_expected_valid_envreference(); return 1;}

	set_env(envreference, envvalue);
	return 1;
}

int set_env(char* envreference, char* envvalue)
{
	if (setenv(envreference, envvalue, 1) != 0) {print_setenv_failure(envreference, envvalue); return 1;}
	return 0;
}

// Prepend environment variable.
int cmd_prependenv(char** args)
{
	int argc = 0;
	while (args[argc] != NULL) argc++;
	if (argc != 2) {print_expected_argc("1"); return 1;}

	if (strchr(args[1], '=') == NULL) {print_expected("="); return 1;}
	char* equalposition = strchr(args[1], '=');
	int envreferencelength = equalposition - args[1];
	int envvaluelength = strlen(equalposition + 1);
	if (envreferencelength == 0 || envreferencelength > 255) {print_expected_envreferencelength(); return 1;}
	if (envvaluelength == 0 || envvaluelength > 1023) {print_expected_envvaluelength(); return 1;}

	char envreference[envreferencelength + 1];
	strncpy(envreference, args[1], envreferencelength);
	envreference[envreferencelength] = '\0';
	char* envvalue = equalposition + 1;
	if (is_valid_reference(envreference) == 0) {print_expected_valid_envreference(); return 1;}

	prepend_env(envreference, envvalue);
	return 1;
}

int prepend_env(char* envreference, char* envvalue)
{
	if (getenv(envreference) == NULL) {
		if (setenv(envreference, envvalue, 1) != 0) {print_setenv_failure(envreference, envvalue); return 1;}
	}
	else {
		char* newenvvalue = (char*) malloc(strlen(getenv(envreference)) + strlen(envvalue) + 2);
		if (newenvvalue == NULL) {print_malloc_failure(); exit(1);}
		sprintf(newenvvalue, "%s:%s", envvalue, getenv(envreference));
		if (setenv(envreference, newenvvalue, 1) != 0) {print_setenv_failure(envreference, newenvvalue); return 1;}
		free(newenvvalue);
	}
	return 0;
}

// Reference := Variable name.
// Return 1 if input forms syntactically-valid reference, or 0 otherwise.
int is_valid_reference(const char *reference)
{
	if (!isalpha(reference[0]) && reference[0] != '_') return 0;
	for (const char *p = reference; *p; p++) if (!isalnum(*p) && *p != '_') return 0;
	return 1;
}

// Try to unset environment variable.
int cmd_unsetenv(char** args)
{
	int argc = 0;
	while (args[argc] != NULL) argc++;
	if (argc != 2) {print_expected_argc("1"); return 1;}

	if (getenv(args[1]) == NULL) print_expected_env();
	else unsetenv(args[1]);
	return 1;
}

// Print usage for each built-in command.
int cmd_usage(char** args)
{
	int argc = 0;
	while (args[argc] != NULL) argc++;
	if (argc != 2) {print_expected_argc("1"); return 1;}

	// Iterate over built-in commands: if second word entered matches any command name, print usage for corresponding command.
	for (int i = 0; i < C(); i++) {
		if (strcmp(args[1], CMDS[i]) == 0) {
			char* locale = getenv("LOCALE");
			if (locale == NULL) {print_expected_env(); return 1;}
			if (strcmp(locale, "EN") == 0) {printf("%s\n", CMDUSAGES_EN[i]); return 1;}
			else {printf("%s\n", CMDUSAGES_FR[i]); return 1;}
		}
	}
	print_expected("built-in command");
	return 1;
}

// Load runtime configuration.
int run_rc(char* rcfilename)
{
	FILE* rcfile = fopen(rcfilename, "r");
	if (rcfile == NULL) {print_expected("file"); return 1;}

	char line[1024];
	while (fgets(line, sizeof(line), rcfile)) {
		line[strcspn(line, "\n")] = '\0';
		if (strncmp(line, "PATH=", strlen("PATH=")) == 0) {
			char* extracted_path = slice(line, strlen("PATH="));
			set_env("PATH", extracted_path);
		}
		else if (strncmp(line, "LOCALE=", strlen("LOCALE=")) == 0) {
			char* extracted_locale = slice(line, strlen("LOCALE="));
			set_env("LOCALE", extracted_locale);
		}
		else if (strncmp(line, "ARTFILENAME=", strlen("ARTFILENAME=")) == 0) {
			char* extracted_artfilename = slice(line, strlen("ARTFILENAME="));
			set_env("ARTFILENAME", extracted_artfilename);
		}
		else if (strncmp(line, "WA_APPID=", strlen("WA_APPID=")) == 0) {
			char* extracted_wa_appid = slice(line, strlen("WA_APPID="));
			set_env("WA_APPID", extracted_wa_appid);
		}
		else if (strncmp(line, "BACKUP_DIR=", strlen("BACKUP_DIR=")) == 0) {
			char* extracted_backup_dir = slice(line, strlen("BACKUP_DIR="));
			set_env("BACKUP_DIR", extracted_backup_dir);
		}
		else {
			char** args = splitline(line);
			execute(args);
			free(args);
		}
	}
	fclose(rcfile);
	return 0;
}

int append_env_to_rc(char *envreference, char *envvalue) {
	FILE *rcfile = fopen("vensh.rc", "a+");
	if (rcfile == NULL) {print_fopen_failure("vensh.rc"); return 1;}

	fseek(rcfile, 0, SEEK_END);
	long rcfilesize = ftell(rcfile);
	if (rcfilesize <= 0) {
		fprintf(rcfile, "%s=%s", envreference, envvalue);
		fclose(rcfile);
		return 0;
	}

	char lastchar;
	// Backtrack to find last non-whitespace character.
	do {
		fseek(rcfile, -2, SEEK_CUR);
		lastchar = fgetc(rcfile);
	} while (isspace(lastchar) && rcfilesize-- > 1);
	if (lastchar != '\n') fprintf(rcfile, "\n");

	fprintf(rcfile, "%s=%s", envreference, envvalue);
	fclose(rcfile);
	return 0;
}

char* slice(char* s, int i) {
	int l = 0;
	while (s[i] != '\0' && s[i] != '\n') {l++; i++;}
	char *extracted = (char*) malloc(l + 1);
	if (extracted == NULL) {print_malloc_failure(); exit(1);}
	strncpy(extracted, s + i - l, l);
	extracted[l] = '\0';
	return extracted;
}