#include <lpc214x.h>
#include <stdio.h>
#define I2C_SCL_PIN  0 // GPIO pin for SCL
#define I2C_SDA_PIN  1 // GPIO pin for SDA

#define I2C_WRITE    0
#define I2C_READ     1
void init_PLL()
{
	PLL0CON=0x01;//Enable clock
	PLL0CFG=0x24;//set up pll for 60Mhz
	PLL0FEED=0xAA;
	PLL0FEED=0x55;
	while((PLL0STAT & 0x400)==0);
	
	PLL0CON=0x03;
	PLL0FEED=0xAA;
	PLL0FEED=0x55;
	VPBDIV=0x01;
}


void i2c_init(void)
{
		PINSEL0|=0x50;
    I2C0CONSET = (1 << 6);  // Enable I2C module
    I2C0SCLH = 75;          // Set SCL clock frequency (change according to your requirement)
    I2C0SCLL = 75;          // Set SCL clock frequency (change according to your requirement)
}

void i2c_start(void)
{
    I2C0CONSET = (1 << 5);  // Start condition
    while (!(I2C0CONSET & (1 << 3)));  // Wait for start condition to be sent
    I2C0CONCLR = (1 << 3);  // Clear start bit
}

void i2c_stop(void)
{
    I2C0CONSET = (1 << 4);  // Stop condition
    I2C0CONCLR = (1 << 3);  // Clear start bit
}

void i2c_write_byte(int data)
{
    I2C0DAT = data;  // Write data to transmit register
    I2C0CONCLR = (1 << 3);  // Clear start bit
    while (!(I2C0CONSET & (1 << 3)));  // Wait for data transmission
}

int i2c_read_byte(void)
{
    I2C0CONSET = (1 << 2);  // Enable ACK
    I2C0CONCLR = (1 << 3);  // Clear start bit
    while (!(I2C0CONSET & (1 << 3)));  // Wait for data reception
    return I2C0DAT;  // Return received data
}

void i2c_write(int slave_address, int *data, int length)
{
    int i;

    i2c_start();
    i2c_write_byte((slave_address << 1) | I2C_WRITE);

    for (i = 0; i < length; i++)
    {
        i2c_write_byte(data[i]);
    }

    i2c_stop();
}

void i2c_read(int slave_address, int *data, int length)
{
    int i;

    i2c_start();
    i2c_write_byte((slave_address << 1) | I2C_READ);

    for (i = 0; i < length - 1; i++)
    {
        data[i] = i2c_read_byte();
    }
    
    I2C0CONCLR = (1 << 2);  // Disable ACK for the last byte
    data[length - 1] = i2c_read_byte();

    i2c_stop();
}
int i;
int main(void)
{
    int i2c_slave_address = 0x50;
    int write_data[4] = {0x00, 0x01, 0x02, 0x03};
    int read_data[4];
		init_PLL();
    i2c_init();
    
    // Example: Write data to I2C slave device
    i2c_write(i2c_slave_address, write_data, sizeof(write_data));

    // Example: Read data from I2C slave device
    i2c_read(i2c_slave_address, read_data, sizeof(read_data));

    // Print received data
    for (i = 0; i < sizeof(read_data); i++)
    {
       // printf("Read Data[%d]: 0x%02X\n", i, read_data[i]);
    }

    return 0;
}

