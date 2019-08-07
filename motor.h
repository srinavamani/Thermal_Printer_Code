#include <linux/jiffies.h>
static unsigned long test_delay;

void time_delay(int usec_delay)
{
	test_delay = usecs_to_jiffies(usec_delay * 1);
	while (time_before(jiffies, test_delay))
		cpu_relax( );
}

void strobe_heating(void);
void heating_strobe(void);
void strobe_150(void);
void strobe_200(void);
void strobe_250(void);
void strobe_300(void);


void lp_strobe_150();
void lp_strobe_200();
void lp_strobe_250();
void lp_strobe_300();

static void even_rotate(void)
{
	//-------------- 1 ----------------------------
	gpio_direction_output(133,1);
	gpio_direction_output(132,1);
	gpio_direction_output(131,0);
	gpio_direction_output(130,1);

//	strobe_250();
	udelay(250);

	//-------------- 2 ----------------------------
	gpio_direction_output(133,1);
	gpio_direction_output(132,0);
	gpio_direction_output(131,1);
	gpio_direction_output(130,1);

//	strobe_150();
	udelay(150);

	//-------------- 3 ----------------------------
	gpio_direction_output(133,1);
	gpio_direction_output(132,1);
	gpio_direction_output(131,1);
	gpio_direction_output(130,1);

//	strobe_150();
	udelay(150);

	//-------------- 4 ----------------------------
	gpio_direction_output(133,1);
	gpio_direction_output(132,0);
	gpio_direction_output(131,1);
	gpio_direction_output(130,1);

//	strobe_200();
	udelay(200);

	//-------------- 5 ----------------------------
	gpio_direction_output(133,1);
	gpio_direction_output(132,1);
	gpio_direction_output(131,1);
	gpio_direction_output(130,1);

//	strobe_150();
	udelay(150);

	//-------------- 6 ----------------------------
	gpio_direction_output(133,1);
	gpio_direction_output(132,0);
	gpio_direction_output(131,1);
	gpio_direction_output(130,1);

//	strobe_250();
	udelay(250);

	//-------------- 7 ----------------------------
	gpio_direction_output(133,0);
	gpio_direction_output(132,1);
	gpio_direction_output(131,1);
	gpio_direction_output(130,1);

//	strobe_150();
	udelay(150);

	//-------------- 8 ----------------------------
	gpio_direction_output(133,1);
	gpio_direction_output(132,1);
	gpio_direction_output(131,1);
	gpio_direction_output(130,1);

//	strobe_150();
	udelay(150);

	//-------------- 9 ----------------------------
	gpio_direction_output(133,0);
	gpio_direction_output(132,1);
	gpio_direction_output(131,1);
	gpio_direction_output(130,1);

//	strobe_300();
	udelay(300);

	//-------------- 10 ----------------------------
	gpio_direction_output(133,1);
	gpio_direction_output(132,1);
	gpio_direction_output(131,1);
	gpio_direction_output(130,1);

//	strobe_150();
	udelay(150);
}


static void lp_even_rotate(void)
{
	//-------------- 1 ----------------------------
	gpio_direction_output(133,1);
	gpio_direction_output(132,1);
	gpio_direction_output(131,0);
	gpio_direction_output(130,1);

	strobe_250();
	//udelay(250);

	//-------------- 2 ----------------------------
	gpio_direction_output(133,1);
	gpio_direction_output(132,0);
	gpio_direction_output(131,1);
	gpio_direction_output(130,1);

	strobe_150();
	//udelay(150);

	//-------------- 3 ----------------------------
	gpio_direction_output(133,1);
	gpio_direction_output(132,1);
	gpio_direction_output(131,1);
	gpio_direction_output(130,1);

	strobe_150();
	//udelay(150);

	//-------------- 4 ----------------------------
	gpio_direction_output(133,1);
	gpio_direction_output(132,0);
	gpio_direction_output(131,1);
	gpio_direction_output(130,1);

	strobe_200();
	//udelay(200);

	//-------------- 5 ----------------------------
	gpio_direction_output(133,1);
	gpio_direction_output(132,1);
	gpio_direction_output(131,1);
	gpio_direction_output(130,1);

	strobe_150();
	//udelay(150);

	//-------------- 6 ----------------------------
	gpio_direction_output(133,1);
	gpio_direction_output(132,0);
	gpio_direction_output(131,1);
	gpio_direction_output(130,1);

	strobe_250();
	//udelay(250);

	//-------------- 7 ----------------------------
	gpio_direction_output(133,0);
	gpio_direction_output(132,1);
	gpio_direction_output(131,1);
	gpio_direction_output(130,1);

	strobe_150();
	//udelay(150);

	//-------------- 8 ----------------------------
	gpio_direction_output(133,1);
	gpio_direction_output(132,1);
	gpio_direction_output(131,1);
	gpio_direction_output(130,1);

	strobe_150();
	//udelay(150);

	//-------------- 9 ----------------------------
	gpio_direction_output(133,0);
	gpio_direction_output(132,1);
	gpio_direction_output(131,1);
	gpio_direction_output(130,1);

	strobe_300();
	//udelay(300);

	//-------------- 10 ----------------------------
	gpio_direction_output(133,1);
	gpio_direction_output(132,1);
	gpio_direction_output(131,1);
	gpio_direction_output(130,1);

	strobe_150();
	//udelay(150);
}


