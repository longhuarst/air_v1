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
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
// 	GPIO_Init(GPIOC, &GPIO_InitStructure);
	 
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_9; // SDIN--PC7	 RES_OLED--PC9
//	GPIO_Init(GPIOC, &GPIO_InitStructure);
// 	GPIO_SetBits(GPIOC,GPIO_Pin_9|GPIO_Pin_6|GPIO_Pin_7);//SDIN,SCLK,RES_OLED �����

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

//============��ʾһ��8*16���ַ�==============================
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr)
{unsigned char c=0,i=0;	
/*8x16������ϰ벿�ּ���8x8*/
	uint8_t j;
	uint8_t Data1,Data2,Data3,Data4,DATA1=0,DATA2=0;
			c=chr-' ';//�õ�ƫ�ƺ��ֵ
	Set_Column_Address(x,x+3);	
	
	Set_Row_Address(y,y+7);
	//Write_Command(0x5c);
			for(j=0;j<4;j++)
		{
			Data1=F8X16[c*16+2*j+0];//һ������ĸ��ֽ�
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
	
	/*****************8x16������°벿�ּ���8x8****************************************/
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


//��ʾһ���ַ��Ŵ�
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
{/*****************16x16������ϰ벿�֣�����8x16****************************************/
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
	
	/******************************16x16������°벿�ּ���8x16**************************************************/
	
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
//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);	 		  
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
//m^n����
uint32_t oled_pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}		


/**************************************
  ����ת�����򣺽�2λ�ֳ�1���ֽڴ����Դ棬����1��seg��ʾ4��������Ҫͬʱд2���ֽڼ�4������
  uchar DATA��ȡģ������ģ����
****************************************/
void Con_4_byte(unsigned char DATA)
{
   unsigned char d1_4byte[4],d2_4byte[4];
   unsigned char i;
   unsigned char d,k1,k2;
   d=DATA;
 
  for(i=0;i<2;i++)   // һ��λ�ķ�ʽд��  2*4=8λ
   {
     k1=d&0xc0;     //��i=0ʱ ΪD7,D6λ ��i=1ʱ ΪD5,D4λ

     /****��4�ֿ��ܣ�16���Ҷ�,һ���ֽ����ݱ�ʾ�������أ�һ�����ض�Ӧһ���ֽڵ�4λ***/

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
	  k2=d&0xc0;     //��i=0ʱ ΪD7,D6λ ��i=1ʱ ΪD5,D4λ

     /****��4�ֿ��ܣ�16���Ҷ�,һ���ֽ����ݱ�ʾ�������أ�һ�����ض�Ӧһ���ֽڵ�4λ***/

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
	  
	  d=d<<2;                                //������λ
      
	 OLED_WR_Byte(d1_4byte[i],OLED_DATA);	    //дǰ2��
	 OLED_WR_Byte(d2_4byte[i],OLED_DATA);               //д��2��	  ����4��
   }

}
 
//��SSD1322д��һ���ֽڡ�
//dat:Ҫд�������/����
//cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
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









unsigned char  FontBuf[128];//�ֿ⻺��	
void CL_Mem(void)
{
  unsigned char  i;
	 
	for(i=0;i<128;i++ )
	{
 //     Font_Map[i]=0;
	  FontBuf[i]=0;
	  }

}
void GBZK_ROMOUTSET(void)		//�ֿ��������
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
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	//��������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
//  	GPIO_Init(GPIOB, &GPIO_InitStructure);		  	 

}

void GBZK_ROMOUTRESET(void)	 	//�ֿ��������
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
//   	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//��©��� 
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
// 	GPIO_Init(GPIOB, &GPIO_InitStructure);		  	 
}

void GBZK_GPIO_Config(void)	 //�ֿ�оƬ��ʼ��
{		
	
//	GPIO_InitTypeDef GPIO_InitStructure;
//	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); 
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 ; //�ֿ�Ƭѡ	
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
//   	GPIO_Init(GPIOB, &GPIO_InitStructure);		  
// 
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	  //�ֿ�SPI���
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
//	GPIO_ResetBits(GPIOB,GPIO_Pin_3 );
}


