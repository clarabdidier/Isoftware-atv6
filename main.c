#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
# define TRUE 1

int qtd_prod, qtd_cons, lim, size_buffer;
int j = 0, in = 0, out = 0;

int *buffer;

sem_t empty, full;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *producer (void *args);
void *consumer (void *args);
int calculator();

int main(){
    int i = 0;

    scanf("%d", &qtd_prod);
    scanf("%d", &qtd_cons);
    scanf("%d", &lim);
    scanf("%d", &size_buffer);
    
    buffer = calloc(size_buffer, sizeof(int));

    sem_init(&empty, 0, size_buffer);
    sem_init(&full, 0, 0);

    int id_prod[qtd_prod];
    int id_cons[qtd_cons];

    pthread_t thread_prods[qtd_prod];
    pthread_t thread_cons[qtd_cons];

    for (i = 0; i < qtd_prod; i++){
        id_prod[i] = i;
        pthread_create(&(thread_prods[i]), NULL, producer, &id_prod[i]);
    }
    for (i = 0; i < qtd_cons; i++){
        id_cons[i] = i;
        pthread_create(&(thread_cons[i]), NULL, consumer, &id_cons[i]);
    }
    for (i = 0; i < qtd_prod; i++){
        pthread_join(thread_prods[i], NULL);
    }
    for (i = 0; i < qtd_cons; i++){
        pthread_join(thread_cons[i], NULL);
    }

    return 0;
}

void *producer (void *args){
    int *id = args;

    while(TRUE){
        int res = calculator();

        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        buffer[in] = res;
        printf("Produtor %d produzindo %d na posição %d\n", *id, res, in);
        
        in = (in + 1) % size_buffer;


        pthread_mutex_unlock(&mutex);
        sem_post(&full);
        sleep(1);
        
    }

}

void *consumer (void *args){
    int *id = args;

    while(TRUE){

        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        
        printf("Consumidor %d consumindo %d na posição %d\n", *id, buffer[out], out);
        out = (out + 1) % size_buffer;

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
        sleep(1);
    }

}

int calculator(){
    int result = (2 * j) + 1;

    if (j == lim){
        j = 0;
    } else {
        j++;
    }

    return result;
}
