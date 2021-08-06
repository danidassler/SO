#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mytar.h"

extern char *use;

/** Copy nBytes bytes from the origin file to the destination file.
 *
 * origin: pointer to the FILE descriptor associated with the origin file
 * destination:  pointer to the FILE descriptor associated with the destination file
 * nBytes: number of bytes to copy
 *
 * Returns the number of bytes actually copied or -1 if an error occured.
 */
int copynFile(FILE * origin, FILE * destination, int nBytes)
{

   int c;
   int nLeidos = 0; // contador de leidos
	while ((c = getc(origin))!= EOF && nLeidos < nBytes ){ //hasta que no llegue al final del fichero de origen o el numero de bytes leidos sea igual que el numero de Bytes a leer no se sale del bucle
		putc((unsigned char)c,destination); //coloca el byte leido en el fichero destino
		nLeidos++; //aumentamos el numero de leidos
	}
	return nLeidos; // devolvemos el numero de leidos

}

/** Loads a string from a file.
 *
 * file: pointer to the FILE descriptor 
 * 
 * The loadstr() function must allocate memory from the heap to store 
 * the contents of the string read from the FILE. 
 * Once the string has been properly built in memory, the function returns
 * the starting address of the string (pointer returned by malloc()) 
 * 
 * Returns: !=NULL if success, NULL if error
 */
char*
loadstr(FILE * file)//leer nombre de ficheros dentro del mtar
{
	int c, nLeidos = 0;
   char* cadena;
   char f;

   if(file==NULL) { 
   	
   	return NULL;}
   else{
        fread(&f, sizeof(char),1,file); // leemos un dato del tamaño de un char  desde file y lo guardamos en f que habiamos declarado anteriormente
	    nLeidos++; // aumentamos el numero de leidos
	    while(f!='\0'){ // miestras que f sea diferente de un espacio o salto de linea?? repetimos el paso anterior con el codigo de abajo
            fread(&f, sizeof(char),1,file); 
	        nLeidos++;
        }
        fseek(file,-nLeidos,SEEK_CUR); //desplazamos el puntero nLeidos bytes (esta en negativo porque queremos desplazar el puntero ese numero de bytes pero hacia la izquierda) desde la posicion actual donde se encuentra el puntero (SEEK_CUR)
       }

   cadena=malloc(sizeof(char)*nLeidos); // se reserva memoria en el heap con el tamaño exacto ( nLeidos anteriormente * tamaño que ocupa un char )
	if(cadena == NULL)return NULL;

	fread(cadena, sizeof(char),nLeidos,file);
	
    return cadena; //devolvemos la cadena leida

   }
// Complete the function


/** Read tarball header and store it in memory.
 *
 * tarFile: pointer to the tarball's FILE descriptor 
 * nFiles: output parameter. Used to return the number 
 * of files stored in the tarball archive (first 4 bytes of the header).
 *
 * On success it returns the starting memory address of an array that stores 
 * the (name,size) pairs read from the tar file. Upon failure, the function returns NULL.
 */
stHeaderEntry*
readHeader(FILE * tarFile, int *nFiles)
{
    stHeaderEntry *entrada; //mirar como es la estructura de stHeaderEntry en donde esta definida (en el .h) pero esta compuesta por nombre y tamaño
	int num=0;
    fread(&num, sizeof(int),1,tarFile); //leemos el numero de ficheros que presenta el tar
  
    *nFiles=num; //asociamos a *nFiles lo leido anteriormente y este parametro de devuelve por referencia
    entrada=malloc(num*sizeof(stHeaderEntry)); //reservamos espacio para el array que contiene el nombre y tamaño de cada archivo
    int i=0, numero = 0;
    int c;

    for(i;i<num;i++){ //para cada uno de los archivos que exixten(num) hacemos:

    entrada[i].name=loadstr(tarFile); //leemos el nombre del archivo con la funcion implementada anteriormente
    fread(&entrada[i].size, sizeof(int),1,tarFile); // leemos el tamaño del archivo y lo guardamos en el apartado de tamaño del fichero correspondiente dentro del array
    numero = 0; //esto nose porque esta

    }
	return entrada; // devolemos la cabecera 
}

