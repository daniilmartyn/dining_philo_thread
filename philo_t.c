#define _XOPEN_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include "random_r.h"

/*
	This program simulated the dining philosophers problem using 5 threads. The concurrency 
	and resource sharing problem is solved using mutexes.
*/


pthread_mutex_t *mutexes;

int think(int id, int total, unsigned int *state){
	//printf("inside think\n");
	//printf("getting random num for philo %d\n", id);
	int time = randomGaussian_r(11,7, state);
	//printf("Think time for philo %d is %d\n", id, time);
	if(time < 0 ) time = 0;
	
	printf("Philo %d thinking for %d seconds. (Total = %d)\n", id, time, total);

	sleep(time);
	return time;
}

int eat(int id, int total, unsigned int *state){
	//printf("inside eat\n");
	int time = randomGaussian_r(9,3, state);
	if(time < 0 ) time = 0;
	
	printf("Philo %d eating for %d seconds. (Total = %d)\n", id, time, total);
	
	sleep(time);
	return time;
}

void dine(int *idPointer){

	int id = *idPointer;
	//printf("inside dine w/ philo %d\n", id);
	int eats = 0;
	int thinks = 0;
    unsigned int state = id + 1;
	
	while(eats < 20){
	
		int thinktemp = think(id, thinks, &state);
		thinks = thinks + thinktemp;
				
		// lock mutexes
		//printf("locking mutexes for philo %d\n", id);
		int result = EBUSY;
		while(result == EBUSY){
			pthread_mutex_lock(&mutexes[id]);
			if((result = pthread_mutex_trylock(&mutexes[(id+1) % 5])) == EBUSY){
				pthread_mutex_unlock(&mutexes[id]);
			}
		}
		//printf("locked mutexes for philo %d\n", id);
		/////// critical section
		int eattemp = eat(id, eats, &state);
		eats = eats + eattemp;
		/////// end critical section
		
		pthread_mutex_unlock(&mutexes[id]);
		pthread_mutex_unlock(&mutexes[(id+1) % 5]);
		//printf("released mutexes for philo %d\n", id);
	}
	printf("Philo %d has finished eating, now leaving table\n", id);
}

void joinErrMsg(int errnum){
	fprintf(stderr, "Error Joining Thread: %d\n", errnum);
	exit(1);
}

void createErrMsg(int errnum){
	fprintf(stderr, "Error Creating Thread: %d", errnum);
	exit(1);
}

int main(){
	
	pthread_t philo_1, philo_2, philo_3, philo_4, philo_5;
	int one, two, three, four, five;
	one = 0; two = 1; three = 2; four = 3; five = 4;
	
	mutexes = (pthread_mutex_t *) malloc(5*sizeof(pthread_mutex_t));
	
	for(int i = 0; i < 5; i++){
		pthread_mutex_init(&mutexes[i], NULL);
	}
	
	//printf("initialized the mutexes\n");
	
	int temp;
	if((temp = pthread_create(&philo_1, NULL, (void *) dine, (void *) &one))) createErrMsg(temp);
	if((temp = pthread_create(&philo_2, NULL, (void *) dine, (void *) &two))) createErrMsg(temp);
	if((temp = pthread_create(&philo_3, NULL, (void *) dine, (void *) &three))) createErrMsg(temp);
	if((temp = pthread_create(&philo_4, NULL, (void *) dine, (void *) &four))) createErrMsg(temp);
	if((temp = pthread_create(&philo_5, NULL, (void *) dine, (void *) &five))) createErrMsg(temp);
	
	if((temp = pthread_join(philo_1, NULL))) joinErrMsg(temp);
	if((temp = pthread_join(philo_2, NULL))) joinErrMsg(temp);
	if((temp = pthread_join(philo_3, NULL))) joinErrMsg(temp);
	if((temp = pthread_join(philo_4, NULL))) joinErrMsg(temp);
	if((temp = pthread_join(philo_5, NULL))) joinErrMsg(temp);
	
	return 0;
}