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
	while ((c = getc(origin))!= EOF && nLeidos < nBytes ){

		putc((unsigned char)c,destination);
		nLeidos++;
	}
	return nLeidos; // devolvemos el numero de leidos

/*
	 int c = 0;
	int nLeidos = 0;
  
  if(!origin){
    printf( "Error no se ha podido abrir el archivo\n" );
    return -1;
  }
  

  

  while(c!=-1 && nLeidos < nBytes){ // creo que si aqui hubiesemos puesto EOF en vez de -1 hubiese funcionado bien 
  	c=fgetc(origin);
  	fputc((unsigned char)c,destination);
  	nLeidos++;
  }

 
  return nLeidos;

  */
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
// nose como hacer la parte de resevar la memoria creo que es con malloc, preguntar a casado/profesor preguntar si es asi:
	
	int c, nLeidos = 0;
   char* cadena;
   char f;
   // cadena=malloc(sizeof(char)*200);// preguntar si vale hacer la trampa de poner un numero muy grande ya que nunca habra una cadena tan grande;
   if(file==NULL) {
   	return NULL;}
   else{
    // primero contabamos cuanto ocupaba exactamente la cadena usando nLeidos
    fread(&f, sizeof(char),1,file); //porque era &?
	nLeidos++;
	while(f!='\0'){
   fread(&f, sizeof(char),1,file);
  // c=fgetc(file);
	nLeidos++;
  }
  // despues desplazabamos el puntero hasta el principio otra vez 
  fseek(file,-nLeidos,SEEK_CUR);
     }

	cadena=malloc(sizeof(char)*nLeidos);
	if(cadena == NULL)return NULL;

	fread(cadena, sizeof(char),nLeidos,file);

    return cadena; /
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
	// lee la cabecera del talFile, lo primero que hay es un int indicando el numero de ficheros que contiene
    stHeaderEntry *entrada;
	int num=0;
    fread(&num, sizeof(int),1,tarFile);
   
    // stHeaderEntry es una estructura declarada en mytar.c
    *nFiles=num; // el numero de ficheros del mtar seran los leidos anteriormente
    entrada=malloc(num*sizeof(stHeaderEntry));
    int i=0, numero = 0;
    int c;
    // en esta parte leeremos los nombres de todos los ficheros contenidos en el mtar, que es lo que viene a continuacion en la cabecera de este
    for(i;i<num;i++){
    entrada[i].name=loadstr(tarFile);
    // aqui se le el tamaño del fichero, que es lo que viene a continuacion del nombre
    fread(&entrada[i].size, sizeof(int),1,tarFile);
    numero = 0;
    }
	return entrada;
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
	FILE *destino;
	FILE *origenes[nFiles];
	int sizes[nFiles];
	int c = '\0';
	int i = 0;
	int numeroBytes = 0;
	if(nFiles<=0)
		return EXIT_FAILURE;

	for(i=0;i<nFiles;i++){
		//calculamos lo que ocupa el fichero
		numeroBytes = numeroBytes + ((strlen(fileNames[i])+1)*sizeof(char)+sizeof(int));
		//creamos el fichero con el nombre correspondiente
		origenes[i] = fopen(fileNames[i],"r");
		if(!origenes[i])
			return EXIT_FAILURE;
	}
	destino = fopen(tarName,"w");
    //desplaza el puntero desde el inicio de destino hatsa numeroBytes+sizofint
	fseek(destino, numeroBytes + sizeof(int), SEEK_SET); 

	for(i = 0; i< nFiles;i++){
        //guardamos cuanto ocupa cada fichero en sizes y ademas copiamos la info de cada fichero en el mtar
		sizes[i] = copynFile(origenes[i], destino,INT_MAX);
		
	}
	//desplazamos el puntero al inicio del mtar para crear su cabezera (su contenido ya ha sido creado en el for anterior)
	fseek(destino, 0, SEEK_SET);
	//escribimos el numero de ficheros que hay
	fwrite(&nFiles, sizeof(int),1,destino);
	// con este for ponemos el numbre y lo que ocupa cada fichero en la cabecera
	for(i = 0; i< nFiles;i++){


		fwrite(fileNames[i],sizeof(char), strlen(fileNames[i])+1,destino);

		fwrite(&sizes[i], sizeof(int),1,destino);
;

	}
	fclose(destino);
	for(i = 0; i< nFiles;i++){

		fclose(origenes[i]);

	}

	// lseek( cual es el fd que tenemos que poner??

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
	// Complete the function
	FILE *origen;
	int nFiles;
	
	origen = fopen(tarName, "r");
	if(origen==NULL)
	return EXIT_FAILURE;
    // se crea el par nombre-tamaño de todos los ficheros del tar
	stHeaderEntry *ficheros = readHeader(origen,&nFiles);

	FILE *destino[nFiles];
	int i =0;
	for(i = 0;i < nFiles;i++){

		//se crea un ficher por cada par nombre-tamaño
		destino[i] = fopen(ficheros[i].name, "w");
		
		if(!destino[i])
		return EXIT_FAILURE;
	}
	
	int nBytesLeidos = 0;
	for(i = 0;i < nFiles;i++){
		nBytesLeidos = copynFile(origen, destino[i], ficheros[i].size);

		
		if(nBytesLeidos!=ficheros[i].size)
			return EXIT_FAILURE;
	}


	return EXIT_SUCCESS;
}
