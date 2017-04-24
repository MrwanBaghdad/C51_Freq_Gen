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
	for(i = 0 ; i <=3 ; i++)
	{
		temp_h += input_high % 10 * (1 << i);
		input_high /= 10;+
		temp_l += input_low %10 *  (1 << i);
		input_low /=10;

	}
	
	temp_duty = temp_h * 10 + temp_l;
	duty = temp_duty /100 ;
	
	lh_high= 0xFF - (0x0e * (duty));
	ll_high= 0xFF - (0x66 * (duty)) + 1;
	
	lh_low = 0xFF - (0x0e * (1-duty));
	ll_low = 0xFF - (0x66 * (1-duty)) + 1;
	return 0;

}


void main()
{	

	// gen a square wave with 50% duty cycle 
	EA = 1;
	TMOD = 0x01;

	TH0 = 0xFF - 0x0e * (1-duty);
	TL0 = 0xFF - 0x66 * (1-duty);

	lh_high= 0xFF - (0x0e * (duty));
	ll_high= 0xFF - (0x66 * (duty)) + 1;

	lh_low = 0xFF - (0x0e * (1-duty));
	ll_low = 0xFF - (0x66 * (1-duty)) + 1;
	P1=1 ;
	TR0 = 1;
	ET0 =1 ;
	P1 = 50;
	while(1)
	{
		//polling delay
		for(j = 0xffff; j>=0;j--);
		for(j = 0xffff; j>=0;j--);
		for(j = 0xffff; j>=0;j--);
		//Getting duty input
		get_input();

	}
}
