//
// Created by salman on 10/11/24.
//

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "driver/rmt_tx.h"
#include "driver/rmt_rx.h"
#include "ReceiveIR.h"

#include <Main.h>
#include <string.h>
#define EXAMPLE_IR_RESOLUTION_HZ     1000000 // 1MHz resolution, 1 tick = 1us
#define EXAMPLE_IR_RX_GPIO_NUM       13
#define EXAMPLE_IR_NEC_DECODE_MARGIN 200     // Tolerance for parsing RMT symbols into bit stream

/**
 * @brief NEC timing spec
 */
#define NEC_LEADING_CODE_DURATION_0  9000
#define NEC_LEADING_CODE_DURATION_1  4500
#define NEC_PAYLOAD_ZERO_DURATION_0  560
#define NEC_PAYLOAD_ZERO_DURATION_1  560
#define NEC_PAYLOAD_ONE_DURATION_0   560
#define NEC_PAYLOAD_ONE_DURATION_1   1690
#define NEC_REPEAT_CODE_DURATION_0   9000
#define NEC_REPEAT_CODE_DURATION_1   2250


static const char *IRRX = "IR";
static bool finished = false;

/**
 * @brief Decode RMT symbols into NEC address and command
 */

/**
 * @brief Saving NEC decode results
 */
static uint16_t s_nec_code_address;
static uint16_t s_nec_code_command;

/**
 * @brief Check whether a duration is within expected range
 */
static inline bool nec_check_in_range(uint32_t signal_duration, uint32_t spec_duration) {
    return (signal_duration < (spec_duration + EXAMPLE_IR_NEC_DECODE_MARGIN)) &&
           (signal_duration > (spec_duration - EXAMPLE_IR_NEC_DECODE_MARGIN));
}

/**
 * @brief Check whether a RMT symbol represents NEC logic zero
 */
static bool nec_parse_logic0(rmt_symbol_word_t *rmt_nec_symbols) {
    return nec_check_in_range(rmt_nec_symbols->duration0, NEC_PAYLOAD_ZERO_DURATION_0) &&
           nec_check_in_range(rmt_nec_symbols->duration1, NEC_PAYLOAD_ZERO_DURATION_1);
}

/**
 * @brief Check whether a RMT symbol represents NEC logic one
 */
static bool nec_parse_logic1(rmt_symbol_word_t *rmt_nec_symbols) {
    return nec_check_in_range(rmt_nec_symbols->duration0, NEC_PAYLOAD_ONE_DURATION_0) &&
           nec_check_in_range(rmt_nec_symbols->duration1, NEC_PAYLOAD_ONE_DURATION_1);
}

static bool nec_parse_frame(rmt_symbol_word_t *rmt_nec_symbols) {
    rmt_symbol_word_t *cur = rmt_nec_symbols;
    uint16_t address = 0;
    uint16_t command = 0;
    bool valid_leading_code = nec_check_in_range(cur->duration0, NEC_LEADING_CODE_DURATION_0) &&
                              nec_check_in_range(cur->duration1, NEC_LEADING_CODE_DURATION_1);
    if (!valid_leading_code) {
        return false;
    }
    cur++;
    for (int i = 0; i < 16; i++) {
        if (nec_parse_logic1(cur)) {
            address |= 1 << i;
        } else if (nec_parse_logic0(cur)) {
            address &= ~(1 << i);
        } else {
            return false;
        }
        cur++;
    }
    for (int i = 0; i < 16; i++) {
        if (nec_parse_logic1(cur)) {
            command |= 1 << i;
        } else if (nec_parse_logic0(cur)) {
            command &= ~(1 << i);
        } else {
            return false;
        }
        cur++;
    }
    // save address and command
    s_nec_code_address = address;
    s_nec_code_command = command;

    return true;
}

static bool example_rmt_rx_done_callback(rmt_channel_handle_t channel, const rmt_rx_done_event_data_t *edata,
                                         void *user_data) {
    BaseType_t high_task_wakeup = pdFALSE;
    QueueHandle_t receive_queue = (QueueHandle_t) user_data;
    // send the received RMT symbols to the parser task
    xQueueSendFromISR(receive_queue, edata, &high_task_wakeup);
    return high_task_wakeup == pdTRUE;
}

/**
 * @brief Check whether the RMT symbols represent NEC repeat code
 */
static bool nec_parse_frame_repeat(rmt_symbol_word_t *rmt_nec_symbols) {
    return nec_check_in_range(rmt_nec_symbols->duration0, NEC_REPEAT_CODE_DURATION_0) &&
           nec_check_in_range(rmt_nec_symbols->duration1, NEC_REPEAT_CODE_DURATION_1);
}


/**
 * @brief Decode RMT symbols into NEC scan code and print the result
 */
