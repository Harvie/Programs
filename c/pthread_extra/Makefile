COPT=-g -O2 -flto -ftree-vectorize -ftree-slp-vectorize -fvisibility=hidden -D_FORTIFY_SOURCE=2 -DMG_ENABLE_FS=0 -fstack-protector-strong -fno-delete-null-pointer-checks $(CARCH)
#COPT=-g -fvisibility=hidden -DMG_ENABLE_FS=0 -fstack-protector-strong $(CARCH)
CDEF=-D_POSIX_C_SOURCE=200809L -D_XOPEN_SOURCE=700 -D_DEFAULT_SOURCE=1 -D_GNU_SOURCE
CERR=-Wall -Wextra -Werror -pedantic -Werror=date-time -Wformat-security -Wduplicated-cond -Wfloat-equal -Wshadow -Wlogical-not-parentheses -Wnull-dereference

CLIB:= -pthread -I .
LIB:= -pthread

CFLAGS+=$(COPT) $(CERR) -Wjump-misses-init $(CDEF) -std=c11 $(CLIB)
CXXFLAGS+=$(COPT) $(CERR) $(CDEF) -std=c++11 $(CLIB)
LDFLAGS+=$(COPT) $(LIB)

BIN=test
OBJ=pthread_user_data.o pthread_pause.o pthread_multi.o pthread_msgqueue.o test.o

all: $(BIN)
$(BIN): $(OBJ)
	$(CXX) -o $(BIN) $(OBJ) $(LDFLAGS)
	strip --strip-unneeded $(BIN)

	gcc -lpthread -I . test_pause.c pthread_user_data.o pthread_pause.o -o test_pause

clean:
	rm -f $(BIN) $(OBJ) $(DEPS)

DEPS:=$(OBJ:.o=.d)
-include $(DEPS)

.c.o:
	$(CC) -c $(CFLAGS) -MD -o $@ $<

.cpp.o:
	$(CXX) -c $(CXXFLAGS) -MD -o $@ $<

