#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<mpi.h>
//#include<unistd.h>
#define N 5

int** Make2DIntArray(int arraySizeX, int arraySizeY) {
int** theArray;
theArray = (int**) malloc(arraySizeX*sizeof(int*));
int i;
for (i = 0; i < arraySizeX; i++)
   theArray[i] = (int*) malloc(arraySizeY*sizeof(int));
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

void init_zeros(int** matrix)
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

void printmat(int** matrix)
{
	int i,j;
	
	for (i=0;i<N;i++)
	{	
		printf("\n");
		for (j=0;j<N;j++)
		{
			printf("%d \t",matrix[i][j]);
		}
	}
	printf("\n");
}

void main(int argc, char *argv[])
{
	int** M1=Make2DIntArray(N,N);
	int** M2=Make2DIntArray(N,N);
	int** Prod=Make2DIntArray(N,N);
	int initialization, rank, size;
	printmat(M1);
	printmat(M2);
	init_zeros(Prod);
	int i,j,k;
	
	initialization=MPI_Init(int argc, char *argv[]);
	if(!initialization)
	{
		printf("\n MPI not initialized");
		return NULL;
	}
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	printf ("\n My rank is: %d", rank);
	printf("\n My size is: %d", size);
	struct timeval start, end;
	gettimeofday(&start, NULL);
	

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

	MPI_Finalize();
	gettimeofday(&end, NULL);

	double delta = ((end.tv_sec  - start.tv_sec) * 1000000u + 
	         end.tv_usec - start.tv_usec) / 1.e6;

	printmat(Prod);
	printf("\nTime spent=%f \n", delta);
	
	
}

