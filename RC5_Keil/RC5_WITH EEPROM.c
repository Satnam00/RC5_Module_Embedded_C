#include <reg51.H> 		 // P0 is data port
#include<intrins.h> //For using [_nop_()]
// lcd wala code start
#define  lcd_data_pin P0

sbit sda=P3^7;
sbit scl=P3^6;

sbit led =P3^3;
sbit led1=P3^4;
					  
sbit rs=P2^7;
sbit en=P2^6;
sbit L1 = P1^0;
sbit L2 = P1^1;
sbit L3 = P1^2;
sbit L4 = P1^3;
sbit L5 = P1^4;
sbit L6 = P1^5;
sbit L7 = P1^6;
sbit L8 = P1^7;
 
unsigned char reead,mem_data;
bit ack;
   
void delay(unsigned int count){
	int i,j;	
	for(i=0;i<count;i++)		
		for(j=0;j<1275;j++);
}
void lcd_command(unsigned char comm)
{	lcd_data_pin=comm;	en=1;	rs=0;	delay(1);	en=0;}
void lcd_data(unsigned char disp)
{	lcd_data_pin=disp;	en=1;	rs=1;	delay(1);	en=0;}

void lcd_dataa(unsigned char *disp)
{	int x;	
	for(x=0;disp[x]!='\0';x++)	{	lcd_data(disp[x]);	}
}
void lcd_ini()
{	
	lcd_command(0x38); delay(5);		  // for using 8-bit 2 row LCD 
	lcd_command(0x0F); delay(5);       // for display on, cursor blinking	
	lcd_command(0x80);	delay(5);
}

///////////////////LCD code over

//char arr[10]={0xbf,0x86,0xdb,0xcf,0xe6,0xed,0xfd,0x87,0xff,0xe7};
char arr[10]={'0','1' ,'2','3','4','5', '6','7','8','9'};
char sbuffer[10], ch, toggle, last_toggle;
unsigned char pos;
unsigned int rc5data;

//receive serial character from serial port
char mygetchar(void)
{
        char c;
        while(!RI);
        RI = 0;
        c  = SBUF;
        return SBUF;
}
//// I2C
void aknowledge()	  //acknowledge condition
{	scl=1;	_nop_();	_nop_();	scl=0;}

void start()		//start condition
{	
	sda=1;	scl=1;	
	_nop_();    _nop_();	     //No operation
 	sda=0;	scl=0;
}
void stop()			//stop condition
{	
	sda=0;	scl=1;	
	_nop_();  _nop_(); 	
	sda=1;	scl=0;
}

void send_byte(unsigned char value)	//send byte serially
{ 	
	unsigned int i;	unsigned char send;	
	send=value;	
	for(i=0;i<8;i++)				   // 0000 0100	 -65
	{		sda=send/128;			//extracting MSB	
			send=send<<1;			//shiftng left	
			scl=1;		_nop_();		_nop_();	
			scl=0;	
	}  
	 ack=sda;					//reading acknowledge  
	 sda=0;
}	
void save(char s)					//save in EEPROM
{	// int i; 
	start();	
		send_byte(0xA0);	aknowledge();		//device address
		send_byte(0x00);	aknowledge();			//word address
		
		send_byte(s);		aknowledge();			//send data
	 /*for (i=0;str[i] != '\0';i++ ) 	
	 {	    send_byte(str[i]);	 	aknowledge();	 } 
		  	send_byte('\0');	 	aknowledge();
	   */
	stop();		
		if(ack==0)	  	
		{	led1=1;		delay(100);		
			led1=0;		delay(100);		
			lcd_command(0x86);
			lcd_data('o' );
			lcd_command(0x87);	
			lcd_data('k');	
		}else	
			led1=1;	
		aknowledge();
}
unsigned char read_byte()			//reading from EEPROM serially
{	
	unsigned int i;	sda=1;	reead=0;	
	for(i=0;i<8;i++)	{		
		reead=reead<<1;		
		scl=1;		_nop_();	_nop_();	
		if(sda==1)			reead++;		
		scl=0;	
	}	
	sda=0;	return reead;				//Returns 8 bit data here
}
void Read()
{	
	start();	
		send_byte(0xA0);	aknowledge();		//device address
		send_byte(0x00);	aknowledge();		//word address
	start();	
		send_byte(0xA1);	//device address	
		aknowledge();	
	
	  	lcd_command(0xc6);	 // bring cursro to line 2 - 6 column
	
	mem_data=read_byte();	aknowledge();
   	/*while (j != '\0') 
	{	lcd_data(j);
	 	j=read_byte(); aknowledge(); 	
	} */	
	stop();
}


