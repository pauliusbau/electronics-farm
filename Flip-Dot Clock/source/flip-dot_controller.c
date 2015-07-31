#define F_CPU 8000000UL
#define BAUD 9600
#define UBRR_VAL F_CPU/16/BAUD-1

#include <inttypes.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util\delay.h>

#define debug_mode 0 //debuginimui

#define LED PINB0

#define LED_BUTTON_1 PINB3
#define LED_BUTTON_2 PINB4	

#define FET_1 PINB1
#define FET_2 PINB2



char Time[12];	//HH:mm:ss 
char Date[12];
//Now Read and format time
uint8_t data;
uint8_t hr, min, sec;
uint8_t date, month, year;
uint8_t temp;
volatile uint8_t sel=0, set=0, finish=0, count=0;


volatile unsigned int   clock_millisecond=0;
volatile char        clock_second=0, timer=0;



static pstlp=6, peil=8; // laiko isspausdinimui, pradios koordinates

unsigned char buff[5];

unsigned char button_pressed=0; //laiko nustatymui
unsigned char digit = 0;
volatile unsigned char set_time = 0;

static unsigned char set_time_timeout = 30; //kiek laiko paliekam laiko nustatymui (sekundemis)

volatile unsigned char 	set_fet = 0;  	//pereisim i FET pasirinkima
volatile unsigned char 	fet_status = 0; //kuris kanalas ijungtas: 0 - nei vienas, 1 - FET_1, 2 - FET_2
volatile unsigned char 	fet_count = 0;
static unsigned char 	fet_timeout = 100; //santykiniu vienetu

#include "flip-dot.h"
#include "flip-dot-font.h"
#include "I2C.h"
#include "ds1307.h"
#include "funkcijos.h"
#include "ds1307_funkcijos.h"




