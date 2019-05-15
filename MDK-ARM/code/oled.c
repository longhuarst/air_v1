#include "oled.h"

#include "main.h"

#include "gpio.h"

#include "oled_font.h"

#include <stdbool.h>
//#include "depth.h"

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Delay Time
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void uDelay(unsigned char l)
{
	while(l--);
}


void mDelay(unsigned char n)
{
unsigned char i,j,k;

	for(k=0;k<n;k++)
	{
				uDelay(1);	
	}
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Read/Write Sequence
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

void Write_Command(unsigned char Data)
{
unsigned char i;

	OLED_CS_Clr();
	OLED_DC_Clr();
	for (i=0; i<8; i++)
	{
		SCLK_Clr();
			if(Data&0x80)
		   SDIN_Set();
		else 
		   SDIN_Clr();
		Data = Data << 1;
//		uDelay(1);
		SCLK_Set();
//		uDelay(1);
	}
//	SCLK_Clr();
	OLED_DC_Set();
	OLED_CS_Set();
}

void Write_Data(unsigned char Data)
{
unsigned char i;

	OLED_CS_Clr();
	OLED_DC_Set();
	for (i=0; i<8; i++)
	{
		SCLK_Clr();
			if(Data&0x80)
		   SDIN_Set();
		else 
		   SDIN_Clr();
		Data = Data << 1;
//		uDelay(1);
		SCLK_Set();
//		uDelay(1);
	}
//	SCLK_Clr();
	OLED_DC_Set();
	OLED_CS_Set();
}



//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Instruction Setting
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Set_Column_Address(unsigned char a, unsigned char b)
{
	Write_Command(0x15);			// Set Column Address
	Write_Command(a);			//   Default => 0x00
	Write_Command(b);			//   Default => 0x3F (Total Columns Devided by 2)
}


void Set_Row_Address(unsigned char a, unsigned char b)
{
	Write_Command(0x75);			// Set Row Address
	Write_Command(a);			//   Default => 0x00
	Write_Command(b);			//   Default => 0x4F
}


void Set_Contrast_Current(unsigned char d)
{
	Write_Command(0x81);			// Set Contrast Value
	Write_Command(d);			//   Default => 0x40
}


void Set_Current_Range(unsigned char d)
{
	Write_Command(0x84|d);			// Set Current Range
						//   Default => 0x84
						//     0x84 (0x00) => Quarter Current Range
						//     0x85 (0x01) => Half Current Range
						//     0x86 (0x02) => Full Current Range
}


void Set_Remap_Format(unsigned char d)
{
	Write_Command(0xA0);			// Set Re-Map & Data Format
	Write_Command(d);			//   Default => 0x00
						//     Column Address 0 Mapped to SEG0
						//     Disable Nibble Re-Map
						//     Horizontal Address Increment
						//     Scan from COM0 to COM[N-1]
						//     Disable COM Split Odd Even
}


void Set_Start_Line(unsigned char d)
{
	Write_Command(0xA1);			// Set Display Start Line
	Write_Command(d);			//   Default => 0x00
}


void Set_Display_Offset(unsigned char d)
{
	Write_Command(0xA2);			// Set Display Offset
	Write_Command(d);			//   Default => 0x00
}


void Set_Display_Mode(unsigned char d)
{
	Write_Command(0xA4|d);			// Set Display Mode
						//   Default => 0xA4
						//     0xA4 (0x00) => Normal Display
						//     0xA5 (0x01) => Entire Display On, All Pixels Turn On at GS Level 15
						//     0xA6 (0x02) => Entire Display Off, All Pixels Turn Off
						//     0xA7 (0x03) => Inverse Display
}


void Set_Multiplex_Ratio(unsigned char d)
{
	Write_Command(0xA8);			// Set Multiplex Ratio
	Write_Command(d);			//   Default => 0x5F
}


void Set_Master_Config(unsigned char d)
{
	Write_Command(0xAD);			// Set Master Configuration
	Write_Command(0x02|d);			//   Default => 0x03
						//     0x02 (0x00) => Select External VCC Supply
						//     0x03 (0x01) => Select Internal DC/DC Voltage Converter
}


void Set_Display_On_Off(unsigned char d)
{
	Write_Command(0xAE|d);			// Set Display On/Off
						//   Default => 0xAE
						//     0xAE (0x00) => Display Off
						//     0xAF (0x01) => Display On
}


void Set_Phase_Length(unsigned char d)
{
	Write_Command(0xB1);			// Phase 1 & 2 Period Adjustment
	Write_Command(d);			//   Default => 0x53 (5 Display Clocks [Phase 2] / 3 Display Clocks [Phase 1])
						//     D[3:0] => Phase 1 Period in 1~15 Display Clocks
						//     D[7:4] => Phase 2 Period in 1~15 Display Clocks
}


void Set_Frame_Frequency(unsigned char d)
{
	Write_Command(0xB2);			// Set Frame Frequency (Row Period)
	Write_Command(d);			//   Default => 0x25 (37 Display Clocks)
}


void Set_Display_Clock(unsigned char d)
{
	Write_Command(0xB3);			// Display Clock Divider/Osciallator Frequency
	Write_Command(d);			//   Default => 0x41
						//     D[3:0] => Display Clock Divider
						//     D[7:4] => Oscillator Frequency
}


void Set_Precharge_Compensation(unsigned char a, unsigned char b)
{
	Write_Command(0xB4);			// Set Pre-Charge Compensation Level
	Write_Command(b);			//   Default => 0x00 (No Compensation)

	if(a == 0x20)
	{
		Write_Command(0xB0);		// Set Pre-Charge Compensation Enable
		Write_Command(0x08|a);		//   Default => 0x08
						//     0x08 (0x00) => Disable Pre-Charge Compensation
						//     0x28 (0x20) => Enable Pre-Charge Compensation
	}
}


void Set_Precharge_Voltage(unsigned char d)
{
	Write_Command(0xBC);			// Set Pre-Charge Voltage Level
	Write_Command(d);			//   Default => 0x10 (Connect to VCOMH)
}


void Set_VCOMH(unsigned char d)
{
	Write_Command(0xBE);			// Set Output Level High Voltage for COM Signal
	Write_Command(d);			//   Default => 0x1D (0.81*VREF)
}


void Set_VSL(unsigned char d)
{
	Write_Command(0xBF);			// Set Segment Low Voltage Level
	Write_Command(0x02|d);			//   Default => 0x0E
						//     0x02 (0x00) => Keep VSL Pin Floating
						//     0x0E (0x0C) => Connect a Capacitor between VSL Pin & VSS
}


void Set_NOP()
{
	Write_Command(0xE3);			// Command for No Operation
}







//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Graphic Acceleration
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void GA_Option(unsigned char d)
{
	Write_Command(0x23);			// Graphic Acceleration Command Options
	Write_Command(d);			//   Default => 0x01
						//     Enable Fill Rectangle
						//     Disable Wrap around in Horizontal Direction During Copying & Scrolling
						//     Disable Reverse Copy
}


void Draw_Rectangle(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e)
{
	Write_Command(0x24);			// Draw Rectangle
	Write_Command(a);			//   Column Address of Start
	Write_Command(c);			//   Row Address of Start
	Write_Command(b);			//   Column Address of End (Total Columns Devided by 2)
	Write_Command(d);			//   Row Address of End
	Write_Command(e);			//   Gray Scale Level
	uDelay(200);
}


void Copy(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e, unsigned char f)
{
	Write_Command(0x25);			// Copy
	Write_Command(a);			//   Column Address of Start
	Write_Command(c);			//   Row Address of Start
	Write_Command(b);			//   Column Address of End (Total Columns Devided by 2)
	Write_Command(d);			//   Row Address of End
	Write_Command(e);			//   Column Address of New Start
	Write_Command(f);			//   Row Address of New Start
	uDelay(200);
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Regular Pattern (Full Screen)
//
//    a: Two Pixels Data
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Fill_RAM(unsigned char a)
{
	GA_Option(0x01);
	Draw_Rectangle(0x00,0x3F,0x00,0x3F,a);
}
void Clear_RAM(unsigned char a)
{unsigned char x,y;

	Set_Column_Address(0x00,0x3f);	
	Set_Row_Address(0x00,0x3f);
	for(x=0;x<64;x++){
		for(y=0;y<64;y++)
			Write_Data (a);
	
	}
		
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Regular Pattern (Partial or Full Screen)
//
//    a: Column Address of Start
//    b: Column Address of End (Total Columns Devided by 2)
//    c: Row Address of Start
//    d: Row Address of End
//    e: Two Pixels Data
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Fill_Block(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e)
{
	GA_Option(0x01);
	Draw_Rectangle(a,b,c,d,e);
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Checkboard (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Checkerboard()
{
unsigned char i,j;
	
	Set_Column_Address(0x00,0x3F);
	Set_Row_Address(0x00,0x5F);

	for(i=0;i<40;i++)
	{
		for(j=0;j<64;j++)
		{
			Write_Data(0xF0);
		}
		for(j=0;j<64;j++)
		{
			Write_Data(0x0F);
		}
	}
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Gray Scale Bar (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Grayscale()
{
	//   Level 16 => Column 1~8
		Fill_Block(0x00,0x03,0x00,0x3F,0xFF);

	//   Level 15 => Column 9~16
		Fill_Block(0x04,0x07,0x00,0x3F,0xEE);

	//   Level 14 => Column 17~24
		Fill_Block(0x08,0x0B,0x00,0x3F,0xDD);

	//   Level 13 => Column 25~32
		Fill_Block(0x0C,0x0F,0x00,0x3F,0xCC);

	//   Level 12 => Column 33~40
		Fill_Block(0x10,0x13,0x00,0x3F,0xBB);

	//   Level 11 => Column 41~48
		Fill_Block(0x14,0x17,0x00,0x3F,0xAA);

	//   Level 10 => Column 49~56
		Fill_Block(0x18,0x1B,0x00,0x3F,0x99);

	//   Level 9 => Column 57~64
		Fill_Block(0x1C,0x1F,0x00,0x3F,0x88);

	//   Level 8 => Column 65~72
		Fill_Block(0x20,0x23,0x00,0x3F,0x77);

	//   Level 7 => Column 73~80
		Fill_Block(0x24,0x27,0x00,0x3F,0x66);

	//   Level 6 => Column 81~88
		Fill_Block(0x28,0x2B,0x00,0x3F,0x55);

	//   Level 5 => Column 89~96
		Fill_Block(0x2C,0x2F,0x00,0x3F,0x44);

	//   Level 4 => Column 97~104
		Fill_Block(0x30,0x33,0x00,0x3F,0x33);

	//   Level 3 => Column 105~112
		Fill_Block(0x34,0x37,0x00,0x3F,0x22);

	//   Level 2 => Column 113~120
		Fill_Block(0x38,0x3B,0x00,0x3F,0x11);

	//   Level 1 => Column 121~128
		Fill_Block(0x3C,0x3F,0x00,0x3F,0x00);
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Frame (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Frame()
{
	GA_Option(0x00);
	Draw_Rectangle(0x00,0x3F,0x00,0x3F,0xFF);
	Fill_Block(0x00,0x00,0x01,0x3E,0x0F);
	Fill_Block(0x3F,0x3F,0x01,0x3E,0xF0);
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Character (5x7)
//
//    a: Database
//    b: Ascii
//    c: Start X Address
//    d: Start Y Address
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Show_Font57(unsigned char a, unsigned char b, unsigned char c, unsigned char d)
{
unsigned char *Src_Pointer;
unsigned char i,Font,MSB,LSB;

	switch(a)
	{
		case 1:
		//	Src_Pointer=&Ascii_1[(b-1)][i];
			break;
		case 2:
		//	Src_Pointer=&Ascii_2[(b-1)][i];
			break;
	}

	Set_Remap_Format(0x54);
	Src_Pointer--;
	for(i=0;i<=4;i+=2)
	{
		LSB=*Src_Pointer;
		Src_Pointer++;
		if(i == 4)
		{
			MSB=0x00;
		}
		else
		{
			MSB=*Src_Pointer;
			Src_Pointer++;
		}
 		Set_Column_Address(c,c);
		Set_Row_Address(d,d+7);

		Font=((MSB&0x01)<<4)|(LSB&0x01);
		Font=Font|(Font<<1)|(Font<<2)|(Font<<3);
		Write_Data(Font);

		Font=((MSB&0x02)<<3)|((LSB&0x02)>>1);
		Font=Font|(Font<<1)|(Font<<2)|(Font<<3);
		Write_Data(Font);

		Font=((MSB&0x04)<<2)|((LSB&0x04)>>2);
		Font=Font|(Font<<1)|(Font<<2)|(Font<<3);
		Write_Data(Font);

		Font=((MSB&0x08)<<1)|((LSB&0x08)>>3);
		Font=Font|(Font<<1)|(Font<<2)|(Font<<3);
		Write_Data(Font);

		Font=((MSB&0x10)<<3)|((LSB&0x10)>>1);
		Font=Font|(Font>>1)|(Font>>2)|(Font>>3);
		Write_Data(Font);

		Font=((MSB&0x20)<<2)|((LSB&0x20)>>2);
		Font=Font|(Font>>1)|(Font>>2)|(Font>>3);
		Write_Data(Font);

		Font=((MSB&0x40)<<1)|((LSB&0x40)>>3);
		Font=Font|(Font>>1)|(Font>>2)|(Font>>3);
		Write_Data(Font);

		Font=(MSB&0x80)|((LSB&0x80)>>4);
		Font=Font|(Font>>1)|(Font>>2)|(Font>>3);
		Write_Data(Font);
		c++;
	}
	Set_Remap_Format(0x50);
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show String
//
//    a: Database
//    b: Start X Address
//    c: Start Y Address
//    * Must write "0" in the end...
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Show_String(unsigned char a, unsigned char *Data_Pointer, unsigned char b, unsigned char c)
{
unsigned char *Src_Pointer;

	Src_Pointer=Data_Pointer;
	Show_Font57(1,96,b,c);			// No-Break Space
						//   Must be written first before the string start...

	while(1)
	{
		Show_Font57(a,*Src_Pointer,b,c);
		Src_Pointer++;
		b+=3;
		if(*Src_Pointer == 0) break;
	}
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Pattern (Partial or Full Screen)
//
//    a: Column Address of Start
//    b: Column Address of End (Total Columns Devided by 2)
//    c: Row Address of Start
//    d: Row Address of End
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Show_Pattern(unsigned char BMP[], unsigned char a, unsigned char b, unsigned char c, unsigned char d)
{
unsigned char *Src_Pointer;
unsigned char i,j;
	 unsigned int K=0;
	

	Set_Column_Address(a,b);
	Set_Row_Address(c,d);

	for(i=0;i<(b-a+1);i++)
	{
		for(j=0;j<(d-c+1);j++)
		{
			Write_Data(BMP[K++]);
	
		}
	}
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Vertical Scrolling (Full Screen)
//
//    a: Scrolling Direction
//       "0x00" (Upward)
//       "0x01" (Downward)
//    b: Set Numbers of Row Scroll per Step
//    c: Set Time Interval between Each Scroll Step
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Vertical_Scroll(unsigned char a, unsigned char b, unsigned char c)
{
unsigned int i,j;	

	switch(a)
	{
		case 0:
			for(i=0;i<80;i+=b)
			{
				Set_Start_Line(i);
				for(j=0;j<c;j++)
				{
					uDelay(200);
				}
			}
			break;
		case 1:
			for(i=0;i<80;i+=b)
			{
				Set_Start_Line(80-i);
				for(j=0;j<c;j++)
				{
					uDelay(200);
				}
			}
			break;
	}
	Set_Start_Line(0x00);
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Continuous Horizontal Scrolling (Partial or Full Screen)
//
//    a: Set Numbers of Column Scroll per Step
//    b: Set Numbers of Row to Be Scrolled
//    c: Set Time Interval between Each Scroll Step in Terms of Frame Frequency
//    d: Delay Time
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Horizontal_Scroll(unsigned char a, unsigned char b, unsigned char c, unsigned char d)
{
	GA_Option(0x03);
	Write_Command(0x26);			// Horizontal Scroll Setup
	Write_Command(a);
	Write_Command(b);
	Write_Command(c);
	Write_Command(0x2F);			// Activate Scrolling
	mDelay(d);
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Continuous Horizontal Fade Scrolling (Partial or Full Screen)
//
//    a: Set Numbers of Column Scroll per Step
//    b: Set Numbers of Row to Be Scrolled
//    c: Set Time Interval between Each Scroll Step in Terms of Frame Frequency
//    d: Delay Time
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Fade_Scroll(unsigned char a, unsigned char b, unsigned char c, unsigned char d)
{
	GA_Option(0x01);
	Write_Command(0x26);			// Horizontal Scroll Setup
	Write_Command(a);
	Write_Command(b);
	Write_Command(c);
	Write_Command(0x2F);			// Activate Scrolling
	mDelay(d);
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Deactivate Scrolling (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Deactivate_Scroll()
{
	Write_Command(0x2E);			// Deactivate Scrolling
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Fade In (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Fade_In()
{
unsigned int i;	

	Set_Display_On_Off(0x01);
	for(i=0;i<(Brightness+1);i++)
	{
		Set_Contrast_Current(i);
    mDelay(1);
	}
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Fade Out (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Fade_Out()
{
unsigned int i;	

	for(i=(Brightness+1);i>0;i--)
	{
		Set_Contrast_Current(i-1);
    mDelay(1);
	}
	Set_Display_On_Off(0x00);
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Sleep Mode
//
//    "0x01" Enter Sleep Mode
//    "0x00" Exit Sleep Mode
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Sleep(unsigned char a)
{
	switch(a)
	{
		case 0:
			Set_Display_On_Off(0x00);
			Set_Display_Mode(0x01);
			break;
		case 1:
			Set_Display_Mode(0x00);
			Set_Display_On_Off(0x01);
			break;
	}
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Connection Test
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Test()
{
unsigned char i;

	OLED_RST_Clr();
	for(i=0;i<200;i++)
	{
		uDelay(200);
	}
	OLED_RST_Set();

	Set_Display_Mode(0x01);			// Entire Display On Mode (0x00/0x01/0x02/0x03)

	while(1)
	{
		Set_Display_On_Off(0x01);	// Display On (0x00/0x01)
		mDelay(2);
		Set_Display_On_Off(0x00);	// Display Off (0x00/0x01)
		mDelay(2);
	}
}


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Gray Scale Table Setting (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Set_Gray_Scale_Table()
{
	Write_Command(0xB8);			// Set Gray Scale Table
	Write_Command(0x01);			//   Gray Scale Level 1
	Write_Command(0x11);			//   Gray Scale Level 3 & 2
	Write_Command(0x22);			//   Gray Scale Level 5 & 4
	Write_Command(0x32);			//   Gray Scale Level 7 & 6
	Write_Command(0x43);			//   Gray Scale Level 9 & 8
	Write_Command(0x54);			//   Gray Scale Level 11 & 10
	Write_Command(0x65);			//   Gray Scale Level 13 & 12
	Write_Command(0x76);			//   Gray Scale Level 15 & 14
}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Initialization
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void OLED_Init()
{
 
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				 // SCLK --PC6 
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
// 	GPIO_Init(GPIOC, &GPIO_InitStructure);
	 
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_9; // SDIN--PC7	 RES_OLED--PC9
//	GPIO_Init(GPIOC, &GPIO_InitStructure);
// 	GPIO_SetBits(GPIOC,GPIO_Pin_9|GPIO_Pin_6|GPIO_Pin_7);//SDIN,SCLK,RES_OLED 输出高

	 OLED_RST_Clr();	
	mDelay(10);	
	OLED_RST_Set();

	Set_Display_On_Off(0x00);		// Display Off (0x00/0x01)
	Set_Display_Clock(0x91);		// Set Clock as 135 Frames/Sec
	Set_Multiplex_Ratio(0x3F);		// 1/64 Duty (0x0F~0x5F)
	Set_Display_Offset(0x4C);		// Shift Mapping RAM Counter (0x00~0x5F)
	Set_Start_Line(0x00);			// Set Mapping RAM Display Start Line (0x00~0x5F)
	Set_Remap_Format(0x50);			// Set Column Address 0 Mapped to SEG0
						//     Disable Nibble Remap
						//     Horizontal Address Increment
						//     Scan from COM[N-1] to COM0
						//     Enable COM Split Odd Even
	Set_Master_Config(0x00);		// Disable Embedded DC/DC Converter (0x00/0x01)
	Set_Contrast_Current(Brightness);	// Set Scale Factor of Segment Output Current Control
	Set_Current_Range(0x02);		// Set Full Current Range
	Set_Gray_Scale_Table();			// Set Pulse Width for Gray Scale Table
	Set_Frame_Frequency(0x46);		// Set Frame Frequency
	Set_Phase_Length(0x22);			// Set Phase 1 as 2 Clocks & Phase 2 as 2 Clocks
	Set_Precharge_Voltage(0x10);		// Set Pre-Charge Voltage Level
	Set_Precharge_Compensation(0x20,0x07);	// Set Pre-Charge Compensation
	Set_VCOMH(0x02);			// Set High Voltage Level of COM Pin
	Set_VSL(0x0C);				// Set Low Voltage Level of SEG Pin
	Set_Display_Mode(0x00);			// Normal Display Mode (0x00/0x01/0x02/0x03)

	Fill_RAM(0x00);				// Clear Screen

	Set_Display_On_Off(0x01);		// Display On (0x00/0x01)
}

//============显示一个8*16的字符==============================
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr)
{unsigned char c=0,i=0;	
/*8x16点阵的上半部分即上8x8*/
	uint8_t j;
	uint8_t Data1,Data2,Data3,Data4,DATA1=0,DATA2=0;
			c=chr-' ';//得到偏移后的值
	Set_Column_Address(x,x+3);	
	
	Set_Row_Address(y,y+7);
	//Write_Command(0x5c);
			for(j=0;j<4;j++)
		{
			Data1=F8X16[c*16+2*j+0];//一次输出四个字节
			Data2=F8X16[c*16+2*j+1];
	//		Data3=F8X16[c*16+4*j+2];
	//		Data4=F8X16[c*16+4*j+3];
			for(i=0;i<8;i++)
			{
				if(Data1&(0x01<<i))	
				{
					DATA1=0x0F;
				}
				if(Data2&(0x01<<i))
				{
					DATA1|=0xF0;
				}	
				Write_Data (DATA1);
				DATA1=0;
				if(Data3&(0x01<<i))	
				{
					DATA2=0x0F;
				}
				if(Data4&(0x01<<i))
				{
					DATA2|=0x0F;
				}
				
		//		Write_Data (DATA2);
				DATA2=0;
			}
		}
	
	/*****************8x16点阵的下半部分即下8x8****************************************/
	Set_Column_Address(x,x+3);	
	Set_Row_Address(y+8,y+15);
	Write_Command(0x5c);
				for(j=4;j<8;j++)
		{
			Data1=F8X16[c*16+2*j+0];
			Data2=F8X16[c*16+2*j+1];
		//	Data3=F8X16[c*16+4*j+2];
		//	Data4=F8X16[c*16+4*j+3];
			for(i=0;i<8;i++)
			{
				if(Data1&(0x01<<i))	
				{
					DATA1=0x0F;
				}
				if(Data2&(0x01<<i))
				{
					DATA1|=0xF0;
				}	
				Write_Data (DATA1);
				DATA1=0;
				if(Data3&(0x01<<i))	
				{
					DATA2=0xF0;
				}
				if(Data4&(0x01<<i))
				{
					DATA2|=0x0F;
				}
				
			//	Write_Data (DATA2);
				DATA2=0;
			}
		}
}	


//显示一个字符号串
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j]);
			x+=4;
			j++;
	}
}
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t *chn)
{/*****************16x16点阵的上半部分，即上8x16****************************************/
	uint8_t i,j;
	uint8_t Data1,Data2,Data3,Data4,DATA1=0,DATA2=0;
	//x_number=x_number/4;
	
	Set_Column_Address(x,x+7);	
	Set_Row_Address(y,y+7);

			for(j=0;j<8;j++)
		{
			Data1=*chn++;
			Data2=*chn++;

			for(i=0;i<8;i++)
			{
				if(Data1&(0x01<<i))	
				{
					DATA1=0x0F;
				}
				if(Data2&(0x01<<i))
				{
					DATA1|=0xF0;
				}	
				Write_Data (DATA1);
					DATA1=0;

			}
		}
	
	/******************************16x16点阵的下半部分即下8x16**************************************************/
	
Set_Column_Address(x,x+7);	
	Set_Row_Address(y+8,y+15);
	
		for(j=0;j<(8);j++)
		{
			Data1=*chn++;
			Data2=*chn++;

			for(i=0;i<8;i++)
			{
				if(Data1&(0x01<<i))	
				{
					DATA1=0x0F;
				}
				if(Data2&(0x01<<i))
				{
					DATA1|=0xF0;
				}	
				Write_Data (DATA1);
					DATA1=0;


			}
		}
}	
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/4)*t,y,' ');
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+4*t,y,temp+'0'); 
	}
}
//m^n函数
uint32_t oled_pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}		


/**************************************
  数据转换程序：将2位分成1个字节存入显存，由于1个seg表示4个列所以要同时写2个字节即4个像素
  uchar DATA：取模来的字模数据
****************************************/
void Con_4_byte(unsigned char DATA)
{
   unsigned char d1_4byte[4],d2_4byte[4];
   unsigned char i;
   unsigned char d,k1,k2;
   d=DATA;
 
  for(i=0;i<2;i++)   // 一两位的方式写入  2*4=8位
   {
     k1=d&0xc0;     //当i=0时 为D7,D6位 当i=1时 为D5,D4位

     /****有4种可能，16级灰度,一个字节数据表示两个像素，一个像素对应一个字节的4位***/

     switch(k1)
       {
	 case 0x00:
           d1_4byte[i]=0x00;
		   
         break;
     case 0x40:  // 0100,0000
           d1_4byte[i]=0x0f;
		   
         break;	
	 case 0x80:  //1000,0000
           d1_4byte[i]=0xf0;
		   
         break;
     case 0xc0:   //1100,0000
           d1_4byte[i]=0xff;
		  
         break;	 
     default:
      	 break;
	   }
     
	   d=d<<2;
	  k2=d&0xc0;     //当i=0时 为D7,D6位 当i=1时 为D5,D4位

     /****有4种可能，16级灰度,一个字节数据表示两个像素，一个像素对应一个字节的4位***/

     switch(k2)
       {
	 case 0x00:
           d2_4byte[i]=0x00;
		   
         break;
     case 0x40:  // 0100,0000
           d2_4byte[i]=0x0f;
		   
         break;	
	 case 0x80:  //1000,0000
           d2_4byte[i]=0xf0;
		 
         break;
     case 0xc0:   //1100,0000
           d2_4byte[i]=0xff;
		  
         break;	 
     default:
      	 break;
	   }
	  
	  d=d<<2;                                //左移两位
      
	 OLED_WR_Byte(d1_4byte[i],OLED_DATA);	    //写前2列
	 OLED_WR_Byte(d2_4byte[i],OLED_DATA);               //写后2列	  共计4列
   }

}
 
//向SSD1322写入一个字节。
//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(uint8_t dat,uint8_t cmd)
{	
	uint8_t i;			  
	if(cmd)
	  OLED_DC_Set();
	else 
	  OLED_DC_Clr();		  
	OLED_CS_Clr();
	for(i=0;i<8;i++)
	{			  
		SCLK_Clr();
		if(dat&0x80)
		   SDIN_Set();
		else 
		   SDIN_Clr();
		   SCLK_Set();
		dat<<=1;   
	}				 		  
	OLED_CS_Set();
	OLED_DC_Set();   	  
} 
void Set_Write_RAM()
{
 
	OLED_WR_Byte(0x5C,OLED_CMD);			// Enable MCU to Write into RAM
}
void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{

	Set_Column_Address(x1,x2);	
	
	Set_Row_Address(y1,y2);

}









unsigned char  FontBuf[128];//字库缓存	
void CL_Mem(void)
{
  unsigned char  i;
	 
	for(i=0;i<128;i++ )
	{
 //     Font_Map[i]=0;
	  FontBuf[i]=0;
	  }

}
void GBZK_ROMOUTSET(void)		//字库输出设置
{		
	GPIO_InitTypeDef GPIO_InitStruct;
	/*Configure GPIO pins : PDPin PDPin PDPin PDPin */
  GPIO_InitStruct.Pin = OLED_FCO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(OLED_FCO_GPIO_Port, &GPIO_InitStruct);
	
	
//	GPIO_InitTypeDef GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 ;	
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	//下拉输入
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
//  	GPIO_Init(GPIOB, &GPIO_InitStructure);		  	 

}

void GBZK_ROMOUTRESET(void)	 	//字库输出设置
{		
	
	GPIO_InitTypeDef GPIO_InitStruct;
	/*Configure GPIO pins : PDPin PDPin PDPin PDPin */
  GPIO_InitStruct.Pin = OLED_FCO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(OLED_FCO_GPIO_Port, &GPIO_InitStruct);
	
	
//	GPIO_InitTypeDef GPIO_InitStructure;
//  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 ;	
//   	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//开漏输出 
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
// 	GPIO_Init(GPIOB, &GPIO_InitStructure);		  	 
}

void GBZK_GPIO_Config(void)	 //字库芯片初始化
{		
	
//	GPIO_InitTypeDef GPIO_InitStructure;
//	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); 
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 ; //字库片选	
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
//   	GPIO_Init(GPIOB, &GPIO_InitStructure);		  
// 
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	  //字库SPI输出
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
//	GPIO_ResetBits(GPIOB,GPIO_Pin_3 );
}


/****送指令到字库IC***/
void ZK_command( unsigned char  data )
{
    unsigned char  i;
	 
	for(i=0;i<8;i++ )
	{
		if(data&0x80)
			{SDIN_Set(); 
			}
		else
			{SDIN_Clr(); 
			}
			data = data<<1;
			SCLK_Clr();  //字库时钟拉低 
			SCLK_Set();  //字库时钟拉高
			}

}

/****从字库IC中取汉字或字符数据（1个字节）***/
unsigned char  get_data_from_ROM(void)
{
	unsigned char  i;
	unsigned char  ret_data=0; //返回数据初始化
	SCLK_Set();//字库时钟拉高
	GBZK_ROMOUTSET();  //下拉输入
	for(i=0;i<8;i++)
	{
		ZK_OUT_Set(); 
		SCLK_Clr();  //字库时钟拉低  
		ret_data=ret_data<<1;
		if( ZK_OUT() )
			{ret_data=ret_data+1; 
			}
		else
			{ret_data=ret_data+0; 
			}
		SCLK_Set(); //字库时钟拉高
	}
	GBZK_ROMOUTRESET();	 //开漏输出
	return(ret_data); //返回读出的一个字节
}

/*从相关地址（AddrHigh：地址高字节,AddrMid：地址中字节,AddrLow：地址低字节）中连续读出DataLen个字节的数据到 pBuff的地址*/
/*连续读取*/
void get_n_bytes_data_from_ROM(unsigned char  AddrHigh,unsigned char  AddrMid,unsigned char  AddrLow,unsigned char  *pBuff,unsigned char  DataLen )
{
	unsigned char  i;
	ZK_CS_Clr(); //字库片选
	SCLK_Clr();  //字库时钟拉低
	ZK_command(0x03);//写指令
	ZK_command(AddrHigh);//写地址高字节
	ZK_command(AddrMid);//写地址中字节
	ZK_command(AddrLow);//写地址低字节
	for(i = 0; i < DataLen; i++ )
	     *(pBuff+i) =get_data_from_ROM();//读一个字节数据
	ZK_CS_Set();//取消字库片选
}



 //*************************************************************************
 //****	Display_GB2312_String（）汉字字符串显示程序	***//
 //zk_num定义：汉字库的标示符 1:GB2312_12*12,2:GB2312_15*16, 3:GB2312_24*24, 4:GB2312_32*32,
 //x：列坐标 y：行坐标
 //text[]：要显示的汉字
 //**************************************************************************
void Display_GB2312_String(unsigned char zk_num,unsigned int  x,unsigned int  y, unsigned char  text[])
{
   
	unsigned char  i= 0;
	unsigned char  AddrHigh,AddrMid,AddrLow ; //字高、中、低地址
	
	unsigned long  FontAddr=0; //字地址
	unsigned long  BaseAdd=0; //字库基地址	
	unsigned char  n= 0, d=0;// 不同点阵字库的计算变量
	
	switch(zk_num)
	{
		case '1':  BaseAdd=0x0; n=24 ;d=12;  break;
		case '2':  BaseAdd=0x2C9D0; n=32;d=16;  break;
	    case '3':  BaseAdd=0x68190; n=72; d=24; break;
	    case '4':  BaseAdd=0xEDF00; n=128;d=32; break;
			}
		
	while((text[i]>0x00))
	{
	    if(x>(64-d/2))
	      {  y=y+d; 
		     x=0; 
			 	 }//溢出换行

		if(((text[i]>=0xA1) &&(text[i]<=0xA9))&&(text[i+1]>=0xA1))
		{						
			/*国标简体（GB2312）汉字在 字库IC中的地址由以下公式来计算：*/
			/*Address = ((MSB - 0xA1) * 94 + (LSB - 0xA1))*n+ BaseAdd; 分三部取地址*/
		 	FontAddr = (text[i]- 0xA1)*94; 
			FontAddr += (text[i+1]-0xA1);
			FontAddr = (unsigned long)((FontAddr*n)+BaseAdd);
			
			AddrHigh = (FontAddr&0xff0000)>>16;  /*地址的高8位,共24位*/
			AddrMid = (FontAddr&0xff00)>>8;      /*地址的中8位,共24位*/
			AddrLow = FontAddr&0xff;	     /*地址的低8位,共24位*/
			get_n_bytes_data_from_ROM(AddrHigh,AddrMid,AddrLow,FontBuf,n );/*取一个汉字的数据，存到"FontBuf[]"*/
			Display_GB2312(zk_num,x,y);//显示一个汉字到OLED上/ 
					
			x=x+d/2; //下一个字坐标
			i+=2;  //下个字符
		}
		else if(((text[i]>=0xB0) &&(text[i]<=0xF7))&&(text[i+1]>=0xA1))
		{						
			/*国标简体（GB2312） 字库IC中的地址由以下公式来计算：*/
			/*Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+846)*n+ BaseAdd; 分三部取地址*/
			 
			FontAddr = (text[i]- 0xB0)*94; 
			FontAddr += (text[i+1]-0xA1)+846;
			FontAddr = (unsigned long)((FontAddr*n)+BaseAdd);
			
			AddrHigh = (FontAddr&0xff0000)>>16;  /*地址的高8位,共24位*/
			AddrMid = (FontAddr&0xff00)>>8;      /*地址的中8位,共24位*/
			AddrLow = FontAddr&0xff;	     /*地址的低8位,共24位*/
			get_n_bytes_data_from_ROM(AddrHigh,AddrMid,AddrLow,FontBuf,n );/*取一个汉字的数据，存到"FontBuf[ ]"*/
			Display_GB2312(zk_num,x,y);//显示一个汉字到OLED上/
	 		x=x+d/2; //下一个字坐标
			i+=2;  //下个字符

		}
		
	}
	
}

//***************************************************************
//  显示12*12 15*16 24*24 32*32 GB2312点阵汉字 2015-11晶奥测试通过
//   zk_num：汉字 库中的类型  GB_1212 1616 2424 3232 
//   x: Start Column  开始列 
//   y: Start Row   开始行 0~63 
//*************************************************************** 
void Display_GB2312( unsigned char zk_num,unsigned int x, unsigned int y )
{
	
	    unsigned char i  ,k,data;
	data=0;
	
   	switch(zk_num)
	{
	  case '1':   
			 {
		 	Address_set(x,y,x+7,y+11);
			 for(i=0;i<24;i++)		 
	{
		
		for(k=0;k<4;k++)
		{ 		     
		 	if((FontBuf[i]&(0x80>>k*2))!=0)
		{data=0x0F;} 
			if((FontBuf[i]&(0x80>>(k*2+1)))!=0)
		{data|=0xF0;} 
		 Write_Data(data);
			data=0;
		}
	
	 }
			 }break;    //	  5x7 ASCII
			 	  case '2':   
			 {
		 	Address_set(x,y,x+7,y+15);
			 for(i=0;i<32;i++)		 
	{
		
		for(k=0;k<4;k++)
		{ 		     
		 	if((FontBuf[i]&(0x80>>k*2))!=0)
		{data=0x0F;} 
			if((FontBuf[i]&(0x80>>(k*2+1)))!=0)
		{data|=0xF0;} 
		 Write_Data(data);
			data=0;
		}
	
	 }
			 }break;    //	  5x7 ASCII
			 	  case '3':   
			 {
		 	Address_set(x,y,x+11,y+23);
			 for(i=0;i<72;i++)		 
	{
		
		for(k=0;k<4;k++)
		{ 		     
		 	if((FontBuf[i]&(0x80>>k*2))!=0)
		{data=0x0F;} 
			if((FontBuf[i]&(0x80>>(k*2+1)))!=0)
		{data|=0xF0;} 
		 Write_Data(data);
			data=0;
		}
	
	 }
			 }break;    //	  5x7 ASCII
			 	  case '4':   
			 {
		 	Address_set(x,y,x+15,y+31);
			 for(i=0;i<128;i++)		 
	{
		
		for(k=0;k<4;k++)
		{ 		     
		 	if((FontBuf[i]&(0x80>>k*2))!=0)
		{data=0x0F;} 
			if((FontBuf[i]&(0x80>>(k*2+1)))!=0)
		{data|=0xF0;} 
		 Write_Data(data);
			data=0;
		}
	
	 }
			 }break;    //	  5x7 ASCII
	
	}
}





  //*************************************************************************
 //****	Display_GBasc_String（）国标扩展字符字符串显示程序	***//
 //zk_num定义： 字库的标示符 1: 6X12 点国标扩展字符,2:8X16 点国标扩展字符,3:8X16 点国标扩展特殊字符, 4:12X24 点国标扩展字符, 5:16X32 点国标扩展字符
 //x：列坐标 y：行坐标
 //text[]：要显示的字符
 //**************************************************************************
void Display_GBasc_String(unsigned char zk_num,unsigned int x, unsigned int y, unsigned char  text[])
{
   
	unsigned char  i= 0;
	unsigned short  FontCode  ; // 字符内码
	unsigned char  AddrHigh,AddrMid,AddrLow ; //字高、中、低地址

	unsigned long  FontAddr=0; //字地址
	unsigned long  BaseAdd=0; //字库基地址	
	unsigned char  n= 0, d=0;// 不同点阵字库的计算变量
	
	switch(zk_num)
	{
		case '1':  BaseAdd=0x1DBE0C; n=12 ;d=8;  break;	 //	   6X12 点国标扩展字符
		case '2':  BaseAdd=0x1DD790; n=16;d=8;  break;	 //8X16 点国标扩展字符
		case '3':  BaseAdd=0x1F2880; n=16;d=8;  break;	 //8X16 点国标扩展特殊字符
	    case '4':  BaseAdd=0x1DFF30; n=48; d=12; break;	 //12X24 点国标扩展字符
	    case '5':  BaseAdd=0x1E5A90; n=64;d=16; break;	 // 16X32 点国标扩展字符
			}
		
	while((text[i]>0x00))
	{	
	     if(x>(256-d))
	      {  y=y+d; 
		     x=0; 
			 	 }//溢出换行

 	     FontCode =	0xAA00|(text[i]|0x80);	//ascii最高位为1
		if((FontCode>=0xAAA1) &&(FontCode<=0xAAFE))
		{						
		    FontAddr = 	FontCode-0xAAA1;
			FontAddr = (unsigned long)((FontAddr*n)+BaseAdd);
			AddrHigh = (FontAddr&0xff0000)>>16;  /*地址的高8位,共24位*/
			AddrMid = (FontAddr&0xff00)>>8;      /*地址的中8位,共24位*/
			AddrLow = FontAddr&0xff;	     /*地址的低8位,共24位*/
			get_n_bytes_data_from_ROM(AddrHigh,AddrMid,AddrLow,FontBuf,n );/*取一个汉字的数据，存到"FontBuf[]"*/
			Display_GBasc(zk_num,x,y);/*显示一个字符到OLED上 */
			i+=1;
			x+=d;//下一个字坐标
		 
		}
		else
		
		  if((FontCode>=0xABA1) &&(FontCode<=0xABC0))	 
		{						
		    FontAddr = 	(FontCode-0xABA1)+95;
			FontAddr = (unsigned long)((FontAddr*n)+BaseAdd);
			AddrHigh = (FontAddr&0xff0000)>>16;  /*地址的高8位,共24位*/
			AddrMid = (FontAddr&0xff00)>>8;      /*地址的中8位,共24位*/
			AddrLow = FontAddr&0xff;	     /*地址的低8位,共24位*/
			get_n_bytes_data_from_ROM(AddrHigh,AddrMid,AddrLow,FontBuf,n );/*取一个汉字的数据，存到"FontBuf[]"*/
			Display_GBasc(zk_num,x,y);/*显示一个字符到OLED上 */
			i+=1;
			x+=d;//下一个字坐标
		 
		}
		 else
			
			if((FontCode>=0xACA1) &&(FontCode<=0xACDF))		// 8X16 点国标扩展特殊字符
		{						
		    FontAddr = 	FontCode-0xACA1;
			FontAddr = (unsigned long)((FontAddr*n)+BaseAdd);
			AddrHigh = (FontAddr&0xff0000)>>16;  //地址的高8位,共24位/
			AddrMid = (FontAddr&0xff00)>>8;      //地址的中8位,共24位/
			AddrLow = FontAddr&0xff;	     //地址的低8位,共24位/
			get_n_bytes_data_from_ROM(AddrHigh,AddrMid,AddrLow,FontBuf,n );//取一个汉字的数据，存到"FontBuf[]"/
			Display_GBasc(zk_num,x,y);//显示一个字符到OLED上 /
			i+=1;
			x+=d;//下一个字坐标
		 
		}


	}
	
}
 
//***************************************************************
//  显示6*12 8*16 12*24 16*32 GB2312点阵ASCII 2015-11晶奥测试通过
//zk_num定义： 字库的标示符 1: 6X12 点国标扩展字符,2:8X16 点国标扩展字符,3:8X16 点国标扩展特殊字符, 4:12X24 点国标扩展字符, 5:16X32 点国标扩展字符
//   x: Start Column  开始列 范围 0~（256-16）
//   y: Start Row   开始行 0~63 
//*************************************************************** 
void Display_GBasc( unsigned char zk_num,unsigned int x, unsigned int y )
{
    //extern unsigned char  FontBuf[128];//字库缓存	
	unsigned char j ,x0,y0,n;
	unsigned int  x1=x/4; 
		
	switch(zk_num)
	{
		case '1':  x0=1;y0=11; n=12;  break;
		case '2':  x0=1;y0=15; n=16;  break;
		case '3':  x0=1;y0=15; n=16;  break;
	    case '4':  x0=3;y0=23; n=48;  break;
	    case '5':  x0=3;y0=31; n=64;  break;
			}
		
	Set_Column_Address(Shift+x1,Shift+x1+x0); // 设置列坐标，shift为列偏移量由1322决定。 
	Set_Row_Address(y,y+y0); //设置行坐标
	//Set_Write_RAM();	 //	写显存
	 
	for(j=0;j<n;j++)
	{
		 Con_4_byte(FontBuf[j]);//数据转换，把数组数据送入SSD1322
	}

}
 







 //*************************************************************************
 //****	Display_Asc_String  ASCII字符串显示程序	***//
 //zk_num定义：汉字库的标示符 1:5x7 ASCII,2:7x8 ASCII, 3:6x12 ASCII, 4:8x16 ASCII,	5: 12x24 ASCII,6:16x32 ASCII;
 //x：列坐标 y：行坐标
 //text[]：要显示的ASCII
 //**************************************************************************
void Display_Asc_String(unsigned char zk_num,unsigned int x, unsigned int y, unsigned char  text[])
{
   
	  
	unsigned char  i= 0;
	unsigned char  AddrHigh,AddrMid,AddrLow ; //字高、中、低地址

	unsigned long  FontAddr=0; //字地址
	unsigned long  BaseAdd=0; //字库基地址	
    unsigned char  n,h,w,d,c;// 不同点阵字库的计算变量
    CL_Mem(); //清缓存数组 ,不清会导致6*12显示异常

	switch(zk_num)
	{	// n个数，h：字高度，w：字宽度， d：字间距，c：页大小
		case '1':  BaseAdd=0x1DDF80; n=8;  h=7;  w=5;  d=6 ; c=1;  break;	 //	  5x7 ASCII
		case '2':  BaseAdd=0x1DE280; n=8;  h=8;  w=7;  d=8;  c=1;  break;	 //   7x8 ASCII
		case '3':  BaseAdd=0x1DBE00; n=12; h=12; w=6;  d=6;  c=2;  break;	 //  6x12 ASCII
	   case '4':  BaseAdd=0x1DD780; n=16; h=16; w=8;  d=8;  c=2;  break;	 //  8x16 ASCII	
	   case '5':  BaseAdd=0x1DFF00; n=48; h=24; w=12; d=12; c=3;  break;	 //  12x24 ASCII
	 	case '6':  BaseAdd=0x1E5A50; n=64; h=32; w=16; d=16; c=4;  break;	 //  16x32 ASCII

			}
	while((text[i]>0x00))
	{	
		 if(x>(64-d/2))
	      {  y=y+c; 
		     x=0; 
			 	 }//溢出换行


	   		if((text[i] >= 0x20) &&(text[i] <= 0x7E))
		{						
		    FontAddr = 	text[i]-0x20;
			FontAddr = (unsigned long)((FontAddr*n)+BaseAdd);
			
			AddrHigh = (FontAddr&0xff0000)>>16;  /*地址的高8位,共24位*/
			AddrMid = (FontAddr&0xff00)>>8;      /*地址的中8位,共24位*/
			AddrLow = FontAddr&0xff;	     /*地址的低8位,共24位*/
			get_n_bytes_data_from_ROM(AddrHigh,AddrMid,AddrLow,FontBuf,n );/*取一个汉字的数据，存到"FontBuf[]"*/

			Display_Asc(zk_num,x,y);/*显示一个ascii到OLED上 */
            i+=1;
			x+=d/2;//下一个字坐标
		 
		}

	}
	
}


//***************************************************************
//  显示 ASCII 2015-11晶奥测试通过
//zk_num定义：汉字库的标示符 1:5x7 ASCII,2:7x8 ASCII, 3:6x12 ASCII, 4:8x16 ASCII,	5: 12x24 ASCII,6:16x32 ASCII;
//   x: Start Column  开始列  
//   y: Start Row   开始行 0~63 
//*************************************************************** 
void Display_Asc( unsigned char zk_num,unsigned int x, unsigned int y )
{
	    unsigned char i  ,k,data;
	data=0;
	
   	switch(zk_num)
	{
	  case '1':   
			 {
		 	Address_set(x,y,x+3,y+7);
			 for(i=0;i<7;i++)		 
	{
		
		for(k=0;k<4;k++)
		{ 		     
		 	if((FontBuf[i]&(0x80>>k*2))!=0)
		{data=0x0F;} 
			if((FontBuf[i]&(0x80>>(k*2+1)))!=0)
		{data|=0xF0;} 
		 Write_Data(data);
			data=0;
		}
	
	 }
			 }break;    //	  5x7 ASCII
			 
		case '2':   	
			 {
		 	Address_set(x,y,x+3,y+7);
			 for(i=0;i<8;i++)		 
	{
		for(k=0;k<4;k++)
		{ 		     
		 	if((FontBuf[i]&(0x80>>k*2))!=0)
		 {data=0x0F;} 
			if((FontBuf[i]&(0x80>>(k*2+1)))!=0)
		{data|=0xF0;} 
		 Write_Data(data);
			data=0;
		}
	
	 }
			 }break;   //	  7x8 ASCII
			 
			 
			 	    case '3':   
			 {
		 	Address_set(x,y,x+3,y+11);
			 for(i=0;i<12;i++)		 
	{
		for(k=0;k<4;k++)
		{ 		     
		 	if((FontBuf[i]&(0x80>>k*2))!=0)
		 {data=0x0F;} 
			if((FontBuf[i]&(0x80>>(k*2+1)))!=0)
		{data|=0xF0;} 
		 Write_Data(data);
			data=0;
		}
	
	 }
			 }break;  //  6x12 ASCII
case '4':   
			 {
		 	Address_set(x,y,x+3,y+15);
			 for(i=0;i<16;i++)		 
	{
		for(k=0;k<4;k++)
		{ 		     
		 	if((FontBuf[i]&(0x80>>k*2))!=0)
		 {data=0x0F;} 
			if((FontBuf[i]&(0x80>>(k*2+1)))!=0)
		{data|=0xF0;} 
		 Write_Data(data);
			data=0;
		}
	
	 }
			 }break;  //  8x18 ASCII
			 case '5':   
			 {
		 	Address_set(x,y,x+7,y+23);
			 for(i=0;i<48;i++)		 
	{
		for(k=0;k<4;k++)
		{ 		     
		 	if((FontBuf[i]&(0x80>>k*2))!=0)
		 {data=0x0F;} 
			if((FontBuf[i]&(0x80>>(k*2+1)))!=0)
		{data|=0xF0;} 
		 Write_Data(data);
			data=0;
		}
	
	 }
			 }break;  //  8x18 ASCII
			 	    case '6':   
			 {
		 	Address_set(x,y,x+7,y+31);
			 for(i=0;i<64;i++)		 
	{
		for(k=0;k<4;k++)
		{ 		     
		 	if((FontBuf[i]&(0x80>>k*2))!=0)
		 {data=0x0F;} 
		 else data=0;
			if((FontBuf[i]&(0x80>>(k*2+1)))!=0)
					{data|=0xF0;}
	 else data=0;					
		 Write_Data(data);
			data=0;
		}
	
	 }
			 }break;  //  6x12 ASCII
		 }
	
}






















/*

 //---------------------------------------------------------------------
 // 	Display_GB2312_String（）汉字字符串显示程序	 
 //zk_num定义：汉字库的标示符 1:GB2312_12*12,2:GB2312_16*16, 3:GB2312_24*24, 4:GB2312_32*32,
 //x：列坐标 y：行坐标
 //text[]：要显示的汉字
 
void Display_Arial_String(unsigned char zk_num,unsigned char  x,unsigned char  y, unsigned char  text[])
{
   
	unsigned char  i= 2;
//	unsigned short  FontCode  ; // 字符内码
	unsigned char  AddrHigh,AddrMid,AddrLow ; //字高、中、低地址

	unsigned long  FontAddr=0; //字地址
	unsigned long  BaseAdd=0; //字库基地址	
	unsigned char  n= 0, d=0;// 不同点阵字库的计算变量
	
	switch(zk_num)
	{
		case '1':  BaseAdd=0x1DC400; n=26 ;d=16 ;  break;	 //	  16x12 ASCII
		case '2':  BaseAdd=0x1DE280; n=8;d=7;  break;	 //   7x8 ASCII
	    case '3':  BaseAdd=0x1DBE00; n=12; d=6; break;	 //  6x12 ASCII
	    case '4':  BaseAdd=0x1DD780; n=16;d=8; break;	 //  8x16 ASCII
		case '5':  BaseAdd=0x1DFF00; n=48;d=12; break;	 //  12x24 ASCII
		case '6':  BaseAdd=0x1E5A50; n=64;d=16; break;	 //  16x32 ASCII

			}
		
	while((text[i]>0x00))
	{	
	   		if((text[i] >= 0x20) &&(text[i] <= 0x7E))
		{						
		 
		    FontAddr = 	text[i]-0x20;
			FontAddr = (unsigned long)((FontAddr*n)+BaseAdd);
			
			AddrHigh = (FontAddr&0xff0000)>>16;  //地址的高8位,共24位/
			AddrMid = (FontAddr&0xff00)>>8;      //地址的中8位,共24位/
			AddrLow = FontAddr&0xff;	     //地址的低8位,共24位/
			get_n_bytes_data_from_ROM(AddrHigh,AddrMid,AddrLow,FontBuf,n );//取一个汉字的数据，存到"FontBuf[]"/

			Display_Asc(zk_num,x,y);//显示一个汉字到OLED上 /
			i+=1;
			x+=d;//下一个字坐标
			if(x>(255-d))
			   {x=0; y+=d;}//溢出换行
		}

	}
	
}











 //---------------------------------------------------------------------------------
 // 
 //zk_num定义：汉字库的标示符 1:GB2312_12*12,2:GB2312_16*16, 3:GB2312_24*24, 4:GB2312_32*32,
 //x：列坐标 y：行坐标
 //text[]：要显示的汉字
 // -----------------------------------------------------------------------------------
void Display_Arial_Asc_String(unsigned char zk_num,unsigned char  x,unsigned char  y, unsigned char  text[])
{
   
	unsigned char  i= 0;
	//	unsigned char  i= 0;
//	unsigned short  FontCode  ; // 字符内码
	unsigned char  AddrHigh,AddrMid,AddrLow ; //字高、中、低地址

	unsigned long  FontAddr=0; //字地址
	unsigned long  BaseAdd=0; //字库基地址	
    unsigned char  n= 0, d=0,c=0;// 不同点阵字库的计算变量
	
	switch(zk_num)
	{
		case '1':  BaseAdd=0x1DC400; n=26 ;d=8 ;c=8;  break;	 //	  12点阵不等宽ARIAL ASCII
		case '2':  BaseAdd=0x1DE280; n=8;d=8; c=8;  break;	 //   7x8 ASCII
	    case '3':  BaseAdd=0x1DBE00; n=12; d=8;c=12; break;	 //  6x12 ASCII
	    case '4':  BaseAdd=0x1DD780; n=16;d=8;c=16; break;	 //  8x16 ASCII
		case '5':  BaseAdd=0x1DFF00; n=48;d=12; c=24;break;	 //  12x24 ASCII
		case '6':  BaseAdd=0x1E5A50; n=64;d=16;c=32; break;	 //  16x32 ASCII

			}
		
	while((text[i]>0x00))
	{	
	   		if((text[i] >= 0x20) &&(text[i] <= 0x7E))
		{						
		 
		    FontAddr = 	text[i]-0x20;
			FontAddr = (unsigned long)((FontAddr*n)+BaseAdd);
			
			AddrHigh = (FontAddr&0xff0000)>>16;  //地址的高8位,共24位 
			AddrMid = (FontAddr&0xff00)>>8;      //地址的中8位,共24位 
			AddrLow = FontAddr&0xff;	     //地址的低8位,共24位 /
			get_n_bytes_data_from_ROM(AddrHigh,AddrMid,AddrLow,FontBuf,n );//取一个汉字的数据，存到"FontBuf[]" 

			Display_Arial_Asc(zk_num,x,y);//显示一个汉字到OLED上  
            i+=1;
			x+=d;//下一个字坐标
			if(x>(255-d))
			   {x=0; y+=c;}//溢出换行
		}

	}
	
}


 
//-----------------------------------------------------------------------
//  显示6*12 8*16 12*24 16*32 GB2312点阵ASCII 2015-11晶奥测试通过
//  取模方式为：横向取模左高位,数据排列:从左到右从上到下   
//   zk_num：汉字库中的ASCII   GB ASCII_6X12 8X16 12X24 16X32 
//   x: Start Column  开始列 范围 0~（256-16）
//   y: Start Row   开始行 0~63 
//-------------------------------------------------------------------------------
void Display_Arial_Asc( unsigned char zk_num,unsigned char x, unsigned char y )
{
    //extern unsigned char  FontBuf[128];//字库缓存	
	unsigned char x1,j ,x0,y0,n;
	x1=x/4; 
		
	switch(zk_num)
	{
		case '1':  x0=FontBuf[1]/4;y0=12; n=26;  break;   //	  12x12 ASCII
		case '2':  x0=1;y0=7; n=8;  break;   //	  7x8 ASCII
	    case '3':  x0=1;y0=11; n=12;  break;   //  6x12 ASCII
	    case '4':  x0=1;y0=15; n=16;  break;   //  8x16 ASCII
	    case '5':  x0=3;y0=23; n=48;  break;   //  12x24 ASCII
	    case '6':  x0=3;y0=31; n=64;  break;   //  16x32 ASCII
			}
		
	Set_Column_Address(Shift+x1,Shift+x1+x0); // 设置列坐标，shift为列偏移量由1322决定。 
	Set_Row_Address(y,y+y0); //设置行坐标
	Set_Write_RAM();	 //	写显存
	 
	for(j=2;j<n;j++)
	{
		 Con_4_byte(FontBuf[j]);//数据转换，把数组数据送入SSD1322
	}

}


 

void ZK_map( unsigned char *getdate,unsigned char *putdata,unsigned char high,unsigned char width)
{
	unsigned char i,j,hbyte,wbyte;
	unsigned char i_8,j_8;
	unsigned char  temp[32]; 
	wbyte = (width+7)/8;
	hbyte = (high+7)/8;

	   	for( i = 0; i < high; i++ )			  //  竖置横排 转 横置横排
			for( j = 0; j < width; j++ )
			{
				i_8 = i/8;
				if((*(putdata+i_8*width+j)&(0x01<<(i%8))) > 0)
					temp[wbyte*i+j/8] |= (0x80>>(j%8));
				else
					temp[wbyte*i+j/8] &= (~(0x80>>(j%8)));
			   }


  		for( i = 0; i < high; i++ )		 //横置横排 转 横置竖排
			for( j = 0; j < width; j++ )
			{
				j_8 = j/8;
				if((*(temp+wbyte*i+j/8)&(0x80>>(j%8))) > 0)
					getdate[j_8*high+i] |= (0x80>>(j%8));
				else
					getdate[j_8*high+i] &= (~(0x80>>(j%8)));
			}

 }

  */



void oled_init(void)
{
	OLED_RST_Clr();	
	HAL_Delay(10);	
	OLED_RST_Set();

	Set_Display_On_Off(0x00);		// Display Off (0x00/0x01)
	Set_Display_Clock(0x91);		// Set Clock as 135 Frames/Sec
	Set_Multiplex_Ratio(0x3F);		// 1/64 Duty (0x0F~0x5F)
	Set_Display_Offset(0x4C);		// Shift Mapping RAM Counter (0x00~0x5F)
	Set_Start_Line(0x00);			// Set Mapping RAM Display Start Line (0x00~0x5F)
	Set_Remap_Format(0x50);			// Set Column Address 0 Mapped to SEG0
					//     Disable Nibble Remap
					//     Horizontal Address Increment
					//     Scan from COM[N-1] to COM0
					//     Enable COM Split Odd Even
	Set_Master_Config(0x00);		// Disable Embedded DC/DC Converter (0x00/0x01)
	Set_Contrast_Current(Brightness);	// Set Scale Factor of Segment Output Current Control
	Set_Current_Range(0x02);		// Set Full Current Range
	Set_Gray_Scale_Table();			// Set Pulse Width for Gray Scale Table
	Set_Frame_Frequency(0x46);		// Set Frame Frequency
	Set_Phase_Length(0x22);			// Set Phase 1 as 2 Clocks & Phase 2 as 2 Clocks
	Set_Precharge_Voltage(0x10);		// Set Pre-Charge Voltage Level
	Set_Precharge_Compensation(0x20,0x07);	// Set Pre-Charge Compensation
	Set_VCOMH(0x02);			// Set High Voltage Level of COM Pin
	Set_VSL(0x0C);				// Set Low Voltage Level of SEG Pin
	Set_Display_Mode(0x00);			// Normal Display Mode (0x00/0x01/0x02/0x03)

	Fill_RAM(0x00);				// Clear Screen

	Set_Display_On_Off(0x01);		// Display On (0x00/0x01)




///

	//Clear_RAM(0x00);




//	Display_GB2312_String('1',0,0, "中景园电子技术");
//	Display_GB2312_String('2',0,12, "中景园");
//	Display_GB2312_String('3',0,28, "中景");
	//Display_GB2312_String('4',10,1, (unsigned char *)"加载中");


	static char buffer[32] = "";
	
	sprintf(buffer,"a");
	
	Display_Asc_String('4',8,20,buffer);
	  
	  Clear_RAM(0x00);
	  
	  
	  sprintf(buffer,"Loading....",1);
	
	Display_Asc_String('4',8,20,buffer);
	  
}


float oled_value = 0.0f;
extern bool mession_error;

void oled_polling(void)
{
	//Clear_RAM(0x00);
	
	
	static bool first = true;
	
	if (first){
		first = false;
		
		Clear_RAM(0x00);
		
	}
	
	//Display_GB2312_String('1',0,0, "中景园电子技术1");
	
	//Display_Asc_String(6,0,0,"1124M");
	
	
	
//	if (mession_error){
//		
//		Display_GB2312_String('2',0,12, "深度异常");

//		return;
//	}
//	
	static char buffer[32] = "";
	
	sprintf(buffer,"%04.2fM  ",oled_value);
	
	Display_Asc_String('6',8,20,buffer);
	
	
//	
//	sprintf(buffer,"%04.2fM  ",deep_real);
//	Display_Asc_String('4',8,52,buffer);
	
	//Display_Asc_String('1',0,0,"aaa");
	
	
}
	








void oled_loading(int i)
{
	
	static int counter = 0;
	
	static char buffer[32] = "";
	
	Clear_RAM(0x00);
	
	if (counter % 4 == 0){
		sprintf(buffer,"Loading.");
	}
	
	if (counter % 4 == 1){
		sprintf(buffer,"Loading..");
	}
	
	
	if (counter % 4 == 2){
		sprintf(buffer,"Loading...");
	}
	
	
	if (counter % 4 == 3){
		sprintf(buffer,"Loading....");
	}
	
	
	Display_Asc_String('4',8,20,buffer);
	
	
	counter++;


}










