CC = gcc
COPTS =-g -Wall -std=c99 -lm -pthread
JUNK = *.o *~ *.gch *.dSYM philo_t

philo_t: random_r.o philo_t.o
	$(CC) $(COPTS) -o philo_t philo_t.o random_r.o

philo_t.o: philo_t.c random_r.h
	$(CC) $(COPTS) -c philo_t.c random_r.h

random_r.o: random_r.c random_r.h
	$(CC) $(COPTS) -c random_r.c random_r.h

clean: 
	rm $(JUNK)
