#include "base.h"

/* Failures */
void print_cd_failure(char* s)
{
	if (strcmp(getenv("LOCALE"), "FR") == 0) fprintf_stderr_red("vensh: Échec du changement de répertoire vers %s\n", s);
	else fprintf_stderr_red("vensh: Failed to change directory to %s\n", s);
}

void print_dup_failure(void)
{
	if (strcmp(getenv("LOCALE"), "FR") == 0) fprintf_stderr_red("vensh: Échec de la duplication du descripteur de fichier\n");
	else fprintf_stderr_red("vensh: Failed to duplicate file descriptor");
}

void print_fgets_failure(char* s)
{
	if (strcmp(getenv("LOCALE"), "FR") == 0) fprintf_stderr_red("vensh: Échec de lecture de %s\n", s);
	else fprintf(stderr, "vensh: Failed to read %s\n", s);
}

void print_fopen_failure(char* s)
{
	if (strcmp(getenv("LOCALE"), "FR") == 0) fprintf_stderr_red("vensh: Échec d'ouverture du fichier %s\n", s);
	else fprintf_stderr_red("vensh: Failed to open %s\n", s);
}

void print_fork_failure(void)
{
	if (strcmp(getenv("LOCALE"), "FR") == 0) fprintf_stderr_red("vensh: Échec du fork\n");
	else fprintf_stderr_red("vensh: Failed to fork");
}

void print_launch_failure(char* s)
{
	if (strcmp(getenv("LOCALE"), "FR") == 0) fprintf_stderr_red("vensh: Échec du lancement de %s\n", s);
	else fprintf_stderr_red("vensh: Failed to launch %s\n", s);
}

void print_malloc_failure(void)
{
	if (strcmp(getenv("LOCALE"), "FR") == 0) fprintf_stderr_red("vensh: Échec d'allocation de mémoire\n");
	else fprintf_stderr_red("vensh: Failed to allocate memory\n");
}

void print_mkdir_failure(char* s)
{
	if (strcmp(getenv("LOCALE"), "FR") == 0) fprintf_stderr_red("vensh: Échec de la création du répertoire %s\n", s);
	else fprintf_stderr_red("vensh: Failed to make directory %s\n", s);
}

void print_mv_failure(char* source, char* destination)
{
	if (strcmp(getenv("LOCALE"), "FR") == 0) fprintf_stderr_red("vensh: Échec du déplacement de %s vers %s\n", source, destination);
	else fprintf_stderr_red("vensh: Failed to move %s to %s\n", source, destination);
}

void print_parse_failure(char* s1, char* s2)
{
	if (strcmp(getenv("LOCALE"), "FR") == 0) fprintf_stderr_red("vensh: Échec d'analyser %s à partir de %s\n", s1, s2);
	else fprintf_stderr_red("vensh: Failed to parse %s from %s\n", s1, s2);
}

void print_popen_failure(char* s)
{
	if (strcmp(getenv("LOCALE"), "FR") == 0) fprintf_stderr_red("vensh: Échec de l'ouverture du pipe %s\n", s);
	else fprintf_stderr_red("vensh: Failed to open pipe %s\n", s);
}

void print_realpath_failure(char* s)
{
	if (strcmp(getenv("LOCALE"), "FR") == 0) fprintf_stderr_red("vensh: Échec de la résolution du chemin réel pour %s\n", s);
	else fprintf_stderr_red("vensh: Failed to resolve realpath for %s\n", s);
}

void print_setenv_failure(char* envreference, char* envvalue)
{
	if (strcmp(getenv("LOCALE"), "FR") == 0) fprintf_stderr_red("vensh: Échec de définir la variable d'environnement %s à %s\n", envreference, envvalue);
	else fprintf_stderr_red("vensh: Failed to set environment variable %s to %s\n", envreference, envvalue);
}

void print_setsid_failure(void)
{
	if (strcmp(getenv("LOCALE"), "FR") == 0) fprintf_stderr_red("vensh: Échec de la création de session\n");
	else fprintf_stderr_red("vensh: Failed to create session\n");
}

