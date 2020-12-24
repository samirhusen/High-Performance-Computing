/************************************************************************************

  The program below implements is the imrpved version of matrix multiplication using single thread
  
  SINGLE THREAD MATRIX MULTIPLICATION IMPROVED 
  
  Samir Husen - 2020 Dec 23
  
  To compile this program:

  cc Single_thread Utils.c -lm -o Single_thread


*************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "config.h"
#include "matrix_utils.h"

#define DIVIDE_SPLIT_LEVEL 7

void single_thread_matrix(float** A, float** B, int n, int p, int m, float** C)
{
	// Here N, P and M are powers of 2
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

		single_threaded_matrix_multiplication(a11, b11, _n, _p, _m, c11_1);
		single_threaded_matrix_multiplication(a11, b12, _n, _p, _m, c12_1);
		single_threaded_matrix_multiplication(a21, b11, _n, _p, _m, c21_1);
		single_threaded_matrix_multiplication(a21, b12, _n, _p, _m, c22_1);

		single_threaded_matrix_multiplication(a12, b21, _n, _p, _m, c11_2);
		single_threaded_matrix_multiplication(a12, b22, _n, _p, _m, c12_2);
		single_threaded_matrix_multiplication(a22, b21, _n, _p, _m, c21_2);
		single_threaded_matrix_multiplication(a22, b22, _n, _p, _m, c22_2);

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
}

int main(int argc, char** argv)
{

	//A[N][P] * B[P][M] = C[N][M]

	printf("SINGLE THREAD MATRIX MULTIPLICATION\n");
	printf(" N : %d\n M : %d\n P : %d\n Number of Repetitions: %d \n", N, M, P, MATRIX_MULTIPLICATION_TEST_COUNT);
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
		single_thread_matrix(A, B, N, P, M, C);
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
