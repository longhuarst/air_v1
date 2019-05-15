#ifndef __oled_h
#define __oled_h



#include "gpio.h"



extern float oled_value;







#define	Shift		0x1C


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Global Variables
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#define Max_Column	0x3F			// 128/2-1 (Total Columns Devided by 2)
#define Max_Row		0x3F			// 64-1
#define	Brightness	0x7F
		    	
//-----------------OLED端口定义----------------  					   

//--------------------------------------------------------------------------
#define SCLK_Clr() HAL_GPIO_WritePin(OLED_SCL_GPIO_Port,OLED_SCL_Pin,GPIO_PIN_RESET)//oled与字库公用spi时钟
#define SCLK_Set() HAL_GPIO_WritePin(OLED_SCL_GPIO_Port,OLED_SCL_Pin,GPIO_PIN_SET)
 
#define SDIN_Clr() HAL_GPIO_WritePin(OLED_SDA_GPIO_Port,OLED_SDA_Pin,GPIO_PIN_RESET)//oled与字库公用spi输入
#define SDIN_Set() HAL_GPIO_WritePin(OLED_SDA_GPIO_Port,OLED_SDA_Pin,GPIO_PIN_SET)	

#define OLED_DC_Clr() HAL_GPIO_WritePin(OLED_DC_GPIO_Port,OLED_DC_Pin,GPIO_PIN_RESET)//oled 命令数据
#define OLED_DC_Set() HAL_GPIO_WritePin(OLED_DC_GPIO_Port,OLED_DC_Pin,GPIO_PIN_SET)

#define OLED_RST_Clr() HAL_GPIO_WritePin(OLED_RES_GPIO_Port,OLED_RES_Pin,GPIO_PIN_RESET)//oled RES
#define OLED_RST_Set() HAL_GPIO_WritePin(OLED_RES_GPIO_Port,OLED_RES_Pin,GPIO_PIN_SET)

#define OLED_CS_Clr()  HAL_GPIO_WritePin(OLED_CS1_GPIO_Port,OLED_CS1_Pin,GPIO_PIN_RESET)//oled片选
#define OLED_CS_Set()  HAL_GPIO_WritePin(OLED_CS1_GPIO_Port,OLED_CS1_Pin,GPIO_PIN_SET)

//----------------------------------------------------------------------------
#define ZK_CS_Clr()  HAL_GPIO_WritePin(OLED_CS2_GPIO_Port,OLED_CS2_Pin,GPIO_PIN_RESET)//字库片选
#define ZK_CS_Set()  HAL_GPIO_WritePin(OLED_CS2_GPIO_Port,OLED_CS2_Pin,GPIO_PIN_SET)					
	
#define ZK_OUT_Clr() HAL_GPIO_WritePin(OLED_FCO_GPIO_Port,OLED_FCO_Pin,GPIO_PIN_RESET)//字库spi数据输出
#define ZK_OUT_Set() HAL_GPIO_WritePin(OLED_FCO_GPIO_Port,OLED_FCO_Pin,GPIO_PIN_SET)	
				
#define ZK_OUT()    HAL_GPIO_ReadPin(OLED_FCO_GPIO_Port,OLED_FCO_Pin)//读取指定管脚的输入
//--------------------------------------------------------------------------------- 


  
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据
//OLED控制用函数




void Write_Register(unsigned char Data);
void Write_Parameter(unsigned char Data);
void Column_Address(unsigned char a, unsigned char b);
void Row_Address(unsigned char a, unsigned char b);
void ClearLED (unsigned char color);
void Set_Colume_Address(unsigned char Addr1,unsigned char Addr2);
void Set_Row_Address(unsigned char Addr1,unsigned char Addr2);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr);
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr);
uint32_t oled_pow(uint8_t m,uint8_t n);
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size);
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t *chn);
void OLED_DrawBMP(uint8_t *BMP,uint8_t Gray_Level,uint8_t Color);
void Initial_ssd1325(void);

