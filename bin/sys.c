#include "../base.h"
#include "../errors.h"

int print_all(void);

int main(void)
{
	int print_all_statuscode = print_all();
	return print_all_statuscode;
}

int print_all(void)
{
	// Get system information.
	struct utsname uname_data;
	struct passwd* pw;
	size_t len = sizeof(long long);
	long long total_mem;
	uname(&uname_data);

	// Get user information.
	char* user = getenv("USER");
	if (user == NULL) {
		pw = getpwuid(getuid());
		user = pw->pw_name;
	}

	// Get RAM information.
	sysctlbyname("hw.memsize", &total_mem, &len, NULL, 0);

	// Get hostname information.
	char hostname[256];
	if (gethostname(hostname, sizeof(hostname)) != 0) {perror("hostname"); return 1;}

	// Get IP information.
	char ip[INET_ADDRSTRLEN];
	struct sockaddr_in addr;
	socklen_t addr_len = sizeof(addr);
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0) {perror("socket"); return 1;}
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(53);
	addr.sin_addr.s_addr = inet_addr("8.8.8.8");
	if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {perror("connect"); return 1;}
	getsockname(sockfd, (struct sockaddr *)&addr, &addr_len);
	close(sockfd);
	inet_ntop(AF_INET, &addr.sin_addr, ip, sizeof(ip));

	// Get disk information.
	struct statvfs vfs;
	if (statvfs("/", &vfs) == -1) {perror("statvfs"); return 1;}
	unsigned long long total_space = vfs.f_blocks * vfs.f_frsize;
	unsigned long long available_space = vfs.f_bavail * vfs.f_frsize;

	char* artfilename;
	if (getenv("ARTFILENAME") == NULL) artfilename = "../assets/art.txt";
	else artfilename = getenv("ARTFILENAME");

	char* venshassets = getenv("VENSHASSETS");
	if (venshassets == NULL) {fprintf(stderr, "vensh: Expected existing VENSHASSETS\n"); return 1;}

	char* artfilepath = (char*) malloc(strlen(venshassets) + strlen(artfilename) + 2);
	if (artfilepath == NULL) {print_malloc_failure(); exit(1);}
	sprintf(artfilepath, "%s/%s", venshassets, artfilename);

	FILE* artfilepointer = fopen(artfilepath, "r");
	if (artfilepointer == NULL) {fprintf(stderr, "vensh: Expected existing art.txt\n"); return 1;}
	char line[256];
	int i = 0;
	printf("\n");
	while (fgets(line, sizeof(line), artfilepointer)) {
		line[strcspn(line, "\n")] = '\0';
		if (i == 0) printf(FONT_BOLD "%s [%s on %s@%s]\n" FONT_REGULAR, line, user, hostname, uname_data.sysname);
		else if (i == 1) printf("%s\n", line);
		else if (i == 2) printf("%s ~ CPU: %s\n", line, uname_data.machine);
		else if (i == 3) printf("%s ~ RAM: %.2f GB total\n", line, (double)total_mem / (1024 * 1024 * 1024));
		else if (i == 4) printf("%s ~ Disk: %.2f GB total, %.2f GB available\n", line, total_space / (1024.0 * 1024.0 * 1024.0), available_space / (1024.0 * 1024.0 * 1024.0));
		else if (i == 5) printf("%s ~ Kernel: v%s\n", line, uname_data.release);
		else if (i == 6) printf("%s ~ IPv4: %s\n", line, ip);
		else {printf("%s\n", line);}
		i++;
	}
	printf("\n");
	fclose(artfilepointer);
	free(artfilepath);
	return 0;
}