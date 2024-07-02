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
#include <libgen.h>
#include <sys/stat.h>
#include <signal.h>
#include <limits.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/utsname.h>
#include <pwd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/statvfs.h>

/* Constants */
#define COLOUR_RED "\x1b[31m"
#define COLOUR_GREEN "\x1b[32m"
#define COLOUR_BLUE "\x1b[34m"
#define COLOUR_PINK "\x1b[35m"
#define COLOUR_RESET "\x1b[0m"

#define FONT_BOLD "\033[1m"
#define FONT_REGULAR "\033[0m"

#define LINE_DELIMITER " \t\r\n\a"

#define IDLE_TIME_LIMIT 300

/* Macros */
#define fprintf_stderr_red(fmt, ...) fprintf(stderr, COLOUR_RED fmt COLOUR_RESET, ##__VA_ARGS__)
#define fprintf_stdout_green(fmt, ...) fprintf(stdout, COLOUR_GREEN fmt COLOUR_RESET, ##__VA_ARGS__)

/* Directives */
#if defined(__linux__)
	#define EXE_FILEPATH "/proc/self/exe"
#elif defined(__APPLE__)
	#include <mach-o/dyld.h>
#else
	#error "vensh: OS is unsupported"
#endif