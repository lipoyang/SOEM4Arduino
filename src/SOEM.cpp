#include <stdarg.h>
#include <stdio.h>
#include <Arduino.h>
#include "osal/osal_defs.h" // EC_DEBUG

#if defined(__cplusplus)
extern "C" {
#endif
// just for debug (not thread safe)
void debug_print(const char* format, ...)
{
    static char buff[1024]; // TODO non-reentrant
    
    va_list args;
    va_start( args, format );
    vsnprintf( buff, sizeof(buff), format, args );
    va_end(args);

    Serial.print(buff);
}
#ifdef __cplusplus
}
#endif

#if defined(GRSAKURA)
/**************************************************
  for GR-SAKURA
**************************************************/

#include "rx63n/iodefine.h"
//#include <Ethernet.h>
#if defined(__cplusplus)
extern "C" {
#endif
#include "utility/driver/r_ether.h"
#include "utility/driver/r_ether_local.h"
#ifdef __cplusplus
}
#endif

#define ETHER_BUFSIZE_MIN 60
#define MY_MAC_ADDR    0x02,0x00,0x00,0x00,0x00,0x00
static uint8_t myethaddr[6] = {MY_MAC_ADDR};

// polling Ethernet instead of interrupt. (TODO This is a bad trick.)
static void ethernet_poll(void)
{
    uint32_t status_ecsr = ETHERC.ECSR.LONG;
    uint32_t status_eesr = EDMAC.EESR.LONG;
                                      
    /* When the ETHERC status interrupt is generated */
    if (status_eesr & EMAC_ECI_INT)
    {
        /* When the Magic Packet detection interrupt is generated */
        if (status_ecsr & EMAC_MPD_INT)
        {
            //g_ether_MpdFlag = ETHER_FLAG_ON;
        }
        /*
         * Because each bit of the ECSR register is cleared when one is written, 
         * the value read from the register is written and the bit is cleared. 
         */
        /* Clear all ETHERC status BFR, PSRTO, LCHNG, MPD, ICD */
        ETHERC.ECSR.LONG = status_ecsr;
    }
    EDMAC.EESR.LONG  = status_eesr; /* Clear EDMAC status bits */
}

#ifdef __cplusplus
extern "C"
{
#endif

// (1) open
// return: 0=SUCCESS
int  hal_ethernet_open(void)
{
	int eth_ret;
    int result = R_ETHER_Open_ZC2(0, myethaddr);
    IEN(ETHER, EINT) = 0;
    
    do //TODO allow for timeout
    {
        eth_ret = R_Ether_CheckLink_ZC(0);
        delay(10);
    }
    while(R_ETHER_OK != eth_ret);
    
    do
    {
        R_ETHER_LinkProcess();
        delay(10);
    }
    while(g_ether_TransferEnableFlag != ETHER_FLAG_ON);
    
    return result;
}

// (2) close
void hal_ethernet_close(void)
{
    R_ETHER_Close_ZC2(0);
}

// (3) send
// return: 0=SUCCESS (!!! not sent size)
int  hal_ethernet_send(unsigned char *pucBuffer, int length)
{
	int ret;
    uint8_t * pwrite_buffer;
    uint16_t write_buf_size;
    
    // get buffer to send
    ret = R_ETHER_Write_ZC2_GetBuf(0, (void **) &pwrite_buffer, &write_buf_size);
    if (R_ETHER_OK != ret)
    {
        //debug_print("R_ETHER_Write_ZC2_GetBuf failed!\n");
        return -1;
    }
    // copy data to buffer if size enough
    if ((write_buf_size >= length) && (write_buf_size >= ETHER_BUFSIZE_MIN))
    {
        memcpy(pwrite_buffer, pucBuffer, length);
    }else{
        //debug_print("R_ETHER_Write_ZC2_GetBuf failed 2!\n");
        return -2;
    }
    // minimum Ethernet frame size is 60 bytes.
    // zero padding and resize if under minimum.
    if (length < ETHER_BUFSIZE_MIN)
    {
        memset((pwrite_buffer + length), 0, (ETHER_BUFSIZE_MIN - length));  // padding
        length = ETHER_BUFSIZE_MIN;                                         // resize
    }
    
    ret = R_ETHER_Write_ZC2_SetBuf(0, (uint16_t)length);
    if(ret != R_ETHER_OK){
        //debug_print("R_ETHER_Write_ZC2_SetBuf failed!\n");
        return -3;
    }
    
    //ret = R_ETHER_CheckWrite(0);
    //if(ret != R_ETHER_OK){
    //    return -4;
    //}
    
    return 0;
}

// (4) receive
// return: received size
int  hal_ethernet_recv(unsigned char **data)
{
    // polling Ethernet instead of interrupt. (TODO This is a bad trick.)
    ethernet_poll();
    int result = R_ETHER_Read_ZC2(0, (void **)data);
    return result;
}

// release received buffer
// don't forget call this after hal_ethernet_recv()
void hal_ethernet_recv_rel(void)
{
    R_ETHER_Read_ZC2_BufRelease(0);
}

#ifdef __cplusplus
}
#endif

