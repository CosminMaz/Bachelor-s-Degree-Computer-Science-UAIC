CC=g++

CFLAGS=-c -Wall -O3

all: app

app: main.o Function.o Rastrigin.o DeJong.o Schwefel.o Michalewicz.o 
	$(CC) main.o Function.o Rastrigin.o DeJong.o Schwefel.o Michalewicz.o -o app

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

Function.o: Function.cpp
	$(CC) $(CFLAGS) Function.cpp

Rastrigin.o: Rastrigin.cpp
	$(CC) $(CFLAGS) Rastrigin.cpp

DeJong.o: DeJong.cpp
	$(CC) $(CFLAGS) DeJong.cpp

Schwefel.o: Schwefel.cpp
	$(CC) $(CFLAGS) Schwefel.cpp

Michalewicz.o: Michalewicz.cpp
	$(CC) $(CFLAGS) Michalewicz.cpp

clean:
	rm -rf *o app
