/**
<archivo>

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
#include <map>          // std::map
#include <vector>       // std::vector
#include <string>       // std::string
#include <iostream>     // std::cout|cerr|endl
#include <fstream>      // std::ifstream|ofstream

#include <algorithm>    // std::sort

#include "vectorizador.h"

using std::string;
using std::vector;
using std::map;

using std::sort;

using std::cout;
using std::cerr;
using std::endl;

using std::ifstream;
using std::ofstream;

void a_minuscula(string& cadena)
{
    transform(cadena.begin(), cadena.end(), cadena.begin(), ::tolower);
}

bool es_caracter(char caracter)
{
    return (('A'<=caracter and caracter<='Z') or
            ('a'<=caracter and caracter<='z') );
}
//map<string,int> palabras_prohibidas

void imprimir_archivos(vector<string>& archivos)
{
    for (unsigned int i = 0;i < archivos.size();i++) {
        cout << archivos[i] << endl;
    }
}

void contar (string archivo,map<string,vector<int> >& palabras_archivos,int i)
{
     ifstream arch;
     arch.open(archivo.c_str());
     map<string,int> palabras = map<string,int>();
     while (arch.good())
     {
        char c = arch.get();
        string res = string("");

        while (arch.good() and es_caracter(c))
        {
            res += c;
            c = arch.get();
        }

        if (res.size()<3) continue;//funcion es palabra valida
                                  //que valida eso y saca palabras
                                 //chotas tipo if in on out at etc.

        a_minuscula(res);

        int cant = palabras.count(res);
        if (cant == 0)
        {
            palabras[res] = 1;
            continue;
        }
        palabras[res] = palabras[res] + 1;

        cant = palabras_archivos.count(res);
        if (cant == 0)
        {
            palabras_archivos[res] = vector<int>();
            palabras_archivos[res].push_back(1);
            palabras_archivos[res].push_back(i);
            continue;
        }
        else
        {
            if (palabras_archivos[res][1]>=i) continue;
            palabras_archivos[res][0] = palabras_archivos[res][0] + 1;
            palabras_archivos[res][1] = i;
        }
     }

     ofstream out;
     out.open((archivo+".res").c_str());
     for (map<string,int>::iterator it=palabras.begin();it!=palabras.end();++it)
     {
        out << it->first << "=" << it->second << endl;
     }
     out.close();
}

void generar_bases(vector<string>& archivos)
{
    map<string, vector<int> > palabras_archivos = map<string,vector<int> >();

    for (unsigned int i = 0;i < archivos.size();i++) {
        contar(archivos[i],palabras_archivos,i);
    }

     ofstream out;
     out.open("./Total.res");
     for (map<string,vector<int> >::iterator it=palabras_archivos.begin();it!=palabras_archivos.end();++it)
     {
        if (it->second[0] < 0.1* archivos.size()) continue;
        out << it->first << "=" << it->second[0] << endl;
     }
     out.close();
}

void obtener_archivos(string directorio,vector<string>& archivos)
{
    DIR *dp;
    struct dirent *dirp;

    if((dp  = opendir(directorio.c_str())) == NULL) {
        cerr << "Error abriendo " << directorio << endl;
    }

    while ((dirp = readdir(dp)) != NULL) {
        archivos.push_back(string(directorio+"/"+dirp->d_name));
    }
    closedir(dp);
}

void vectorizar (string directorio)
{
    vector<string> archivos = vector<string>();
    map<string,int> palabras_archivos = map<string,int>();

    obtener_archivos(directorio,archivos);

    sort(archivos.begin(),archivos.end());

    generar_bases(archivos);
}
