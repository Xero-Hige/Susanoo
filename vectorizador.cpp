/**
 vectorizador.cpp

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

#include <sys/types.h>
#include <dirent.h>
#include <string.h>

#include <map>          // std::map
#include <vector>       // std::vector
#include <string>       // std::string
#include <iostream>     // std::cout|cerr|endl
#include <fstream>      // std::ifstream|ofstream

#include <algorithm>    // std::sort

#include "vectorizador.h"
#include "vector_modelo.h"
#include "Porter.h"

using std::string;
using std::vector;
using std::map;

using std::sort;

using std::cout;
using std::cerr;
using std::endl;

using std::ifstream;
using std::ofstream;

#define CARPETA_TEMPORAL "./temp"
#define EXTENSION_BASES  ".res"

#define BUFFSIZE 200

void a_minuscula(string& cadena) {
	transform(cadena.begin(), cadena.end(), cadena.begin(), ::tolower);
}

bool es_caracter(char caracter) {
	return (('A' <= caracter and caracter <= 'Z')
			or ('a' <= caracter and caracter <= 'z'));
}

void imprimir_archivos(vector<string>& archivos) {
	for (unsigned int i = 0; i < archivos.size(); i++) {
		cout << archivos[i] << endl;
	}
}

void contar(const string& directorio, const string& archivo,
		map<string, vector<int> >& palabras_archivos, int numero_archivo) {
	ifstream arch;

	string path_archivo = directorio + "/";
	path_archivo += archivo;

	arch.open(path_archivo.c_str());
	map<string, int> palabras = map<string, int>();
	while (arch.good()) {
		char c = arch.get();
		string res = string("");

		while (arch.good() and es_caracter(c)) {
			res += c;
			c = arch.get();
		}

		if (res.size() < 3)
			continue; //funcion es palabra valida
		//que valida eso y saca palabras
		//chotas tipo if in on out at etc.

		a_minuscula(res);

		int cant = palabras.count(res);
		if (cant == 0) {
			palabras[res] = 1;
			continue;
		}
		palabras[res] = palabras[res] + 1;
	}

	map<string, int> palabras_reducidas = map<string, int>();

	for (map<string, int>::iterator it = palabras.begin(); it != palabras.end();
			++it) {
		char* palabra = (char*) it->first.c_str();
		stemmer_t* z = create_stemmer();
		stemword(z, palabra, it->first.size());

		string res = string(palabra);

		int cant = palabras_reducidas.count(res);
		//TODO VER
		if (cant == 0) {
			palabras_reducidas[res] = it->second;
			continue;
		}
		palabras[res] = palabras[res] + it->second;

		cant = palabras_archivos.count(res);
		if (cant == 0) {
			palabras_archivos[res] = vector<int>();
			palabras_archivos[res].push_back(1);
			palabras_archivos[res].push_back(numero_archivo);
			continue;
		} else {
			if (palabras_archivos[res][1] >= numero_archivo)
				continue;
			palabras_archivos[res][0] = palabras_archivos[res][0] + 1;
			palabras_archivos[res][1] = numero_archivo;
		}

	}

	ofstream out;
	string path = CARPETA_TEMPORAL;
	path += "/" + archivo + EXTENSION_BASES;
	out.open(path.c_str());
	for (map<string, int>::iterator it = palabras_reducidas.begin();
			it != palabras_reducidas.end(); ++it) {
		out << it->first << "=" << it->second << endl;
	}
	out.close();
}

map<string, vector<int> > generar_bases(const string& directorio,
		const vector<string>& archivos) {
	map<string, vector<int> > palabras_archivos = map<string, vector<int> >();

	for (unsigned int i = 0; i < archivos.size(); i++) {
		contar(directorio, archivos[i], palabras_archivos, i);
	}

	ofstream out;

	string path_salida = CARPETA_TEMPORAL;
	path_salida += "/Total.trs";

	out.open(path_salida.c_str());

	for (map<string, vector<int> >::iterator it = palabras_archivos.begin();
			it != palabras_archivos.end(); ++it) {
		if (it->second[0] == 1)
			continue;
		out << it->first << "=" << it->second[0] << endl;
	}
	out.close();
	return palabras_archivos;
}

void obtener_archivos(string directorio, vector<string>& archivos) {
	DIR *dp;
	struct dirent *dirp;

	if ((dp = opendir(directorio.c_str())) == NULL) {
		cerr << "Error abriendo " << directorio << endl;
	}

	while ((dirp = readdir(dp)) != NULL) {
		if (dirp->d_name[0] == '.')
			continue;
		archivos.push_back(string(dirp->d_name));
	}
	closedir(dp);
}

/**
 * Genera una carpeta en el sistema para agrupar
 * diferentes archivos intermedios
 */
void generar_carpeta(const string& path_carpeta) {
	string comando_carpeta_temporal = "mkdir ";
	comando_carpeta_temporal += path_carpeta;
	system(comando_carpeta_temporal.c_str());
}

void generar_vector(const string& archivo, Vector_Modelo modelo)
{
	string path_base = CARPETA_TEMPORAL;
	path_base += "/"+archivo + EXTENSION_BASES;

	ifstream arch;
	arch.open(path_base.c_str());

	char buffer[BUFFSIZE];

	while (arch.good()) {
		arch.getline(buffer,BUFFSIZE-1);
		string datos = string(buffer);

		size_t separador = datos.find("=");
		string clave = datos.substr(0,separador);
		string valor = datos.substr(separador+1);
		modelo.set_coordenada(clave,atoi(valor.c_str()));
	}

	string path_vector = CARPETA_VECTORES;
	path_vector += "/"+ archivo + EXTENSION_VECTORES;

	ofstream vect;
	vect.open(path_vector.c_str());

	modelo.guardar_en_archivo(vect);
}

/**
 * Genera los archivos de vectores que se van a utilizar
 */
void generar_vectores(const vector<string>& archivos,
		map<string, vector<int> > palabras_archivos) {

	vector<string> coordenadas;
	vector<int> valores;

	for (map<string, vector<int> >::iterator it = palabras_archivos.begin();
			it != palabras_archivos.end(); ++it)
	{
		if (it->second[0] < 2) continue;
		coordenadas.push_back(it->first);
		//TODO: el map esta mal formado y hay que sacar de esta manera el
		//valor
		valores.push_back(it->second[0]);
	}

	Vector_Modelo modelo = Vector_Modelo(coordenadas);

	for (size_t i = 0; i < archivos.size(); i++) {
		//TODO: hacer de a mas y con threads
		generar_vector(archivos[i], modelo);
	}
}

/**
 * 	Genera los archivos de vectores a utilizar
 */

void vectorizar(const string& directorio) {
	vector<string> archivos = vector<string>();

	obtener_archivos(directorio, archivos);
	sort(archivos.begin(), archivos.end());

	generar_carpeta(CARPETA_TEMPORAL);//TODO: eliminar los datos innecesarios del vector
	map<string, vector<int> > palabras_archivos = generar_bases(directorio, archivos);

	generar_carpeta(CARPETA_VECTORES);
	generar_vectores(archivos,palabras_archivos);
}
