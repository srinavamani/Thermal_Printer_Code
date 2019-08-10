//***********************************************************************//
//                                                                       //
//                                                                       //
//      PRINTER DRIVER SOURCE FOR IMAGE PRINTING AND TEXT PRINTING       //
//                                                                       //
//                                                                       //
//***********************************************************************//

//-------Kernel Header files included------

#include<linux/module.h>
#include<linux/fs.h>
#include<linux/gpio.h>
#include<linux/delay.h>
#include <linux/cdev.h>
#include <linux/spi/spi.h>
#include <asm/uaccess.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/file.h>
#include <linux/proc_fs.h>

//-------Header files included------

#include "motor.h"
#include "final_font.h"

//----------- Tamil Fonts included -------

#include "tamil_fonts.h"

unsigned char Get_Tamil_Array[30];
unsigned char* tamilfont_generation(unsigned char* tamilunicode,unsigned int tamil_word_size);
unsigned int parse_char(char c);
char part1[2];
int tamil_letter_count,tamil;
char *senduart;

unsigned int parse_char(char c)
{
	if ('0' <= c && c <= '9') return c - '0';
	if ('a' <= c && c <= 'f') return 10 + c - 'a';
	if ('A' <= c && c <= 'F') return 10 + c - 'A';
}

//-------macros defined -------

#define SPI_BUFF_SIZE	128
#define USER_BUFF_SIZE	2048
#define DATA_BUFF_SIZE  102400

#define SPI_BUS 1
#define SPI_BUS_CS1 0
//#define SPI_BUS_SPEED 10000000
#define SPI_BUS_SPEED 750000

//------function decleration--------------

int Noofbytes(void);
void rotate(int);
int rotate_stat;
int bat_present=0;
static unsigned char langprintdata[DATA_BUFF_SIZE];

//------Driver name-----------------------
const char this_driver_name[] = "printer";
const char shared_driver_name[] = "iprinter";

struct timeval t;
struct tm broken;

//-------------
struct printer_control
{
	struct spi_message msg;
	struct spi_transfer transfer;
	u8 *tx_buff;
	u8 *rx_buff;
};
struct printer_control printer_ctl;

//------------
struct printer_dev {
	struct semaphore spi_sem;
	struct semaphore fop_sem;
	dev_t devt;
	struct cdev cdev;
	struct class *class;
	struct spi_device *spi_device;
	char *user_buff;
	u8 test_data;
};
struct printer_dev printer_dev;

struct PRINTER_image {
	dev_t devt;
	struct cdev cdev;
	char *user_buff;
};
struct PRINTER_image PRINTER_image;

struct bytedata {
	unsigned int bit7: 1;
	unsigned int bit6: 1;
	unsigned int bit5: 1;
	unsigned int bit4: 1;
	unsigned int bit3: 1;
	unsigned int bit2: 1;
	unsigned int bit1: 1;
	unsigned int bit0: 1;
};
struct bytedata pbyte;

//-------------------------variable declarations---------------------
char **buff,temp_image,q;
int length,i,k,z,n,y=100,g[2000],w,x,file=10,low_bat,rotate_pulse_count=1,rotate_loop=0,loop=0;
u8 tmp[48];
u32 *addr=&tmp;
char data_read[100];

char data[200];
int data_size=0,data_length,buffer_check1=0,allignment;
int start=0,end=0,j;
int no_of_lines=0,font_size,font_type;
int width=0,height,line_wise=0;
unsigned short int envy[50];
char tmp_buff[48];

//-----------------------paper sensing variable--------------------------

struct file *f,*f1,*fp_paper_sensing,*f_LB;
char buf1[1000000],paper[2];
mm_segment_t fs;
mm_segment_t new;

char filepath[128];
struct path path;
int error;
struct inode *inode;

static int lineprint_status;
static int printer_stop_status=0;

char *value;
unsigned char data1,data2,data3;
char m;

unsigned int NoOfBytes;
unsigned int Temp=0;
unsigned char Dummy=0;

struct file *f2;
char buf[2046],LB_buf1[5];
char *paper_value;
char paper_buff[6];

unsigned int paper_NoOfBytes;
unsigned int paper_Temp=0;
unsigned char paper_Dummy=0;

unsigned int printer_counter=0;

//--------------------------To find the no of bytes in the protocol & height of the image--------------

int Noofbytes()
{
	NoOfBytes=((g[2]&0x0F)<<12);
	NoOfBytes|=((g[3]&0x0F)<<8);
	NoOfBytes|=((g[4]&0x0F)<<4);
	NoOfBytes|=(g[5]&0x0F);

	Dummy=NoOfBytes>>12&0x0F;
	Temp=Temp*0x0A+Dummy;
	Dummy=NoOfBytes>>8&0x0F;
	Temp=Temp*0x0A+Dummy;
	Dummy=NoOfBytes>>4&0x0F;
	Temp=Temp*0x0A+Dummy;
	Dummy=NoOfBytes&0x0F;
	Temp=Temp*0x0A+Dummy;
	NoOfBytes=Temp;

	return NoOfBytes;
}

int strtoint(char *data)
{
	int bat;
	bat = ((data[0]-0x30)*10)+(data[1]-0x30);
	return bat;
}

