/* Libraries */
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/sysctl.h>
#include <dirent.h>
#include <errno.h>

/* Prototypes */
void loop(void);
char* readline(void);
char** splitline(char* line);
int execute(char** args);
int N(void);
int launch(char** args);

int cmd_cd(char** args);

int cmd_help(char** args);

int cmd_env(char** args);

int cmd_exit(char** args);

int cmd_setenv(char** args);
int is_valid_reference(const char *name);

int cmd_unsetenv(char** args);

int cmd_unsetenv(char** args);

int cmd_usage(char** args);

int run_rc(const char* filename);
int safeset_env(char* reference, char* value);
char* slice(const char *s, int i);

void print_malloc_failure(void);
void print_setenv_failure(char* envreference, char* envvalue);
void print_safeset_env_failure(char* envreference, char* envvalue);

/* Constants */
char* COMMANDS[] = {
	"cd",
	"env",
	"exit",
	"help",
	"setenv",
	"unsetenv",
	"usage"
};

int (*FUNCTIONS[]) (char**) = {
	&cmd_cd,
	&cmd_env,
	&cmd_exit,
	&cmd_help,
	&cmd_setenv,
	&cmd_unsetenv,
	&cmd_usage
};

extern char** environ;

char* USAGES[] = {
	"vensh: Built-in command \"cd\" is used with 1 argument, as in \"cd <file path>\"",
	"vensh: Built-in command \"env\" is used with no argument, as in \"env\"",
	"vensh: Built-in command \"exit\" is used with no argument, as in \"exit\"",
	"vensh: Built-in command \"help\" is used with no argument, as in \"help\"",
	"vensh: Built-in command \"setenv\" is used with 2 arguments, as in \"setenv <process environment variable name> <process environment variable value>\"",
	"vensh: Built-in command \"unsetenv\" is used with 1 argument, as in \"unsetenv <process environment variable name>\"",
	"vensh: Built-in command \"usage\" is used with 1 argument, as in \"usage <built-in command name>\""
};

/* Functions */
int main(int argc, char** argv)
{
	// Load configurations.
	static int first = 1;
	if (first == 1)
	{
		#ifdef _WIN32
			system("cls");
		#else
			system("clear");
		#endif
		first = 0;
	}
	fflush(stdout);
	// Try to safely set PATH and VENSHBIN to bindirectory if it exists.
	char cwd[PATH_MAX];
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
		DIR* bindirectory = opendir("bin");
		DIR* assetsdirectory = opendir("assets");
		if (ENOENT != errno) {
			char* binfilepath = (char*) malloc(sizeof(cwd) + 5);
			if (binfilepath == NULL) {print_malloc_failure(); exit(1);}
			sprintf(binfilepath, "%s/bin", cwd);
			if (safeset_env("PATH", binfilepath) != 0) print_safeset_env_failure("PATH", binfilepath);
			if (safeset_env("VENSHBIN", binfilepath) != 0) print_safeset_env_failure("VENSHBIN", binfilepath);
			free(binfilepath);

			char* assetsfilepath = (char*) malloc(sizeof(cwd) + 8);
			if (assetsfilepath == NULL) {print_malloc_failure(); exit(1);}
			sprintf(assetsfilepath, "%s/assets", cwd);
			if (safeset_env("VENSHASSETS", assetsfilepath) != 0) print_safeset_env_failure("VENSHASSETS", assetsfilepath);
			free(assetsfilepath);
		}
	}
	run_rc("vensh.rc");
	// Loop continuously.
	loop();
	// Exit.
	return 0;
}

