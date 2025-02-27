# Custom Shell in C

## Overview

This is a simple custom shell implemented in C. It provides basic command execution, variable assignment, and built-in commands like `cd`, `echo`, and `export`. The shell also supports background processes.

## Features

- Execute system commands using `execvp`
- Built-in commands:
  - `cd <directory>`: Change the current working directory
  - `echo <message>`: Print a message to the console
  - `export VAR=value`: Set an environment-like variable
- Variable substitution using `$VAR`
- Background process execution using `&`
- Signal handling for child processes (`SIGCHLD`)

## Prerequisites

- GCC compiler
- Linux or macOS environment (for `execvp` and process handling functions)

## Installation & Compilation

```sh
# Clone the repository
git clone <repository-url>
cd <repository-folder>

# Compile the shell
gcc -o custom_shell shell.c
```

## Usage

```
#Run the shell
./custom_shell
```

## Example Comands

```
Enter input: echo Hello World
Hello World

Enter input: export MYVAR=123

Enter input: echo $MYVAR
123

Enter input: ls &  # Run `ls` in the background
```

## Code Stracture

```
        • setup_environment(): Sets up the shell environment.
	• evaluate_expression(): Handles variable substitution.
	• parseInput(): Tokenizes user input.
	• execute_shell_builtin(): Handles built-in commands (cd, echo, export).
	• execute_command(): Executes external system commands.
	• shell(): Main loop for user interaction.
	• on_child_exit(): Handles background process termination.
```