ISR(USART_RXC_vect){

	uint8_t c;
	c = receiveByte();	

		
	if(c == 'c'){
	send_string("\033[2J"); //isvalo terminalo langa
	send_string("\033[0;0H"); //pastato kursoriu i 0,0
	}
		else if(c == 'a') apie();
			else if(c == 'm') meniu();
						
				else if(c=='x') {send_string("\033[2J"); send_string("\033[0;0H"); sel=0; set=0; apie(); meniu(); TIMSK = _BV (TOIE0);} //isejimas is nustatymu ciklu
				


			//----------------------------------- laiko nustatymas ----------------------------------------------------
				else if(c == 't' && sel==0) {

				send_string("------------- Time  setup -------------\n\r");
				send_string("     (press X to exit at eny time)     \n\r\n\r");
				TIMSK &= ~(_BV (TOIE0)); //Isjungiam Timer 0 pertraukti
				ds1307_get_td();
				send_string("System clock: ");
				send_string(Time);
				send_string("\n\r");
	
				send_string("Set hours (0-24): ");
				sel=1; set=0;
				}

				if(sel==1){ //valandos 0-24
					if (set==0) {if((c-48)>=0 && (c-48)<=2) set=1;}				//realiai reikia, kad nenusoktu tiesiai i set=1
						else if (set==1) {if((c-48)>=0 && (c-48)<=9) set=2;}	//realiai reikia, kad nenusoktu tiesiai i set=2
				
					if (set==1){ hr=(c-48)*10; send_string(itoa((c-48),buff,10));}
				 	else if (set==2){ hr=hr+((c-48)*1); set=3; send_string(itoa((c-48),buff,10)); }					
					else if (set==3) {sel=2; set=0; send_string("\n\rSet minutes (0-59): "); }

					}

				else if (sel==2){//minutes
					if (set==0) {if((c-48)>=0 && (c-48)<=5) set=1;}
						else if (set==1) {if((c-48)>=0 && (c-48)<=9) set=2;}

					if (set==1){ min=(c-48)*10; send_string(itoa((c-48),buff,10)); }
				 	else if (set==2){ min=min+((c-48)*1); set=3; send_string(itoa((c-48),buff,10));}					
					else if (set==3) {sel=3; set=0; send_string("\n\rSet seconds (0-59): ");}
					}
				
				else if (sel==3){//sekundes
					if (set==0) {if((c-48)>=0 && (c-48)<=5) set=1;}
						else if (set==1) {if((c-48)>=0 && (c-48)<=9) set=2;}

					if (set==1){ sec=(c-48)*10; send_string(itoa((c-48),buff,10)); }
				 	else if (set==2){ sec=sec+((c-48)*1); set=3; send_string(itoa((c-48),buff,10));}					
					else if (set==3) {sel=4; set=0; send_string("\n\rSave? (Y/N)");}
					}

				else if (sel==4){//irasymas
					if (c=='y' || c=='Y'){
					 	if(hr<25 && min<60 && sec<60) {ds1307_set_time(); send_string("\n\rSuccess!\n\r\n\n"); 
														sel=0; set=0; hr=0; min=0; sec=0; 
														laikas(pstlp, peil); 
														apie(); meniu(); 
														TIMSK = _BV (TOIE0);}
							else {send_string("\n\rError!\n\r\n\n"); sel=0; set=0; hr=0; min=0; sec=0; meniu(); TIMSK = _BV (TOIE0);}
					
					}
					else if(c == 'n' || c== 'N') {send_string("\n\rAbort!\n\r\n\n"); sel=0; set=0; hr=0; min=0; sec=0; apie(); meniu(); TIMSK = _BV (TOIE0);}
				}


			//-------------------------------------------datos nustatymas--------------------------------------------------------------------
			
				else if(c == 'd' && sel==0) {

				send_string("------------- Date setup -------------\n\r");
				send_string("     (press X to exit at eny time)     \n\r\n\r");
				TIMSK &= ~(_BV (TOIE0)); //Isjungiam Timer 0 pertraukti
				ds1307_get_td();
				send_string("System date: ");
				send_string(Date);
				send_string("\n\r");
	
				send_string("Set year (20xx): ");
				sel=5; set=0;
				}

				if(sel==5){ //metai iki 2100
					if (set==0) {if((c-48)>=0 && (c-48)<=9) set=1;}
						else if (set==1) {if((c-48)>=0 && (c-48)<=9) set=2;}				
								
					if (set==1){ year=(c-48)*10; send_string(itoa((c-48),buff,10));}
				 	else if (set==2){ year=year+((c-48)); set=3; send_string(itoa((c-48),buff,10)); }										
					else if (set==3) {sel=6; set=0; send_string("\n\rSet month (0-12): "); }

					}

				else if (sel==6){//menesis
					if (set==0) {if((c-48)>=0 && (c-48)<=1) set=1;}
						else if (set==1) {if((c-48)>=0 && (c-48)<=9) set=2;}

					if (set==1){ month=(c-48)*10; send_string(itoa((c-48),buff,10)); }
				 	else if (set==2){ month=month+((c-48)*1); set=3; send_string(itoa((c-48),buff,10));}					
					else if (set==3) {sel=7; set=0; send_string("\n\rSet day (0-31): ");}
					}
				
				else if (sel==7){//diena
					if (set==0) {if((c-48)>=0 && (c-48)<=3) set=1;}
						else if (set==1) {if((c-48)>=0 && (c-48)<=9) set=2;}

					if (set==1){ date=(c-48)*10; send_string(itoa((c-48),buff,10)); }
				 	else if (set==2){ date=date+((c-48)*1); set=3; send_string(itoa((c-48),buff,10));}					
					else if (set==3) {sel=8; set=0; send_string("\n\rSave? (y/N)");}
					}

				else if (sel==8){//irasymas
					if (c=='y' || c=='Y'){
					 	if(year<=99 && month<=12 && date<=31) {ds1307_set_date(); send_string("\n\rSuccess!\n\r\n\n"); 
																sel=0; set=0; year=0; month=0; date=0; 
																apie(); meniu(); 
																TIMSK = _BV (TOIE0);}
							else {send_string("\n\rError!\n\r\n\n"); sel=0; set=0; year=0; month=0; date=0; meniu(); TIMSK = _BV (TOIE0);}
					
					}
					else if(c == 'n' || c== 'N') {send_string("\n\rAbort!\n\r\n\n"); sel=0; set=0; year=0; month=0; date=0; apie(); meniu(); TIMSK = _BV (TOIE0);}
				}

					//---------------------------------------- FET ijungimas/ isjungimas -----------------------------------------------------
				else if(c == 'f' && sel==0) {
					send_string("Select FET:\n\r");
					send_string("0 - FET 1 [OFF] & FET 2 [OFF] \n\r");
					send_string("1 - FET 1 [ON] & FET 2 [OF] \n\r");
					send_string("2 - FET 1 [OFF] & FET 2 [ON] \n\r\n\r");
					sel=9;
				}

					if(sel==9){
						switch(c-48){
							case 0: PORTB &= ~_BV (FET_1); PORTB &= ~_BV (FET_2); fet_status=0; sel=0; apie(); meniu(); break;
							case 1: PORTB |= _BV (FET_1); PORTB &= ~_BV (FET_2); fet_status=1; sel=0; apie(); meniu(); break;
							case 2: PORTB &= ~_BV (FET_1); PORTB |= _BV (FET_2); fet_status=2; sel=0; apie(); meniu(); break;
						}
					}
				//----------------------------------------------------------------------
}

