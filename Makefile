CC = gcc
CFLAGS = -Wall -Wextra -g -pedantic
CPPFLAGS = -Wall -Wextra -g -pedantic -lstdc++ -lm -std=c++11 -O3

OBJECTS = vectorizador.o vector_modelo.o medidor_distancia.o clusterizador.o centroide.o

all: Susanoo

Snowball: Snowball/Makefile
	cd ./Snowball	
	make
	cd ..
	
vector_modelo.o: vector_modelo.cpp vector_modelo.h
	$(CC) $(CPPFLAGS) -c vector_modelo.cpp

vectorizador.o: vectorizador.cpp vectorizador.h Snowball vector_modelo.o
	$(CC) $(CPPFLAGS) -c vectorizador.cpp

medidor_distancia.o: medidor_distancia.cpp medidor_distancia.h
	$(CC) $(CPPFLAGS) -c medidor_distancia.cpp

clusterizador.o: clusterizador.cpp clusterizador.h
	$(CC) $(CPPFLAGS) -c clusterizador.cpp
	
centroide.o: centroide.cpp centroide.h
	$(CC) $(CPPFLAGS) -c centroide.cpp
	
Susanoo: Susanoo.cpp Snowball $(OBJECTS)
	$(CC) $(CPPFLAGS) $(OBJECTS) Susanoo.cpp -o Susanoo

clean:
	rm -r *.o *.save *~ Susanoo temp temp_vects Centroide Clusters

clean-o:
	rm -r *.o *~

clean-temp:
	rm -r temp temp_vects 

run: Susanoo
	./Susanoo -d ./HardTest
