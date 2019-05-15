#include "ppm.h"

#include <stdint.h>
#include <stdbool.h>

#include "tim.h"

//#include "mession.h"


//uint8_t ppm_value = 0;




//uint32_t middle_direct = 0;
//uint32_t up_direct = 0;
//uint32_t down_direct = 0;

//uint32_t middle_direct_last = 0;
//uint32_t up_direct_last = 0;
//uint32_t down_direct_last = 0;


extern void ppm0_change(bool up);
extern void ppm1_change(bool up);
extern void ppm2_change(bool up);


bool ppm_init_return_middle_flag = false;//PPM初始化回中标志

typedef enum{
	ppm_changed_none,
	ppm_changed_up,
	ppm_changed_middle,
	ppm_changed_down,
}ppm_changed_enum;

typedef struct{
	uint8_t ppm_sum;//PPM和
	
	
	uint8_t ppm_value;//当前PPM值
	uint32_t up_counter;//上面计数值
	uint32_t middle_counter;//中间计数值
	uint32_t down_counter;//下面计数值
	uint32_t up_counter_last;//上面计数值
	uint32_t middle_counter_last;//中间计数值
	uint32_t down_counter_last;//下面计数值
	
	ppm_changed_enum changed;
	ppm_changed_enum changed_last;
}ppm_type;







#define ppm_max  (3)
ppm_type ppm[ppm_max];


void ppm_init(void)
{

}




void ppm_interrupt(void)
{

	static uint8_t counter = 0;

	
	//通道0
	if (HAL_GPIO_ReadPin(PPM0_GPIO_Port,PPM0_Pin)){
		ppm[0].ppm_sum++;
	}
	
	//通道1
	if (HAL_GPIO_ReadPin(PPM1_GPIO_Port,PPM1_Pin)){
		ppm[1].ppm_sum++;
	}
	
	
	//通道2
	if (HAL_GPIO_ReadPin(PPM2_GPIO_Port,PPM2_Pin)){
		ppm[2].ppm_sum++;
	}
		

	

	counter++;
	counter%=250;
	if (counter == 0){
		
		for (int i=0;i<ppm_max;++i){
			
			ppm[i].ppm_value = ppm[i].ppm_sum++;
			ppm[i].ppm_sum = 0;
			
			if (ppm[i].ppm_value >=100 && ppm[i].ppm_value <= 200){
				ppm[i].middle_counter++;
			}else if (ppm[i].ppm_value <100){
				ppm[i].down_counter++;
			}else{
				ppm[i].up_counter++;
			}
		
		}

		
	}
		
}






//extern float oled_value; //当前设定水深

//float depth_sp_tab[] = {0.0f, 0.1f, 0.2f, 0.3f, 0.5f, 
//1.0f, 1.5f, 2.0f, 2.5f, 3.0f, 3.5, 4.0, 4.5, 5.0, 5.5, 6.0, 6.5, 7.0, 7.5, 8.0, 8.5, 9.0, 9.5, 10.0};
//uint8_t depth_sp_tab_idx = 0;//当前水深档位




void ppm_polling(void)
{
	
	
	for (int i=0;i<ppm_max;++i){
		if (ppm[i].down_counter!=ppm[i].down_counter_last){
			//变化了
			ppm[i].changed = ppm_changed_down;
		}
		
		if (ppm[i].up_counter!=ppm[i].up_counter_last){
			//变化了
			ppm[i].changed = ppm_changed_up;
		}
		
		if (ppm[i].middle_counter!=ppm[i].middle_counter_last){
			//变化了
			ppm[i].changed = ppm_changed_middle;
		}
	
	}
	
	
	
	
	
	
	for (int i=0;i<ppm_max;++i){
		ppm[i].down_counter_last = ppm[i].down_counter;
		ppm[i].up_counter_last = ppm[i].up_counter;
		ppm[i].middle_counter_last = ppm[i].middle_counter;
	}
	
	
	
	//up down
	if (ppm[0].changed == ppm_changed_up && ppm[0].changed_last != ppm_changed_up){
			ppm0_change(true);
	}else if (ppm[0].changed == ppm_changed_down && ppm[0].changed_last != ppm_changed_down){
			ppm0_change(false);
	}else{
		ppm[0].changed_last = ppm[0].changed;
	}
	ppm[0].changed_last = ppm[0].changed;
	

	if (ppm[1].changed == ppm_changed_up && ppm[1].changed_last != ppm_changed_up){
		ppm1_change(true);
	}else if (ppm[1].changed == ppm_changed_down && ppm[1].changed_last != ppm_changed_down){
		ppm1_change(false);
	}else{
		ppm[1].changed_last = ppm[1].changed;
	}
	ppm[1].changed_last = ppm[1].changed;

	if (ppm[2].changed == ppm_changed_up && ppm[2].changed_last != ppm_changed_up){
		ppm2_change(true);
	}else if (ppm[2].changed == ppm_changed_down && ppm[2].changed_last != ppm_changed_down){
		ppm2_change(false);
	}else{
		ppm[2].changed_last = ppm[2].changed;
	}
	ppm[2].changed_last = ppm[2].changed;
	
	
	
	
	#if 0
	//变化控制状态  [收线]
	if (ppm[0].changed == ppm_changed_up && ppm[0].changed_last != ppm_changed_up){
		//if(ppm_init_return_middle_flag)mession_back_request_force();//无论如何都收线
	}else if (ppm[0].changed == ppm_changed_middle && ppm[0].changed_last != ppm_changed_middle){
		//if (ppm_init_return_middle_flag) mession_stop_request_force();//无论如何都收线先暂停任务
		//else ppm_init_return_middle_flag = true;
	}else if (ppm[0].changed == ppm_changed_down && ppm[0].changed_last != ppm_changed_down){
		//if (ppm_init_return_middle_flag) mession_action_request();//开始任务
	}
	
	ppm[0].changed_last = ppm[0].changed;
	#endif
	
	
	
	
	
	
}
































