//-----------------------------------Motor Rotation-------------------------------------------------

void rotate(int rotate_loop)
{
	if(gpio_get_value(162)==1)
	{
		printer_stop_status=0;
		for(loop=0;loop<rotate_loop;loop++)
		{
			if(rotate_pulse_count==1)
			{
				if(tamil == 1)
					lp_even_rotate();
				else
				{
					gpio_direction_output(134,1);
					gpio_direction_output(135,1);
					gpio_direction_output(136,1);
					even_rotate();
					gpio_direction_output(134,0);
					gpio_direction_output(135,0);
					gpio_direction_output(136,0);
				}

				rotate_pulse_count=2;
			}
			else if(rotate_pulse_count==2)
			{
				if(tamil == 1)
					lp_odd_rotate();
				else
				{
					gpio_direction_output(134,1);
					gpio_direction_output(135,1);
					gpio_direction_output(136,1);
					odd_rotate();
					gpio_direction_output(134,0);
					gpio_direction_output(135,0);
					gpio_direction_output(136,0);
				}

				rotate_pulse_count=1;
			}
		}
		gpio_direction_output(130,0);
		gpio_direction_output(131,0);
		gpio_direction_output(132,0);
		gpio_direction_output(133,0);
		gpio_direction_output(134,0);
		gpio_direction_output(135,0);
		gpio_direction_output(136,0);
	} else {
		printer_stop_status=1;
		if(printer_counter>0){
			lineprint_status=0;
		}
		do_gettimeofday(&t);
	}
}

//-------------------------------The program starts from here------------------------------------------

