#m!/bin/bash

set -e    # Para que retorne el script si algo falla

# Este script es para asistir a la compilacion y ejecucion del sistems

# Creamos directorios de buildeo si no existen ya
mkdir -p bin


# ----- COMPILACION -------

if [ "$1" = "client" ]
then
	# Corremos la regla para buildear el cliente
	echo "\e[32mCompilando cliente...\e[0m"
	make client;
	# Ejecutamos el cliente
	echo "\e[32mEjecutando cliente...\e[0m"
	./bin/client 127.0.0.1 5000;
elif [ "$1" = "backend" ]
then
	echo "\e[32mLimpiando proyecto...\e[0m"
	make clean;
	# Corremos la regla para buildear el backend
	echo "\e[32mCompilando servidor...\e[0m"
	# Hacemos clean para asegurarnos que no estamos usando archivos desactualizados
	make server;
	make auth;
	make fileserv;
	# Ejecutamos el backend
	echo "\e[32mEjecutando backend...\e[0m"
	./bin/auth 5001 &
	./bin/server 5000 127.0.0.1 5001
else
	echo "\e[32mOpcion no reconocida: use 'client' o 'backend'\e[0m"
fi
