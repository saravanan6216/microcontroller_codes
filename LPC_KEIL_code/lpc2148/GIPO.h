#include<LPC214x.h>
#define input 0
#define output 1
#define HIGH 1
#define LOW 0
void pinMode(int pinNo,int Dir)
{
	if(pinNo<100)
	{
		if(Dir==output)
		{
			IODIR0=IODIR0|(1<<pinNo);
		}
		else
		{
			IODIR0=IODIR0&(~(1<<pinNo));
		}
			
	}
	else
	{
		pinNo=pinNo-100;
		if(Dir==output)
		{
			IODIR1=IODIR1|(1<<pinNo);
		}
		else
		{
			IODIR1=IODIR1&(~(1<<pinNo));
		}
	}
}

void delay(int x)
{
	int i,j;
	for(i=0;i<x;i++)
	{
		for(j=0;j<2000 ;j++);
	}
}
void digitalWrite(int pinNo,unsigned int value)
{
	if(pinNo<100)
	{
		if(value==HIGH)
		{
			IOSET0=IOSET0|(1<<pinNo);
		}
		else
		{
			IOCLR0=IOCLR0|(1<<pinNo);
		}
			
	}
	else
	{
		pinNo=pinNo-100;
		if(value==HIGH)
		{
			IOSET1=IOSET1|(1<<pinNo);
		}
		else
		{
			IOCLR1=IOCLR1|(1<<pinNo);
		}
	}
}
int digitalRead(int pinNo)
{
	if(pinNo<100)
	{
		if((IOPIN0&(1<<pinNo)))
		{
			return 1;
		}
		else
		{
			return 0;
		}
			
	}
	else
	{
		pinNo=pinNo-100;
		if((IOPIN1&(1<<pinNo)))
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
}
void portWrite(int portNo,unsigned int portval)
{
	if(portNo<10)
	{
		//port1
		if(portNo==0)
		{
			IODIR0=IODIR0 | (0xFF);
			IOSET0=IOSET0 | (portval);
			IOCLR0=IOCLR0 | ~(portval);
		}
		else if(portNo==1)//p0.8 to p0.15
		{
			IODIR0=IODIR0 | ((0xFF)<<8);
			IOSET0=IOSET0 | ((portval)<<8);
			IOCLR0=IOCLR0 | (~((portval)<<8));
		}
		else if(portNo==2)//p0.8 to p0.15
		{
			IODIR0=IODIR0 | ((0xFF)<<16);
			IOSET0=IOSET0 | ((portval)<<16);
			IOCLR0=IOCLR0 | (~((portval)<<16));
		}
		else if(portNo==3)//p0.8 to p0.15
		{
			IODIR0=IODIR0 | ((0xFF)<<24);
			IOSET0=IOSET0 | ((portval)<<24);
			IOCLR0=IOCLR0 | (~((portval)<<24));
		}
		else 
			{
			IODIR0=IODIR0 | ((0xFFFFFFFF));
			IOSET0=IOSET0 | ((portval));
			IOCLR0=IOCLR0 | (~((portval)));
		}
			
	}
	else
	{//port2
		//port1
		/*if(portNo==10)
		{
			IODIR0=IODIR0 | (0xFF);
			IOSET0=IOSET0 | (portval);
			IOCLR0=IOCLR0 | ~(portval);
		}
		else if(portNo==11)//p0.8 to p0.15
		{
			IODIR0=IODIR0 | ((0xFF)<<8);
			IOSET0=IOSET0 | ((portval)<<8);
			IOCLR0=IOCLR0 | (~((portval)<<8));
		}*/
		if(portNo==12)//p0.8 to p0.15
		{
			IODIR1=IODIR1 | ((0xFF)<<16);
			IOSET1=IOSET1 | ((portval)<<16);
			IOCLR1=IOCLR1 | (~((portval)<<16));
		}
		else if(portNo==13)//p0.8 to p0.15
		{
			IODIR1=IODIR1 | ((0xFF)<<24);
			IOSET1=IOSET1 | ((portval)<<24);
			IOCLR1=IOCLR1 | (~((portval)<<24));
		}
		else 
			{
			IODIR1=IODIR1 | ((0xFFFFFFFF));
			IOSET1=IOSET1 | ((portval));
			IOCLR1=IOCLR1 | (~((portval)));
		}
		
	}
}