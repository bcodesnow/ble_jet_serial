#ifndef _ble_uart_h
#define _ble_uart_h

#include "stdint.h"
#include "ble_uart_types.h"

#define USED_ON_MICRO 0

/* ------------------------------------------------------------------- */

#if USED_ON_MICRO == 1

#include "bluenrg_types.h"
#include "bluenrg_gatt_server.h"
#include "bluenrg_gap.h"
#include "string.h"
#include "bluenrg_gap_aci.h"
#include "bluenrg_gatt_aci.h"
#include "hci_const.h"
#include "bluenrg_hal_aci.h"
#include "bluenrg_aci_const.h"
#include "hci.h"
#include "hci_le.h"
#include "sm.h"
#include <stdlib.h>

/* Stack Init Modes:*/
#define DISCOVERY_MODE               (1u<<1u)
#define	ADV_MODE                     (1u<<2u)
#define BLE_UART_MODE                (1u<<3u)
#define USE_TX_POOL                  (1u<<4u)
#define BLE_UART_TX_TIMEOUT           100u

#define CHAR_MAX_PAYLOAD              20u
#define INDEX_SIZE                    1u
#define HC_USABLE_PAYLOAD            (CHAR_MAX_PAYLOAD - INDEX_SIZE)
#define POOLED_HUGE_CHUNK_TIMEOUT     10000

#endif // USED ON MICRO

/* ------------------------------------------------------------------- */

/* Simple Protocol: */

/*1st byte:
    CMD BYTE
**/

#define CMD_OK                          0xBB

#define CMD_WRITE_CATCH_SUCCESS         0x03
#define CMD_SET_SHUT_UP                 0x04 // when set on device it should not send any notifications
#define CMD_SET_CONN_MODE   						0x05 // from central msg to receive conn period

#define CMD_SET_LOGGING_MEDIA          0x06 // from central
//#define CMD_TURN_ON_BLE_SENDING         0x07 // from central

#define CMD_REQUEST_SENSORDATA          0x22 // from central it also sends catch success if sd enabled and triggers a save to sd
#define REPLY_START_HUGE_CHUNK          0x08 // from device this is the reply to requestsensordata
#define REPLY_NO_CHUNK_AVAILABLE        0x80 // from device this is the reply to requestsensordata

#define CMD_START_HUGE_CHUNK_ACK_PROC   0x09 // from device
#define CMD_REQUEST_MISSING_PACKAGE     0x0A // from central
#define REPLY_MISSED_PACKAGE            0x0B // from device
#define CMD_HC_ACK_OK                       0x0C // from central
#define CMD_HC_START_OK                     0xDC

#define TRIGGERED                       0x0D
//#define DATA_SAVED_TO_SD                0x0E // used when data is saved to sd... DATA_COLLECTED
//#define SENDING_SENSORDATA_FINISHED     0x0F // emitted when all the files were sent
#define ALIVE                           0x11

#define CMD_SENSORDATA_AVAILABLE            0x10 // emitted by the sensor  data can be requested or saved to sd
#define CMD_CONNECTION_VALID                0xCC

// CMD NUMBER RANGE
//

// REPLY NUMBER RANGE
//

// STREAM NUMBER RANGE
//

//

#define CONN_PARAM_INFO                 0x12 // this is also to be handled as a cmd!

#define CMD_STOP			  								0x14
#define CMD_START   		  							0x15

/* Connection Modes: */
#define SLOW    1u
#define MID     2u
#define FAST    3u
#define UNKNOWN 4u

#define S_MIN   250
#define S_MAX   500
#define S_LAT   0
#define S_SUP   5000

#define M_MIN   15
#define M_MAX   50
#define M_LAT   0
#define M_SUP   2000

#define F_MIN   7.5
#define F_MAX   7.5
#define F_LAT   0
#define F_SUP   300

/* Catch Success: */
#define CATCH 1u
#define DROP  2u
#define FLOP  3u

/* ------------------------------------------------------------------- */

#define IGNORE_LAST_X               0x16 // not implemented yet, would manipulate the file index

#define DIAG_INFO									  0x17
#define DIAG_1_TYPE_LENGTH_TEST			3u

/* ------------------------------------------------------------------- */

#define TS_MSG                          0xAA