void print_signal_failure(char* s1, char* s2)
{
	if (strcmp(getenv("LOCALE"), "FR") == 0) fprintf_stderr_red("vensh: Échec de configuration du gestionnaire de signal pour %s à %s\n", s1, s2);
	else fprintf_stderr_red("vensh: Failed to set signal handler for %s to %s\n", s1, s2);
}

void print_strstr_failure(char* s1, char* s2)
{
	if (strcmp(getenv("LOCALE"), "FR") == 0) fprintf(stderr, "vensh: Échec de trouver %s dans %s\n", s1, s2);
	else fprintf(stderr, "vensh: Failed to find %s in %s\n", s1, s2);
}

void print_system_failure(void)
{
	if (strcmp(getenv("LOCALE"), "FR") == 0) fprintf_stderr_red("vensh: Échec de l'exécution du système\n");
	else fprintf_stderr_red("vensh: Failed to run system command\n");
}

void print_zip_failure(char* s)
{
	if (strcmp(getenv("LOCALE"), "FR") == 0) fprintf_stderr_red("vensh: Échec de la compression du répertoire %s\n", s);
	else fprintf_stderr_red("vensh: Failed to zip directory %s\n", s);
}

/* Expecteds */
void print_expected(char* s)
{
	if (strcmp(getenv("LOCALE"), "FR") == 0) fprintf_stderr_red("vensh: %s attendu\n", s);
	else fprintf_stderr_red("vensh: Expected %s\n", s);
}

void print_expected_activity(void)
{
	if (strcmp(getenv("LOCALE"), "FR") == 0) fprintf_stderr_red("vensh: Activité attendue, sortie\n");
	else fprintf_stderr_red("vensh: Expected activity, exiting...\n");
}

void print_expected_argc(char* s)
{
	if (strcmp(getenv("LOCALE"), "FR") == 0) fprintf_stderr_red("vensh: Nombre attendu d'arguments de %s\n", s);
	else fprintf_stderr_red("vensh: Expected argument count of %s\n", s);
}

void print_expected_env(void)
{
	if (strcmp(getenv("LOCALE"), "FR") == 0) fprintf_stderr_red("vensh: Variable d'environnement attendue\n");
	else fprintf_stderr_red("vensh: Expected environment variable\n");
}

void print_expected_envreferencelength(void)
{
	if (strcmp(getenv("LOCALE"), "FR") == 0) fprintf_stderr_red("vensh: Longueur attendue du nom de la variable d'environnement entre 1 et 255 caractères (inclus)\n");
	else fprintf_stderr_red("vensh: Expected environment variable name length between 1 and 255 characters (inclusive)\n");
}

void print_expected_envvaluelength(void)
{
	if (strcmp(getenv("LOCALE"), "FR") == 0) fprintf_stderr_red("vensh: Longueur attendue de la valeur de la variable d'environnement entre 1 et 1023 caractères (inclus)\n");
	else fprintf_stderr_red("vensh: Expected environment variable value length between 1 and 1023 characters (inclusive)\n");
}

void print_expected_filepath_length(char* s)
{
	if (strcmp(getenv("LOCALE"), "FR") == 0) fprintf_stderr_red("vensh: Longueur de chemin de fichier attendu pour %s\n", s);
	else fprintf_stderr_red("vensh: Expected filepath length for %s\n", s);
}

void print_expected_int(int l, int h)
{
	if (strcmp(getenv("LOCALE"), "FR") == 0) fprintf_stderr_red("vensh: Entier attendu entre %d et %d (inclus)\n", l, h);
	else fprintf_stderr_red("vensh: Expected integer between %d and %d (inclusive)\n", l, h);
}

void print_expected_valid_envreference(void)
{
	if (strcmp(getenv("LOCALE"), "FR") == 0) fprintf_stderr_red("vensh: Nom de variable d'environnement valide attendu\n");
	else fprintf_stderr_red("vensh: Expected valid environment variable name\n");
}

/* Successes */
void print_backup_success(char* backupdirectory, char* backupdestination)
{
	if (strcmp(getenv("LOCALE"), "FR") == 0) fprintf_stdout_green("vensh: Sauvegarde de %s effectuée et stockée dans %s\n", backupdirectory, backupdestination);
	else fprintf_stdout_green("vensh: Backup of %s made and stored at %s\n", backupdirectory, backupdestination);
}