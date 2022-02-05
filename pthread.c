#include "common.h"
#include <pthread.h>

int threads = 3;

void *eliminate(void *t)
{
    struct t_info info = *((struct t_info *)t);
    int r, c;
    double mult;
    int id = info.id + 1;
    int start = info.start;
    for (r = id + start; r < n; r += threads)
    {
        mult = A[r][start] / A[start][start];
        for (c = start; c < n; c++)
        {
            A[r][c] -= A[start][c] * mult;
        }
        B[r] -= B[start] * mult;
    }
}

void forward()
{
    pthread_t pthreads[threads];
    struct t_info *param = malloc(threads * sizeof(struct t_info));
    int start;
    for (start = 0; start < n - 1; start++)
    {
        int id;
        for (id = 0; id < threads; id++)
        {
            param[id].id = id;
            param[id].start = start;
            pthread_create(&pthreads[id], NULL, eliminate, &param[id]);
        }
        for (id = 0; id < threads; id++)
        {
            pthread_join(pthreads[id], NULL);
        }
    }
    free(param);
}

int main(int argc, char **argv)
{
    srand(1);
    n = atoi(argv[1]);
    threads = atoi(argv[2]);
    readmatrix_2(A, "A");
    readmatrix(B, "B");

    forward();
    back();
    write_f(X, "X");
}
