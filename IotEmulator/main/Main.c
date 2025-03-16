#include <stdio.h>
#include <string.h>
#include <ESPNOW/espNow.h>
#include <ESPNOW/EspNowHandler.h>
#include <RFID/rfidmanager.h>
#include "Main.h"

#include <esp_timer.h>
#include <DispositivesInterfaces/IRDevices/IRDevices.h>
#include <DispositivesInterfaces/RFDevices/RFDevices.h>
#include <DispositivesInterfaces/RFIDDevices/RFIDDevices.h>
#include <IR/Receive/ReceiveIR.h>
#include <Mqtt/MqttHandler.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "esp_wifi.h"

static const char *TAG = "MAIN";


QueueHandle_t xQueue = NULL; // Definir la variable global


void process_task() {
    command_t receivedCmd;
    while (1) {
        if (xQueueReceive(xQueue, &receivedCmd, portMAX_DELAY)) {
            int64_t time = esp_timer_get_time(); // Tiempo en µs
            switch (receivedCmd.cmd) {
                case CMD_RFID:
                    ESP_LOGI(TAG, "[%lld µs] Recibido RFID: %s", time, receivedCmd.data);
                    processRFIDCommand(receivedCmd.data);
                    break;
                case CMD_IR:
                    ESP_LOGI(TAG, "[%lld µs] Recibido IR: %s", time, receivedCmd.data);
                    processIRCommand(receivedCmd);
                    break;
                case CMD_ESP_NOW:
                    ESP_LOGI(TAG, "[%lld µs] Recibido ESP-NOW: %s", time, receivedCmd.data);
                    processRFCommand(receivedCmd.data);
                    break;
                default:
                    ESP_LOGW(TAG, "Comando desconocido");
            }
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_main(void) {
    xQueue = xQueueCreate(10, sizeof(command_t));
    if (xQueue == NULL) {
        ESP_LOGE("MAIN", "Failed to create queue!");
        return;
    }

    //initRF();
    initRFID();
    initMqtt(receive);


    xTaskCreate(executeReceiveRfid, "receiveRFID", 4096, NULL, 5, NULL);
    xTaskCreate(executeReceiveIR, "receiveIR", 4096, NULL, 5, NULL);
    xTaskCreate(process_task, "processTask", 4096, NULL, 5, NULL);
}
