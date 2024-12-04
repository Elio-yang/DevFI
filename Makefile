ALL= mmap_vic mmap_atk mmap_vic2
CFLAGS= -ggdb3 -O2 -std=gnu99 
all : $(ALL)
	chmod  777 $(ALL)

mmap_atk: mmap_atk.c
	gcc $(CFLAGS) mmap_atk.c -o mmap_atk
mmap_vic: mmap_vic.c
	gcc $(CFLAGS) mmap_vic.c -o mmap_vic
mmap_vic2: mmap_vic2.c
	gcc $(CFLAGS) mmap_vic2.c -o mmap_vic2

clean:
	rm -f $(ALL)