static void printer_prepare_spi_message(void)
{
	/***************Rotate Status Check***************/
	printer_counter=0;
	gpio_direction_output(137,1);
	udelay(100);
	memset(buf,0x00,128);

	/***************Rotate Status Check***************/
	data_read[48]=0,data_read[49]=1,data_read[50]=2,data_read[51]=3,data_read[52]=4,data_read[53]=5,data_read[54]=6,data_read[55]=7,data_read[56]=8,data_read[57]=9,data_read[65]=10,data_read[66]=11,data_read[67]=12,data_read[68]=13,data_read[69]=14,data_read[70]=15;

	paper_Temp=0;
	spi_message_init(&printer_ctl.msg);
	printer_dev.spi_device->bits_per_word=8;

	memset(tmp,0x00,48);

	for(k=0;k<=5;k++)
	{
		g[k]=(**(buff))-32;
		++(*buff);
	}

	Noofbytes();

	for(k=6; k<Temp; k++)
	{
		g[k]=(**(buff))-32;
		++(*buff);
	}

	//---------------------------------------PROTOCOL starting------------------------------------------
	// ~	Start Byte
	// ^!	End Byte

	if(g[0]+32 == '~' && g[Temp-2]+32 == '^' && g[Temp-1]+32 == '!')	// Start & End byte check
	{
		paper_Temp=0;

		switch(g[1])
		{
			//******************** Paper feed ****************************
			case 50: // R - motor rotation

				for(k=2;k<=5;k++)
					g[k]=g[k+4];

				Temp = 0;
				Noofbytes();

				for(i=0;i<Temp;i++)
					empty_rotate();

				Temp=0;
				break;

				//******************** TAMIL PRINTING *************************
			case 52:  //T ----------------------Tamil printing  g[1]

				tamil = 1;	// Flag - strobe heating for tamil fonts
				tamil_letter_count = 0;
				no_of_lines=0;

				for(k=6;k<=Temp-4;k++)
				{
					part1[0]=g[k]+32;
					k++;
					part1[1]=g[k]+32;
					Get_Tamil_Array[tamil_letter_count++] = parse_char(part1[0]) * 0x10 + parse_char(part1[1]);
				}
				GCu_StringSize=tamil_letter_count;

				senduart = tamilfont_generation(Get_Tamil_Array,GCu_StringSize);
				int main_count_x=0, main_count_j=0, main_count_z=0;
				for(main_count_x=0; main_count_x<26; main_count_x++)
				{
					main_count_z=0;
					for(main_count_j=0; main_count_j<48; main_count_j++)
						tmp[main_count_z++] = GCu_FinalBuffer2[(main_count_x*48)+main_count_j];

					spi_write(printer_dev.spi_device, addr, 48);

					if(g[Temp-3]+32 == 'L')	// Large Font
						rotate(2);
					else if(g[Temp-3]+32 == 'S')	// Small Font
						rotate(1);

					memset(tmp, 0, 48);
					memset(addr, 0, 48);
					memset(Get_Tamil_Array, 0, 30);
				}
				Temp=0;
				break;

				//******************** ENGLISH PRINTING *************************

			case 37:  //E ----------------------english printing  g[1]

				tamil = 0;
				no_of_lines=0;

				for(k=6;k<Temp-1;k++)
				{
					if(g[k]+32 == '~')
						no_of_lines++;
				}

				buffer_check1=6;
				lineprint_status=0;
				for(line_wise=0;line_wise<no_of_lines;line_wise++)
				{
					if(printer_stop_status==0)
					{
						lineprint_status++;
						do_gettimeofday(&t);
						printer_counter=line_wise;
					}

					data_size=1;
					do
					{
						data[data_size]=g[buffer_check1];
						data_size++;
						buffer_check1++;
					} while(g[buffer_check1]!=94);

					buffer_check1=buffer_check1+1;
					data_length=data_size;

					//---------------------------------------------------------------------------------------------
					switch (data[data_size-1])
					{
						case 44:
							allignment = 1;	// Left allignment
							break;
						case 50:
							allignment = 2;	// Right allignment
							break;
						case 35:
							allignment = 3;	// Center allignment
							break;
					}

					switch(data[data_size-3])
					{
						case 50:
							font_type = 1;	// Regular
							break;
						case 34:
							font_type = 2;	// Bold
							break;
					}

					switch(data[data_size-2])
					{
						case 51:
							font_size=1;		// Small
							if(data_size>42)
								data_size=42;
							break;
						case 45:
							font_size=2;		// Medium
							if(data_size>36)
								data_size=36;
							break;
					}

					//********** Small Font Printing **********
					if(font_size == 1)
					{
						memset(envy,0,48);

						for(height=0;height<10;height++)
						{
							for(width=1;width<=41 && width<(data_size-3);width++)
							{
								if(font_type == 1)
									envy[width]=Small_Regular[data[width]][height];
							}

							// Font width pixel count (10) 
							tmp_buff[0]=((envy[1] & 0xff00) >> 8 );
							tmp_buff[1]=((envy[1] & 0x00c0) | ((envy[2] & 0xfc00) >> 10));
							tmp_buff[2]=(((envy[2] & 0x03c0) >> 2 ) | ((envy[3] & 0xf000) >> 12));
							tmp_buff[3]=(((envy[3] & 0x0fc0) >> 4 ) | ((envy[4] & 0xc000) >> 14));
							tmp_buff[4]=((envy[4] & 0x3fc0) >> 6 );

							tmp_buff[5]=((envy[5] & 0xff00) >> 8 );
							tmp_buff[6]=((envy[5] & 0x00c0) | ((envy[6] & 0xfc00) >> 10));
							tmp_buff[7]=(((envy[6] & 0x03c0) >> 2 ) | ((envy[7] & 0xf000) >> 12));
							tmp_buff[8]=(((envy[7] & 0x0fc0) >> 4 ) | ((envy[8] & 0xc000) >> 14));
							tmp_buff[9]=((envy[8] & 0x3fc0) >> 6 );

							tmp_buff[10]=((envy[9] & 0xff00) >> 8 );
							tmp_buff[11]=((envy[9] & 0x00c0) | ((envy[10] & 0xfc00) >> 10));
							tmp_buff[12]=(((envy[10] & 0x03c0) >> 2 ) | ((envy[11] & 0xf000) >> 12));
							tmp_buff[13]=(((envy[11] & 0x0fc0) >> 4 ) | ((envy[12] & 0xc000) >> 14));
							tmp_buff[14]=((envy[12] & 0x3fc0) >> 6 );

							tmp_buff[15]=((envy[13] & 0xff00) >> 8 );
							tmp_buff[16]=((envy[13] & 0x00c0) | ((envy[14] & 0xfc00) >> 10));
							tmp_buff[17]=(((envy[14] & 0x03c0) >> 2 ) | ((envy[15] & 0xf000) >> 12));
							tmp_buff[18]=(((envy[15] & 0x0fc0) >> 4 ) | ((envy[16] & 0xc000) >> 14));
							tmp_buff[19]=((envy[16] & 0x3fc0) >> 6 );

							tmp_buff[20]=((envy[17] & 0xff00) >> 8 );
							tmp_buff[21]=((envy[17] & 0x00c0) | ((envy[18] & 0xfc00) >> 10));
							tmp_buff[22]=(((envy[18] & 0x03c0) >> 2 ) | ((envy[19] & 0xf000) >> 12));
							tmp_buff[23]=(((envy[19] & 0x0fc0) >> 4 ) | ((envy[20] & 0xc000) >> 14));
							tmp_buff[24]=((envy[20] & 0x3fc0) >> 6 );

							tmp_buff[25]=((envy[21] & 0xff00) >> 8 );
							tmp_buff[26]=((envy[21] & 0x00c0) | ((envy[22] & 0xfc00) >> 10));
							tmp_buff[27]=(((envy[22] & 0x03c0) >> 2 ) | ((envy[23] & 0xf000) >> 12));
							tmp_buff[28]=(((envy[23] & 0x0fc0) >> 4 ) | ((envy[24] & 0xc000) >> 14));
							tmp_buff[29]=((envy[24] & 0x3fc0) >> 6 );

							tmp_buff[30]=((envy[25] & 0xff00) >> 8 );
							tmp_buff[31]=((envy[25] & 0x00c0) | ((envy[26] & 0xfc00) >> 10));
							tmp_buff[32]=(((envy[26] & 0x03c0) >> 2 ) | ((envy[27] & 0xf000) >> 12));
							tmp_buff[33]=(((envy[27] & 0x0fc0) >> 4 ) | ((envy[28] & 0xc000) >> 14));
							tmp_buff[34]=((envy[28] & 0x3fc0) >> 6 );

							tmp_buff[35]=((envy[29] & 0xff00) >> 8 );
							tmp_buff[36]=((envy[29] & 0x00c0) | ((envy[30] & 0xfc00) >> 10));
							tmp_buff[37]=(((envy[30] & 0x03c0) >> 2 ) | ((envy[31] & 0xf000) >> 12));
							tmp_buff[38]=(((envy[31] & 0x0fc0) >> 4 ) | ((envy[32] & 0xc000) >> 14));
							tmp_buff[39]=((envy[32] & 0x3fc0) >> 6 );

							tmp_buff[40]=((envy[33] & 0xff00) >> 8 );
							tmp_buff[41]=((envy[33] & 0x00c0) | ((envy[34] & 0xfc00) >> 10));
							tmp_buff[42]=(((envy[34] & 0x03c0) >> 2 ) | ((envy[35] & 0xf000) >> 12));
							tmp_buff[43]=(((envy[35] & 0x0fc0) >> 4 ) | ((envy[36] & 0xc000) >> 14));
							tmp_buff[44]=((envy[36] & 0x3fc0) >> 6 );

							tmp_buff[45]=((envy[37] & 0xff00) >> 8 );
							tmp_buff[46]=((envy[37] & 0x00c0) | ((envy[38] & 0xfc00) >> 10));
							tmp_buff[47]=(((envy[38] & 0x03c0) >> 2 ) | ((envy[39] & 0xf000) >> 12));
							tmp_buff[48]=(((envy[39] & 0x0fc0) >> 4 ) | ((envy[40] & 0xc000) >> 14));
							tmp_buff[49]=((envy[40] & 0x3fc0) >> 6 );


							if(data_length>=38)
							{
								data_length=38;
							}

							if(allignment==1) // left allignment
							{
								start=((data_size-4)*12);
								start=384-start;
								start=((start/8));
								start=48-start;

								for(i=0;i<start;i++)
									tmp[i]=tmp_buff[i];
								j=0;

								for(i=start;i<48;i++)
									tmp[i]=0;
							}

							else if(allignment==2) // right allignment
							{
								start=((data_size-4)*10);
								start=384-start;
								start=((start/8));

								for(i=0;i<start;i++)
									tmp[i]=0;
								j=0;

								for(i=start;i<48;i++)
									tmp[i]=tmp_buff[j++];
							}

							else if(allignment==3) // center allignment
							{
								start=((data_size-4)*10);
								start=384-start;
								start=((start/2)+(start%2));
								start=((start/8));
								end=((data_size-4)*10);
								end=((end/8)+(end%8));
								end=(start+end);

								for(i=0;i<start;i++)
									tmp[i]=0;
								j=0;

								for(i=start;i<end;i++)
									tmp[i]=tmp_buff[j++];

								for(i=end;i<48;i++)
									tmp[i]=0;
							}

							spi_write(printer_dev.spi_device, addr, 48);
							rotate(2);

							memset(envy,0,48);

						}
					}  // small font - ends

					//********** Medium Font Printing **********
					if(font_size == 2)
					{
						memset(envy,0,48);

						for(height=0;height<12;height++)
						{
							for(width=1;width<=32 && width<(data_size-3);width++)
							{

								if(font_type == 1)
									envy[width] = Medium_Regular[data[width]][height];
								if(font_type == 2)
									envy[width] = Medium_Bold[data[width]][height];
							}

							// Font width pixel count (12)
							tmp_buff[0]=((envy[1] & 0xff00) >> 8 );
							tmp_buff[1]=((envy[1] & 0x00f0) | ((envy[2] & 0xf000) >> 12));
							tmp_buff[2]=((envy[2] & 0x0ff0) >> 4);

							tmp_buff[3]=((envy[3] & 0xff00) >> 8 );
							tmp_buff[4]=((envy[3] & 0x00f0) | ((envy[4] & 0xf000) >> 12));
							tmp_buff[5]=((envy[4] & 0x0ff0) >> 4);

							tmp_buff[6]=((envy[5] & 0xff00) >> 8 );
							tmp_buff[7]=((envy[5] & 0x00f0) | ((envy[6] & 0xf000) >> 12));
							tmp_buff[8]=((envy[6] & 0x0ff0) >> 4);

							tmp_buff[9]=((envy[7] & 0xff00) >> 8 );
							tmp_buff[10]=((envy[7] & 0x00f0) | ((envy[8] & 0xf000) >> 12));
							tmp_buff[11]=((envy[8] & 0x0ff0) >> 4);

							tmp_buff[12]=((envy[9] & 0xff00) >> 8 );
							tmp_buff[13]=((envy[9] & 0x00f0) | ((envy[10] & 0xf000) >> 12));
							tmp_buff[14]=((envy[10] & 0x0ff0) >> 4);

							tmp_buff[15]=((envy[11] & 0xff00) >> 8 );
							tmp_buff[16]=((envy[11] & 0x00f0) | ((envy[12] & 0xf000) >> 12));
							tmp_buff[17]=((envy[12] & 0x0ff0) >> 4);

							tmp_buff[18]=((envy[13] & 0xff00) >> 8 );
							tmp_buff[19]=((envy[13] & 0x00f0) | ((envy[14] & 0xf000) >> 12));
							tmp_buff[20]=((envy[14] & 0x0ff0) >> 4);

							tmp_buff[21]=((envy[15] & 0xff00) >> 8 );
							tmp_buff[22]=((envy[15] & 0x00f0) | ((envy[16] & 0xf000) >> 12));
							tmp_buff[23]=((envy[16] & 0x0ff0) >> 4);

							tmp_buff[24]=((envy[17] & 0xff00) >> 8 );
							tmp_buff[25]=((envy[17] & 0x00f0) | ((envy[18] & 0xf000) >> 12));
							tmp_buff[26]=((envy[18] & 0x0ff0) >> 4);

							tmp_buff[27]=((envy[19] & 0xff00) >> 8 );
							tmp_buff[28]=((envy[19] & 0x00f0) | ((envy[20] & 0xf000) >> 12));
							tmp_buff[29]=((envy[20] & 0x0ff0) >> 4);

							tmp_buff[30]=((envy[21] & 0xff00) >> 8 );
							tmp_buff[31]=((envy[21] & 0x00f0) | ((envy[22] & 0xf000) >> 12));
							tmp_buff[32]=((envy[22] & 0x0ff0) >> 4);

							tmp_buff[33]=((envy[23] & 0xff00) >> 8 );
							tmp_buff[34]=((envy[23] & 0x00f0) | ((envy[24] & 0xf000) >> 12));
							tmp_buff[35]=((envy[24] & 0x0ff0) >> 4);

							tmp_buff[36]=((envy[25] & 0xff00) >> 8 );
							tmp_buff[37]=((envy[25] & 0x00f0) | ((envy[26] & 0xf000) >> 12));
							tmp_buff[38]=((envy[26] & 0x0ff0) >> 4);

							tmp_buff[39]=((envy[27] & 0xff00) >> 8 );
							tmp_buff[40]=((envy[27] & 0x00f0) | ((envy[28] & 0xf000) >> 12));
							tmp_buff[41]=((envy[28] & 0x0ff0) >> 4);

							tmp_buff[42]=((envy[29] & 0xff00) >> 8 );
							tmp_buff[43]=((envy[29] & 0x00f0) | ((envy[30] & 0xf000) >> 12));
							tmp_buff[44]=((envy[30] & 0x0ff0) >> 4);

							tmp_buff[45]=((envy[31] & 0xff00) >> 8 );
							tmp_buff[46]=((envy[31] & 0x00f0) | ((envy[32] & 0xf000) >> 12));
							tmp_buff[47]=((envy[32] & 0x0ff0) >> 4);

							if(data_length>=32)
								data_length=32;

							if(allignment==1) // left allignment
							{
								start=((data_size-4)*12);
								start=384-start;
								start=((start/8));
								start=48-start;

								for(i=0;i<start;i++)
									tmp[i]=tmp_buff[i];
								j=0;

								for(i=start;i<48;i++)
									tmp[i]=0;
							}

							else if(allignment==2) // right allignment
							{
								start=((data_size-4)*12);
								start=384-start;
								start=((start/8));

								for(i=0;i<start;i++)
									tmp[i]=0;
								j=0;

								for(i=start;i<48;i++)
									tmp[i]=tmp_buff[j++];
							}

							else if(allignment==3) // center allignment
							{
								start=((data_size-4)*12);
								start=384-start;
								start=((start/2)+(start%2));
								start=((start/8));
								end=((data_size-4)*12);
								end=((end/8)+(end%8));
								end=(start+end);

								for(i=0;i<start;i++)
									tmp[i]=0;
								j=0;

								for(i=start;i<end;i++)
									tmp[i]=tmp_buff[j++];

								for(i=end;i<48;i++)
									tmp[i]=0;
							}


							spi_write(printer_dev.spi_device, addr, 48);
							rotate(2);

							memset(envy,0,48);
						}
					}	// Medium font - ends


					memset(tmp,0,48);
					memset(tmp_buff,0,48);
				}

				Temp=0;
				break;
				// --------------------- IMAGE PRINTING ----------------------------

			case 41: // I - Image printing

				memset(filepath,0x00,128);
				unsigned int PrintCount=0;

				low_bat=0;

				for(k=2;k<=5;k++)
				{
					g[k]=g[k+4];
				}
				Temp = 0;
				Noofbytes();

				memset(addr,0x00,48);
				memset(tmp,0x00,48);
				memset(buf1,0x00,1000000);

				sprintf(filepath,"/usr/share/status/PRINTER_image%c%c",(char *)g[6], (char *)g[7]);
				printk("FilePath: %s\n", filepath);

				f = filp_open(filepath, O_RDONLY, 0);
				if(IS_ERR(f)){
					printk(" [OPTIGOV] custom image file_open failed filp_open\n");
					f = filp_open("/usr/share/status/PRINTER_image", O_RDONLY, 0);
					if(IS_ERR(f)){
						printk(" [OPTIGOV] default image file_open failed filp_open\n");
						return NULL; // <FAILS HERE
					}else{
						fs = get_fs();
						set_fs(get_ds());
						f->f_op->read(f, buf1, 1000000, &f->f_pos);
						//                vfs_read(f, buf1, 1000000, &f->f_pos);
						set_fs(fs);
						value=buf1;
						filp_close(f, NULL);
					}
					// return NULL; // <FAILS HERE
				}else{
					fs = get_fs();
					set_fs(get_ds());
					f->f_op->read(f, buf1, 1000000, &f->f_pos);
					//                vfs_read(f, buf1, 1000000, &f->f_pos);
					set_fs(fs);
					value=buf1;
					filp_close(f, NULL);
				}

				lineprint_status=0;
				for(x=0;x<Temp;x++)
				{
					if(printer_stop_status==0) {
						lineprint_status++;
						do_gettimeofday(&t);
						printer_counter=line_wise;
					}
					for(i=0;i<49;i++)
					{
						if((*value)!=10)
						{
							data1=*value;
							value++;
							y++;
							data2=*value;
							value++;
							y++;

							data1=data_read[data1];
							data2=data_read[data2];

							data3=(((data1&0x0f)<<4) | (data2&0x0f));
							w=7;
							q=0;

							while(w>=0)
							{
								temp_image |= (((data3 >> q)&1)<< w);
								w--;
								q++;
							}
							tmp[i]=temp_image;
							temp_image=0;

						}

						else
						{
							i=49;
							value++;
						}

					}
					spi_write(printer_dev.spi_device, addr, 48);
					rotate(1);
				}
				rotate(5);

				Temp=0;
				paper_Temp=0;

				break;
		}
	}
	else
	{
		printk("Printer_Log: Invalid Packet\n");
		Temp = 0;
	}

	memset(addr,0,sizeof(addr));
	memset(tmp,0,sizeof(tmp));

	memset(printer_ctl.tx_buff, 0, SPI_BUFF_SIZE);
	spi_message_add_tail(&printer_ctl.transfer, &printer_ctl.msg);
	gpio_direction_output(137,0);
}