ISR(TIMER0_OVF_vect) //timer overflow vector
{

	count++;

/*
	send_string("count: ");
	send_string(itoa(count,buff,10));
	send_string("\n\r");
*/
	// ------------ Laiko isvedimui / atnaujinimui flip-dot lentoje
	if (count==100){

	//send_string("\n\rPertrauktis \n\r ");

	count=0;

			ds1307_get_td();

		if ( (((Time[3]-48)*10)+(Time[4]-48)) != min){ //ar nepasikeite minutes

		//	send_string("\n\rPertrauktis - minutes \n\r ");
		
			if ( (Time[3]-48) != (min/10) ) print_number((Time[3]-48),(pstlp+10),peil);
			if ( (Time[4]-48) != (min%10) ) print_number((Time[4]-48),(pstlp+14),peil);

			min=(((Time[3]-48)*10)+(Time[4]-48));
		}

		if ( (((Time[0]-48)*10)+(Time[1]-48)) != hr){ //ar nepasikeite valandos
		
			if ( (Time[0]-48) != (hr/10) ) print_number((Time[0]-48),(pstlp),peil);
			if ( (Time[1]-48) != (hr%10) ) print_number((Time[1]-48),(pstlp+4),peil);

			hr=(((Time[0]-48)*10)+(Time[1]-48));
		}


	}

	// ------------ FET'u perjunginejimui (apsvietimui) ---------
	if(set_fet==1){
	fet_count++;

		if(fet_count==fet_timeout){
			fet_count=0;
			set_fet=0;
			PORTB &= ~_BV (LED_BUTTON_2);
			GICR|=_BV(INT1)|_BV(INT0);

			#if debug_mode
			send_string("\n\rIsejau is FET nustatymu!\n\r");		
			#endif
		}
	
	}

}

ISR(TIMER1_COMPA_vect){

   clock_millisecond++;
	
	if (clock_millisecond==100){
		timer++; //sekundziu skaiciavimui
		clock_millisecond=0;
   	}


   if(timer==10){
      	clock_second++;
      	timer=0;
		
		#if debug_mode
		send_string(itoa(set_time_timeout - clock_second,buff,10));
		send_string("\n\r");
		#endif
			
		if(set_time_timeout - clock_second == 0){
			
			PORTB &= ~_BV(LED_BUTTON_1); //uzgesinam mygtuko LED
			
			stop_timer1();
			clock_second=0;
			clock_millisecond=0;
			timer=0;
					
			pilnas_juodas();
			ds1307_get_td();
			laikas(pstlp,peil);
			GICR|=_BV(INT1)|_BV(INT0); 
			TIMSK = _BV (TOIE0); // ijungiam timer 0 pertraukti
			set_time=0;	
			digit=0; 
			sei();
		}
		else clock_second = clock_second;	  
	  
	}



}
	

