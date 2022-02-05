#include "common.h"
#include <omp.h>

void forward();

int main(int argc, char **argv) {
   
    n = atoi(argv[1]);
    int threads = atoi(argv[2]);
    omp_set_num_threads(threads);
    readmatrix_2(A, "A");
    readmatrix(B, "B");
    forward();
    back();
    write_f(X, "X");
}


void forward() {
    int start, r, c;  
    double mult;

    for (start = 0; start < n - 1; start++) {
        #pragma omp parallel for shared(A,B,start,n) private(r,mult,c) default(none)
        for (r = start + 1; r < n; r++) {
            mult = A[r][start] / A[start][start];
            for (c = start; c < n; c++) {
                A[r][c] -= A[start][c] * mult;
            }
            B[r] -= B[start] * mult;
        }
    }
}


