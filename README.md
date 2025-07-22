# 🖥️ Systems Programming – UNIX Projects

This repository contains coursework-based projects focused on **low-level systems programming** in a UNIX/Linux environment using C and shell scripting. Each project demonstrates core OS-level concepts such as system calls, process control, and file handling.

---

## 📁 Project 1 – File I/O and System Calls

Focuses on **file manipulation using low-level C system calls** like `open()`, `read()`, `write()`, and `close()`.

### ✅ Features
- Reads input from files using `read()`
- Writes output using `write()` with file descriptors
- Manages errors with manual return value checks
- Demonstrates precise control over file descriptors

## 📁 Project 2 – Process Creation and Execution

Explores **process management** in UNIX using `fork()`, `exec()`, and `wait()`. Demonstrates how child processes can be used to execute shell commands and chain them together.

---

### ✅ Features
- Creates child processes using `fork()`
- Executes shell commands with `execvp()`
- Waits for child termination using `wait()`
- Demonstrates simple shell behavior and command chaining

# 💬 Project 3 - TCP Chat Client – C Socket Programming

This project is a simple, terminal-based **TCP chat client** built in C using **POSIX sockets** and **multithreading**. It connects to a server, allows the user to send and receive messages asynchronously, and supports username identification and graceful shutdown.

---

## ✅ Features

- Connects to a remote chat server via IP and port
- Prompts for and sends a username to identify the client
- Sends user-typed messages to the server
- Receives server messages in real-time using a background thread
- Supports clean disconnection using `/quit` command



