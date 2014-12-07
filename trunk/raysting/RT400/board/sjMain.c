#include "sjDefine.h"
#include "sjSerial.h"
#include "AD5791.h"
//#include "CH374INC.h"
#include "HAL.h"
#include "lcd.h"
#include "font.h"
#include "window.h"
#include "math.h"
#include <stdio.h>

//the MARK of header ender and error and quest and reset
#define  HMARK 0x55	//U
#define  EMARK 0x56 //V
#define  QMARK 0x57 //W
#define  RMARK 0x58 //X
  
//the length of the switch table ,
//which must lower than 3fh
#define  LINIT 32 

//for P1 output
//for P0 output
const unsigned char code tbl[8]={0x7F,0xBF,0xDF,0xEF,0xF7,0xFB,0xFD,0xFE};




//for Init use(regaddr of pc)
const unsigned char code inittbl[LINIT]={

/*********** 05-1-25  8:44 ****************/

//  FKI1,FKI2,FKI3,OKI4,PKTT,FHP9,FHP7,FST2,
    0x37,0x36,0x35,0x3D,0x33,0x32,0x31,0x30,

//  FHP3,FHN3,FHP4,FHN4,FST1,FHX1,FHX2,FFX1,  
	0x26,0x27,0x24,0x25,0x22,0x23,0x20,0x21,

//  FHP10,FHP1,FHN1,FHP2,FHN2,FHN5,FHP8,FHN6,
	0x17,0x16,0x15,0x14,0x13,0x12,0x11,0x10,

//  FKN1,FKN2,FKN3,FKN4,FKN5,OKN6,FHP6,FHP5
	0x06,0x07,0x04,0x05,0x02,0x0A,0x00,0x01
/*******************************************
//  FKI1,FKI2,FKI3,OKI4,PKTT,FHP9,FHP7,FST2,
    0x37,0x36,0x35,0x3D,0x33,0x17,0x11,0x30,

//  FHP3,FHN3,FHP4,FHN4,FST1,FHX1,FHX2,FFX1,  
	0x24,0x27,0x01,0x25,0x22,0x23,0x20,0x21,

//  FHP10,FHP1,FHN1,FHP2,FHN2,FHN5,FHP8,FHN6,
	0x32,0x14,0x15,0x26,0x13,0x12,0x32,0x10,

//  FKN1,FKN2,FKN3,FKN4,FKN5,OKN6,FHP6,FHP5
	0x06,0x07,0x04,0x05,0x02,0x0A,0x31,0x00
********************************************/
};

//when modification is applied,
//no address should be modified ,just set the mcu addr= 256,
//and add new item in mcuaddr
//C? IT IS SO EASY ,
//BUT ,A NEW REGISTER MODEL WILL BE APPLIED IN THE FUTURE.
/*	    board allocation (F,O)=P0*8+P1
		48,49	50,51	52,53	54,55	57,56	59,58	61,60	63,62	//HPo,HNo,??,HP0,H8,H7,STo,STi
		33,32	35,34	37,36	39,38	41,40	43,42	45,44	47,46   //HN4,HP4,HN3,HP3,HN6,HP6,HN5,HP5
		17,16	19,18	21,20	23,22	25,24	27,26	29,28	31,30	//KN4,KN3,KN2,KN1,H9,FX1,KTT,KN5
		0,1		2,3		4,5		6,7		9,8		11,10	13,12	15,14 	//H2,H1,HX2,HX1,KI4,KI3,KI2,KI1
const unsigned char pcaddr[] ={
		0x16,0x14,0x26,0x24,0x01,0x00,0x31,0x11,0x32,0x17,//FHP 0-9 (pc addr)
		0x1f,0x1d,0x2f,0x2d,0x08,0x09,0x38,0x18,0x3b,0x1e,//OHP 10-19 (pc addr)
		0x15,0x13,0x27,0x25,0x12,0x10,					  //FHN 20-25  (pc addr)
		0x1c,0x1a,0x2e,0x2c,0x1b,0x19,					  //OHN 26-31  (pc addr)
		0x06,0x07,0x04,0x05,0x02,0x03,					  //FKN 32-37  (pc addr)
		0x0f,0x0e,0x0d,0x0c,0x0b,0x0a,					  //OKN 38-42  (pc addr)
		0x37,0x36,0x35,0x34,							  //FKI 44-47	   (pc addr)
		0x3e,0x3f,0x3c,0x3d,							  //OKI 48-51	   (pc addr)
		0x23,0x20,0x21,0x33,0x22,0x30,					  //FHX1,FHX2,FFX1,PKTT,FSTI,FSTO 52-57  (pc addr)
		0x2a,0x29,0x28,0x3a,0x2b,0x39,					  //OHX1,OHX2,OFX1,NKTT,OSTI,OSTO, 58-63 (pc addr)
		0x41,0x40,0x43,0x42};							  //FHPO,OHP0,FHNO,OHNO  (pc addr)
const unsigned char mcuaddr[] ={
	     54,  2	,  0,	39,	 35,  47,  43,	59,  57, 25,   //FHP 0-9 (pc addr)
		 55,  3 ,  1,	38,	 34,  46,  42,	58,	 56, 24,   //OHP 10-19 (pc addr)
  		 255, 255,  37,  33,  45,   41,					   //FHN 20-25  (pc addr)
		 255, 255,  36,  32,  44,   40,                    //OHN 26-31  (pc addr)
		 23,   21,  19,  17,  31,  255, 		   		   //FKN 32-37  (pc addr) ?(54)
		 22,   20,  18,  16,  30,  255,	                   //OKN 38-42  (pc addr) ?(55)
		 15,   13,  11,  9,								   //FKI 44-47	   (pc addr)
		 14,   12,  10,  8,								   //OKI 48-51	   (pc addr)
		 6,   4,   27,  29,  63,  61, 					   //FHX1,FHX2,FFX1,PKTT,FSTI,FSTO 52-57  (pc addr)
		 7,   5,   26,  28,  62,  60,					   //OHX1,OHX2,OFX1,NKTT,OSTI,OSTO, 58-63 (pc addr)
		 48,  49,  50,  51};							   //FHPO,OHP0,FHNO,OHNO  (pc addr)

*/
const unsigned char code lut[]={
/*****05-1-25 8:44****
47,35,31,255,19,17,23,21,  //0-7
34,46,255,30,16,18,20,22,  //8-f
41,59,45,255,2,255,54,25,  //10-17
58,40,255,44,255,3,24,55,  //18-1f
4,27,63,6,39,33,0,37,      //20-27
26,5,7,62,32,38,36,1,      //28-2f
61,43,57,29,9,11,13,15,    //30-37
42,60,28,56,10,8,14,12,    //38-3f 
49,48,51,50				   //40-43
**********************/
/*****05-1-25 8:44****/
43,47,31,255,19,17,23,21,  //0-7  
46,42,255,30,16,18,20,22,  //8-f
41,57,45,255,0,255,2,54,  //10-17
56,40,255,44,255,1,55,3,  //18-1f
4,27,63,6,35,33,39,37,      //20-27
26,5,7,62,32,34,36,38,      //28-2f
61,59,25,29,9,11,13,15,    //30-37
58,60,28,24,10,8,14,12,    //38-3f 
49,48,51,50				   //40-43
/**********************/
};

