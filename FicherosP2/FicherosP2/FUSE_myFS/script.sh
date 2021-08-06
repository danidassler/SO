#!/bin/bash

#creamos un directorio temporal:
echo "Creando directorio temporal"
rm -rf temp # por si acaso ya existia
mkdir temp

#copiamos fuselib.c y myfs a temp y a mount
echo "Copiando ficheros"
cp src/fuseLib.c mount/fuseLib.c
cp src/fuseLib.c temp/fuseLib.c

cp src/myFS.h mount/myFS.h
cp src/myFS.h temp/myFS.h

#mostramos diferencias
echo "Mostrando diferencias del fichero fuseLib.c"
if diff mount/fuseLib.c temp/fuseLib.c
then echo "Los ficheros son iguales"
else echo "Los ficheros son diferentes"
fi

#modificamos tamaño de fiselib en ambos lugares
echo "modificando el tamaño de fuseLib en mount y temp"
truncate mount/fuseLib.c -s -3
truncate temp/fuseLib.c -s -3
echo "Mostrando diferencias del fichero fuseLib.c"
if diff mount/fuseLib.c temp/fuseLib.c
then echo "Los ficheros son iguales"
else echo "Los ficheros son diferentes"
fi

#copiamos un tercer fichero de texto
echo "Copiamos fichero1.txt"
cp fichero1.txt mount/fichero1.txt

echo "Mostramos diferencias del fichero1"
if diff mount/fichero1.txt fichero1.txt
then echo "Los ficheros son iguales"
else echo "Los ficheros son diferentes"
fi

#modificamos tamaño de myFS en temp y mount
echo "modificando el tamaño myFS en mount y temp"
truncate mount/myFS.h -s +3
truncate temp/myFS.h -s +3
echo "Mostrando diferencias del fichero fuseLib.c"
if diff mount/myFS.h temp/myFS.h
then echo "Los ficheros son iguales"
else echo "Los ficheros son diferentes"
fi

echo "Creando carpeta con mkdir"
ls -la mount






