void apie(){


	/* per USART */
	send_string("\033[2J"); //isvalo terminalo langa
	send_string("\033[0;0H"); //pastato kursoriu i 0,0

	send_string("\n\r");
	send_string("LED Dimmer V1.0 \n\r");
	send_string("Paulius Bautrenas 2014");
	send_string("\n\r");
	send_string("\n\r");



		/* clear display and home cursor */

        lcd_clrscr();
        
		lcd_puts("Paulius\n");
		lcd_puts("Bau");
		_delay_ms(500);
        
		lcd_clrscr();
        lcd_puts("LedDmr\n");
        lcd_puts("v1.0");
		_delay_ms(500);

	


}

void init_uart(unsigned int ubrr) {
	/* Set baud rate */
	UBRRH = (unsigned char)(ubrr>>8 ) ;
	UBRRL = (unsigned char)(ubrr);
	/* Enable receiver and transmitter */
//	UCSRB |= (1<<RXEN);
	UCSRB |= (1<<TXEN);
	/* Set frame format: 8data, 1stop bit */
	UCSRC = (1<<URSEL)|(3<<UCSZ0); //|(1<<USBS)


}


void send_char(unsigned char data) {
	/* Wait for empty transmit buffer */
	while (!(UCSRA & (1<<UDRE)));
		/* Put data into buffer, sends the data */
		UDR = data;
}

void send_string(char *data) {
	while (*data) {
		send_char(*data);
		data++;
	}
}

unsigned char receiveByte( void )
{
	unsigned char data, status;
	
	while(!(UCSRA & (1<<RXC))); 	// Wait for incomming data
	
	status = UCSRA;
	data = UDR;
	
	return(data);
}


void INT_init()
{	
	
//	MCUCR&=~(_BV(ISC01)) | ~(_BV(ISC00));
	MCUCR|=_BV(ISC01) | _BV(ISC00);

	MCUCR&=~(_BV(ISC10));
	MCUCR|=_BV(ISC11);


//	GICR|=_BV(INT0);
	GICR|=_BV(INT1);

}


void ADC_init(){
//ADC nustatymai	
	ADCSRA |=_BV(ADEN);	
	ADCSRA |=_BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); //| _BV(ADIE);

	ADMUX  |= _BV(MUX0) | _BV(MUX2);
	ADMUX  &=~(_BV(REFS1));
	ADMUX  |= _BV(REFS0);

	ADCSRA |=_BV(ADSC);

	
}


void timer_init_1(){ //count down timer
 
      TCCR1B=(1<<WGM12)|(1<<CS11)|(1<<CS10);
      OCR1A=0x007C;

}

void timer_init_0()
{
	TIFR |= (1<<TOV0);		//set interrupt on overflow
	TIMSK |= (1<<TOIE0);
	TCNT0 = 223;
}

void start_timer0()	{TCCR0 |= (1<<CS00)|(1<<CS02); }
void stop_timer0()	{TCCR0 =0x00;}

void start_timer1() {TIMSK |=(_BV(OCIE1A));}
void stop_timer1()	{TIMSK &=~(_BV(OCIE1A));}

void timer2_set(unsigned char skaicius){ //TIMER2 valdymui: nustato OCR2 bei visiskai isjungia TIMER2, jei nustatom, kad OCR2==0  
	OCR2=skaicius;	

	if (skaicius==0) TCCR2 &=~((1<<CS21)|(1<<CS20)|(1<<COM21)); // No clock source || Normal port operation, OC2 disconnected
	else if (TCCR2!=0x6B) TCCR2 |= (1<<CS21)|(1<<CS20)|(1<<COM21); // /32 clock source prescaler || Clear OC2 on Compare Match, set OC2 at BOTTOM (PWM->OC2)
	else TCCR2=TCCR2;

	/*
	send_string("skaicius: ");
	send_string(itoa(skaicius, buff, 10));
	send_string(" TCCR2: 0x");
	send_string(itoa(TCCR2, buff, 16));
	send_string(" TCCR2: 0b");
	send_string(itoa(TCCR2, buff, 2));
	send_string("\n\r");
	*/
}


void EEPROM_write(unsigned int uiAddress, unsigned long int ucData)
{
/* Wait for completion of previous write */
while(EECR & (1<<EEWE));
/* Set up address and data registers */
EEAR = uiAddress;
EEDR = ucData;

/* Write logical one to EEMWE */
EECR |= (1<<EEMWE);
/* Start eeprom write by setting EEWE */
EECR |= (1<<EEWE);
}

unsigned char EEPROM_read(unsigned int uiAddress)
{
/* Wait for completion of previous write */
while(EECR & (1<<EEWE))
;
/* Set up address register */
EEAR = uiAddress;
/* Start eeprom read by writing EERE */
EECR |= (1<<EERE);
/* Return data from data register */
return EEDR;
}


