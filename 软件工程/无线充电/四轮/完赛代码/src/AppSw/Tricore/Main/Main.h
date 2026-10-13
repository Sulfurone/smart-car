/*!
  * @file     Main.h
  *
  * @brief    
  *
  * @company  北京龙邱智能科技
  *
  * @author   LQ-005
  *
  * @note     Tab键 4个空格
  *
  * @version  V1.0
  *
  * @par URL  http://shop36265907.taobao.com
  *           http://www.lqist.cn
  *
  * @date     2020年6月11日
  */ 
#ifndef SRC_APPSW_TRICORE_MAIN_MAIN_H_
#define SRC_APPSW_TRICORE_MAIN_MAIN_H_

#include <stdint.h>



extern IfxCpu_mutexLock mutexCpu0InitIsOk;   /** CPU0 初始化完成标志位  */

extern IfxCpu_mutexLock mutexTFTIsOk;        /** TFT18使用标志位  */
/**
 * 定时器 5ms和50ms标志位
 */
extern volatile unsigned char cpu1Flage5ms;
extern volatile unsigned char cpu1Flage50ms;

/* 期望速度  */
extern volatile signed short targetSpeed;
/* 避障标志位 */
extern volatile unsigned char evadibleFlage;

int core0_main (void);
void CAR_Drive_duty(void);         //车辆控制
#endif /* SRC_APPSW_TRICORE_MAIN_MAIN_H_ */
