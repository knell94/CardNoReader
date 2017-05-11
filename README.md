# CardNoReader
读身份证号   
模块是千景科技的mini模块   
http://www.qianjingkeji.com/yueduqimozu/xiaomokuai.html   
板子是stm32f103rc
MCU为串口TTL，数据线接口与串口232相反
读卡器模块为串口232，3.3V,模块白线(RXD)接PA3，绿线(TXD)接PA2   
串口输入输出   
似乎github不支持ANSI，由于中文乱码问题，已经转换成keil4工程，请用CardNoReader.uvproj打开  
如果需要用keil3，请用CardNoReader.uv2打开，并改#include <stm32f10x_map.h>和#include <stm32f10x_nvic.h>两句为include <stm32f10x_lib.h>  
在keil3下，中文为乱码，可以通过用记事本打开.c和.h文件，另存为改文件编码为ANSI解决