static void odd_rotate(void)
{
	//-------------- 11 ----------------------------
	gpio_direction_output(133,0);
	gpio_direction_output(132,1);
	gpio_direction_output(131,1);
	gpio_direction_output(130,1);

//	strobe_250();
	udelay(250);

	//-------------- 12 ----------------------------
	gpio_direction_output(133,1);
	gpio_direction_output(132,1);
	gpio_direction_output(131,1);
	gpio_direction_output(130,0);

//	strobe_150();
	udelay(150);

	//-------------- 13 ----------------------------
	gpio_direction_output(133,1);
	gpio_direction_output(132,1);
	gpio_direction_output(131,1);
	gpio_direction_output(130,1);

//	strobe_150();
	udelay(150);

	//-------------- 14 ----------------------------
	gpio_direction_output(133,1);
	gpio_direction_output(132,1);
	gpio_direction_output(131,1);
	gpio_direction_output(130,0);

//	strobe_200();
	udelay(200);

	//-------------- 15 ----------------------------
	gpio_direction_output(133,1);
	gpio_direction_output(132,1);
	gpio_direction_output(131,1);
	gpio_direction_output(130,1);

//	strobe_150();
	udelay(150);

	//-------------- 16 ----------------------------
	gpio_direction_output(133,1);
	gpio_direction_output(132,1);
	gpio_direction_output(131,1);
	gpio_direction_output(130,0);

//	strobe_250();
	udelay(250);

	//-------------- 17 ----------------------------
	gpio_direction_output(133,1);
	gpio_direction_output(132,1);
	gpio_direction_output(131,0);
	gpio_direction_output(130,1);

//	strobe_150();
	udelay(150);

	//-------------- 18 ----------------------------
	gpio_direction_output(133,1);
	gpio_direction_output(132,1);
	gpio_direction_output(131,1);
	gpio_direction_output(130,1);

//	strobe_150();
	udelay(150);

	//-------------- 19 ----------------------------
	gpio_direction_output(133,1);
	gpio_direction_output(132,1);
	gpio_direction_output(131,0);
	gpio_direction_output(130,1);

//	strobe_300();
	udelay(300);

	//-------------- 20 ----------------------------
	gpio_direction_output(133,1);
	gpio_direction_output(132,1);
	gpio_direction_output(131,1);
	gpio_direction_output(130,1);

//	strobe_150();
	udelay(150);
}

static void lp_odd_rotate(void)
{
	//-------------- 11 ----------------------------
	gpio_direction_output(133,0);
	gpio_direction_output(132,1);
	gpio_direction_output(131,1);
	gpio_direction_output(130,1);

	strobe_250();
	//udelay(250);

	//-------------- 12 ----------------------------
	gpio_direction_output(133,1);
	gpio_direction_output(132,1);
	gpio_direction_output(131,1);
	gpio_direction_output(130,0);

	strobe_150();
	//udelay(150);

	//-------------- 13 ----------------------------
	gpio_direction_output(133,1);
	gpio_direction_output(132,1);
	gpio_direction_output(131,1);
	gpio_direction_output(130,1);

	strobe_150();
	//udelay(150);

	//-------------- 14 ----------------------------
	gpio_direction_output(133,1);
	gpio_direction_output(132,1);
	gpio_direction_output(131,1);
	gpio_direction_output(130,0);

	strobe_200();
	//udelay(200);

	//-------------- 15 ----------------------------
	gpio_direction_output(133,1);
	gpio_direction_output(132,1);
	gpio_direction_output(131,1);
	gpio_direction_output(130,1);

	strobe_150();
	//udelay(150);

	//-------------- 16 ----------------------------
	gpio_direction_output(133,1);
	gpio_direction_output(132,1);
	gpio_direction_output(131,1);
	gpio_direction_output(130,0);

	strobe_250();
	//udelay(250);

	//-------------- 17 ----------------------------
	gpio_direction_output(133,1);
	gpio_direction_output(132,1);
	gpio_direction_output(131,0);
	gpio_direction_output(130,1);

	strobe_150();
	//udelay(150);

	//-------------- 18 ----------------------------
	gpio_direction_output(133,1);
	gpio_direction_output(132,1);
	gpio_direction_output(131,1);
	gpio_direction_output(130,1);

	strobe_150();
	//udelay(150);

	//-------------- 19 ----------------------------
	gpio_direction_output(133,1);
	gpio_direction_output(132,1);
	gpio_direction_output(131,0);
	gpio_direction_output(130,1);

	strobe_300();
	//udelay(300);

	//-------------- 20 ----------------------------
	gpio_direction_output(133,1);
	gpio_direction_output(132,1);
	gpio_direction_output(131,1);
	gpio_direction_output(130,1);

	strobe_150();
}