void first_time_boot(){

unsigned int first_boot=EEPROM_read(12);

if(first_boot!=0x55){
	EEPROM_write(24, 0xFF); //setting PWM to fully on
	EEPROM_write(30, 0x08);//minPWM
	EEPROM_write(31, 0xFF);//maxPWM
	EEPROM_write(32, 25);//timeOUT
	EEPROM_write(33, 20);//LDRth
	EEPROM_write(34, 0);//LDRinv
	EEPROM_write(35, 0);//LDRkoef
	EEPROM_write(36, 10);//ADCgain

	EEPROM_write(12, 0x55); //making a note, that first boot is over

	for(unsigned char k=0; k<=5; k++){
	PORTDDR(LED_PORT)^= (1<<LED3_BIT);
	_delay_ms(250);
	}
	
	send_string("--- FIRST BOOT ---");
	
}

else{  //not the first boot
	for(unsigned char k=0; k<=5; k++){
	PORTDDR(LED_PORT)^= (1<<LED3_BIT);
	_delay_ms(50);}
	}
	
	//send_string("--- Nth BOOT ---");
}

//--------------nuskaitom config is eeprom-------

void read_config(){

	minPWM=EEPROM_read(30);
	maxPWM=EEPROM_read(31);
	timeOUT=EEPROM_read(32);
	LDRth=EEPROM_read(33);
	LDRinv=EEPROM_read(34);
	LDRkoef=EEPROM_read(35);
	ADCgain=EEPROM_read(36);

}

//--------------irasom config i eeprom-------

void write_config(){

	EEPROM_write(30, minPWM);
	EEPROM_write(31, maxPWM);
	EEPROM_write(32, timeOUT);
	EEPROM_write(33, LDRth);
	EEPROM_write(34, LDRinv);
	EEPROM_write(35, LDRkoef);
	EEPROM_write(36, ADCgain);

}	

char read_keypad () {	// funkcija klausytis klaviaturos

	char which_button=0;


	while(1) { 

		
		if (bit_is_clear(PINB, PB6)) {
			if (button_pressed == 0) {
				button_pressed = 1;
				which_button = 1;
			}				 
		} 

		

		else {
			button_pressed = 0;
		//	break; // kad skaitmenis visa laika rodytu
		}

		if (button_pressed == 1) {
			break;
		}		
	}

	return which_button;

}


unsigned char nuostato_radimas(char nustatymas){
char eilnr;
for(unsigned char i=0;i<=46;i++){ //randam atitikmeni masyve

	 if (fadein[i]>=nustatymas) {
	 eilnr=i;
	 break;} 
	}
return eilnr;

}



// grynai PIR darbo rezimui
void fade_in(){
char min=nuostato_radimas(OCR2); // kad nepradetu fade_in daryti nuo min_PWM is configo.
char max=nuostato_radimas(EEPROM_read(31)); //maxPWM - is configo.. ale didziausiais leidziamas LED sviesos stipris

#if debug_mode 
send_string("fade_in(): ");
send_string("min: ");
send_string(itoa(min,buff,10));
send_string("  max: ");
send_string(itoa(max,buff,10));
send_string("\n\r");
#endif




for( char i=min;i<=max;i++){

	//OCR2=fadein[i];
	timer2_set(fadein[i]);
	_delay_ms(50);
}

 	 
}



// grynai PIR darbo rezimui
void fade_out(){

char max=nuostato_radimas(OCR2); // kad nepradetu fade_out daryti nuo max_PWM is configo.
char min=nuostato_radimas(EEPROM_read(30)); //minPWM - is configo.. ale maziausias leidziamas LED sviesos stipris

#if debug_mode 
send_string("fade_out(): ");
send_string("max: ");
send_string(itoa(max,buff,10));
send_string("  min: ");
send_string(itoa(min,buff,10));
send_string("\n\r");
#endif

for(unsigned char i=max;i>=min;i--){

	/*
	send_string("i: ");
	send_string(itoa(i,buff,10));
	send_string("  fadein[i]: ");
	send_string(itoa(fadein[i],buff,10));
	send_string("\n\r");
	*/

	if (i==0) {timer2_set(fadein[i]); break;}
	else{
	//OCR2=fadein[i];
	timer2_set(fadein[i]);
	_delay_ms(50);
	}
}

 	 
}

unsigned int adc_procentais(){
	return ((adc_value/8)*25)/256; 
}

unsigned int adc_procentais_P(unsigned int P){
	return ((adc_value/8)*(25-P))/256; 
}

unsigned int adc_pwm(){
	return ((adc_value/8)*25)/100; 
}

unsigned int adc_pwm_P(unsigned int P){
	return ((adc_value/8)*(25-P))/100;
}

unsigned int adc_gain(unsigned char gain){
	unsigned int temp;

	if(ADCW*gain/10<=1023) temp=ADCW*gain/10;
	else temp=1023;
	
	return temp;
}

