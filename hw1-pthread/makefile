CC = gcc
CFLAGS = -std=gnu99 -O2
LIBS = -pthread
OBJS = pi

all:pi.c
	$(CC) $(LIBS) $(CFLAGS) -s pi.c -o $(OBJS)
clean:
	rm -f pi
