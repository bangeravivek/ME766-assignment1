#include<stdio.h>
#include<stdlib.h>
#include<time.h>
//#include<unistd.h>
#define N 5000

double** Make2DdoubleArray(int arraySizeX, int arraySizeY) {
double** theArray;
theArray = (double**) malloc(arraySizeX*sizeof(double*));
int i;
for (i = 0; i < arraySizeX; i++)
   theArray[i] = (double*) malloc(arraySizeY*sizeof(double));
int j;

for (i=0;i<arraySizeX;i++)
{
    for (j=0;j<arraySizeY;j++)
    {
        theArray[i][j]=rand()%100;
    }
}

   return theArray;
}

void init_zeros(double** matrix)
{
	int i,j;
	for (i=0;i<N;i++)
	{
		for (j=0;j<N;j++)
		{
			matrix[i][j]=0;
		}
	}
}

void printmat(double** matrix)
{
	int i,j;
	
	for (i=0;i<N;i++)
	{	
		printf("\n");
		for (j=0;j<N;j++)
		{
			printf("%f \t",matrix[i][j]);
		}
	}
	printf("\n");
}

void main()
{
	double** M1=Make2DdoubleArray(N,N);
	double** M2=Make2DdoubleArray(N,N);
	double** Prod=Make2DdoubleArray(N,N);
	
	//printmat(M1);
	//printmat(M2);
	init_zeros(Prod);
	int i,j,k;
	
	struct timeval start, end;
	gettimeofday(&start, NULL);
	
	#pragma omp parallel for private(i,j,k)
	for (i=0;i<N;i++)
	{
		//printf("Thread rank: %d\n", omp_get_thread_num());
		
		for (j=0;j<N;j++)
		{

			for (k=0;k<N;k++)
			{
				Prod[i][j]+=M1[i][k]*M2[k][j];
			}
		}
	}

	gettimeofday(&end, NULL);

	double delta = ((end.tv_sec  - start.tv_sec) * 1000000u + 
	         end.tv_usec - start.tv_usec) / 1.e6;

	//printmat(Prod);
	printf("\nTime spent=%f \n", delta);
	
	
}

