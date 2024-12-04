#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <string.h>

#define PAGE_SIZE 0x1000

inline void do_print_page(uint64_t address, uint64_t data) {
    printf("0x%-16lx : pfn %-16lx soft-dirty %ld file/shared %ld "
        "swapped %ld present %ld\n",
        address,
        data & 0x7fffffffffffff,
        (data >> 55) & 1,
        (data >> 61) & 1,
        (data >> 62) & 1,
        (data >> 63) & 1);
}

inline void print_page_range(pid_t pid, uint64_t start_vaddr, uint64_t end_vaddr){
    char filename[BUFSIZ];
    snprintf(filename, sizeof filename, "/proc/%d/pagemap", pid);
    int fd = open(filename, O_RDONLY);
    if (fd < 0){
        perror("open");
        exit(1);
    }

    for(uint64_t vaddr = start_vaddr; vaddr < end_vaddr; vaddr += PAGE_SIZE){
        uint64_t data;
        uint64_t idx = (vaddr / PAGE_SIZE) * sizeof(data);
        // read the pagemap
        if(pread(fd, &data, sizeof(data), idx) != sizeof(data)){
            perror("pread");
            exit(1);
        }
        // print the info
        do_print_page(vaddr, data);
    }
    close(fd);
}

// vaddr must be page aligned
inline uint64_t get_pfn(pid_t pid, uint64_t vaddr){
    char filename[BUFSIZ];
    snprintf(filename, sizeof filename, "/proc/%d/pagemap", pid);
    int fd = open(filename, O_RDONLY);
    if (fd < 0){
        perror("open");
        exit(1);
    }

    uint64_t data;
    uint64_t idx = (vaddr / PAGE_SIZE) * sizeof(data);
    // read the pagemap
    if(pread(fd, &data, sizeof(data), idx) != sizeof(data)){
        perror("pread");
        exit(1);
    }
    // print the info
    uint64_t pfn = data & 0x7fffffffffffff;
    close(fd);
    return pfn;
}

inline uint64_t get_phy_page_addr(uint64_t pfn){
    return pfn * PAGE_SIZE ;
}


inline void print_physical_page(uint64_t pfn){
    char buf[BUFSIZ];
    int ram_fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (ram_fd < 0){
        perror("open");
        exit(1);
    }else{
        printf("open /dev/mem success\n");
    }

    // map the ram
    char *addr = mmap(0, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, ram_fd, get_phy_page_addr(pfn));
    if (addr == MAP_FAILED){
        perror("mmap");
        exit(1);
    }


    memcpy(buf, addr, PAGE_SIZE);
    for (int i = 0; i < PAGE_SIZE; i++){
        printf("%02x ", buf[i]);
        if (i % 16 == 15){
            printf("\n");
        }
    }
    munmap(addr, PAGE_SIZE);
}