void strobe_150(void)
{
	gpio_direction_output(134,1);
	gpio_direction_output(135,0);
	gpio_direction_output(136,1);
	udelay(55);
	gpio_direction_output(134,1);
	gpio_direction_output(135,1);
	gpio_direction_output(136,0);
	udelay(50);
	gpio_direction_output(134,0);
	gpio_direction_output(135,1);
	gpio_direction_output(136,1);
	udelay(50);
	gpio_direction_output(134,0);
	gpio_direction_output(135,0);
	gpio_direction_output(136,0);
}

void strobe_200(void)
{
	udelay(50);
	gpio_direction_output(134,1);
	gpio_direction_output(135,0);
	gpio_direction_output(136,1);
	udelay(55);
	gpio_direction_output(134,1);
	gpio_direction_output(135,1);
	gpio_direction_output(136,0);
	udelay(50);
	gpio_direction_output(134,0);
	gpio_direction_output(135,1);
	gpio_direction_output(136,1);
	udelay(50);
	gpio_direction_output(134,0);
	gpio_direction_output(135,0);
	gpio_direction_output(136,0);

}

void strobe_250(void)
{
	udelay(100);
	gpio_direction_output(134,1);
	gpio_direction_output(135,0);
	gpio_direction_output(136,1);
	udelay(55);
	gpio_direction_output(134,1);
	gpio_direction_output(135,1);
	gpio_direction_output(136,0);
	udelay(50);
	gpio_direction_output(134,0);
	gpio_direction_output(135,1);
	gpio_direction_output(136,1);
	udelay(50);
	gpio_direction_output(134,0);
	gpio_direction_output(135,0);
	gpio_direction_output(136,0);
}

void strobe_300(void)
{
	udelay(150);
	gpio_direction_output(134,1);
	gpio_direction_output(135,0);
	gpio_direction_output(136,1);
	udelay(55);
	gpio_direction_output(134,1);
	gpio_direction_output(135,1);
	gpio_direction_output(136,0);
	udelay(50);
	gpio_direction_output(134,0);
	gpio_direction_output(135,1);
	gpio_direction_output(136,1);
	udelay(50);
	gpio_direction_output(134,0);
	gpio_direction_output(135,0);
	gpio_direction_output(136,0);
}

void lp_strobe_150(void)
{
	gpio_direction_output(134,1);
	gpio_direction_output(135,0);
	gpio_direction_output(136,1);
	udelay(50);
	gpio_direction_output(134,1);
	gpio_direction_output(135,1);
	gpio_direction_output(136,0);
	udelay(50);
	gpio_direction_output(134,0);
	gpio_direction_output(135,1);
	gpio_direction_output(136,1);
	udelay(50);
	gpio_direction_output(134,0);
	gpio_direction_output(135,0);
	gpio_direction_output(136,0);
}

void lp_strobe_200(void)
{
	udelay(50);
	gpio_direction_output(134,1);
	gpio_direction_output(135,0);
	gpio_direction_output(136,1);
	udelay(50);
	gpio_direction_output(134,1);
	gpio_direction_output(135,1);
	gpio_direction_output(136,0);
	udelay(50);
	gpio_direction_output(134,0);
	gpio_direction_output(135,1);
	gpio_direction_output(136,1);
	udelay(50);
	gpio_direction_output(134,0);
	gpio_direction_output(135,0);
	gpio_direction_output(136,0);

}

void lp_strobe_250(void)
{
	udelay(100);
	gpio_direction_output(134,1);
	gpio_direction_output(135,0);
	gpio_direction_output(136,1);
	udelay(55);
	gpio_direction_output(134,1);
	gpio_direction_output(135,1);
	gpio_direction_output(136,0);
	udelay(50);
	gpio_direction_output(134,0);
	gpio_direction_output(135,1);
	gpio_direction_output(136,1);
	udelay(55);
	gpio_direction_output(134,0);
	gpio_direction_output(135,0);
	gpio_direction_output(136,0);
}

