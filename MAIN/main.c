#include <stm32f10x_map.h>
#include <stm32f10x_nvic.h>
#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h" 
#include "key.h"
#include "usart2.h"
//串口中断在usart.c中
//USART1波特率9600
//USART1波特率115200	 
											   
int main(void)			 //从usart2读卡
{			
	u8 t;
	//int len;
	u8 Ucommand[10];	//命令
	u16 times = 0;
	u8 key;
	u8 sign;
	u8 CardNo[18];		//身份证号
	Stm32_Clock_Init(9); //系统时钟设置
	delay_init(72);	     //延时初始化 
	uart_init(72,9600);	 //串口1初始化为9600
	uart2_init(72,230400);//串口2的时钟频率只有1的一半230400
	LED_Init();		  	 //初始化与LED连接的硬件接口
	KEY_Init();			 //初始化按键
	USART2_RX_STA = 0;
	sign = 0;    
	while(1)
	{
	    // USART_SendData(USART2, 0xAA);
	    //for(t = 0;t < 10;t++)
		//{
		//    USART2->DR = Ucommand[t];
		//}
		key = 0;
		key = KEY_Scan();		 //按键值
		if(key == 3)			 //按键WK_UP
		{
		    sign++;
			LED1 = !LED1;		 //LED2状态变化
			delay_ms(200);
		}
		delay_ms(10);
		if(sign == 1)			 //先发送寻卡命令
		{
		    Ucommand[0] = 0xAA;		   //AAAAAA96690003200122
	        Ucommand[1] = 0xAA;
        	Ucommand[2] = 0xAA;
        	Ucommand[3] = 0x96;
        	Ucommand[4] = 0x69;
        	Ucommand[5] = 0x00;
        	Ucommand[6] = 0x03;
        	Ucommand[7] = 0x20;
        	Ucommand[8] = 0x01;
        	Ucommand[9] = 0x22;
			for(t=0;t < 10;t++)
		    {
				USART2->DR = Ucommand[t];		//给USART2寄存器
				//printf("%02X ",Ucommand[t]);
				delay_ms(1);					//延时等发完
		    }
			delay_ms(10);						//延时等一下返回值发送完毕
		    Ucommand[0] = 0xAA;					//选取命令
	        Ucommand[1] = 0xAA;					//AAAAAA96690003200221
        	Ucommand[2] = 0xAA;
        	Ucommand[3] = 0x96;
        	Ucommand[4] = 0x69;
        	Ucommand[5] = 0x00;
        	Ucommand[6] = 0x03;
        	Ucommand[7] = 0x20;
        	Ucommand[8] = 0x02;
        	Ucommand[9] = 0x21;
			for(t=0;t < 10;t++)
		    {
				USART2->DR = Ucommand[t];
				//printf("%02X ",Ucommand[t]);
				delay_ms(1);
		    }
			delay_ms(10);					   //延时等一下返回值发送完毕
		    Ucommand[0] = 0xAA;				   //获取信息命令
	        Ucommand[1] = 0xAA;				   //AAAAAA96690003300132
        	Ucommand[2] = 0xAA;
        	Ucommand[3] = 0x96;
        	Ucommand[4] = 0x69;
        	Ucommand[5] = 0x00;
        	Ucommand[6] = 0x03;
        	Ucommand[7] = 0x30;
        	Ucommand[8] = 0x01;
        	Ucommand[9] = 0x32;
			for(t=0;t < 10;t++)
		    {
				USART2->DR = Ucommand[t];
				//printf("%02X ",Ucommand[t]);
				delay_ms(1);
		    }
			delay_ms(10);
			sign = 0;
		}
		/*if(USART_RX_STA&0x80)				  //手动输入命令部分
		{					   
			printf("Data recognized\r\n");
			len = USART_RX_STA&0X3F;
			printf("%02X ",len);
			for(t=0;t < len;t++)
		    {
				USART2->DR = USART_RX_BUF[t];
				printf("%02X ",USART_RX_BUF[t]);
				//printf("%02X ",t);			  
				//printf("u2 %02X u2 ",USART2_RX_BUF[t]);
		    }
			USART_RX_STA = 0;
		}*/
		//len = 0;
	    if(USART2_RX_STA&0x8000)			   //收到读卡器返回值
		{					   
			//printf("Card recognized\r\n");
			//len = USART2_RX_STA&0X3FFF;
			if(USART2_RX_BUF[5] == 0x05)	  //收到身份证文字照片信息
	    	{
	    	    for(t=136;t < 171;t++)								   //身份证号在的段落
				{	
				    if(t%2 == 0)									   //USC2四位一个字符
					{
					    USART2_RX_BUF[t] = USART2_RX_BUF[t]&0x0F;	   //USC2转换为数字
				        printf("%d ",USART2_RX_BUF[t]);				   //输出身份证号
						CardNo[t] = USART2_RX_BUF[t];				   //身份证号
					}	        
				}
				printf("\r\n");									   	   //不延时会有bug
				delay_ms(1000);										   //延迟等待其他信息发送完毕,可以更大一点
				//Sys_Soft_Reset(); 
	    	}
			/*else													   //输出寻卡等返回信息
			{														   //如果需要这些信息，需要每个循环发送一个命令
			    for(t=0;t < len ;t++)								   //不能像现在，一个循环就发完了
		        {
			       	//printf("%02X ",USART2_RX_BUF[t]);
				}
			}*/
			USART2_RX_STA = 0;										   //USART2状态置零
		}
		//len = 0;
		if(times%30==0)
		{
			LED0=!LED0;//闪烁LED,提示系统正在运行.
			//printf("%04X ",USART2_RX_STA&0X3FFF);
			//printf("%02X ",USART_RX_BUF[6]);
			//printf("%02X \r\n",USART2_RX_BUF[0]);
			times = 0;	
		}
		times++;
		//delay_ms(10); 
	}
											  	 
}













































