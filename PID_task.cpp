#include <Adafruit_Sensor.h>
#include <Adafruit_MPU6050.h>
#include <Wire.h>
#include "freertos/task.h"
#define NULL 0

Adafruit_MPU6050 mpu;

#define MPU_SDA D4
#define MPU_SCL D5

#define MOTOR_LEFT_IN_1 D6
#define MOTOR_LEFT_IN_2 D3
#define MOTOR_RIGHT_IN_3 D7
#define MOTOR_RIGHT_IN_4 D9

static void mpu_init()
{
    if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
        while (1) {
            delay(10);
        }
    }

    mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
    mpu.setGyroRange(MPU6050_RANGE_250_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

static void motors_init()
{
    pinMode(MOTOR_LEFT_IN_1, OUTPUT);
    pinMode(MOTOR_LEFT_IN_2, OUTPUT);
    pinMode(MOTOR_RIGHT_IN_3, OUTPUT);
    pinMode(MOTOR_RIGHT_IN_4, OUTPUT);

    digitalWrite(MOTOR_LEFT_IN_1, LOW);
    digitalWrite(MOTOR_LEFT_IN_2, HIGH);
    digitalWrite(MOTOR_RIGHT_IN_3, LOW);
    digitalWrite(MOTOR_RIGHT_IN_4, HIGH);
}

static void process_mpu()
{
    /* Get new sensor events with the readings */
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    /* Print out the values */
    Serial.print("AccelX:");
    Serial.print(a.acceleration.x);
    Serial.print(",");
    Serial.print("AccelY:");
    Serial.print(a.acceleration.y);
    Serial.print(",");
    Serial.print("AccelZ:");
    Serial.print(a.acceleration.z);
    Serial.print(", ");
    Serial.print("GyroX:");
    Serial.print(g.gyro.x);
    Serial.print(",");
    Serial.print("GyroY:");
    Serial.print(g.gyro.y);
    Serial.print(",");
    Serial.print("GyroZ:");
    Serial.print(g.gyro.z);
    Serial.println("");
}

void PID_Task(void *parameter) {
    #define PID_PERIOD_MS 2
    const TickType_t xFrequency = pdMS_TO_TICKS(PID_PERIOD_MS);
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    mpu_init();
    motors_init();

    for(;;) {
        process_mpu();
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

void PID_task_create()
{
    #define PID_TASK_CORE 1
    #define PID_TASK_PRIORITY 6

    xTaskCreatePinnedToCore(
        PID_Task,
        "PID_Task",
        4096,
        NULL,
        PID_TASK_PRIORITY,
        NULL,
        PID_TASK_CORE
    );
}
