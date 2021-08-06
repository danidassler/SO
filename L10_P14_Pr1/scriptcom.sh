#!/bin/bash
if [ -x mytar ]; #comprueba si mytar esta en el direcorio actual y si es ejecutable
then
	if [ -d tmp/ ]; #comprueba si existe directorio tmp dentro del directorio actual y si existe lo borra
	then
		rm -rf tmp/
		
	fi

	mkdir tmp #crea directorio tmp
	cd tmp #entra en tmp
	touch file1.txt #crea fichero file1.txt
	echo "Hello world!" > file1.txt #ecribe hola mundo en file1,txt
	touch file2.txt #crea fichero file1.txt
	head /etc/passwd > file2.txt #copia de las 10 primeras líneas del fichero
#/etc/passwd se hace fácil utilizando el programa head y redirigiendo la entrada
	touch file3.dat 
	head -c 1024 /dev/urandom > file3.dat

	../mytar -cf filetar.mtar file1.txt file2.txt file3.dat #invocamos el programa mytar

	mkdir out
	cp filetar.mtar out/ #copiamos el fichero filetar.mtar en el directorio out
	cd out

    ../../mytar -xf filetar.mtar #extraemos el filetar.mtar en el directorio out

# comprobamos si los ficheros son diferentes con diff
	if  diff file1.txt ../file1.txt ;
	then
		
			if diff file2.txt ../file2.txt;
		then
			

				if diff file3.dat ../file3.dat;
				then
					cd ../..
					echo "Correct"
					exit 0
					else 
						cd ../..
						echo "El fichero file3.dat es distinto a file3.dat extraido"
						exit 1
				fi
		else 
			
		cd ../..
		echo "El fichero file2.txt es distinto a file2.txt extraido"
		exit 1
	fi
	else 
		cd ../..
		echo "El fichero file1.txt es distinto a file1.txt extraido"
		exit 1
	fi

	




else
	echo "NO se ha encontrado el mytar."
	exit 1
fi
