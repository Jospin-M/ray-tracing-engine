
all: # TODO: Create MS1_assg, MS2_assg, FS_assg. Delete all intermediate object files

MS1_assg: MS1

MS2_assg: MS2

# Executables
MS1: assg.o spheres.o vector.o
	gcc -std=c99 assg.o spheres.o vector.o -o MS1 -lm
	rm -f assg.o spheres.o vector.o
	
MS2: assg.o spheres.o vector.o color.o
	gcc -std=c99 assg.o spheres.o vector.o color.o -o MS2 -lm
	rm -f assg.o spheres.o vector.o color.o

FS: # TODO ...

# Object files
# TODO: Makefile rules to compile source files into object files
assg.o: ./src/assg.c ./src/assg.h
	gcc -Wall -std=c99 -c ./src/assg.c

spheres.o: ./src/spheres.c ./src/spheres.h
	gcc -Wall -std=c99 -c ./src/spheres.c

vector.o: ./src/vector.c ./src/vector.h
	gcc -Wall -std=c99 -c ./src/vector.c

color.o: ./src/color.c ./src/color.h
	gcc -Wall -std=c99 -c ./src/color.c
