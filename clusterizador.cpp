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
#include <fstream>
#include <iostream>
#include <map>

using std::vector;
using std::string;
using std::map;
using std::ifstream;
using std::cout;
using std::endl;
using std::ios;

#define BUFFSIZE 200

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

void Clusterizador::cargar_vector(map<int, float> coordenadas, string archivo) {
	string path_archivo = carpeta_origen + "/" + archivo;
	ifstream arch(path_archivo.c_str(), ios::in | ios::binary);

	while (arch.good()) {
		char buff[10];

		arch.get(buff, 4);
		int coordenada = atoi(buff);

		arch.get(buff, 4);
		float valor = atof(buff);

		coordenadas[coordenada] = valor;
	}
}

void Clusterizador::hacer_clusters() {
	cout << "Inicializando K-Means" << endl;

	//TODO: cambiar por la distancia
	for (int i = 0; i < 20; i++) {
		clusters_viejos.clear();
		clusters_viejos.resize(clusters);

		for (int n_archivo = 0; n_archivo < archivos.size(); n_archivo++) {
			map<int, float> coordenadas = map<int, float>();

			cargar_vector(coordenadas, archivos[n_archivo]);

			float minimo_coseno = 100;
			int centroide = 0;

			for (int i = 0; i < centroides_viejos.size(); i++) {
				float coseno = centroides_viejos[i].calcular_coseno(
						coordenadas);

				//TODO: Modificar para tolerancia
				if (coseno < minimo_coseno) {
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

				map<int, float> coordenadas = map<int, float>();

				cargar_vector(coordenadas, clusters_viejos[centroide][archivo]);

				nuevo_centroide.agregar_vector(coordenadas);
			}
			nuevo_centroide.normalizar();
			centroides_viejos.push_back(nuevo_centroide);
		}
	}

	cout << "Cluster 0:" << endl;

	for (int archivo = 0; archivo < clusters_viejos[0].size(); archivo++) {
		cout << clusters_viejos[0][archivo] << endl;
	}
}
