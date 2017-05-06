#include "sys.h"
#include "usart2.h"
//从usart1改的
 

#ifdef EN_USART2_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART2_RX_BUF[2321];     //接收缓冲,最大2321个字节.
//接收状态
//bit7，接收完成标志
u16 USART2_RX_STA=0;       //接收状态标记	  
  
void USART2_IRQHandler(void)
{
	u8 res;
	u16 length;	
	res = 0;
	length = 7;    
	if(USART2->SR&(1<<5))//接收到数据
	{
	    //printf("\n接收到数据\r\n");	 
		res=USART2->DR;				 //读取USART2寄存器里的东西
		if((USART2_RX_STA&0X3FFF) > 5)	   //返回值第6、7位是剩下的长度
		{
		    length = USART2_RX_BUF[5]*256 + USART2_RX_BUF[6] + 5;//返回值的第6、7位为长度 
		}
		if(USART2_RX_BUF[5] == 0x05)	  //只有身份证信息会有这么长
		{
		    length = 14 + 255; 			  //只读取文字信息，剩下的扔掉
		} 
		if((USART2_RX_STA&0X3FFF) == length)	  //达到长度
		{  
		    USART2_RX_STA|=0x8000;   //标记数据接收完成
			USART2_RX_BUF[length]=res;		 //最后一个数据
			USART2_RX_STA++;
		}
		else
		{		
			USART2_RX_BUF[USART2_RX_STA&0X3FFF]=res;	  //数据置入接收缓冲中
			USART2_RX_STA++;	  
		}		 		 									     
	}
	USART2_RX_STA|=0x4000;   //标记识别到卡										 
} 
#endif										 
//初始化IO 串口2
//pclk2:PCLK2时钟频率(Mhz)这里usart2要用pclk1时钟频率
//bound:波特率
//CHECK OK
void uart2_init(u32 pclk1,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;
		   
	temp=(float)(pclk1*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   //使能PORTA口时钟  
	RCC->APB1ENR|=1<<17;  //使能串口时钟 
	GPIOA->CRL&=0XFFFF00FF; 
	GPIOA->CRL|=0X00008B00;//IO状态设置	 PA2，PA3,低八位	  
	RCC->APB1RSTR|=1<<17;   //复位串口2
	RCC->APB1RSTR&=~(1<<17);//停止复位	   	   
	//波特率设置
 	USART2->BRR=mantissa; // 波特率设置	 
	USART2->CR1|=0X200C;  //1位停止,无校验位.
#ifdef EN_USART2_RX		  //如果使能了接收
	//使能接收中断
	USART2->CR1|=1<<8;    //PE中断使能
	USART2->CR1|=1<<5;    //接收缓冲区非空中断使能	    	
	MY_NVIC_Init(3,3,USART2_IRQChannel,2);//组2，最低优先级 
#endif
}
