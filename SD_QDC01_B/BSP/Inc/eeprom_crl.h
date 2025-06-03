#ifndef __EEPROM_CRL_H_
#define __EEPROM_CRL_H_

#include "eeprom.h"
#include "sys.h"
#include "power_crl.h"
#include "ntc.h"
#include "stdio.h"

#define PWM_ADDR              0x0000
#define LED_ADDR              0x0001
#define AC220_ADDR1           0x0002
#define AC220_ADDR2           0x0003
#define SYNC_ADDR             0X0004 
#define MODE_ADDR             0X0005
#define TEMP_ALARM            0X0006
#define GONGLV_MIN            0X0007
#define GONGLV_H_H            0X0008
#define GONGLV_H_L            0X0009

#define EEPROM_STATU_JUDGE    0X0010         

typedef struct 
{
    uint8_t  pwm_info;
    uint8_t  led_info;         
    uint8_t  ac220_switch;  
    uint8_t  ac220_level; 
    uint8_t  sync_info;
    uint8_t  mode_info;     
    uint8_t  temp_alarm_value;       
    uint8_t  gonglv_min;
    uint8_t  gonglv_h_H;
    uint8_t  gonglv_h_L;
}EEPROM_INFO;

extern EEPROM_INFO eeprom;

void eeprom_statu_judge( void );
void eeprom_data_record( void );
void eeprom_data_init( void );

#endif