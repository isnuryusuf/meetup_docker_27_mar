#ifndef __SRC_COMMON_H
#define __SRC_COMMON_H

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

static void _exit_error(int line, char *msg) {
	fprintf(stderr, "[ERROR line %3d] %s\n", line, msg);
	_exit(1);
}

static void _exit_errno(int line) {
	_exit_error(line, strerror(errno));
}

static int _ensure(int line, int retval) {
	if(retval == -1) _exit_errno(line);
	return retval;
}

static void _ensure_argc(int line, int needed, int argc) {
	if(argc < needed) _exit_error(line, "Incomplete argument");
}

#define exit_error(a)      _exit_error(__LINE__, a)
#define exit_errno()       _exit_errno(__LINE__)
#define ensure_argc(a, b)  _ensure_argc(__LINE__, a, b)
#define ensure(a)          _ensure(__LINE__, a)

#define ARRAY_SIZE(a)       sizeof(a)/sizeof(a[0])

static int chain_command(int shift, int argc, char *argv[]) {
	if (argc - shift <= 0) { errno = EINVAL; return -1; }

	char *new_arg[argc - shift + 1];
	for(int i = shift; i < argc; ++i) new_arg[i - shift] = argv[i];
	new_arg[argc - shift] = 0;

	return execvp(new_arg[0], new_arg);
}

#endif
