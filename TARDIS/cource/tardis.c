#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h> 

#define F_CPU 8000000UL	

static char tardis[]={0,0,2,5,10,16,24,33,43,54,67,80,94,109,124,139,154,168,182,195,208,219,229,237,244,250,253,255};
static char tardis_count = 3;

void fade_in(){
	for (char i=0;i<(sizeof(tardis)/sizeof(tardis[0]));i++){
	_delay_ms(30);
	OCR0B = tardis[i];
	}
}

void fade_out(){
	for (char i=(sizeof(tardis)/sizeof(tardis[0]))-1;i>0;i--){
	OCR0B = tardis[i];
	_delay_ms(30);
	}
}

void tardis_light(){
	for(char i=0; i<tardis_count;i++){
		fade_in();
		_delay_ms(250);
		fade_out();
	
		_delay_ms(250);
	}
}

void go_to_sleep(){
 	 cli();
	 set_sleep_mode(SLEEP_MODE_PWR_DOWN); 
	 sleep_enable();
     sei();
	 sleep_cpu();
	 sleep_disable();
     //sei();	
}


ISR(INT0_vect){
	
	cli();

	tardis_light();

	sei();

	_delay_ms(100);
	go_to_sleep();
}


int main(){

	//Status LED
	DDRB |= (1<<PB4);
	PORTB &= ~_BV(PB4); 

	//Main LED
	DDRB |= (1<<PB1);  // make OC0B PWM output pin

	//PWM
	TCCR0A = (1<<COM0B1)|(1<<WGM00);  // mode #1
	TCCR0B = (1<<CS00);  // clkI/O/(No prescaling)


	//INT0 enable:
	MCUCR&=~(_BV(ISC01)) | ~(_BV(ISC00));
	GIMSK|=_BV(INT0);

	//PB2 as input for INT0
	DDRB &=~_BV(PB2);
	PORTB |= (1<<PB2);
	
	sei();

	//Disable ADC, saves ~230uA
	ADCSRA &= ~(1<<ADEN);
	
	
	tardis_light();
	
	

	go_to_sleep();

		

	while(5){

			
	}

}
