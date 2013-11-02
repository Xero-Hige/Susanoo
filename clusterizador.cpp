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

#include <fstream>
#include <stdlib.h>


using std::vector;
using std::string;
using std::ifstream;

#define BUFFSIZE 200

Clusterizador::Clusterizador(int n_clusters, const string& carpeta_vectores) {

	centroides_viejos = new vector<double> [n_clusters]();
	centroides_nuevos = new vector<double> [n_clusters]();
	clusters_viejos = new vector<string> [n_clusters]();
	clusters_nuevos = new vector<string> [n_clusters]();

	clusters = n_clusters;
	carpeta_origen = carpeta_vectores;
	this->archivos = archivos;
}

Clusterizador::~Clusterizador() {
	delete[] centroides_viejos;
	delete[] centroides_nuevos;
	delete[] clusters_viejos;
	delete[] clusters_nuevos;
}

void Clusterizador::inicializar()
{
	char* buff = new char[BUFFSIZE]();

	for (int i = 0;i<clusters;i++)
	{
		ifstream vector;
		string path = carpeta_origen + "/" +archivos[i];
		vector.open(path.c_str());
		while (vector.good())
		{
			vector.getline(buff,BUFFSIZE-1);
			double numero = atof(buff);
			centroides_viejos[i]->push_back(numero);
		}
		vector.close();
	}
}

void Clusterizador::hacer_clusters(){
	inicializar();
	//TODO: cambiar por la distancia
	for (int i=0,i<4)
	{

	}
}