//---------------------------------------SPI system calls------------------------------------------//

static ssize_t printer_write(struct file* F, const char *buf[], size_t count, loff_t *f_pos)
{

	//printk(KERN_ALERT "count is ...........   %d  \n",count);
	//printk(KERN_ALERT "*buf is ...........   %d  \n",*buf);
	//printk(KERN_ALERT "buf+1  is ...........   %d  \n",*(buf+1));
	//printk(KERN_ALERT "checking ...........   %s  \n",buf);

	buff=&buf;
	length=count;

	ssize_t status = 0;

	if (down_interruptible(&printer_dev.spi_sem))
		return -ERESTARTSYS;

	if (!printer_dev.spi_device)
	{		up(&printer_dev.spi_sem);
		return -ENODEV;
	}

	printer_prepare_spi_message();

	status = spi_sync(printer_dev.spi_device, &printer_ctl.msg);
	up(&printer_dev.spi_sem);


	if ( copy_from_user(printer_dev.user_buff, buf, count) )
	{
		return -EFAULT;
	}

	if (status)
	{
		sprintf(printer_dev.user_buff,
				"printer_do_one_message failed : %d\n",
				status);

	}
	else
	{
		sprintf(printer_dev.user_buff,
				"Status: %d\n printing ..........  \n");
	}

	up(&printer_dev.fop_sem);
	return count;
}

