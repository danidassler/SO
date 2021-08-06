#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

int my_system(char* argv[]){
	int pid = 1;

	pid = fork(); // hacemos un fork para crear un hijo
	if(pid==0){ // si es el hijo se ejecuta el siguiente codigo


		execl("/bin/bash","bash","-c", argv[1],(char*) 0); //el hijo ejecuta la llamada al sistema execl con los argumentos que se utilizan para ejecutar el programa
		exit(0); // finaliza
	}else if(pid>0){ // si es el padre, se hace un wait esperando la terminacion del hijo
			wait(NULL);
		
    if (errno == ECHILD) { //error
		printf("ERROR when waiting for children to finish\n");
		exit(-1);
	}

	}
	return 0;
}

int main(int argc, char* argv[])
{
	if (argc!=2){
		fprintf(stderr, "Usage: %s <command>\n", argv[0]);
		exit(1);
	}

	return my_system(argv);
}


