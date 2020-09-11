import numpy as np
import matplotlib.pyplot as plt

1j #imaginary unit


def GetBasis (L):
    N = L
    B = np . zeros ((N,L), dtype = complex)
    for l in range (L):
        for n in range (N):
            B[n][l] = np.exp(n * 1j * 2 * np.pi * l / N)

    k = 1/np.sqrt(N)
    C = k * B
    A = C.T
    return A

A = GetBasis(8)
A_i = np.linalg.inv(A)
A_h = np.conjugate(A.T)

diff = np.linalg.norm(A_i - A_h)
sq_error = diff**2

print("Squared error: ", sq_error)
