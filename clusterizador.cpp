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

	centroides_viejos = new vector<double> [n_clusters]();
	centroides_nuevos = new vector<double> [n_clusters]();
	clusters_viejos = new vector<string> [n_clusters]();
	clusters_nuevos = new vector<string> [n_clusters]();

	clusters = n_clusters;
	carpeta_origen = carpeta_vectores;
	this->archivos = archivos;
	this->dimensiones = dimensiones;
}

Clusterizador::~Clusterizador() {
	delete[] centroides_viejos;
	delete[] centroides_nuevos;
	delete[] clusters_viejos;
	delete[] clusters_nuevos;
}

void Clusterizador::inicializar() {

	for (int i = 0; i < clusters; i++) {
		for (int x = 0; x < dimensiones; x++) {
			centroides_viejos[i].push_back((rand() % 1000) / 1000.0);
		}
	}
}

void Clusterizador::hacer_clusters() {
	cout << "Inicializando" << endl;
	inicializar();

	//TODO: cambiar por la distancia
	for (int i = 0; i < 4; i++) {
		for (int n_archivo = 0; n_archivo < archivos.size(); n_archivo++){
			map<int,float> coordenadas = map<int,float>();

			string path_archivo = carpeta_origen + "/" + archivos[n_archivo];
			ifstream arch (path_archivo.c_str(),ios::in | ios::binary);

			while (arch.good())
			{
				char buff[10];

				arch.get(buff,4);
				int coordenada = atoi(buff);

				arch.get(buff,4);
				float valor = atof(buff);

				coordenadas[coordenada] = valor;
			}
		}
	}
}
