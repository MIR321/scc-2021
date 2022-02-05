import sys
from mpi4py import MPI
import numpy as np

N = 20000  
n = 0
p = 0
rank = 0
A = []
B = []
X = []
COMM = MPI.COMM_WORLD

def back():
    for r in range(n-1,-1,-1):
        X[r] = B[r]
        for c in range(n-1, r, -1):
            X[r] -= A[r][c] * X[c]
        X[r] /= A[r][r]

def forward():

    print(COMM.Get_rank())

    for start in range(0, n-1):

        request1 = [0 for _ in range(n)]
        request2 = [0 for _ in range(n)]
        COMM.Bcast(A[start], root=0)
        COMM.Bcast(B[start], root=0)

        if rank == 0:
            for i in range(1, p):
                for r in range(start + 1 + i, n, p):
                    request1[r] = COMM.Isend(A[r], dest=i)
                    request2[r] = COMM.Isend(B[r], dest=i)

            for r in range(start+1, n, p):
                mult = A[r][start] / A[start][start]
                A[r] -= A[start] * mult
                B[r] -= B[start] * mult

            for i in range(1, p):
                for r in range(start+1+i, n, p):
                    request1[r].wait()
                    request2[r].wait()
                    COMM.Recv(A[r], source=i)
                    COMM.Recv(B[r], source=i)
        else:
            for r in range(start+1+rank, n, p):
                COMM.Recv(A[r], source=0)
                COMM.Recv(B[r], source=0)
                mult = A[r][start] / A[start][start]
                A[r] -= A[start] * mult
                B[r] -= B[start] * mult
                COMM.Send(A[r], dest=0)
                COMM.Send(B[r], dest=0)
        COMM.barrier() 

def main():
    global n
    global rank
    global p
    global A
    global B
    global X

    rank = COMM.Get_rank()
    p = COMM.Get_size()

    np.random.seed(0)
    n = int(sys.argv[1])
    A = np.zeros([n, n])
    B = np.zeros(n).reshape([n,1])
    X = np.zeros(n)

    if rank == 0:    
        with open('A', 'r') as f:
            A = np.array([[float(num) for num in line.strip().split(' ')] for line in f])
        with open('B', 'r') as f:
            B = np.array([float(num) for num in f.read().strip().split(' ')]).reshape([n,1])

    forward()

    if rank == 0:
        back()
        with open('X', 'w') as fh:
            for x in X:
                fh.write(f'{x} ')

    MPI.Finalize()
main()