#include "ble_blink.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "ble_gatts_config.h"

#define BLINK_LED_PIN (2)

void vToggleLed(void *pvParameters);

TaskHandle_t toggleLedHandle = NULL;

uint32_t delay = 1000;

void app_main(void)
{
    gpio_pad_select_gpio(BLINK_LED_PIN);
    gpio_set_direction(BLINK_LED_PIN, GPIO_MODE_OUTPUT);

    init_ble();
    
    xTaskCreatePinnedToCore(vToggleLed, "toggle-LED", configMINIMAL_STACK_SIZE, NULL, 3, &toggleLedHandle, 1);
}


void vToggleLed(void *pvParameters){
    while(1){
        gpio_set_level(BLINK_LED_PIN, 0);
        printf("LED turned on\n");
        vTaskDelay(pdMS_TO_TICKS(delay));

        gpio_set_level(BLINK_LED_PIN, 1);
        printf("LED turned off\n");
        vTaskDelay(pdMS_TO_TICKS(delay));
    }

    vTaskDelete(NULL);
}


void updateBlinkFr(uint8_t command){
    switch(command){
        case 0x00:
            delay = 200;
            break;
        case 0x01:
            delay = 500;
            break;
        case 0x02:
            delay = 2000;
            break;
        case 0x03:
            delay = 1000;
            break;
        default:
            printf("invalid command: %d", command);
            return;
    }
    printf("updated blink delay %d ms", delay);
}