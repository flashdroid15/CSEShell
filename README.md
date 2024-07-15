[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/u16ttUuk)
# CSEShell

CSEShell is a simple, custom shell for Unix-based systems, designed to provide an interface for executing system programs. This project includes a basic shell implementation, a set of system programs (`find`, `ld`, `ldr`), and some test files.

## Directory Structure

The project is organized as follows:

- `bin/` - Contains compiled executables for system programs.
  - `find` - Program to find files.
  - `ld` - Program for listing the contents of the current directory.
  - `ldr` - Program for listing the contents of the current directory recursively.
- `cseshell` - The main executable for the CSEShell.
- `files/` - Contains various test files used with the shell and system programs.
  - `combined.txt`, `file1.txt`, `file2.txt`, ... - Test text files.
  - `notes.pdf` - A PDF file for testing.
  - `ss.png` - An image file.
- `makefile` - Makefile for building the CSEShell and system programs.
- `source/` - Source code for the shell and system programs.
  - `shell.c` and `shell.h` - Source and header files for the shell.
  - `system_programs/` - Source code and header for the system programs.
 
## Source Directory

Contains all the necessary source code for the shell and system programs. It is divided into the shell implementation (`shell.c`, `shell.h`) and system programs (`system_programs/`).

## Files Directory

The `files/` directory contains various text, PDF, and image files for testing the functionality of the CSEShell and its system programs.

## Makefile

The Makefile contains rules for compiling the shell and system programs. You can clean the build by running:

```bash
make clean
```

## 1. How to compile and run our shell:

### Compiling CSEShell

To build the CSEShell and system programs, run the following command in the root directory:

```bash
make
```

This will compile the source code and place the executable files in the appropriate directories.

### Running CSEShell

After building, you can start the shell by running:

```bash
./cseshell
```

From there, you can execute built-in commands and any of the included system programs (e.g., `find`, `ld`, `ldr`).

## 2. Builtin functions supported:

* cd: Change the current working directory.
```bash
cd <directory>
# Example:
cd /home/user/documents
```

* help: Display the help information for built-in commands.
```bash
help
```

* exit: Exit the shell.
```bash
exit
```

* usage: Display usage information.
```bash
usage
```

* env: List all environment variables.
```bash
env
```

* setenv: Set an environment variable.
```bash
setenv <variable>=<value>
# Example:
setenv PATH=/usr/bin
```

* unsetenv: Unset an environment variable.
```bash
unsetenv <variable>
# Example:
unsetenv PATH
```

### System Programs

- `find.c` - Searches for files in a directory.
- `ld.c` - List the contents of the curent directory.
- `ldr.c` - List the contents of the current directory recursively.

Each program can be executed from the CSEShell once it is running. This starter code only allows the shell to execute a command once before exiting because `execv` replace the entire process' address space. Students need to fix this and allow the shell to prompt for more commands in Programming Assignment 1.

### System Programs that we created

* sys: Display simple system information.
```bash
sys
```

* dspawn: Spawn a daemon process.
```bash
dspawn
```

* dcheck: Check the status of a daemon process.
```bash
dcheck
```

* backup: Zip a specified directory and move the zipped directory to ./archive as a backup.
```bash
backup
```

### .cseshellrc File:
Supports setting the PATH environment variable and executing commands automatically at startup.
Place the .cseshellrc file in the root directory with the desired configurations.

## 3. Additional Features Supported:

### history: 
* The 'history' command lists all commands entered in the CLI from the start of a shell session to the most recent command.
* Commands are listed in chronological order, with the earliest entered commands appearing first and the most recent commands appearing last.
* The history includes all commands entered, including erroneous commands (which includes plain text) that was entered by mistake.
* Difference between built-in history command and our additional feature: All records are cleared when we relaunch our shell.

Command:
```bash
history
```
Output example:
```bash
1  ls -l
2  cd /usr/local
3  mkdir new_folder
4  wroncommand
5  echo "Hello, World!"
6  history
```
Benefits:
* Shell interfaces are typically small and concise, often with limited space for displaying extensive error messages or logs.
* The history command provides a concise summary list of all commands executed, which helps users quickly locate and address errors without having to scroll through long error messages.
* This summary list approach simplifies the user interface, making it easier to manage and review command execution history.


## 4. Considerations for Sustainability and Inclusivity:

### Sustainability:
#### Energy-Efficient Algorithms
We used algorithms that optimised energy efficiency.
Task: Implement an algorithm that sorts data efficiently or processes files with minimal CPU usage. Measure and compare the energy consumption of different approaches.

Implementation Details:

* Efficient Command Parsing and Execution: The shell minimizes resource usage by efficiently managing processes and memory allocation. The read_command function reads and parses user input efficiently, reducing unnecessary memory operations. The execute_command function handles command execution with proper resource management using fork() and waitpid(), ensuring that system resources are utilized efficiently.
* Automation to Reduce Repetitive Tasks: The shell supports automation through features like .cseshellrc, allowing users to automate repetitive tasks. This reduces the overall energy consumption by minimizing the need for manual intervention and repeated command execution.

### Inclusivity
#### Inclusive Error Messaging
We designed error messages to be clear, informative, and user-friendly and to avoid technical jargon.
Task: Write detailed, plain language error messages that help users understand and fix issues, including links to relevant documentation or tips.

Implementation Details:

* Clear and Accessible Error Messages: The shell provides clear and accessible error messages to assist users of all experience levels. Functions like shell_cd, shell_usage, set_env_var, and unset_env_var are designed to offer user-friendly error messages, ensuring that even novice users can understand and resolve issues.
* Extensive Documentation: The shell includes extensive documentation through help and usage commands (shell_help and shell_usage). This ensures users understand how to utilize the shell effectively, promoting an inclusive environment where users feel supported and informed.
* Customization via Environment Variables: Environment variable handling allows customization, catering to diverse user requirements and preferences. Users can set and unset environment variables to control the shell’s behavior, making it adaptable to various needs and enhancing the overall user experience.

By focusing on energy-efficient algorithms and inclusive error messaging, the shell is designed to be both sustainable and accessible, providing an optimal experience for all users.
