# Compiler and flags
CC = g++
CFLAGS = -Wall -O3
OBJFLAGS = -c

# Targets
all: server client

# Server target
server: server.o Command.o XMLClass.o
	$(CC) server.o Command.o XMLClass.o -o server

# Client target
client: client.o
	$(CC) client.o -o client

# Compilation rules for object files
%.o: %.cpp %.h
	$(CC) $(CFLAGS) $(OBJFLAGS) $< -o $@

server.o: serverfinal7.cpp
	$(CC) $(CFLAGS) $(OBJFLAGS) serverfinal7.cpp -o server.o

client.o: client.cpp
	$(CC) $(CFLAGS) $(OBJFLAGS) client.cpp -o client.o

# Clean rule
clean:
	rm -f *.o server client
