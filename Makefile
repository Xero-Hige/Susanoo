CC = gcc
CFLAGS = -Wall -Wextra -g -pedantic
CPPFLAGS = -Wall -Wextra -g -pedantic -lstdc++ -lm

OBJECTS = Porter.o vectorizador.o

all: Susanoo

Porter.o: Porter.c Porter.h
	$(CC) $(CFLAGS) -c Porter.c

vectorizador.o: vectorizador.cpp vectorizador.h Porter.o
	$(CC) $(CPPFLAGS) -c vectorizador.cpp
	
Susanoo: Susanoo.cpp $(OBJECTS)
	$(CC) $(CPPFLAGS) $(OBJECTS) Susanoo.cpp -o Susanoo
	
clean:
	rm *.o *.save *~ Susanoo
	rm -r temp