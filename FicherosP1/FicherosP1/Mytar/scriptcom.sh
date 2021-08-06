#!/bin/bash
if [ -x mytar ];
then
	if [ -d tmp/ ];
	then
		rm -rf tmp/
		
	fi

	mkdir tmp
	cd tmp
	touch file1.txt
	echo "Hello world!" > file1.txt
	touch file2.txt
	head /etc/passwd > file2.txt
	touch file3.dat
	head -c 1024 /dev/urandom > file3.dat

	../mytar -cf filetar.mtar file1.txt file2.txt file3.dat

	mkdir out
	cp filetar.mtar out/
	cd out

    ../../mytar -xf filetar.mtar

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