/****��ָ��ֿ�IC***/
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
			SCLK_Clr();  //�ֿ�ʱ������ 
			SCLK_Set();  //�ֿ�ʱ������
			}

}

/****���ֿ�IC��ȡ���ֻ��ַ����ݣ�1���ֽڣ�***/
unsigned char  get_data_from_ROM(void)
{
	unsigned char  i;
	unsigned char  ret_data=0; //�������ݳ�ʼ��
	SCLK_Set();//�ֿ�ʱ������
	GBZK_ROMOUTSET();  //��������
	for(i=0;i<8;i++)
	{
		ZK_OUT_Set(); 
		SCLK_Clr();  //�ֿ�ʱ������  
		ret_data=ret_data<<1;
		if( ZK_OUT() )
			{ret_data=ret_data+1; 
			}
		else
			{ret_data=ret_data+0; 
			}
		SCLK_Set(); //�ֿ�ʱ������
	}
	GBZK_ROMOUTRESET();	 //��©���
	return(ret_data); //���ض�����һ���ֽ�
}

/*����ص�ַ��AddrHigh����ַ���ֽ�,AddrMid����ַ���ֽ�,AddrLow����ַ���ֽڣ�����������DataLen���ֽڵ����ݵ� pBuff�ĵ�ַ*/
/*������ȡ*/
void get_n_bytes_data_from_ROM(unsigned char  AddrHigh,unsigned char  AddrMid,unsigned char  AddrLow,unsigned char  *pBuff,unsigned char  DataLen )
{
	unsigned char  i;
	ZK_CS_Clr(); //�ֿ�Ƭѡ
	SCLK_Clr();  //�ֿ�ʱ������
	ZK_command(0x03);//дָ��
	ZK_command(AddrHigh);//д��ַ���ֽ�
	ZK_command(AddrMid);//д��ַ���ֽ�
	ZK_command(AddrLow);//д��ַ���ֽ�
	for(i = 0; i < DataLen; i++ )
	     *(pBuff+i) =get_data_from_ROM();//��һ���ֽ�����
	ZK_CS_Set();//ȡ���ֿ�Ƭѡ
}



 //*************************************************************************
 //****	Display_GB2312_String���������ַ�����ʾ����	***//
 //zk_num���壺���ֿ�ı�ʾ�� 1:GB2312_12*12,2:GB2312_15*16, 3:GB2312_24*24, 4:GB2312_32*32,
 //x�������� y��������
 //text[]��Ҫ��ʾ�ĺ���
 //**************************************************************************
void Display_GB2312_String(unsigned char zk_num,unsigned int  x,unsigned int  y, unsigned char  text[])
{
   
	unsigned char  i= 0;
	unsigned char  AddrHigh,AddrMid,AddrLow ; //�ָߡ��С��͵�ַ
	
	unsigned long  FontAddr=0; //�ֵ�ַ
	unsigned long  BaseAdd=0; //�ֿ����ַ	
	unsigned char  n= 0, d=0;// ��ͬ�����ֿ�ļ������
	
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
			 	 }//�������

		if(((text[i]>=0xA1) &&(text[i]<=0xA9))&&(text[i+1]>=0xA1))
		{						
			/*������壨GB2312�������� �ֿ�IC�еĵ�ַ�����¹�ʽ�����㣺*/
			/*Address = ((MSB - 0xA1) * 94 + (LSB - 0xA1))*n+ BaseAdd; ������ȡ��ַ*/
		 	FontAddr = (text[i]- 0xA1)*94; 
			FontAddr += (text[i+1]-0xA1);
			FontAddr = (unsigned long)((FontAddr*n)+BaseAdd);
			
			AddrHigh = (FontAddr&0xff0000)>>16;  /*��ַ�ĸ�8λ,��24λ*/
			AddrMid = (FontAddr&0xff00)>>8;      /*��ַ����8λ,��24λ*/
			AddrLow = FontAddr&0xff;	     /*��ַ�ĵ�8λ,��24λ*/
			get_n_bytes_data_from_ROM(AddrHigh,AddrMid,AddrLow,FontBuf,n );/*ȡһ�����ֵ����ݣ��浽"FontBuf[]"*/
			Display_GB2312(zk_num,x,y);//��ʾһ�����ֵ�OLED��/ 
					
			x=x+d/2; //��һ��������
			i+=2;  //�¸��ַ�
		}
		else if(((text[i]>=0xB0) &&(text[i]<=0xF7))&&(text[i+1]>=0xA1))
		{						
			/*������壨GB2312�� �ֿ�IC�еĵ�ַ�����¹�ʽ�����㣺*/
			/*Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+846)*n+ BaseAdd; ������ȡ��ַ*/
			 
			FontAddr = (text[i]- 0xB0)*94; 
			FontAddr += (text[i+1]-0xA1)+846;
			FontAddr = (unsigned long)((FontAddr*n)+BaseAdd);
			
			AddrHigh = (FontAddr&0xff0000)>>16;  /*��ַ�ĸ�8λ,��24λ*/
			AddrMid = (FontAddr&0xff00)>>8;      /*��ַ����8λ,��24λ*/
			AddrLow = FontAddr&0xff;	     /*��ַ�ĵ�8λ,��24λ*/
			get_n_bytes_data_from_ROM(AddrHigh,AddrMid,AddrLow,FontBuf,n );/*ȡһ�����ֵ����ݣ��浽"FontBuf[ ]"*/
			Display_GB2312(zk_num,x,y);//��ʾһ�����ֵ�OLED��/
	 		x=x+d/2; //��һ��������
			i+=2;  //�¸��ַ�

		}
		
	}
	
}

