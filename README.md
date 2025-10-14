# 🔧 Custom Unix Pipe

A **lightweight Uinux pipeline engine** implemented in **C**, designed
to handle multi-process execution and I/O redirection with precision and
efficiency.

------------------------------------------------------------------------

## 🚀 Overview

This project re-engineers the functionality of the **Unix pipe system**,
enabling process chaining and data flow between commands using custom
file descriptor management. It provides a clear, performant architecture
for command execution and inter-process communication --- built entirely
from the ground up in C.

Also it is built on top of my **custom C standard library**,
developed separately to provide optimized string manipulation, memory
handling, and utility functions used throughout the pipeline logic.

------------------------------------------------------------------------

## ⚙️ Features

-   **Custom Pipeline Engine:** Efficiently connects process
    input/output streams using low-level file descriptors.
-   **Multi-Command Execution:** Supports chained processes with dynamic
    argument parsing and execution flow.
-   **Process Management:** Handles process creation, synchronization,
    and clean termination.
-   **Error Control:** Reliable handling of invalid commands, I/O
    failures, and process exit statuses.
-   **Custom Library Integration:** Utilizes a personal **standard C
    library** for improved control and consistency.
-   **Modular Architecture:** Independent modules for parsing,
    execution, and system I/O.

------------------------------------------------------------------------

## 💻 Usage

Run the executable with an input file, a list of commands, and an output
file:

``` bash
./custom-linux-pipe infile cmd1 cmd2 cmd3 outfile
```

Example:

``` bash
./custom-linux-pipe input.txt "grep data" "sort" "uniq -c" output.txt
```

This command: 1. Reads from `input.txt` 2. Pipes the output of
`grep data` into `sort`, then into `uniq -c` 3. Writes the final output
to `output.txt`

------------------------------------------------------------------------

## 🧠 Engineering Highlights

-   Built for **robustness and speed**, closely mirroring shell-level
    pipeline behavior.
-   Implements **core Unix system calls** (`fork()`, `execve()`,
    `dup2()`, `pipe()`) for full control of process flow.
-   Integrates a **custom C standard library** to ensure consistent
    low-level behavior and optimized utilities.
-   Optimized for **resource safety** and **low-level performance**
    under concurrent execution.

------------------------------------------------------------------------

## 🛠️ Technologies

**Language:** C\
**Environment:** Linux\
**Libraries:** Custom C Standard Library \
**Core Concepts:** System Calls, Process Management, I/O Redirection,
File Descriptors

------------------------------------------------------------------------
