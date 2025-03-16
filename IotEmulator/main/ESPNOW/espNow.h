//
// Created by bnsal on 30/09/2024.
//

#ifndef ESPNOW_H
#define ESPNOW_H

#include "esp_now.h"

void wifi_init(void);

esp_err_t init_esp_now(esp_now_recv_cb_t recv);

esp_err_t register_peer(uint8_t *peer_addr);

void recv_cb(const esp_now_recv_info_t *esp_now_info, const uint8_t *data, int data_len);

void send_cb(const uint8_t *mac_addr, esp_now_send_status_t status);

void sendESPNowData(const uint8_t *data, size_t len);

#endif //ESPNOW_H
