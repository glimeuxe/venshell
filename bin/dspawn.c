#include "../base.h"
#include "../errors.h"

int main(void)
{
	// Create intermediate process and make it session leader.
	pid_t pid = fork();
	if (pid < 0) {print_fork_failure(); exit(1);}
	if (pid > 0) {setsid(); wait(NULL); exit(0);}
	if (setsid() < 0) {print_setsid_failure(); exit(1);}

	signal(SIGHUP, SIG_IGN);
	signal(SIGCHLD, SIG_IGN);

	// Create daemon process as session participant.
	pid = fork();
	if (pid < 0) {print_fork_failure(); exit(1);}
	if (pid > 0) {exit(0);}

	// Set daemon permissions to maximum.
	umask(0);

	if (chdir("/") < 0) {print_cd_failure("/"); exit(1);}

	// Close open file descriptors.
	for (int x = sysconf(_SC_OPEN_MAX); x >= 0; x--) close(x);

	// Point file descriptors 0, 1, and 2 to /dev/null.
	int fd0 = open("/dev/null", O_RDWR);
	int fd1 = dup(0);
	int fd2 = dup(0);
	if (fd0 != 0 || fd1 != 1 || fd2 != 2) {print_dup_failure(); exit(1);}

	char* venshlogs = getenv("VENSHLOGS");
	char* logfilepath = (char*) malloc(strlen(venshlogs) + strlen("/dspawn.log") + 1);
	if (logfilepath == NULL) {print_malloc_failure(); exit(1);}
	sprintf(logfilepath, "%s/dspawn.log", venshlogs);
	FILE* logfilepointer = fopen(logfilepath, "a");
	if (logfilepointer == NULL) {print_fopen_failure(logfilepath); exit(1);}
	fprintf(logfilepointer, "\nDaemon process created with PID %d and PPID %d\n", getpid(), getppid());
	fflush(logfilepointer);

	// Simulate daemon work.
	for (int i = 1; i <= 60; i++) sleep(1);

	fclose(logfilepointer);
	free(logfilepath);
	return 0;
}