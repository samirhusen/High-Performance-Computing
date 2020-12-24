/************************************************************************************

  The program below implements matrix multiplication using multithreading
  
  MULTI THREAD MATRIX MULTIPLICATION
  
  To compile this program:

  	cc Multi_thread.c Utils.c -lm -lpthread -o Multi_thread 

  To run:
	
	./Multi_thread 
  
  Samir Husen - 2020 Dec 23
  

*************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <math.h>
#include <pthread.h>

#include "config.h"
#include "matrix_utils.h"

#define DIVIDE_SPLIT_LEVEL 7

typedef struct MatrixMultiplicationArgs {

	float** A;
	float** B;
	float** C;
	uint16_t n;
	uint16_t p;
	uint16_t m;
	
}MatrixMultiplicationArgs_t;

void* multi_threaded_matrix_multiplication2(void* args)
{
	MatrixMultiplicationArgs_t* _args = (MatrixMultiplicationArgs_t*) args;
	float** A = _args->A;
	float** B = _args->B;
	float** C = _args->C;
	uint16_t n = _args->n;
	uint16_t p = _args->p;
	uint16_t m = _args->m;

	// Here, N, P and M are powers of 2
	int side_length = pow(2, DIVIDE_SPLIT_LEVEL);
	if(n == side_length && p == side_length && m == side_length)
	{
		for (int i = 0; i < n; i++)
		{
			for(int j = 0; j < m; j++)
			{
				for(int k = 0; k < p; k++)
				{
					C[i][j] += A[i][k] * B[k][j];
				}
			}
		}
	}
	else
	{
		int _n = n / 2, _p = p / 2, _m = m / 2;

		float ** a11 = zero_matrix(_n, _p);
		float ** a12 = zero_matrix(_n, _p);
		float ** a21 = zero_matrix(_n, _p);
		float ** a22 = zero_matrix(_n, _p);

		float ** b11 = zero_matrix(_n, _p);
		float ** b12 = zero_matrix(_n, _p);
		float ** b21 = zero_matrix(_n, _p);
		float ** b22 = zero_matrix(_n, _p);

		float ** c11_1 = zero_matrix(_n, _p);
		float ** c12_1 = zero_matrix(_n, _p);
		float ** c21_1 = zero_matrix(_n, _p);
		float ** c22_1 = zero_matrix(_n, _p);

		float ** c11_2 = zero_matrix(_n, _p);
		float ** c12_2 = zero_matrix(_n, _p);
		float ** c21_2 = zero_matrix(_n, _p);
		float ** c22_2 = zero_matrix(_n, _p);

		for(int i = 0; i < _n; i++)
		{
			for(int j = 0; j < _p; j++)
			{
				a11[i][j] = A[i][j];
				a12[i][j] = A[i][_n + j];
				a21[i][j] = A[_n + i][j];
				a22[i][j] = A[_n + i][_n + j];

				b11[i][j] = B[i][j];
				b12[i][j] = B[i][_n + j];
				b21[i][j] = B[_n + i][j];
				b22[i][j] = B[_n + i][_n + j];
			}
		}

		pthread_t threads[8];
		MatrixMultiplicationArgs_t* _args1 = (MatrixMultiplicationArgs_t*) malloc(sizeof(MatrixMultiplicationArgs_t));
		_args1->A = a11;
		_args1->B = b11;
		_args1->C = c11_1;
		_args1->n = _n;
		_args1->p = _p;
		_args1->m = _m;
		pthread_create(&threads[0], NULL, multi_threaded_matrix_multiplication2, _args1);

		MatrixMultiplicationArgs_t* _args2 = (MatrixMultiplicationArgs_t*) malloc(sizeof(MatrixMultiplicationArgs_t));
		_args2->A = a11;
		_args2->B = b12;
		_args2->C = c12_1;
		_args2->n = _n;
		_args2->p = _p;
		_args2->m = _m;
		pthread_create(&threads[1], NULL, multi_threaded_matrix_multiplication2, _args2);

		MatrixMultiplicationArgs_t* _args3 = (MatrixMultiplicationArgs_t*) malloc(sizeof(MatrixMultiplicationArgs_t));
		_args3->A = a21;
		_args3->B = b11;
		_args3->C = c21_1;
		_args3->n = _n;
		_args3->p = _p;
		_args3->m = _m;
		pthread_create(&threads[2], NULL, multi_threaded_matrix_multiplication2, _args3);

		MatrixMultiplicationArgs_t* _args4 = (MatrixMultiplicationArgs_t*) malloc(sizeof(MatrixMultiplicationArgs_t));
		_args4->A = a21;
		_args4->B = b12;
		_args4->C = c22_1;
		_args4->n = _n;
		_args4->p = _p;
		_args4->m = _m;
		pthread_create(&threads[3], NULL, multi_threaded_matrix_multiplication2, _args4);

		MatrixMultiplicationArgs_t* _args5 = (MatrixMultiplicationArgs_t*) malloc(sizeof(MatrixMultiplicationArgs_t));
		_args5->A = a12;
		_args5->B = b21;
		_args5->C = c11_2;
		_args5->n = _n;
		_args5->p = _p;
		_args5->m = _m;
		pthread_create(&threads[4], NULL, multi_threaded_matrix_multiplication2, _args5);

		MatrixMultiplicationArgs_t* _args6 = (MatrixMultiplicationArgs_t*) malloc(sizeof(MatrixMultiplicationArgs_t));
		_args6->A = a12;
		_args6->B = b22;
		_args6->C = c12_2;
		_args6->n = _n;
		_args6->p = _p;
		_args6->m = _m;
		pthread_create(&threads[5], NULL, multi_threaded_matrix_multiplication2, _args6);

		MatrixMultiplicationArgs_t* _args7 = (MatrixMultiplicationArgs_t*) malloc(sizeof(MatrixMultiplicationArgs_t));
		_args7->A = a22;
		_args7->B = b21;
		_args7->C = c21_2;
		_args7->n = _n;
		_args7->p = _p;
		_args7->m = _m;
		pthread_create(&threads[6], NULL, multi_threaded_matrix_multiplication2, _args7);

		MatrixMultiplicationArgs_t* _args8 = (MatrixMultiplicationArgs_t*) malloc(sizeof(MatrixMultiplicationArgs_t));
		_args8->A = a22;
		_args8->B = b22;
		_args8->C = c22_2;
		_args8->n = _n;
		_args8->p = _p;
		_args8->m = _m;
		pthread_create(&threads[7], NULL, multi_threaded_matrix_multiplication2, _args8);

		for(int i = 0; i < 8; i++)
		{
			pthread_join(threads[i], NULL);
		}

		free(_args1);
		free(_args2);
		free(_args3);
		free(_args4);
		free(_args5);
		free(_args6);
		free(_args7);
		free(_args8);

		float ** c11 = add(c11_1, c11_2, _n, _m);
		float ** c12 = add(c12_1, c12_2, _n, _m);
		float ** c21 = add(c21_1, c21_2, _n, _m);
		float ** c22 = add(c22_1, c22_2, _n, _m);

		for(int i = 0; i < _n; i++)
		{
			for(int j = 0; j < _p; j++)
			{
				C[i][j] = c11[i][j];
				C[i][_n + j] = c12[i][j];
				C[_n + i][j] = c21[i][j];
				C[_n + i][_n + j] = c22[i][j];
			}
		}

		delete_matrix(a11, _n, _p);
		delete_matrix(a12, _n, _p);
		delete_matrix(a21, _n, _p);
		delete_matrix(a22, _n, _p);

		delete_matrix(b11, _n, _p);
		delete_matrix(b12, _n, _p);
		delete_matrix(b21, _n, _p);
		delete_matrix(b22, _n, _p);

		delete_matrix(c11_1, _n, _p);
		delete_matrix(c12_1, _n, _p);
		delete_matrix(c21_1, _n, _p);
		delete_matrix(c22_1, _n, _p);

		delete_matrix(c11_2, _n, _p);
		delete_matrix(c12_2, _n, _p);
		delete_matrix(c21_2, _n, _p);
		delete_matrix(c22_2, _n, _p);

		delete_matrix(c11, _n, _p);
		delete_matrix(c12, _n, _p);
		delete_matrix(c21, _n, _p);
		delete_matrix(c22, _n, _p);
	}
	return NULL;
}

int main(int argc, char** argv)
{
	
	//A[N][P] * B[P][M] = C[N][M]

	printf("MULTI THREAD MATRIX MULTIPLICATION\n");
	printf(" N : %d\n M : %d\n P : %d\n Number of Repetitions: %d\n Number of Threads: %d \n",
			N, M, P, MATRIX_MULTIPLICATION_TEST_COUNT, MATRIX_MULTIPICATION_NO_OF_THREADS);
	printf("%10s %10s \n", "REPETITION", "TIME(seconds)");

	float** A = random_matrix(N, P);
	float** B = random_matrix(P, M);
	float** C = zero_matrix(N, M);


	struct timespec start, finish;
	double test_time[MATRIX_MULTIPLICATION_TEST_COUNT];
	double total_time = 0, total_square_time = 0, average_time = 0, variance_time = 0;

	for(int i = 0; i < MATRIX_MULTIPLICATION_TEST_COUNT; i++)
	{
		clock_gettime(CLOCK_REALTIME, &start);
		MatrixMultiplicationArgs_t* _args = (MatrixMultiplicationArgs_t*) malloc(sizeof(MatrixMultiplicationArgs_t));
		 _args->A = A;
		 _args->B = B;
		 _args->C = C;
		 _args->n = N;
		 _args->p = P;
		 _args->m = M;
		 multi_threaded_matrix_multiplication2(_args);
		free(_args);
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
		printf("%5d %15.3f \n", (i + 1), time_elapsed);
		fflush(stdout);
	}

	average_time = total_time / MATRIX_MULTIPLICATION_TEST_COUNT;

	for(int i = 0; i < MATRIX_MULTIPLICATION_TEST_COUNT; i++)
	{
		total_square_time += pow(test_time[i] - average_time, 2);
	}

	variance_time = sqrt(total_square_time / MATRIX_MULTIPLICATION_TEST_COUNT);
	//Average time 
	printf("\n Average time %5.3f +/- %5.3f seconds \n", average_time, variance_time);


#ifdef PRINT_RESULT
	print_matrix(A, N, P);
	print_matrix(B, P, M);
	print_matrix(C, N, M);
#endif

	delete_matrix(A, N, P);
	delete_matrix(B, P, M);
	delete_matrix(C, N, M);

	return 0;
}