#elif defined(GRROSE)
/**************************************************
  for GR-ROSE
**************************************************/

#if defined(__cplusplus)
#include "Arduino.h"
extern "C" {
#endif
#include "FreeRTOS.h"
#include "task.h"
#include "r_ether_rx_if.h"
#include "r_ether_rx_pinset.h"
int32_t callback_ether_regist(void);
#ifdef __cplusplus
}
#endif

#define ETHER_BUFSIZE_MIN 60
#define MY_MAC_ADDR    0x02,0x00,0x00,0x00,0x00,0x00
static uint8_t myethaddr[6] = {MY_MAC_ADDR};

#ifdef __cplusplus
extern "C"
{
#endif

#if 0
static TaskHandle_t my_ether_link_check_task_handle = 0;
static void my_check_ether_link(void * pvParameters)
{
    R_INTERNAL_NOT_USED(pvParameters);

    while(1)
    {
        vTaskDelay(1000);
        R_ETHER_LinkProcess(0);
    }
}
#endif

void callback_ether(void * pparam);
static int32_t my_callback_ether_regist(void)
{
    ether_param_t   param;
    ether_cb_t      cb_func;

    int32_t         ret;

    /* Set the callback function (LAN cable connect/disconnect event) */
    cb_func.pcb_func     = &callback_ether;
    param.ether_callback = cb_func;
    ret = R_ETHER_Control(CONTROL_SET_CALLBACK, param);
    if (ETHER_SUCCESS != ret)
    {
        return -1;
    }
#if 0
    /* Set the callback function (Ether interrupt event) */
    cb_func.pcb_int_hnd     = &EINT_Trig_isr;
    param.ether_callback = cb_func;
    ret = R_ETHER_Control(CONTROL_SET_INT_HANDLER, param);
    if (ETHER_SUCCESS != ret)
    {
        return -1;
    }
#endif
    return 0;
}

// (1) open
// return: 0=SUCCESS
int  hal_ethernet_open(void)
{
    ether_param_t   param;
    ether_return_t eth_ret;
    
    R_ETHER_PinSet_ETHERC0_RMII();
    R_ETHER_Initial();
    my_callback_ether_regist();
    int result = R_ETHER_Open_ZC2(0, myethaddr, ETHER_FLAG_OFF);
    param.channel = ETHER_CHANNEL_0;
    eth_ret = R_ETHER_Control(CONTROL_POWER_ON, param); // PHY mode settings, module stop cancellation
    
    do //TODO allow for timeout
    {
        eth_ret = R_ETHER_CheckLink_ZC(0);
        vTaskDelay(10); // TODO
    }
    while(ETHER_SUCCESS != eth_ret);
    
    R_ETHER_LinkProcess(0);
    
#if 0
    long return_code;
    return_code = xTaskCreate(my_check_ether_link,
                              "CHECK_ETHER_LINK_TIMER",
                              100,
                              0,
                              configMAX_PRIORITIES,
                              &my_ether_link_check_task_handle);
    if (pdFALSE == return_code)
    {
        return pdFALSE;
    }
#endif
    return result;	
}

// (2) close
void hal_ethernet_close(void)
{
    R_ETHER_Close_ZC2(0);
}

// (3) send
// return: 0=SUCCESS (!!! not sent size)
int  hal_ethernet_send(unsigned char *pucBuffer, int length)
{
    ether_return_t ret;
    uint8_t * pwrite_buffer;
    uint16_t write_buf_size;
    
    // get buffer to send data
    ret = R_ETHER_Write_ZC2_GetBuf(ETHER_CHANNEL_0, (void **) &pwrite_buffer, &write_buf_size);
    if (ETHER_SUCCESS != ret)
    {
        //debug_print("R_ETHER_Write_ZC2_GetBuf failed!\n");
        return -1;
    }
    // copy data to buffer if size enough
    if ((write_buf_size >= length) && (write_buf_size >= ETHER_BUFSIZE_MIN))
    {
        memcpy(pwrite_buffer, pucBuffer, length);
    }else{
        return -2;
    }
    // minimum Ethernet frame size is 60 bytes.
    // zero padding and resize if under minimum.
    if (length < ETHER_BUFSIZE_MIN)
    {
        memset((pwrite_buffer + length), 0, (ETHER_BUFSIZE_MIN - length));  // padding
        length = ETHER_BUFSIZE_MIN;                                         // resize
    }
    
    ret = R_ETHER_Write_ZC2_SetBuf(ETHER_CHANNEL_0, (uint16_t)length);
    if(ret != ETHER_SUCCESS){
        return -3;
    }
    
    ret = R_ETHER_CheckWrite(ETHER_CHANNEL_0);
    if(ret != ETHER_SUCCESS){
        return -4;
    }
    
    return 0;
}

// (4) receive
// return: received size
int  hal_ethernet_recv(unsigned char **data)
{
    int result = R_ETHER_Read_ZC2(ETHER_CHANNEL_0, (void **)data);
#if 0
    int i;
    for(i=0;i<result;i++){
        debug_print("%02X " , *data[i]);
    }
    if(result>0) hoge_print("\n");
#endif
    return result;
}

// release received buffer
// don't forget call this after hal_ethernet_recv()
void hal_ethernet_recv_rel(void)
{
    R_ETHER_Read_ZC2_BufRelease(ETHER_CHANNEL_0);
}

#ifdef __cplusplus
}
#endif

