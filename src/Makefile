CCFLAG+= -std=c++11
OBJ = snake.o board.o game.o
INC = -I../include

snake.o:
	g++ -c ${INC} snake.cc $(CCFLAG)
board.o:
	g++ -c ${INC} board.cc $(CCFLAG)
game.o:
	g++ -c ${INC} -DHIDE_CURSOR game.cc $(CCFLAG)

all: ${OBJ}
	g++ -o snake ${INC} play.cc ${OBJ} $(CCFLAG) -lpthread
clean:
	rm *.o
