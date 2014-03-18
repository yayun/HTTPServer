cc=gcc
objects=web_server.o
server:$(objects)
	gcc -o server $(objects)
$(objects):web_server.c
	gcc -c web_server.c
clean:
	rm -f web_server.o
