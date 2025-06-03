#include "GUI.h"
#include "pic.h"

GUI_INFO gui_info;
GUI_BEAT gui_beat;

void gui_init( void )
{
    gui_info.power_percentage       = 50;

    gui_info.led_switch             = LED_ON; 
    gui_info.fan_level              = LEVEL_3;
    gui_info.mode_allow             = 0;
    gui_info.mode_num               = MOED_1;
    gui_info.channel_num            = 7;
    gui_info.channel_select         = 0;
    gui_info.sync_switch            = SYNC_ON;
    gui_info.alarm_temp_value       = 50;
    gui_info.gonglv_h               = 0.0f;
    
    gui_beat.beat_start_flag        = 0;
    gui_beat.beat_clear             = 0;

    gui_beat.beat_select            = 0;
    gui_beat.beat_switch            = BEAT_OFF; 
    
    gui_info.connect_on_flag        = UNCONNECT;
}

static void fan_beat( void )
{   
    if( gui_beat.beat_clear == 1 )
    {
        LCD_Fill(80,202,92,226,BACK_COLOR);
    }else
    {
        fan_dis();
    }
}

static void led_beat( void )
{  
    if( gui_beat.beat_clear == 1 )
    {
        LCD_Fill(137,193,169,225,BACK_COLOR);
    }else
    {
        led_dis();
    }
}

static void mode_beat( void )
{   
    if( gui_beat.beat_clear == 1 )
    {
        LCD_Fill(278,203,310,219,BACK_COLOR);
    }else
    {
        mode_dis();
    }
}

static void alarm_temp_beat( void )
{
    if( gui_beat.beat_clear == 1 )
    {
        LCD_Fill(260,54,284,70,BACK_COLOR);
    }else
    {
        alarm_temp_dis();
    }
}

void power_dis( void )
{
    LCD_ShowNum(110,101,gui_info.power_percentage,3,32,GREEN,BACK_COLOR);
}

void fan_dis( void )
{
    LCD_ShowNum(80,202,gui_info.fan_level,1,24,GREEN,BACK_COLOR);
}

void led_dis( void )
{
    if( gui_info.led_switch == LED_ON )
    {
        LCD_Show_Image_Internal_Flash(137,193,32,32,gImage_led_on,2048);
    }else
    {
        LCD_Show_Image_Internal_Flash(137,193,32,32,gImage_led_off,2048);
    }
}

void mode_dis( void )
{
    switch (gui_info.mode_num)
    {
        case 1:
            PutChinese_16(203,278,"节",BACK_COLOR,GREEN);
            PutChinese_16(203,295,"能",BACK_COLOR,GREEN);

            break;

        case 2:
            PutChinese_16(203,278,"普",BACK_COLOR,YELLOW);
            PutChinese_16(203,295,"通",BACK_COLOR,YELLOW);

            break;

        case 3:
            PutChinese_16(203,278,"强",BACK_COLOR,BRRED);
            PutChinese_16(203,295,"劲",BACK_COLOR,BRRED);

            break;

        default:
            break;
    }
}

void mode_select( void )
{
    gui_info.mode_allow = 1;
    
    switch (gui_info.mode_num)
    {
        case 1:
            gui_info.power_percentage = 30;
            gui_info.fan_level        = 3;

            break;

        case 2:
            gui_info.power_percentage = 50;
            gui_info.fan_level        = 4;

            break;

        case 3:
            gui_info.power_percentage = 80;
            gui_info.fan_level        = 6;

            break;

        default:
            break;
    }
}

void channel_dis( void )
{
    switch (gui_info.channel_select)
    {
        case 1: //001
            LCD_Show_Image_Internal_Flash(29,51,46,22,gImage_channel_on,2024);
            LCD_Show_Image_Internal_Flash(112,51,46,22,gImage_channel_off,2024);
            LCD_Show_Image_Internal_Flash(194,51,46,22,gImage_channel_off,2024);
            gui_info.channel_num = 1;

            break;

        case 2: //010
            LCD_Show_Image_Internal_Flash(29,51,46,22,gImage_channel_off,2024);
            LCD_Show_Image_Internal_Flash(112,51,46,22,gImage_channel_on,2024);
            LCD_Show_Image_Internal_Flash(194,51,46,22,gImage_channel_off,2024);
            gui_info.channel_num = 2;
            
            break;

        case 3:
            LCD_Show_Image_Internal_Flash(29,51,46,22,gImage_channel_off,2024);
            LCD_Show_Image_Internal_Flash(112,51,46,22,gImage_channel_off,2024);
            LCD_Show_Image_Internal_Flash(194,51,46,22,gImage_channel_on,2024);
            gui_info.channel_num = 4;

            break;

        case 4: //011
            LCD_Show_Image_Internal_Flash(29,51,46,22,gImage_channel_on,2024);
            LCD_Show_Image_Internal_Flash(112,51,46,22,gImage_channel_on,2024);
            LCD_Show_Image_Internal_Flash(194,51,46,22,gImage_channel_off,2024);
            gui_info.channel_num = 3;

            break;
            
        case 5: //111
            LCD_Show_Image_Internal_Flash(29,51,46,22,gImage_channel_on,2024);
            LCD_Show_Image_Internal_Flash(112,51,46,22,gImage_channel_on,2024);
            LCD_Show_Image_Internal_Flash(194,51,46,22,gImage_channel_on,2024);
            gui_info.channel_num = 7;

            break;

    default:
            gui_info.channel_select = 0;
            gui_info.channel_num = 0;
            LCD_Show_Image_Internal_Flash(29,51,46,22,gImage_channel_off,2024);
            LCD_Show_Image_Internal_Flash(112,51,46,22,gImage_channel_off,2024);
            LCD_Show_Image_Internal_Flash(194,51,46,22,gImage_channel_off,2024);

            break;
    }
}

