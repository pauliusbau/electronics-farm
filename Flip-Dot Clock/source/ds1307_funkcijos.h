void ds1307_get_td(){
//I Time[x] skaitmenys atsigula ascii kodu


//Laikas
		//Sekundes
		DS1307Read(0x00,&data);
		Time[8]='\0';
		Time[7]=48+(data & 0b00001111);
		Time[6]=48+((data & 0b01110000)>>4);
		Time[5]=':';
	
		//Minutes
		DS1307Read(0x01,&data);
	
		Time[4]=48+(data & 0b00001111);
		Time[3]=48+((data & 0b01110000)>>4);
		Time[2]=':';
	
		//Valandos
		DS1307Read(0x02,&data);
	
		Time[1]=48+(data & 0b00001111);
		Time[0]=48+((data & 0b00110000)>>4);
		
		
//Data

		//Diena
		DS1307Read(0x04,&data);
		Date[10]='\0';
		Date[9]=48+(data & 0b00001111);
		Date[8]=48+((data & 0b00110000)>>4);
		Date[7]='-';
	
		//Menesis
		DS1307Read(0x05,&data);	
		Date[6]=48+(data & 0b00001111);
		Date[5]=48+((data & 0b00010000)>>4);
		Date[4]='-';
	
		//Metai
		DS1307Read(0x06,&data);	
		Date[3]=48+(data & 0b00001111);
		Date[2]=48+((data & 0b11110000)>>4);
		Date[1]='0';
		Date[0]='2';

}

void ds1307_set_time(){  //Irasymas i laikrodi

		temp=((sec/10)<<4)|(sec%10);
		DS1307Write(0x00,temp);
	

		temp=((min/10)<<4)|(min%10);
		DS1307Write(0x01,temp);

		temp=((hr/10)<<4)|(hr%10);
		DS1307Write(0x02,temp);

}

void ds1307_set_date(){//Irasymas i laikrodi

		temp=((date/10)<<4)|(date%10);
		DS1307Write(0x04,temp);
	

		temp=((month/10)<<4)|(month%10);
		DS1307Write(0x05,temp);

		temp=((year/10)<<4)|(year%10);
		DS1307Write(0x06,temp);



}


