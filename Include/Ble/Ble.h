#ifndef __BLE_H__
#define __BLE_H__

#ifdef __cplusplus
extern "C" {
#endif
typedef void(* ble_event)(int status);
typedef void(* ble_rec_data)(unsigned char*data,int len);

void lib_ble_main_init(void);
void lib_ble_main_exit(void);

int  ble_smt_advertise_adv_data(unsigned char* data, int len);
int  ble_smt_advertise_adv_resp_data(unsigned char* data, int len);

int  ble_smt_send_ble(const char *data, int len);

#ifndef BLE_AIC8800DL

int  ble_smt_set_ble_cb(ble_event ecb,ble_rec_data dcb);

#ifdef __cplusplus
}
#endif

#else

#define LBH_SERVER_HEADER 3 // lbh服务端协议头长度

typedef void (*app_queue_cb)(uint8_t *data, uint16_t length);

typedef enum _host_msg
{
    BLE_INIT_DONE,
    BLE_MSG_DONE,
    BLE_ADV_ENABLE,
    BLE_ADV_DISABLE,
    BLE_SCAN_ENABLE,
    BLE_SCAN_DISABLE,
    BLE_ADV_REPORT,
    BLE_CONNECTION_IND,
    BLE_DISCONNECTED,
    BLE_CONNECTION_PARAM_UPDATE,
    BLE_EXCHANGED_MTU_IND,
    BLE_SMARTCONFIG_DATA_RECV,
    BLE_PKT_SIZE_IND,
    BLE_SERVICE_CHANGED_IND,
    BLE_NTF_SENT_DONE,
    BLE_IND_SENT_DONE,
    BLE_CON_UPDATE_CMP_DONE,
    BLE_HOGPRH_DATA_RECV,
    BLE_LTK_IND,
    BLE_ADDR_IND,
    BLE_UDFC_DATABASE_IND,
    BLE_UDFC_NTF_DATA_RECV,
    BLE_UDFC_IND_DATA_RECV,
    BLE_RSSI_IND,
    BLE_IRK_IND,
    BLE_PSK_IND,
    BLE_HOST_MAX_MSG = 0x7F,
    BT_HOST_MSG = 0xFF,
} host_msg; //lbh服务端消息ID列表

typedef struct
{
    uint8_t msg_id;
    uint16_t len;
    uint8_t *buff;
    /*
    buff内容根据id的不同，数据组成可能是：
    1、数据域(n byte)；
    2、连接索引(1 byte)+数据域(n byte)；
    3、连接索引(1 byte)+服务UUID(16 byte)+数据域(n byte)；
    4、其他
    */
} BLE_HST_MSG;

typedef void (*ble_event)(int status);
typedef void (*ble_rec_data)(unsigned char *data, int len);

struct ble_pkt_size_ind
{
    /// The maximum number of payload octets in TX
    uint16_t max_tx_octets;
    /// The maximum time that the local Controller will take to TX
    uint16_t max_tx_time;
    /// The maximum number of payload octets in RX
    uint16_t max_rx_octets;
    /// The maximum time that the local Controller will take to RX
    uint16_t max_rx_time;
};

/*****************************************************/
// 以下是移植AIC8800的部分ble库函数
/*****************************************************/
/*---------------------------------------------------------------------------
    * app_queue_reg_callback()
    *
    *     register a callback for ble application
    *
    * Parameters:
    *
    *     ble application callback
    *
    * Returns:
    *
    *     void
    */
void app_queue_reg_callback(app_queue_cb cb);
/*---------------------------------------------------------------------------
    * app_ble_gap_name_set_msg_send()
    *
    *     set device name in gap.
    *
    * Parameters:
    *
    *     buff : ptr for name.
    *     len : len for name.
    * Returns:
    *
    *     void
    */
int app_ble_gap_name_set_msg_send(uint8_t *buff, uint8_t len);
/*---------------------------------------------------------------------------
    * app_ble_adv_data_set_msg_send()
    *
    *     set adv data before adv is running.
    *
    * Parameters:
    *
    *     buff : ptr for adv data.
    *            eg: | ad_len | ad_type | ad_value | ad_len | ad_type | ad_value | ....
    *     len : len for adv data.
    *           max len is 29.
    * Returns:
    *
    *     0: success.
    *    -1: send error.
    */
int app_ble_adv_data_set_msg_send(uint8_t *buff, uint8_t len);
/*---------------------------------------------------------------------------
    * app_ble_scan_response_set_msg_send()
    *
    *     set scan response .
    *
    * Parameters:
    *
    *     buff : ptr for scan response.
    *            eg: | ad_len | ad_type | ad_value | ad_len | ad_type | ad_value | ....
    *     len : len for scan response.
    *           max len is 29.
    * Returns:
    *
    *     0: success.
    *    -1: send error.
    */
int app_ble_scan_response_set_msg_send(uint8_t *buff, uint8_t len);
/*---------------------------------------------------------------------------
    * app_ble_adv_param_set_msg_send()
    *
    *     set adv parameter .
    *
    * Parameters:
    *
    *     adv_intv_min : min adv interval. The unit is 0.625 milliseconds
    *     adv_intv_max : max adv interval. The unit is 0.625 milliseconds
    *     max_tx_power : max tx level is 10.
    *
    * Returns:
    *
    *     0: success.
    *    -1: send error.
    */
