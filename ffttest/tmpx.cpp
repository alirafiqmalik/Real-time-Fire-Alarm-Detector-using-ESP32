#include <iostream>
// #include <stdio.h>
// #include <stdint.h>
#include <math.h>
using namespace std;

#define M_PI 3.14159265358979323846

typedef struct {
    float* data;
    uint32_t length;
} signal_t;

typedef struct {
    float* data;
    uint32_t rows;
    uint32_t cols;
} matrix_t;

// Calculate the Mel filterbank matrix
matrix_t mel_filterbank(uint32_t num_bins, uint32_t sample_rate, uint32_t num_filters) {
    matrix_t mel_filters = {
        .data =(float *) malloc(num_filters * num_bins * sizeof(float)),
        .rows = num_filters,
        .cols = num_bins
    };
    if (mel_filters.data == NULL) {
        printf("Error allocating memory for Mel filterbank matrix\n");
        exit(1);
    }

    // Convert frequencies to Mel scale
    float fft_freqs[num_bins];
    for (uint32_t i = 0; i < num_bins; i++) {
        fft_freqs[i] = i * sample_rate / num_bins;
    }
    float mel_freqs[num_bins];
    for (uint32_t i = 0; i < num_bins; i++) {
        mel_freqs[i] = 1127 * log(1 + fft_freqs[i] / 700);
    }

    // Calculate the lower and upper limits of each Mel filter
    float mel_limits = mel_freqs[num_bins - 1] / num_filters;
    float mel_low[num_filters];
    float mel_high[num_filters];
    for (uint32_t i = 0; i < num_filters; i++) {
        mel_low[i] = i * mel_limits;
        mel_high[i] = mel_low[i] + mel_limits;
    }

    // Calculate the Mel filterbank matrix
    for (uint32_t i = 0; i < num_filters; i++) {
        // Calculate the filter coefficients
        float lower = 700 * (exp(mel_low[i] / 1127) - 1) / (sample_rate / 2);
        float upper = 700 * (exp(mel_high[i] / 1127) - 1) / (sample_rate / 2);
        for (uint32_t j = 0; j < num_bins; j++) {
            mel_filters.data[i * num_bins + j] = (fft_freqs[j] >= lower && fft_freqs[j] <= upper) ? 1 : 0;
        }
    }

    return mel_filters;
}

// Calculate the Mel spectrogram from a generated FFT
matrix_t mel_spectrogram(matrix_t fft, uint32_t sample_rate, uint32_t num_mel_filters) {
    // Calculate the Mel filterbank matrix
    matrix_t mel_filters = mel_filterbank(fft.cols, sample_rate, num_mel_filters);

    // Calculate the Mel spectrogram
    matrix_t mel_spec = {
        .data = (float *)malloc(fft.rows * num_mel_filters * sizeof(float)),
        .rows = fft.rows,
        .cols = num_mel_filters
    };
    if (mel_spec.data == NULL) {
        printf("Error allocating memory for Mel spectrogram\n");
        exit(1);
    }
    for (uint32_t i = 0; i < mel_spec.rows; i++) {
        // Calculate the power spectrum
        float power_spectrum[fft.cols];
        for (uint32_t j = 0; j < fft.cols; j++) {
            power_spectrum[j] = fft.data[i * fft.cols + j] * fft.data[i * fft.cols + j] + fft.data[i * fft.cols + j + 1] * fft.data[i * fft.cols + j + 1];
        }

        // Calculate the Mel spectrum
        float mel_spectrum[num_mel_filters];
        for (uint32_t j = 0; j < num_mel_filters; j++) {
            mel_spectrum[j] = 0;
            for (uint32_t k = 0; k < fft.cols; k++) {
                mel_spectrum[j] += power_spectrum[k] * mel_filters.data[j * fft.cols + k];
            }
        }

        // Store the Mel spectrum in the Mel spectrogram
        for (uint32_t j = 0; j < num_mel_filters; j++) {
            mel_spec.data[i * num_mel_filters + j] = mel_spectrum[j];
        }
    }

    return mel_spec;
}



// Set the size of the FFT and the number of Mel filters
#define NUM_BINS 256
#define NUM_MEL_FILTERS 32



int main(){


  // Generate dummy FFT data
float real[NUM_BINS];
float imag[NUM_BINS];
for (int i = 0; i < NUM_BINS; i++) {
    real[i] = (float) rand() / RAND_MAX;
    imag[i] = (float) rand() / RAND_MAX;
}

// Create a matrix to store the FFT data
float fft[NUM_BINS][2];

// Store the real and imaginary parts of the FFT in the matrix
for (int i = 0; i < NUM_BINS; i++) {
    fft[i][0] = real[i];
    fft[i][1] = imag[i];
}


// Set the sample rate of the audio signal (in Hz)
uint32_t sample_rate = 16000;

// Calculate the Mel spectrogram
matrix_t mel_spec = mel_spectrogram((matrix_t) {
    .data = (float *) fft,
    .rows = NUM_BINS,
    .cols = 2
}, sample_rate, NUM_MEL_FILTERS);

  // Print the Mel spectrogram
for (int i = 0; i < mel_spec.rows; i++) {
    for (int j = 0; j < mel_spec.cols; j++) {
        printf("%f ", mel_spec.data[i * mel_spec.cols + j]);
    }
    printf("\n");
}

 
 return 0; 
}