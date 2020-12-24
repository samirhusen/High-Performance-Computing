#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <unistd.h>

#include "config.h"
#include "time.h"
#include <math.h>
#include <pthread.h>

/************************************************************************************
  Demonstrates how to crack an encrypted password using a simple
  "brute force" algorithm. Works on passwords that consist only of 2 uppercase
  letters and a 2 digit integer.

  Compile with:
  
  	cc CrackAZ99MultiThread.c -lm -lcrypt -lpthread -o CrackAZ99MultiThread

  To run:
	./CrackAZ99MultiThread
   
  Dr Kevan Buckley, University of Wolverhampton, 2018 Modified by Dr. Ali Safaa 2019
 
  Samir Husen 2020 Dec 24

 ***********************************************************************************/

int count = 0;     // A counter used to track the number of combinations explored so far

typedef struct CrackPasswordArgs{
	char* salt_and_encrypted;
	char start;
	char stop;
} CrackPasswordArgs_t;

/**
 Required by lack of standard function in C.   
 */

void substr(char *dest, char *src, int start, int length){
	memcpy(dest, src + start, length);
	*(dest + length) = '\0';
}


// The function below can crack the password by running multiple loop

static void* crack_func(void* args){
	CrackPasswordArgs_t* pwd_args = (CrackPasswordArgs_t*)args;
	int x, y, z;     // Loop counters
	char salt[7];    // String used in hashing the password
	char plain[7];   // The combination of letters currently being checked
	char *enc;       // Pointer to the encrypted password

	substr(salt, pwd_args->salt_and_encrypted, 0, 6);

	for(x=pwd_args->start; x<=pwd_args->stop; x++){
		for(y='A'; y<='Z'; y++){
			for(z=0; z<=99; z++){
				sprintf(plain, "%c%c%02d", x, y, z);
				enc = (char *) crypt(plain, salt);
				count;
				if(strcmp(pwd_args->salt_and_encrypted, enc) == 0){
					printf("#%-8d%s %s\n", count, plain, enc);
					// return;	
				}
			}
		}
	}

	free(pwd_args->salt_and_encrypted);
	pwd_args->salt_and_encrypted = NULL;
	free(pwd_args);

	return NULL;
}

void crack(char *salt_and_encrypted){
	pthread_t threads[PWD_CRACK_NO_OF_THREADS];

	char start[PWD_CRACK_NO_OF_THREADS] = {'A', 'N'};
	char stop[PWD_CRACK_NO_OF_THREADS] = {'M', 'Z'};

	for(int i = 0; i < PWD_CRACK_NO_OF_THREADS; i++)
	{
		CrackPasswordArgs_t* pwd_args = (CrackPasswordArgs_t*)calloc(1, sizeof(CrackPasswordArgs_t));
		pwd_args->salt_and_encrypted = strdup(salt_and_encrypted);
		pwd_args->start = start[i];
		pwd_args->stop = stop[i];
		pthread_create(&threads[i], NULL, crack_func, pwd_args);
	}

	for(int i = 0; i < PWD_CRACK_NO_OF_THREADS; i++)
	{
		pthread_join(threads[i], NULL);
	}
}

int main(int argc, char *argv[]){

	// SH75
	// Using EncyptSHA512 algorithm
	// $6$AS$Kcq4LBuQFdOTuB6newN240B8BvTsrcmt84AWZ3gYh.rw0RIKsQ/wL0AhUowWuTMqG8FjX8Lq.5AAGDmh0jfQe/

	printf("PASSWORD CRACKING USING POSIX MULTI THREAD ( 2 UPPER CASE LETTERS AND 2 INTEGER NUMBERS )\n");
	printf("Number OF Repetitions: %d \n", CRYPT_TEST_COUNT);


	struct timespec start, finish;
	double test_time[CRYPT_TEST_COUNT];
	double total_time = 0, total_square_time = 0, average_time = 0, variance_time = 0;

	for(int i = 0; i < CRYPT_TEST_COUNT; i++)
	{
						
		clock_gettime(CLOCK_REALTIME, &start);
		
		//The password to be encrypt
	  	crack("$6$AS$Kcq4LBuQFdOTuB6newN240B8BvTsrcmt84AWZ3gYh.rw0RIKsQ/wL0AhUowWuTMqG8FjX8Lq.5AAGDmh0jfQe/");
		clock_gettime(CLOCK_REALTIME, &finish);

	    	long seconds = finish.tv_sec - start.tv_sec;
	    	long ns = finish.tv_nsec - start.tv_nsec;

	    	if (start.tv_nsec > finish.tv_nsec)
	   	{
	    		--seconds;
	    		ns += 1000000000;
	    	}

	   	double time_elapsed = (double)seconds + (double)ns/(double)1000000000;

		test_time[i] = time_elapsed;
		total_time += time_elapsed;
		printf("%10s %10s \n", "REPETITION", "TIME(seconds)");
		printf("%5d %15.3f \n", (i + 1), time_elapsed);
		fflush(stdout);
	}

	average_time = total_time / CRYPT_TEST_COUNT;

	for(int i = 0; i < CRYPT_TEST_COUNT; i++)
	{
		total_square_time += pow(test_time[i] - average_time, 2);
	}

	variance_time = sqrt(total_square_time / CRYPT_TEST_COUNT);
	printf("\n Average time %5.3f +/- %5.3f seconds \n", average_time, variance_time);
	return 0;
}

