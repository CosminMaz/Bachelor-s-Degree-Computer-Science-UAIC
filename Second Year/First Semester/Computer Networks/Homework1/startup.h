#!/bin/bash
rm client
rm server
rm clientServerFifo
gcc -Wall client.c -o client
gcc -Wall server.c -o server

./server &
./client
