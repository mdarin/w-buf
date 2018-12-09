a: wbuf.o main.o 
	gcc --std=gnu99 wbuf.o main.o -o a
main.o: main.c
	gcc --std=gnu99 -c main.c
sllist.o: wbuf.c wbuf.h
	gcc --std=gnu99 -c wbuf.c
clean:
	rm -f a c main.o wbuf.o
install:
	cp a /usr/local/bin/a
uninstall: 
	rm -f /usr/local/bin/a
