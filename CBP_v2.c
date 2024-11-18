#include<reg51.h>
#include<string.h>
#include<stdio.h>

#define lcdport P1

#define PWM_Period 0xB7FE

#define ON 1
#define OFF 0

sbit row1=P2^0;
sbit row2=P2^1;
sbit row3=P2^2;
sbit row4=P2^3;

sbit col1=P2^4;
sbit col2=P2^5;
sbit col3=P2^6;
sbit col4=P2^7;

sbit rs=P1^0;
sbit rw=P1^1;
sbit en=P1^2;

sbit m1=P0^4;

sbit buzzer=P0^2;

unsigned int ON_Period, OFF_Period, DutyCycle;

char i,rx_data[50];

char rfid[13],ch=0;

char pass[4];


void servo_delay(unsigned int count)
{
    int i,j;
    for(i=0; i<count; i++)
			for(j=0; j<112; j++);
}

 void delay(int itime)

{

    int i,j;

    for(i=0;i<itime;i++)

    for(j=0;j<1275;j++);

}


void daten()

{

    rs=1;

    rw=0;

    en=1;

    delay(5);

    en=0;

}


void lcddata(unsigned char ch)

{

    lcdport=ch & 0xf0;

    daten();

    lcdport=(ch<<4) & 0xf0;

    daten();

}



void cmden(void)

{

    rs=0;

    en=1;

    delay(5);

    en=0;

}


void lcdcmd(unsigned char ch)

{

    lcdport=ch & 0xf0;

    cmden();

    lcdport=(ch<<4) & 0xf0;

    cmden();

}


void lcdstring(char *str)

{

    while(*str)

    {

        lcddata(*str);

        str++;

    }

}


void lcd_init(void)

{

    lcdcmd(0x02);

    lcdcmd(0x28);

    lcdcmd(0x0e);

    lcdcmd(0x01);

}


void uart_init()

{

	TMOD=0x21;
	
	SCON=0x50;
	
	TH1=0xfd;
	
	TR1=1;
	
	TH0 = (PWM_Period >> 8);/* 20ms timer value */
	
	TL0 = PWM_Period;
	
	TR0 = 1;		/* Start timer0 */

}

/* Timer0 interrupt service routine (ISR) */
void Timer0_ISR() interrupt 1	
{
	m1 = !m1;
	if(m1)
	{
		TH0 = (ON_Period >> 8);
		TL0 = ON_Period;
	}	
	else
	{
		TH0 = (OFF_Period >> 8);
		TL0 = OFF_Period;
	}	
			
}

/* Calculate ON & OFF period from duty cycle */
void Set_DutyCycle_To(float duty_cycle)
{
	float period = 65535 - PWM_Period;
	
	ON_Period = ((period/100.0) * duty_cycle);
	
	OFF_Period = (period - ON_Period);	
	
	ON_Period = 65535 - ON_Period;	
	
	OFF_Period = 65535 - OFF_Period;
}

char rxdata()

{

  while(!RI);

    ch=SBUF;    

    RI=0;

    return ch;

}


void keypad()

{

	lcdcmd(1);

    lcdstring("Enter Ur Passkey");

    lcdcmd(0xc0);

    i=0;

    while(i<4)

    {

		col1=0;
	
	    col2=col3=col4=1;
	
	    if(!row1)
	
	    {
	
	        lcddata('1');
	
			delay(100);
	
	        pass[i++]='1';
	
	        while(!row1);
	
		}
	
	    else if(!row2)
	
	    {
	
	        lcddata('4');
	
			delay(100);
	
	        pass[i++]='4';
	
	        while(!row2);
	
	    }
	
	    else if(!row3)
	
	    {
	
	        lcddata('7');
	
			delay(100);
	
	        pass[i++]='7';
	
	        while(!row3);
	
	    }
	
	    else if(!row4)
	
	    {
	
	        lcddata('*');
	
			delay(100);
	
	        pass[i++]='*';
	
	        while(!row4);
	
	    }
	
	    col2=0;
	
	    col1=col3=col4=1;
	
	    if(!row1)
	
	    {
	
	        lcddata('2');
	
			delay(100);
	
	        pass[i++]='2';
	
	        while(!row1);
	
	    }
	
	    else if(!row2)
	
	    {
	
	        lcddata('5');
	
			delay(100);
	
	        pass[i++]='5';
	
	        while(!row2);
	
	    } 
	
	    else if(!row3)
	
	    {
	
	        lcddata('8');
	
			delay(100);
	
	        pass[i++]='8';
	
	        while(!row3);
	
	    }
	
	    else if(!row4)
	
	    {
	
	        lcddata('0');
	
			delay(100);
	
	        pass[i++]='0';
	
	        while(!row4);
	
	    }
	
	    col3=0;
	
	    col1=col2=col4=1;
	
	    if(!row1)
	
	    {
	
	        lcddata('3');
	
			delay(100);
	
	        pass[i++]='3';
	
	        while(!row1);
	
	    }
	
	    else if(!row2)
	
	    {
	
	        lcddata('6');
	
			delay(100);
	
	        pass[i++]='6';
	
	        while(!row2);
	
	    }
	
	    else if(!row3)
	
	    {
	
	        lcddata('9');
	
			delay(100);
	
	        pass[i++]='9';
	
	        while(!row3);
	
	    }
	
	    else if(!row4)
	
	    {
	
	        lcddata('#');
	
			delay(100);
	
	        pass[i++]='#';
	
	        while(!row4);
	
	    }
	
	    col4=0;
	
	    col1=col3=col2=1;
	
	    if(!row1)
	
	    {
	
	        lcddata('A');
	
			delay(100);
	
	        pass[i++]='A';
	
	        while(!row1);
	
	    }
	
	    else if(!row2)
	
	    {
	
	        lcddata('B');
	
			delay(100);
	
	        pass[i++]='B';
	
	        while(!row2);
	
	    }
	
	    else if(!row3)
	
	    {
	
	        lcddata('C');
	
			delay(100);
	
	        pass[i++]='C';
	
	        while(!row3);
	
	    }  
	
	    else if(!row4)
	
	    {
	
	        lcddata('D');
	
			delay(100);
	
	        pass[i++]='D';
	
	        while(!row4);
	
	    }
	} 
}


