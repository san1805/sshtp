all: compile

compile:
	gcc -o netserver netserver.c

clean:
	rm netserver
