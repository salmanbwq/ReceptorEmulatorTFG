//
// Created by salman on 14/02/25.
//
#include "freertos/FreeRTOS.h"
#include "EspNowHandler.h"
#include <esp_err.h>
#include <esp_log.h>
#include <Main.h>
#include <portmacro.h>
#include <stdio.h>
#include <string.h>
#include <esp32p4/rom/ets_sys.h>
#include <ESPNOW/espNow.h>
#include <freertos/queue.h>


void receive(const esp_now_recv_info_t *esp_now_info, const uint8_t *data, int data_len) {
    printf("Recibiendo \n");
    ESP_LOGI("ESP_RECV", "Data recibida desde " MACSTR ": %s", MAC2STR(esp_now_info->src_addr), (const char *)data);
    command_t cmd;
    cmd.cmd = CMD_ESP_NOW;
    char received_data[50];
    strncpy(received_data, (const char *) data, sizeof(received_data) - 1);
    received_data[sizeof(received_data) - 1] = '\0';
    snprintf(cmd.data, sizeof(cmd.data), "ESP-NOW: %s", (char *) received_data);
    xQueueSend(xQueue, &cmd, portMAX_DELAY);
    vTaskDelete(NULL);
}

esp_err_t esp_now_send_data(const uint8_t *peer_addr, const uint8_t *data, size_t len) {
    esp_now_send(peer_addr, data, len);
    return ESP_OK;
}
