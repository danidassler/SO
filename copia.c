#include<stdio.h>
#include<stdlib.h>


int main(){
  FILE *origen, *destino;
  int c;
  origen=fopen("origent.txt", "r");
  if(!origen){
    printf( "Error no se ha podido abrir el archivo\n" );
    return -1;
  }
  destino=fopen("destino.txt", "w");

  c=fgetc(origen);
  while(c!=-1){
   fputc(c,destino);
   c=fgetc(origen);
  }
  fclose(origen);
  fclose(destino);
  return 0;
}
