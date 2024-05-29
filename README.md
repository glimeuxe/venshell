# venshell
## OS Support
- Almost any sufficiently POSIX-compliant (e.g. MacOS, Linux, etc.) operating system and/or distribution (cf. https://en.wikipedia.org/wiki/POSIX#POSIX-oriented_operating_systems).

## Setup
1. Download this repository to a local directory.
2. Optionally, re-compile the file named “main.c” to overwrite the file named “main” (e.g. using `gcc -o main main.c`).
3. Run the file named “main”, and grant user permissions if prompted.

## Startup Configuration
- On any line of the startup configuration file named “vensh.rc”, you may write `<c> <a> ...` to specify a command named `<c>` with argument sequence `<a> ...` (if any) to run at startup, or write `<n>=<v>:...` to “safely set”[^1] a venshell process environment variable named `<n>` to value `<v>`.

## Built-In Commands
1. `cd <d>`: Changes current directory to filepath `<d>`.
2. `env`: Lists venshell process environment variables.
3. `exit`: Quits current venshell process.
4. `help`: Lists venshell built-in commands.
5. `setenv <n> <v>`: “Safely sets”[^1] a venshell process environment variable named `<n>` to value `<v>`.
6. `unsetenv <n>`: Deletes process environment variable named `<n>`, if it exists.
7. `usage <c>`: Explain how to use venshell built-in command named `<c>`.

## Program Commands
1. `sys`: Lists username, (vendor-specified) OS name, (vendor-specified) CPU name, total RAM size (in MB), and (vendor-specified) kernel version, beside either the default text art (found in the file within the “assets” folder named “art.txt”), or custom text art (cf. the Additional Features section).
2. `trans`: Translates from English to another language of choice, or from a detected language to English, in various ways (cf. https://github.com/soimort/translate-shell/blob/develop/README.md).

## Additional Features
1. Custom `sys` text art: The `sys` command can display text art (cf. the Program Commands section). To change the displayed text art, navigate to the folder named “assets” and supply a text art file whose name ends with the “.txt” file format extension; supposing your text art file is named “art-x.txt”, either replace the existing `ARTFILENAME` value (if any) with “art-x.txt”, or write `ARTFILENAME=art-x.txt` on a new line down.

[^1]: To “safely set” a venshell process environment variable named `<n>` to value `<v>` is to either assign the value `<v>` to a non-existent process environment variable named `<n>`, or append the value `<v>` to an existing process environment variable named `<n>`.
