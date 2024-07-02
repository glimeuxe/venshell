# Venshell[^0]
## Disclaimer
Venshell is provided as-is, and while efforts have been made to ensure its reliability and security, it has only been tested on MacOS Sonoma and later versions. Users are encouraged to review the source code for transparency and to ensure it meets their security standards. Feedback, bug reports, and contributions are welcome to improve compatibility and reliability across different platforms.

## Setup
1. Download (and extract) the contents of this repository to a local directory.
2. For safety, re-compile each downloaded C file (e.g. by having `make` program (cf. https://www.gnu.org/software/make/) installed, navigating to the directory on terminal, and entering `make`).
3. Run “vensh”, and grant permissions whenever prompted.
4. Optionally, modify “vensh.rc” to configure venshell (cf. [Configuration](#Configuration)).

## Support
Venshell should be able to run on virtually any sufficiently modern POSIX-compliant OS (cf. https://en.wikipedia.org/wiki/POSIX#POSIX-oriented_operating_systems) with pre-installed system program “zip”.

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
| Line                   | Action                                                                            |
| -------------------------- | --------------------------------------------------------------------------------- |
| `cmd args`                 | Specifies command named `cmd` with argument sequence `args` to be run on startup. |
| `N=V`                      | Specifies environment variable named `N` to be set to value `V` on startup.       |
| `LOCALE=EN`                | Sets language to English.                                                         |
| `LOCALE=FR`                | Sets language to French.                                                          |
| `ARTFILENAME=filename.txt` | Sets `sys` program art to (contents of) “filename.txt” in ‘assets‘.          |
| `WA_APPID=key`             | Sets WolframAlpha API key of `wa` to `key`.                                       |
| `BACKUP_DIR=dir`           | Sets backup (source) directory[^1] to absolute directorypath ‘dir’.                        |

## Other Features
1. Non-transient prompt with current working directorypath display: venshell prompt is displayed after every command run, and contains directorypath to current working directory.
2. Configurability for pre-defined environment variables: Environment variables for locale, `sys` program art file, WolframAlpha API key, and backup (source) directory can be configured.
3. Idle timeout: For efficiency, venshell (automatically) exits when no command has run in last 5-minute window.
4. Accessible text colours: venshell uses combination of blue and pink text colours for prompt, green text colour for success messages, and red text colour for error messages.
5. Multi-language support: venshell (mostly) supports English and French.


[^0]: Wrapping plaintext, single quotes (‘‘) are used to specify either a directory name or directorypath, and double quotes (““) are used to specify either a file name or filepath.
[^1]: The backup (source) directory is the directory to be zipped.
