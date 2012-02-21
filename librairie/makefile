all: libtrack

libtrack: libtrack.o
	gcc libtrack.o -o libtrack `pkg-config --cflags --libs opencv`

libatrack.o : libtrack.c
	gcc -Wall -fPIC -c libtrack.c

clean:
	rm *.o
