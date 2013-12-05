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
//#include <time.h>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <utility>
#include <fstream>
#include <sstream>
#include <string.h>

using std::map;
using std::vector;
using std::string;

#define ASOCIADOS_STR "Asociados"
#define ASOCIADOS_EXT ".txt"
#define CENTROIDE_STR "Centroide"
#define CENTROIDE_EXT ".vec"

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
//  suma_acumulados_cuadrado -= (anterior * anterior);
//	double suma_acumulados_cuadrado = 0; //+= (actual * actual);

	for (map<int, double>::iterator it = vector_reducido.begin();
			it != vector_reducido.end(); ++it) {

		int coordenada = it->first;
		double valor = it->second;
//    std::cout << "coordenada = " << coordenada << "                       valor = " << valor << std::endl;
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
	if (vectores_asociados == 0 || vectores_asociados == 1)
		return;

	modulo = 0;
	for (size_t i = 0; i < acumulados.size(); i++) {
		double valor = acumulados[i] / vectores_asociados;
		acumulados[i] = valor;
		modulo += (valor*valor);
	}

	modulo = sqrt(modulo);
	vectores_asociados = 1;
}


Centroide::Centroide(const std::string& archivo){
  std::ifstream arch_centroide;
  arch_centroide.open(archivo.c_str(), std::ios::binary);

  char buffer[BUFFSIZE];
  
  arch_centroide.getline(buffer, BUFFSIZE - 1);
  memcpy(&vectores_asociados, buffer, sizeof(double));
  arch_centroide.getline(buffer, BUFFSIZE - 1);
  memcpy(&modulo, buffer, sizeof(double));  

  while (arch_centroide.good()){
    arch_centroide.getline(buffer, BUFFSIZE - 1);
    double peso;
    memcpy(&peso, buffer, sizeof(double));
    acumulados.push_back(peso);
  }
  arch_centroide.close();
}




void Centroide::guardar(const string& ruta_carp_centroide, int nro_centroide) {
    normalizar();

    std::ofstream arch_centroide;
    std::stringstream ss;
    ss << nro_centroide;
    string ruta_centroide = ruta_carp_centroide;
    ruta_centroide += "/";
    ruta_centroide += CENTROIDE_STR + ss.str();
    ruta_centroide += CENTROIDE_EXT;
    arch_centroide.open(ruta_centroide.c_str(),  std::ios::out | std::ios::binary);


    arch_centroide << vectores_asociados << "-" << modulo;
    arch_centroide.write((char*) &vectores_asociados, sizeof(double));
    arch_centroide.write((char*) &modulo, sizeof(double));

	for (size_t i = 0; i < acumulados.size(); i++) {
		arch_centroide << acumulados[i] << std::endl;
		arch_centroide.write((char*) &acumulados[i], sizeof(double));
	}

	arch_centroide.close();
}

void Centroide::guardarAsociados(const string& ruta_carp_centroide, int nro_centroide, std::vector<std::string> cluster) {
    std::ofstream arch_asociados;
    std::stringstream ss;
    ss << nro_centroide;
    string ruta_asociados = ruta_carp_centroide;
    ruta_asociados += "/";
    ruta_asociados += ASOCIADOS_STR + ss.str();
    ruta_asociados += ASOCIADOS_EXT;
    arch_asociados.open(ruta_asociados.c_str(), std::ios::binary);

	for (size_t i = 0; i < cluster.size(); i++) {
		arch_asociados << cluster[i] << std::endl;
	}

	arch_asociados.close();
}
