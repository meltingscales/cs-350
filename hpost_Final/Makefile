CC=gcc
CFLAGS=-I.

all: build clean

clean:
	rm *.o -f

build: hpost_LC3_part2.o
	$(CC) -o hpost_LC3_part2.exe hpost_LC3_part2.o

run:
	./hpost_LC3_part2.exe