//
// Created by salman on 31/10/24.
//

#include <esp_check.h>
#include <esp_err.h>
#include <picc/rc522_mifare.h>
#include "readRFID.h"

static void dump_block(uint8_t buffer[RC522_MIFARE_BLOCK_SIZE], char *TAG) {
    for (uint8_t i = 0; i < RC522_MIFARE_BLOCK_SIZE; i++) {
        esp_log_write(ESP_LOG_INFO, TAG, "%02" RC522_X " ", buffer[i]);
    }

    esp_log_write(ESP_LOG_INFO, TAG, "\n");
}

uint8_t *readRFID(rc522_handle_t scanner, rc522_picc_t *picc) {
    static const char *TAG = "RFID_rx";
    const uint8_t block_address = 4;
    static uint8_t read_buffer[RC522_MIFARE_BLOCK_SIZE];
    // Hacemos estático para evitar que desaparezca después de la ejecución
    rc522_mifare_key_t key = {
        .value = {RC522_MIFARE_KEY_VALUE_DEFAULT},
    };

    if (rc522_mifare_auth(scanner, picc, block_address, &key) != ESP_OK) {
        ESP_LOGE(TAG, "Auth failed");
        return NULL;
    }

    ESP_LOGI(TAG, "Reading data from block %d", block_address);
    if (rc522_mifare_read(scanner, picc, block_address, read_buffer) != ESP_OK) {
        ESP_LOGE(TAG, "Read failed");
        return NULL;
    }

    ESP_LOGI(TAG, "Data read:");
    dump_block(read_buffer, TAG);

    return read_buffer; // Retornamos el buffer con los datos leídos
}
