#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

int my_system(char* argv[]){
	int pid = 1;

	pid = fork();
	if(pid==0){


		execl("/bin/bash","bash","-c", argv[1],(char*) 0);
		exit(0);

	}else if(pid>0){

			wait(NULL);
			//while (wait(NULL) != -1) ;
if (errno == ECHILD) {
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


