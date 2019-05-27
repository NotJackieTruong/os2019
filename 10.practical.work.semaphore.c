#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<stdbool.h>
#include<semaphore.h>

#define BUFFER_SIZE 10

int pid = -1;


typedef struct {
	char type; // 0=bottle, 1=pencil
	int amount;
	char unit; //0=gram, 1=things
} item;

item buffer [BUFFER_SIZE];
int first = 0;
int last = 0;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

int pthread_create(
	pthread_t *thread,
	const pthread_attr_t *attr,
	void *(*start_routine) (void *),
	void *arg
);



void produce(item *i) {
	while ((first + 1) % BUFFER_SIZE == last){
		printf("No free buffer item\n");
	}
	memcpy(&buffer[first], i, sizeof(item));
	i->amount += 1;
	first = (first + 1) % BUFFER_SIZE;
	printf("first = %d\t last = %d\n", first, last);

}

item *consume() {
	item *i = malloc(sizeof(item));
	while (first == last){
		printf("Nothing to consume");
	}
	memcpy(i, &buffer[last], sizeof(item));
	i->amount -= 1;
	last = (last +1) % BUFFER_SIZE;
	printf("first = %d\t last = %d\n", first, last);
	return i;

}

item* createItem(char type, int amount, char unit){
	item* i = malloc(sizeof(item));
	i->type= type;
	i->amount = amount;
	i->unit = unit;
	return i;
}

void *producerThread(void* param){

	printf("Producer Thread is speaking\nProducing bottles...\n");
	pthread_mutex_lock(&m);
	item* bottle = createItem('0', 3, '0');
	produce(bottle);
	produce(bottle);
	produce(bottle);
	pthread_mutex_unlock(&m);
	pthread_exit(NULL);

}


void *consumerThread(void* param){
	
	printf("Consumer Thread is speaking\nConsuming bottles...\n");
	pthread_mutex_lock(&m);
	consume();
	consume();
	consume();
	pthread_mutex_unlock(&m);
	pthread_exit(NULL);

}


int main(){
	pthread_t tid1, tid2;
	int error;
	if(pthread_create(&tid1, NULL, producerThread, NULL) !=0){
		pthread_mutex_error("pthread_create() fails");
	}
	if(pthread_create(&tid2, NULL, consumerThread, NULL) !-0){
		pthread_mutex_error("pthread_create() fails");
	}
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	pthread_mutex_destroy(&m);
	return 0;

}


