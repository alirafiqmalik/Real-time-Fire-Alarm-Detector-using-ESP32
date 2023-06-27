#include "header.h"

static void integerToFloat(int32_t *integer, float *vReal, float *vImag, uint16_t samples)
{
    for (uint16_t i = 0; i < samples; i++)
    {
        vReal[i] = (integer[i] >> 16) / 10.0;
        vImag[i] = 0.0;
    }
}

unsigned int countSetBits(unsigned int n)
{
    unsigned int count = 0;
    while (n)
    {
        count += n & 1;
        n >>= 1;
    }
    return count;
}
// detecting 2 frequencies. Set wide to true to match the previous and next bin as well
bool detectFrequency(unsigned int *mem, unsigned int minMatch, double peak, unsigned int bin1, unsigned int bin2, bool wide)
{
    *mem = *mem << 1;
    if (peak == bin1 || peak == bin2 || (wide && (peak == bin1 + 1 || peak == bin1 - 1 || peak == bin2 + 1 || peak == bin2 - 1)))
    {
        *mem |= 1;
    }
    if (countSetBits(*mem) >= minMatch)
    {
        return true;
    }
    return false;
}

void vCalculateFFT(void *vpParameter)
{

    static int32_t samples[BLOCK_SIZE];
    size_t num_bytes_read;
    esp_err_t err;

    // TickType_t xLastWakeTime1;

    for (;;)
    {

        // xLastWakeTime1 = xTaskGetTickCount(); //update xLastWakeTime
        // Read multiple samples at once and calculate the sound pressure
        err = i2s_read(I2S_PORT,
                       (char *)samples,
                       BLOCK_SIZE, // the doc says bytes, but its elements.
                       &num_bytes_read,
                       portMAX_DELAY); // no timeout

        int samples_read = num_bytes_read / 8;

        Serial.println(portMAX_DELAY / pdMS_TO_TICKS(1));
        // integer to float

        integerToFloat(samples, real, imag, SAMPLES);

        fft.Windowing(FFT_WIN_TYP_FLT_TOP, FFT_FORWARD);
        fft.Compute(FFT_FORWARD);

        unsigned int peak = (int)floor(fft.MajorPeak());
        Serial.print("PEAK: ");
        Serial.println(peak);

        // if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE)
        // {
        //     // We have the semaphore, write to the global variables
        //     portENTER_CRITICAL(&xMux);
            // detecting 1kHz and 1.5kHz
            detectFrequency(&bell, 20, peak, 45, 68, true);
            // detecting frequencies around 3kHz
            detectFrequency(&fireAlarm, 20, peak, 136, 137, true);
        //     portEXIT_CRITICAL(&xMux);
        //     // Release the semaphore
        //     xSemaphoreGive(xSemaphore);
        // }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
    vTaskDelete(NULL);
}

void vWifi(void *vpParameter)
{
    for (;;)
    {

        // if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE)
        // {
        //     // We have the semaphore, read from global variables
        //     portENTER_CRITICAL(&xMux);
            server.handleClient();
            Serial.println("Server");
        //     portEXIT_CRITICAL(&xMux);

        //     xSemaphoreGive(xSemaphore);
        // }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    vTaskDelete(NULL);
}
