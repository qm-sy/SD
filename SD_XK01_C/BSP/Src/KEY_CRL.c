#include "KEY_CRL.h"

KEY key;

void key_init( void )
{

}

void jump_to_init( void )
{
    refresh_icon();
    /*       无图标跳动     */
    gui_beat.beat_switch = BEAT_OFF;

    /*       可继续查询从机状态     */
    modbus.modbus_04_scan_allow = 1;

    /*       复位图标跳动起始点     */
    gui_beat.beat_select = 0;
}

void key_scan( void )
{
    uint8_t key_value;

    /*       50ms检测一次     */
    if(key.key_scan_flag == 1)
	{
		key_value = (B1_VAL) | (B2_VAL<<1) | (B3_VAL<<2) | (B4_VAL<<3) | (B5_VAL<<4);
	}
    switch (key_value)
    {
        case KEY1:      KEY1_press();       break;

        case KEY2:      KEY2_press();       break;

        case KEY3:      KEY3_press();       break;

        case KEY4:      KEY4_press();       break;

        case KEY5:      KEY5_press();       break;

        default:                            break;
    }
}

void KEY1_press()
{
    uint8_t key_value = KEY1;
    uint8_t key1_delay_cnt = 100;

    buzzer_open();

    while((key_value == KEY1)&&(key1_delay_cnt!=0)) 
    {
        key_value = (B1_VAL) | (B2_VAL<<1) | (B3_VAL<<2) | (B4_VAL<<3) | (B5_VAL<<4);
        key1_delay_cnt--;
        delay_ms(10); 
    }
    if( key1_delay_cnt == 0 )
    {
        gui_beat.beat_switch = BEAT_ON;
        gui_beat.beat_select = ALARM_TEMP;
        
        buzzer_close();
        delay_ms(500);
    }else
    {
        gui_info.channel_select++;
        channel_dis();
        send_to_slave();
    }
    
    buzzer_close();
}

void KEY2_press()
{
    buzzer_open();

    if( gui_beat.beat_switch == 0 )
    {
        if( gui_info.power_percentage < 100 )
        {
            gui_info.power_percentage += 5;
        }
        power_dis();
        send_to_slave();
    }else
    {
        switch(gui_beat.beat_select)
        {
            case FAN_ICON:
                if( gui_info.fan_level < 6 )
                {
                    gui_info.fan_level += 1;
                }
                fan_dis();
                break;

            case LED_ICON:
                gui_info.led_switch = LED_ON;
                led_dis();    

                break;

            case MODE_ICON:
                gui_info.mode_num += 1;
                if( gui_info.mode_num >= 3 )
                {
                    gui_info.mode_num = 3;
                }
                mode_select();
                mode_dis();

                break;

            case ALARM_TEMP:
                if( gui_info.alarm_temp_value < 120 )
                {
                    gui_info.alarm_temp_value += 1;
                }
                alarm_temp_dis();

                break;

            default:
                break;
        }
        send_to_slave();
    }
    
    buzzer_close();
    key.key_scan_flag = 0;
}


void KEY3_press()
{
    buzzer_open();

    switch(gui_beat.beat_select)
    {
        case FAN_ICON:
            refresh_icon();
            gui_beat.beat_select = LED_ICON;

            break;

        case LED_ICON:
            refresh_icon();
            gui_beat.beat_select = MODE_ICON;

            break;

        case MODE_ICON:
            jump_to_init();
            break;

        default:
            gui_beat.beat_select++;
            gui_beat.beat_switch = BEAT_ON;

            break;
        
    }
    
    send_to_slave();
    
    buzzer_close();
    key.key_scan_flag = 0;
}

void KEY4_press()
{
    buzzer_open();

    if( gui_beat.beat_switch == 0 )
    {
        if( gui_info.power_percentage > 20 )
        {
            gui_info.power_percentage -= 5;
        }
        send_to_slave();
        power_dis();
    }else
    {
        switch(gui_beat.beat_select)
        {
            case FAN_ICON:
                if( gui_info.fan_level > 0)
                {
                    gui_info.fan_level -= 1;
                }
                fan_dis();
                break;

            case LED_ICON:
                gui_info.led_switch = LED_OFF;
                led_dis();    

                break;

            case MODE_ICON:
                gui_info.mode_num -= 1;
                if( gui_info.mode_num <= 1 )
                {
                    gui_info.mode_num = 1;
                }
                mode_select();
                mode_dis();
                break;

            case ALARM_TEMP:
                if( gui_info.alarm_temp_value > 20 )
                {
                    gui_info.alarm_temp_value -= 1;
                }
                alarm_temp_dis();

                break;

            default:
                break;
        }
        send_to_slave();
    }
    
    buzzer_close();
    key.key_scan_flag = 0;
 }

void KEY5_press()
{
    uint8_t key_value = KEY5;
    uint8_t key5_delay_cnt = 100;

    buzzer_open();
    while((key_value == KEY5)&&(key5_delay_cnt!=0)) 
    {
        key_value = (B1_VAL) | (B2_VAL<<1) | (B3_VAL<<2) | (B4_VAL<<3) | (B5_VAL<<4);
        key5_delay_cnt--;
        delay_ms(10); 
    }
    if( key5_delay_cnt == 0 )
    {
        gui_info.sync_switch = 1 - gui_info.sync_switch;
        sync_dis();
        send_to_slave();

        buzzer_close();
        delay_ms(500);
    }else
    {
        jump_to_init();
        send_to_slave();
    }

    
    buzzer_close();
    key.key_scan_flag = 0;
}

void buzzer_open( void )
{
    HAL_GPIO_WritePin(Buzzer_GPIO_Port,Buzzer_Pin,GPIO_PIN_SET);
}

void buzzer_close( void )
{
    delay_ms(150);
    HAL_GPIO_WritePin(Buzzer_GPIO_Port,Buzzer_Pin,GPIO_PIN_RESET);
}