void Show_Pattern(unsigned char *Data_Pointer, unsigned char a, unsigned char b, unsigned char c, unsigned char d);

void Set_Write_RAM(void);
void uDelay(unsigned char l);
void mDelay(unsigned char n);
void Write_Command(unsigned char Data);
void Write_Data(unsigned char Data);
void Set_Column_Address(unsigned char a, unsigned char b);
void Set_Row_Address(unsigned char a, unsigned char b);
void Set_Contrast_Current(unsigned char d);
void Set_Current_Range(unsigned char d);
void Set_Remap_Format(unsigned char d);
void Set_Start_Line(unsigned char d);
void Set_Display_Offset(unsigned char d);
void Set_Display_Mode(unsigned char d);
void Set_Multiplex_Ratio(unsigned char d);
void Set_Master_Config(unsigned char d);
void Set_Display_On_Off(unsigned char d);
void Set_Phase_Length(unsigned char d);
void Set_Frame_Frequency(unsigned char d);
void Set_Display_Clock(unsigned char d);
void Set_Precharge_Compensation(unsigned char a, unsigned char b);
void Set_Precharge_Voltage(unsigned char d);
void Set_VCOMH(unsigned char d);
void Set_VSL(unsigned char d);
void Set_NOP(void);
void GA_Option(unsigned char d);
void Draw_Rectangle(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e);
void Copy(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e, unsigned char f);
void Fill_RAM(unsigned char a);
void Clear_RAM(unsigned char a);
void Fill_Block(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e);
void Checkerboard(void);
void Grayscale(void);
void Frame(void);
void Show_Font57(unsigned char a, unsigned char b, unsigned char c, unsigned char d);
void Show_String(unsigned char a, unsigned char *Data_Pointer, unsigned char b, unsigned char c);
void Show_Pattern(unsigned char BMP[], unsigned char a, unsigned char b, unsigned char c, unsigned char d);
void Vertical_Scroll(unsigned char a, unsigned char b, unsigned char c);
void Horizontal_Scroll(unsigned char a, unsigned char b, unsigned char c, unsigned char d);
void Fade_Scroll(unsigned char a, unsigned char b, unsigned char c, unsigned char d);
void Deactivate_Scroll(void);
void Fade_In(void);
void Fade_Out(void);
void Sleep(unsigned char a);
void OLED_Init(void);
void OLED_WR_Byte(uint8_t dat,uint8_t cmd);
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr);
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t *chn);
	  
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size);
uint32_t oled_pow(uint8_t m,uint8_t n);

//硬字库函数
void GBZK_ROMOUTSET(void);		//字库输出设置 
void GBZK_ROMOUTRESET(void);	 	//字库输出设置
void GBZK_GPIO_Config(void);	 //字库芯片初始化
void ZK_command( unsigned char  data );
unsigned char  get_data_from_ROM(void);
void get_n_bytes_data_from_ROM(unsigned char  AddrHigh,unsigned char  AddrMid,unsigned char  AddrLow,unsigned char  *pBuff,unsigned char  DataLen );
void Display_GB2312_String(unsigned char zk_num,unsigned int x,unsigned int y, unsigned char  text[]);	// 显示汉字字符串
void Display_GB2312( unsigned char zk_num,unsigned int x,unsigned int y );								 //	显示汉字
void Display_GBasc_String(unsigned char zk_num,unsigned int x,unsigned int y , unsigned char  text[]);	  // 显示GB2312 ascii 字符串
void Display_GBasc( unsigned char zk_num,unsigned int x,unsigned int y  );								  //  显示GB2312 ascii 
void Display_Asc_String(unsigned char zk_num,unsigned int x,unsigned int y , unsigned char  text[]);		  //   显示ASCII 字符串
void Display_Asc( unsigned char zk_num,unsigned int x,unsigned int y);									  //   显示ASCII
void CL_Mem(void);
void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);






void oled_init(void);
void oled_polling(void);

void oled_loading(int i);


#endif


