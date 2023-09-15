#include <lpc214x.h>

#define I2C_SCL_PIN  0 // GPIO pin for SCL
#define I2C_SDA_PIN  1 // GPIO pin for SDA

#define I2C_WRITE    0
#define I2C_READ     1

void i2c_init(void)
{
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

void i2c_write_byte(uint8_t data)
{
    I2C0DAT = data;  // Write data to transmit register
    I2C0CONCLR = (1 << 3);  // Clear start bit
    while (!(I2C0CONSET & (1 << 3)));  // Wait for data transmission
}

uint8_t i2c_read_byte(void)
{
    I2C0CONSET = (1 << 2);  // Enable ACK
    I2C0CONCLR = (1 << 3);  // Clear start bit
    while (!(I2C0CONSET & (1 << 3)));  // Wait for data reception
    return I2C0DAT;  // Return received data
}

void i2c_write(uint8_t slave_address, uint8_t *data, uint8_t length)
{
    uint8_t i;

    i2c_start();
    i2c_write_byte((slave_address << 1) | I2C_WRITE);

    for (i = 0; i < length; i++)
    {
        i2c_write_byte(data[i]);
    }

    i2c_stop();
}

void i2c_read(uint8_t slave_address, uint8_t *data, uint8_t length)
{
    uint8_t i;

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

int main(void)
{
    uint8_t i2c_slave_address = 0x50;
    uint8_t write_data[4] = {0x00, 0x01, 0x02, 0x03};
    uint8_t read_data[4];

    i2c_init();
    
    // Example: Write data to I2C slave device
    i2c_write(i2c_slave_address, write_data, sizeof(write_data));

    // Example: Read data from I2C slave device
    i2c_read(i2c_slave_address, read_data, sizeof(read_data));

    // Print received data
    for (uint8_t i = 0; i < sizeof(read_data); i++)
    {
       // printf("Read Data[%d]: 0x%02X\n", i, read_data[i]);
    }

    return 0;
}