//------------------------
static ssize_t printer_read(struct file *filp, char __user *buff, size_t count,
		loff_t *offp)
{

	size_t len;
	ssize_t status = 0;

	if (!buff)
		return -EFAULT;

	if (*offp > 0)
		return 0;

	if (down_interruptible(&printer_dev.fop_sem))
		return -ERESTARTSYS;


	//        status = printer_do_one_message();

	//        if (status)
	//        {
	//                sprintf(printer_dev.user_buff,
	//                        "printer_do_one_message failed : %d\n",
	//                        status);
	//        }
	//        else
	//        {

	//sprintf(printer_dev.user_buff,"PrintedLine: %d-%ld%03lu\n", lineprint_status,t,t.tv_usec); //timestamp (16dig) in milliseconds
	sprintf(printer_dev.user_buff,"PrintedLine: %d-%ld\n", lineprint_status,t); //timestamp (10dig) in seconds
	//        }

	//    do_gettimeofday(&t);
	//    time_to_tm(t.tv_sec, 0, &broken);
	//    printk("%ld\n", t);
	//    printk("%d:%d:%d:%ld\n", broken.tm_hour, broken.tm_min, broken.tm_sec, t.tv_usec);

	len = strlen(printer_dev.user_buff);

	if (len < count)
		count = len;

	if (copy_to_user(buff, printer_dev.user_buff, count))  {
		printk(KERN_ALERT "printer_read(): copy_to_user() failed\n");
		status = -EFAULT;
	} else {
		*offp += count;
		status = count;
	}

	up(&printer_dev.fop_sem);
	return status;
}

