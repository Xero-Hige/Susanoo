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

#include <iostream>
#include <map>
#include <string>
#include <vector>

//#include "vector_modelo.h"

#define CARPETA_VECTORES "./temp_vects"
#define EXTENSION_VECTORES  ".vec"


typedef struct _coordenada {
	int coordenada;
	double peso;
} coordenada_t;

class Vectorizador {

private:
	std::string _directorio;
	std::vector<std::string> archivos;

	std::map<std::string, std::vector<int> > palabras_archivos;
	// clave: palabra, valor [ cantidad total de documentos en los que aparece , marca para saber si ya se leyo ]
	// DF es el primer subindice
	std::map<std::string, std::string> reduccion_palabras; // porter

	std::map<std::string, int> coordenadas_vector;
	// palabra y devuelve coordenada del vector, cantidad de claves es M

private:
	void contar(const std::string& directorio, const std::string& archivo,
			int numero_archivo);

	void generar_bases(const std::string& directorio,
			const std::vector<std::string>& archivos);

	void obtener_archivos(std::string directorio,
			std::vector<std::string>& archivos);

	/**
	 * Genera una carpeta en el sistema para agrupar
	 * diferentes archivos intermedios
	 */
	void generar_carpeta(const std::string& path_carpeta);

	void contar_palabras(std::ifstream& arch,
			std::map<std::string, int>& palabras);

	void generar_vectores(const std::vector<std::string>& archivos,
			std::map<std::string, std::vector<int> > palabras_archivos);

	void generar_vector(const std::string & archivo);

	void reducir_palabras(std::map<std::string, int>& palabras,
			std::map<std::string, int>& palabras_reducidas);
	void agregar_stopwords();

	// almacena el contenido del pesos_vector en el archivo abierto vect.
	void guardar_vector(double modulo, std::vector<coordenada_t> &pesos_vector,
			std::ofstream &vect);
  
  //
  void separar_del_path(const std::string &arch_completo,
                          std::string &path,
                          std::string &archivo);

  void almacenar_df();
  
  void cargar_df();

public:

	Vectorizador();

	std::vector<std::string> vectorizar(const std::string& directorio, size_t& dimensiones);

  //
  void agregar_archivo(const std::string &path_archivo);

};
#endif
