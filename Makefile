OBJS = initial.o main.o

CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

initial: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o initial -lpthread

main.o: main.cpp node.h
	$(CC) $(CFLAGS) main.cpp -lpthread

initial.o: initial.cpp node.h
	$(CC) $(CFLAGS) initial.cpp -lpthread

clean:
	\rm *.o initial
