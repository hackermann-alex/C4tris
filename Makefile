OBJ = main.o render.o game.o
DEST = game
INCS = -lSDL2
CFLAGS = -std=c99 -Wall -O3 -pedantic
CC = cc

all: ${OBJ}
	${CC} ${INCS} -o ${DEST} ${OBJ}

main.o: main.c game.h render.h
	${CC} ${CFLAGS} -c -o $@ $<

render.o: render.c game.h render.h
	${CC} ${CFLAGS} -c -o $@ $<

game.o: game.c game.h
	${CC} ${CFLAGS} -c -o $@ $<

clean:
	rm *.o
