CC			= c++
FLAGS		=
CFLAGS		= -g -O2 -Wno-write-strings -Wno-cpp -w -lm
OBJ_FILE	= main.o clifx.o
EXE_FILE	= lifxlan

${EXE_FILE}: ${OBJ_FILE}
	${CC} ${FLAGS} -o ${EXE_FILE} ${OBJ_FILE}

main.o: main.cpp
	${CC} ${CFLAGS} -c main.cpp

clifx.o: clifx.cpp
	${CC} ${CFLAGS} -c clifx.cpp

clean:
	rm -f ${OBJ_FILE} ${EXE_FILE}
