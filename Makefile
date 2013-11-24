CC = gcc
CFLAGS = -Wall -Wextra -g -pedantic
CPPFLAGS = -Wall -Wextra -g -pedantic -lstdc++ -lm

OBJECTS = Porter.o vectorizador.o vector_modelo.o medidor_distancia.o clusterizador.o

all: run

Porter.o: Porter.c Porter.h
	$(CC) $(CFLAGS) -c Porter.c
	
vector_modelo.o: vector_modelo.cpp vector_modelo.h
	$(CC) $(CPPFLAGS) -c vector_modelo.cpp

vectorizador.o: vectorizador.cpp vectorizador.h Porter.o vector_modelo.o
	$(CC) $(CPPFLAGS) -c vectorizador.cpp

medidor_distancia.o: medidor_distancia.cpp medidor_distancia.h
	$(CC) $(CPPFLAGS) -c medidor_distancia.cpp

clusterizador.o: clusterizador.cpp clusterizador.h
	$(CC) $(CPPFLAGS) -c clusterizador.cpp
	
Susanoo: Susanoo.cpp $(OBJECTS)
	$(CC) $(CPPFLAGS) $(OBJECTS) Susanoo.cpp -o Susanoo

clean:
	rm -r  *.o *.save *~ Susanoo temp temp_vects

run: Susanoo
	./Susanoo -d ./Pruebas
