#!/bin/bash

# Este script es para asistir a la compilacion y ejecucion del sistems

# Create build directories if not already built
mkdir -p build/client
mkdir -p build/server
mkdir -p build/obj

# ----- COMPILACION -------
cd src
# Hacemos clean para asegurarnos que no estamos usando archivos desactualizados
make clean
# Corremos la regla para buildear el cliente
make usb_writer
# Corremos la regla para buildear el backend
make usb_writer_backend

# ----- EJECUCION ---------
cd ..
# Ejecutamos el backend
./build/server/usb_writer_backend 5000 &
sleep 1    # Esperamos un segundo a que arranque
# Ejecutamos el cliente
./build/client/usb_writer 127.0.0.1 5000
