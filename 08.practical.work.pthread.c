#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<stdbool.h>
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
	item* bottle = createItem('0', 3, '0');
	produce(bottle);
	produce(bottle);
	produce(bottle);
	pthread_exit(NULL);

}


void *consumerThread(void* param){
	printf("Consumer Thread is speaking\nConsuming bottles...\n");
	consume();
	consume();
	consume();
	pthread_exit(NULL);

}


int main(){
	pthread_t tid;
	printf("Main Thread is speaking\n");
	printf("Creating Producer Thread\n");
	pthread_create(&tid, NULL, producerThread, NULL);
	printf("Now creating Cosumer Thread\n");
	pthread_create(&tid, NULL, consumerThread, NULL);
	pthread_join(tid, NULL);
	pthread_join(tid, NULL);
	
	pthread_exit(NULL);

}


