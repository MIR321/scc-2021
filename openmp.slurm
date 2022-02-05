#!/bin/bash
#SBATCH --nodes=1
#SBATCH --tasks-per-node=1
#SBATCH -p tornado
#SBATCH -t 00-00:15:00
#SBATCH -J jobname
#SBATCH -o jobname-%j.out 
if [ -f /etc/profile.d/modules-basis.sh ]; then
source /etc/profile.d/modules-basis.sh
fi

module purge
module load compiler/gcc/9
module load python/3.8

gcc -o openmp openmp.c -fopenmp -std=c99

echo "\n-----------------openmp MATRIX 4000 1 thread------------------"
time ./openmp 4000 1
echo "\n"
python3 ./check.py

echo "\n-----------------openmp MATRIX 4000 2 thread------------------"
time ./openmp 4000 2
echo "\n"
python3 ./check.py

echo "\n-----------------openmp MATRIX 4000 5 thread------------------"
time ./openmp 4000 5
echo "\n"
python3 ./check.py

echo "\n-----------------openmp MATRIX 4000 10 thread------------------"
time ./openmp 4000 10
echo "\n"
python3 ./check.py

echo "\n-----------------openmp MATRIX 4000 20 thread------------------"
time ./openmp 4000 20
echo "\n"
python3 ./check.py

echo "\n-----------------openmp MATRIX 4000 40 thread------------------"
time ./openmp 4000 40
echo "\n"
python3 ./check.py

echo "\n-----------------openmp MATRIX 4000 56 thread------------------"
time ./openmp 4000 56
echo "\n"
python3 ./check.py

echo "\n-----------------openmp MATRIX 4000 70 thread------------------"
time ./openmp 4000 70
echo "\n"
python3 ./check.py

echo "\n-----------------openmp MATRIX 4000 100 thread------------------"
time ./openmp 4000 100
echo "\n"
python3 ./check.py