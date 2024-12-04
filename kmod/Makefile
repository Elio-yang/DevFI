obj-m+=pw.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	gcc  v2pa1.c -o v2pa1
	gcc  v2pa2.c -o v2pa2
	gcc  test.c -o test