#else
/**************************************************
  for Ethernet Shield (W5500)
**************************************************/

#include <SPI.h>
#include <Ethernet2.h>
#include <utility/w5500.h>

// W5500 RAW socket
static SOCKET sock;
// W5500 RAW socket buffer
static unsigned char socketBuffer[1536];

#ifdef __cplusplus
extern "C"
{
#endif

// (1) open
// return: 0=SUCCESS
int hal_ethernet_open(void)
{
    w5500.init();
    w5500.writeSnMR(sock, SnMR::MACRAW); 
    w5500.execCmdSn(sock, Sock_OPEN);
    return 0;
}

// (2) close
void hal_ethernet_close(void)
{
    // w5500 doesn't have close() or terminate() method
    w5500.swReset();
}

// (3) send
// return: 0=SUCCESS (!!! not sent size)
int hal_ethernet_send(unsigned char *data, int len)
{
    w5500.send_data_processing(sock, (byte *)data, len);
    w5500.execCmdSn(sock, Sock_SEND_MAC);
    return 0;
}

// (4) receive
// return: received size
int hal_ethernet_recv(unsigned char **data)
{
    unsigned short packetSize;
    int res = w5500.getRXReceivedSize(sock);
    if(res > 0){
        // first 2byte shows packet size
        w5500.recv_data_processing(sock, (byte*)socketBuffer, 2);
        w5500.execCmdSn(sock, Sock_RECV);
        // packet size
        packetSize  = socketBuffer[0];
        packetSize  <<= 8;
        packetSize  &= 0xFF00;
        packetSize  |= (unsigned short)( (unsigned char)socketBuffer[1] & 0x00FF);
        packetSize  -= 2;
        // get received data
        memset(socketBuffer, 0x00, 1536);
        w5500.recv_data_processing(sock, (byte *)socketBuffer, packetSize);
        w5500.execCmdSn(sock, Sock_RECV);
        *data = socketBuffer;
    }
    return packetSize;
}

#ifdef __cplusplus
}
#endif

#endif // for Ethernet Shield (W5500)

