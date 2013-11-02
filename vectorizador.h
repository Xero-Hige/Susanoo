/**
vectorizador.h

Copyright 2013 Debora Elisa Martin <debbie1new.world@gmail.com>
               Gaston Alberto Martinez <Gaston.martinez.90@gmail.com>
               Matias Fernando Vazquez <mfvazquezfiuba@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses>
*/

#ifndef VECTORIZADOR_H
#define VECTORIZADOR_H
#include <string>
#include <vector>

#define CARPETA_VECTORES "./temp_vects"
#define EXTENSION_VECTORES  ".vec"

std::vector<std::string> vectorizar (const std::string& directorio);

#endif
