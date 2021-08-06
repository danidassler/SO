#include <stdio.h> 
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define NR_PHILOSOPHERS 5

pthread_t philosophers[NR_PHILOSOPHERS];
pthread_mutex_t forks[NR_PHILOSOPHERS];


void init()
{
    int i;
    for(i=0; i<NR_PHILOSOPHERS; i++)
        pthread_mutex_init(&forks[i],NULL);
    
}

void think(int i) { //funcion que imprime que el filosofo esta pensando
    printf("Philosopher %d thinking... \n" , i);
    sleep(random() % 10);
    printf("Philosopher %d stopped thinking!!! \n" , i);

}

void eat(int i) { //funcion que imprime que el filosofo esta comiendo
    printf("Philosopher %d eating... \n" , i);
    sleep(random() % 5);
    printf("Philosopher %d is not eating anymore!!! \n" , i);

}

void toSleep(int i) { //funcion que imprime que el filosofo va a dormir
    printf("Philosopher %d sleeping... \n" , i);
    sleep(random() % 10);
    printf("Philosopher %d is awake!!! \n" , i);
    
}

void* philosopher(void* i)
{
    int nPhilosopher = (int)i; // numero del filosofo
    int right = nPhilosopher; // tenedor de la derecha = numero de filosofo (en teoria es el tenedor que le corresponde a el
    int left = (nPhilosopher - 1 == -1) ? NR_PHILOSOPHERS - 1 : (nPhilosopher - 1); // esto se hace para que de la vuelta es decir 0 - 1 ( el tenedor de la izquierda del filosofo 0 es el 4 no el -1 ya que no existe )
    while(1)
    {
        
        think(nPhilosopher);
        
        if(i==0){ // si el numero de filosofo es el 0 ( el primero )
        // esto lo haciamos al reves (primero izquierda yluego derecha para que no hubiese un problema de que se bloquease todo 
        pthread_mutex_lock(&forks[left]); // lock del cerrojo izquierdo, coge el tenedor izquierdo 
        pthread_mutex_lock(&forks[right]); // lock del cerrojo derecho, coge el tenedor derecho

        }
        else{
        pthread_mutex_lock(&forks[right]); // lock del cerrojo derecho, coge el tenedor derecho
        pthread_mutex_lock(&forks[left]); // lock del cerrojo izquierdo, coge el tenedor izquierdo 
    }

        eat(nPhilosopher);
        
        // PUT FORKS BACK ON THE TABLE, lo mismo que antes pero dejando los tenedores en la mesa (unlock del mutex)
        if(i==0){

        pthread_mutex_unlock(&forks[left]);
        pthread_mutex_unlock(&forks[right]);

        }
        else{
        pthread_mutex_unlock(&forks[right]);
        pthread_mutex_unlock(&forks[left]);
    }
        toSleep(nPhilosopher);
   }

}

int main()
{
    init();
    unsigned long i;
// creamos un hilo para cada filosofo
    for(i=0; i<NR_PHILOSOPHERS; i++)
        pthread_create(&philosophers[i], NULL, philosopher, (void*)i);
    
    for(i=0; i<NR_PHILOSOPHERS; i++)
        pthread_join(philosophers[i],NULL);
    return 0;
} 