/** Creates a tarball archive 
 *
 * nfiles: number of files to be stored in the tarball
 * filenames: array with the path names of the files to be included in the tarball
 * tarname: name of the tarball archive
 * 
 * On success, it returns EXIT_SUCCESS; upon error it returns EXIT_FAILURE. 
 * (macros defined in stdlib.h).
 *
 * HINTS: First reserve room in the file to store the tarball header.
 * Move the file's position indicator to the data section (skip the header)
 * and dump the contents of the source files (one by one) in the tarball archive. 
 * At the same time, build the representation of the tarball header in memory.
 * Finally, rewind the file's position indicator, write the number of files as well as 
 * the (file name,file size) pairs in the tar archive.
 *
 * Important reminder: to calculate the room needed for the header, a simple sizeof 
 * of stHeaderEntry will not work. Bear in mind that, on disk, file names found in (name,size) 
 * pairs occupy strlen(name)+1 bytes.
 *
 */
int
createTar(int nFiles, char *fileNames[], char tarName[])
{
	FILE *destino; // fichero destino
	FILE *origenes[nFiles]; // array de ficheros origenes
	int sizes[nFiles]; // tamaños de esos ficheros origenes
	int c = '\0';
	int i = 0;
	int numeroBytes = 0;
	if(nFiles<=0) // si nFiles es <= 0 es un error
		return EXIT_FAILURE;

	for(i=0;i<nFiles;i++){ // para todos los fcheros origen:
		numeroBytes = numeroBytes + ((strlen(fileNames[i])+1)*sizeof(char)+sizeof(int)); // calcula el numero de bytes que ocupan su numbre(una cadena, de ahi la funcion strlen que calcula su longitud) y su tamaño (un entero)
		
		origenes[i] = fopen(fileNames[i],"r"); //abrimos el fichero origen
		if(!origenes[i])
			return EXIT_FAILURE;
	}
	destino = fopen(tarName,"w"); //abrimos el fichero destino

	fseek(destino, numeroBytes + sizeof(int), SEEK_SET); //desplazamos el numero de bytes desde el inicio del fichero

	for(i = 0; i< nFiles;i++){//calculamos el tamaño de cada fichero origen con la funcion copynfile y a su vez nos copia su contenido al fichero destino

		sizes[i] = copynFile(origenes[i], destino,INT_MAX);
		
	}
	fseek(destino, 0, SEEK_SET); //devolvemos el puntero a la posicion inicial del fichero destino
	
	fwrite(&nFiles, sizeof(int),1,destino); // escribimos el numero de ficheros que contiene nuestro file (nFiles) en destino.
	for(i = 0; i< nFiles;i++){ // escribimos por cada fichero su nombre y su tamaño

		fwrite(fileNames[i],sizeof(char), strlen(fileNames[i])+1,destino);

		fwrite(&sizes[i], sizeof(int),1,destino);

	}
	fclose(destino); // cerramos destino
	for(i = 0; i< nFiles;i++){ //cerramos todos los ficheros origen

		fclose(origenes[i]);

	}

	return EXIT_SUCCESS;
}

/** Extract files stored in a tarball archive
 *
 * tarName: tarball's pathname
 *
 * On success, it returns EXIT_SUCCESS; upon error it returns EXIT_FAILURE. 
 * (macros defined in stdlib.h).
 *
 * HINTS: First load the tarball's header into memory.
 * After reading the header, the file position indicator will be located at the 
 * tarball's data section. By using information from the 
 * header --number of files and (file name, file size) pairs--, extract files 
 * stored in the data section of the tarball.
 *
 */
int
extractTar(char tarName[])
{

	FILE *origen;
	int nFiles;
	
	origen = fopen(tarName, "r"); //abrimos el archivo a descomprimir
	if(origen==NULL)
	return EXIT_FAILURE;
	stHeaderEntry *ficheros = readHeader(origen,&nFiles); // array para los ficheros que vamos a extraer , lo obtenemos a la funcion readHeader

	FILE *destino[nFiles]; //array de ficheros destino
	int i =0;
	for(i = 0;i < nFiles;i++){ //por cada fichero destino se abre 1 fichero

		
		destino[i] = fopen(ficheros[i].name, "w");
		
		if(!destino[i])
		return EXIT_FAILURE;
	}
	
	int nBytesLeidos = 0;
	for(i = 0;i < nFiles;i++){
		nBytesLeidos = copynFile(origen, destino[i], ficheros[i].size);//copiamos con copynfile todo el fichero desde origen a destino 
		
		if(nBytesLeidos!=ficheros[i].size)
			return EXIT_FAILURE;
	}


	return EXIT_SUCCESS;
}
