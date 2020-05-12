#include <iostream>
#include <ctime>
#include <iomanip>
#include <unistd.h>
#include <omp.h>
#include <pthread.h>
using namespace std;
int** Inicialise_Matrix(int N);
void Free_matrix(double**& matrix);
void Print_matrix(int N, double**& matrix);
void omp_calculation1(int dim_matrix, int count_proc, int**& matrix1, int**& matrix2 , int**& matrix_answer);
void pthread_calculation(int dim_matrix, int count_proc, int**& matrix1, int**& matrix2 , int**& matrix_answer);

struct struct_matrix
{
    int dim_matrix;
    int count_proc;
    int **matrix1;
    int **matrix2;
    int **matrix_answer;
    int i_proc;
};
int** Inicialise_Matrix(int N)
{
	int fMin=0, fMax=10;
	int *data = (int *)malloc(N*N*sizeof(int));
	int **matrix= (int **)malloc(N*sizeof(int*));
	int z;
	z=1;
	for (int i=0; i<N; i++)
        matrix[i] = &(data[N*i]);

    for(int i=0; i<N;i++)
	{
		for(int j=0; j<N;j++)
			matrix[i][j]=rand() % 100;
	}
	return matrix;
}



void Print_matrix(int N, int**& matrix)
{
	cout << left;
	for(int i=0; i<N; i++)
		{
			for(int j=0; j<N; j++)
				{
						cout<<setw(9)<<matrix[i][j]<<"  ";
				}
				cout<<endl;
		}
}
void Free_matrix(int**& matrix)
{
	free(matrix[0]);
	free(matrix);
}

void omp_calculation1(int dim_matrix, int count_proc, int**& matrix1, int**& matrix2 , int**& matrix_answer)
{

       cout<<"OMP\n";
       double start, end;
       start = omp_get_wtime();
       #pragma omp parallel num_threads(count_proc) shared(matrix1, matrix2, matrix_answer) 
           for (int i = 0; i < dim_matrix; ++i)
           {
               for (int j = 0; j < dim_matrix; ++j)
               {
                   matrix_answer[i][j] = matrix1[i][j] + matrix2[i][j];
               }
           }
		#pragma omp barrier
        end = omp_get_wtime();
        cout<<"Time of calculations = "<<end - start<<" c \n";
}

void *pthread_calculation(void *arg)
{


	struct struct_matrix temp = *(struct struct_matrix *)arg;
	for (int i=temp.i_proc; i<temp.dim_matrix; i+=temp.count_proc)
	{
		for (int j=0;j<temp.dim_matrix; j++)
			temp.matrix_answer[i][j]=temp.matrix1[i][j]+temp.matrix2[i][j];
	}
	
	//Print_matrix(temp.dim_matrix, temp.matrix_answer);

}




int main(int argc, char* argv[])
{
	srand(time(NULL));
	int count_proc, dim_matrix, key;

	cout<<"\nEnter size of square matrix  ";
	cin>>dim_matrix;
	cout<<"\nEnter count of process ";
	cin>>count_proc;
	cout<<"\nChoice type of parallel \n[1] OMP \n[2] Pthread\n";
	cin>>key;
	int **matrix1, **matrix2, **matrix_answer;
	matrix1 = Inicialise_Matrix(dim_matrix);
	matrix2 = Inicialise_Matrix(dim_matrix);
	matrix_answer = Inicialise_Matrix(dim_matrix);
	double start, end;
	if (key == 1)
	{
		omp_calculation1(dim_matrix, count_proc, matrix1, matrix2, matrix_answer);
	}
	else if(key == 2)
	{
		cout<<"PTHREAD\n";
		if (count_proc>dim_matrix)
			count_proc=dim_matrix;
		pthread_t threads[dim_matrix];
		struct_matrix *matrix= new struct struct_matrix[count_proc];



		//cout<<"\n"<<(matrix->A[1]);
        int j=1;
        j++;
        start = omp_get_wtime();
        for(int i=0; i<count_proc; i++)
        {
        	matrix[i].dim_matrix=dim_matrix;
			matrix[i].count_proc=count_proc;
			matrix[i].matrix1 = &matrix1[0];
			matrix[i].matrix2 = &matrix2[0];
			matrix[i].matrix_answer = &matrix_answer[0];
        	matrix[i].i_proc=i;
        	pthread_create(&threads[i], NULL, pthread_calculation, (void *)(&matrix[i]));
        }
        for (int i = 0; i < count_proc; i++)
        {
            pthread_join(threads[i], NULL);
        }
		end = omp_get_wtime();
        cout<<"Time of calculations = "<<end - start<<" c \n";
		//pthread_calculation1(dim_matrix, count_proc, matrix1, matrix2, matrix_answer);
	}
	cout<<"\nMatrix A \n";
	Print_matrix(dim_matrix, matrix1);
	cout<<"\nMatrix B \n";
	Print_matrix(dim_matrix, matrix2);
	cout<<"\nMatrix C = A + B \n";
	Print_matrix(dim_matrix, matrix_answer);
}