int app_ble_adv_param_set_msg_send(uint32_t adv_intv_min, uint32_t adv_intv_max, int8_t max_tx_power);
/*---------------------------------------------------------------------------
    * app_ble_adv_stop_msg_send()
    *
    *     update stop adv after adv is running .
    *
    * Parameters:
    *
    * Returns:
    *
    *     0: success.
    *    -1: send error.
    */
int app_ble_adv_stop_msg_send(void);
/*---------------------------------------------------------------------------
    * app_ble_adv_start_msg_send()
    *
    *     update start adv before adv is running .
    *
    * Parameters:
    *
    * Returns:
    *
    *     0: success.
    *    -1: send error.
    */
int app_ble_adv_start_msg_send(void);
/*---------------------------------------------------------------------------
    * app_ble_disconnect_msg_send()
    *
    *     disconnect the connection between lbh_server and remote device .
    *
    * Parameters:
    *
    *     conidx : connection index
    *
    * Returns:
    *
    *     0: success.
    *    -1: send error.
    */
int app_ble_disconnect_msg_send(uint8_t conidx);
/*---------------------------------------------------------------------------
    * app_ble_set_scan_param()
    *
    *     set scan interval and window before scan was starting. .
    *
    * Parameters:
    *
    * Returns:
    *
    *     0: success.
    *    -1: send error.
    */
int app_ble_set_scan_param(uint16_t scan_wd, uint16_t scan_intv);
/*---------------------------------------------------------------------------
    * app_ble_scan_msg_start()
    *
    *     start scan process and will receive adv from remote device .
    *
    * Parameters:
    *
    * Returns:
    *
    *     0: success.
    *    -1: send error.
    */
int app_ble_scan_msg_start(void);
/*---------------------------------------------------------------------------
    * app_ble_scan_msg_stop()
    *
    *     stop scan process  .
    *
    * Parameters:
    *
    * Returns:
    *
    *     0: success.
    *    -1: send error.
    */
int app_ble_scan_msg_stop(void);
/*---------------------------------------------------------------------------
    * app_ble_smartconfig_send_notification()
    *
    *     send notification by UUID in buff.
    *
    * Parameters:
    *
    *      buff : UUID+data
    *      len : len of(UUID+data)
    * Returns:
    *
    *     0: success.
    *    -1: send error.
    */
int app_ble_smartconfig_send_notification(uint8_t *buff, uint16_t len);
/*---------------------------------------------------------------------------
    * app_ble_set_bt_addr()
    *
    *     set local bt addr .if not set, local device will use default mac addr in efuse.
    *
    * Parameters:
    *
    *      addr : bt addr .eg: 0x00,0x01,0x02,0x03,0x04,0x05
    *
    * Returns:
    *
    *     0: success.
    *    -1: send error.
    */
int app_ble_set_bt_addr(uint8_t *addr);
/*---------------------------------------------------------------------------
    * app_ble_get_bt_addr()
    *
    *     get local bt addr .if not set, local device will return default mac addr in efuse.
    *
    * Parameters:
    *
    *      addr : bt addr .eg: 0x00,0x01,0x02,0x03,0x04,0x05
    *
    * Returns:
    *
    *     0: success.
    *    -1: send error.
    */
int app_ble_get_bt_addr();
/*---------------------------------------------------------------------------
    * app_ble_set_data_len()
    *
    *     set packet len in air tx .if not set,default value is 23 byte.
    *
    * Parameters:
    *
    *      conidx
    *      tx_octets  range 0x001B to 0x00FB
    *      tx_time    range 0x0148 to 0x4290
    * Returns:
    *
    *     0: success.
    *    -1: send error.
    */
int app_ble_set_data_len(uint8_t conidx, uint16_t tx_octets, uint16_t tx_time);
/*---------------------------------------------------------------------------
    * app_ble_set_defalut_mtu()
    *
    *     set defalut mtu, must bt set before connected.
    *
    * Parameters:
    *
    *      max_mtu
    * Returns:
    *
    *     0: success.
    *    -1: send error.
    */
int app_ble_set_defalut_mtu(uint16_t max_mtu);
/*---------------------------------------------------------------------------
    * app_ble_get_curr_rssi()
    *
    *     get current rssi, must have existing le connection.
    *
    * Parameters:
    *
    *      conidx
    * Returns:
    *
    *     0: success.
    *    -1: send error.
    */
int app_ble_get_curr_rssi(uint8_t conidx);
/*---------------------------------------------------------------------------
    * app_ble_set_sec_mode()
    *
    *     set security mode.
    *
    * Parameters:
    *
    *      mode
    *        0: no bond
    *        1: just work
    *        2: passkey display
    * Returns:
    *
    *     0: success.
    *    -1: send error.
    */
int app_ble_set_sec_mode(uint8_t mode);
/*---------------------------------------------------------------------------
    * lbh_client_socket_and_msg_cb_init()
    *
    *     initialializes a socket to link the lbh_server socket, and bond the callback function registered in the specified function
    *     app_queue_reg_callback() or app_queue_reg_bt_msg_callback().
    * Parameters:
    *
    *     ble application callback
    *
    * Returns:
    *
    *     void
    */
int lbh_client_socket_and_msg_cb_init(void);

#ifdef __cplusplus
}
#endif

#endif

#endif
