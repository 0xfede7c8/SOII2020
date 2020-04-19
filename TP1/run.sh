#m!/bin/bash

set -e    # Para que retorne el script si algo falla

# Este script es para asistir a la compilacion y ejecucion del sistems

# Create build directories if not already built
mkdir -p build/client
mkdir -p build/server
mkdir -p build/obj

# ----- COMPILACION -------
cd src

# Hacemos clean para asegurarnos que no estamos usando archivos desactualizados
echo "\e[32mLimpiando proyecto...\e[0m"
make clean;

if [ "$1" = "client" ]
then
	# Corremos la regla para buildear el cliente
	echo "\e[32mCompilando cliente...\e[0m"
	make usb_writer;
	cd ..;
	# Ejecutamos el cliente
	echo "\e[32mEjecutando cliente...\e[0m"
	./build/client/usb_writer 127.0.0.1 5000;
elif [ "$1" = "backend" ]
then
	# Corremos la regla para buildear el backend
	echo "\e[32mCompilando servidor...\e[0m"
	make usb_writer_backend;
	cd ..;
	# Ejecutamos el backend
	echo "\e[32mEjecutando backend...\e[0m"
	./build/server/usb_writer_backend 5000
else
	echo "\e[32mOpcion no reconocida: use 'client' o 'backend'\e[0m"
fi
