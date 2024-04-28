#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <libgen.h>
#include <assert.h>
#include <windows.h>
#include <stdbool.h>

char const HELP[] =
    "Usage: mutexed [OPTS...] COMMAND..."                                  "\n"
    ""                                                                     "\n"
    "Ensure that only one instance of COMMAND is executed at the same time. \n"
    ""                                                                     "\n"
    "  -v       : Verbose mode."                                           "\n"
    "  -n NAME  : Use NAME as the mutex name (default MUTEXED)."           "\n"
    "  -r SECS  : Retry waiting every SECS seconds (Default 0)."           "\n"
    "  -t SECS  : Timeout after SECS seconds (Default 3600)."              "\n"
    ""                                                                     "\n"
    "Examples:"                                                            "\n"
    "  mutexed -n myapp myapp.exe"                                         "\n"
    "  mutexed -n myapp -r 5 -t 60 myapp.exe"                              "\n"
    "  mutexed -n myapp cmd /C pause"                                      "\n"
    ""                                                                     "\n"
    "Copyright (c) 2024 Harkaitz Agirre, harkaitz.aguirre@gmail.com"       "\n"
    ;

int
main(int _argc, char *_argv[])
{
	int	 opt;
	char	*name = "DEFAULT";
	HANDLE	 mutex;
	int	 ret;
	bool	 verbose = false;
	DWORD	 retry_waiting_seconds = 0;
	DWORD	 failure_timeout_seconds = 3600;
	DWORD	 total_waited_seconds = 0;
	DWORD	 wait_result;
	
	if (_argc == 1 || !strcmp(_argv[1], "-h") || !strcmp(_argv[1], "--help")) {
		printf("%s", HELP);
		return 0;
	}
	
	while ((opt = getopt(_argc, _argv, "vn:r:t:")) != -1) {
		switch (opt) {
		case 'n': name = optarg; break;
		case 'v': verbose = true; break;
		case 'r': retry_waiting_seconds = atoi(optarg); break;
		case 't': failure_timeout_seconds = atoi(optarg); break;
		default:
			return 1;
		}
	}
	
	if (optind >= _argc) {
		fprintf(stderr, "Error: missing COMMAND.\n");
		return 1;
	}
	
	char mutex_name[strlen("MUTEXED_") + strlen(name) + 1];
	sprintf(mutex_name, "MUTEXED_%s", name);
	
	mutex = CreateMutex(NULL, FALSE, mutex_name);
	if (mutex == NULL) {
		fprintf(stderr, "Error: CreateMutex failed.\n");
		return 1;
	}
	
	
	if (!retry_waiting_seconds) {
		wait_result = WaitForSingleObject(mutex, failure_timeout_seconds);
		if (wait_result != WAIT_OBJECT_0 && wait_result != WAIT_ABANDONED) {
			fprintf(stderr, "Error: WaitForSingleObject failed.\n");
			return 1;
		}
	}
	
	while (retry_waiting_seconds) {
		wait_result = WaitForSingleObject(mutex, 0);
		if (wait_result == WAIT_TIMEOUT) {
			if (verbose) {
				fprintf(stderr, "Waiting other instances %li/%li...\n", total_waited_seconds, failure_timeout_seconds);
			}
			Sleep(retry_waiting_seconds * 1000);
			total_waited_seconds += retry_waiting_seconds;
		} else if (wait_result == WAIT_OBJECT_0 || wait_result == WAIT_ABANDONED) {
			break;
		} else {
			fprintf(stderr, "Error: WaitForSingleObject failed.\n");
			return 1;
		}
		if (total_waited_seconds >= failure_timeout_seconds) {
			fprintf(stderr, "Error: Timeout.\n");
			return 1;
		}
	}
	
	ret = _spawnvp(_P_WAIT, _argv[optind], (char const **) _argv + optind);
	
	ReleaseMutex(mutex);
	CloseHandle(mutex);
	
	return ret;
}