static void example_parse_nec_frame(rmt_symbol_word_t *rmt_nec_symbols, size_t symbol_num) {
    printf("NEC frame start---\r\n");
    for (size_t i = 0; i < symbol_num; i++) {
        printf("{%d:%d},{%d:%d}\r\n", rmt_nec_symbols[i].level0, rmt_nec_symbols[i].duration0,
               rmt_nec_symbols[i].level1, rmt_nec_symbols[i].duration1);
    }
    printf("---NEC frame end: ");
    // decode RMT symbols
    switch (symbol_num) {
        case 34: // NEC normal frame
            if (nec_parse_frame(rmt_nec_symbols)) {
                printf("Address=%04X, Command=%04X\r\n\r\n", s_nec_code_address, s_nec_code_command);
            }
            break;
        case 2: // NEC repeat frame
            if (nec_parse_frame_repeat(rmt_nec_symbols)) {
                printf("Address=%04X, Command=%04X, repeat\r\n\r\n", s_nec_code_address, s_nec_code_command);
            }
            break;
        default:
            printf("Unknown NEC frame\r\n\r\n");
            break;
    }
}


void executeReceiveIR() {
    static rmt_channel_handle_t rx_channel = NULL;
    static QueueHandle_t receive_queue = NULL;
    static bool initialized = false;

    command_t command;
    command.cmd = CMD_IR;

    if (!initialized) {
        ESP_LOGI(IRRX, "Initializing RMT RX channel...");
        rmt_rx_channel_config_t rx_channel_cfg = {
            .clk_src = RMT_CLK_SRC_DEFAULT,
            .resolution_hz = EXAMPLE_IR_RESOLUTION_HZ,
            .mem_block_symbols = 64,
            .gpio_num = EXAMPLE_IR_RX_GPIO_NUM,
        };

        if (rmt_new_rx_channel(&rx_channel_cfg, &rx_channel) != ESP_OK) {
            ESP_LOGE(IRRX, "Failed to create RMT RX channel");
            vTaskDelete(NULL);
            return;
        }

        receive_queue = xQueueCreate(1, sizeof(rmt_rx_done_event_data_t));
        if (receive_queue == NULL) {
            ESP_LOGE(IRRX, "Failed to create queue");
            vTaskDelete(NULL);
            return;
        }

        rmt_rx_event_callbacks_t cbs = {
            .on_recv_done = example_rmt_rx_done_callback,
        };
        ESP_ERROR_CHECK(rmt_rx_register_event_callbacks(rx_channel, &cbs, receive_queue));

        initialized = true;
    }

    rmt_receive_config_t receive_config = {
        .signal_range_min_ns = 1250,
        .signal_range_max_ns = 12000000,
    };

    rmt_symbol_word_t raw_symbols[64];
    rmt_rx_done_event_data_t rx_data;

    while (true) {
        finished = false;
        ESP_LOGI(IRRX, "Waiting for IR signal...");
        ESP_ERROR_CHECK(rmt_enable(rx_channel));

        ESP_ERROR_CHECK(rmt_receive(rx_channel, raw_symbols, sizeof(raw_symbols), &receive_config));

        if (xQueueReceive(receive_queue, &rx_data, pdMS_TO_TICKS(20000)) == pdPASS) {
            example_parse_nec_frame(rx_data.received_symbols, rx_data.num_symbols);

            // Reservar memoria para el comando
            char *commandToSend = malloc(15);
            snprintf(commandToSend, 15, "%X/%X", s_nec_code_address, s_nec_code_command);
            if (commandToSend != NULL) {
                // Verificación de memoria

                snprintf(command.data, sizeof(command.data), "%s", commandToSend);
                xQueueSend(xQueue, &command, portMAX_DELAY);

                free(commandToSend); // Liberar memoria después de usarla
            } else {
                command.data[0] = '\0';
                ESP_LOGE(IRRX, "Memory allocation failed for commandToSend");
            }

            ESP_LOGI(IRRX, "IR signal received and processed");
            finished = true;
        } else {
            ESP_LOGI(IRRX, "No IR signal received, continuing listening...");
        }

        ESP_ERROR_CHECK(rmt_disable(rx_channel));
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}


bool hasIRReceived() {
    return finished;
}

void initializeReceiveIR(void) {
    s_nec_code_address = 0;
    s_nec_code_command = 0;
    finished = false;
}

char *getIRStatus() {
    if (!finished) {
        ESP_LOGE("IR", "Receive IR signal not finished");
        return NULL;
    }
    char *command = malloc(15);
    if (!command) {
        ESP_LOGE("IR", "Memory allocation failed");
        return NULL;
    }
    sprintf(command, "%X/%X", s_nec_code_address, s_nec_code_command);
    return command;
}

// Llamar a esta función para liberar memoria después de usar getIRStatus()
void deleteIRStatus(char *status) {
    if (status) {
        free(status);
    }
}
