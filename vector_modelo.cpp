/**
 vectormodelo.cpp

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
#include "vector_modelo.h"

using std::string;
using std::vector;
using std::ofstream;
using std::map;
using std::endl;

Vector_Modelo::Vector_Modelo(vector<string>& coordenadas) {
	for (size_t i = 0; i < coordenadas.size(); i++) {
		_coordenadas[coordenadas[i]] = 0;
	}

}

void Vector_Modelo::guardar_en_archivo(ofstream& salida) {
	for (map<string, int>::iterator it = _coordenadas.begin();
			it != _coordenadas.end(); ++it) {
		salida << _coordenadas[it->first] << endl;
	}
}

void Vector_Modelo::set_coordenada(string coordenada, int valor) {
	//Si no existe en el vector como coordenada no lo agrega
	if (_coordenadas.count(coordenada) == 0)
		return;
	_coordenadas[coordenada] = valor;
}
