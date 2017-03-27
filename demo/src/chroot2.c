// You can use "chroot" that already provided by the OS

#include <unistd.h>

#include "common.h"

int main(int argc, char *argv[]) {
	ensure_argc(3, argc);

	ensure(chroot(argv[1]));
	ensure(chdir("/"));

	ensure(chain_command(2, argc, argv));
}
