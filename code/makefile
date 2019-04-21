all: server_core client

server_core: server_core.c server_core.h hashmap.c hashmap.h
	gcc -O0 server_core.c hashmap.c -o server_core -lpthread

# thread.o: thread.c
# 	gcc thread_test.c -o thread_test -lpthread

client: client.c server_core.h
	gcc client.c -o client

# server: server.c server_core.h
# 	gcc server.c -o server
