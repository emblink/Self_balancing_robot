#include "SPI.h"
#include <Adafruit_GFX.h>
#include "Adafruit_GC9A01A.h"
#include "pinout.h"

Adafruit_GC9A01A tft(TFT_CS, TFT_DC, TFT_RST);

#define EYE_COLOR tft.color565(0, 150, 255)
#define PUPIL_COLOR tft.color565(0, 0, 0)
#define BACKGROUND_COLOR tft.color565(0, 0, 0)

const int EYE_RADIUS = 40;
const int PUPIL_RADIUS = 15;
const int EYE_SPACING = 30;

const int CENTER_X = tft.width() / 2;
const int CENTER_Y = tft.height() / 2;

const int LEFT_EYE_CENTER_X = CENTER_X - EYE_RADIUS - (EYE_SPACING / 2);
const int RIGHT_EYE_CENTER_X = CENTER_X + EYE_RADIUS + (EYE_SPACING / 2);
const int EYE_CENTER_Y = CENTER_Y;

const int PUPIL_MAX_OFFSET = EYE_RADIUS - PUPIL_RADIUS - 10;

int pupil_offset_x = 0;
int pupil_offset_y = 0;

void draw_eye(int center_x, int center_y, int offset_x, int offset_y) {
    tft.fillCircle(center_x, center_y, EYE_RADIUS, EYE_COLOR);
    tft.fillCircle(center_x + offset_x, center_y + offset_y, PUPIL_RADIUS, PUPIL_COLOR);
}

void update_eyes() {
    pupil_offset_x = random(-PUPIL_MAX_OFFSET, PUPIL_MAX_OFFSET + 1);
    pupil_offset_y = random(-PUPIL_MAX_OFFSET, PUPIL_MAX_OFFSET + 1);
    draw_eye(LEFT_EYE_CENTER_X, EYE_CENTER_Y, pupil_offset_x, pupil_offset_y);
    draw_eye(RIGHT_EYE_CENTER_X, EYE_CENTER_Y, pupil_offset_x, pupil_offset_y);
}

void display_task(void *parameter) {
    // #define DISPLAY_PERIOD_MS 100
    #define EYES_ANIMATION_UPDATE_MS 2000
    // const TickType_t xFrequency = pdMS_TO_TICKS(DISPLAY_PERIOD_MS);
    // TickType_t xLastWakeTime;
    // xLastWakeTime = xTaskGetTickCount();
    
    tft.begin();
    tft.setRotation(1);
    tft.fillScreen(BACKGROUND_COLOR);

    for(;;) {
        update_eyes();
        vTaskDelay(EYES_ANIMATION_UPDATE_MS);
        // vTaskDelayUntil(&xLastWakeTime, xFrequency);
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