// Read and execute input in a loop.
void loop(void)
{
	int runstatus;
	do {
		char cwd[PATH_MAX];
		if (getcwd(cwd, sizeof(cwd)) != NULL) {
			printf("%s >w< ", cwd);
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
	char* buffer = malloc(sizeof(char) * buffersize);
	int nextchar;

	if (!buffer) {
		print_malloc_failure();
		exit(1);
	}
	while (1) {
		nextchar = getchar();
		// If next character is EOF, replace it with null and return.
		if (nextchar == EOF || nextchar == '\n') {
			buffer[position] = '\0';
			return buffer;
		}
		// Otherwise, "append" nextchar to buffer.
		else {
			buffer[position] = nextchar;
		}
		// Increment position.
		position++;
		// If position exceeds buffer size, double buffer size and reallocate memory.
		if (position >= buffersize) {
			buffersize += 1024;
			buffer = realloc(buffer, buffersize);
			if (!buffer) {
				print_malloc_failure();
				exit(1);
			}
		}
	}
}

#define delimiter " \t\r\n\a"
char** splitline(char* line)
{
	// Initialise token buffer size to 64.
	int buffersize = 64;
	int position = 0;
	char** tokens = malloc(buffersize * sizeof(char*));
	char* token;
	if (!tokens) {
		print_malloc_failure();
		exit(1);
	}
	token = strtok(line, delimiter);
	while (token != NULL) {
		tokens[position] = token;
		// Increment position.
		position++;
		// If position exceeds buffer size, double buffer size and reallocate memory.
		if (position >= buffersize) {
			buffersize *= 2;
			tokens = realloc(tokens, buffersize * sizeof(char*));
			if (!tokens) {
				print_malloc_failure();
				exit(1);
			}
		}
		token = strtok(NULL, delimiter);
	}
	tokens[position] = NULL;
	return tokens;
}

// Try to run function or launch program.
int execute(char** args)
{
	// Line := Delimited sequence of words.
	if (args[0] == NULL) return 1;
	// Iterate over commands: if first word entered matches any command name, try to run corresponding function.
	for (int i = 0; i < N(); i++) if (strcmp(args[0], COMMANDS[i]) == 0) return (*FUNCTIONS[i])(args);
	// Otherwise, try to launch corresponding program.
	launch(args);
	return 1;
}

// Return calculated number of commands.
int N(void)
{
	return sizeof(COMMANDS) / sizeof(char *);
}

// Try to launch program.
int launch(char** args)
{
	pid_t pid, wpid;
	int status;
	pid = fork();
	if (pid == 0) {
		// Print "vensh: No such file or directory".
		if (execvp(args[0], args) == -1) perror("vensh");
		exit(1);
	}
	else {
		do {
			wpid = waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	return 1;
}

// Try to change directory.
int cmd_cd(char** args)
{
	int argc = 0;
	while (args[argc] != NULL) argc++;
	if (argc != 2) {fprintf(stderr, "vensh: Expected 1 argument\n"); return 1;}
	if (chdir(args[1]) != 0) perror("vensh");
	return 1;
}

// Print list of commands.
int cmd_help(char** args)
{
	printf("vensh: Built-in commands are ");
	for (int i = 0; i < N()-1; i++) printf("\"%s\", ", COMMANDS[i]);
	printf("and \"%s\"\n", COMMANDS[N()-1]);
	return 1;
}

// List process environment variables.
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

// Try to safely set process environment variable in format "REFERENCE=VALUE".
int cmd_setenv(char** args)
{
	// Validates words.
	int argc = 0;
	while (args[argc] != NULL) argc++;
	if (argc != 2) {fprintf(stderr, "vensh: Expected 1 argument\n"); return 1;}

	if (strchr(args[1], '=') == NULL) {fprintf(stderr, "vensh: Expected '='\n"); return 1;}

	char* equalposition = strchr(args[1], '=');
	int envreferencelength = equalposition - args[1];
	int envvaluelength = strlen(equalposition + 1);
	if (envreferencelength == 0 || envreferencelength > 255) {fprintf(stderr, "vensh: Expected process environment variable name length between 1 and 255 characters (inclusive)\n"); return 1;}
	if (envvaluelength == 0 || envvaluelength > 1023) {fprintf(stderr, "vensh: Expected process environment variable value length between 1 and 1023 characters (inclusive)\n"); return 1;}

	char envreference[envreferencelength + 1];
	strncpy(envreference, args[1], envreferencelength);
	envreference[envreferencelength] = '\0';
	char* envvalue = equalposition + 1;
	if (is_valid_reference(envreference) == 0) {printf("vensh: Expected valid process environment variable name"); return 1;}

	if (safeset_env(envreference, envvalue) != 0) print_safeset_env_failure(envreference, envvalue);
	return 1;
}

// Reference := Variable name.
// Return 1 if input forms syntactically-valid reference, or 0 otherwise.
int is_valid_reference(const char *reference)
{
	if (!isalpha(reference[0]) && reference[0] != '_') return 0;
	for (const char *p = reference; *p; p++) if (!isalnum(*p) && *p != '_') return 0;
	return 1;
}

int cmd_unsetenv(char** args)
{
	// Validates words.
	int argc = 0;
	while (args[argc] != NULL) argc++;
	if (argc != 2) {fprintf(stderr, "vensh: Expected 1 argument\n"); return 1;}

	if (getenv(args[1]) == NULL) printf("vensh: Expected existing process environment variable\n");
	else unsetenv(args[1]);
	return 1;
}

int cmd_usage(char** args)
{
	// Validates words.
	int argc = 0;
	while (args[argc] != NULL) argc++;
	if (argc != 2) {fprintf(stderr, "vensh: Expected 1 argument\n"); return 1;}
	// Iterate over commands: if second word entered matches any command name, try to run corresponding function.
	for (int i = 0; i < N(); i++) {
		if (strcmp(args[1], COMMANDS[i]) == 0) {
			printf("%s\n", USAGES[i]);
			return 1;
		}
	}
	return 1;
}

int run_rc(const char* rcfilename)
{
	FILE* rcfile = fopen(rcfilename, "r");
	if (rcfile == NULL) {
		fprintf(stderr, "vensh: Expected %s\n", rcfilename);
		return 1;
	}
	char line[1024];
	while (fgets(line, sizeof(line), rcfile)) {
		line[strcspn(line, "\n")] = '\0';
		// Try to safely set PATH process environment variable, if line is prefixed with "PATH=".
		if (strncmp(line, "PATH=", 5) == 0) {
			char* extracted_path = slice(line, 5);
			if (safeset_env("PATH", extracted_path) != 0) print_safeset_env_failure("PATH", extracted_path);
		}
		// Try to safely set ARTFILENAME process environment variable, if line is prefixed with "ARTFILENAME=".
		else if (strncmp(line, "ARTFILENAME=", 12) == 0) {
			char* extracted_artfilename = slice(line, 12);
			if (safeset_env("ARTFILENAME", extracted_artfilename) != 0) print_safeset_env_failure("ARTFILENAME", extracted_artfilename);
		}
		// Try to run function or launch program, otherwise.
		else {
			char** args = splitline(line);
			execute(args);
			free(args);
		}
	}
	fclose(rcfile);
	return 0;
}

int safeset_env(char* envreference, char* envvalue)
{
	if (getenv(envreference) == NULL) {
		if (setenv(envreference, envvalue, 1) != 0) return 1;
	}
	else {
		char* newenvvalue = (char*) malloc(strlen(getenv(envreference)) + strlen(envvalue) + 2);
		if (newenvvalue == NULL) {print_malloc_failure(); exit(1);}
		sprintf(newenvvalue, "%s:%s", getenv(envreference), envvalue);
		if (setenv(envreference, newenvvalue, 1) != 0) return 1;
		free(newenvvalue);
	}
	return 0;
}

char* slice(const char *s, int i) {
	int l = 0;
	while (s[i] != '\0' && s[i] != '\n') {
		l++;
		i++;
	}
	char *extracted = (char*) malloc(l + 1);
	if (extracted == NULL) {
		print_malloc_failure();
		exit(1);
	}
	strncpy(extracted, s + i - l, l);
	extracted[l] = '\0';
	return extracted;
}

void print_malloc_failure(void)
{
	fprintf(stderr, "vensh: Failed to allocate memory\n");
}

void print_setenv_failure(char* envreference, char* envvalue)
{
	fprintf(stderr, "vensh: Set process environment variable %s to %s\n", envreference, envvalue);
}

void print_safeset_env_failure(char* envreference, char* envvalue)
{
	fprintf(stderr, "vensh: Failed to safely set process environment variable %s to %s\n", envreference, envvalue);
}