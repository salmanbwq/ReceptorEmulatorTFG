#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define RC522_SPI_BUS_GPIO_MISO    (19)
#define RC522_SPI_BUS_GPIO_MOSI    (23)
#define RC522_SPI_BUS_GPIO_SCLK    (18)
#define RC522_SCANNER_GPIO_SDA      (21)
#define RC522_SCANNER_GPIO_RST      (-1) // soft-reset
#include <driver/rc522_spi.h>

static rc522_spi_config_t driver_config = {
    .host_id = SPI3_HOST,
    .bus_config = &(spi_bus_config_t){
        .miso_io_num = RC522_SPI_BUS_GPIO_MISO,
        .mosi_io_num = RC522_SPI_BUS_GPIO_MOSI,
        .sclk_io_num = RC522_SPI_BUS_GPIO_SCLK,
    },
    .dev_config = {
        .spics_io_num = RC522_SCANNER_GPIO_SDA,
    },
    .rst_io_num = RC522_SCANNER_GPIO_RST,
};

void executeReceiveRfid();


/***
 * Receive RFID data
 */
static void receiveRFID();

/***
 * Function to initialize RFID
 */
void initRFID();


static bool hasSent();

static uint8_t *getMessageRFID();


static bool hasReceived();
#ifdef __cplusplus
}
#endif
