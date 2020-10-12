/*
 =========================================================================================
 Name        : PMOD_HYGRO.c
 Author      : JSC
 Website	 : https://www.jscblog.com/post/bbb-hygro-temperature-and-humidity-pmod
 Description : This project is to communicate with the PMOD HYGRO Temperature and Humidity
 	 	 	   sensor using the BeagleBone Black.
 =========================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>

#define HYGRO_SLAVE_ADDR	0x40			// Slave Address
#define I2C_FILE_PATH		"/dev/i2c-2"	// Files path for the i2c-2 directory

int file;

// This function initialises the sensor
int hygro_init()
{
	// Two element buffer to hold address and data
	int writeBuffer[2];

	// Open file
	if ((file=open(I2C_FILE_PATH, O_RDWR)) < 0)
	{
		perror("Failed to open the bus\n");
		return -1;
	}

	// Connect to slave
	if ((ioctl(file, I2C_SLAVE, HYGRO_SLAVE_ADDR)) < 0)
	{
		perror("Failed to connect to the sensor\n");
		return -1;
	}

	// Configure the configuration register and set mode to
	// temperature or humidity acquired and both in 14-bit mode.
	writeBuffer[0] = 0x02;
	writeBuffer[1] = 0x00;
	if (write(file, writeBuffer, 2) != 2)
	{
		perror("Failed to write to the register\n");
		return -1;
	}

	close(file);

	return 0;
}

// This function reads the temperature and shows in the terminal
int get_temperature()
{
	// Open file
	if ((file=open(I2C_FILE_PATH, O_RDWR)) < 0)
	{
		perror("Failed to open the bus\n");
		return -1;
	}

	// Connect to slave
	if ((ioctl(file, I2C_SLAVE, HYGRO_SLAVE_ADDR)) < 0)
	{
		perror("Failed to connect to the sensor\n");
		return -1;
	}

	// Set the pointer register to the temperature address
	short rw_buffer[1] = {0x00};
	if (write(file, rw_buffer, 1) != 1)
	{
		perror("Failed to write to the register\n");
		return -1;
	}

	// Wait for measurement to complete
	usleep(7000);

	// Read from the temperature address
	if (read(file, rw_buffer, 1) != 1)
	{
		perror("Failed to read from the buffer\n");
		return -1;
	}

	close(file);

	// Shift the temperature data two bits to the write [15:2]
	int temperature = (rw_buffer[0] >> 2);

	printf("The temperature is %d\n", temperature);
	return 0;
}

// This function reads the humidity and shows in the terminal
int get_humidity()
{
	// Open file
	if ((file=open(I2C_FILE_PATH, O_RDWR)) < 0)
	{
		perror("Failed to open the bus\n");
		return -1;
	}

	// Connect to the slave
	if ((ioctl(file, I2C_SLAVE, HYGRO_SLAVE_ADDR)) < 0)
	{
		perror("Failed to connect to the sensor\n");
		return -1;
	}

	// Set the pointer register to the humidity address
	short rw_buffer[1] = {0x01};
	if (write(file, rw_buffer, 1) != 1)
	{
		perror("Failed to write to the register\n");
		return -1;
	}

	// Wait for measurement to complete
	usleep(7000);

	// Read from the humidity address
	if (read(file, rw_buffer, 1) != 1)
	{
		perror("Failed to read to the buffer\n");
		return -1;
	}

	close(file);

	// Shift the humidity data two bits to the write [15:2]
	int humidity = (rw_buffer[0] >> 2);

	printf("The humidity is %d\n", humidity);

	return 0;
}

int main(void)
{

	hygro_init();
	usleep(1000);

	get_temperature();
	get_humidity();


}
