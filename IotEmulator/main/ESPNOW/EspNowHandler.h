//
// Created by salman on 14/02/25.
//

#ifndef ESPNOWHANDLER_H
#define ESPNOWHANDLER_H
#include <esp_err.h>
#include <esp_now.h>


static uint8_t lcd[ESP_NOW_ETH_ALEN] = {0x24, 0xdc, 0xc3, 0x49, 0x5e, 0x10};

esp_err_t esp_now_send_data(const uint8_t *peer_addr, const uint8_t *data, size_t len);

void receive(const esp_now_recv_info_t *esp_now_info, const uint8_t *data, int data_len);

#endif //ESPNOWHANDLER_H
