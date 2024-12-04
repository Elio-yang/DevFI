#include <sys/types.h>		
#include <sys/stat.h>
#include <sys/termios.h>	
#include <stdio.h>		
#include <stdlib.h>	
#include <stddef.h>	
#include <string.h>	
#include <unistd.h>	
#include <fcntl.h>
#include <sys/mman.h>


int main()
{
    char *file = "data";
    int fd = open(file, O_RDWR);
    if (fd < 0){
        perror("open");
        exit(1);
    }
    int len = 4*1024;
    int off = 4*1024;
    char *ptr = mmap(0, len, PROT_READ|PROT_WRITE, MAP_SHARED, fd, off);
    if (ptr == MAP_FAILED){
        perror("mmap");
        exit(1);
    }
    while (*ptr){
        write(STDOUT_FILENO, ptr, 128);
        sleep(5);
    }
    munmap(ptr, len);
    close(fd);
    return 0;
}