//delay loop
void swiDelay(unsigned char dl1,unsigned char dl2)
{
	unsigned char i,j;
	for(i=0;i<dl1;i++)
		for(j=0;j<dl2;j++)
			;
}


void swiAction(unsigned char SwiID)
{
	unsigned char exID;

	if(SwiID >= sizeof(lut))
		return;

	exID = lut[SwiID];
	if(exID == 255)
		return;

	P1 = tbl[(exID&7)];
	P0 = tbl[(exID>>3)];
	swiDelay(0x1f,0xff);

	P1 = 0xff;
	P0 = 0xff;
	swiDelay(0x06,0xff);
}
void swiReset()
{
	unsigned char i;
	for(i=0;i<LINIT;i++)
	{
		swiAction(inittbl[i]);
	}
}

unsigned char delayc;

/*
*/
//end interface routine
extern int	test( void );
extern void DBGS(char* s);
extern void DBG(unsigned char);


//extern void btnencode_loop();
unsigned long count = 0;
BOOL once = 0;
void timer_isr(void) interrupt 1 using 1
{
	asp_handler();
	asp_handler2();
	TF0 = 0; //clear timer
}
extern int	test( void );
extern void sleepms(unsigned int);
#define ONEMS	1000

uchar dispbuf[20];
LABEL code bootup = {LBL_HZ16,30,30,7,"正在启动..."};

LABEL code statelbl = {LBL_HZ16,30,30,26,dispbuf};

uchar nav1v = 1;
void navto120mv()
{               
   nav1v = 0;  
   nav_command(NAV_120MV);
   sleepms(200*ONEMS);            
}           

void navto1v()
{            
    nav1v = 1;
    nav_command(NAV_1V);
    sleepms(200*ONEMS);
}

void DBGS(const char* s)
{
	while(*s != 0x00)
	{
		sjSerialSendByte(*s++);
	}
}