void PI_reguliatorius(){ //LDR->PWM proporcinis reguliatorius
	
	if(LDRinv==0) LDRn=nuostato_radimas(adc_pwm_P(LDRkoef));
		else  LDRn=nuostato_radimas(255-adc_pwm_P(LDRkoef)); 
//	else LDRn=LDRn;

	
	if(LDRn>LDRn0){
			for(unsigned char i=LDRn0;i<=LDRn;i++){
			timer2_set(fadein[i]); //OCR2=fadein[i];
				#if debug_ADC
				send_string("\n\r LDRn>LDRn0:   fadein[i]: ");
				send_string(itoa(fadein[i],buff,10));
				send_string("\n\r");
				#endif	

			_delay_ms(50);
			}
	LDRn0=LDRn; //lyginimas su pries tai buvusia reiksme
	}
	
	else if (LDRn<LDRn0) {	
			for(unsigned char i=LDRn0;i>LDRn;i--){
			timer2_set(fadein[i]); //OCR2=fadein[i];
				#if debug_ADC
				send_string("\n\r LDRn<LDRn0   fadein[i]: ");
				send_string(itoa(fadein[i],buff,10));
				send_string("\n\r");
				#endif	
			_delay_ms(50);

			
			}
	LDRn0=LDRn;
	} 
	else LDRn0=LDRn0;

	#if debug_ADC
	send_string("LDRinv: ");
	send_string(itoa(LDRinv,buff,10));
	send_string(" LDRn: ");
	send_string(itoa(LDRn,buff,10));
	send_string(" LDRn0: ");
	send_string(itoa(LDRn0,buff,10));
	send_string(" OCR2: ");
	send_string(itoa(OCR2,buff,10));
	send_string("\n\r");
	#endif	

}


//funcionalumo ijungimas/isjungimas
void wmode(unsigned char kuris){

	switch(kuris){
		//OFF
		case 0: {
				TCCR2=0x00; 	//isjungiam PWM
				stop_timer0(); 	//isjungia ADC skaityma
				GICR&=~_BV(INT0);
		//		stop_timer1();
				break;
				}	
		//ON
		case 1: {
				TCCR2=0x6B; //ijungiam PWM
				timer2_set(EEPROM_read(24)); //OCR2=EEPROM_read(24);
				stop_timer0(); 	//isjungia ADC skaityma
				GICR&=~_BV(INT0);
				//stop_timer1();
				break;
				}

		//PIR/LDR
		case 2: {
				TCCR2=0x6B; //ijungiam PWM
				timer2_set(EEPROM_read(30)); //OCR2=EEPROM_read(30); //minPWM
				start_timer0(); //ADC nuskaitymas
				PIR=0;
				GICR&=~_BV(INT0); //Kol ADCW nenukrenta iki LDRth, tol PIR pertrauktis isjungta

				break;
				}			
		
		//PIR
		case 3: {
				TCCR2=0x6B; //ijungiam PWM
				timer2_set(EEPROM_read(30)); //OCR2=EEPROM_read(30); //minPWM
				stop_timer0(); 	//isjungia ADC skaityma
				GICR|=_BV(INT0);
		//		stop_timer1();
				break;
				}
		//LDR
		case 4: {
				TCCR2=0x6B; //ijungiam PWM
				start_timer0(); //ADC nuskaitymas
				GICR&=~_BV(INT0);
		//		stop_timer1();
				break;
				}


	}
	

}
 




#if debug_mode 
void debug_meniu(){

	send_string("\n\r");
	send_string("meniu:");
	send_string(itoa(meniu, buff, 10));
	send_string("; ");
	send_string("config:");
	send_string(itoa(config, buff, 10));
	send_string("; ");
	send_string("meniu_mode:");
	send_string(itoa(meniu_mode, buff, 10));
	send_string("; ");
	send_string("\n\r");
	send_string("work_mode:");
	send_string(itoa(work_mode, buff, 10));
	send_string("; ");
	send_string("menu_page:");
	send_string(itoa(menu_page, buff, 10));
	send_string("; ");
	send_string("sub_menu_page:");
	send_string(itoa(sub_menu_page, buff, 10));
	send_string("; ");
	send_string("config_menu_page:");
	send_string(itoa(config_menu_page, buff, 10));
	send_string("; ");
	send_string("\n\r");


	//config items:
	send_string("OCR2:");
	send_string(itoa(OCR2, buff, 10));
	send_string("; \n\r");
	send_string("minPWM:");
	send_string(itoa(minPWM, buff, 10));
	send_string("; ");
	send_string("maxPWM:");
	send_string(itoa(maxPWM, buff, 10));
	send_string("; ");
	send_string("timeOUT:");
	send_string(itoa(timeOUT, buff, 10));
	send_string("; ");
	send_string("LDRth:");
	send_string(itoa(LDRth, buff, 10));
	send_string("; ");
	send_string("LDRinv:");
	send_string(itoa(LDRinv, buff, 10));
	send_string("; ");
	send_string("LDRkoef:");
	send_string(itoa(LDRkoef, buff, 10));
	send_string("; \n\r");
	send_string("nuostatas:");
	send_string(itoa(nuostatas, buff, 10));
	send_string("; ");
	send_string("\n\r");

}
#endif
