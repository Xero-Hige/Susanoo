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

#include <cstdlib>
#include <stddef.h>
#include <cstring>
#include <fstream>
#include <iostream>

using std::vector;
using std::string;
using std::map;
using std::ifstream;
using std::cout;
using std::endl;
using std::ios;

#define BUFFSIZE 200

#define CLUSTER_STR "cluster"
#define CLUSTER_EXT ".res"
#define CENTROIDE_PATH "./Centroide"

Clusterizador::Clusterizador(int n_clusters, const string& carpeta_vectores,
		const std::vector<std::string>& archivos, int dimensiones) {
	cout << "Inicializacion" << endl << endl;

	clusters = n_clusters;
	carpeta_origen = carpeta_vectores;
	this->archivos = archivos;
	this->dimensiones = dimensiones;

	for (int i = 0; i < n_clusters; i++) {
		cout << "Inicializando centroide: " << i << endl;

		map<int, double> coordenadas = map<int, double>();
		cargar_vector(coordenadas, archivos[rand() % archivos.size()]);
		Centroide cent = Centroide(dimensiones, false);
		cent.agregar_vector(coordenadas);

		centroides_nuevos.push_back(cent);
	}

	cout  << endl << "Fin inicializacion" << endl;
}

Clusterizador::~Clusterizador() {
}

void Clusterizador::cargar_vector(map<int, double>& coordenadas,
		string archivo) {
	string path_archivo = carpeta_origen + "/" + archivo + ".vec";
	ifstream arch(path_archivo.c_str(), ios::in | ios::binary);

	int coordenada = 1;
	double valor = 1;

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

		coordenadas[coordenada] = valor;

		int pos = arch.tellg();
		if (arch.get() != EOF) {
			arch.seekg(pos);
		}
	}
}

void Clusterizador::hacer_clusters() {
	cout << "Inicializando K-Means" << endl;

	size_t iteracion = 0;

	double distancia_maxima = 0;
	while (distancia_maxima < 0.99 && iteracion < 20) {

		centroides_viejos = centroides_nuevos;

#ifdef _DEBUG
		cout << "Inicio teracion: " << ++iteracion << " con "
				<< distancia_maxima << endl;
#endif

		distancia_maxima = 1.0 / 0.0;
		;

		clusters_nuevos.clear();
		clusters_nuevos.resize(dimensiones);

		for (size_t n_archivo = 0; n_archivo < archivos.size(); n_archivo++) {
			map<int, double> coordenadas = map<int, double>();

			cargar_vector(coordenadas, archivos[n_archivo]);

			double minimo_coseno = 0;
			int centroide = 0;

			for (size_t i = 0; i < centroides_nuevos.size(); i++) {
				double coseno = centroides_nuevos[i].calcular_coseno(
						coordenadas);

				//TODO: Modificar para tolerancia
				if (coseno >= minimo_coseno) {
					minimo_coseno = coseno;
					centroide = i;
				}
			}
			centroides_nuevos[centroide].agregar_vector(coordenadas);
			clusters_nuevos[centroide].push_back(archivos[n_archivo]);
		}

		for (int i = 0; i < clusters; i++) {

			Centroide& viejo = centroides_viejos[i];
			Centroide& nuevo = centroides_nuevos[i];

			double coseno = nuevo.calcular_coseno(viejo);

			if (coseno <= distancia_maxima) {
			//	cout << "Maximo centroide n:" << i << endl;
				distancia_maxima = coseno;
			}

			centroides_viejos[i].normalizar();
			centroides_nuevos[i].normalizar(); //En realidad esto no normaliza
		}
//#ifdef _DEBUG
//		cout << "Finaliza iteracion: " << iteracion << " con "
//				<< distancia_maxima << endl;
//#endif
	}
	crearCarpeta(CENTROIDE_PATH);
    for(int i = 0; i < clusters; i++) {
        centroides_nuevos[i].guardar(CENTROIDE_PATH, i);
        for (size_t archivo = 0; archivo < clusters_nuevos[i].size(); archivo++) {
            centroides_nuevos[i].guardarAsociados(CENTROIDE_PATH, i, clusters_nuevos[i]);
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

	for (int i = 0; i < clusters; i++) {
		std::stringstream ss;
		ss << i;
		string ruta_cluster = ruta_carp_cluster;
		ruta_cluster += "/";
		ruta_cluster += CLUSTER_STR + ss.str();
		ruta_cluster += CLUSTER_EXT;

		std::ofstream arch_cluster;
		arch_cluster.open(ruta_cluster.c_str(), std::ios::binary);

		for (size_t archivo = 0; archivo < clusters_nuevos[i].size();
				archivo++) {
			arch_cluster << clusters_nuevos[i][archivo] << std::endl;
		}
		arch_cluster.close();
	}
}
