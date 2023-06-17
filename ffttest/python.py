import numpy as np
from scipy.signal import hann
import scipy
import scipy.io.wavfile
N = 1024 # Length of the input signal

import numpy as np

# Read the WAV file
sample_rate, audio_data = scipy.io.wavfile.read('/mnt/d/alis files/Coding/Program/cpp/DSA/ffttest/f2bjrop1.0.wav')

print(sample_rate, audio_data)

# Convert the audio data to a NumPy array
signal = np.array(audio_data[:N], dtype=np.float32)



# Calculate the Hanning window
window = hann(N)

# Apply the Hanning window to the input signal
signal *= window


# Calculate the FFT result
fft_result = np.fft.rfft(signal)


N = 256 # Length of the FFT
real = fft_result.real
imag = fft_result.imag


import numpy as np

import numpy as np

def mel_filterbank(num_bins, sample_rate, num_filters):
    """Calculate the Mel filterbank matrix.
    
    Args:
        num_bins: The number of FFT bins.
        sample_rate: The sample rate of the signal, in Hz.
        num_filters: The number of Mel filters to use.
        
    Returns:
        The Mel filterbank matrix, as a 2D NumPy array with shape (num_filters, num_bins).
    """
    # Convert frequencies to Mel scale
    fft_freqs = np.fft.rfftfreq(num_bins, d=1/sample_rate)
    mel_freqs = hz_to_mel(fft_freqs)
    
    # Calculate the lower and upper limits of each Mel filter
    mel_limits = mel_freqs[-1] / num_filters
    mel_low = np.arange(0, mel_freqs[-1], mel_limits)
    mel_high = mel_low + mel_limits
    
    # Initialize the Mel filterbank matrix
    mel_filters = np.empty((num_filters, num_bins), dtype=np.float32)
    
    # Calculate the Mel filterbank matrix
    for i in range(num_filters):
        # Calculate the filter coefficients
        lower = mel_to_hz(mel_low[i]) / (sample_rate / 2)
        upper = mel_to_hz(mel_high[i]) / (sample_rate / 2)
        mel_filters[i] = np.linspace(lower, upper, num_bins)
        
    return mel_filters

def hz_to_mel(frequencies):
    """Convert frequencies to the Mel scale."""
    return 1127 * np.log(1 + frequencies / 700)

def mel_to_hz(mels):
    """Convert frequencies from the Mel scale to Hz."""
    return 700 * (np.exp(mels / 1127) - 1)
















# # Compute the power spectrum
# power_spectrum = (real ** 2 + imag ** 2).astype(np.float32)

# # Convert the power spectrum to the Mel scale
# mel_spectrum = 1127 * np.log(1 + np.arange(N) / 700.0)

# # Compute the MFCCs using a discrete cosine transform (DCT)
# num_coefficients = 20
# mfccs = np.empty((num_coefficients,), dtype=np.float32)
# for i in range(num_coefficients):
#     mfccs[i] = np.sum(mel_spectrum * np.cos(np.pi * i * (np.arange(N) + 0.5) / N))

# print(mel_spectrum)


# import matplotlib.pyplot as plt





# # Create a figure and axis
# # fig, ax = plt.subplots()

# # # Plot the MFCCs
# # ax.plot(mfccs)

# # # Set the x-axis labels
# # ax.set_xlabel('MFCC coefficient')

# # # Set the y-axis labels
# # ax.set_ylabel('Magnitude')

# # # Show the plot
# # plt.show()
