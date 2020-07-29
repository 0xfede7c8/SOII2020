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
	# Matamos procesos viejos
	echo "\e[32mMatando procesos viejos...\e[0m" 
	kill -9 $(pidof ./bin/auth) 2> /dev/null || true
	kill -9 $(pidof ./bin/server) 2> /dev/null || true
	kill -9 $(pidof ./bin/fileserv) 2> /dev/null || true
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
	./bin/fileserv 5002 &
	./bin/server 5000 127.0.0.1 5001 5002
else
	echo "\e[32mOpcion no reconocida: use 'client' o 'backend'\e[0m"
fi
