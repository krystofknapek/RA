# RA
# MacOS Keylogger 

## Overview

This project focuses on creating keylogger supporting Czech keyboard layout, implemented for educational purposes only.

## Features

## Features

- **Keylogger Implementation:**
  - Captures keyboard inputs including special keys like `[ENTER]`, `[TAB]`, `[SPACE]`, etc.
  - Detects keyboard modifiers like `[SHIFT]`, `[CONTROL]`, `[COMMAND]`, and `[OPTION]`.
  - Logs keystrokes into a `log.txt` file for analysis.

- **Detection and Prevention Focus:**
  - Designed to test methods for detecting malicious software.
  - Emphasizes reverse analysis techniques to understand its operation and mitigate potential threats.

## Project Structure

- **Source Code:**
  - `keylogger.cpp`: Contains the main implementation of the keylogger.

- **Log File:**
  - `log.txt`: Stores captured keystrokes for further analysis.

## Keylogger Behavior

- Forks into a child process to run in the background.
- Redirects standard input/output/error to `/dev/null` for stealth operation.
- Launches a calculator application to mask its activity.
- Monitors keystrokes using macOS-specific APIs (`ApplicationServices`).

## Installation and Usage

1. **Compile the Source Code**:
   Use the following command to compile the code:
   ```bash
   g++ keylogger.cpp -o keylogger -framework ApplicationServices

## Turning off krylogger
kill <PID>
ps aux | grep keylogger


## TODO
  - sending logs from local to external email/server
