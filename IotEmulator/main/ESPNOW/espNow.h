//
// Created by bnsal on 30/09/2024.
//

#ifndef ESPNOW_H
#define ESPNOW_H
#include "esp_now.h"
#endif //ESPNOW_H

/**
 * @brief Mac of the sync system
 */
static uint8_t peer_mac[ESP_NOW_ETH_ALEN];

/**
 * Function to initilize wifi module
 * @return ESP_OK
 */
esp_err_t init_wifi(void);

/**
 *
 * @param esp_now_info
 * @param data
 * @param data_len
 */
void recv_cb(const esp_now_recv_info_t *esp_now_info, const uint8_t *data, int data_len);

/**
 *
 * @param mac_addr
 * @param status
 */
void send_cb(const uint8_t *mac_addr, esp_now_send_status_t status);


/**
 *
 * @return
 */
esp_err_t init_esp_now(esp_now_recv_cb_t recv);

/**
 *
 * @param peer_addr
 * @return
 */
esp_err_t register_peer(uint8_t *peer_addr);
