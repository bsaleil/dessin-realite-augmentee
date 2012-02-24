all: libtrack

libtrack: libtrack.o
	g++ -O2 libtrack.o -o libtrack `pkg-config --cflags --libs opencv`

libatrack.o : libtrack.cpp
	g++ -c libtrack.cpp 

clean:
	rm *.o
