#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

#include "common.h"

#define MEMSIZE   512

int main(int argc, char *argv[]) {
	const char * const uuid =  "d8641e7a-5727-441e-aa05-0caeba183a8c";
	const int key           = 0xd8641e7a;
	const int uuid_len      = strlen(uuid);

	ensure_argc(2, argc);

	if(strchr(argv[1], 'w')) {
		ensure_argc(3, argc);

		char *memptr = 0;

		int shmid = shmget(key, MEMSIZE, IPC_CREAT | IPC_EXCL | 0666);
		if(shmid != -1) { // creating
			memptr = (char *) shmat(shmid, 0, 0);
			if(memptr == (void *) -1) exit_errno();

			strcpy(memptr, uuid);
			memptr += uuid_len;

		} else { // try to reuse
			shmid = ensure(shmget(key, MEMSIZE, 0));
			memptr = (char *) shmat(shmid, 0, 0);
			if(memptr == (void *) -1) exit_errno();

			if(strncmp(memptr, uuid, uuid_len) != 0) exit_error("Invalid UUID");
			memptr += uuid_len;
		}

		if(strlen(argv[2]) == 0) shmctl(shmid, IPC_RMID, 0);
		else                     strncpy(memptr, argv[2], MEMSIZE - uuid_len);

	} else if(strchr(argv[1], 'r')) {
		int shmid = ensure(shmget(key, MEMSIZE, 0));

		char *memptr = (char *) shmat(shmid, 0, 0);
		if(memptr == (void *) -1) exit_errno();

		if(strncmp(memptr, uuid, uuid_len) != 0) exit_error("Invalid UUID");
		memptr += uuid_len;

		printf("%s\n", memptr);

	}

	return 0;
}
