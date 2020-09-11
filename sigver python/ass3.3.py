import scipy as scipy
from scipy.io import wavfile
from scipy import signal
import numpy as np
import matplotlib.pyplot as plt
import matplotlib as mpl

def spectrogram(signal, fs):
    signal_fft = np.abs(np.fft.fft(signal))
    freq = np.fft.fftfreq(len(signal_fft), d=1.0 / fs)
    freq = np.abs(freq[:signal.shape[0] // 2])
    signal_fft = np.abs(signal_fft[:signal.shape[0] // 2])
    return signal_fft, freq

fs, sig_bad = wavfile.read('./sig_bad.wav')
time = np.linspace(0, (len(sig_bad) / fs), len(sig_bad))
signal_spec, freq = spectrogram(sig_bad, fs)

plt.subplots()
timeplot = plt.axes()
plt.plot(time, sig_bad)
plt.title('Time analysis of the bad signal')
timeplot.set_xlabel('Time (s)')
timeplot.set_ylabel('Signal amplitude')
plt.show()

freqplot = plt.axes()
plt.plot(freq, signal_spec)
plt.title('Frequency analysis of the bad signal')
formatter = mpl.ticker.ScalarFormatter(useMathText=True)
freqplot.yaxis.set_major_formatter(formatter)
freqplot.set_xlabel('Frequency (Hz)')
freqplot.set_ylabel('Magnitude')
plt.xticks(np.arange(0, fs/2 + 250, step=500),rotation='45')
plt.show()


def notchfilter(signal, fs, f, Q):
    w0 = f/(fs/2)
    b, a = scipy.signal.iirnotch(w0, Q)
    return scipy.signal.lfilter(b, a, signal)

def bandpass(signal, fs, N, f0, f1):
  w1 = f0 / (fs / 2)
  w2 = f1 / (fs / 2)
  w = [w1, w2]
  b, a = scipy.signal.butter(N, w, 'bandpass', analog=False, output='ba')
  return scipy.signal.lfilter(b, a, signal)


filtered_seq1 = notchfilter(sig_bad, fs, 275, 1)
filtered_seq2 = notchfilter(filtered_seq1, fs, 1640, 50)
filtered_seq3 = notchfilter(filtered_seq2, fs, 2560, 10)
filtered_seq4 = notchfilter(filtered_seq3, fs, 4600, 10)
filtered_seq5 = notchfilter(filtered_seq4, fs, 6555, 10)
filtered_seq6 = bandpass(filtered_seq5, fs, 2, 200, 3000)

filtered_spec, freq = spectrogram(filtered_seq6, fs)

fs, sig_good = wavfile.read('./sig_good.wav')
good_spec, freq = spectrogram(sig_good, fs)


freqplot1 = plt.axes()
plt.plot(freq, filtered_spec)
plt.title('Frequency analysis of the filtered signal')
formatter = mpl.ticker.ScalarFormatter(useMathText=True)
freqplot1.yaxis.set_major_formatter(formatter)
freqplot1.set_xlabel('Frequency (Hz)')
freqplot1.set_ylabel('Magnitude')
plt.xticks(np.arange(0, fs/2 + 250, step=500),rotation='45')
plt.show()

freqplot2 = plt.axes()
plt.plot(freq, good_spec)
plt.title('Frequency analysis of the good signal')
formatter = mpl.ticker.ScalarFormatter(useMathText=True)
freqplot2.yaxis.set_major_formatter(formatter)
freqplot2.set_xlabel('Frequency (Hz)')
freqplot2.set_ylabel('Magnitude')
plt.xticks(np.arange(0, fs/2 + 250, step=500),rotation='45')
plt.show()


output = np.array(filtered_seq6, 'int16', copy=True)
wavfile.write("sig_filtered.wav", fs, output)