#ifndef __POWER_CRL_H_
#define __POWER_CRL_H_

#include "sys.h"
#include "gpio.h"
#include "ntc.h"
#include "eeprom_crl.h"
#include "dht11.h"

#define FAN_ON    1
#define FAN_OFF   0

typedef struct 
{
    uint8_t  zero_flag;             //220V输入触发标志位
    uint16_t time_delay;            //移相延时
    uint8_t  ac220_out1_enable;     //通道1输出使能
    uint8_t  ac220_out2_enable;     //通道2输出使能
    uint8_t  ac220_out3_enable;     //通道3输出使能
    uint8_t  ac220_out_temp_allow;  //3路通道温度检测低于报警温度时方可输出
    uint8_t  sync_flag;             //同步标志位
    uint8_t  mode_info;             //模式选择

    uint8_t signal_in_flag;         //24V输入检测
    uint8_t ac220_flag;             //220V输入检测
}AC_DC;

typedef struct 
{
    uint8_t  gonglv_cnt;            //功率累加计数
    uint8_t  gonglv_s;              //功率累加1s
    uint8_t  gonglv_min;            //功率累加1min
    uint16_t gonglv_h;              //功率累加1h
    uint8_t  gonglv_record_flag;    //功率累加记录标志位
}GONGLV;

extern AC_DC ac_dc;
extern GONGLV gonglv;

void Power_Statu_Init( void );
void led_ctrl( uint8_t on_off );
void ac_220v_crl( uint8_t power_level );
void sync_ctrl( void );
void temp_scan( void );
void fan_ctrl( uint8_t level );
void mode_ctrl( uint8_t mode_num );
void Power_consumption_Init( void );
void Power_consumption_scan( void );

#endif