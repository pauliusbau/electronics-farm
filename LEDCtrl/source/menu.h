void show_work_mode(){
	

	switch(work_mode){
		case 0: {lcd_clrscr();
		        lcd_puts_p(Menu21); //OFF
				break;}	

		case 1: {lcd_clrscr();
				lcd_puts_p(Menu20); //ON
				lcd_puts_p(dvitaskis_t);
		        lcd_puts(itoa(OCR2*100/255, buff, 10));
				lcd_puts("%");
		      	break;}	

		case 2: {lcd_clrscr();
		        lcd_puts_p(Menu1[1]); //PIR/LDR
			//	lcd_puts("");
				break;}	

		case 3: {lcd_clrscr();
		        lcd_puts_p(Menu1[2]); //PIR
				break;}	

		case 4:	{lcd_clrscr();
		        lcd_puts_p(Menu1[3]); //LDR				
				lcd_puts_p(dvitaskis_t);
				//toliau atvaizdavimas realiu laiku vyksta ISR (TIMER0_OVF_vect) pertrauktyje
				break;}	


	}
}

// reikia nustatyti menu_page_max parametra pagal tai, kiek yra meniu puslapiu

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------ ivairiu meniu isvedimas i lcd -----------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

void puslapiai(){

	lcd_clrscr();
	lcd_puts_p(Menu1[menu_page]); lcd_gotoxy(7,0); lcd_puts_p(rodykle_n);
	
	if(menu_page==menu_page_max) lcd_puts_p(Menu1[0]); else lcd_puts_p(Menu1[menu_page+1]);
}


void puslapiai_2(){

	lcd_clrscr();
	lcd_puts_p(Menu20); 
	lcd_puts("\n");
	lcd_puts_p(Menu21); 

	if(sub_menu_page==0) {lcd_gotoxy(7,0); lcd_puts_p(rodykle); }
	else {lcd_gotoxy(7,1); lcd_puts_p(rodykle);}
	

}


void puslapiai_config(){

	lcd_clrscr();
	lcd_puts_p(Menu3[config_menu_page]); lcd_gotoxy(7,0); lcd_puts_p(rodykle_n);
	
	if(config_menu_page==config_menu_page_max) lcd_puts_p(Menu3[0]); else lcd_puts_p(Menu3[config_menu_page+1]);
}

void puslapiai_config_2(){

	lcd_clrscr();
	lcd_puts_p(Menu3[config_menu_page]); 
	lcd_puts_p(dvitaskis_n);



	switch(config_menu_page){

		case 0:{ //minPWM config langas
				//live preview
				wmode(1);
				timer2_set(minPWM); //OCR2=minPWM;

				lcd_puts(itoa(minPWM*100/255, buff, 10));
				lcd_puts_p(procentas_n);				
				break;
				}

		case 1:{ //maxPWM config langas
				//live preview
				wmode(1);
				timer2_set(maxPWM); //OCR2=maxPWM;				

				lcd_puts(itoa(maxPWM*100/255, buff, 10));
				lcd_puts_p(procentas_n);
				break;
				}

		case 2:{ //timeOUT PIR pertraukciai (kiek laiko ant maxPWM bus uzdegta diodu juosta)
				lcd_puts(itoa(timeOUT, buff, 10));
				lcd_puts_p(sekunde_n);
				break;
				}
			
		case 3:{ //LDR threshold nustatymas 
				 //live preview realizuotas ISR (TIMER0_OVF_vect) pertrauktyje
				wmode(4);

				lcd_gotoxy(0,1); lcd_puts_p(R);
				// "reali" ADC reiksme atvaizduojama LDR ISR pertrauktyje 							
				lcd_gotoxy(4,1); lcd_puts_p(T); 
				lcd_puts(itoa(LDRth, buff, 10)); //iskarto procentais!
				
				break;
				}

		case 4:{ //LDR reiksmes invertavimo nustatymas
		 		//live preview realizuotas ISR (TIMER0_OVF_vect) pertrauktyje
		

				wmode(4);
				
				lcd_gotoxy(7,0);
				lcd_puts(itoa(LDRinv, buff, 10));

				lcd_gotoxy(0,1); lcd_puts_p(R);
				// "reali" ADC reiksme atvaizduojama LDR ISR pertrauktyje 							
				lcd_gotoxy(4,1); lcd_puts_p(I);
									
				break;
				}

		case 5:{//ADCgain nustatymas
				wmode(4);	

				lcd_gotoxy(0,1);

				lcd_puts(itoa(ADCgain/10, buff, 10));
				lcd_puts(".");
				lcd_puts(itoa(ADCgain%10, buff, 10));

				break;
				}
	}

}

