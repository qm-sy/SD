#include "common.h"

/**
 * @brief	一些定时执行的事件   Timer3：10ms
 *
 * @param   
 *
 * @return  void
**/
void Tim3_ISR( void ) interrupt 19
{

    static uint16_t temp_scan_cnt = 0;
    static uint8_t channel1_cnt,channel2_cnt,channel3_cnt = 0;
    static uint16_t gonglv_time = 0;
    static uint16_t ac220_flag_cnt = 0;

    /* 1, Signal_IN状态查询           */
    if( Signal_IN == 1 )
    {
        ac_dc.signal_in_flag = 1;
    }else
    {
        ac_dc.signal_in_flag = 0;
    }

    /* 2, temp 上报 1s/次                             */
    if( temp.temp_scan_flag == 0 )
    {
        temp_scan_cnt++;
        if( temp_scan_cnt == 100 )
        {
            temp.temp_scan_flag = 1;
            temp_scan_cnt = 0;
        }
    }
    
    /*3, 检测220V是否接入 5s关一次耗电计时，检测到220v输入再次开启计时*/
    if( ac_dc.ac220_flag == 1 ) 
    {
        ac220_flag_cnt++;
        if( ac220_flag_cnt == 500 )
        {
            ac220_flag_cnt = 0;
            ac_dc.ac220_flag = 0;
        }
    }

    /*4, 耗电计时*/
    if( ac_dc.ac220_flag == 1) 
    {
        if(( ac_dc.ac220_out1_enable == 1 ) && (ac_dc.ac220_out_temp_allow == 1))
        {
            channel1_cnt++;
        }
        if(( ac_dc.ac220_out2_enable == 1 ) && (ac_dc.ac220_out_temp_allow == 1))
        {
            channel2_cnt++;
        }
        if(( ac_dc.ac220_out3_enable == 1 ) && (ac_dc.ac220_out_temp_allow == 1))
        {
            channel3_cnt++;
        }
    }
    
    gonglv_time = (channel1_cnt + channel2_cnt + channel3_cnt) * eeprom.ac220_level;

    if( gonglv_time >= 30000 )  //1s
    {
        gonglv.gonglv_s ++;
        gonglv_time = 0;
        channel1_cnt = channel2_cnt = channel3_cnt = 0;

        if( gonglv.gonglv_s == 60 )   //1min
        {
            gonglv.gonglv_min++;
            gonglv.gonglv_s = 0;

            eeprom.gonglv_min = gonglv.gonglv_min;  //三路全开满功率1min记录一次
            gonglv.gonglv_record_flag = 1;

            if( gonglv.gonglv_min == 60 )    //1h
            {
                gonglv.gonglv_h++;
                eeprom.gonglv_h_H = (gonglv.gonglv_h >> 8);
                eeprom.gonglv_h_L = gonglv.gonglv_h;
                gonglv.gonglv_min = 0;
            }
        }
    }
}
