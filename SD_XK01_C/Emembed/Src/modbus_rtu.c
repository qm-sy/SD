#include "modbus_rtu.h"

MODBUS_INFO modbus;

/**
 * @brief	发送1帧数据
 * 
 * @param   buf：待发送数组首地址           
 * @param   len：数组长度           
 * 
  @return  void
 */
void modbus_send_data( uint8_t *buf , uint8_t len )
{
    HAL_UART_Transmit(&huart1,(uint8_t*)buf,len,1000);
    
    while (__HAL_UART_GET_FLAG(&huart1,UART_FLAG_TC) != SET);
}

/**
 * @brief	modbus接收函数，接收并判断Function后转到相应Function函数进行处理
 * 
 * @param   buf：待发送数组首地址           
 * @param   len：数组长度           
 * 
  @return  void
 */
void Modbus_Event( void )
{
    uint16_t crc,rccrc;

    /*1.接收完毕                                           */
    if( rs485.reflag == 1 )
    { 
        rs485.reflag = 0;
        printf("arrive here \r\n");
        /*2.CRC校验                                         */
        crc = MODBUS_CRC16(rs485.rcvbuf, rs485.recount-2);
        rccrc = (rs485.rcvbuf[rs485.recount-2]<<8) | (rs485.rcvbuf[rs485.recount-1]);
        if ( crc == rccrc)
        {
            if( rs485.rcvbuf[0] == SLAVE_ADDR )
            {
                switch ( rs485.rcvbuf[1] )
                {         
                    case 0x03:		Modbus_Fun3();		break;
                    case 0x04:		Modbus_Fun4();      break;            

                    default:						    break;
                }
                    /* RX允许继续接收，开启超时接收计时               */
                    rs485.recount = 0;
            }
        }
    }
}

void Modbus_Fun3()
{
   uint8_t start_addr_03 = 3;              //Slave reply  DATA1_H address

   for( uint16_t i = 0; i < 6; i++)
   {
       switch (i)
       {
        case 0:
            gui_info.fan_level = rs485.rcvbuf[start_addr_03 + 1];   

            break;

        case 1:
            gui_info.led_switch = rs485.rcvbuf[start_addr_03 + 1];

            break;

        case 2:
            gui_info.power_percentage = rs485.rcvbuf[start_addr_03];    
            gui_info.channel_num      = rs485.rcvbuf[start_addr_03 + 1];
            
            break; 

        case 3:
            gui_info.sync_switch = rs485.rcvbuf[start_addr_03 + 1];

            break;

        case 4:
            gui_info.mode_num = rs485.rcvbuf[start_addr_03 + 1];

            break;   

        case 5:
            gui_info.alarm_temp_value = rs485.rcvbuf[start_addr_03 + 1];

            break;   

        default:
            break;
       }
       start_addr_03 += 2;
   }
   gui_info.connect_on_flag = CONNECT;
}

void Modbus_Fun4()
{
    uint8_t start_addr_04 = 3;              //Slave reply  DATA1_H address

    for( uint16_t i = 0; i < 5; i++)
    {
        switch (i)
        {
            case 0:
            
                gui_info.temp_value = rs485.rcvbuf[start_addr_04 + 1];
                break;

            case 1:
                
                break;

            case 2:
                gui_info.envir_humidity = rs485.rcvbuf[start_addr_04]; 
                gui_info.envir_temp     = rs485.rcvbuf[start_addr_04 + 1];     

                break; 

            case 3:
                
                break;

            case 4:
                gui_info.gonglv_min =    rs485.rcvbuf[start_addr_04 + 1];      

                break;

            case 5:

                gui_info.gonglv_h = ((rs485.rcvbuf[start_addr_04] << 8) |rs485.rcvbuf[start_addr_04 + 1]);      

                break;

            default:
                break;
        }
        start_addr_04 += 2;
    }
}

/**
 * @brief	crc校验函数
 * 
 * @param   buf：  Address(1 byte) +Funtion(1 byte) ）+Data(n byte)   
 * @param   length:数据长度           
 * 
  @return  crc16:crc校验的值 2byte
 */
//buf内的值为 
uint16_t MODBUS_CRC16(uint8_t *buf, uint8_t length)
{
	uint8_t	i;
	uint16_t	crc16;

    /* 1, 预置16位CRC寄存器为0xffff（即全为1）                          */
	crc16 = 0xffff;	

	do
	{
        /* 2, 把8位数据与16位CRC寄存器的低位相异或，把结果放于CRC寄存器     */        
		crc16 ^= (uint16_t)*buf;		//
		for(i=0; i<8; i++)		
		{
            /* 3, 如果最低位为1，把CRC寄存器的内容右移一位(朝低位)，用0填补最高位 再异或0xA001    */
			if(crc16 & 1)
            {
                crc16 = (crc16 >> 1) ^ 0xA001;
            }
            /* 4, 如果最低位为0，把CRC寄存器的内容右移一位(朝低位)，用0填补最高位                */
            else
            {
                crc16 >>= 1;
            }		
		}
		buf++;
	}while(--length != 0);

	return	(crc16);
}


void get_slave_statu_03( void )
{
    uint8_t send_buf[8] = {0x1B,0x03,0x00,0x00,0x00,0x06,0xF2,0xC7};

    TX1_485;
    delay_ms(5);

    memcpy(modbus.modbus_send_buf,send_buf,8);

    modbus_send_data(modbus.modbus_send_buf,8); 

    RX1_485;
}

void get_slave_statu_04( void )
{
    uint8_t send_buf[8] = {0x1B,0x04,0x00,0x00,0x00,0x06,0x32,0x72};

    TX1_485;
    delay_ms(5);
    
    memcpy(modbus.modbus_send_buf,send_buf,8);

    modbus_send_data(modbus.modbus_send_buf,8); 

    RX1_485;
}

void send_to_slave( void )
{
   uint8_t send_buf[21];
   uint16_t crc;

   modbus.modbus_04_scan_allow = 0;

   TX1_485;
   delay_ms(5);

   send_buf[0] = 0x1B;
   send_buf[1] = 0x10;
   send_buf[2] = 0x00;
   send_buf[3] = 0x00;
   send_buf[4] = 0x00;
   send_buf[5] = 0x06;
   send_buf[6] = 0x0C;

   send_buf[7] = 0x00;
   send_buf[8] = gui_info.fan_level;

   send_buf[9] = 0x00;
   send_buf[10] = gui_info.led_switch;

   send_buf[11] = gui_info.power_percentage;
   send_buf[12] = gui_info.channel_num;

   send_buf[13] = 0x00;
   send_buf[14] = gui_info.sync_switch;

   send_buf[15] = gui_info.mode_allow;
   send_buf[16] = gui_info.mode_num;

   send_buf[17] = 0x00;
   send_buf[18] = gui_info.alarm_temp_value;

   crc = MODBUS_CRC16(send_buf,19);

   send_buf[19] = crc>>8;
   send_buf[20] = crc;

   memcpy(modbus.modbus_send_buf,send_buf,21);

   modbus_send_data(modbus.modbus_send_buf,21);
   RX1_485;
   modbus.modbus_04_scan_allow = 1;
   gui_info.mode_allow = 0;
}

