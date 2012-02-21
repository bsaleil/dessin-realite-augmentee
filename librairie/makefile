all: libtrack

libtrack: libtrack.o
	gcc -O2 libtrack.o -o libtrack `pkg-config --cflags --libs opencv`

libatrack.o : libtrack.c
	gcc -c libtrack.c 

clean:
	rm *.o