//***************************************************************
//  ��ʾ12*12 15*16 24*24 32*32 GB2312������ 2015-11���²���ͨ��
//   zk_num������ ���е�����  GB_1212 1616 2424 3232 
//   x: Start Column  ��ʼ�� 
//   y: Start Row   ��ʼ�� 0~63 
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
 //****	Display_GBasc_String����������չ�ַ��ַ�����ʾ����	***//
 //zk_num���壺 �ֿ�ı�ʾ�� 1: 6X12 �������չ�ַ�,2:8X16 �������չ�ַ�,3:8X16 �������չ�����ַ�, 4:12X24 �������չ�ַ�, 5:16X32 �������չ�ַ�
 //x�������� y��������
 //text[]��Ҫ��ʾ���ַ�
 //**************************************************************************
void Display_GBasc_String(unsigned char zk_num,unsigned int x, unsigned int y, unsigned char  text[])
{
   
	unsigned char  i= 0;
	unsigned short  FontCode  ; // �ַ�����
	unsigned char  AddrHigh,AddrMid,AddrLow ; //�ָߡ��С��͵�ַ

	unsigned long  FontAddr=0; //�ֵ�ַ
	unsigned long  BaseAdd=0; //�ֿ����ַ	
	unsigned char  n= 0, d=0;// ��ͬ�����ֿ�ļ������
	
	switch(zk_num)
	{
		case '1':  BaseAdd=0x1DBE0C; n=12 ;d=8;  break;	 //	   6X12 �������չ�ַ�
		case '2':  BaseAdd=0x1DD790; n=16;d=8;  break;	 //8X16 �������չ�ַ�
		case '3':  BaseAdd=0x1F2880; n=16;d=8;  break;	 //8X16 �������չ�����ַ�
	    case '4':  BaseAdd=0x1DFF30; n=48; d=12; break;	 //12X24 �������չ�ַ�
	    case '5':  BaseAdd=0x1E5A90; n=64;d=16; break;	 // 16X32 �������չ�ַ�
			}
		
	while((text[i]>0x00))
	{	
	     if(x>(256-d))
	      {  y=y+d; 
		     x=0; 
			 	 }//�������

 	     FontCode =	0xAA00|(text[i]|0x80);	//ascii���λΪ1
		if((FontCode>=0xAAA1) &&(FontCode<=0xAAFE))
		{						
		    FontAddr = 	FontCode-0xAAA1;
			FontAddr = (unsigned long)((FontAddr*n)+BaseAdd);
			AddrHigh = (FontAddr&0xff0000)>>16;  /*��ַ�ĸ�8λ,��24λ*/
			AddrMid = (FontAddr&0xff00)>>8;      /*��ַ����8λ,��24λ*/
			AddrLow = FontAddr&0xff;	     /*��ַ�ĵ�8λ,��24λ*/
			get_n_bytes_data_from_ROM(AddrHigh,AddrMid,AddrLow,FontBuf,n );/*ȡһ�����ֵ����ݣ��浽"FontBuf[]"*/
			Display_GBasc(zk_num,x,y);/*��ʾһ���ַ���OLED�� */
			i+=1;
			x+=d;//��һ��������
		 
		}
		else
		
		  if((FontCode>=0xABA1) &&(FontCode<=0xABC0))	 
		{						
		    FontAddr = 	(FontCode-0xABA1)+95;
			FontAddr = (unsigned long)((FontAddr*n)+BaseAdd);
			AddrHigh = (FontAddr&0xff0000)>>16;  /*��ַ�ĸ�8λ,��24λ*/
			AddrMid = (FontAddr&0xff00)>>8;      /*��ַ����8λ,��24λ*/
			AddrLow = FontAddr&0xff;	     /*��ַ�ĵ�8λ,��24λ*/
			get_n_bytes_data_from_ROM(AddrHigh,AddrMid,AddrLow,FontBuf,n );/*ȡһ�����ֵ����ݣ��浽"FontBuf[]"*/
			Display_GBasc(zk_num,x,y);/*��ʾһ���ַ���OLED�� */
			i+=1;
			x+=d;//��һ��������
		 
		}
		 else
			
			if((FontCode>=0xACA1) &&(FontCode<=0xACDF))		// 8X16 �������չ�����ַ�
		{						
		    FontAddr = 	FontCode-0xACA1;
			FontAddr = (unsigned long)((FontAddr*n)+BaseAdd);
			AddrHigh = (FontAddr&0xff0000)>>16;  //��ַ�ĸ�8λ,��24λ/
			AddrMid = (FontAddr&0xff00)>>8;      //��ַ����8λ,��24λ/
			AddrLow = FontAddr&0xff;	     //��ַ�ĵ�8λ,��24λ/
			get_n_bytes_data_from_ROM(AddrHigh,AddrMid,AddrLow,FontBuf,n );//ȡһ�����ֵ����ݣ��浽"FontBuf[]"/
			Display_GBasc(zk_num,x,y);//��ʾһ���ַ���OLED�� /
			i+=1;
			x+=d;//��һ��������
		 
		}


	}
	
}
 
