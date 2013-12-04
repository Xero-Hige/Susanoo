/**
 centroide.cpp

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

#include "centroide.h"

#include <stddef.h>
#include <time.h>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <utility>

using std::map;
using std::vector;

Centroide::Centroide(int dimensiones, bool random) {
	vector<double> coordenadas;
	suma_acumulados_cuadrado = 0;

	for (int x = 0; x < dimensiones; x++) {
		double valor = 0;
		if (random) {
			valor = ( rand()%1000 ) / 1000.0;
		}

		suma_acumulados_cuadrado += (valor * valor);
		coordenadas.push_back(valor);
	}

	long double modulo = random ? sqrt(suma_acumulados_cuadrado) : 1;

	vectores_asociados = random ? 1 : 0;
	suma_acumulados_cuadrado = 0;

	if (random) {
		for (int x = 0; x < dimensiones; x++) {
			coordenadas[x] = (coordenadas[x] / modulo);
			promedios.push_back(coordenadas[x]);
			suma_acumulados_cuadrado += (promedios[x] * promedios[x]);
		}
	} else {
		promedios = coordenadas;
	}

	modulo = sqrtl (suma_acumulados_cuadrado);
}

double Centroide::calcular_coseno(map<int, double>& vector_reducido) {
	if (suma_acumulados_cuadrado == 0)
		return 0;

	double resultado = 0;
	for (map<int, double>::iterator it = vector_reducido.begin();
			it != vector_reducido.end(); ++it) {
		int coordenada = it->first;
		double valor = it->second;

		resultado += ((promedios[coordenada] / vectores_asociados) * valor);
	}

	double modulo = suma_acumulados_cuadrado
			* (1 / (vectores_asociados * vectores_asociados));
	modulo = sqrt(modulo);

	return (resultado / modulo);
}

double Centroide::calcular_coseno(Centroide& otro_centroide) {
	if (suma_acumulados_cuadrado == 0
			&& otro_centroide.suma_acumulados_cuadrado == 0)
		return 1;

	double resultado = 0;
	for (size_t i = 0; i < promedios.size(); i++) {
		double valor_propio = promedios[i];
		double valor_otro_centroide = otro_centroide.promedios[i];

		resultado += (valor_propio * valor_otro_centroide);
	}

	double mod_a = suma_acumulados_cuadrado /(vectores_asociados * vectores_asociados);
	mod_a = sqrt(mod_a);

	double mod_b = otro_centroide.suma_acumulados_cuadrado /(otro_centroide.vectores_asociados * otro_centroide.vectores_asociados);
	mod_b = sqrt(mod_b);

	return (resultado/(mod_a * mod_b));
}

void Centroide::agregar_vector(map<int, double>& vector_reducido) {
	for (map<int, double>::iterator it = vector_reducido.begin();
			it != vector_reducido.end(); ++it) {
		int coordenada = it->first;
		double valor = it->second;

		double anterior = promedios[coordenada];

		promedios[coordenada] += valor;

		double actual = promedios[coordenada];

		suma_acumulados_cuadrado -= (anterior * anterior);
		suma_acumulados_cuadrado += (actual * actual);
	}
	vectores_asociados++;
}

void Centroide::normalizar() {
	if (vectores_asociados == 0) return;

	double modulo = suma_acumulados_cuadrado
			* (1 / (vectores_asociados * vectores_asociados));
	modulo = sqrt(modulo);

	suma_acumulados_cuadrado = 0;

	for (size_t i = 0; i < promedios.size(); i++) {
		promedios[i] = promedios[i] / (vectores_asociados) * modulo;
		suma_acumulados_cuadrado += (promedios[i] * promedios[i]);
	}

	//suma_acumulados_cuadrado = 1;
	std::cout << "Acumulado cuadrado: " << suma_acumulados_cuadrado << std::endl;
	vectores_asociados = 1;
}
