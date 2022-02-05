#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <sys/time.h>


#define N 20000  
int n;
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
    for (c = 0; c < n; c++)
    {
        for (r = 0; r < n; r++)
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
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
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
    for (int i = 0; i < n; i++)
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

    for (size_t i = 0; i < n; ++i)
    {
        for (size_t j = 0; j < n; ++j)
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

    for (size_t i = 0; i < n; ++i)
    {
        fscanf(pf, "%lf", &arr[i]);
    }

    fclose(pf);
    return 1;
}

void back()
{
    int r, c;
    for (r = n - 1; r >= 0; r--)
    {
        X[r] = B[r];
        for (c = n - 1; c > r; c--)
        {
            X[r] -= A[r][c] * X[c];
        }
        X[r] /= A[r][r];
    }
}