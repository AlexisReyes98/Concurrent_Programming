#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    printf("\n\nHola Mundo.\n");

    for (int i=0; i<10; i++) {
	fflush(stdout);
	printf("%d ", i);
	sleep(2);
    }
    printf("\n");
    
    return 0;
}
