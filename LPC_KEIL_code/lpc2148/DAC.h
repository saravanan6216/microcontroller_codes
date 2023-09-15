void DAC(unsigned int d)
{
	PINSEL1|=0xC0000;
	DACR=d<<6;
}