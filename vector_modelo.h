/**
 vectormodelo.h

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
#ifndef VECTORMODELO_H_
#define VECTORMODELO_H_

#include <map>
#include <vector>
#include <string>
#include <fstream>

class Vector_Modelo {
private:
	std::map<std::string, int> _coordenadas;

public:
	Vector_Modelo(std::vector<std::string>& coordenadas);
	void guardar_en_archivo(std::ofstream& salida);
	void set_coordenada(std::string coordenada, int valor);
};

#endif /* VECTORMODELO_H_ */
