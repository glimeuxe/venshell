# venshell[^1]
## Setup
1. Download and extract the contents of this repository to a local directory.
2. Optionally, re-compile “main.c” to overwrite “main” (e.g. using `gcc -o main main.c`).
3. Run “main”, and grant permissions whenever prompted.
4. Optionally, modify the newly-created “vensh.rc” to configure venshell (cf. [Configuration](#Configuration)).

## Configuration
| New Line            | Action                                                                                                                              |
| ------------------- | ----------------------------------------------------------------------------------------------------------------------------------- |
| `c <a>`             | Specifies a command named `c` with argument sequence `<a>` to be run at startup.                                                    |
| `n=<v>`             | Specifies an environment variable named `n` to be “safely set”[^2] to value `<v>` at startup.                                       |
| `LOCALE=EN/FR`      | Sets language to either English (`EN`) or French (`FR`) at startup.                                                                 |
| `ARTFILENAME=a.txt` | Sets `sys` program art to contents of `a.txt` in ‘assets‘.<br>Example: `ARTFILENAME=monalisa.txt`, where `monalisa.txt` is in ‘assets’. |

## Support
- OSes: Almost any version of any sufficiently POSIX-compliant OS and/or distro (cf. https://en.wikipedia.org/wiki/POSIX#POSIX-oriented_operating_systems).
- Languages: English and French, where English is the default language.

## Built-In Commands
1. `cd <d>`: Changes current directory to (relative or absolute) filepath `<d>`.
2. `env`: Lists venshell process environment variables.
3. `exit`: Quits current venshell process.
4. `help`: Lists venshell built-in commands.
5. `setenv <n> <v>`: “Safely sets”[^2] a venshell environment variable named `<n>` to value `<v>`.
6. `unsetenv <n>`: Deletes environment variable named `<n>`, if it exists.
7. `usage <c>`: Explain how to use venshell built-in command named `<c>`.

## Program Commands
1. `dcheck`: 
2. `dspawn`: 
3. `sys`: Lists username, OS name, CPU name, total RAM size (in MB), and kernel version, beside either the default text art (the contents of “art.txt” in ‘assets’), or custom text art (cf. [Configuration](#Configuration)).
4. `trans`: Translates (cf. https://github.com/soimort/translate-shell/blob/develop/README.md).
5. `wa`: Queries WolframAlpha (cf. https://github.com/archelaus/wolfram-alpha-cli/blob/main/README.md).

[^1]: Wrapping plaintext, single quotes (‘‘) specifies a directory name, where as double quotes (““) specifies a file name.
[^2]: To “safely set” an environment variable named `n` to value `<v>` is to either instantiate an environment variable named `n` with value `<v>`, or append the value `<v>` to an existing process environment variable named `n`.
