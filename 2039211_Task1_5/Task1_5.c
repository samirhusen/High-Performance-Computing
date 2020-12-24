/*******************************************************************************************


The main function below show how we can simulate the process of producing the C and B values 

To compile this file:

	cc Task1_5.c -o Task1_5

Samir Husen - 2020 Dec 20

*******************************************************************************************/


int main(int argc, char** argv)

{
	int A = 1; 
	int B = 2;
 	int C = 1; 
	int D = 4;

	int B1 = A + C;
	int C1 = C;

	B = C1 + D;
	C = B1 + D;
}
