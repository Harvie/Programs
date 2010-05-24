#CFLAGS = -g -Wall -pedantic -ansi $$(pkg-config --cflags glib-2.0)
#LDFLAGS = -lncursesw $$(pkg-config --libs glib-2.0)
CFLAGS = -O2 -Wall $$(pkg-config --cflags glib-2.0)
LDFLAGS = -s -lncursesw $$(pkg-config --libs glib-2.0)
PREFIX = /usr/local
SOURCES = fm.c fs.c ui.c cmd.c
HEADERS = fm.h
OBJECTS = ${SOURCES:.c=.o}
BINARY = fm

#all: tags ${BINARY}
all: ${BINARY}

${BINARY}: ${OBJECTS}
	${CC} ${LDFLAGS} -o ${BINARY} ${OBJECTS}

${OBJECTS}: ${HEADERS}

tags: ${SOURCES} ${HEADERS}
	ctags ${SOURCES} ${HEADERS}

install: all
	install -D -m 755 -o root -g root ${BINARY} ${PREFIX}/bin/${BINARY}

uninstall:
	rm -f ${PREFIX}/bin/${BINARY}

clean:
	rm -f fm *.o tags
