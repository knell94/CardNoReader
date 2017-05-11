#ifndef __USART2_H
#define __USART2_H
#include <stm32f10x_map.h>
#include <stm32f10x_nvic.h>
#include "stdio.h"	 

	  	
extern u8 USART2_RX_BUF[2321];     //接收缓冲,最大63个字节.末字节为换行符 
extern u16 USART2_RX_STA;         //接收状态标记	

//如果想串口中断接收，请不要注释以下宏定义
#define EN_USART2_RX //使能串口1接收
void uart2_init(u32 pclk2,u32 bound);

#endif	   











