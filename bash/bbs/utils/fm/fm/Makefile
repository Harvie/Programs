# Comment next 2 line to disable Linux inotify support.
INOTIFY = -DINOTIFY
INOTIFY_SOURCE = ino.c
CFLAGS = -Wall -pedantic -ansi ${INOTIFY} $$(pkg-config --cflags glib-2.0) -g #-O2
LDFLAGS = -lncursesw $$(pkg-config --libs glib-2.0) #-s
PREFIX = /usr/local
SOURCES = fm.c fs.c ui.c cmd.c ${INOTIFY_SOURCE}
HEADERS = fm.h
OBJECTS = ${SOURCES:.c=.o}
BINARY = fm

all: ${BINARY}

${BINARY}: ${OBJECTS}
	${CC} ${LDFLAGS} -o ${BINARY} ${OBJECTS}

${OBJECTS}: ${HEADERS}

tags: ${SOURCES} ${HEADERS}
	ctags ${SOURCES} ${HEADERS}

install: all
	install -D -m 755 -o root -g root ${BINARY} ${DESTDIR}/${PREFIX}/bin/${BINARY}

uninstall:
	rm -f ${PREFIX}/bin/${BINARY}

clean:
	rm -f ${BINARY} *.o tags
