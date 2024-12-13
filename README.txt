DevFI: /dev/mem based fault injection

Yuhao Peng (fhw9hh@virginia.edu)
Yang Yang  (yangyang@virginia.edu)
Jasmine Zhang (qz6tf@virginia.edu)

University of Virginia

A. Project description
    1. ptw-kern/
        a kernel module with hardcoded page table walk (PTW) example

    2. devfi-devmem-test.c
        a test program to verify whther /dev/mem works
    
    3. devfi-example.c 
        a demo program which has been shown in presentation
    
    4. devfi-va2pa.c
        a utility program that transfer virtual address to physical address

    5. devfi-inject.c
        a injection tool to read and write physical memory

    6. pagemap.h
        needed linux support for virtual/physical address mapping
    
    7. mmap_atk.c and mmap_vic.c
        simulate physical memory using file

B. Usage
    1. Linux kernel must be configured with CONFIG_STRICT_DEVMEM=n
    2. Run make in the current folder
    3. For the ptw module, make it in that folder with correct makefile linux kernel source updated
    4. For devfi-devmem-test.bin, directly execute it. 
        a. if /dev/mem is working, the you should see:
            Buffer: DATA WRITTEN BY USER
            Buffer: DATA CHANGED BY DEVMEM
        b. otherwise recompile the kernel
    5. Run devfi-example.bin
        a. it output a pid
        b. it also output a vaddr
        c. open another terminal and run step 5
    5. For devfi-va2pa.bin, run it by passing process pid and a virtual address
        a. run ./devfi-va2pa.bin <pid> <vaddr>
        b. it should output a paddr in decimal
    6. Trigger error using devfi-inject.bin
        a. read a special physical location
            ./devfi-inject.bin <paddr>
        b. write to a special physical location
            ./devfi-inject.bin <paddr> [b/h/w] <data>
    7. mmap attck example
        a. just run a victim process and run the attcker


Credit: https://github.com/brgl/busybox/blob/master/miscutils/devmem.c