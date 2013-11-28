/**
 clusterizador.cpp

 Copyright 2013 Gaston Martinez Gaston.martinez.90@gmail.com

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses
 */

#include "clusterizador.h"

#include <stddef.h>
//#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
//#include <map>

using std::vector;
using std::string;
using std::map;
using std::ifstream;
using std::cout;
using std::endl;
using std::ios;

#define BUFFSIZE 200

#define VECTOR_STR "vector"
#define VECTOR_EXT ".res"
#define ALPHA 0.000

Clusterizador::Clusterizador(int n_clusters, const string& carpeta_vectores,
		const std::vector<std::string>& archivos, int dimensiones) {
	cout << "Inicializacion" << endl;

	for (int i = 0; i < n_clusters; i++) {
		cout << "Inicializando centroide: " << i << endl;

		centroides_viejos.push_back(Centroide(dimensiones, true));
	}

	clusters = n_clusters;
	carpeta_origen = carpeta_vectores;
	this->archivos = archivos;
	this->dimensiones = dimensiones;

	cout << "Fin inicializacion" << endl;
}

Clusterizador::~Clusterizador() {
}

void Clusterizador::cargar_vector(map<int, double>& coordenadas,
		string archivo) {
	string path_archivo = carpeta_origen + "/" + archivo + ".vec";
	ifstream arch(path_archivo.c_str(), ios::in | ios::binary);

	int coordenada = 1;
	double valor = 1;
	// std::cout << "ARCHIVO = " << archivo << std::endl;
	int pos = arch.tellg();
	if (arch.get() != EOF) {
		arch.seekg(pos);
	}
	while (arch.good()) {
		char buff_a[sizeof(int)];
		char buff_b[sizeof(double)];

		for (size_t i = 0; i < sizeof(int); i++) {
			buff_a[i] = arch.get();
		}
		memcpy(&coordenada, buff_a, sizeof(int));

		for (size_t i = 0; i < sizeof(double); i++) {
			buff_b[i] = arch.get();
		}
		memcpy(&valor, buff_b, sizeof(double));

		//cout << coordenada << "---" << valor << endl;

		coordenadas[coordenada] = valor;

		int pos = arch.tellg();
		if (arch.get() != EOF) {
			arch.seekg(pos);
		}
	}
}

void Clusterizador::hacer_clusters() {
	cout << "Inicializando K-Means" << endl;

	//TODO: cambiar por la distancia
	for (int i = 0; i < 300; i++) {
		clusters_viejos.clear();
		clusters_viejos.resize(clusters);

		for (size_t n_archivo = 0; n_archivo < archivos.size(); n_archivo++) {
			map<int, double> coordenadas = map<int, double>();

			cargar_vector(coordenadas, archivos[n_archivo]);

			float maximo_coseno = 0;
			int centroide = 0;

			for (size_t i = 0; i < centroides_viejos.size(); i++) {
				float coseno = centroides_viejos[i].calcular_coseno(
						coordenadas);

				//TODO: Modificar para tolerancia
				if (coseno > maximo_coseno) {
					maximo_coseno = coseno;
					centroide = i;
				}
			}

			centroides_viejos[centroide].agregar_vector(coordenadas);
			clusters_viejos[centroide].push_back(archivos[n_archivo]);
		}
		//TODO: cambiar los centroides para recalcular la distancia entre ellos al principio del for

		centroides_viejos.clear();
		for (int centroide = 0; centroide < clusters; centroide++) {
			Centroide nuevo_centroide = Centroide(dimensiones, false);
			for (int archivo = 0; archivo < clusters_viejos[centroide].size();
					archivo++) {

				map<int, double> coordenadas = map<int, double>();

				cargar_vector(coordenadas, clusters_viejos[centroide][archivo]);

				nuevo_centroide.agregar_vector(coordenadas);
			}
			nuevo_centroide.normalizar();
			centroides_viejos.push_back(nuevo_centroide);
		}
		for (int i = 0; i < clusters; i++) {
			centroides_viejos[i].normalizar();
		}
	}

	for (int i = 0; i < clusters; i++) {
		cout << "Cluster " << i << ":" << endl;

		for (size_t archivo = 0; archivo < clusters_viejos[i].size();
				archivo++) {
			cout << clusters_viejos[i][archivo] << endl;
		}
	}
}

void Clusterizador::crearCarpeta(const string& path_carpeta) {
	string comando_carpeta_temporal = "mkdir ";
	comando_carpeta_temporal += path_carpeta;
	system(comando_carpeta_temporal.c_str());
}

void Clusterizador::guardarClusters(const string& ruta_carp_cluster) {
	crearCarpeta(ruta_carp_cluster);

	int nro_vector = 0;
	for (vector<vector<string>>::iterator it_todos = clusters_viejos.begin();
			it_todos != clusters_viejos.end(); ++it_todos) {
		std::stringstream ss;
		ss << nro_vector++;
		string ruta_cluster = ruta_carp_cluster;
		ruta_cluster += "/";
		ruta_cluster += VECTOR_STR + ss.str();
		ruta_cluster += VECTOR_EXT;

		std::ofstream arch_cluster;
		arch_cluster.open(ruta_cluster.c_str(), std::ios::binary);
		for (vector<string>::iterator it_cluster = it_todos->begin();
				it_cluster != it_todos->end(); ++it_cluster)
			arch_cluster << *it_cluster << std::endl;
		arch_cluster.close();
	}
}