//***************************************************************
//  ��ʾ6*12 8*16 12*24 16*32 GB2312����ASCII 2015-11���²���ͨ��
//zk_num���壺 �ֿ�ı�ʾ�� 1: 6X12 �������չ�ַ�,2:8X16 �������չ�ַ�,3:8X16 �������չ�����ַ�, 4:12X24 �������չ�ַ�, 5:16X32 �������չ�ַ�
//   x: Start Column  ��ʼ�� ��Χ 0~��256-16��
//   y: Start Row   ��ʼ�� 0~63 
//*************************************************************** 
void Display_GBasc( unsigned char zk_num,unsigned int x, unsigned int y )
{
    //extern unsigned char  FontBuf[128];//�ֿ⻺��	
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
		
	Set_Column_Address(Shift+x1,Shift+x1+x0); // ���������꣬shiftΪ��ƫ������1322������ 
	Set_Row_Address(y,y+y0); //����������
	//Set_Write_RAM();	 //	д�Դ�
	 
	for(j=0;j<n;j++)
	{
		 Con_4_byte(FontBuf[j]);//����ת������������������SSD1322
	}

}
 







 //*************************************************************************
 //****	Display_Asc_String  ASCII�ַ�����ʾ����	***//
 //zk_num���壺���ֿ�ı�ʾ�� 1:5x7 ASCII,2:7x8 ASCII, 3:6x12 ASCII, 4:8x16 ASCII,	5: 12x24 ASCII,6:16x32 ASCII;
 //x�������� y��������
 //text[]��Ҫ��ʾ��ASCII
 //**************************************************************************
