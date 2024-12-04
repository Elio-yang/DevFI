#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <string.h>
#define PAGE_SIZE 0x1000

int main(){
    char buf[BUFSIZ];
    int ram_fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (ram_fd < 0){
        perror("open");
        exit(1);
    }else{
        printf("open /dev/mem success\n");
    }
    //             0x443a99130
    uint64_t pfn = 0x443a99000;
    // map the ram
    
    // read the ram_fd at offset pfn
    lseek(ram_fd, pfn, SEEK_SET);
    read(ram_fd, buf, PAGE_SIZE);
    for (int i = 0; i < PAGE_SIZE; i++){
        printf("%02x ", buf[i]);
        if (i % 16 == 15){
            printf("\n");
        }
    }
}