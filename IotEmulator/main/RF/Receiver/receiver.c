//
// Created by salman on 31/10/24.
//

#include <esp_log.h>
#include "freertos/FreeRTOS.h"
#include <esp_task_wdt.h>
#include "receiver.h"
#include <rf69.h>
#include <string.h>
#include <freertos/task.h>

static bool finish = false;

esp_err_t rx_task(void *pvParameter) {
    ESP_LOGI(pcTaskGetName(0), "Listening for messages...");
    esp_task_wdt_deinit(); // Deshabilita el WDT para tareas
    ESP_LOGI(pcTaskGetName(0), "Waiting for task to start...");

    // Comprobar si hay un mensaje disponible
    if (available()) {
        uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
        uint8_t len = sizeof(buf);
        ESP_LOGI(pcTaskGetName(0), "Receiving messages...");
        if (recv(buf, &len)) {
            if (len > 0) {
                buf[len] = 0; // Añadir terminador de cadena
                ESP_LOGI(pcTaskGetName(0), "Received [%d]: %s", len, (char *)buf);
                ESP_LOGI(pcTaskGetName(0), "RSSI: %d", lastRssi());

                // Asegurarse de que pvParameter tenga suficiente espacio
                if (len < RH_RF69_MAX_MESSAGE_LEN) {
                    pvParameter = malloc((len + 1) * sizeof(uint8_t));
                    strncpy((char *) pvParameter, (char *) buf, len + 1); // Copiar el mensaje recibido
                } else {
                    ESP_LOGE(pcTaskGetName(0), "Received message is too long");
                    finish = true;
                    return ESP_FAIL; // Mensaje demasiado largo para el búfer
                }
                finish = true;
                return ESP_OK; // Retorna éxito si se recibe un mensaje
            }
        } else {
            ESP_LOGE(pcTaskGetName(0), "Receive failed");
            finish = true;
            return ESP_FAIL; // Retorna fallo si no se puede recibir
        }
    }


    ESP_LOGI(pcTaskGetName(0), "No message available");

    vTaskDelay(10 / portTICK_PERIOD_MS); // 10 ms de delay

    return ESP_FAIL; // Si no hay mensajes disponibles
}
