#include "power_crl.h"

AC_DC ac_dc;
GONGLV gonglv;

/**
 * @brief	移相触发调用结构体初始化
 *
 * @param   
 *
 * @return  void
**/
void Power_Statu_Init( void )
{
    ac_dc.zero_flag  = 0;
    ac_dc.ac220_flag = 0;
    temp.temp_scan_flag = 0;
    AC_Out1 = AC_Out2 = AC_Out3 = 1;
}

/**
 * @brief	功率计时初始化
 *
 * @param   
 *
 * @return  void
**/
void Power_consumption_Init( void )
{
    gonglv.gonglv_cnt = 0;
    gonglv.gonglv_min = 0;
    gonglv.gonglv_h = 0;
    gonglv.gonglv_record_flag = 0;
}

/**
 * @brief	外部中断0中断处理函数
 *
 * @param   
 *
 * @return  void
**/
void INT0_ISR( void ) interrupt 0
{
    /* 1, 检测到外部中断后，等待THL\TLI后触发TIM1中断       */
    TL1 = ac_dc.time_delay;				
	TH1 = ac_dc.time_delay >> 8;				

    ac_dc.zero_flag = 1;
    ac_dc.ac220_flag = 1;

    /* 2, 定时器1开始计时，打开TIM1中断                     */
    TR1 = 1;				     
    ET1 = 1; 
}

/**
 * @brief	Timer1中断处理函数
 *
 * @param   
 *
 * @return  void
**/
void Tim1_ISR( void ) interrupt 3   //10ms
{
    /* 1, 中断触发后，power_ch电平 由高电平变为低电平           */
    if( ac_dc.zero_flag == 1 )
    {
        ac_dc.zero_flag = 0;

         /* 2, 温度允许，使能为1时可开启输出          */
        if(( ac_dc.ac220_out1_enable == 1 ) && (ac_dc.ac220_out_temp_allow == 1))
        {
            AC_Out1 = 0;
        }
        if(( ac_dc.ac220_out2_enable == 1 ) && (ac_dc.ac220_out_temp_allow == 1))
        {
            AC_Out2 = 0;
        }
        if(( ac_dc.ac220_out3_enable == 1 ) && (ac_dc.ac220_out_temp_allow == 1))
        {
            AC_Out3 = 0;
        }

         /* 3, 设置下一次Timer1中断触发所需时间，即脉冲时间       */
        TL1 = 0xF7;				
        TH1 = 0xFF;				
    }else
    {
        /* 2, 下一次进入Timer1中断，power_ch电平 由低电平变为高电平，完成一次10us脉冲，即斩波*/
        AC_Out1 = AC_Out2 = AC_Out3 = 1;

        /* 3, 定时器1停止计时，关闭TIM1中断，等待下一次外部中断     */
        TR1 = 0;				    
        ET1 = 0; 
    }
}

/**
 * @brief	220V输出功率控制函数 
 *
 * @param   power_level：输出功率百分比  0%~100%
 *
 * @return  void
**/
void ac_220v_crl( uint8_t power_level )
{
    ac_dc.time_delay = 56500 + 90*power_level;
}

/**
 * @brief	24V LED开关控制函数
 *
 * @param   on_off：0：关闭 1：开启
 *
 * @return  void
**/
void led_ctrl( uint8_t on_off )
{
    if( on_off == FAN_ON )
    {
        LED = 0;
    }else
    {
        LED = 1;
    }
}

/**
 * @brief	24V PWM风扇档位控制函数
 *
 * @param   level :风扇档位 0~6档
 *
 * @return  void
**/
void fan_ctrl( uint8_t level )
{
    PWMB_CCR7= level * 184;
}

/**
 * @brief	同步控制函数：无外部信号时 PWM风扇和220V输出关闭
 *
 * @param   
 *
 * @return  void
**/
void sync_ctrl( void )
{
    if( ac_dc.sync_flag == 1 )
    {
        if( ac_dc.signal_in_flag == 1 )
        {
            PWMB_BKR = 0x80;    //PWM控制
            EX0 = 1;            //外部中断控制
        }else
        {
            PWMB_BKR = 0x00; 
            EX0 = 0;
        }
    }else
    {
        PWMB_BKR = 0x80; 
        EX0 = 1;
    }
}

/**
 * @brief 温度扫描，DHT11温湿度扫描 1s/次 控制220V输出使能
 *
 * @param[in] 
 * 
 * @return  
 * 
**/
void temp_scan( void )
{
    if( temp.temp_scan_flag == 1)
    {
        temp.temp_value1 =  get_temp(NTC);

        Read_DHT11();

        if( temp.temp_value1 >= temp.temp_alarm_value )  
        {
            ac_dc.ac220_out_temp_allow = 0;     
        }else
        {
            ac_dc.ac220_out_temp_allow = 1;     
        }

        temp.temp_scan_flag = 0;
    }
}

/**
 * @brief 用电量累计扫描 三路全开满功率1min记录一次
 *
 * @param[in] 
 * 
 * @return  
 * 
**/
void Power_consumption_scan( void )
{
    if( gonglv.gonglv_record_flag == 1 )
    {
        eeprom_data_record(); 
        gonglv.gonglv_record_flag = 0;
    }
}

/**
 * @brief	模式控制函数 
 *
 * @param   
 *
 * @return  void
**/
void mode_ctrl( uint8_t mode_num )
{
    switch (mode_num)
    {
        case 1:
            ac_220v_crl(30);
            fan_ctrl(3);

            eeprom.ac220_level = 30;
            eeprom.pwm_info = 3;
            eeprom_data_record();

            break;

        case 2:
            ac_220v_crl(50);
            fan_ctrl(4);

            eeprom.ac220_level = 50;
            eeprom.pwm_info = 4;
            eeprom_data_record();

            break;

        case 3:
            ac_220v_crl(80);
            fan_ctrl(6);

            eeprom.ac220_level = 80;
            eeprom.pwm_info = 6;
            eeprom_data_record();

            break;

        default:
            break;
    }
}