#ifndef __MAGIC_CONNECT_H__
#define __MAGIC_CONNECT_H__

#include "mbed.h"

Serial output(USBTX, USBRX);

#define ETHERNET        1
#define WIFI_ESP8266    2
#define MESH_LOWPAN_ND  3
#define MESH_THREAD     4

// #if MBED_CONF_APP_NETWORK_INTERFACE == WIFI_ESP8266
// #include "ESP8266Interface.h"

// #ifdef MBED_CONF_APP_ESP8266_DEBUG
// ESP8266Interface esp(MBED_CONF_APP_ESP8266_TX, MBED_CONF_APP_ESP8266_RX, MBED_CONF_APP_ESP8266_DEBUG);
// #else
// ESP8266Interface esp(MBED_CONF_APP_ESP8266_TX, MBED_CONF_APP_ESP8266_RX);
// #endif

// #elif MBED_CONF_APP_NETWORK_INTERFACE == ETHERNET
#include "EthernetInterface.h"
EthernetInterface eth;
// #elif MBED_CONF_APP_NETWORK_INTERFACE == MESH_LOWPAN_ND
// #define MESH
// #include "NanostackInterface.h"
// LoWPANNDInterface mesh;
// #elif MBED_CONF_APP_NETWORK_INTERFACE == MESH_THREAD
// #define MESH
// #include "NanostackInterface.h"
// ThreadInterface mesh;
// #else
// #error "No connectivity method chosen. Please add 'config.network-interfaces.value' to your mbed_app.json (see README.md for more information)."
// #endif

// #if defined(MESH)
// #if MBED_CONF_APP_MESH_RADIO_TYPE == ATMEL
// #include "NanostackRfPhyAtmel.h"
// NanostackRfPhyAtmel rf_phy(ATMEL_SPI_MOSI, ATMEL_SPI_MISO, ATMEL_SPI_SCLK, ATMEL_SPI_CS,
//                            ATMEL_SPI_RST, ATMEL_SPI_SLP, ATMEL_SPI_IRQ, ATMEL_I2C_SDA, ATMEL_I2C_SCL);
// #elif MBED_CONF_APP_MESH_RADIO_TYPE == MCR20
// #include "NanostackRfPhyMcr20a.h"
// NanostackRfPhyMcr20a rf_phy(MCR20A_SPI_MOSI, MCR20A_SPI_MISO, MCR20A_SPI_SCLK, MCR20A_SPI_CS, MCR20A_SPI_RST, MCR20A_SPI_IRQ);
// #endif //MBED_CONF_APP_RADIO_TYPE
// #endif //MESH

// #ifndef MESH
// // This is address to mbed Device Connector
// #define MBED_SERVER_ADDRESS "coap://api.connector.mbed.com:5684"
// #else
// // This is address to mbed Device Connector
// #define MBED_SERVER_ADDRESS "coaps://[2607:f0d0:2601:52::20]:5684"
// #endif

NetworkInterface* easy_connect(bool log_messages = false) {
    NetworkInterface* network_interface = 0;
    int connect_success = -1;
// #if MBED_CONF_APP_NETWORK_INTERFACE == WIFI_ESP8266
//     if (log_messages) {
//         output.printf("[EasyConnect] Using WiFi (ESP8266) \r\n");
//         output.printf("[EasyConnect] Connecting to WiFi..\r\n");
//     }
//     connect_success = esp.connect(MBED_CONF_APP_ESP8266_SSID, MBED_CONF_APP_ESP8266_PASSWORD);
//     network_interface = &esp;
// #elif MBED_CONF_APP_NETWORK_INTERFACE == ETHERNET
    if (log_messages) {
        output.printf("[EasyConnect] Using Ethernet\r\n");
    }
    connect_success = eth.connect();
    network_interface = &eth;
// #endif
// #ifdef MESH
//     if (log_messages) {
//         output.printf("[EasyConnect] Using Mesh\r\n");
//         output.printf("[EasyConnect] Connecting to Mesh..\r\n");
//     }
//     connect_success = mesh.connect();
//     network_interface = &mesh;
// #endif
    if(connect_success == 0) {
        if (log_messages) {
            output.printf("[EasyConnect] Connected to Network successfully\r\n");
        }
    } else {
        if (log_messages) {
            output.printf("[EasyConnect] Connection to Network Failed %d!\r\n", connect_success);
        }
        return NULL;
    }
    if (log_messages) {
        const char *ip_addr = network_interface->get_ip_address();
        if (ip_addr) {
            output.printf("[EasyConnect] IP address %s\r\n", ip_addr);
        } else {
            output.printf("[EasyConnect] No IP address\r\n");
        }
    }
    return network_interface;
}

#endif // __MAGIC_CONNECT_H__
