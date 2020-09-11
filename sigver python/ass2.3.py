import numpy as np
import matplotlib.pyplot as plt
import math


def period (theta):
    N = (2*np.pi)/(theta)
    return N

def Signal1 (a, theta, n, phi_1):
    x1 = a * np.cos(theta * n + phi_1)
    return x1

def Signal2 (b, theta, n, phi_2):
    x2 = b * np.sin(theta * n + phi_2)
    return x2

N = period(np.pi/8)

n = (np.arange(0,2*N,1)).T #2N is here to cover the whole domain

x1_1 = Signal1(0.2, np.pi/8, n, np.pi/16)
x1_2 = Signal2(0.8, np.pi/8, n, -(np.pi/3))
x1_3 = x1_1 + x1_2

plt.subplots_adjust(hspace = 1)

plot_x11 = plt.subplot('311')
plot_x11.stem(n, x1_1, 'b', basefmt='k', markerfmt='bo',
             use_line_collection=True)
plot_x11.set_xlabel('n')
plot_x11.set_ylabel(r'$\langle x_{a, \frac{\pi}{16}}[n] \rangle$')
plt.yticks(np.arange(-0.3, 0.31, step=0.1))


plot_x12 = plt.subplot('312')
plot_x12.stem(n, x1_2, 'b', basefmt='k', markerfmt='bo',
             use_line_collection=True)
plot_x12.set_xlabel('n')
plot_x12.set_ylabel(r'$\langle x_{b, -\frac{\pi}{3}}[n] \rangle$')
plt.yticks(np.arange(-1, 1.1, step=0.25))


plot_x13 = plt.subplot('313')
plot_x13.stem(n, x1_3, 'b', basefmt='k', markerfmt='bo',
             use_line_collection=True)
plot_x13.set_xlabel('n')
plot_x13.set_ylabel(r'$\langle x_{a, b, \frac{\pi}{16},-\frac{\pi}{3}}[n] \rangle$')
plt.yticks(np.arange(-1, 1.1, step=0.25))


plt.show()

x1_dft = np.fft.fft(x1_3)
freq = np.fft.fftfreq(x1_dft.size)


plot_x13 = plt.subplot('111')
plot_x13.stem(freq, np.abs(x1_dft), 'b', basefmt='k', markerfmt='bo',
             use_line_collection=True)
plot_x13.set_xlabel('k')
plot_x13.set_ylabel(r'$X[k]$')
plt.show()

print(2*freq[1]*2*np.pi) #multiplied with 2 as it covers 2 periods

p_diff = np.sum(np.abs(x1_3)**2) - 1/(2*N) * np.sum(np.abs(x1_dft)**2)
print(p_diff)


b1 = x1_1 / (math.sqrt(np.sum(x1_1**2)))

v2 = Signal1(0.2, np.pi/8, n, (-7*np.pi)/16)

b2 = v2 - (np.dot(v2, b1)/np.dot(b1, b1))*b1

print(np.dot(b1, b2))

plot_b1 = plt.subplot('211')
plot_b1.stem(n, b1, 'b', basefmt='k', markerfmt='bo',
             use_line_collection=True)
plot_b1.set_xlabel('n')
plot_b1.set_ylabel(r'$\langle b_1 \rangle$')
plt.yticks(np.arange(-0.35, 0.4, step=0.1))

plot_b2 = plt.subplot('212')
plot_b2.stem(n, b2, 'b', basefmt='k', markerfmt='bo',
             use_line_collection=True)
plot_b2.set_xlabel('n')
plot_b2.set_ylabel(r'$\langle b_2 \rangle$')
plt.show()

def Signal (a, b, phi1, phi2):
    s = Signal1(a, np.pi/8, n, phi1) + Signal2(b, np.pi/8, n, phi2)
    return s

par_a = [0.2, 0.11, 0.35, 0.42, 0.14, 0.71, 0.56, 0.17, 0.18, 0.5]
par_b = [0.79, 0.43, 0.09, 0.11, 0.58, 0.18, 0.14, 0.69, 0.71, 0.13]
par_phi1 = [0.02, 0.16, -0.08, 0.15, -0.11, 0.08, 0.02, -0.18, -0.15, 0]
par_phi2 = [-0.03, -0.11, 0.19, 0.08, -0.19, 0.02, 0.18, -0.1, 0.19, 0.08]

seq = np.zeros((10,32))

for i in range(10):
    seq[i] = Signal(par_a[i], par_b[i], par_phi1[i], par_phi2[i])

for i in range(1,11):
    plot_s = plt.subplot(5,2,i)
    plt.subplots_adjust(hspace=0.8)
    plot_s.stem(n, seq[i-1], 'b', basefmt='k', markerfmt='bo',
             use_line_collection=True)
    plot_s.set_xlabel('n')
    plot_s.set_ylabel(r'$\langle x_{%d} \rangle$' %i)
    plt.yticks(np.arange(-1, 1.1, step=0.5))
plt.show()

plt.scatter(par_a, par_b)
plt.xlabel(r'a')
plt.ylabel(r'b')

plt.show()

print(np.ptp(np.degrees(par_phi1)))
print(np.ptp(np.degrees(par_phi2)))

x1 = np.dot(seq, b1)
x2 = np.dot(seq, b2)

x = np.linspace(0, 5, 5)

plt.scatter(x1, x2)
plt.plot(x, x)
plt.xlabel(r'x1')
plt.ylabel(r'x2')
plt.show()

def classify (a, b):
    cls = np.greater(a, b) * 1
    return cls

cls = classify(x1, x2)

print(cls)

theta = np.pi/8

y1 = 0.5 * np.cos(theta * n) + 0.3 * np.sin(theta * n)
y2 = 0.2 * np.cos(theta * n - np.pi/3) + 0.1 * np.sin(theta * n + np.pi/4)
y3 = 0.5 * np.cos(theta * n + np.pi/16) + 0.7 * np.sin(theta * n - np.pi/7)

y1a = np.dot(y1, b1)
y1b = np.dot(y1, b2)
y2a = np.dot(y2, b1)
y2b = np.dot(y2, b2)
y3a = np.dot(y3, b1)
y3b = np.dot(y3, b2)

x = np.linspace(0, 5, 5)

print('Classes of y_1[n], y_2[n], y_3[n]: ',
       classify(y1a, y1b),',', classify(y2a, y2b),',', classify(y3a, y3b))

mw1 = (np.sum(x1*cls)+np.sum(x2*cls))/10
mw0 = (np.sum(x1*(1-cls))+np.sum(x2*(1-cls)))/10 #(1-cls) to flip the bits

print('Distances y_1[n] to the class 1 - class 0: ',
      math.sqrt((y1a-mw1)**2+(y1b-mw1)**2), '-',
      math.sqrt((y1a-mw0)**2+(y1b-mw0)**2))
print('Distances y_2[n] to the class 1 - class 0: ',
      math.sqrt((y2a-mw1)**2+(y2b-mw1)**2), '-',
      math.sqrt((y2a-mw0)**2+(y2b-mw0)**2))
print('Distances y_3[n] to the class 1 - class 0: ',
      math.sqrt((y3a-mw1)**2+(y3b-mw1)**2), '-',
      math.sqrt((y3a-mw0)**2+(y3b-mw0)**2))
