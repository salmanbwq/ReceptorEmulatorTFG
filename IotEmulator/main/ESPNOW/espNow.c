//
// Created by bnsal on 30/09/2024.
//
#include "esp_wifi.h"
#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "esp_now.h"
#include "esp_netif.h"
#include "esp_mac.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "espNow.h"

#define ESP_CHANNEL 1

//  ADDR ESP32
//static uint8_t peer_mac[ESP_NOW_ETH_ALEN] = {0x40, 0x22, 0xd8, 0x1e, 0x6c, 0x9c};

//  ADDR LCD
static uint8_t peer_mac[ESP_NOW_ETH_ALEN] = {0x24, 0xdc, 0xc3, 0x49, 0x5e, 0x10};


 esp_err_t init_wifi(void) {
    wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();

    esp_netif_init();
    esp_event_loop_create_default();
    nvs_flash_init();
    esp_wifi_init(&wifi_init_config);
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_start();

    ESP_LOGI("WIFI_INIT", "WIfi_init inicializado");
    return ESP_OK;
}

void recv_cb(const esp_now_recv_info_t *esp_now_info, const uint8_t *data, int data_len) {
    ESP_LOGI("ESP_RECV", "Data recibida" MACSTR "%s", MAC2STR(esp_now_info->src_addr), data);
}

void send_cb(const uint8_t *mac_addr, esp_now_send_status_t status) {
    if (status == ESP_NOW_SEND_SUCCESS) {
        ESP_LOGI("ESP_SEND", "ESP_NOW_SEND_SUCCESS");
    } else {
        ESP_LOGI("ESP_SEND", "ESP_NOW_SEND_FAIL");
    }
}

esp_err_t init_esp_now(esp_now_recv_cb_t recv) {
    esp_now_init();
    esp_now_register_recv_cb(recv);
    esp_now_register_send_cb(send_cb);

    ESP_LOGI("ESP_NOW_INIT", "Esp_NOW inicializado");
    return ESP_OK;
}

esp_err_t register_peer(uint8_t *peer_addr) {
    esp_now_peer_info_t esp_now_peer_info = {};
    memcpy(esp_now_peer_info.peer_addr, peer_mac, ESP_NOW_ETH_ALEN);
    esp_now_peer_info.channel = ESP_CHANNEL;
    esp_now_peer_info.ifidx = ESP_IF_WIFI_STA;
    esp_now_add_peer(&esp_now_peer_info);
    return ESP_OK;
}

