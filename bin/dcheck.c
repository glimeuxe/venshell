#include "../base.h"
#include "../errors.h"

int main(void)
{
	int daemoncount = 0;
	char* dcheck_cmd = "ps -efj | grep dspawn | grep -Ev 'tty|pts' | grep -v grep | wc -l";
	FILE* fp = popen(dcheck_cmd, "r");
	if (fp == NULL) {print_popen_failure(dcheck_cmd); return 1;}
	fscanf(fp, "%d", &daemoncount);
	printf("vensh: Number of daemons is %d\n", daemoncount);
	pclose(fp);

	return 0;
}