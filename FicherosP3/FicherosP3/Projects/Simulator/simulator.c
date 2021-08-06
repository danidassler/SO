#include <stdio.h> 
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#define N_PARADAS  5   // numero de paradas de la ruta
#define EN_RUTA    0   // autobus en ruta
#define EN_PARADA  1   // autobus en la parada
#define MAX_USUARIOS 40  // capacidad del autobus
#define USUARIOS   4   // numero de usuarios
// estado inicial
int estado        = EN_RUTA;
int id_user=0;
int parada_actual = 0;     // parada en la que se encuentra el autobus
int n_ocupantes   = 0;     // ocupantes que tiene el autobus
pthread_cond_t vCondSubir;
pthread_cond_t vCondBajar;
pthread_cond_t autobus;
pthread_mutex_t mutex;

// personas que desean subir en cada parada
int esperando_parada[N_PARADAS]; //= {0,0,...0};
// personas que desean bajar en cada parada
int esperando_bajar[N_PARADAS];  //= {0,0,...0};

// Otras definiciones globales (comunicacion y sincronizacion)

void Conducir_Hasta_Siguiente_Parada(){
/*
Establecer un Retardo que simule el trayecto y actualizar numero de parada
*/
	sleep(rand()%10);
	pthread_mutex_lock(&mutex);
	parada_actual=(parada_actual+1)%N_PARADAS;
	pthread_mutex_unlock(&mutex);
}


void Autobus_En_Parada(){
/*
Ajustar el estado y bloquear al autobus hasta que no haya pasajeros que
quieran bajar y/o subir la parada actual. Despues se pone en marcha
*/
	pthread_mutex_lock(&mutex);
	estado=EN_PARADA;
	printf("Estoy en la parada %d\n",parada_actual );
	while(esperando_parada[parada_actual]>0 || esperando_bajar[parada_actual]>0){
		pthread_cond_broadcast(&vCondSubir);
        pthread_cond_broadcast(&vCondBajar);
        pthread_cond_wait(&autobus, &mutex);
    }
    estado=EN_RUTA;
    printf("Conduzco a la siguiente parada\n");
    pthread_mutex_unlock(&mutex);
}


void Subir_Autobus(int id_usuario, int origen){
/*
El usuario indicara que quiere subir en la parada ’origen’, esperara a que
el autobus se pare en dicha parada y subira. El id_usuario puede utilizarse para
proporcionar informacion de depuracion
*/
	pthread_mutex_lock(&mutex);
	esperando_parada[origen]++;
	while(estado==EN_RUTA || parada_actual != origen){
		pthread_cond_wait(&vCondSubir, &mutex);
	}
	printf("Usuario %d subiendo al bus\n",id_usuario );
	n_ocupantes++;
	esperando_parada[origen]--;
	pthread_cond_signal(&autobus);
	pthread_mutex_unlock(&mutex);
}


void Bajar_Autobus(int id_usuario, int destino){
/*
El usuario indicara que quiere bajar en la parada ’destino’, esperara a que
el autobus se pare en dicha parada y bajara. El id_usuario puede utilizarse para
proporcionar informacion de depuracion
*/
	pthread_mutex_lock(&mutex);
	esperando_bajar[destino]++;
	while(estado==EN_RUTA || parada_actual != destino){
		pthread_cond_wait(&vCondSubir, &mutex);
	}
	printf("Usuario %d bajando del bus\n",id_usuario );
	n_ocupantes--;
	esperando_bajar[destino]--;
	pthread_cond_signal(&autobus);
	pthread_mutex_unlock(&mutex);
}


void Usuario(int id_usuario, int origen, int destino) {
// Esperar a que el autobus este en parada origen para subir
Subir_Autobus(id_usuario, origen);
// Bajarme en estacion destino
Bajar_Autobus(id_usuario, destino);
}



void*thread_autobus(void*args) {
while (1) {
// esperar a que los viajeros suban y bajen
Autobus_En_Parada();
// conducir hasta siguiente parada
Conducir_Hasta_Siguiente_Parada();
}
}



void*thread_usuario(void*arg) {
int id_usuario, a ,b;
id_usuario=id_user++;
while (1) {
a=rand() % N_PARADAS;
do{
b=rand() % N_PARADAS;
} while(a==b);
Usuario(id_usuario,a,b);
}
}




int main(int argc, char* argv[])
{
	int i;
	pthread_t bus;
	pthread_t users[USUARIOS];
	pthread_mutex_init(&mutex, NULL);

// Definicion de variables locales a main
// Opcional: obtener de los argumentos del programa la capacidad del
// autobus, el numero de usuarios y el numero de paradas
// Crear el thread Autobus
	pthread_create(&bus,NULL,&thread_autobus,NULL);
for (i = 0; i<USUARIOS;i++){
	pthread_create(&users[i],NULL,&thread_usuario,NULL);
// Crear thread para el usuario i
// Esperar terminacion de los hilos
}
	for (i = 0; i<USUARIOS;i++){
		pthread_join(users[i],NULL);
	}
		pthread_join(bus,NULL);

return 0;

}