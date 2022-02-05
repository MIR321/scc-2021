#!/bin/bash
#SBATCH --nodes=4
#SBATCH -p tornado
#SBATCH -t 00-00:50:00
#SBATCH -J jobname
#SBATCH -o jobname-%j.out 

module purge
module load compiler/gcc/7
module load mpi/openmpi/4.0.1/gcc/7
module load python/3.8

ulimit -s 10240
mpicc -c mpi.c
mpicc -o mpi mpi.o

echo "\n-----------------INIT MATRIX 4000x4000------------------"
./initm 4000


echo "\n-----------------mpi MATRIX 1------------------"
time mpirun -npernode 1 --mca btl openib,self,vader --mca btl_openib_allow_ib 1 -v mpi 4000
echo "\n"
python3 ./check.py

echo "\n-----------------mpi MATRIX 2------------------"
time mpirun -npernode 2 --mca btl openib,self,vader --mca btl_openib_allow_ib 1 -v mpi 4000
echo "\n"
python3 ./check.py

echo "\n-----------------mpi MATRIX 5------------------"
time mpirun -npernode 5 --mca btl openib,self,vader --mca btl_openib_allow_ib 1 -v mpi 4000
echo "\n"
python3 ./check.py

echo "\n-----------------mpi MATRIX 10------------------"
time mpirun -npernode 10 --mca btl openib,self,vader --mca btl_openib_allow_ib 1 -v mpi 4000
echo "\n"
python3 ./check.py

echo "\n-----------------mpi MATRIX 20------------------"
time mpirun -npernode 20 --mca btl openib,self,vader --mca btl_openib_allow_ib 1 -v mpi 4000
echo "\n"
python3 ./check.py

echo "\n-----------------mpi MATRIX 40------------------"
time mpirun -npernode 40 --mca btl openib,self,vader --mca btl_openib_allow_ib 1 -v mpi 4000
echo "\n"
python3 ./check.py