/* TS_MSG                               -> byte[1] CMD
 * TIME SYNC MSG FROM SERVER OR CLIENT  -> byte[2] timeStampOnServer / knownProcessingTime
                                        -> byte[3] timeStampOnServer / knownProcessingTime
                                        -> byte[4] timeStampOnServer / knownProcessingTime
                                        -> byte[5] timeStampOnServer / knownProcessingTime
*/

#define TS_CMD_TIMESTAMP_IN_PAYLOAD             1u
#define TS_CMD_LAST_ONE_WAS_GOOD_ONE            2u
#define TS_CMD_ACK                              3u
#define TS_CMD_KNOWN_PROCESSING_TIME_IN_PAYLOAD 4u
#define TS_CMD_SYNC_START                       5u
#define TS_CMD_SYNC_FINISH                      6u


// TODO BOERGI THIS BLOCK HAS NOTHING TO DO HERE is only specific to transmitted data.. move it to write file..
#define TYPE_AUD 1u
#define	TYPE_ACC 2u
#define TYPE_GYR 3u
#define TYPE_MAG 4u
#define TYPE_PRS 5u
#define TYPE_LOG 0xAA

#define TYPE_COUNT 5 // different sensor types

#define FREQ_AUD 8000
#define FREQ_ACC 1000
#define FREQ_GYR 1000
#define FREQ_MAG 100
#define FREQ_PRS 100
// THIS BLOCK HAS NOTHING TO DO HERE



/* ------------------------------------------------------------------- */

#if USED_ON_MICRO==1
#define BLE_POLL_DELAY 8

#define COPY_RECEIVED_TO_BUF (1u<<3u)

typedef struct t_ble_uart_struct
{
    uint8_t state;
    uint32_t len;
    uint8_t rx_data[20];
    uint8_t tx_data[20];
    uint8_t cmd_len;
    uint8_t tx_cmd[20];
    uint32_t Timer1; // tx, pool
    uint32_t Timer2; // connection mode
    uint32_t Timer3; // cmd retry
    uint32_t mode_reg;
    uint8_t bnrg_sta_tx_pool_available;
    uint8_t waiting_for_cmd_ok;
    uint8_t retries;
} t_ble_uart_struct;

typedef struct
{
    uint8_t type;
    uint16_t req_pkg;
    uint8_t state;
}	hc_appr_proc_t;

extern t_ble_uart_struct ble_uart_struct;
extern void ble_uart_timerproc (void);
extern void init_blue_nrg_ms_stack(uint8_t mode_select);

extern void HCI_Event_CB(void *pckt);

extern uint8_t connected;

extern tBleStatus ble_uart_tx (uint8_t *data, uint16_t len);
extern tBleStatus ble_uart_tx_pool (uint8_t *data, uint16_t len);

extern void hci_reinit(void);
extern void ble_uart_tx_stream (uint8_t *data, uint16_t len);

//    extern uint8_t ble_uart_tx_huge_chunk ( uint8_t* buff,	uint16_t bytesToWrite );
extern uint8_t ble_uart_tx_huge_chunk_pool ( uint8_t* buff,	uint16_t bytesToWrite );
extern uint8_t ble_uart_tx_huge_chunk_start ( uint8_t type, uint16_t bytesToWrite, uint16_t arg2, uint8_t channel_count );
//    extern uint8_t ble_uart_tx_huge_chunk_finish ( uint8_t type );
extern uint8_t ble_uart_tx_cmd_ok(void);
extern uint8_t ble_uart_tx_cmd_with_resp( uint8_t* buff, uint8_t len, uint16_t timeout  );
extern void determinate_current_conn_mode(void);
extern uint8_t send_missing_package_hc_appr_proc (uint8_t* ptrToLastBuf, uint16_t packageCnt);

extern uint8_t finish_hc_appr_proc(void);
extern uint8_t send_start_hc_ack_proc(uint8_t* ptr_to_last_buffer);
extern uint8_t send_diag_info (uint8_t type, uint8_t arg1, uint8_t arg2, uint8_t arg3, uint8_t arg4);
extern void handle_ble_upload(void);

extern void update_connection_parameters(void);

#endif

/* ------------------------------------------------------------------- */

#endif
