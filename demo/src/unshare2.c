// You can use "unshare" that already provided by the OS

#define _GNU_SOURCE
#include <sched.h>
#include <sys/wait.h>
#include <string.h>

#include "common.h"

int main(int argc, char *argv[]) {
	ensure_argc(3, argc);

	int flags = 0;
	if(strchr(argv[1], 'm')) flags |= CLONE_NEWNS;
	if(strchr(argv[1], 'u')) flags |= CLONE_NEWUTS;
	if(strchr(argv[1], 'i')) flags |= CLONE_NEWIPC;
	if(strchr(argv[1], 'p')) flags |= CLONE_NEWPID;
	if(strchr(argv[1], 'n')) flags |= CLONE_NEWNET;

	ensure(unshare(flags));

	pid_t cpid = ensure(fork());
	if(cpid == 0) ensure(chain_command(2, argc, argv));

	int status;
	ensure(waitpid(cpid, &status, 0));
	return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
}