ISR(INT0_vect){
#if debug_mode 
send_string("INT0\n\r"); 
#endif
//cli();
TIMSK &= ~(_BV (TOIE0)); // isjungiam timer 0 pertraukti

set_time = 1; //pradedam rankini laiko nustatyma
PORTB |= _BV (LED_BUTTON_1); //uzdegam mygtuko LED

clock_second=0;
clock_millisecond=0;
timer=0;
start_timer1(); //paleidziam TimeOUT timeris, kuris nukilis procesa, kai praeis laikas set-time_timeout


	#if debug_mode
	send_string("\n\rSet Time...\n\r");		
	#endif
	
	//isjungem isorines pertrauktis
	GICR&=~(_BV(INT1)); 
	GICR&=~(_BV(INT0));
	ds1307_get_td();
}






//Lauktranziu junginejimas
ISR(INT1_vect){ 
//cli();

	#if debug_mode
	send_string("  INT1: Select FET\n\r");
	#endif

	set_fet=1;
	fet_count=0;


	PORTB |= _BV (LED_BUTTON_2); //uzdegam mygtuko LED

	//isjungem isorines pertrauktis
	GICR&=~(_BV(INT1)); 
	GICR&=~(_BV(INT0));

}


int main(void) {

	cli(); 

	
	DDRB=0xFF;
	PORTB=0x00;

	DDRA=0xFF;
	PORTA=0x00;

	DDRC=0xFF;
	PORTC=0x00;

	DDRD=0xFF;
	PORTD=0x00;

	//INT0 ir INT1 kaip iejimai
	DDRD&= ~(_BV (PD2));
	DDRD&= ~(_BV (PD3));
	PORTD |= _BV (PD2);
	PORTD |= _BV (PD3);

	
	init_uart(UBRR_VAL);
	I2CInit();
	rtc_init();
	init_timer();
	timer_init_1();
	

	sei();

	apie();
	meniu();





pilnas_geltonas();
pilnas_juodas();

_delay_ms(200);

laikas(pstlp,peil);

init_isr();
TIMSK = _BV (TOIE0); // ijungiam timer 0 pertraukti



while(5){

PORTB ^= (1<<PB0);
_delay_ms(100);

	

	//---------------------- FET'u ijungimas / isjungimas (apsvietimui) ----------
	while (set_fet !=0){
		PORTB ^= (1<<PB0);
		_delay_ms(20); //tam kad normaliai aptiktu mygtuku paspaudimus
	
	
		if(read_keypad()==3){

			if(fet_status==0){
				PORTB |= _BV (FET_1);
				fet_count = 0;
				fet_status = 1;

				#if debug_mode
				send_string("\n\rFET_1 [ON] \n\r");	
				debug_light();
				#endif
			}

			else if(fet_status==1){
							
					PORTB &= ~_BV (FET_1);
					PORTB |= _BV (FET_2);
					fet_count = 0;
					fet_status = 2;

					#if debug_mode
					send_string("\n\rFET_1 [OFF] & FET_2 [ON] \n\r");	
					debug_light();
					#endif
						
			}
		
			else if(fet_status==2){
						PORTB &= ~_BV (FET_2); 
						fet_count = 0;
						fet_status=0;
						
						#if debug_mode
						send_string("\n\rFET_2 [OFF] \n\r");
						debug_light();	
						#endif		
			}
			else fet_status=fet_status;
		

		}

	}// end_of: while (set_fet !=0)

	
	// --------------------- laiko nustatymas (rankinis) -----------------------------
	while (set_time != 0){
	
	PORTB ^= (1<<PB0);
	_delay_ms(30); //tam kad normaliai aptiktu mygtuku paspaudimus


		//sokinejimas per skaicius	
		if(read_keypad()==2 ){
			timer_1_reset();

			digit=digit+1;
			
			#if debug_mode
			send_string("Digit: ");
			send_string(itoa(digit,buff,10));
			send_string("\n\r");
			#endif
	
			switch(digit){
							case 1:	bruksniukas(pstlp,peil-2,3,1); break;
							case 2:	bruksniukas(pstlp,peil-2,3,0); bruksniukas(pstlp+4,peil-2,3,1); 
									//patikrinimas nuo durniaus
									//if(Time[1]-48>=3 && Time[0]-48==2) Time[1]=48; else Time[1]=Time[1];
									break;
							case 3:	bruksniukas(pstlp+4,peil-2,3,0); bruksniukas(pstlp+10,peil-2,3,1); break;
							case 4:	bruksniukas(pstlp+10,peil-2,3,0); bruksniukas(pstlp+14,peil-2,3,1); break;
							case 5: bruksniukas(pstlp+14,peil-2,3,0); 
																																	
										sec=0; 
										min=(Time[3]-48)*10+(Time[4]-48); 
										hr=(Time[0]-48)*10+(Time[1]-48);
									
										if(hr<=23 && min<=59){ 
											#if debug_mode
											send_string("SET time: ");	
											send_string(itoa(hr,buff,10)); 
											send_string(" :");	send_string(itoa(min,buff,10));			
											send_string(" : "); send_string(itoa(sec,buff,10)); send_string("\n\r");
											#endif
											PORTB &= ~_BV(LED_BUTTON_1); //uzgesinam mygtuko LED											

											stop_timer1();
											timer_1_reset();

											ds1307_set_time();
											pilnas_juodas();
											_delay_ms(50);															
											laikas(pstlp,peil);
											GICR|=_BV(INT1)|_BV(INT0); 
											TIMSK = _BV (TOIE0); // ijungiam timer 0 pertraukti
											set_time=0;	
											digit=0; 
											sei();
									
										}
										else {
											#if debug_mode
											send_string("\n\rTime Error!\n\r");	
											#endif		
											timer_1_reset();							
											bruksniukas(pstlp,peil-2,3,1); digit=1; 
										}																
									break; //cia padaryti isejima ir laiko irasyma
							}
			
		
		}		
		else digit=digit;		
		
		
		
		//nustatom skaicius		
		if(read_keypad()==3){
			timer_1_reset();	
						
			if(digit==1){		
				if(Time[0]-48!=2) Time[0]=Time[0]+1; else {Time[0]=48;}   			
				print_number(Time[0]-48,pstlp,peil);
			}

			else if(digit==2){		
				if(Time[0]-48<=1) {	if(Time[1]-48!=9) Time[1]=Time[1]+1; else Time[1]=48;}
				else if (Time[0]-48==2) {if(Time[1]-48>=3) Time[1]=48; else Time[1]=Time[1]+1; }
				else Time[0]=Time[0];		
				print_number(Time[1]-48,pstlp+4,peil);
			}

			else if(digit==3){		
				if(Time[3]-48!=5) Time[3]=Time[3]+1; else Time[3]=48;
							
				print_number(Time[3]-48,pstlp+10,peil);
			}

			else if(digit==4){		
				if(Time[4]-48!=9) Time[4]=Time[4]+1; else Time[4]=48;
				print_number(Time[4]-48,pstlp+14,peil);
			}

			else Time[2]=Time[2];
		


			#if debug_mode
			send_string("Digit: ");	send_string(itoa(digit,buff,10));
			send_string(" Time[]: ");	send_string(itoa(Time[0]-48,buff,10));			
			send_string(" Time: "); send_string(Time); send_string("\n\r");
			#endif
		}		
			
 	}


}


}



