#include "mession.h"
#include "timer.h"
#include "gpio.h"






#define max_time_value (9)
uint8_t time_value[5] = {0,0,0,0,0};
uint8_t time_value_lock[5] = {0,0,0,0,0};


uint8_t current_index = 0;//当前序号

bool oled_update = true;//更新oled


uint8_t mst_state = 0;


extern bool inited_;

void ppm0_change(bool up)
{

	if (inited_ == false)
		return;

	if (mst_state != 0 && mst_state != 10){
		return;//开始取气后无法修改
	}

	oled_update = true;
	if (up){
		if (time_value[current_index] >= max_time_value){
			time_value[current_index] = max_time_value;
		}else{
			time_value[current_index]++;
		}
	}else{
		if (time_value[current_index] <= 0){
			time_value[current_index] = 0;
		}else{
			time_value[current_index]--;
		}
	}
}

 
void ppm1_change(bool up)
{
//	if (inited_ == false)
//		return;

//	if (mst_state != 0 && mst_state != 10){
//		return;//开始取气后无法修改
//	}

//	oled_update = true;
//	if (up){
//		time_value_lock[current_index] = time_value[current_index];
//		current_index++;
//		current_index%=5;
//	}else{
//		
//	}
}



uint64_t start_timestamp_;
uint64_t stop_timestamp_;


void ppm2_change(bool up)
{
	if (inited_ == false)
		return;

	if (up){
		//
		if (mst_state %2 == 0){ //
			mst_state++;//每次ppm改变都加1
			mst_state %= 10;
			start_timestamp_ = timer_flag.timestamps_ms;//记录当前时间戳
		}
		
	}
}





//timer_flag.timestamps_ms



void mession_init(void)
{

}





void mession_polling(void)
{
	
	
	
	switch(mst_state){
		case 0: //空闲
			//不需要做任何事
			break;
		case 1:
			//等待
			HAL_GPIO_WritePin(MOTOR1_GPIO_Port,MOTOR1_Pin,GPIO_PIN_SET);
			stop_timestamp_ = timer_flag.timestamps_ms;//记录当前时间戳
			if ((stop_timestamp_ - start_timestamp_) >= (time_value[0] * 8000)){
				mst_state++;//退出
				HAL_GPIO_WritePin(MOTOR1_GPIO_Port,MOTOR1_Pin,GPIO_PIN_RESET);
			}
			break;

		case 2:
			//空闲
			break;
		case 3:
			HAL_GPIO_WritePin(MOTOR2_GPIO_Port,MOTOR2_Pin,GPIO_PIN_SET);
			stop_timestamp_ = timer_flag.timestamps_ms;//记录当前时间戳
			if ((stop_timestamp_ - start_timestamp_) >= (time_value[0] * 8000)){
				mst_state++;//退出
				HAL_GPIO_WritePin(MOTOR2_GPIO_Port,MOTOR2_Pin,GPIO_PIN_RESET);
			}
			//等待
			break;
		case 4:
			//空闲
			break;
		case 5:
			HAL_GPIO_WritePin(MOTOR3_GPIO_Port,MOTOR3_Pin,GPIO_PIN_SET);
			stop_timestamp_ = timer_flag.timestamps_ms;//记录当前时间戳
			if ((stop_timestamp_ - start_timestamp_) >= (time_value[0] * 8000)){
				mst_state++;//退出
				HAL_GPIO_WritePin(MOTOR3_GPIO_Port,MOTOR3_Pin,GPIO_PIN_RESET);
			}
			//等待
			break;
		case 6:
			//空闲
			break;
		case 7:
			HAL_GPIO_WritePin(MOTOR4_GPIO_Port,MOTOR4_Pin,GPIO_PIN_SET);
			stop_timestamp_ = timer_flag.timestamps_ms;//记录当前时间戳
			if ((stop_timestamp_ - start_timestamp_) >= (time_value[0] * 8000)){
				mst_state++;//退出
				HAL_GPIO_WritePin(MOTOR4_GPIO_Port,MOTOR4_Pin,GPIO_PIN_RESET);
			}
			//等待
			break;
		case 8:
			//空闲
			break;
		case 9:
			HAL_GPIO_WritePin(MOTOR5_GPIO_Port,MOTOR5_Pin,GPIO_PIN_SET);
			stop_timestamp_ = timer_flag.timestamps_ms;//记录当前时间戳
			if ((stop_timestamp_ - start_timestamp_) >= (time_value[0] * 8000)){
				mst_state++;//退出
				HAL_GPIO_WritePin(MOTOR5_GPIO_Port,MOTOR5_Pin,GPIO_PIN_RESET);
			}
			//等待
			break;
		case 10:
			//结束一次任务
			break;
		default:
			break;

	}






	//用于显示 OLED？？
	if (oled_update){
		oled_update = false;
		
		static bool first = true;
		
		if (first == true){
			first = false;
			
			Clear_RAM(0x00);
		}
		


		static char buffer[32] = "";
	
		sprintf(buffer,"%2.1f L",
			time_value[0]/2.0f);
		
	Display_Asc_String('6',12,20,buffer);
	


	}

	




}


























