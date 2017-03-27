// You can use "nseter" that already provided by the OS

#define _GNU_SOURCE
#include <sched.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

#include "common.h"

static char buffer[512];

static void nsenter(char *pid, char *type) {
	snprintf(buffer, ARRAY_SIZE(buffer), "/proc/%s/ns/%s", pid, type);
	int fd = ensure(open(buffer, 0));
	ensure(setns(fd, 0));
	ensure(close(fd));
}

int main(int argc, char *argv[]) {
	ensure_argc(4, argc);

	int flags = 0;
	if(strchr(argv[1], 'u')) nsenter(argv[2], "uts");
	if(strchr(argv[1], 'i')) nsenter(argv[2], "ipc");
	if(strchr(argv[1], 'p')) nsenter(argv[2], "pid");
	if(strchr(argv[1], 'n')) nsenter(argv[2], "net");

	// beacuse we need to open file on filesystem, we need to enter mount ns lastly
	if(strchr(argv[1], 'm')) nsenter(argv[2], "mnt");

	pid_t cpid = ensure(fork());
	if(cpid == 0) ensure(chain_command(3, argc, argv));

	int status;
	ensure(waitpid(cpid, &status, 0));
	return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
}
