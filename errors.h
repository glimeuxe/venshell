#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Failures */
void print_cd_failure(char* s)
{
	if (strcmp(getenv("LOCALE"), "FR") == 0) fprintf(stderr, "vensh: Échec du changement de répertoire vers %s\n", s);
	else fprintf(stderr, "vensh: Failed to change directory to %s\n", s);
}

void print_dup_failure(void)
{
	if (strcmp(getenv("LOCALE"), "FR") == 0) fprintf(stderr, "vensh: Échec de la duplication du descripteur de fichier\n");
	else fprintf(stderr, "vensh: Failed to duplicate file descriptor");
}

void print_fopen_failure(char* s)
{
	if (strcmp(getenv("LOCALE"), "FR") == 0) fprintf(stderr, "vensh: Échec d'ouverture du fichier %s\n", s);
	else fprintf(stderr, "vensh: Failed to open %s\n", s);
}

void print_fork_failure(void)
{
	if (strcmp(getenv("LOCALE"), "FR") == 0) fprintf(stderr, "vensh: Échec du fork\n");
	else fprintf(stderr, "vensh: Failed to fork");
}

void print_launch_failure(char* s)
{
	if (strcmp(getenv("LOCALE"), "FR") == 0) fprintf(stderr, "vensh: Échec du lancement de %s\n", s);
	else fprintf(stderr, "vensh: Failed to launch %s\n", s);
}

void print_malloc_failure(void)
{
	if (strcmp(getenv("LOCALE"), "FR") == 0) fprintf(stderr, "vensh: Échec d'allocation de mémoire\n");
	else fprintf(stderr, "vensh: Failed to allocate memory\n");
}

void print_mkdir_failure(char* s)
{
	if (strcmp(getenv("LOCALE"), "FR") == 0) fprintf(stderr, "vensh: Échec de la création du répertoire %s\n", s);
	else fprintf(stderr, "vensh: Failed to make directory %s\n", s);
}

void print_mv_failure(char* source, char* destination)
{
	if (strcmp(getenv("LOCALE"), "FR") == 0) fprintf(stderr, "vensh: Échec du déplacement de %s vers %s\n", source, destination);
	else fprintf(stderr, "vensh: Failed to move %s to %s\n", source, destination);
}

void print_popen_failure(void)
{
	if (strcmp(getenv("LOCALE"), "FR") == 0) fprintf(stderr, "vensh: Échec de l'ouverture du pipe\n");
	else fprintf(stderr, "vensh: Failed to open pipe");
}

void print_setenv_failure(char* envreference, char* envvalue)
{
	if (strcmp(getenv("LOCALE"), "FR") == 0) fprintf(stderr, "vensh: Échec de définir la variable d'environnement %s à %s\n", envreference, envvalue);
	else fprintf(stderr, "vensh: Failed to set environment variable %s to %s\n", envreference, envvalue);
}

void print_setsid_failure(void)
{
	if (strcmp(getenv("LOCALE"), "FR") == 0) fprintf(stderr, "vensh: Échec de la création de session\n");
	else fprintf(stderr, "vensh: Failed to create session\n");
}

void print_system_failure(void)
{
	if (strcmp(getenv("LOCALE"), "FR") == 0) fprintf(stderr, "vensh: Échec de l'exécution du système\n");
	else fprintf(stderr, "vensh: Failed to run system command\n");
}

void print_zip_failure(char* s)
{
	if (strcmp(getenv("LOCALE"), "FR") == 0) fprintf(stderr, "vensh: Échec de la compression du répertoire %s\n", s);
	else fprintf(stderr, "vensh: Failed to zip directory %s\n", s);
}

/* Expecteds */
void print_expected(char* s)
{
	if (strcmp(getenv("LOCALE"), "FR") == 0) fprintf(stderr, "vensh: %s attendu\n", s);
	else fprintf(stderr, "vensh: Expected %s\n", s);
}

void print_expected_argc(char* s)
{
	if (strcmp(getenv("LOCALE"), "FR") == 0) fprintf(stderr, "vensh: Nombre attendu d'arguments de %s\n", s);
	else fprintf(stderr, "vensh: Expected argument count of %s\n", s);
}

void print_expected_env(void)
{
	if (strcmp(getenv("LOCALE"), "FR") == 0) fprintf(stderr, "vensh: Variable d'environnement attendue\n");
	else fprintf(stderr, "vensh: Expected environment variable\n");
}

void print_expected_envreferencelength(void)
{
	if (strcmp(getenv("LOCALE"), "FR") == 0) fprintf(stderr, "vensh: Longueur attendue du nom de la variable d'environnement entre 1 et 255 caractères (inclus)\n");
	else fprintf(stderr, "vensh: Expected environment variable name length between 1 and 255 characters (inclusive)\n");
}

void print_expected_envvaluelength(void)
{
	if (strcmp(getenv("LOCALE"), "FR") == 0) fprintf(stderr, "vensh: Longueur attendue de la valeur de la variable d'environnement entre 1 et 1023 caractères (inclus)\n");
	else fprintf(stderr, "vensh: Expected environment variable value length between 1 and 1023 characters (inclusive)\n");
}

void print_expected_int(int l, int h)
{
	if (strcmp(getenv("LOCALE"), "FR") == 0) fprintf(stderr, "vensh: Entier attendu entre %d et %d (inclus)\n", l, h);
	else fprintf(stderr, "vensh: Expected integer between %d and %d (inclusive)\n", l, h);
}

void print_expected_valid_envreference(void)
{
	if (strcmp(getenv("LOCALE"), "FR") == 0) fprintf(stderr, "vensh: Nom de variable d'environnement valide attendu\n");
	else fprintf(stderr, "vensh: Expected valid environment variable name\n");
}