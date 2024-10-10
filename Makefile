CC = gcc
CFLAGS = -Wall -pthread
OBJS = main.o project.o networking.o

all: project_manager

project_manager: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

main.o: main.c project.h networking.h
	$(CC) $(CFLAGS) -c main.c

project.o: project.c project.h
	$(CC) $(CFLAGS) -c project.c

networking.o: networking.c networking.h project.h
	$(CC) $(CFLAGS) -c networking.c

clean:
	rm -f *.o project_manager













