#ifndef __timer_h
#define __timer_h


#include <stdint.h>
#include <stdbool.h>



typedef struct{

	

	bool flag_1000hz;// = false;
	//bool flag_500hz;// = false;
	//bool flag_200hz;// = false;
	bool flag_100hz;// = false;
	//bool flag_50hz;// = false;
	//bool flag_20hz;// = false;
	bool flag_10hz;// = false;
	//bool flag_5hz;// = false;
	//bool flag_2hz;// = false;
	bool flag_1hz;// = false;
	bool flag_0_1hz;// = false;

	uint64_t timestamps_ms;
	
}time_flag_type;

extern time_flag_type timer_flag;

void timer_init(void);

void timer_polling(void);

#endif

