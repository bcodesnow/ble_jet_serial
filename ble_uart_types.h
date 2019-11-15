#ifndef __BLE_UART_TYPES_H
#define __BLE_UART_TYPES_H

#include "stdint.h"

typedef struct conn_param_info_s {
    uint8_t  requested_mode;
    uint8_t  current_mode;
    uint16_t interval;
    uint16_t supervision;
    uint8_t  latency;
    uint8_t reserved;
} __attribute__((packed)) conn_param_info_t;

typedef struct huge_chunk_start_s {
    uint16_t incoming_byte_count;
    uint16_t incoming_package_count;
    uint8_t  incoming_type;
    uint16_t write_pointer;
    uint8_t  channel_count;
} __attribute__((packed)) huge_chunk_start_t;

typedef struct request_missing_pkg_s {
    uint16_t pkg_id;
} __attribute__((packed)) request_missing_pkg_t;

typedef struct ble_uart_cmd_s {
    uint8_t  cmd_code;
    uint8_t  payload[19];
} __attribute__((packed)) ble_uart_cmd_t;

typedef struct alive_msg_s {
    uint16_t mainState;
    uint8_t reserved;
    uint8_t lastError;
    uint8_t isSendingOverBLEenabled;
    uint8_t isSavingToSDenabled;
    uint8_t isSDMounted;
    uint16_t currentFileIndex;
}__attribute__((packed)) alive_msg_t;


//typedef struct request_sensor_data_s {
//    uint8_t  requested_mode;
//} __attribute__((packed)) request_sensor_data_t;

#endif
