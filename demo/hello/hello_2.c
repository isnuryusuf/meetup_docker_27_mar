#include <stdio.h>

#define BUFFER_SIZE 512
char buffer[BUFFER_SIZE];

int main(int argc, char *argv[]) {
	printf("Hello ");
	if(argc < 2) printf("World\n");
	else         printf("%s\n", argv[1]);

	fgets(buffer, BUFFER_SIZE, stdin);
	printf("Your input: %s", buffer);

	return 0;
}
