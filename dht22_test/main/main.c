#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "rom/ets_sys.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "dht.h"

#define DHT_GPIO 16
#define DHT_TYPE DHT_TYPE_AM2301

void dht_task(void *pvParameters)
{
    float temperature, humidity;

    while (1) {
        if (dht_read_float_data(DHT_TYPE, DHT_GPIO, &humidity, &temperature) == ESP_OK) {
            printf("Temperature: %.1f°C, Humidity: %.1f%%\n", temperature, humidity);
        } else {
            printf("Failed to read data from DHT sensor\n");
        }
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Create DHT task
    xTaskCreate(&dht_task, "dht_task", 2048, NULL, 5, NULL);
}