//---------------------
static int printer_open(struct inode *inode, struct file *filp)
{	
	int status = 0;

	if (down_interruptible(&printer_dev.fop_sem))
		return -ERESTARTSYS;

	if (!printer_dev.user_buff)
	{
		printer_dev.user_buff = kmalloc(USER_BUFF_SIZE, GFP_KERNEL);
		if (!printer_dev.user_buff)
			status = -ENOMEM;
	}

	up(&printer_dev.fop_sem);

	return status;
}

//------------------
static int printer_probe(struct spi_device *spi_device)
{
	if (down_interruptible(&printer_dev.spi_sem))
		return -EBUSY;

	printer_dev.spi_device = spi_device;

	up(&printer_dev.spi_sem);

	return 0;
}

//--------------------
static int printer_remove(struct spi_device *spi_device)
{
	if (down_interruptible(&printer_dev.spi_sem))
		return -EBUSY;

	printer_dev.spi_device = NULL;

	up(&printer_dev.spi_sem);

	return 0;
}

//--------------------
static int __init add_printer_device_to_bus(void)
{
	struct spi_master *spi_master;
	struct spi_device *spi_device;
	struct device *pdev;
	char buff[64];
	int status = 0;

	spi_master = spi_busnum_to_master(SPI_BUS);
	if (!spi_master) {
		printk(KERN_ALERT "spi_busnum_to_master(%d) returned NULL\n",
				SPI_BUS);
		printk(KERN_ALERT "Missing modprobe omap2_mcspi?\n");
		return -1;
	}

	spi_device = spi_alloc_device(spi_master);
	if (!spi_device) {
		put_device(&spi_master->dev);
		printk(KERN_ALERT "spi_alloc_device() failed\n");
		return -1;
	}

	spi_device->chip_select = SPI_BUS_CS1;

	/* Check whether this SPI bus.cs is already claimed */
	snprintf(buff, sizeof(buff), "%s.%u",
			dev_name(&spi_device->master->dev),
			spi_device->chip_select);

	pdev = bus_find_device_by_name(spi_device->dev.bus, NULL, buff);
	if (pdev) {
		/* We are not going to use this spi_device, so free it */
		spi_dev_put(spi_device);

		/*
		 * There is already a device configured for this bus.cs
		 * It is okay if it us, otherwise complain and fail.
		 */
		if (pdev->driver && pdev->driver->name &&
				strcmp(this_driver_name, pdev->driver->name)) {
			printk(KERN_ALERT
					"Driver [%s] already registered for %s\n",
					pdev->driver->name, buff);
			status = -1;
		}
	} else {
		spi_device->max_speed_hz = SPI_BUS_SPEED;
		//        spi_device->mode = SPI_MODE_3;
		spi_device->mode = 0x00; //Chagned by gokul
		spi_device->bits_per_word = 8;
		spi_device->irq = -1;
		spi_device->controller_state = NULL;
		spi_device->controller_data = NULL;
		strlcpy(spi_device->modalias, this_driver_name, SPI_NAME_SIZE);

		status = spi_add_device(spi_device);
		if (status < 0) {
			spi_dev_put(spi_device);
			printk(KERN_ALERT "spi_add_device() failed: %d\n",
					status);
		}
	}

	put_device(&spi_master->dev);

	return status;
}

