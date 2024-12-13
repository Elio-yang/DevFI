#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>

#define ORIG_BUFFER "DATA WRITTEN BY USER"
#define NEW_BUFFER "DATA CHANGED BY DEVMEM"

#define PAGE_SHIFT 12
#define PAGEMAP_LENGTH 8

void* create_buffer(void);
unsigned long get_page_frame_number_of_address(void *addr);
int open_memory(void);
void seek_memory(int fd, unsigned long offset);

int main(void) {
   void *buffer = create_buffer();

   // 1. get physical address of buffer (pdf here, page aligned)
   unsigned int page_frame_number = get_page_frame_number_of_address(buffer);
   printf("Page frame: 0x%x\n", page_frame_number);
   // 2. get address offset
   unsigned int distance_from_page_boundary = (unsigned long)buffer % getpagesize();
   // 3. exact physical address
   uint64_t offset = (page_frame_number << PAGE_SHIFT) + distance_from_page_boundary;
   // 4. open /dev/mem
   int mem_fd = open_memory();
   // 5. seek to the physical address
   seek_memory(mem_fd, offset);
   printf("Buffer: %s\n", buffer);
   // 6. write to the physical address [change the value using /dev/mem]
   if(write(mem_fd, NEW_BUFFER, strlen(NEW_BUFFER)) == -1) {
      fprintf(stderr, "Write failed: %s\n", strerror(errno));
   }
   printf("Buffer: %s\n", buffer);
   free(buffer);
   close(mem_fd);
   return 0;
}


void* create_buffer(void) {
   size_t buf_size = strlen(ORIG_BUFFER) + 1;

   void *buffer = malloc(buf_size);
   if(buffer == NULL) {
      fprintf(stderr, "Failed to allocate memory for buffer\n");
      exit(1);
   }

   // Lock the page in memory
   if(mlock(buffer, buf_size) == -1) {
      fprintf(stderr, "Failed to lock page in memory: %s\n", strerror(errno));
      exit(1);
   }

   strncpy(buffer, ORIG_BUFFER, strlen(ORIG_BUFFER));

   return buffer;
}

unsigned long get_page_frame_number_of_address(void *addr) {

   FILE *pagemap = fopen("/proc/self/pagemap", "rb");

   unsigned long offset = (unsigned long)addr / getpagesize() * PAGEMAP_LENGTH;
   if(fseek(pagemap, (unsigned long)offset, SEEK_SET) != 0) {
      fprintf(stderr, "Failed to seek pagemap to proper location\n");
      exit(1);
   }

   // The page frame number is in bits 0-54 so read the first 7 bytes and clear the 55th bit
   unsigned long page_frame_number = 0;
   fread(&page_frame_number, 1, PAGEMAP_LENGTH-1, pagemap);

   // pfn for physical address
   page_frame_number &= 0x7FFFFFFFFFFFFF;

   fclose(pagemap);

   return page_frame_number;
}

int open_memory(void) {
   // kernel must be configured with CONFIG_STRICT_DEVMEM=n
   int fd = open("/dev/mem", O_RDWR);
   if(fd == -1) {
      fprintf(stderr, "Error opening /dev/mem: %s\n", strerror(errno));
      exit(1);
   }
   return fd;
}

void seek_memory(int fd, unsigned long offset) {
   unsigned pos = lseek(fd, offset, SEEK_SET);
   if(pos == -1) {
      fprintf(stderr, "Failed to seek /dev/mem: %s\n", strerror(errno));
      exit(1);
   }
}