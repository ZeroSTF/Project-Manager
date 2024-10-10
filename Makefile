CC = gcc
CFLAGS = -Wall -pthread $(shell pkg-config --cflags gtk+-3.0)
LIBS = $(shell pkg-config --libs gtk+-3.0)
OBJS = main.o project.o networking.o gui.o

all: project_manager

project_manager: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

main.o: main.c project.h networking.h gui.h
	$(CC) $(CFLAGS) -c main.c

project.o: project.c project.h
	$(CC) $(CFLAGS) -c project.c

networking.o: networking.c networking.h project.h
	$(CC) $(CFLAGS) -c networking.c

gui.o: gui.c gui.h project.h
	$(CC) $(CFLAGS) -c gui.c

clean:
	rm -f *.o project_manager
