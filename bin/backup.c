#include "../base.h"
#include "../errors.h"

int main(void)
{
	char *backupdirectory = getenv("BACKUP_DIR");
	char *backupdestination = getenv("VENSHBACKUPS");
	if (backupdirectory == NULL) {print_expected_env(); return 1;}
	if (backupdestination == NULL) {print_expected_env(); return 1;}

	if (chdir(backupdirectory) != 0) {print_cd_failure(backupdirectory); return 1;}

	char zip_cmd[256];
	snprintf(zip_cmd, sizeof(zip_cmd), "zip -r backup.zip .");
	if (system(zip_cmd) != 0) {print_zip_failure(backupdirectory); return 1;}

	char mv_cmd[256];
	snprintf(mv_cmd, sizeof(mv_cmd), "mv backup.zip %s", backupdestination);
	if (system(mv_cmd) != 0) {print_mv_failure("backup.zip", backupdestination); return 1;}

	print_backup_success(backupdirectory, backupdestination);

	return 0;
}