/**
 Centroide.h

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
#ifndef CENTROIDE_H_
#define CENTROIDE_H_

#include <map>
#include <vector>

class Centroide {

private:
	/**
	 * Vector que almacena la suma de los vectores asociados
	 * el valor de cada coordenada del vector real es igual
	 * al valor de su coordenada en este vector dividido por
	 * la cantidad de vectores asociados.
	 */
	std::vector<double> promedios;

	/**
	 * Almacena la suma de cada uno de los valores del vector
	 * promedios al cuadrado
	 */
	double suma_acumulados_cuadrado;

	/**
	 * Cantidad de vectores asociados
	 */
	int vectores_asociados;

	Centroide(){};

public:
	Centroide(int dimensiones, bool random = true);

	/**
	 * Calcula el coseno contra un vector representado por un map
	 * (el vector esta normalizado)
	 */
	double calcular_coseno(std::map<int, double> vector_reducido);

	/**
	 * Calcula el coseno contra otro centroide
	 * (ambos centroides deben estar normalizados)
	 */
	double calcular_coseno(Centroide otro_centroide);

	void agregar_vector(std::map<int, double> vector_reducido);
	void normalizar();
};

#endif /* CENTROIDE_H_ */
