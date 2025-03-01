//
// Created by salman on 29/10/24.
//
#include <esp_check.h>
#include <esp_log.h>
#include <../components/rc522/include/rc522_types.h>
#include <stdio.h>
#include <time.h>
#include <../components/rc522/include/driver/rc522_spi.h>
#include <../components/rc522/include/picc/rc522_mifare.h>
#include <../components/rc522/include/rc522.h>
#include "rfidmanager.h"

#include <Main.h>

#include "Read/readRFID.h"
static const char *TAG = "RFID";


static rc522_driver_handle_t driver;
static rc522_handle_t scanner;
static bool has_received = false;
static uint8_t *receivedDataBuffer;

static void receive_RFID(void *arg, esp_event_base_t base, int32_t event_id, void *data) {
    if (has_received) return; // Evitar múltiples lecturas

    rc522_picc_state_changed_event_t *event = (rc522_picc_state_changed_event_t *) data;
    rc522_picc_t *picc = event->picc;

    if (picc->state != RC522_PICC_STATE_ACTIVE) return;

    rc522_picc_print(picc);

    if (!rc522_mifare_type_is_classic_compatible(picc->type)) {
        ESP_LOGW(TAG, "Card is not supported");
        return;
    }

    receivedDataBuffer = readRFID(scanner, picc);
    if (receivedDataBuffer != NULL) {
        ESP_LOGI(TAG, "Read success");
        has_received = true;
    } else {
        ESP_LOGE(TAG, "Read failed");
    }

    rc522_mifare_deauth(scanner, picc);
}


static void receiveRFID() {
    has_received = false;
    receivedDataBuffer = NULL;
    rc522_register_events(scanner, RC522_EVENT_PICC_STATE_CHANGED, receive_RFID, NULL);
}


static uint8_t *getMessageRFID() {
    return receivedDataBuffer;
}


static bool hasReceived() {
    return has_received;
}


void initRFID() {
    srand(time(NULL)); // Initialize random generator

    rc522_spi_create(&driver_config, &driver);
    rc522_driver_install(driver);

    rc522_config_t scanner_config = {
        .driver = driver,
    };

    rc522_create(&scanner_config, &scanner);


    rc522_start(scanner);
}

void executeReceiveRfid() {
    while (1) {
        command_t command;
        command.cmd = CMD_RFID;
        uint8_t *callback = NULL;

        receiveRFID();
        while (!hasReceived()) {
            vTaskDelay(pdMS_TO_TICKS(10)); // Espera no bloqueante
        }

        callback = getMessageRFID();
        if (callback != NULL) {
            snprintf(command.data, sizeof(command.data), "%s", (char *) callback);
            if (xQueue != NULL) {
                xQueueSend(xQueue, &command, pdMS_TO_TICKS(1000));
            } else {
                ESP_LOGE("QUEUE", "xQueue is NULL, cannot send message!");
            }
        } else {
            ESP_LOGI("ESP_NOW", "Received null ack");
        }

        vTaskDelay(pdMS_TO_TICKS(1000)); // Espera antes de la siguiente lectura
    }
}