#define VX_MULTI	100.0
void main()
{
	unsigned char sid;//id of serial to send 
	BYTE temp,key;
	BOOL bOn = false;
	unsigned char bBlick = 0;
	double VxMeasure;   //feedback voltage reading
	double lastDirectOutput = 0; //current da output
	double VxOutput; //new da output
	double thisVx,volt;
	unsigned char bStableCnt = 0;
	long i=1000;


	IE = 0;//close int
				  // S1   CCP  SPI
	P_SW1 = 0x08  ; // 0 0  0 0  1 0   0  0
				  // X X PWM6  PWM2345 X  S4_S S3_S S2_S
    P_SW2 = 0x07  ; // 0 0  0      0     0    1    1     1

	AUXR = 0x10 ;// t0x12  t1x12  m0x6 t2r t2ct t2x12 extram s1st2
				 // 0       0      0    1   0     0      0     0
	T4T3M = 0x88;// t4r  t4ct  t4x12 t4clko t3r t3ct t3x12 t3clk
				 //	1     0     0      0      1  0    0     0
	INT_CLKO = 0x00;  //x ex4 ex3 ex2 x t2 t1 t0


	sjSerialInit();	//init COM1 and COM2 buffer

	CLK_DIV = 0x40; //MCKO_S1 MCKO_S0 ADRJ TX_RX MCLKO_2 CLKS2 S1 S0 //24Mhz
					//	0      1        0    0     0      0       0 0  
	//CLK_DIV = 0x80; //MCKO_S1 MCKO_S0 ADRJ TX_RX MCLKO_2 CLKS2 S1 S0  //12Mhz
					//	1      0        0    0     0      0       0 0  
	init_uart();  //fake serial port 3
	init_uart2(); //fake serial port 4

	asp_set_timer();


	sid = 0;
	
	AD5791Init();
	
//	sjRelayInit();

	/*
	*	Protocal routine: 
	*	1.	HMARK sid(!=HMARK) :set sid
	*   2.	normal HMARK is repeated
	*/

	//test();
	//mDelaymS(5000);


	//CH374_PORT_INIT( );  /* CH374接口初始化 */


//	mDelaymS(50);
//	CH374_PORT_INIT( );  /* CH374接口初始化 */
//	Init374Device( );  // 初始化USB设备



	TR1 = 1;		//ENABLE COM1
	AUXR |= 0x10;	//Enable COM2

	PT0 = 1; 		//improve timer0 interrupt priority
    ET0 = 1;        //enable timer0 interrupt

	IE = 0x92;//enable serial int and timer0 interrupt//IE=90
	IE2 = 1;
	EA = 1;

	DBGS("STARTUP DONE\r\n");	
//	swiReset();
	/*
	*	Protocal routine: 
	*	1.	HMARK sid(!=HMARK) :set sid
	*   2.	normal HMARK is repeated
	*/
//	swiDelay(0x0f,0xff);
//	sjSerialSendByte('!');

	    // intialize LED. 
    sleepms(20*ONEMS);
			DBGS("123");
    LCD_Init();
			DBGS("456");
    wnd_msgbox(&bootup);
			DBGS("789");
    //init the DMM
/*    nav_command(NAV_INIT);              
    sleepms(200*ONEMS);                                
    navto1v();
    nav_command(NAV_SLOWMODE);
    sleepms(200*ONEMS);
    nav_command(NAV_AFLTON);
    sleepms(200*ONEMS);
                     
    sleepms(2*ONEMS); //wait until all the node is ready after power up        
	key = KEY_INVALID;
*/
	display_buttons(KEY_BTN1,bOn); //bOn is false
	cm_ad5791(DACMD_RESCUE, 0);
	i =0;
	while(1)
	{
			if (i == 10)
			{
							cm_ad5791(DACMD_OUTPUT, 1.2);
			}
			if(i>20)
			{
				cm_ad5791(DACMD_OUTPUT, 1.2);
				i = 0;
			}
			i++;
	//		sjSerialSendByte2('?');
	//		DBGS("...");
					VxMeasure = nav_read() * VX_MULTI*10.0; //mulitplier is 100
						sprintf(dispbuf,"%.3f", VxMeasure);
						    sleepms(1000*ONEMS);        
					draw_label(&statelbl,SW_NORMAL);                        
						//	DBGS(dispbuf);
						//	DBGS("\r\n");
	}
	while(1)
	{
		if(key != KEY_INVALID)
		{
			if(key == KEY_BTN1)
			{
				bOn = 1 - bOn;
				display_buttons(KEY_BTN1,bOn);
			}
			if (bOn)
			{
				//display the feedback current and adjust it
				VxMeasure = nav_read() * VX_MULTI*10.0; //mulitplier is 100
				thisVx = VxMeasure;
				//variation < 5/10000 or < 1volt
				if( (fabs(VxOutput - thisVx) < 0.0005) ||
					(fabs(VxOutput - thisVx) < VxOutput*0.0005))
				{
					if(bStableCnt++ > 2)
						bStableCnt = 3;	//stick to 3
					sprintf(dispbuf,"%.3f", VxMeasure);
				}else{
					bStableCnt = 0;
					volt = (VxOutput - VxMeasure) / (2.0*VX_MULTI);
					if(fabs(volt) > (10 / 262144.0)) //bigger than 10 counts
					{
						lastDirectOutput = lastDirectOutput + volt;
						if(lastDirectOutput > 10)
							lastDirectOutput = 10;
						if(lastDirectOutput < 0)
							lastDirectOutput = 0;

						cm_ad5791(DACMD_OUTPUT, (lastDirectOutput));
					}
					//toggle Blick status
					bBlick = 1 + bBlick;
					if(bBlick > 3)
						bBlick = 0;

					if (bBlick == 0)
					{	sprintf(dispbuf,"%.3f -", VxMeasure);
					}else if (bBlick == 1){
						sprintf(dispbuf,"%.3f \\", VxMeasure);
					}else if (bBlick == 2){
						sprintf(dispbuf,"%.3f |", VxMeasure);
					}else{
						sprintf(dispbuf,"%.3f /", VxMeasure);
					}
				}
			}
		}
	}
}



