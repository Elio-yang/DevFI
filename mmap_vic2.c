
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
#include "pagemap.h"

#define COPYINCR (1024 * 1024 * 1024)

int main()
{
    pid_t pid = getpid();
    printf("pid: %d\n", pid);
    char buf[4096*4];
    // this pointer must be page aligned
    // to access pfn, etc.
    char *new_ptr = buf;
    while (scanf("%s", buf) != EOF){
        printf("vaddr: %p\n", new_ptr);
        printf("page table info\n");
        print_page_range(pid,(uint64_t)new_ptr,(uint64_t)new_ptr+PAGE_SIZE);
        printf("====================================\n");
        uint64_t pfn = get_pfn(pid, (uint64_t)new_ptr);
        uint64_t ppa = get_phy_page_addr(pfn);
        printf("pfn: %lx\n", pfn);
        printf("ppa: %lx\n", ppa);
        print_physical_page(pfn);
        
        memcpy(new_ptr, buf, strlen(buf));
        new_ptr += PAGE_SIZE;
    }
    
    return 0;
}