/*
   500 HZ signal generator with a variable duty cycle
   based on the user's input on port 2 as a packed BCD
   an error is raised in case of wrong duty cycle
   Timer 0 is used to genrate signal

   Written by:
   Marwan Nabil 2767
   Mohammed Nasser 2858
 */

#include <at89c51xd2.h>
#include <stdlib.h>


sbit output = P2^0;
sbit err = P2^6;

//sbit edge = 1 //start from high output
//int total_sig_time = 0xe66;

bit edge = 0;
float duty = 0.75;
char lh_high, ll_high, lh_low, ll_low;

void Interupt(void) interrupt 1
{
	output = ~ output;
	if(edge)
	{
		TH0 = lh_high;
		TL0 = ll_high;
		edge = !edge;
	}
	else{
		edge = !edge;
		TH0 = lh_low;
		TL0 = ll_low;
	}
	//edge= ~edge;

}
char input_high, input_low, temp_h, temp_l;
float temp_duty=0;
char i,j;
short int all_delay;
char get_input()
{	
	input_low = P1 & 0x0f;
	input_high = (P1 >> 4) & 0x0f ;
	
	if(input_low > 9 || input_high > 9)
	{
		err = 0;
		return -1;
	}
	temp_h= temp_l =0;
	// convert to decimal
	err = 1;
	for(i = 0 ; i <=3 ; i++)
	{
		temp_h += input_high % 10 * (1 << i);
		input_high /= 10;
		temp_l += input_low %10 *  (1 << i);
		input_low /=10;

	}
	
	temp_duty = temp_h * 10 + temp_l;
	duty = temp_duty /100 ;
	
	all_delay =  0xffff - (int)(0x0e66 * duty);
	lh_high = all_delay >> 8 & 0xff;
	ll_high = all_delay & 0xff;
	
	all_delay =  0xffff - (int)(0x0e66 *(1-duty) );
	lh_low = all_delay >> 8 & 0xff;
	ll_low= all_delay & 0xff;
	

	if(!TR0)
		TR0 = 1;
	return 0;

}


sbit on_off_switch = P2^7;


void main()
{	

	// gen a square wave with 50% duty cycle 
	//enable interripts
	EA = 1;
	//enable timer 0
	TMOD = 0x01;

	all_delay =  0xffff - (int)(0x0e65 * duty);
	lh_high = all_delay >> 8 & 0xff;
	ll_high = all_delay & 0xff;
	
	all_delay =  0xffff - (int)(0x0e65 *(1-duty) );
	lh_low = all_delay >> 8 & 0xff;
	ll_low= all_delay & 0xff;

	P1=1 ;
	ET0 =1 ;
	TR0 = 1;
	P1 = 0x50;
	on_off_switch = 1;
	edge = 0 ;
	output = 1;
	while(1)
	{
		//polling delay
		//Getting duty input
		while(on_off_switch == 0)
		{
			TR0 = 0;
			output  = 1;
		}
		get_input();
		
	}
}

