all: MS1_assg MS2_assg FS_assg

MS1_assg: MS1

MS2_assg: MS2

FS_assg: FS

# Executables
MS1: assg.o spheres.o vector.o color.o
	gcc -std=c99 assg.o spheres.o vector.o color.o -o MS1_assg -lm

MS2: assg.o spheres.o vector.o color.o
	gcc -std=c99 assg.o spheres.o vector.o color.o -o MS2_assg -lm

FS: assg.o spheres.o vector.o color.o
	gcc -std=c99 assg.o spheres.o vector.o color.o -o FS_assg -lm
	rm -f *.o rm -f *.o assg.o spheres.o vector.o color.o

# Object files
assg.o: ./src/assg.c ./src/assg.h
	gcc -Wall -std=c99 -c ./src/assg.c

spheres.o: ./src/spheres.c ./src/spheres.h
	gcc -Wall -std=c99 -c ./src/spheres.c

vector.o: ./src/vector.c ./src/vector.h
	gcc -Wall -std=c99 -c ./src/vector.c

color.o: ./src/color.c ./src/color.h
	gcc -Wall -std=c99 -c ./src/color.c