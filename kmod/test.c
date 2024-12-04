#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

void tclflush(volatile void *p)
{
    asm volatile ("clflush (%0)" :: "r"(p));
}

int main()
{

    pid_t pid = getpid();
    printf("pid %d\n",pid);
    volatile uint64_t test_value = 0x1111111111111111;
    printf("vaddr %p\n",&test_value);
    while (1)
    {
        test_value = 0x1111111111111111;
        tclflush((void*)&test_value);
    }
}