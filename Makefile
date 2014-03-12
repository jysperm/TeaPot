all: coffeepot

coffeepot: main.o
	gcc -o coffeepot main.o
	
main.o: main.c
	gcc -c main.c

clean:
	rm coffeepot main.o