void accept()

{

    lcdcmd(1);

    lcdstring("Welcome");

    lcdcmd(192);

    lcdstring("Password Accept");

    delay(200);

}


void wrong()

{

	buzzer=ON;

    lcdcmd(1);

    lcdstring("Wrong Passkey");

    lcdcmd(192);

    lcdstring("PLZ Try Again");

    delay(200);

    buzzer=OFF;

}


void main()

{
 	EA  = 1;		/* Enable global interrupt */
    ET0 = 1;         	/* Enable timer0 interrupt */
    
	buzzer=0;

    uart_init();

    lcd_init();

    lcdstring("  RFID Based    ");

    lcdcmd(0xc0);

    lcdstring("Security system "); 

    delay(400);

    while(1)

    {

    	lcdcmd(1);

        lcdstring("Place Your Card:");

        lcdcmd(0xc0);

        i=0;

        for(i=0;i<12;i++)

        rfid[i]=rxdata();

        rfid[i]='\0';

		delay(200);

        lcdcmd(1);

        lcdstring("Your ID No. is:");

        lcdcmd(0xc0);

        for(i=0;i<12;i++)

        lcddata(rfid[i]);

        delay(100);

        if(strncmp(rfid,"254006A1498B",12)==0)

        {

            keypad();

            if(strncmp(pass,"1704",4)==0)

            {

	            accept();
	
	            lcdcmd(1);
	
	            lcdstring("Access Granted ");
	
	            lcdcmd(0xc0);
	
	            lcdstring("Aditya");
	
	            Set_DutyCycle_To(2.7);/* 0.54ms(2.7%) of 20ms(100%) period */
	
				servo_delay(1000);
	
				//Set_DutyCycle_To(9);/* 1.4ms(7%) of 20ms(100%) period */
	
				servo_delay(1000);
	
				Set_DutyCycle_To(12);/* 2.4ms(12%) of 20ms(100%) period */
	
				servo_delay(1000);

           }

           else

            wrong();

        }

        else if(strncmp(rfid,"160066BD7AB7",12)==0)

        {

            keypad();

            if(strncmp(pass,"4202",4)==0)

            {

	            accept();
	
	            lcdcmd(1);
	
	            lcdstring("Access Granted ");
	
	            lcdcmd(0xc0);
	
	            lcdstring("Person2");
	
	            Set_DutyCycle_To(2.7);/* 0.54ms(2.7%) of 20ms(100%) period */
	
				servo_delay(1000);
	
				//Set_DutyCycle_To(9);/* 1.4ms(7%) of 20ms(100%) period */
	
				servo_delay(1000);
	
				Set_DutyCycle_To(12);/* 2.4ms(12%) of 20ms(100%) period */
	
				servo_delay(1000);

        	}

        	else

            wrong();
 
        }   

        else if(strncmp(rfid,"160066203060",12)==0)

        {

    		keypad();

            if(strncmp(pass,"4203",4)==0)

            {

	            accept();
	
	            lcdcmd(1);
	
	            lcdstring("Access Granted ");
	
	            lcdcmd(0xc0);
	
	            lcdstring("Person3");
	
	            Set_DutyCycle_To(2.7);/* 0.54ms(2.7%) of 20ms(100%) period */
	
				servo_delay(1000);
	
				//Set_DutyCycle_To(9);/* 1.4ms(7%) of 20ms(100%) period */
	
				servo_delay(1000);
	
				Set_DutyCycle_To(12);/* 2.4ms(12%) of 20ms(100%) period */
	
				servo_delay(1000);

          	}

            else

            wrong();

    	}


        else 

        {

           lcdcmd(1);

           lcdstring("Access Denied");

           buzzer=ON;

           delay(200);

           buzzer=OFF;

        }

	}

}
