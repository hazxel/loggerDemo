CC = clang++

all: main liblogger test

logger: logger.cpp
	$(CC) logger.cpp -c

liblogger: logger
	ar -r liblogger.a logger.o

main: liblogger main.cpp
	$(CC) main.cpp liblogger.a -o main

test: liblogger test.cpp
	$(CC) test.cpp liblogger.a -o test

clean:
	rm -f *.o *.a main