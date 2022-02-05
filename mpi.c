#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <sys/time.h>


#define N 6100  
int matrix_size;
double A[N][N];
double B[N];
double X[N];

struct t_info
{
    int id;
    int start;
};

void init()
{
    int r, c;
    for (c = 0; c < matrix_size; c++)
    {
        for (r = 0; r < matrix_size; r++)
        {
            A[r][c] = (double)rand() / 300.0;
        }
        B[c] = (double)rand() / 300.0;
        X[c] = 0.0;
    }
}

void write_f2(double arr[N][N], char *name)
{
    FILE *fp;
    if ((fp = fopen(name, "wb")) == NULL)
    {
        exit(0);
    }
    for (int i = 0; i < matrix_size; i++)
    {
        for (int j = 0; j < matrix_size; j++)
        {
            fprintf(fp, "%f ", arr[i][j]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}

void write_f(double *arr, char *name)
{
    FILE *fp;
    if ((fp = fopen(name, "wb")) == NULL)
    {
        exit(0);
    }
    for (int i = 0; i < matrix_size; i++)
    {
        fprintf(fp, "%f ", arr[i]);
    }
    fclose(fp);
}

int readmatrix_2(double arr[N][N], const char *filename)
{

    FILE *pf;
    pf = fopen(filename, "r");
    if (pf == NULL)
        return 0;

    for (size_t i = 0; i < matrix_size; ++i)
    {
        for (size_t j = 0; j < matrix_size; ++j)
        {
            fscanf(pf, "%lf", &arr[i][j]);
        }
    }

    fclose(pf);
    return 1;
}

int readmatrix(double *arr, const char *filename)
{

    FILE *pf;
    pf = fopen(filename, "r");
    if (pf == NULL)
        return 0;

    for (size_t i = 0; i < matrix_size; ++i)
    {
        fscanf(pf, "%lf", &arr[i]);
    }

    fclose(pf);
    return 1;
}

void back()
{
    int r, c;
    for (r = matrix_size - 1; r >= 0; r--)
    {
        X[r] = B[r];
        for (c = matrix_size - 1; c > r; c--)
        {
            X[r] -= A[r][c] * X[c];
        }
        X[r] /= A[r][r];
    }
}

int rank; 
int p;      

void forward();

int main(int argc, char **argv)
{

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Comm_size(MPI_COMM_WORLD, &p);
    
    matrix_size = atoi(argv[1]);

    if (rank == 0)
    {
        readmatrix_2(A, "A");
        readmatrix(B, "B");
    }

    forward();

    if (rank == 0)
    {
        back();
        write_f(X, "X");
    }

    MPI_Finalize();
    exit(0);
}

void forward()
{
    int start, r, c, i;
    double mult;

    for (start = 0; start < matrix_size - 1; start++)
    {

        MPI_Bcast(&A[start][0], matrix_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(&B[start], 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        if (rank == 0)
        {
            MPI_Status status1[matrix_size], status2[matrix_size], status3, status4;
            MPI_Request request1[matrix_size], request2[matrix_size];
            for (i = 1; i < p; i++)
            {
                for (r = start + 1 + i; r < matrix_size; r += p)
                {
                    MPI_Isend(&A[r], matrix_size, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &request1[r]);
                    MPI_Isend(&B[r], 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &request2[r]);
                }
            }

            for (r = start + 1; r < matrix_size; r += p)
            {
                mult = A[r][start] / A[start][start];
                for (c = start; c < matrix_size; c++)
                {
                    A[r][c] -= A[start][c] * mult;
                }
                B[r] -= B[start] * mult;
            }

            for (i = 1; i < p; i++)
            {
                for (r = start + 1 + i; r < matrix_size; r += p)
                {
                    MPI_Wait(&request1[r], &status1[r]);
                    MPI_Wait(&request2[r], &status2[r]);
                    MPI_Recv(&A[r], matrix_size, MPI_DOUBLE, i, 1, MPI_COMM_WORLD, &status3);
                    MPI_Recv(&B[r], 1, MPI_DOUBLE, i, 1, MPI_COMM_WORLD, &status4);
                }
            }
        }
        else
        {
            MPI_Status status1;
            MPI_Status status2;

            for (r = start + 1 + rank; r < matrix_size; r += p)
            {
                MPI_Recv(&A[r], matrix_size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status1);
                MPI_Recv(&B[r], 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status2);

                mult = A[r][start] / A[start][start];
                for (c = start; c < matrix_size; c++)
                {
                    A[r][c] -= A[start][c] * mult;
                }
                B[r] -= B[start] * mult;

                MPI_Send(&A[r], matrix_size, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
                MPI_Send(&B[r], 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
            }
        }
        MPI_Barrier(MPI_COMM_WORLD); 
    }
}