void lp_strobe_300(void)
{
	udelay(150);
	//udelay(150);
	gpio_direction_output(134,1);
	gpio_direction_output(135,0);
	gpio_direction_output(136,1);
	udelay(55);
	gpio_direction_output(134,1);
	gpio_direction_output(135,1);
	gpio_direction_output(136,0);
	udelay(50);
	gpio_direction_output(134,0);
	gpio_direction_output(135,1);
	gpio_direction_output(136,1);
	udelay(55);
	gpio_direction_output(134,0);
	gpio_direction_output(135,0);
	gpio_direction_output(136,0);

}

static void empty_rotate(void)
{
	//-------------- 1 ----------------------------
	gpio_direction_output(133,1);
	gpio_direction_output(132,1);
	gpio_direction_output(131,0);
	gpio_direction_output(130,1);

	udelay(250);

	//-------------- 2 ----------------------------
	gpio_direction_output(133,1);
	gpio_direction_output(132,0);
	gpio_direction_output(131,1);
	gpio_direction_output(130,1);

	udelay(150);

	//-------------- 3 ----------------------------
	gpio_direction_output(133,1);
	gpio_direction_output(132,1);
	gpio_direction_output(131,1);
	gpio_direction_output(130,1);

	udelay(150);

	//-------------- 4 ----------------------------
	gpio_direction_output(133,1);
	gpio_direction_output(132,0);
	gpio_direction_output(131,1);
	gpio_direction_output(130,1);

	udelay(200);

	//-------------- 5 ----------------------------
	gpio_direction_output(133,1);
	gpio_direction_output(132,1);
	gpio_direction_output(131,1);
	gpio_direction_output(130,1);

	udelay(150);

	//-------------- 6 ----------------------------
	gpio_direction_output(133,1);
	gpio_direction_output(132,0);
	gpio_direction_output(131,1);
	gpio_direction_output(130,1);

	udelay(250);

	//-------------- 7 ----------------------------
	gpio_direction_output(133,0);
	gpio_direction_output(132,1);
	gpio_direction_output(131,1);
	gpio_direction_output(130,1);

	udelay(150);

	//-------------- 8 ----------------------------
	gpio_direction_output(133,1);
	gpio_direction_output(132,1);
	gpio_direction_output(131,1);
	gpio_direction_output(130,1);

	udelay(150);

	//-------------- 9 ----------------------------
	gpio_direction_output(133,0);
	gpio_direction_output(132,1);
	gpio_direction_output(131,1);
	gpio_direction_output(130,1);

	udelay(300);

	//-------------- 10 ----------------------------
	gpio_direction_output(133,1);
	gpio_direction_output(132,1);
	gpio_direction_output(131,1);
	gpio_direction_output(130,1);

	udelay(150);

	//-------------- 11 ----------------------------
	gpio_direction_output(133,0);
	gpio_direction_output(132,1);
	gpio_direction_output(131,1);
	gpio_direction_output(130,1);

	udelay(250);

	//-------------- 12 ----------------------------
	gpio_direction_output(133,1);
	gpio_direction_output(132,1);
	gpio_direction_output(131,1);
	gpio_direction_output(130,0);

	udelay(150);

	//-------------- 13 ----------------------------
	gpio_direction_output(133,1);
	gpio_direction_output(132,1);
	gpio_direction_output(131,1);
	gpio_direction_output(130,1);

	udelay(150);

	//-------------- 14 ----------------------------
	gpio_direction_output(133,1);
	gpio_direction_output(132,1);
	gpio_direction_output(131,1);
	gpio_direction_output(130,0);

	udelay(200);

	//-------------- 15 ----------------------------
	gpio_direction_output(133,1);
	gpio_direction_output(132,1);
	gpio_direction_output(131,1);
	gpio_direction_output(130,1);

	udelay(150);

	//-------------- 16 ----------------------------
	gpio_direction_output(133,1);
	gpio_direction_output(132,1);
	gpio_direction_output(131,1);
	gpio_direction_output(130,0);

	udelay(250);

	//-------------- 17 ----------------------------
	gpio_direction_output(133,1);
	gpio_direction_output(132,1);
	gpio_direction_output(131,0);
	gpio_direction_output(130,1);

	udelay(150);

	//-------------- 18 ----------------------------
	gpio_direction_output(133,1);
	gpio_direction_output(132,1);
	gpio_direction_output(131,1);
	gpio_direction_output(130,1);

	udelay(150);

	//-------------- 19 ----------------------------
	gpio_direction_output(133,1);
	gpio_direction_output(132,1);
	gpio_direction_output(131,0);
	gpio_direction_output(130,1);

	udelay(300);

	//-------------- 20 ----------------------------
	gpio_direction_output(133,1);
	gpio_direction_output(132,1);
	gpio_direction_output(131,1);
	gpio_direction_output(130,1);

	udelay(150);

}
