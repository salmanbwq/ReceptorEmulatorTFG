//
// Created by bnsal on 30/09/2024.
//
//#include "esp_wifi.h"
//#include <stdio.h>
//#include <string.h>
//#include "esp_log.h"
//#include "esp_now.h"
//#include "esp_netif.h"
//#include "esp_mac.h"
//#include "esp_event.h"
//#include "nvs_flash.h"
//#include "espNow.h"
//
//#include <Mqtt/MqttHandler.h>
//
//#define ESP_CHANNEL 1
//
////  ADDR ESP32
////static uint8_t peer_mac[ESP_NOW_ETH_ALEN] = {0x40, 0x22, 0xd8, 0x1e, 0x6c, 0x9c};
//
////  ADDR LCD
//static uint8_t peer_mac[ESP_NOW_ETH_ALEN] = {0x24, 0xdc, 0xc3, 0x49, 0x5e, 0x10};
//
//void force_espnow_channel() {
//    ESP_LOGI("ESP_NOW", "🔧 Configurando ESP-NOW en canal 11...");
//    ESP_ERROR_CHECK(esp_wifi_set_channel(11, WIFI_SECOND_CHAN_NONE));
//}
//
//
//void recv_cb(const esp_now_recv_info_t *esp_now_info, const uint8_t *data, int data_len) {
//    ESP_LOGI("ESP_RECV", "Data recibida" MACSTR "%s", MAC2STR(esp_now_info->src_addr), data);
//}
//
//void send_cb(const uint8_t *mac_addr, esp_now_send_status_t status) {
//    if (status == ESP_NOW_SEND_SUCCESS) {
//        ESP_LOGI("ESP_SEND", "ESP_NOW_SEND_SUCCESS");
//    } else {
//        ESP_LOGI("ESP_SEND", "ESP_NOW_SEND_FAIL");
//    }
//}
//
//esp_err_t init_esp_now(esp_now_recv_cb_t recv) {
//    esp_now_init();
//    esp_now_register_recv_cb(recv);
//    esp_now_register_send_cb(send_cb);
//
//    force_espnow_channel();
//
//    ESP_LOGI("ESP_NOW_INIT", "Esp_NOW inicializado");
//    return ESP_OK;
//}
//
//esp_err_t register_peer(uint8_t *peer_addr) {
//    esp_now_peer_info_t peerInfo = {};
//    memcpy(peerInfo.peer_addr, peer_addr, ESP_NOW_ETH_ALEN);
//
//    // 🔥 Usa el mismo canal de WiFi STA para evitar problemas
//    peerInfo.channel = 1;
//    peerInfo.ifidx = ESP_IF_WIFI_STA;
//    peerInfo.encrypt = false;
//
//    if (esp_now_is_peer_exist(peer_addr)) {
//        ESP_ERROR_CHECK(esp_now_mod_peer(&peerInfo));
//    } else {
//        ESP_ERROR_CHECK(esp_now_add_peer(&peerInfo));
//    }
//
//    ESP_LOGI("ESP-NOW", "Peer registrado en canal %d", getChannel());
//    return ESP_OK;
//}
//
