# ProcMod v0.1

## About

Modifies running processes on linux.

* Finds ASCII text or UTF-16 and replaces it with alternative text
* Dumps processes to disk

## Compilation

Should compile fine on 32/64 bit architectures, simply by typing 'make'.

## Arguments 

* -p PROCID - Process ID to work on
* -f        - Text to find in the process
* -r        - Text to replace in the proces
* -u        - Convert find/replace parameters to UTF-16 (for JVM)
* -d FILE   - dumps process to file

## Licence

Licenced under GPL v2
