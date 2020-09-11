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

N = 32
n = np.arange(0,N,1).T
x1 = 1 + np.cos(((2*np.pi)/16)*n)

Cl = GetBasis(N)
Cl_h = np.conjugate(Cl.T)

alpha = np.dot(Cl_h, x1)


plot_c1 = plt.subplot('111')
plot_c1.stem(n, alpha, 'b', basefmt='k', markerfmt='bo',
             use_line_collection=True)
plot_c1.set_xlabel('n')
plot_c1.set_ylabel(r'$\alpha_l$')
plt.yticks(np.arange(0, 10, step=1))
plt.show()