void Display_Asc_String(unsigned char zk_num,unsigned int x, unsigned int y, unsigned char  text[])
{
   
	  
	unsigned char  i= 0;
	unsigned char  AddrHigh,AddrMid,AddrLow ; //�ָߡ��С��͵�ַ

	unsigned long  FontAddr=0; //�ֵ�ַ
	unsigned long  BaseAdd=0; //�ֿ����ַ	
    unsigned char  n,h,w,d,c;// ��ͬ�����ֿ�ļ������
    CL_Mem(); //�建������ ,����ᵼ��6*12��ʾ�쳣

	switch(zk_num)
	{	// n������h���ָ߶ȣ�w���ֿ�ȣ� d���ּ�࣬c��ҳ��С
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
			 	 }//�������


	   		if((text[i] >= 0x20) &&(text[i] <= 0x7E))
		{						
		    FontAddr = 	text[i]-0x20;
			FontAddr = (unsigned long)((FontAddr*n)+BaseAdd);
			
			AddrHigh = (FontAddr&0xff0000)>>16;  /*��ַ�ĸ�8λ,��24λ*/
			AddrMid = (FontAddr&0xff00)>>8;      /*��ַ����8λ,��24λ*/
			AddrLow = FontAddr&0xff;	     /*��ַ�ĵ�8λ,��24λ*/
			get_n_bytes_data_from_ROM(AddrHigh,AddrMid,AddrLow,FontBuf,n );/*ȡһ�����ֵ����ݣ��浽"FontBuf[]"*/

			Display_Asc(zk_num,x,y);/*��ʾһ��ascii��OLED�� */
            i+=1;
			x+=d/2;//��һ��������
		 
		}

	}
	
}


//***************************************************************
//  ��ʾ ASCII 2015-11���²���ͨ��
//zk_num���壺���ֿ�ı�ʾ�� 1:5x7 ASCII,2:7x8 ASCII, 3:6x12 ASCII, 4:8x16 ASCII,	5: 12x24 ASCII,6:16x32 ASCII;
//   x: Start Column  ��ʼ��  
//   y: Start Row   ��ʼ�� 0~63 
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
 // 	Display_GB2312_String���������ַ�����ʾ����	 
 //zk_num���壺���ֿ�ı�ʾ�� 1:GB2312_12*12,2:GB2312_16*16, 3:GB2312_24*24, 4:GB2312_32*32,
 //x�������� y��������
 //text[]��Ҫ��ʾ�ĺ���
 
void Display_Arial_String(unsigned char zk_num,unsigned char  x,unsigned char  y, unsigned char  text[])
{
   
	unsigned char  i= 2;
//	unsigned short  FontCode  ; // �ַ�����
	unsigned char  AddrHigh,AddrMid,AddrLow ; //�ָߡ��С��͵�ַ

	unsigned long  FontAddr=0; //�ֵ�ַ
	unsigned long  BaseAdd=0; //�ֿ����ַ	
	unsigned char  n= 0, d=0;// ��ͬ�����ֿ�ļ������
	
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
			
			AddrHigh = (FontAddr&0xff0000)>>16;  //��ַ�ĸ�8λ,��24λ/
			AddrMid = (FontAddr&0xff00)>>8;      //��ַ����8λ,��24λ/
			AddrLow = FontAddr&0xff;	     //��ַ�ĵ�8λ,��24λ/
			get_n_bytes_data_from_ROM(AddrHigh,AddrMid,AddrLow,FontBuf,n );//ȡһ�����ֵ����ݣ��浽"FontBuf[]"/

			Display_Asc(zk_num,x,y);//��ʾһ�����ֵ�OLED�� /
			i+=1;
			x+=d;//��һ��������
			if(x>(255-d))
			   {x=0; y+=d;}//�������
		}

	}
	
}











 //---------------------------------------------------------------------------------
 // 
 //zk_num���壺���ֿ�ı�ʾ�� 1:GB2312_12*12,2:GB2312_16*16, 3:GB2312_24*24, 4:GB2312_32*32,
 //x�������� y��������
 //text[]��Ҫ��ʾ�ĺ���
 // -----------------------------------------------------------------------------------
