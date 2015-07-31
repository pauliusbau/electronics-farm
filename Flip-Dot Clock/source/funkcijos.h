void init_isr(){

//---Mygtukø pertrauktis

	MCUCR&=~(_BV(ISC11)) | ~(_BV(ISC10));
	MCUCR&=~(_BV(ISC01)) | ~(_BV(ISC00));
	GICR|=_BV(INT1)|_BV(INT0); 
	//GICR=_BV(INT0);

}

void init_timer(){

	//NAUJAS, Timeris 0.
	TCCR0=_BV(CS00)|_BV(CS02);
	
    /* Enable timer 0 overflow interrupt. */
//    TIMSK = _BV (TOIE0);

	//Initialize Counter
    TCNT0=0;
}


void timer_init_1(){ //count down timer
 
     TCCR1B=(1<<WGM12)|(1<<CS11)|(1<<CS10);
     OCR1A=0x007C;

}


void timer_1_reset(){
	clock_second=0;
	clock_millisecond=0;
	timer=0;
}

void start_timer1() {TIMSK |=(_BV(OCIE1A));}
void stop_timer1()	{TIMSK &=~(_BV(OCIE1A));}






void init_uart(unsigned int ubrr) {
	/* Set baud rate */
	UBRRH = (unsigned char)(ubrr>>8 ) ;
	UBRRL = (unsigned char)(ubrr);
	/* Enable receiver and transmitter */
	UCSRB |= (1<<RXEN);
	UCSRB |= (1<<TXEN);
	/* Set frame format: 8data, 1stop bit */
	UCSRC = (1<<URSEL)|(3<<UCSZ0); //|(1<<USBS)

	UCSRB |= (1 << RXCIE); // RXCIE, Enable RX interrupt


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


void rtc_init(void){
	//Clear CH bit of RTC
	#define CH 7

	
	DS1307Read(0x00,&temp);

	//Clear CH Bit
	temp&=(~(1<<CH));

	DS1307Write(0x00,temp);

	

	//Set 24 Hour Mode
	DS1307Read(0x02,&temp);

	//Set 24 Hour BIT
	temp&=(0b10111111);

	//Write Back to DS1307
	DS1307Write(0x02,temp);
	
	//Set Squore Wave Output to 1Hz
	DS1307Read(0x07,&temp);

	//Set Squore Wave Output to 1Hz
	temp|=(0b10010000);

	temp&=(0b11111100);

	//Write Back to DS1307
	DS1307Write(0x07,temp);

}


void pilnas_geltonas(){
	
	for (int i=0;i<=28;i++){
	for (int j=0;j<=16;j++){
	segmentas(i,j,1);}
	}

}

void pilnas_juodas(){

	for (int i=0;i<=28;i++){
	for (int j=0;j<=16;j++){
	segmentas(i,j,0);}
	}

}



void laikas(unsigned char pstulpelis, unsigned char peilute){


	print_number((Time[0]-48),pstulpelis,peilute);
	print_number((Time[1]-48),(pstulpelis+4),peilute);
	print_number((Time[2]-48),(pstulpelis+7),peilute);
	print_number((Time[3]-48),(pstulpelis+10),peilute);
	print_number((Time[4]-48),(pstulpelis+14),peilute);


}


void print_number(unsigned char number, unsigned char pst, unsigned char peil){

switch (number){

	case 0: print_column(0,pst,peil);
			print_column(1,pst+1,peil);
			print_column(2,pst+2,peil);
			break;

	case 1: print_column(3,pst,peil);
			print_column(4,pst+1,peil);
			print_column(5,pst+2,peil);
			break;

	case 2: print_column(6,pst,peil);
			print_column(7,pst+1,peil);
			print_column(8,pst+2,peil);
			break;

	case 3: print_column(9,pst,peil);
			print_column(10,pst+1,peil);
			print_column(11,pst+2,peil);
			break;

	case 4: print_column(12,pst,peil);
			print_column(13,pst+1,peil);
			print_column(14,pst+2,peil);
			break;

	case 5: print_column(15,pst,peil);
			print_column(16,pst+1,peil);
			print_column(17,pst+2,peil);
			break;

	case 6: print_column(18,pst,peil);
			print_column(19,pst+1,peil);
			print_column(20,pst+2,peil);
			break;

	case 7: print_column(21,pst,peil);
			print_column(22,pst+1,peil);
			print_column(23,pst+2,peil);
			break;

	case 8: print_column(24,pst,peil);
			print_column(25,pst+1,peil);
			print_column(26,pst+2,peil);
			break;

	case 9: print_column(27,pst,peil);
			print_column(28,pst+1,peil);
			print_column(29,pst+2,peil);
			break;

	// :
	case 10: 	print_column(30,pst,peil);
				print_column(31,pst+1,peil);
				print_column(32,pst+2,peil);
				break;

}

}



void print_column(unsigned char elementas, unsigned char pst, unsigned char peil){

	unsigned int eil=fdfont[elementas];

	segmentas(pst,peil,(eil & 0x01));
	
	for (uint8_t j=0;j<4;j++){
		eil=eil>>1;
		peil=peil++;
		segmentas(pst,peil,(eil & 0x01));
	}


}


void bruksniukas(unsigned char pstulpelis, unsigned char peilute,unsigned char ilgis, unsigned char spalva){
	
	for(unsigned char i=0; i<ilgis; i++){
		segmentas(pstulpelis+i,peilute,spalva);
	}

}



//--------------------------------------------FET status----------------------------------------

void show_fet_status(void){

	switch(fet_status){
						case 0: send_string("FET 1 [OFF] & FET 2 [OFF] \n\r"); break;
						case 1: send_string("FET 1 [ON] & FET 2 [OF] \n\r"); break;
						case 2: send_string("FET 1 [OFF] & FET 2 [ON] \n\r"); break;
	
	
	}	 
}
//----------------------------------------------------------------------------------------

void apie(void){
//--------------------------------------------Apie----------------------------------------

	send_string("-------------------------");
	send_string("\n\r");
	send_string("flip-dot clock v1.4\n\r");
	send_string("Paulius Bautrenas 2014");
	send_string("\n\r"); 
		
	send_string("\n\r");
	ds1307_get_td();
	//send_string("  ");
	send_string(Date);
	send_string("  ");
	send_string(Time);
	send_string("\n\r");
	show_fet_status();	
	send_string("-------------------------");
	send_string("\n\r"); 

//----------------------------------------------------------------------------------------
}

void meniu(void){
//--------------------------------------------Meniu----------------------------------------
	send_string("Meniu:\n\r");
	send_string("a - about\n\r");
	send_string("t - set time\n\r");
	send_string("d - set date\n\r");
	send_string("f - select FET\n\r");
	send_string("c - clear screen\n\r");
	send_string("m - main meniu\n\r");
	send_string("x - exit\n\r");
	send_string("\n\r"); 	send_string("\n\r");

//----------------------------------------------------------------------------------------

}




char read_keypad () {// funkcija klausytis klaviaturos

	char which_button;

	
	while(1) { 

		// Check button 1 [ >>, OK  ]
		if (bit_is_clear(PIND, PD2)) {
			if (button_pressed == 0) {
				button_pressed = 1;
				which_button = 2;
					
			}				 
		} 

		// Check button 2 [ ++ ]
		else if (bit_is_clear(PIND, PD3)) {
			if (button_pressed == 0) {
				button_pressed = 1; 
				which_button = 3;
			} 
		} 


		else { 
			button_pressed = 0;
			break; // kad skaitmenis visa laika rodytu
		}

		if (button_pressed == 1) {
			break;
		}		
	}

	return which_button;

}




#if debug_mode

void debug_light(){
	send_string("set_fet: ");
	send_string(itoa(set_fet,buff,10));
	send_string(" fet_status: ");
	send_string(itoa(fet_status,buff,10));

	send_string(" fet_count: ");
	send_string(itoa(fet_count,buff,10));
	send_string(" fet_timeout: ");
	send_string(itoa(fet_timeout,buff,10));

}

#endif