void main()
{			 
    pos = 0;
	  lcd_ini();
	  lcd_dataa("hello"); 

    SCON = 0x52; // 8-bit UART mode
    TMOD = 0x20; // timer 1 mode 2 auto reload
    TH1	 = 0xfd; // 9600 8-n-1
    TR1  = 1; // run timer1
    last_toggle = -1;
    L1=L2=L3=L4=L5=L6=L7=L8=0;  
	while(1)
    {	


		ch = mygetchar(); //loop till character received
        if(ch==0x0A) // if received character is <LF> end of line, time to display
        {
                pos = 0; // buffer position reset for next reading// extract rc5 data from serial buffer to 16 bit value
                rc5data = ((sbuffer[2]-'0')<<4)|(sbuffer[3]-'0'); // convert data from ASCII to low byte
                rc5data |= (((sbuffer[0]-'0')<<4)|(sbuffer[1]-'0'))<<8; // convert data from ASCII to high byte                                                
                if((rc5data&0x0800)==0x0800) // check toggle bit from rc5 data is set
                        toggle = 1;
                 else 
                        toggle = 0;
                if(toggle!=last_toggle) // check if new key is pressed
                {
                    last_toggle = toggle; // store value for next time compare
                    switch(rc5data&0x003F) // mask other bits and compare command bytes
                    {	case 0x00:      lcd_data (arr[0]); //P0=arr[0];
											break;
                        case 0x01:     	lcd_data (arr[1]); //P0=arr[1];
										L1= !L1; break;
                        case 0x02:      lcd_data (arr[2]); //P0=arr[2];                // Key2 pressed
                                        L2= !L2;break;
                        case 0x03:      lcd_data (arr[3]); //P0=arr[3];          // Key3 pressed
                                        L3= !L3 ;break;         
						case 0x04:      lcd_data (arr[4]); //P0=arr[4];                                                                                                                                   
                                        L4= !L4; break;
                        case 0x05:      lcd_data (arr[5]); //P0=arr[5];                 // Key2 pressed
                                        
										L5= !L5; break;
                        case 0x06:      lcd_data (arr[6]); //P0=arr[6];         // Key3 pressed
                                        L6= !L6 ; break;    
						case 0x07:      lcd_data (arr[7]); //P0=arr[7];                 // Key2 pressed
                                        L7=!L7 ; break;
                        case 0x08:      lcd_data (arr[8]); //P0=arr[8];          // Key3 pressed
                                        L8=!L8; break;     
						case 0x09:      lcd_data (arr[9]); //P0=arr[9];          // Key3 pressed
                                        break;                                                                   
                    }
					// make the status in mem_data variable 
				
					if (L1 ==1) mem_data |= 1;
					if (L2 ==1) mem_data |= 2;
				    if (L3 ==1) mem_data |= 4;
					if (L4 ==1) mem_data |= 8;
					if (L5 ==1) mem_data |= 16;
					if (L6 ==1) mem_data |= 32;
				    if (L7 ==1) mem_data |= 64;
					if (L8 ==1) mem_data |= 128;
				
                }
        }
        else
        {                             //store serial data to buffer
                sbuffer[pos] = ch;
                pos++;
        }
    } // end while
}// end main