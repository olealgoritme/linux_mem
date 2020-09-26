#define _GNU_SOURCE
#include <sys/uio.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <string.h>

int main(void)
{

    char hideme[3] = "kuk";
    printf("PID: %d\n", getpid());
    printf("Addr: %p\n", &hideme);

    for(;;) {
        if(strcmp(hideme, "kuk") != 0) {
            printf ("hideme changed to: %s\nresetting hideme\n\n",  hideme);
            hideme[0] = 'k'; hideme[1] = 'u'; hideme[2] = 'k';
        }
        usleep(500);
    }
    return 0;
}

