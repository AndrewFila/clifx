CC			= c++
FLAGS		=
CFLAGS		= -g -O2 -Wno-write-strings -Wno-cpp -w -lm
OBJ_FILE	= cliApp.o clifx.o
EXE_FILE	= clifx

${EXE_FILE}: ${OBJ_FILE}
	${CC} ${FLAGS} -o ${EXE_FILE} ${OBJ_FILE}

cliApp.o: cliApp.cpp
	${CC} ${CFLAGS} -c cliApp.cpp

clifx.o: clifx.cpp
	${CC} ${CFLAGS} -c clifx.cpp

clean:
	rm -f ${OBJ_FILE} ${EXE_FILE}
