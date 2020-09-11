import numpy as np
import matplotlib.pyplot as plt

N = 32

n = ((np.arange(0,N,1)).reshape(1,N)).T

x1 = 1 + np.cos(((2*np.pi)/16)*n)

x2 = (np.zeros([1,N],dtype=np.complex_)).T
x2[2,0] = 0 + 1j

x1_h = np.conjugate(x1.T)
x2_h = np.conjugate(x2.T)

p_x1x2h = np.dot(x1, x2_h);

print("Dimension of x1: ", x1.shape)
print("Dimensions of x2^H: ", x2_h.shape)
print("Dimension of the product between x1 and x2^H: ",
      p_x1x2h.shape)

y1 = np.dot(x1_h, x2)
y2 = np.dot(x2_h, x1)

print(y1)
print(y2)

plt.figure(1)

plot_x1 = plt.subplot ('111')
plot_x1.stem(n, x1, 'b', basefmt='k', markerfmt='bo',
             use_line_collection=True)
plot_x1.set_xlabel('n')
plot_x1.set_ylabel(r'$x_1[n]$')
plt.show()

plt.figure(2)

plt.subplots_adjust(hspace = 1)

plot_x2 = plt.subplot('411')
plot_x2.stem(n,np.real(x2), 'b', basefmt='k', markerfmt='bo',
             use_line_collection=True)
plot_x2.set_xlabel('n')
plot_x2.set_ylabel(r'Re{$x_2[n]$}')
plt.yticks(np.arange(0.0, 2.0, step=0.5))

plot_x2 = plt.subplot('412')
plot_x2.stem(n,np.imag(x2), "r", basefmt="k", markerfmt='ro',
             use_line_collection=True)
plot_x2.set_xlabel('n')
plot_x2.set_ylabel(r'Im{$x_2[n]$}')
plt.yticks(np.arange(0.0, 2.0, step=0.5))

plot_x2 = plt.subplot('413')
plot_x2.stem(n,np.abs(x2), "c", basefmt="k", markerfmt='co',
             use_line_collection=True)
plot_x2.set_xlabel('n')
plot_x2.set_ylabel(r'|$x_2[n]$|')
plt.yticks(np.arange(0.0, 2.5, step=0.5))

plot_x2 = plt.subplot('414')
plot_x2.stem(n,np.angle(x2)/np.pi*180, "m", basefmt="k",
             markerfmt='mo', use_line_collection=True)
plot_x2.set_xlabel('n')
plot_x2.set_ylabel(r'Phase{$x_2[n]$} [°]')
plt.yticks(np.arange(0, 100, step=45))

plt.show()


