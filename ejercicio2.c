// DANIEL SANZ MAYO 3ºB
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>



int main(){
    // con un puntero a dir abriremos el directorio
    DIR *dir;
    // aqui esta la info
    struct dirent *ent;
    struct stat data;
    char *cadena; 
    //empezamos a leer en el directorio actual    
    dir = opendir(".");
    if(dir==NULL){
    perror("No se ha podido abrir el directorio");  
    exit(EXIT_FAILURE);
    }
    // mientras que la lectura no de null ...
    while((ent=readdir(dir)) != NULL){
        cadena=ent->d_name;
        if(stat(ent->d_name, &data)==-1){
            return DT_UNKNOWN;
        }
          
        if ( (strcmp(ent->d_name, ".")!=0) && (strcmp(ent->d_name, "..")!=0) ){
            printf("\n");
            printf("Nombre del fichero:        %s\n", cadena);        
            printf("Tipo de fichero:           ");
            switch (data.st_mode & S_IFMT)
            {
               case S_IFBLK:  printf("dispositivo de bloques\n");   break;
               case S_IFCHR:  printf("dispositivo de caracteres\n");break;
               case S_IFDIR:  printf("directorio\n");               break;
               case S_IFIFO:  printf("FIFO/tuberia nombrada\n");    break;
               case S_IFLNK:  printf("enlace simbolico\n");         break;
               case S_IFREG:  printf("fichero regular\n");          break;
               case S_IFSOCK: printf("conector\n");                 break;
               default:       printf("desconocido?\n");             break;
               }

            printf("Permisos:                  ");
            printf( (data.st_mode & S_IRUSR) ? "r" : "-");
            printf( (data.st_mode & S_IWUSR) ? "w" : "-");
            printf( (data.st_mode & S_IXUSR) ? "x" : "-");
            printf( (data.st_mode & S_IRGRP) ? "r" : "-");
            printf( (data.st_mode & S_IWGRP) ? "w" : "-");
            printf( (data.st_mode & S_IXGRP) ? "x" : "-");
            printf( (data.st_mode & S_IROTH) ? "r" : "-");
            printf( (data.st_mode & S_IWOTH) ? "w" : "-");
            printf( (data.st_mode & S_IXOTH) ? "x" : "-");            
            printf("\n");

            printf("Numero de i-nodo:          %ld\n", (long) data.st_ino);

            printf("Numero de enlaces fisicos: %ld\n", (long) data.st_nlink);
            printf("ID del grupo propietario:  UID=%ld   GID=%ld\n",
                       (long) data.st_uid, (long) data.st_gid);

            printf("Tamaño total:              %lld bytes\n",
                       (long long) data.st_size);

            printf("Ultima modificacion:       %s", ctime(&data.st_mtime));
            printf("\n");

            }
        }
    // cerramos el directorio.      
    closedir(dir);
    return EXIT_SUCCESS;
}


