#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

/*
**The one command-line argument is
** the number of times to loop.
** Debian 8: 100000000
** Gouda: 10000000
** OS X: 100000
** You will need to compile with a "-lpthread" option.
*/

#define assertsyscall(x, y) if(!((x) y)){int err = errno; \
    fprintf(stderr, "In file %s at line %d: ", __FILE__, __LINE__); \
        perror(#x); exit(err);}

#define NUM_THREADS 2




//NOTES: you need to look at pthread_mutex! and theres an error in if statement of main
pthread_mutex_t lock;
int i;


void *foo (void *bar)
{
    pthread_t *me = new pthread_t (pthread_self());
    assertsyscall(printf("in a foo thread, ID %ld\n", *me), != 0);


    while (i < *((int *) bar)) 
    {
	pthread_mutex_lock(&lock);
        int tmp = i;

        if (tmp != i)
        {
            assertsyscall(printf ("aargh: %d != %d\n", tmp, i), !=0);
        }
	i++;


    pthread_mutex_unlock(&lock);
    }

	pthread_exit(me);
}

int main(int argc, char **argv)
{
    int iterations = strtol(argv[1], NULL, 10);
    pthread_t threads[NUM_THREADS];

    pthread_mutex_init(&lock,NULL);
    for (int i = 0; i < NUM_THREADS; i++)
    {
        assertsyscall(pthread_create(&threads[i], NULL, foo, (void *) &iterations), ==0);
        
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        void *status;
        assertsyscall(pthread_join (threads[i], &status), ==0 );
        assertsyscall(printf("joined a foo thread, number %ld\n", *((pthread_t *) status)), !=0);
    }

    pthread_mutex_destroy(&lock);

    return (0);
}
