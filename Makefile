CC = gcc
CFLAGS = -g -Wall -ansi -pedantic
OBJS = lexTester.o
INCL = lexical.h nextInputChar.h tokenStack.h doOperator.h

all:	rpnCalc liblexical.a

rpnCalc:	rpnCalc.o doOperator.o liblexical.a
	$(CC)  -o $@ $^ -L. -llexical

%.o:	 %.c $(INCL)
	$(CC) $(CFLAGS) -c -o $@ $< 

liblexical.a:	lexical.o nextInputChar.o tokenStack.o
	ar rcs liblexical.a lexical.o nextInputChar.o tokenStack.o

clean:
	rm $(OBJS) lex.a
