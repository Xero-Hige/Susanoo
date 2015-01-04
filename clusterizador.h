/**
 clusterizador.h

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
#ifndef CLUSTERIZADOR_H_
#define CLUSTERIZADOR_H_

#include <map>
#include <string>
#include <vector>

#include "centroide.h"

class Clusterizador {

	std::vector<Centroide> centroides_viejos;
	std::vector<Centroide> centroides_nuevos;

	//TODO: ver
	std::vector<std::vector<std::string> > clusters_viejos;
	std::vector<std::vector<std::string> > clusters_nuevos;

	int clusters;
	int dimensiones;

	std::string carpeta_origen;
	std::vector<std::string> archivos;

private:
	void cargar_vector(std::map<int, double>& coordenadas, std::string archivo);
	void crearCarpeta(const std::string& path_carpeta);

public:
	Clusterizador(int n_clusters, const std::string& carpeta_vectores,
			const std::vector<std::string>& archivos, int dimensiones);
	virtual ~Clusterizador();
	void hacer_clusters();
	void guardarClusters(const std::string& ruta_carp_cluster);
	void cargarClusters(const std::string& ruta_carp_cluster);
	void agregar_archivo(const std::string archivo);
};

#endif /* CLUSTERIZADOR_H_ */
