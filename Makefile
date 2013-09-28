# Makefile de ejemplo para C++
#
# Creado: jue abr 15 15:34:19 ART 2004
#
# Copyleft 2004 - Leandro Lucarella, Bajo licencia GPL [http://www.gnu.org/]
#
#
# Modificado: sab sep 28 18:19:25 ART 2013
#
# Copyleft 2013 - Gaston Alberto Martinez, Bajo licencia GPL [http://www.gnu.org/]
#

# CONFIGURACION
################

# Nombre del ejecutable.
target = tp

# Extensión de los archivos a compilar (c para C, cpp o cc o cxx para C++).
extension = cpp

# Argumentos para pasarle al programa cuando se ejecuta con Valgrind
arguments =

# Archivos con el código fuente que componen el ejecutable. Si no se especifica,
# toma todos los archivos con la extensión mencionada. Para especificar hay que
# descomentar la línea (quitarle el '#' del principio).
# NOTA: No poner cabeceras (.h).
#fuentes = entrada.cpp

# Si usa funciones de math.h, descomentá (quitale el '#' a) la siguiente línea.
#math = si

# Si usa pthreads, descomentá (quitale el '#' a) la siguiente línea.
#pthreads = si

# Si usa gthreads, descomentá (quitale el '#' a) la siguiente línea.
#gthreads = si

# Si es un programa GTK+, descomentá (quitale el '#' a) la siguiente línea.
#gtk = si

# Si es un programa gtkmm, descomentá (quitale el '#' a) la siguiente línea.
#gtkmm = si

# Descomentar si se quiere ver como se invoca al compilador
#verbose = si


# CONFIGURACION "AVANZADA"
###########################

# Opciones para el compilador C/C++ en modo ansi.
CFLAGS = -Wall -pedantic -pedantic-errors

# Para que explote lo mas posible
EXPLODE = -O3
CFLAGS += $(EXPLODE)
CFLAGS +=  -DNDEBUG

# Para valgrind o debug
CFLAGS += -ggdb -DDEBUG -fno-inline

# Opciones del enlazador.
#LDFLAGS =

# Estandar de C a usar
CSTD = c99

# Estandar de C++ a usar
CXXSTD = c++98

# Si se quiere compilar estaticamente, descomentar la siguiente linea
#static = si

#Opciones para Valgrind
VFLAGS = --leak-check=full --track-origins=yes --track-fds=yes --show-reachable=yes -v


# VARIABLES CALCULADAS A PARTIR DE LA CONFIGURACION
####################################################

# Agrego flags y libs de GTK+ de ser necesario.
ifdef gtk
CFLAGS += $(shell pkg-config --cflags gtk+-2.0)
LDFLAGS += $(shell pkg-config --libs gtk+-2.0)
endif

# Agrego flags y libs de GTK+ de ser necesario.
ifdef gtkmm
CFLAGS += $(shell pkg-config --cflags gtkmm-2.4)
LDFLAGS += $(shell pkg-config --libs gtkmm-2.4)
endif

# Agrego flags y libs de gthreads de ser necesario.
ifdef gthreads
CFLAGS += $(shell pkg-config --cflags gthread-2.0)
LDFLAGS += $(shell pkg-config --libs gthread-2.0)
endif

# Linkeo con libm de ser necesario.
ifdef math
LDFLAGS += -lm
endif

# Linkeo con pthreads de ser necesario.
ifdef pthreads
LDFLAGS += -lpthread -pthread
endif

ifdef static
LDFLAGS += -static
endif

# Pongo flags de C para C++ también
CXXFLAGS += $(CFLAGS)

# Uso enlazador de c++ si es código no C.
ifeq ($(extension), c)
CFLAGS += -std=$(CSTD)
LD = $(CC)
else
CXXFLAGS += -std=$(CXXSTD)
LD = $(CXX)
endif

# Si no especifica archivos, tomo todos.
fuentes ?= $(wildcard *.$(extension))

occ := $(CC)
ocxx := $(CXX)
orm := $(RM)
old := $(LD)
ifdef verbose
RM := $(RM) -v
else
CC =  @echo "  CC  $@"; $(occ)
CXX = @echo "  CXX $@"; $(ocxx)
RM =  @echo "  CLEAN"; $(orm)
LD =  @echo "  LD  $@"; $(old)
endif


# REGLAS
#########

.PHONY: all clean

all: $(target)

o_files = $(patsubst %.$(extension),%.o,$(fuentes))

%.deps: %.$(extension)
	@set -e; rm -f $@; \
		$(old) -MM -MG -MP $(CXXFLAGS) $(CFLAGS) $< -MF $@.$$$$; \
		sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
		rm -f $@.$$$$

-include $(o_files:.o=.deps)

$(target): $(o_files)
	@if [ -z "$(o_files)" ]; \
	then \
		echo "No hay archivos de entrada, recuerde que la extensión debe ser '.$(extension)'."; \
		false; \
	fi >&2
	$(LD) $(LDFLAGS) $(o_files) $(LOADLIBS) $(LDLIBS) -o $(target)

valgrind: all
	valgrind $(VFLAGS) ./$(target) $(arguments)

test: all
	python CorrerPruebas.py

cpplint:
	python ./cpplint.py --filter=`cat filter_options` `find -regextype posix-egrep -regex '.*\.(h|hpp|c|cpp)'`

clean:
	$(RM) $(o_files) $(o_files:.o=.deps) $(target) *~ *.save *.deps

PADRON=XXXXX
ARCHIVOS=*.h *.c #Tipos de archivos que se van a agregar
CUATRIMESTRE=2013-2C
ENTREGA=Pila
ENCODING=ISO-8859-1 
OUTPUTFILE=out.ps #archivo intermedio
FORMATO=landscape #portrait o landscape (vertical u horizontal)
COLUMNAS=2 #paginas por hoja (en columnas)
NUMEROS_LINEA=1 #cada cuantas lineas se imprime el numero de linea
ARCHIVO_ENTREGA=$(PADRON)-$(ENTREGA)
NOMBRE_ZIP= $(ARCHIVO_ENTREGA).zip
NOMBRE_PDF= $(ARCHIVO_ENTREGA).pdf 
ENCABEZADO = "[75.41] Algoritmos y Programacion II"
PIE = "Padrón $(PADRON) (curso $(CUATRIMESTRE)) Entrega: $(ENTREGA)"


pdf: clean
	a2ps $(ARCHIVOS) -Av --header=$(ENCABEZADO) --footer=$(PIE) --line-numbers=$(NUMEROS_LINEA) --borders=yes --columns=$(COLUMNAS)  --$(FORMATO) --output=$(OUTPUTFILE) --encoding=$(ENCODING) --tabsize=4 --major=columns --toc  | ps2pdf $(OUTPUTFILE) $(NOMBRE_PDF)
	rm *.ps #elimino los archivos temporales

entrega: pdf
	zip $(NOMBRE_ZIP) $(ARCHIVOS) *.pdf

clean_entrega: clean
	rm *.zip *.pdf
