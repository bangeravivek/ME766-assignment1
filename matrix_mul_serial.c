#include<stdio.h>
#include<stdlib.h>
#include<time.h>
//#include<unistd.h>
#define N 1000

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

void main()
{
	int** M1=Make2DIntArray(N,N);
	int** M2=Make2DIntArray(N,N);
	int** Prod=Make2DIntArray(N,N);
	
	//printmat(M1);
	//printmat(M2);
	init_zeros(Prod);
	int i,j,k;
	clock_t begin, end;
	double time_spent;

	begin = clock();
	for (i=0;i<N;i++)
	{
		for (j=0;j<N;j++)
		{
			for (k=0;k<N;k++)
			{
				Prod[i][j]+=M1[i][k]*M2[k][j];
			}
		}
	}
	end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("\nTime spent=%f", time_spent);
	
	//printmat(Prod);
}

