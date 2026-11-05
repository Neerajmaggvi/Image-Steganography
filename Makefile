CC = gcc

OBJ = $(patsubst %.c, %.o, $(wildcard *.c))

stego.exe : $(OBJ)
	gcc -o $@ $^
	
clean: 
	rm -f *.exe *.o