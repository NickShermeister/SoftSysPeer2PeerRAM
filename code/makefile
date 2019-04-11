all: server_core

server_core: server_core.c server_core.h hashmap.c hashmap.h
	gcc server_core.c hashmap.c -o server_core -lpthread

# thread.o: thread.c
# 	gcc thread_test.c -o thread_test -lpthread
