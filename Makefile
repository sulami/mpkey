NAME = mpkey

PREFIX ?= /usr/local
BINDIR ?= ${PREFIX}/bin

INCS = -I. -I${PREFIX}/include
LIBS = -lc `pkg-config --libs xcb-keysyms libmpdclient`

CFLAGS += -std=c99 -pedantic -Wall -Wextra
LDFLAGS += ${LIBS}

EXEC = ${NAME}
DEPS = config.h
SRC = ${NAME}.c
OBJ = ${SRC:.c=.o}

.c.o:
	@echo CC $<
	@${CC} -c ${CFLAGS} $<

${OBJ}: $(DEPS)

${NAME}: ${OBJ}
	@echo CC -o $@
	@${CC} -o $@ ${OBJ} ${LDFLAGS}

