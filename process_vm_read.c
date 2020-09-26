#define _GNU_SOURCE
#include <sys/uio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "termcolor.h"

#define KB(x)   ((size_t) (x) << 10)
#define MB(x)   ((size_t) (x) << 20)

#define PAGE_SIZE KB(4)

static void mem_write(struct iovec *local, struct iovec *remote, pid_t pid)
{
    char *ost = "ost";
    local[0].iov_base = ost;
    local[0].iov_len = strlen(ost);
    process_vm_writev(pid, local, 1, remote, 1, 0);
}

int main(int argc, char ** argv)
{
    struct iovec local[1];
    struct iovec remote[1];
    ssize_t nread;

    pid_t pid = strtol(argv[1], NULL, 10);
    void *mem_start = (void *) strtol(argv[2], 0, 16);
    void *mem_end   = (void *) strtol(argv[3], 0, 16);
    ssize_t buf_size = (mem_end - mem_start);

    text_cyan(stdout);

    printf("-- memoa -oa@2020-");
    reset_colors(stdout);

    text_blue(stdout);
    printf("\n[  Process  ]: ");
    text_red(stdout);
    printf("%zd", (unsigned long) pid);
    text_white(stdout);
    printf(" (pid)\n");

    text_blue(stdout);
    printf("[  Memory   ]: ");
    text_red(stdout);
    printf("0x%lx -> 0x%lx", (unsigned long) mem_start, (unsigned long) mem_end);
    text_white(stdout);
    printf(" (range)\n");

    text_blue(stdout);
    printf("[  Buffer   ]: ");
    text_red(stdout);
    printf("%zd", buf_size);
    text_white(stdout);
    printf(" (bytes)\n");


    char buf[buf_size];
    local[0].iov_base = buf;
    local[0].iov_len = buf_size;
    remote[0].iov_base = mem_start;
    remote[0].iov_len = buf_size;

    nread = process_vm_readv(pid, local, 1, remote, 1, 0);
    if (nread != buf_size && nread < 0) {
        text_underline(stdout);
        text_red(stdout);
        printf("MEM READ FAILED: %d\n", (int) nread);
    } else {
        //bomb it
        mem_write(local, remote, pid);
        text_green(stdout);
        printf("\nMEM READ SUCCESS\n");
        reset_colors(stdout);
        text_green(stdout);
        printf("=====================\n");
        for(int i = 0; i < sizeof(buf); i++)
            printf("%c: 0x%lx\n", buf[i], (unsigned long) (mem_start + i));
        text_blue(stdout);
        printf("[String]: %s\n", buf);
    }
    reset_colors(stdout);

    return 0;
}
