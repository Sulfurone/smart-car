#ifndef _AD_smaple_H_
#define _AD_smaple_H_

#include"include.h"

void Get_ADC(void);
void ADC_duty(void);

extern uint16 ADC_0;
extern uint16 ADC_1;
extern uint16 ADC_2;
extern uint16 ADC_3;
extern uint16 ADC_4;
extern uint16 ADC_5;

extern uint16 Lleft1, Lleft2, Lright2, Lright1,mid1,mid2;  // 电感偏移量
extern uint16 MagneticField;     // 磁场强度    magnetic field intensity,判断圆环是否出现

#endif






