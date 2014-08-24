/**
 Susanoo.cpp

 Copyright 2013 Debora Elisa Martin <debbie1new.world@gmail.com>
 Gaston Alberto Martinez <Gaston.martinez.90@gmail.com>
 Matias Fernando Vazquez <mfvazquezfiuba@gmail.com>

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses>
 */

#include <getopt.h>
#include <stddef.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>

#include "clusterizador.h"
#include "vectorizador.h"

using std::string;
using std::vector;

void print_help() {
	printf(
			"Modo de uso: TpGrupoX [opciones] argumentos \n\n\
    *Opciones de construccion de grupos: \n\
        Opciones: --directorio|-d <path> --multi|-o <Y/N> [--categorias|-c] <cantidad>\n\
        [-d] Indica el path a donde están almacenados los documentos.\n\
        [-c] Indica la cantidad de categorias a crear, si no se indica se decide automaticamente.\n\
        [-o] Indica si un documento puede estar en mas de un grupo.\n\n\
    *Opciones de indexacion: \n\
        Opciones: []\n\
        [-l] Lista todos los documentos del repositorio y la categoría a la cual pertenece cada uno.\n\
        [-g] Lista los grupos o categorías existentes y los documentos dentro de cada grupo o categoría.\n\
        [-a] Agrega y clasifica el texto pasado como parametro e indica a que grupo lo ha agregado\n\
             (a partir de aqui debería aparecer al listad con -l o -g)\n\n");
}

void clusterizar(int n_clusters, const vector<string>& archivos,
		size_t dimensiones) {
	Clusterizador c = Clusterizador(n_clusters, CARPETA_VECTORES, archivos,
			dimensiones);
	c.hacer_clusters();
	c.guardarClusters("./Clusters");
}

void indexar(const string& directorio, int numero_clusters) {
	Vectorizador vectorizador = Vectorizador();
	size_t dimensiones = 0;
	vector<string> archivos = vectorizador.vectorizar(directorio, dimensiones);

	if (numero_clusters == 0) {
		numero_clusters = sqrt(archivos.size());
	}

	clusterizar(numero_clusters, archivos, dimensiones);
}

void agregar_archivo(const string& archivo) {
	Vectorizador vectorizador = Vectorizador();
	vectorizador.agregar_archivo(archivo);
  vector<string> archivos_centroides;
  vectorizador.obtener_archivos("./Centroides", archivos_centroides);
  // vector<Centroide> centroides;
  // cargar cada centroide
}

int main(int argc, char **argv) {
	srand(time(NULL)); //inicializo el random

	int c;

	int cantidad = 0;
	string directorio = "";
	string archivo_agregar = "";
	bool l = false;
	bool g = false;

	static struct option long_options[] = { { "directorio", 1, 0, 0 }, {
			"categorias", 1, 0, 0 }, { "multi", 1, 0, 0 },
			{ "agregar", 1, 0, 0 }, { "listar", 0, 0, 0 },
			{ "grupos", 0, 0, 0 }, { "help", 0, 0, 0 }, { NULL, 0, NULL, 0 } };

	int option_index = 0;

	while ((c = getopt_long(argc, argv, "d:c:o:a:lgh", long_options,
			&option_index)) != -1) {
		if (c == 0) {
			c = long_options[option_index].name[0];
		}

		switch (c) {

		case 'a':
			archivo_agregar = string(optarg);
			break;

		case 'c':
			cantidad = atoi(optarg);
			break;

		case 'd':
			directorio = string(optarg);
			break;

		case 'g':
			g = true;
			break;

		case 'l':
			l = true;
			break;

		case 'm':
		case 'o':
			break;

		case 'h':
			print_help();
			break;

		case '?':
			break;

		default:
			printf("?? getopt returned character code 0%o ??\n", c);
		}
	}
	if (optind < argc) {
		printf("non-option ARGV-elements: ");
		while (optind < argc)
			printf("%s ", argv[optind++]);
		printf("\n");
	}

	if (directorio != "") {
		indexar(directorio, cantidad);
	} else if (archivo_agregar != "") {
		agregar_archivo(archivo_agregar);
	} else if (l) {

	} else if (g) {

	} else {
		printf("No hay opcion especificada");
	}

	return 0;
}
