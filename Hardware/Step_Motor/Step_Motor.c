#include "stm32f10x.h"                  // Device header
#include "step_motor.h"                

void StepMotor_DIR_ENA_Init(void)
{
 
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��GPIOC�˿�ʱ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;  //PC1 DIR  PC2 ENA
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
    GPIO_Init(GPIOC, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOC1 GPIOC2
 
    //GPIO_SetBits(GPIOC,GPIO_Pin_1 | GPIO_Pin_2);			 //DIR��ENA����ߵ�ƽ����ΪDIR+��ENA+������5V�ߵ���������DIR-��ENA-�ǵ͵�ƽ��Ч����EN+��EN-һ��һ��ʱ���������Ĺ��ͨ�������Ȧû��ֹͣ��������ΪDIR��ENA�ǽ��ڹ����һ�ˡ��ɲ鿴ԭ��ͼ���˴��Ĺ���������Ϊ����������ENA��DIR��1��ENA-��DIR-���0�������Ч��ƽ�������Ȧ�޵��������Բ�ת
    //Ӳ��û�й������ֱ������EN-��ֹͣ���
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