void Display_Arial_Asc_String(unsigned char zk_num,unsigned char  x,unsigned char  y, unsigned char  text[])
{
   
	unsigned char  i= 0;
	//	unsigned char  i= 0;
//	unsigned short  FontCode  ; // �ַ�����
	unsigned char  AddrHigh,AddrMid,AddrLow ; //�ָߡ��С��͵�ַ

	unsigned long  FontAddr=0; //�ֵ�ַ
	unsigned long  BaseAdd=0; //�ֿ����ַ	
    unsigned char  n= 0, d=0,c=0;// ��ͬ�����ֿ�ļ������
	
	switch(zk_num)
	{
		case '1':  BaseAdd=0x1DC400; n=26 ;d=8 ;c=8;  break;	 //	  12���󲻵ȿ�ARIAL ASCII
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
			
			AddrHigh = (FontAddr&0xff0000)>>16;  //��ַ�ĸ�8λ,��24λ 
			AddrMid = (FontAddr&0xff00)>>8;      //��ַ����8λ,��24λ 
			AddrLow = FontAddr&0xff;	     //��ַ�ĵ�8λ,��24λ /
			get_n_bytes_data_from_ROM(AddrHigh,AddrMid,AddrLow,FontBuf,n );//ȡһ�����ֵ����ݣ��浽"FontBuf[]" 

			Display_Arial_Asc(zk_num,x,y);//��ʾһ�����ֵ�OLED��  
            i+=1;
			x+=d;//��һ��������
			if(x>(255-d))
			   {x=0; y+=c;}//�������
		}

	}
	
}


 
//-----------------------------------------------------------------------
//  ��ʾ6*12 8*16 12*24 16*32 GB2312����ASCII 2015-11���²���ͨ��
//  ȡģ��ʽΪ������ȡģ���λ,��������:�����Ҵ��ϵ���   
//   zk_num�����ֿ��е�ASCII   GB ASCII_6X12 8X16 12X24 16X32 
//   x: Start Column  ��ʼ�� ��Χ 0~��256-16��
//   y: Start Row   ��ʼ�� 0~63 
//-------------------------------------------------------------------------------
void Display_Arial_Asc( unsigned char zk_num,unsigned char x, unsigned char y )
{
    //extern unsigned char  FontBuf[128];//�ֿ⻺��	
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
		
	Set_Column_Address(Shift+x1,Shift+x1+x0); // ���������꣬shiftΪ��ƫ������1322������ 
	Set_Row_Address(y,y+y0); //����������
	Set_Write_RAM();	 //	д�Դ�
	 
	for(j=2;j<n;j++)
	{
		 Con_4_byte(FontBuf[j]);//����ת������������������SSD1322
	}

}


 

void ZK_map( unsigned char *getdate,unsigned char *putdata,unsigned char high,unsigned char width)
{
	unsigned char i,j,hbyte,wbyte;
	unsigned char i_8,j_8;
	unsigned char  temp[32]; 
	wbyte = (width+7)/8;
	hbyte = (high+7)/8;

	   	for( i = 0; i < high; i++ )			  //  ���ú��� ת ���ú���
			for( j = 0; j < width; j++ )
			{
				i_8 = i/8;
				if((*(putdata+i_8*width+j)&(0x01<<(i%8))) > 0)
					temp[wbyte*i+j/8] |= (0x80>>(j%8));
				else
					temp[wbyte*i+j/8] &= (~(0x80>>(j%8)));
			   }


  		for( i = 0; i < high; i++ )		 //���ú��� ת ��������
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




//	Display_GB2312_String('1',0,0, "�о�԰���Ӽ���");
//	Display_GB2312_String('2',0,12, "�о�԰");
//	Display_GB2312_String('3',0,28, "�о�");
	//Display_GB2312_String('4',10,1, (unsigned char *)"������");


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
	
	//Display_GB2312_String('1',0,0, "�о�԰���Ӽ���1");
	
	//Display_Asc_String(6,0,0,"1124M");
	
	
	
//	if (mession_error){
//		
//		Display_GB2312_String('2',0,12, "����쳣");

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










