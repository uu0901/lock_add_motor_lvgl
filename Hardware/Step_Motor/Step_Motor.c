#include "stm32f10x.h"                  // Device header
#include "step_motor.h"                

void StepMotor_DIR_ENA_Init(void)
{
 
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //使能GPIOC端口时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;  //PC1 DIR  PC2 ENA
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
    GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOC1 GPIOC2
 
    //GPIO_SetBits(GPIOC,GPIO_Pin_1 | GPIO_Pin_2);			 //DIR和ENA输出高电平。因为DIR+和ENA+都接了5V高电屏，所以DIR-和ENA-是低电平有效【当EN+和EN-一高一低时。驱动器的光耦导通，电机线圈没电停止】，又因为DIR和ENA是接在光耦的一端【可查看原理图，此处的光耦可以理解为反相器】，ENA和DIR给1，ENA-和DIR-输出0，输出有效电平，电机线圈无电流，所以不转
    //硬件没有光耦，所以直接拉低EN-，停止电机
    GPIO_ResetBits(GPIOC,GPIO_Pin_1 | GPIO_Pin_2);
}
void StepMotor_Set_Dir(uint8_t pram)
{
    GPIO_WriteBit(GPIOC, GPIO_Pin_1, (BitAction)pram);   
}
void StepMotor_Set_En(uint8_t pram)
{
    GPIO_WriteBit(GPIOC, GPIO_Pin_2, (BitAction)pram);   
}
