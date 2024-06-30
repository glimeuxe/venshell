# venshell[^1]
## Setup
1. Download (and extract) the contents of this repository to a local directory.
2. Optionally, re-compile “main.c” (e.g. using `gcc -o main main.c` if GCC is available).
3. Run “main”, and grant permissions whenever prompted.
4. Optionally, modify “vensh.rc” to configure venshell (cf. [Configuration](#Configuration)).

## Support
- OS: Virtually any sufficiently modern POSIX-compliant OS (cf. https://en.wikipedia.org/wiki/POSIX#POSIX-oriented_operating_systems).
- Language: English and French.

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
3. `sys`: Lists username, OS name, CPU name, total RAM size (in MB), and kernel version, beside either the default text art (the contents of “art.txt” in ‘assets’), or custom text art (cf. [Configuration](#Configuration)).
4. `elio q`: Queries the Library of Babel with `q` (cf. https://libraryofbabel.app/, https://en.wikipedia.org/wiki/The_Library_of_Babel).
5. `wa q`: Queries WolframAlpha with `q` using WolframAlpha API key specified by environment variable `WA_APPID` (cf. [Configuration](#Configuration), https://github.com/archelaus/wolfram-alpha-cli/blob/main/README.md).

## Configuration
| New Line                   | Action                                                                                |
| -------------------------- | ------------------------------------------------------------------------------------- |
| `cmd args`               | Specifies command named `cmd` with argument sequence `args` to be run on startup. |
| `N=V`                      | Specifies environment variable named `N` to be set to value `V` on startup.        |
| `LOCALE=EN`                | Sets language to English.                                                             |
| `LOCALE=FR`                | Sets language to French.                                                              |
| `ARTFILENAME=filename.txt` | Sets art displayed by `sys` to (contents of) “filename.txt” in ‘assets‘.                   |
| `WA_APPID=key`             | Sets WolframAlpha API key of `wa` to `key`.                                    |

[^1]: Wrapping plaintext, single quotes (‘‘) specifies a directory name, where as double quotes (““) specifies a file name.
