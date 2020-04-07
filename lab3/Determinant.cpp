#include <stdio.h>
#include <iostream>
#include <ctime>
#include <iomanip>
#include "mpi.h"
using namespace std;
double** Inicialise_Matrix(int N, int flag);
double fRand(double &fMin, double &fMax);
double Determinant_tring_matrix(int N, double**& matrix);
void Cast_to_non_zero_element(int N, int zero_el_current_matrix, double**& matrix);
void Free_matrix(double**& matrix);
void Free_matrix(int N, double**& matrix);
void add_rows_to_matrix(double**& in_matrix, double**& out_matrix, int start_row, int finish_row, int start_coll, int N);
void Cast_first_to_zero(int start_row, int finish_row, int N, int zero_el_current_matrix, double**& matrix);
void Print_matrix(int N, double**& matrix);

int sign = 1;

double** Inicialise_Matrix(int N, int flag=1)
{
	double fMin=0, fMax=10;
	double *data = (double *)malloc(N*N*sizeof(double));
	double **matrix= (double **)malloc(N*sizeof(double*));
	int z;
	z=1;
	for (int i=0; i<N; i++)
        matrix[i] = &(data[N*i]);

    if (flag == 2)
    {
    	for(int i=0; i<N;i++)
		{
			for(int j=0; j<N;j++)
				matrix[i][j]=fRand(fMin, fMax);
		}
    }
    else if(flag == 1)
    {
    	for(int i=0; i<N; i++)
		{
			for(int j=0; j<N; j++)
				{
					matrix[i][j]=z++;
				}
		}
    }
	return matrix;
}

void Cast_to_non_zero_element(int N, int zero_el_current_matrix, double**& matrix)
{
	bool flag = false;
	double temp;
	if(matrix[zero_el_current_matrix][zero_el_current_matrix]!=0)
		return;
	else
	{
		for(int i=zero_el_current_matrix; i<N; i++)
		{
			if(matrix[zero_el_current_matrix][i]!=0)
			{
				for(int j=0;j<N;j++)
				{
					temp = matrix[j][zero_el_current_matrix];
					matrix[j][zero_el_current_matrix] = matrix[j][i];
					matrix[j][i] = temp;
				}
				sign*=(-1);
				flag=true;
				break;
			}
		}
		
		if(!flag)
		{
			cout<<"!!!    Degenerate matrix    !!!\n";
			Print_matrix(N, matrix);
			cout<<endl<<"!!!    Determinant = 0   !!!"<<endl;
			int nothing;
			MPI_Abort(MPI_COMM_WORLD, nothing);
		}
			
	}
}


void Cast_first_to_zero(int start_row, int finish_row, int N, int zero_el_current_matrix, double**& matrix)
{
	double det;
	for(int i=start_row; i<finish_row; i++)
		{
			det = matrix[i][zero_el_current_matrix]/matrix[zero_el_current_matrix][zero_el_current_matrix];
			for(int j=zero_el_current_matrix; j<N; j++)
				{
					matrix[i][j]-=det*matrix[zero_el_current_matrix][j];
				}
		}
}


double fRand(double &fMin, double &fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

void Print_matrix(int N, double**& matrix)
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
void Free_matrix(double**& matrix)
{
	free(matrix[0]);
	free(matrix);
}

double Determinant_tring_matrix(int N, double**& matrix)
{
	double answer=1;
	for (int i=0; i<N; i++)
		answer*=matrix[i][i];
	return answer;
}

void add_rows_to_matrix(double**& in_matrix, double**& out_matrix, int start_row, int finish_row, int start_coll, int N)
{
	for(int i=start_row;i<finish_row;i++)
	{
		for(int j=start_coll; j<N; j++)
			out_matrix[i][j]=in_matrix[i][j];
	}
}

int main(int argc, char* argv[])
{
	srand(time(NULL));
	int N, key;
	N=5;
	MPI_Status Status;
	MPI_Request request;
	int ProcRank, ProcNum;
	double z=1;
	double **matrix, **parent_matrix;
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&ProcNum);
	MPI_Comm_rank(MPI_COMM_WORLD,&ProcRank);
	
	
	if(ProcRank == 0)
	{
		cout<<"\nEnter size of square matrix  ";
		cin>>N;
		cout<<"\nВыберите \n[1] Матрица с последовательным заполнением\n[2] Матрица с рандомным (от 0 до 10) заполнением\n";
		cin>>key;
		cout<<"\nSource matrix\n";		
	}
	MPI_Bcast(&key, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
	matrix= Inicialise_Matrix(N,key);
	if(ProcRank == 0)
	{
		Print_matrix(N, matrix);
		cout<<"\n\n\n\n\n";
	}

	Cast_to_non_zero_element(N, 0, matrix);

	parent_matrix=Inicialise_Matrix(N,key);

	for(int count_size=0;count_size< (N-1);count_size++)
	{
		MPI_Bcast(&(matrix[0][0]), N*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
		int k = (N-count_size)/ (ProcNum-1);
		if (ProcRank==0)
		{
			Cast_to_non_zero_element(N, count_size, matrix);
			
			for(int i=1;i<ProcNum; i++)
				{
					MPI_Recv(&(parent_matrix[0][0]), N*N, MPI_DOUBLE, i, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
					int start_row = count_size + k * (i-1);
					if (start_row == count_size) start_row++;
					int finish_row = count_size + k * i;
					if ( i == ProcNum-1 ) finish_row = N;

					add_rows_to_matrix(parent_matrix, matrix, start_row, finish_row, count_size, N);

				}
		}
		else
		{
			int start_row = count_size + k * (ProcRank-1);
			if (start_row == count_size) start_row++;
			int finish_row = count_size + k * ProcRank;
			if ( ProcRank == ProcNum-1 ) finish_row = N;
			
			Cast_first_to_zero(start_row, finish_row, N, count_size, matrix);
			MPI_Ssend(&(matrix[0][0]), N*N,MPI_DOUBLE,0,0,MPI_COMM_WORLD);
		}
	}
	
    if(ProcRank==0)
    {
    	cout<<"Upper triangular matrix\n";
    	Print_matrix(N, matrix);
    	cout<<"\nDeterminant of matrix = "<<sign*Determinant_tring_matrix(N, matrix)<<endl;
    }
    Free_matrix(matrix);
    Free_matrix(parent_matrix);

	MPI_Finalize();
}