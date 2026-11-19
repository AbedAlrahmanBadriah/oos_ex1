ex1b1 - spawn in Windows (Cygwin)
Exercise 1b - Simple Shell using spawn

Written by: <Student Name>, id = <ID>, login = <login>
            <Student Name>, id = <ID>, login = <login>

Description
-----------
This is the Windows/Cygwin version of the exercise 1b shell. It provides
the same shell behavior as ex1b, but child programs are started with
spawn (process creation) instead of fork+exec.

Supported commands:
  pid                     - runs ./pid
  char_in_str <ch> <str>  - runs ./char_in_str
  unique_str <words>      - runs ./unique_str
  exit                    - exits the shell

Unknown commands or invalid arguments print:
  error

The shell waits for each spawned command to finish before reading the
next command.

Files
-----
  ex1ba.c            - simple shell for Windows/Cygwin (uses spawn)
  char_in_str.c      - helper: counts occurrences of a character in a string
  unique_str.c       - helper: counts unique words in a string
  pid.c              - helper: prints process id and parent process id
  README_windows.txt - this file

(Compile the helper programs in the same Cygwin environment so the shell
can run ./pid, ./char_in_str, and ./unique_str.)

Compile (Cygwin)
----------------
  gcc ex1ba.c -o ex1ba
  gcc pid.c -o pid
  gcc char_in_str.c -o char_in_str
  gcc unique_str.c -o unique_str

Run (Cygwin)
------------
  ./ex1ba

Then enter commands, for example:
  pid
  char_in_str a xxabcayy
  char_in_str z xxabxcyy
  unique_str aaaaab ab abc
  exit

Behavior notes
--------------
- Implemented with spawn (no fork), as required for the Windows/Cygwin
  version.
- char_in_str takes a character and a string and prints the number of
  occurrences (>= 0).
- unique_str takes one argument containing words separated by spaces and
  prints the number of unique words (MAX_STRS = 10 words considered).
- pid prints: <pid> <ppid>
- If the unique_str argument is entered with surrounding quotes in the
  shell, the shell removes those outer quotes before passing the argument.

Tests
-----
1. char_in_str - basic match
   Command: char_in_str a xxabcayy
   Expected output: 2

2. char_in_str - no match
   Command: char_in_str z xxabxcyy
   Expected output: 0

3. char_in_str - empty string (edge case)
   Command: char_in_str x ""
   Expected output: 0

4. unique_str - basic unique count
   Command: unique_str aaaaab ab abc
   Expected output: 3

5. unique_str - duplicates and spaces
   Command: unique_str "a b a c a"
   Expected output: 3

6. unique_str - more than MAX_STRS (10) words
   Command: unique_str "1 2 3 4 5 6 7 8 9 10 11 12"
   Expected output: 10
   (only the first 10 words are considered)

7. pid - output format
   Command: pid
   Expected output: two integers in the format "<pid> <ppid>"
   (values vary by run)

8. shell - supported commands and exit
   Input:
     pid
     char_in_str a xxabcayy
     unique_str ab gh hg ba ab gh
     exit
   Expected output:
     <pid> <ppid>
     2
     4
   (pid/ppid values vary; shell terminates after exit)

9. shell - invalid command and missing arguments
   Input:
     foo
     char_in_str
     unique_str
     char_in_str a
     exit
   Expected output:
     error
     error
     error
     error

10. shell - quoted unique_str argument
    Input:
      unique_str "ab gh hg ba ab gh"
      exit
    Expected output:
      4
    (outer quotes are handled by the shell)