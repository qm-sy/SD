#ifndef __MODBUS_RTU_H_
#define __MODBUS_RTU_H_

#include "usart.h"
#include "main.h"
#include "PWM_CRL.h"
#include "communication.h"
#include "delay.h"
#include "GUI.h"

typedef struct  
{
    uint8_t modbus_send_buf[48];
    uint8_t modbus_rcv_buf[48];

    uint8_t fan_info;               //40001
    uint8_t led_info;               //40002
    uint8_t channel_info;             //40003
    uint8_t power_info;       //40004
    uint8_t sync_info;
    uint8_t mode_num;
    uint8_t temp;       //40004
    uint8_t alarm_temp;       //40005

    uint8_t envir_temp;
    uint8_t envir_humidity;

    uint8_t signal_in_flag;

    uint8_t modbus_04_scan_flag;
    uint8_t modbus_04_scan_allow;
}MODBUS_INFO;

extern MODBUS_INFO modbus;

void Modbus_Fun3( void );

void Modbus_Fun4( void );

// void Modbus_Fun6( void );

// void Modbus_Fun16( void );

void Modbus_Event( void );

void modbus_send_data( uint8_t *buf , uint8_t len );

uint8_t modbus_wait_receive( void );

uint16_t MODBUS_CRC16(uint8_t *buf, uint8_t length);

void slave_statu_query_modify( uint8_t fun, uint16_t reg_addr,uint16_t reg_num,uint16_t reg_val );
void test_hanshu(void);
void Modbus_fun03_Master( uint16_t reg_addr,uint16_t reg_num );
void Modbus_fun04_Master( uint16_t reg_addr,uint16_t reg_num );
void Modbus_fun06_Master( uint16_t reg_addr,uint16_t reg_num, uint16_t reg_val );
void send_to_slave( void );
void send_param( void );
void get_slave_statu_03( void );
void get_slave_statu_04( void );

#endif
