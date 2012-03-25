all: libblob

libtrack: libtrack.o
	g++ libtrack.o -o libtrack `pkg-config opencv cvblob --cflags --libs`

libatrack.o : libtrack.cpp
	g++ -c libtrack.cpp
	
clean:
	rm *.o
