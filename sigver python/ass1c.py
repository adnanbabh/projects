import numpy as np
import matplotlib.pyplot as plt

1j #imaginary unit

N = 32
L = N
B = np.zeros((N,L), dtype = complex)
i = np.arange(0,N,1).T #time index vector


for l in range (L):
    for n  in range (N):
        B[n][l] = np.exp(n * 1j * 2 * np.pi * l / N)

k = 1/np.sqrt(N)
C = k * B

print("Vector Norm c2: ", np.linalg.norm(C[:][1]))
print("Vector Norm c4: ", np.linalg.norm(C[:][3]))
print("Vector Norm cL: ", np.linalg.norm(C[:][31]))


plt.figure(dpi=1000)
plt.figure(figsize=(12,12))

plt.subplots_adjust(left=None, bottom=None,
                    right=None, top=None, wspace=0.3, hspace=0.3)

plot_c1 = plt.subplot('411')
plot_c1.stem(i, np.real(C[:][0]), 'b', label=r'Re{$c_1$}',
             basefmt='k', markerfmt='bo', use_line_collection=True)
plot_c1.stem(i, np.imag(C[:][0]), 'r', label=r'Im{$c_1$}',
             basefmt='k', markerfmt='ro', use_line_collection=True)
plot_c1.set_xlabel('n')
plot_c1.set_ylabel(r'c1[n]')
plt.yticks(np.arange(-0.5, 0.5, step=0.1))
plot_c1.legend()

plot_c2 = plt.subplot('412')
plot_c2.stem(i, np.real(C[:][1]), 'b', label=r'Re{$c_1$}',
             basefmt='k', markerfmt='bo', use_line_collection=True)
plot_c2.stem(i, np.imag(C[:][1]), 'r', label=r'Im{$c_1$}',
             basefmt='k', markerfmt='ro', use_line_collection=True)
plot_c2.set_xlabel('n')
plot_c2.set_ylabel(r'c2[n]')
plt.yticks(np.arange(-0.5, 0.5, step=0.1))
plot_c2.legend()

plot_c3 = plt.subplot('413')
plot_c3.stem(i, np.real(C[:][3]), 'b', label=r'Re{$c_1$}',
             basefmt='k', markerfmt='bo', use_line_collection=True)
plot_c3.stem(i, np.imag(C[:][3]), 'r', label=r'Im{$c_1$}',
             basefmt='k', markerfmt='ro', use_line_collection=True)
plot_c3.set_xlabel('n')
plot_c3.set_ylabel(r'c3[n]')
plt.yticks(np.arange(-0.5, 0.5, step=0.1))
plot_c3.legend()

plot_cN = plt.subplot('414')
plot_cN.stem(i, np.real(C[:][31]), 'b', label=r'Re{$c_1$}',
             basefmt='k', markerfmt='bo', use_line_collection=True)
plot_cN.stem(i, np.imag(C[:][31]), 'r', label=r'Im{$c_1$}',
             basefmt='k', markerfmt='ro', use_line_collection=True)
plot_cN.set_xlabel('n')
plot_cN.set_ylabel(r'cL[n]')
plt.yticks(np.arange(-0.5, 0.5, step=0.1))
plot_cN.legend()
plt.show()



