#include<LPC214x.h>
#include"GPIO.h"

int main()
{
	int x;
	//IODIR0=IODIR0|(1<<21);
	pinMode(21,output);
	pinMode(22,input);
	while(1)
	{
		//IOSET0=IOSET0|(1<<21);
		x=digitalRead(22);
		if(x==1)
		{
		digitalWrite(21,HIGH);
		delay(2000);
		
		}
		else
		{
			digitalWrite(21,LOW);
		//IOCLR0=IOCLR0|(1<<21);
		delay(2000);
		}
	}
}
