#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<mpi.h>
//#include<unistd.h>
#define N 1000
#define FROM_MASTER 1
#define FROM_SLAVE 2

double** Make2DdoubleArray(double arraySizeX, double arraySizeY) {
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
			matrix[i][j]=0.0;
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

void freese(double** ptr)
{
    int i;
     for (i=0;i<N;i++)
        free(ptr[i]);
    free(ptr);
}

void main(int argc, char *argv[])
{
	double** M1=Make2DdoubleArray(N,N);
	double** M2=Make2DdoubleArray(N,N);
	double** Prod=Make2DdoubleArray(N,N);
	int rank, size, dest, extra;
	int slaves, stripsize, rows, offset, master_rows, loopsize;
	double starttime, endtime, timetaken;
	//printmat(M1);
	//printmat(M2);
	init_zeros(Prod);
	int i,j,k;

	
	MPI_Status status;
	MPI_Init(&argc, &argv);
	
	//printf("\n Initialization=%d",initialization);
	/*if(!initialization)
	{
		printf("\n MPI not initialized");
		return NULL;
	}*/
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	//printf ("\n My rank is: %d", rank);
	//printf("\n My size is: %d", size);
	MPI_Bcast(M2[0], N*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	slaves=size-1;
	if (rank==0)
	{
		//printf("\n %d is in town",rank);
		starttime=MPI_Wtime();
		stripsize=N/slaves;
		extra=N%slaves;
		offset=0;
		master_rows=stripsize;

		for (dest=1;dest<=slaves;dest++)
		{
			rows = (dest <= extra) ? stripsize+1 : stripsize;   	
			//printf("\n Sending rows %d, offset %d,  of matrix M1 to slave process %d \n",rows,offset, dest);
			//printf("\n Garbage printing \\m/ ");
			MPI_Send(&offset, 1, MPI_INT, dest, FROM_MASTER, MPI_COMM_WORLD);
			//printf("\n Offset sent");
			MPI_Send(&rows, 1, MPI_INT, dest, FROM_MASTER, MPI_COMM_WORLD);
			//printf("\n Stripsize sent");
			//printf("\n %p \n", &M1[offset][0]);
			MPI_Send(M1[offset], rows*N, MPI_DOUBLE, dest , FROM_MASTER, MPI_COMM_WORLD);
			//printf("\n Matrix strip sent");
			//MPI_Send(M2[0], N*N, MPI_DOUBLE, dest, FROM_MASTER, MPI_COMM_WORLD);
			//printf("\n Matrix 2  sent");
			offset+=rows;
			//printf("\n Offset incremented\n");
		}
		//printf("\n Exiting loop");
		/*loopsize=stripsize;
		for (i=0;i<loopsize;i++)
		{
			for (j=0;j<N;j++)
			{

				for (k=0;k<N;k++)
				{
					Prod[i][j]+=M1[i][k]*M2[k][j];
				}
			}
		}*/
		//printf("\n Done with master matrix multiply");
		for (i=1;i<=slaves;i++)
		{
			MPI_Recv(&offset, 1, MPI_INT, i, FROM_SLAVE, MPI_COMM_WORLD, &status);
			MPI_Recv(&rows, 1, MPI_INT, i, FROM_SLAVE, MPI_COMM_WORLD, &status);
			MPI_Recv(Prod[offset], rows*N, MPI_DOUBLE, i, FROM_SLAVE, MPI_COMM_WORLD, &status);
		}
		//printf("\n Received Rows");
		endtime=MPI_Wtime();
		//printmat(Prod);
		printf("\nTime spent=%f \n", endtime-starttime);
		
		
	}
	if (rank>0)
	{
		//printf("\n Rank %d starting receiveing", rank);
		MPI_Recv(&offset, 1, MPI_INT, 0, FROM_MASTER, MPI_COMM_WORLD, &status);
		//printf("\n Rank %d received offset", rank);
		MPI_Recv(&rows, 1, MPI_INT, 0, FROM_MASTER, MPI_COMM_WORLD, &status);
		//printf("\n Rank %d received rows", rank);
		//printf("\n Slave %d has M1 as %p \n", rank, &M1);
		MPI_Recv(M1[0], rows*N, MPI_DOUBLE, 0 , FROM_MASTER, MPI_COMM_WORLD, &status);
		//printf("\n Rank %d received M1", rank);
		//MPI_Recv(&M2[0], N*N, MPI_DOUBLE, 0, FROM_MASTER, MPI_COMM_WORLD, &status);
		//printf("\n Rank %d starting matrix multiply", rank);
		loopsize=offset+rows;
		for (i=0;i<rows;i++)
		{
			//printf("\n Entering outermost loop");
			for (j=0;j<N;j++)
			{

				for (k=0;k<N;k++)
				{
					//printf("\n Entering innermost loop");
					Prod[i][j]+=M1[i][k]*M2[k][j];
				}
			}
		}
		MPI_Send(&offset, 1, MPI_INT, 0, FROM_SLAVE, MPI_COMM_WORLD);
		MPI_Send(&rows, 1, MPI_INT, 0, FROM_SLAVE, MPI_COMM_WORLD);
		MPI_Send(Prod[0], rows*N, MPI_DOUBLE, 0, FROM_SLAVE, MPI_COMM_WORLD);
	}
	
	MPI_Finalize();
	
	
	freese(M1);
	freese(M2);
	freese(Prod);
	//printmat(Prod);

	
	
}