//--------------------
static struct spi_driver printer_driver = {
	.driver = {
		.name =	this_driver_name,
		.owner = THIS_MODULE,
	},
	.probe = printer_probe,
	.remove =printer_remove,

};

//---------------------
static int __init printer_init_spi(void)       // initializing the spi
{
	int error;

	printer_ctl.tx_buff = kmalloc(SPI_BUFF_SIZE, GFP_KERNEL | GFP_DMA);
	if (!printer_ctl.tx_buff) {
		error = -ENOMEM;
		goto printer_init_error;
	}

	printer_ctl.rx_buff = kmalloc(SPI_BUFF_SIZE, GFP_KERNEL | GFP_DMA);
	if (!printer_ctl.rx_buff) {
		error = -ENOMEM;
		goto printer_init_error;
	}

	error = spi_register_driver(&printer_driver);
	if (error < 0) {
		printk(KERN_ALERT "spi_register_driver() failed %d\n", error);
		goto printer_init_error;
	}

	error = add_printer_device_to_bus();
	if (error < 0) {
		printk(KERN_ALERT "add_printer_to_bus() failed\n");
		spi_unregister_driver(&printer_driver);
		goto printer_init_error;
	}

	return 0;

printer_init_error:

	if (printer_ctl.tx_buff) {
		kfree(printer_ctl.tx_buff);
		printer_ctl.tx_buff = 0;
	}

	if (printer_ctl.rx_buff) {
		kfree(printer_ctl.rx_buff);
		printer_ctl.rx_buff = 0;
	}

	return error;
}

//-----------------
static const struct file_operations printer_fops = {
	.owner =	THIS_MODULE,
	.read  = 	printer_read,
	.open  =	printer_open,
	.write =    printer_write,
};

//-----------------
static int __init printer_init_cdev(void)      //function to create device node
{
	int error;
	printer_dev.devt = MKDEV(0, 0);

	error = alloc_chrdev_region(&printer_dev.devt, 0, 1, this_driver_name);
	if (error < 0) {
		printk(KERN_ALERT "alloc_chrdev_region() failed: %d \n",
				error);
		return -1;
	}

	cdev_init(&printer_dev.cdev, &printer_fops);
	printer_dev.cdev.owner = THIS_MODULE;

	error = cdev_add(&printer_dev.cdev, printer_dev.devt, 1);
	if (error) {
		printk(KERN_ALERT "cdev_add() failed: %d\n", error);
		unregister_chrdev_region(printer_dev.devt, 1);
		return -1;
	}

	return 0;
}

