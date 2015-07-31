//-------------eilutes--------

#define A0_12 PINA0
#define A1_12 PINA1
#define A2_12 PINA2
#define B0_12 PINA3
#define B1_12 PINA4
#define E1	  PINA5
#define E2 	  PINA6


//------------stulpeliai-------

#define A0_3  PINC2			//PINC4
#define A1_3  PINC3
#define A2_3  PINC4			//PINC2
#define B0_3  PINC5			//PIND7
#define B1_3  PIND6
#define E3	  PIND4
#define D3 	  PIND5





void segmentas (unsigned short stulpelis, unsigned short eilute, unsigned short spalva){ //spalva: 1 - geltona, 0 - juoda

switch(stulpelis){
	case 1: PORTC |= _BV(A0_3);
			//PORTC |= _BV(A1_3);
			//PORTC |= _BV(A2_3);
			//PORTC |= _BV(B0_3);
			//PORTD |= _BV(B1_3);
			break;

	case 2: PORTC |= _BV(A1_3);
			break;

	case 3: PORTC |= _BV(A0_3); 
			PORTC |= _BV(A1_3);
			break;

	case 4: PORTC |= _BV(A2_3);
			break;

	case 5: PORTC |= _BV(A0_3);
			//PORTC |= _BV(A1_3);
			PORTC |= _BV(A2_3);
			//PORTC |= _BV(B0_3);
			//PORTD |= _BV(B1_3);
			break;

	case 6: //PORTC |= _BV(A0_3);
			PORTC |= _BV(A1_3);
			PORTC |= _BV(A2_3);
			//PORTC |= _BV(B0_3);
			//PORTD |= _BV(B1_3);
			break;

	case 7: PORTC |= _BV(A0_3);
			PORTC |= _BV(A1_3);
			PORTC |= _BV(A2_3);
			//PORTC |= _BV(B0_3);
			//PORTD |= _BV(B1_3);
			break;

	case 8: PORTC |= _BV(A0_3);
			//PORTC |= _BV(A1_3);
			//PORTC |= _BV(A2_3);
			PORTC |= _BV(B0_3);
			//PORTD |= _BV(B1_3);
			break;

	case 9: //PORTC |= _BV(A0_3);
			PORTC |= _BV(A1_3);
			//PORTC |= _BV(A2_3);
			PORTC |= _BV(B0_3);
			//PORTD |= _BV(B1_3);
			break;

	case 10: PORTC |= _BV(A0_3);
			PORTC |= _BV(A1_3);
			//PORTC |= _BV(A2_3);
			PORTC |= _BV(B0_3);
			//PORTD |= _BV(B1_3);
			break;

	case 11: //PORTC |= _BV(A0_3);
			//PORTC |= _BV(A1_3);
			PORTC |= _BV(A2_3);
			PORTC |= _BV(B0_3);
			//PORTD |= _BV(B1_3);
			break;

	case 12: PORTC |= _BV(A0_3);
			//PORTC |= _BV(A1_3);
			PORTC |= _BV(A2_3);
			PORTC |= _BV(B0_3);
			//PORTD |= _BV(B1_3);
			break;

	case 13: //PORTC |= _BV(A0_3);
			PORTC |= _BV(A1_3);
			PORTC |= _BV(A2_3);
			PORTC |= _BV(B0_3);
			//PORTD |= _BV(B1_3);
			break;

	case 14: PORTC |= _BV(A0_3);
			PORTC |= _BV(A1_3);
			PORTC |= _BV(A2_3);
			PORTC |= _BV(B0_3);
			//PORTD |= _BV(B1_3);
			break;

	case 15: PORTC |= _BV(A0_3);
			//PORTC |= _BV(A1_3);
			//PORTC |= _BV(A2_3);
			//PORTC |= _BV(B0_3);
			PORTD |= _BV(B1_3);
			break;

	case 16: //PORTC |= _BV(A0_3);
			PORTC |= _BV(A1_3);
			//PORTC |= _BV(A2_3);
			//PORTC |= _BV(B0_3);
			PORTD |= _BV(B1_3);
			break;

	case 17: PORTC |= _BV(A0_3);
			PORTC |= _BV(A1_3);
			//PORTC |= _BV(A2_3);
			//PORTC |= _BV(B0_3);
			PORTD |= _BV(B1_3);
			break;

	case 18: //PORTC |= _BV(A0_3);
			//PORTC |= _BV(A1_3);
			PORTC |= _BV(A2_3);
			//PORTC |= _BV(B0_3);
			PORTD |= _BV(B1_3);
			break;

	case 19: PORTC |= _BV(A0_3);
			//PORTC |= _BV(A1_3);
			PORTC |= _BV(A2_3);
			//PORTC |= _BV(B0_3);
			PORTD |= _BV(B1_3);
			break;

	case 20: //PORTC |= _BV(A0_3);
			PORTC |= _BV(A1_3);
			PORTC |= _BV(A2_3);
			//PORTC |= _BV(B0_3);
			PORTD |= _BV(B1_3);
			break;

	case 21: PORTC |= _BV(A0_3);
			PORTC |= _BV(A1_3);
			PORTC |= _BV(A2_3);
			//PORTC |= _BV(B0_3);
			PORTD |= _BV(B1_3);
			break;

	case 22: PORTC |= _BV(A0_3);
			//PORTC |= _BV(A1_3);
			//PORTC |= _BV(A2_3);
			PORTC |= _BV(B0_3);
			PORTD |= _BV(B1_3);
			break;

	case 23: //PORTC |= _BV(A0_3);
			PORTC |= _BV(A1_3);
			//PORTC |= _BV(A2_3);
			PORTC |= _BV(B0_3);
			PORTD |= _BV(B1_3);
			break;

	case 24: PORTC |= _BV(A0_3);
			PORTC |= _BV(A1_3);
			//PORTC |= _BV(A2_3);
			PORTC |= _BV(B0_3);
			PORTD |= _BV(B1_3);
			break;

	case 25: //PORTC |= _BV(A0_3);
			//PORTC |= _BV(A1_3);
			PORTC |= _BV(A2_3);
			PORTC |= _BV(B0_3);
			PORTD |= _BV(B1_3);
			break;

	case 26: PORTC |= _BV(A0_3);
			//PORTC |= _BV(A1_3);
			PORTC |= _BV(A2_3);
			PORTC |= _BV(B0_3);
			PORTD |= _BV(B1_3);
			break;

	case 27: //PORTC |= _BV(A0_3);
			PORTC |= _BV(A1_3);
			PORTC |= _BV(A2_3);
			PORTC |= _BV(B0_3);
			PORTD |= _BV(B1_3);
			break;

	case 28: PORTC |= _BV(A0_3);
			PORTC |= _BV(A1_3);
			PORTC |= _BV(A2_3);
			PORTC |= _BV(B0_3);
			PORTD |= _BV(B1_3);
			break;

}	

switch(eilute){
	case 1: //PORTA |= _BV(A0_12);
			PORTA |= _BV(A1_12);
			//PORTA |= _BV(A2_12);
			//PORTA |= _BV(B0_12);
			PORTA |= _BV(B1_12);
			break;
			
	case 2: PORTA |= _BV(A0_12);
			//PORTA |= _BV(A1_12);
			//PORTA |= _BV(A2_12);
			//PORTA |= _BV(B0_12);
			PORTA |= _BV(B1_12);
			break;
	case 3: PORTA |= _BV(A0_12);
			PORTA |= _BV(A1_12);
			PORTA |= _BV(A2_12);
			PORTA |= _BV(B0_12);
			//PORTA |= _BV(B1_12);
			break;
			
	case 4: //PORTA |= _BV(A0_12);
			PORTA |= _BV(A1_12);
			PORTA |= _BV(A2_12);
			PORTA |= _BV(B0_12);
			//PORTA |= _BV(B1_12);
			break;
			
	case 5: PORTA |= _BV(A0_12);
			//PORTA |= _BV(A1_12);
			PORTA |= _BV(A2_12);
			PORTA |= _BV(B0_12);
			//PORTA |= _BV(B1_12);
			break;		

	case 6: //PORTA |= _BV(A0_12);
			//PORTA |= _BV(A1_12);
			PORTA |= _BV(A2_12);
			PORTA |= _BV(B0_12);
			//PORTA |= _BV(B1_12);
			break;

	case 7: PORTA |= _BV(A0_12);
			PORTA |= _BV(A1_12);
			//PORTA |= _BV(A2_12);
			PORTA |= _BV(B0_12);
			//PORTA |= _BV(B1_12);
			break;

	case 8: //PORTA |= _BV(A0_12);
			PORTA |= _BV(A1_12);
			//PORTA |= _BV(A2_12);
			PORTA |= _BV(B0_12);
			//PORTA |= _BV(B1_12);
			break;

	case 9: PORTA |= _BV(A0_12);
			//PORTA |= _BV(A1_12);
			//PORTA |= _BV(A2_12);
			PORTA |= _BV(B0_12);
			//PORTA |= _BV(B1_12);
			break;

	case 10: PORTA |= _BV(A0_12);
			PORTA |= _BV(A1_12);
			PORTA |= _BV(A2_12);
			//PORTA |= _BV(B0_12);
			//PORTA |= _BV(B1_12);
			break;

	case 11: //PORTA |= _BV(A0_12);
			PORTA |= _BV(A1_12);
			PORTA |= _BV(A2_12);
			//PORTA |= _BV(B0_12);
			//PORTA |= _BV(B1_12);
			break;

	case 12: PORTA |= _BV(A0_12);
			//PORTA |= _BV(A1_12);
			PORTA |= _BV(A2_12);
			//PORTA |= _BV(B0_12);
			//PORTA |= _BV(B1_12);
			break;

	case 13: //PORTA |= _BV(A0_12);
			//PORTA |= _BV(A1_12);
			PORTA |= _BV(A2_12);
			//PORTA |= _BV(B0_12);
			//PORTA |= _BV(B1_12);
			break;

	case 14: PORTA |= _BV(A0_12);
			PORTA |= _BV(A1_12);
			//PORTA |= _BV(A2_12);
			//PORTA |= _BV(B0_12);
			//PORTA |= _BV(B1_12);
			break;

	case 15: //PORTA |= _BV(A0_12);
			PORTA |= _BV(A1_12);
			//PORTA |= _BV(A2_12);
			//PORTA |= _BV(B0_12);
			//PORTA |= _BV(B1_12);
			break;

	case 16: PORTA |= _BV(A0_12);
			//PORTA |= _BV(A1_12);
			//PORTA |= _BV(A2_12);
			//PORTA |= _BV(B0_12);
			//PORTA |= _BV(B1_12);
			break;

}



// krypties isrinkimas ir siuntimas i lenta
if (spalva==1){
	PORTD != ~(_BV(D3));

	//enablai
	PORTA |= _BV(E1);
	PORTD |= _BV(E3);
	_delay_ms(4);	
	}	


	else if (spalva==0){
	//kryptis
	PORTD |= _BV(D3);


	//enablai
	PORTA |= _BV(E2);
	PORTD |= _BV(E3);
	_delay_ms(4);
	}

		else {
		PORTA=0x00;
		PORTC=0x00;
		PORTD=0x00;
		}


// nunulinam portus
PORTA=0x00;
PORTC=0x00;
PORTD=0x00;
}
