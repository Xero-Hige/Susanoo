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

	modulo = 0;

	for (int x = 0; x < dimensiones; x++) {
		double valor = 0;
		if (random) {
			valor = (rand() % 1000);
		}
		coordenadas.push_back(valor);

		modulo += (valor * valor);
	}

	modulo = sqrt(modulo);

	vectores_asociados = random ? 1 : 0;

	double suma_acumulados_cuadrado = 0;

	if (random) {
		for (int x = 0; x < dimensiones; x++) {
			coordenadas[x] = (coordenadas[x] / modulo);
			acumulados.push_back(coordenadas[x]);
			suma_acumulados_cuadrado += (coordenadas[x] * coordenadas[x]);
		}
	} else {
		acumulados = coordenadas;
	}
//
//	modulo = sqrt(suma_acumulados_cuadrado);
}

double Centroide::calcular_coseno(map<int, double>& vector_reducido) {

	double resultado = 0, modulo_vector = 0;
	for (map<int, double>::iterator it = vector_reducido.begin();
			it != vector_reducido.end(); ++it) {
		int coordenada = it->first;
		double valor = it->second;

		resultado += ((acumulados[coordenada] / vectores_asociados) * valor);
		modulo_vector += valor * valor;
	}
    modulo_vector = sqrt(modulo_vector);
	//std::cout << "Coseno V: " << resultado / (modulo * modulo_vector) << std::endl;

	return (resultado / (modulo * modulo_vector));
}

double Centroide::calcular_coseno(Centroide& otro_centroide) {
	double resultado = 0;
	for (size_t i = 0; i < acumulados.size(); i++) {
		double valor_propio = acumulados[i] / vectores_asociados;
		double valor_otro_centroide = otro_centroide.acumulados[i]
				/ otro_centroide.vectores_asociados;

		resultado += (valor_propio * valor_otro_centroide);
	}

	double mod_a = modulo;
	double mod_b = otro_centroide.modulo;

//	std::cout << "Coseno C: " <<(resultado / (mod_a * mod_b)) << std::endl;

	return (resultado / (mod_a * mod_b));
}

void Centroide::agregar_vector(map<int, double>& vector_reducido) {
//	std::cout << "init" << std::endl;
	//suma_acumulados_cuadrado -= (anterior * anterior);
//	double suma_acumulados_cuadrado = 0; //+= (actual * actual);

	for (map<int, double>::iterator it = vector_reducido.begin();
			it != vector_reducido.end(); ++it) {

		int coordenada = it->first;
		double valor = it->second;
    std::cout << "coordenada = " << coordenada << "                       valor = " << valor << std::endl;
		acumulados[coordenada] += valor;
	}
	vectores_asociados++;

	modulo = 0;
	for (size_t i = 0; i < acumulados.size(); i++) {
		double agregar = acumulados[i] / vectores_asociados;
        agregar = (agregar * agregar);
        modulo += agregar;
	}

	modulo = sqrt(modulo);
}

void Centroide::normalizar() {
	if (vectores_asociados == 0)
		return;

	double suma_acumulado_cuadrados = 0;

	for (size_t i = 0; i < acumulados.size(); i++) {
		double valor = acumulados[i] / modulo;
		acumulados[i] = valor;

		suma_acumulado_cuadrados += (valor*valor);
	}

	modulo = sqrt(suma_acumulado_cuadrados);
	if (modulo != 1)
	{
		std::cout << "Bad Module: " << modulo << std::endl;
	}
	vectores_asociados = 1;
}
