#include "timer.h"
#include <stdint.h>
#include <stdbool.h>

#include "tim.h"

#include <stdbool.h>
#include "ppm.h"


#include <string.h>




time_flag_type timer_flag;





void timer_init(void)
{
	
	HAL_TIM_Base_Start_IT(&htim3);//PPM检测
	
	HAL_TIM_Base_Start_IT(&htim2);//定时任务
	
	
	memset(&timer_flag,0,sizeof(timer_flag));
}





void timer_polling(void)
{

}






void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	
	
	if(htim->Instance == TIM3){
		ppm_interrupt();//PPM
	}
	
	if(htim->Instance == TIM2){
		//
		static uint16_t counter = 0;
		static uint16_t counter2 = 0;
		
		timer_flag.flag_1000hz = true;
		
//		if (counter % 2 == 0){
//			timer_flag.flag_500hz = true;
//		}
//		
//		if (counter % 5 == 0){
//			timer_flag.flag_200hz = true;
//		}
		
		if (counter % 10 == 0){
			timer_flag.flag_100hz = true;
		}
		
//		if (counter % 20 == 0){
//			timer_flag.flag_50hz = true;
//		}
//		
//		if (counter % 50 == 0){
//			timer_flag.flag_20hz = true;
//		}
//		
		if (counter % 100 == 0){
			timer_flag.flag_10hz = true;
		}
//		
//		if (counter % 200 == 0){
//			timer_flag.flag_5hz = true;
//		}
//		
//		if (counter % 500 == 0){
//			timer_flag.flag_2hz = true;
//		}
		
		if (counter == 0){
			timer_flag.flag_1hz = true;
		}
		
		if (counter2 == 0){
			timer_flag.flag_0_1hz = true;
		}
		
		counter2++;
		counter2%= 10000;
		counter++;
		counter%=1000;
		
		
		
		
		timer_flag.timestamps_ms++;
		
		
		
	}
	
//	if(htim->Instance == TIM7){
//		time_esp_ms++;
//	}
}