//-------------------
static int __init printer_init_class(void)          //creating class
{
	printer_dev.class = class_create(THIS_MODULE, this_driver_name);

	if (!printer_dev.class) {
		printk(KERN_ALERT "class_create() failed\n");
		return -1;
	}

	if (!device_create(printer_dev.class, NULL, printer_dev.devt, NULL,
				this_driver_name)) {
		printk(KERN_ALERT "device_create(..., %s) failed\n",
				this_driver_name);
		class_destroy(printer_dev.class);
		return -1;
	}
	return 0;
}

static struct proc_dir_entry *parent, *filep;

static int langbuff_open(struct inode *inode, struct file *file)
{
	//printk("Print Buffer Opened\n");
	return 0;
}

static ssize_t langbuff_write(struct file *file, const char __user *buffer, size_t count, loff_t *off)
{
	//printk("Printer Buffer Writing\n");

	memcpy(langprintdata, buffer, DATA_BUFF_SIZE);
	//        copy_from_user(langprintdata,buffer,count);
	//        for(i=0;i<count;i++) {
	//           printk("%c",langprintdata[i]);
	//        }
	return count;
}

static struct file_operations fops_langbuff =
{
	.owner = THIS_MODULE,
	.open = langbuff_open,
	.write = langbuff_write,
};

static int __init printer_procfs_init(void)
{
	if ((parent = proc_mkdir("printer", NULL)) == NULL)
	{
		return -1;
	}
	if ((filep = proc_create("langbuff", 0666, parent, &fops_langbuff)) == NULL)
	{
		remove_proc_entry("printer",NULL);
		return -1;
	}

	return 0;
}

static void __exit printer_procfs_exit(void)
{
	remove_proc_entry("langbuff",parent);
	remove_proc_entry("printer", NULL);
}

//---------------------
static int __init printer_init(void)   
{
	memset(&printer_dev, 0, sizeof(printer_dev));   //allocating memory for printer_dev
	memset(&printer_ctl, 0, sizeof(printer_ctl));

	sema_init(&printer_dev.spi_sem, 1);      //semaphore initialization function
	sema_init(&printer_dev.fop_sem, 1);

	if (printer_init_cdev() < 0)       //function to create device nodes
		goto fail_1;

	if (printer_init_class() < 0)    //function to create class
		goto fail_2;

	if (printer_init_spi() < 0)      //function to initialize spi
		goto fail_3;

	//    procfs_init();
	//--------------------------- GPIO ----------------------------------------//

	gpio_request(130,"motor_line1");      // requesting the pin48 of gpio
	gpio_export(130,true);                // exporting it to the sysfs entry

	gpio_request(131,"motor_line2");
	gpio_export(131,true);

	gpio_request(132,"motor_line3");     // requesting the pin60 of gpio
	gpio_export(132,true);               // exporting it to the sysfs entry

	gpio_request(133,"motor_line4");
	gpio_export(133,true);

	gpio_request(226,"logic power");
	gpio_export(226,true);

	gpio_request(137,"Driver IC enable");
	gpio_export(137,true);

	gpio_request(227,"spi latch");
	gpio_export(227,true);

	gpio_request(134,"strobe1&2");
	gpio_export(134,true);

	gpio_request(135,"strobe3&4");
	gpio_export(135,true);

	gpio_request(136,"strobe5&6");
	gpio_export(136,true);

	gpio_direction_output(227,0); // ------------ SPI latch default---------
	gpio_direction_output(226,1); // --------------logic power -----------
	gpio_direction_output(137,0); // --------------Driver IC enable -----------

	//    printer_procfs_init();

	return 0;

fail_3:
	device_destroy(printer_dev.class, printer_dev.devt);
	class_destroy(printer_dev.class);

fail_2:
	cdev_del(&printer_dev.cdev);
	unregister_chrdev_region(printer_dev.devt, 1);

fail_1:
	return -1;
}

//--------------------------------
static void __exit printer_exit(void)    // exit function to free all the resources 
{

	//    printer_procfs_exit();

	spi_unregister_device(printer_dev.spi_device);
	spi_unregister_driver(&printer_driver);

	device_destroy(printer_dev.class, printer_dev.devt);
	class_destroy(printer_dev.class);

	cdev_del(&printer_dev.cdev);
	unregister_chrdev_region(printer_dev.devt, 1);

	if (printer_ctl.tx_buff)
		kfree(printer_ctl.tx_buff);

	if (printer_ctl.rx_buff)
		kfree(printer_ctl.rx_buff);

	if (printer_dev.user_buff)
		kfree(printer_dev.user_buff);

	gpio_direction_output(226,0);// --------------logic power -----------
	gpio_direction_output(137,0);// --------------Driver IC enable -----------

	gpio_unexport(130);                // exporting it to the sysfs entry
	gpio_unexport(131);
	gpio_unexport(132);               // exporting it to the sysfs entry
	gpio_unexport(133);
	gpio_unexport(226);
	gpio_unexport(137);
	gpio_unexport(227);
	gpio_unexport(134);
	gpio_unexport(135);
	gpio_unexport(136);
}
//----------------------------------

module_init(printer_init);  // Driver always starts execution from here ( insmod ./printer.ko)
module_exit(printer_exit);  // Driver exectues this function while exit  (rmmod  printer.ko)           

MODULE_AUTHOR("Clancor Team");
MODULE_DESCRIPTION("printer module - SPI driver");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.2");


