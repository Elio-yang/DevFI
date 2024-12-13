#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

void tclflush(volatile void *p)
{
    asm volatile ("clflush (%0)" :: "r"(p));
}

// run this application backend using Ctrl+Z 
int main()
{

    pid_t pid = getpid();
    printf("pid %d\n",pid);
    char * data = malloc(100*sizeof(char));
    data [0] = 0x11;
    data [1] = 0x22;
    data [3] = 0x33;
    data [4] = 0x44;
    data [5] = 0xaa;
    printf("vaddr %p\n",data);
    while (1)
    {
        tclflush((void*)data);
    }
}
