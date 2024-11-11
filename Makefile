
all: # TODO: Create MS1_assg, MS2_assg, FS_assg. Delete all intermediate object files

MS1_assg: MS1

# Executables
MS1: assg.o spheres.o vector.o
	gcc -Wall -std=c99 assg.o spheres.o vector.o -o MS1 -lm
	rm -f assg.o spheres.o vector.o
	
MS2: # TODO ...

FS: # TODO ...

# Object files
# TODO: Makefile rules to compile source files into object files
assg.o: ./src/assg.c ./src/assg.h
	gcc -Wall -Werror -std=c99 -c ./src/assg.c

spheres.o: ./src/spheres.c ./src/spheres.h
	gcc -Wall -Werror -std=c99 -c ./src/spheres.c

vector.o: ./src/vector.c ./src/vector.h
	gcc -Wall -Werror -std=c99 -c ./src/vector.c
