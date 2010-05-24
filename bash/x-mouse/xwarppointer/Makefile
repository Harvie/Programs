X11HOME=/usr/X11

CC = gcc
CFLAGS= -Wall -I${X11HOME}/include
LIBS=-L${X11HOME}/lib -lX11

# uncomment for Solaris
#LIBS+=-lsocket -lnsl

xwarppointer: main.o
	$(CC) $(CFLAGS) -o xwarppointer main.o $(LIBS)

clean:
	rm -f main.o xwarppointer
