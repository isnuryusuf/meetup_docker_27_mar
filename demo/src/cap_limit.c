#include <unistd.h>
#include <sys/prctl.h>
#include <linux/capability.h>

#include "common.h"

// default cap in docker container
// see: https://github.com/docker/docker/blob/8d1d9eebe3ded3105216dc9a88cc94d16ac6198c/oci/defaults_linux.go#L62
static unsigned long caplist[] = {
	CAP_CHOWN,
	CAP_DAC_OVERRIDE,
	CAP_FSETID,
	CAP_FOWNER,
	CAP_MKNOD,
	CAP_NET_RAW,
	CAP_SETGID,
	CAP_SETUID,
	CAP_SETFCAP,
	CAP_SETPCAP,
	CAP_NET_BIND_SERVICE,
	CAP_SYS_CHROOT,
	CAP_KILL,
	CAP_AUDIT_WRITE,
};

static int in_list(unsigned long what) {
	for(int i = 0; i < ARRAY_SIZE(caplist); ++i)
		if(caplist[i] == what)
			return 1;
	return 0;
}

int main(int argc, char *argv[]) {
	ensure_argc(2, argc);

	for(int cap = 0; cap <= CAP_LAST_CAP; ++cap)
		if(!in_list(cap) && cap != CAP_SETPCAP)
			ensure(prctl(PR_CAPBSET_DROP, cap, 0, 0, 0));

	// CAP_SETPCAP is needed to do PR_CAPBSET_DROP, we need to drop it lastly
	if(!in_list(CAP_SETPCAP)) ensure(prctl(PR_CAPBSET_DROP, CAP_SETPCAP, 0, 0, 0));

	ensure(chain_command(1, argc, argv));
}
