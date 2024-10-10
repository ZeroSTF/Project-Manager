CC = gcc
CFLAGS = -Wall -pthread
OBJS = main.o project.o networking.o

all: project_manager

project_manager: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $

clean:
	rm -f *.o project_manager
