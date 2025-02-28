//
// Created by salman on 14/02/25.
//


#include <Main.h>
#include <string.h>
#include <ESPNOW/EspNowHandler.h>

#include "IRmanager.h"
#include "Receive/ReceiveIR.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/rmt_tx.h"


void executeReceiveIR() {
    command_t command;
    command.cmd = CMD_IR;
    initializeReceiveIR();
    receiveIR();

    while (!hasIRReceived()) {
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }


    result = getIRStatus();


    if (result != NULL) {
        ESP_LOGI("IRManager", "IR Status: %p", result);
        snprintf(command.data, sizeof(command.data), "IR: %s", result);
        xQueueSend(xQueue, &command, portMAX_DELAY);
        deleteIRStatus(result); // Liberar memoria después de usarla
    } else {
        ESP_LOGI("IRManager", "Nothing received");
    }
    vTaskDelay(pdMS_TO_TICKS(1000)); // Espera antes de la siguiente lectura
}
