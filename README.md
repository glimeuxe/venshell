# venshell[^0]

## Preview
https://github.com/user-attachments/assets/c68e7c8b-8083-4597-8a01-44fac27513c4

## Setup
1. For safety, re-compile each C file.
2. Ensure “vensh” is executable.

## Usage
1. Run “vensh”.
2. Optionally, modify “vensh.rc” to configure venshell (cf. [Configuration](#Configuration)).

## Support
venshell supports POSIX-compliant OSes (cf. https://en.wikipedia.org/wiki/POSIX#POSIX-oriented_operating_systems).

## Built-In Commands
1. `cd f`: Changes current directory to filepath `f`.
2. `env`: Lists environment variables.
3. `exit`: Quits current process.
4. `help`: Lists built-in commands.
5. `setenv N=V`: Sets environment variable named `N` to value `V`.
6. `prependenv N=V`: Prepends value `V` to environment variable named `N`.
7. `unsetenv N`: Deletes environment variable named `N`, if it exists.
8. `usage cmd`: Explains how to use built-in command named `cmd`.

## Program Commands
1. `dspawn`: Spawns a daemon that does nothing for 60 seconds before terminating.
2. `dcheck`: Prints the number of daemons spawned by `dspawn`.
3. `sys`: Lists username, hostname, OS name, CPU name, total RAM size (in GB), total and available disk space (in GB), kernel version, and IPv4 address, beside either the default text art (the contents of “art.txt” in ‘assets’), or custom text art (cf. [Configuration](#Configuration)).
4. `backup`: Zips backup (source) directory[^1] specified by environment variable `BACKUP_DIR` into filepath “backups/backup.zip”.
5. `elio q`: Queries the Library of Babel with `q` (cf. https://libraryofbabel.app, https://en.wikipedia.org/wiki/The_Library_of_Babel).
6. `trans args`: Translates (cf. https://github.com/soimort/translate-shell).
7. `wa q`: Queries WolframAlpha with `q` using WolframAlpha API key specified by environment variable `WA_APPID` (cf. [Configuration](#Configuration), https://github.com/archelaus/wolfram-alpha-cli).

## Configuration
| Line                       | Action                                                                            |
| -------------------------- | --------------------------------------------------------------------------------- |
| `cmd args`                 | Specifies command named `cmd` with argument sequence `args` to be run on startup. |
| `N=V`                      | Specifies environment variable named `N` to be set to value `V` on startup.       |
| `LOCALE=EN`                | Sets language to English.                                                         |
| `LOCALE=FR`                | Sets language to French.                                                          |
| `ARTFILENAME=filename.txt` | Sets `sys` program art to (contents of) “filename.txt” in ‘assets‘.               |
| `WA_APPID=key`             | Sets WolframAlpha API key of `wa` to `key`.                                       |
| `BACKUP_DIR=dir`           | Sets backup (source) directory[^1] to absolute directorypath ‘dir’.               |

## Other Features
1. Instant prompt with current working directorypath display: venshell prompt is displayed after every command run, and contains directorypath to current working directory.
2. Configurability for pre-defined environment variables: Environment variables for locale, `sys` program art file, WolframAlpha API key, and backup (source) directory can be configured.
3. Idle timeout: For efficiency, venshell (automatically) exits when no command has run in last 5-minute window.
4. Accessible text colours: venshell uses combination of blue and pink text colours for prompt, green text colour for success messages, and red text colour for error messages.
5. Multilingual support: venshell supports English and French.


[^0]: Wrapping plaintext, single quotes (‘‘) are used to specify either a directory name or directorypath, and double quotes (““) are used to specify either a file name or filepath.
[^1]: The backup (source) directory is the directory to be zipped.