//------------------- MENIUS ISVEDIMAS I LCD ----------------------//
//pirmas meniu lygis
void menu(){
	
		if(bit_is_set(PIND, PD4)) //i kuria puse sukasi rot encoderis
		if(menu_page!=menu_page_max) menu_page++; else menu_page=0;
		else {if(menu_page==0)	menu_page=menu_page_max; else menu_page--;}

		puslapiai();

		#if debug_mode
		send_string("menu_page: ");
		send_string(itoa((menu_page),buff,10));
		send_string("\n\r");
		#endif
}

//2as meniu lygis
void menu_2(){
	
		if(bit_is_set(PIND, PD4)) //i kuria puse sukasi rot encoderis
		if(sub_menu_page!=sub_menu_page_max) sub_menu_page++; else sub_menu_page=0;
		else {if(sub_menu_page==0)	sub_menu_page=sub_menu_page_max; else sub_menu_page--;}

		puslapiai_2();

		#if debug_mode
		send_string("sub_menu_page: ");
		send_string(itoa((sub_menu_page),buff,10));
		send_string("\n\r");
		#endif
}

//config meniu (1as lygis)
void menu_config(){
	
		if(bit_is_set(PIND, PD4)) //i kuria puse sukasi rot encoderis
		if(config_menu_page!=config_menu_page_max) config_menu_page++; else config_menu_page=0;
		else {if(config_menu_page==0)	config_menu_page=config_menu_page_max; else config_menu_page--;}

		
		
					


		puslapiai_config();

		#if debug_mode
		send_string("config_menu_page: ");
		send_string(itoa((config_menu_page),buff,10));
		send_string("\n\r");
		#endif
}

//config meniu (2as lygis)
void menu_config_2(){
		
		
		
		switch(config_menu_page){

			case 0:{ //minPWM
					if(bit_is_set(PIND, PD4)) {if (nuostatas!=(sizeof(fadein)/sizeof(fadein[0])-1))
												nuostatas++; else nuostatas=0;} 												
												
												else {if (nuostatas==0 )
														nuostatas=(sizeof(fadein)/sizeof(fadein[0])-1); else nuostatas--;}		
											
					minPWM=fadein[nuostatas];					
					timer2_set(minPWM); //OCR2=minPWM;					
					break;
					}
			
			case 1:{ //maxPWM
					if(bit_is_set(PIND, PD4)) {if (nuostatas!=(sizeof(fadein)/sizeof(fadein[0])-1))
												nuostatas++; else nuostatas=0;} 												
												
												else {if (nuostatas==0 )
														nuostatas=(sizeof(fadein)/sizeof(fadein[0])-1); else nuostatas--;}						
					
					maxPWM=fadein[nuostatas];					
					timer2_set(maxPWM); //OCR2=maxPWM;
					break;
					}

			case 2:{ //PIR TimeOUT
					if(bit_is_set(PIND, PD4)) {if (timeOUT!=60) timeOUT++; else timeOUT=0;} 
												else {if (timeOUT==0) timeOUT=60; else timeOUT--;}
					
					//timeOUT++; else timeOUT--;
					break;
					}

			case 3:{ //LDRth

					if(bit_is_set(PIND, PD4)) {if (LDRth!=100) LDRth=(255/100)*LDRth++; else LDRth=0;} 
												else {if (LDRth==0) LDRth=100; else LDRth=(255/100)*LDRth--;}			
					
					break;
					}

			case 4:{ //LDRinv

					if(bit_is_set(PIND, PD4)) {if (LDRinv!=1) LDRinv=LDRinv++; else LDRinv=0;} 
												else {if (LDRinv==0) LDRinv=1; else LDRinv--;}			
					
					break;
					}

			case 5:{ //LDRinv

					if(bit_is_set(PIND, PD4)) {if (ADCgain!=20) ADCgain++; else ADCgain=0;} 
												else {if (ADCgain==0) ADCgain=20; else ADCgain--;}			
					
					break;
					}


		}
		
		


		//if(config_menu_page!=config_menu_page_max) config_menu_page++; else config_menu_page=0;
		//else {if(config_menu_page==0)	config_menu_page=config_menu_page_max; else config_menu_page--;}

		puslapiai_config_2();

		#if debug_mode
		send_string("config_2_menu_page: ");
		send_string(itoa((config_menu_page),buff,10));
		send_string(" nuostatas: ");
		send_string(itoa((nuostatas),buff,10));
		send_string("\n\r");
		#endif
}


