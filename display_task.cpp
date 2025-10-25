#include "SPI.h"
#include <Adafruit_GFX.h>
#include "Adafruit_GC9A01A.h"

// XIAO S3
#define TFT_RST D0
#define TFT_DC D1
#define TFT_CS D2
// TFT_MOSI D10
// TFT_SCK D8

Adafruit_GC9A01A tft(TFT_CS, TFT_DC, TFT_RST);

void display_task(void *parameter) {
    #define DISPLAY_PERIOD_MS 200
    const TickType_t xFrequency = pdMS_TO_TICKS(DISPLAY_PERIOD_MS);
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    
    tft.begin();

    for(;;) {
        tft.fillScreen (rand() % 0xFFFF);
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

void display_task_create()
{
    #define DISPLAY_TASK_CORE 1
    #define DISPLAY_TASK_PRIORITY 2

    xTaskCreatePinnedToCore(
        display_task,
        "Display_Task",
        4096,
        NULL,
        DISPLAY_TASK_PRIORITY,
        NULL,
        DISPLAY_TASK_CORE
    );
}