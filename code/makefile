all: server_core request donate

server_core: server_core.c server_core.h hashmap.c hashmap.h
	gcc -O0 server_core.c hashmap.c -o server_core -lpthread

# thread.o: thread.c
# 	gcc thread_test.c -o thread_test -lpthread

request: request.c server_core.h
	gcc request.c -o request

donate: donate.c server_core.h hashmap.c
	gcc donate.c hashmap.c -o donate
# server: server.c server_core.h
# 	gcc server.c -o server
