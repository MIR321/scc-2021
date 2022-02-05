import numpy as np
from numpy import save

with open('A', 'r') as f:
    A = [[float(num) for num in line.strip().split(' ')] for line in f]
with open('B', 'r') as f:
    B = [float(num) for num in f.read().strip().split(' ')]
with open('X', 'r') as f:
    X_real = [float(num) for num in f.read().strip().split(' ')]

a = np.array(A)
b = np.array(B)
x = np.linalg.solve(a, b)

with open('X_REAL', 'w') as f:
    for item in x:
        f.write("%s " % item)

if np.allclose(X_real, x, atol=1e-01):
    print("SOLUTION IS CORRECT")
else:
    print("SOLUTION IS NOT CORRECT")