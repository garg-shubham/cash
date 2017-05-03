# CA$H
> A mini shell written in the C language.

## Features

* I/O redirection.
* Piping.
* Command history.
* Color based on errors or successful execution.
* Easy integration with any linux distro.

## Installation

### Dependencies
* GNU make
* rlwrap
* GCC

### Setup instructions.
* git clone `https://github.com/garg-shubham/cash` ~/cash
* Run `make install` in the source directory 
* Add `rlwrap -a ~/cash/cash` to your default shell's configuration file (~/.bashrc for bash, ~/.zshrc for zsh etc.)