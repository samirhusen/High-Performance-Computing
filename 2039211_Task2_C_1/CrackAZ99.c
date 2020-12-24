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
  "brute force" algorithm. Works on passwords that consist only of 2 uppercase
  letters and a 2 digit integer.

  Compile with:
  
  	cc CrackAZ99.c -lm -lcrypt -o CrackAZ99

  To run:

	./CrackAZ99

  Dr Kevan Buckley, University of Wolverhampton, 2018 Modified by Dr. Ali Safaa 2019
 
  Samir Husen 2020 Dec 23
 
 **********************************************************************************/

int count = 0;     // A counter used to track the number of combinations explored so far


void substr(char *dest, char *src, int start, int length){
	memcpy(dest, src + start, length);
	*(dest + length) = '\0';
}

// The function below crack password by running multiple loops
void crack(char *salt_and_encrypted){
	int x, y, z;     // Loop counters
	char salt[7];    // String used in hashing the password
	char plain[7];   // The combination of letters currently being checked 
	char *enc;       // Pointer to the encrypted password

	substr(salt, salt_and_encrypted, 0, 6);

	for(x='A'; x<='Z'; x++){
		for(y='A'; y<='Z'; y++){
			for(z=0; z<=99; z++){
				sprintf(plain, "%c%c%02d", x, y, z);
				enc = (char *) crypt(plain, salt);
				count++;
				if(strcmp(salt_and_encrypted, enc) == 0){
					printf("#%-8d%s %s\n", count, plain, enc);
					//return
				}
			}
		}
	}
}


int main(int argc, char *argv[]){

	// SH75 encrypted form
	// Using EncyptSHA512 algorithm
	// $6$AS$Kcq4LBuQFdOTuB6newN240B8BvTsrcmt84AWZ3gYh.rw0RIKsQ/wL0AhUowWuTMqG8FjX8Lq.5AAGDmh0jfQe/

	printf("PASSWORD CRACKING USING POSIX SINGLE THREAD ( 2 UPPER CASE LETTERS AND 2 INTEGER NUMBERS )\n");
	printf("Number OF Repetitions: %d \n", CRYPT_TEST_COUNT);

	struct timespec start, finish;
	double test_time[CRYPT_TEST_COUNT];
	double total_time = 0, total_square_time = 0, average_time = 0, variance_time = 0;

	for(int i = 0; i < CRYPT_TEST_COUNT; i++)
	{
		clock_gettime(CLOCK_REALTIME, &start);
		
		//SH75 the password to be cracked 
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

