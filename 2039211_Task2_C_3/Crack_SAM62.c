#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <unistd.h>

#include "config.h"
#include "time.h"
#include <math.h>

/**********************************************************************************
  Demonstrates how to crack an encrypted password using a simple
  "brute force" algorithm. Works on passwords that consist only of 3 uppercase
  letters and a 2 digit integer.

  Compile with:
  
  	cc CrackSAM62.c -lm -lcrypt -lpthread -o CrackSAM62

  To run:
	
	./CrackSAM62


  Dr Kevan Buckley, University of Wolverhampton, 2018 Modified by Dr. Ali Safaa 2019
    
  Samir Husen 2020 Dec 23

 **********************************************************************************/

int count = 0;     // A counter used to track the number of combinations explored so far


void substr(char *dest, char *src, int start, int length){
	memcpy(dest, src + start, length);
	*(dest + length) = '\0';
}


//the function below can crack the password by running multiple loops
void crack(char *salt_and_encrypted){
	int i, j, k, l;  // Loop counters
	char salt[7];    // String used in hashing the password
	char plain[7];   // The combination of letters currently being checked
	char *enc;       // Pointer to the encrypted password

	substr(salt, salt_and_encrypted, 0, 6);

	for(i='A'; i<='Z'; i++){
		for(j='A'; j<='Z'; j++){
			for(k='A'; k<='Z'; k++){
				for(l=0; l<=99; l++){
					sprintf(plain, "%c%c%c%02d", i, j, k, l);
					enc = (char *) crypt(plain, salt);
					count++;
					if(strcmp(salt_and_encrypted, enc) == 0){
						printf("#%-8d%s %s\n", count, plain, enc);
						
					}
				}
			}
		}
	}
}

int main(int argc, char *argv[]){

	// SAM62 three uppercase and two digit integer
	// Using EncyptSHA512 algorithm
	// $6$AS$GM5BIhCm/SpTdef6B9f1BqkJVnXcyonjn1QLx1UfRR1FQf38vZ3hamRqCjPUO7y1xWYYyH.VusSC.GDQAxuff.

	printf("PASSWORD CRACKING USING POSIX SINGLE THREAD ( 3 UPPER CASE LETTERS AND 2 INTEGER NUMBERS )\n");
	printf("Number OF Repetitions: %d \n", CRYPT_TEST_COUNT);

	struct timespec start, finish;
	double test_time[CRYPT_TEST_COUNT];
	double total_time = 0, total_square_time = 0, average_time = 0, variance_time = 0;

	for(int i = 0; i < CRYPT_TEST_COUNT; i++)
	{
		clock_gettime(CLOCK_REALTIME, &start);
		
		//The password to be encrypt
		crack("$6$AS$GM5BIhCm/SpTdef6B9f1BqkJVnXcyonjn1QLx1UfRR1FQf38vZ3hamRqCjPUO7y1xWYYyH.VusSC.GDQAxuff.");
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

