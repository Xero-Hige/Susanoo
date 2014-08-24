/**
 medidor_distancia.h

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
#ifndef MEDIDOR_DISTANCIA_H_
#define MEDIDOR_DISTANCIA_H_

#include <math.h>
#include <stdlib.h>

#include <vector>

/*
 *
 */
class Medidor_Distancia {

private:
	static double producto_interno(const std::vector<int>& vect_1,
			const std::vector<int>& vect_2);

public:
	static double medir_distancia(const std::vector<int>& vect_1,
			const std::vector<int>& vect_2);
};

#endif /* MEDIDOR_DISTANCIA_H_ */
