//
// Created by salman on 31/10/24.
//


#include <string.h>
#include "mainRF.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "Receiver/receiver.h"
#include "rf69.h"


#define TAG "MAIN"

#define CONFIG_TRANSMITTER 1

#if CONFIG_RECEIVER

#endif // CONFIG_RECEIVER
static char *message;
static char *response;
static bool finishSend = false;
static bool finishReceive = false;

void receiveRF(void *pvParameter) {
    if (pvParameter == NULL) {
        ESP_LOGE("receiveRF", "pvParameter is NULL");
        vTaskDelete(NULL);
        return;
    }

    RFStatus *rf = pvParameter;
    TickType_t startTime = xTaskGetTickCount(); // Tiempo de inicio
    const TickType_t timeout = pdMS_TO_TICKS(5000); // 5 segundos de timeout

    while (1) {
        esp_err_t result = rx_task(rf->message); // Llama a la función para recibir el mensaje

        // Comprueba el resultado
        if (result == ESP_OK) {
            // Si se recibió un mensaje, puedes realizar otras acciones aquí
            ESP_LOGI(pcTaskGetName(0), "Message received, stopping listen task");
            xTaskNotifyGive(rf->receiveTaskHandle);
            finishReceive = true;
            vTaskDelete(NULL); // Eliminar la tarea actual
        } else {
            // Si no se recibió nada, verifica el timeout
            TickType_t elapsedTime = xTaskGetTickCount() - startTime; // Tiempo transcurrido
            if (elapsedTime >= timeout) {
                ESP_LOGI(pcTaskGetName(0), "Timeout reached, stopping task");
                rf->message = (char *) result;
                xTaskNotifyGive(rf->receiveTaskHandle);
                finishReceive = true;
                vTaskDelete(NULL); // Eliminar la tarea actual si se alcanzó el timeout
            } else {
                // Si no ha alcanzado el timeout, espera un momento y vuelve a intentar
                ESP_LOGI(pcTaskGetName(0), "No message received, retrying...");
                vTaskDelay(pdMS_TO_TICKS(1000)); // Esperar 1 segundo antes de volver a intentar
            }
        }
    }
}


void initRF() {
    if (!init()) {
        ESP_LOGE(TAG, "RFM69 radio init failed");
        while (1) { vTaskDelay(1); }
    }
    ESP_LOGI(TAG, "RFM69 radio init OK!");
    setTxPower(20, true); // range from 14-20 for power, 2nd arg must be true for 69HCW
    ESP_LOGW(TAG, "Set TX power high");
}

void setFrecuency(const float frequency) {
    if (!setFrequency(frequency)) {
        ESP_LOGE(TAG, "setFrequency failed");
        return;
    }
    ESP_LOGI(TAG, "RFM69 radio setFrequency OK!");
}


void setData(char *command, const float frequency) {
    setFrecuency(frequency);
    message = command;
}


uint8_t *getData() {
    return (uint8_t *) response;
}

bool hasFinishReceiving() {
    return finishReceive;
}
