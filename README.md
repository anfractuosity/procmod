# ProcMod v0.1

## About

Modifies running processes on linux.

* Finds ASCII/UTF-16/binary and replaces it appropiately
* Dumps processes to disk

## Compilation

Should compile fine on 32/64 bit architectures, simply by typing 'make'.

## Arguments 

* -p PROCID - Process ID to work on
* -f        - Text to find in the process
* -r        - Text to replace in the proces
* -u        - Convert find/replace parameters to UTF-16 (for JVM)
* -d FILE   - dumps process to file
* -l NUM    - Lower bound of search
* -t NUM    - Top bound of search
* -h        - Hexadecimal find/replace strings


## Example usage

Search for the text HELLOWORLD in a firefox process, from the hex address 0x00007fef32000000.

./procmod -p `pgrep firefox` -f HELLOWORLD -l `printf "%u" 0x00007fef32000000`


Find the hexadecimal bytes 0xDEADBEEF in firefox.

./procmod -p `pgrep firefox` -f "DEADBEEF" -h

## Problems

If you're running a Grsecurity kernel with "Configuration option: CONFIG_GRKERNSEC_PROC_MEMMAP" enabled
then you will find that most of the addresses in /proc/PROCESSID/maps are null.  This is because:

"If you say Y here, the /proc/<pid>/maps and /proc/<pid>/stat files will give no information about the addresses of its mappings if PaX features that rely on random addresses are enabled on the task. If you use PaX it is greatly recommended that you say Y here as it closes up a hole that makes the full ALSR useless for suid binaries."


## Licence

Licenced under GPL v2
