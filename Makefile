all: teapot

teapot: main.o
	gcc -o teapot main.o
	
main.o: main.c
	gcc -c main.c

clean:
	rm teapot main.o
