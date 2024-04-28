# NAME

mutexed - Run one instance of a program at the same time

# SYNOPSIS

    mutexed [-v][-n NAME] COMMAND...

# INSTALLING

Download [mutexed.exe](./mutexed.exe) and save it in your path (ie "C:/Windows").

# DESCRIPTION

This program allows you to ensure that only one instance of a command
is executed at a time.

This program understands the following flags.

*-v* Execute in verbose mode, printing to the standard error when a
lock happens.

*-n NAME* By default a mutex named "MUTEXED_DEFAULT" is used, you can
change this to "MUTEXED_NAME" with this argument.

*-t SECS* Wait SECS seconds at maximun other instances to finish, by
default 3600 seconds (one hour).

*-r SECS* Wait blocks of SECS seconds. This makes more probable that
subsequent calls to mutexed from the same script are executed one after
the other.

# EXAMPLES

Ensure that "myapp" is called alone.

    mutexed -n myapp myapp.exe

Test locking with "pause" in MS Windows.

    mutexed -n myapp cmd /C pause

# RETURN VALUE

The one COMMAND returns, 1 in case of failure.

# COLLABORATING

For making bug reports, feature requests and donations visit
one of the following links:

1. [gemini://harkadev.com/oss/](gemini://harkadev.com/oss/)
2. [https://harkadev.com/oss/](https://harkadev.com/oss/)

# SEE ALSO

**flock(1)**, **pause(1)**
