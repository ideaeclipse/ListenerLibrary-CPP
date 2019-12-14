CC = g++
CPPFLAGS = -std=c++11 -g
OBJ = main.o ListenerLibrary.o

Output: $(OBJ)
	$(CC) $(CPPFLAGS) $(OBJ)

ListenerLibrary.o:
	$(CC) $(CPPFLAGS) -c ListenerLibrary.hpp -o ListenerLibrary.o

main.o:
	$(CC) $(CPPFLAGS) -c main.cpp -o main.o

clean:
	rm *.o a.out
