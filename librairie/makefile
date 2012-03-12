all: libblob

libtrack: libtrack.o
	g++ -O2 libtrack.o -o libtrack `pkg-config --cflags --libs opencv`

libatrack.o : libtrack.cpp
	g++ -c libtrack.cpp

libblob: libblob.o
	g++ -O2 libblob.o -o libblob `pkg-config opencv cvblob --cflags --libs`

libblob.o : libblob.cpp
	g++ -c libblob.cpp 

clean:
	rm *.o