void channel_switch( void )
{
    if(( gui_info.channel_num == 0 ) | ( gui_info.channel_num == 1 ) | ( gui_info.channel_num == 2 ))
    {
        gui_info.channel_select = gui_info.channel_num;
    }
    if( gui_info.channel_num == 4 )
    {
        gui_info.channel_select = 3;
    }
    if( gui_info.channel_num == 3 )
    {
        gui_info.channel_select = 4;
    }
    if( gui_info.channel_num == 7 )
    {
        gui_info.channel_select = 5;
    }
}
void alarm_temp_dis( void )
{
    LCD_ShowNum(260,54,gui_info.alarm_temp_value,3,16,POINT_COLOR,BACK_COLOR);
}

void temp_dis( void )
{
    LCD_ShowNum(260,54,gui_info.temp_value,3,16,POINT_COLOR,BACK_COLOR);
}

void alarm_icon_dis( void )
{
    if( gui_info.temp_value >= gui_info.alarm_temp_value )
    {
        LCD_Show_Image_Internal_Flash(97,3,29,29,gImage_temp_alarm_red,1682);
    }else
    {
        LCD_Fill(97,3,126,32,BACK_COLOR);
    }
}
void connect_dis( void )
{
    if( gui_info.connect_on_flag == CONNECT )
    {
        LCD_Fill(13,3,43,33,BACK_COLOR);
        LCD_Show_Image_Internal_Flash(13,3,29,30,gImage_connect_on,1740);
    }else
    {
        LCD_Show_Image_Internal_Flash(13,3,30,30,gImage_connect_off,1800);
    }
}

void dht11_dis( void )
{
    LCD_ShowNum(215,10,gui_info.envir_temp,3,16,POINT_COLOR,BACK_COLOR);
    LCD_ShowNum(257,10,gui_info.envir_humidity,3,16,POINT_COLOR,BACK_COLOR);
}
void icon_beat(uint8_t pic_code , uint8_t beat_switch )
{
    if( gui_beat.beat_start_flag == 1 )
    {
        delay_ms(1);
        gui_beat.beat_clear = 1 -  gui_beat.beat_clear;
        switch (pic_code)
        {
            case FAN_ICON:
                fan_beat();
                break;

            case LED_ICON:
                led_beat();
                break;

            case MODE_ICON:
                mode_beat();
                break;

            case ALARM_TEMP:
                alarm_temp_beat();
                break;

            default:
                break;
        }
    }

    gui_beat.beat_start_flag = 0; 
}

void refresh_icon( void )
{
    fan_dis();
    led_dis();
    mode_dis();
    power_dis();
    temp_dis();
    alarm_icon_dis();
    sync_dis();
    dht11_dis();
    channel_dis();
}

void sync_dis( void )
{
    if( gui_info.sync_switch == 1 )
    {
        LCD_Show_Image_Internal_Flash(55,3,30,30,gImage_sync_on,1800);
    }else
    {
        LCD_Fill(55,3,85,33,BACK_COLOR);
    }
}

void slave_statu_update( void )
{
    if( gui_beat.beat_switch == 0 )
    {
        if(( modbus.modbus_04_scan_flag == 1 ) && ( modbus.modbus_04_scan_allow == 1 ))
        {
            get_slave_statu_04();
            temp_dis();
            alarm_icon_dis();
            dht11_dis();
            gui_info.gonglv = ((gui_info.gonglv_h * 4.0f) + ( gui_info.gonglv_min *4.0f / 60.0f ));
            delay_ms(1);
            LCD_ShowxFloat(230,161,gui_info.gonglv,1,16,BACK_COLOR,POINT_COLOR);
            modbus.modbus_04_scan_flag = 0;
        }
    }else
    {
        modbus.modbus_04_scan_allow = 0;
    }   
}

void gui_icon_init( void )
{
    /*              connect icon                */
    LCD_Show_Image_Internal_Flash(13,3,30,30,gImage_connect_off,1800);

    /*              sync icon                */
    LCD_Show_Image_Internal_Flash(55,3,30,30,gImage_sync_on,1800);

    /*              alarm icon                */


    /*              temp && shidu icon                */
    dht11_dis();
    PutChinese_12(13,245,"度",BACK_COLOR,POINT_COLOR);
    LCD_Show_Image_Internal_Flash(285,13,13,12,gImage_baifenbi,312);

    /*              channel icon                */
    channel_dis();
    temp_dis();
    PutChinese_12(57,290,"度",BACK_COLOR,POINT_COLOR);

    /*              gonglv icon                */
    power_dis();

    LCD_Show_Image_Internal_Flash(182,113,35,35,gImage_baifenbi_big,2450);

    PutChinese_strings(150,162,"实时耗电",BACK_COLOR,POINT_COLOR);
    LCD_ShowChar(215,161,':',16,POINT_COLOR,BACK_COLOR);
    LCD_ShowString(280,161,32,16,16,"kw/h",POINT_COLOR,BACK_COLOR);

    /*              Fan icon                */
    LCD_Show_Image_Internal_Flash(42,193,32,32,gImage_fan1,2048);

    fan_dis();

    /*              LED icon                */
    led_dis();

    /*              Mode icon                */
    LCD_Show_Image_Internal_Flash(230,193,42,31,gImage_mode,2604);
    mode_dis();

    /*              draw line                */
    LCD_DrawLine(1,40,320,40,GRAY);

    LCD_DrawLine(20,180,300,180,GRAY);
}




