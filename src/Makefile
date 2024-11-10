
all: # TODO: Create MS1_assg, MS2_assg, FS_assg. Delete all intermediate object files

MS1_assg: MS1

# Executables
MS1: assg.o spheres.o vector.o
	gcc -Wall -std=c99 assg.o spheres.o vector.o -o ../MS1 -lm
	rm -f assg.o spheres.o vector.o
	
MS2: # TODO ...

FS: # TODO ...


# Object files
# TODO: Makefile rules to compile source files into object files
assg.o: assg.c assg.h
	gcc -Wall -std=c99 -c assg.c

spheres.o: spheres.c spheres.h
	gcc -Wall -std=c99 -c spheres.c

vector.o: vector.c vector.h
	gcc -Wall -std=c99 -c vector.c
