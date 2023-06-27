#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

#include "header.h"
#include "index.h" //Web page header file

WebServer server(80);

// Enter your SSID and PASSWORD
const char *ssid = "espwifi";
const char *password = "123456789";

// size of noise sample
#define SAMPLES 1024
const i2s_port_t I2S_PORT = I2S_NUM_0;
const int BLOCK_SIZE = SAMPLES;

// our FFT data
static float real[SAMPLES];
static float imag[SAMPLES];


static unsigned int bell = 0;
static unsigned int fireAlarm = 0;

static arduinoFFT fft(real, imag, SAMPLES, SAMPLES);

// static SemaphoreHandle_t xSemaphore;
// static portMUX_TYPE xMux = portMUX_INITIALIZER_UNLOCKED;

void vhandleRoot()
{
    String s = MAIN_page;             // Read HTML contents
    server.send(200, "text/html", s); // Send web page
}

void vhandleMSG()
{
    String msg;

    if (bell > 0 & fireAlarm > 0)
    {
        msg = "Detected fire alarm and bell";
    }
    else if (bell)
    {
        msg = "Detected bell";
    }
    else if (fireAlarm)
    {
        msg = "Detected fire alarm";
    }
    else
    {
        msg = "No Alarm Detected";
    }

    server.send(200, "text/plane", msg); // Send MSG value only to client ajax request
}




void setup(void)
{
    Serial.begin(115200);

    // WiFi.mode(WIFI_AP); //Access Point mode
    // WiFi.softAP(ssid, password);

    WiFi.mode(WIFI_STA); // Connectto your wifi
    WiFi.begin(ssid, password);

    // Wait for WiFi to connect
    while (WiFi.waitForConnectResult() != WL_CONNECTED)
    {
        Serial.print(".");
    }

    // If connection successful show IP address in serial monitor
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP()); // IP address assigned to your ESP

    server.on("/", vhandleRoot);   // This is display page
    server.on("/MSG", vhandleMSG); // To get update of MSG Value only

    server.begin(); // Start server
    Serial.println("HTTP server started");

    Serial.println("Configuring I2S...");
    esp_err_t err;
    // The I2S config as per the example
    const i2s_config_t i2s_config = {
        .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX), // Receive, not transfer
        .sample_rate = 22627,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT, // for old esp-idf versions use RIGHT
        .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1, // Interrupt level 1
        .dma_buf_count = 8,                       // number of buffers
        .dma_buf_len = BLOCK_SIZE,                // samples per buffer
        .use_apll = true};
    // The pin config as per the setup
    const i2s_pin_config_t pin_config = {
        .bck_io_num = 14,   // BCKL
        .ws_io_num = 15,    // LRCL
        .data_out_num = -1, // not used (only for speakers)
        .data_in_num = 32   // DOUTESP32-INMP441 wiring
    };
    // Configuring the I2S driver and pins.
    // This function must be called before any I2S driver read/write operations.

    err = i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);

    if (err != ESP_OK)
    {
        Serial.printf("Failed installing driver: %d\n", err);
        while (true)
            ;
    }
    err = i2s_set_pin(I2S_PORT, &pin_config);
    if (err != ESP_OK)
    {
        Serial.printf("Failed setting pin: %d\n", err);
        while (true)
            ;
    }
    Serial.println("I2S driver installed.");

    // xSemaphore = xSemaphoreCreateBinary();

    // // Check if the semaphore was created successfully
    // if (xSemaphore != NULL)
    // {
        Serial.println("Binary Semaphore Created.");
        xTaskCreatePinnedToCore(vCalculateFFT, "TASK A", 2024, NULL, 1, NULL, 0);
        xTaskCreatePinnedToCore(vWifi, "TASK B", 2024, NULL, 1, NULL, 1);
    // }

    // xPortStartScheduler()
}


void loop(void)
{
}
