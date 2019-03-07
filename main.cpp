#include <stdio.h>
#include "fingerPrint.h"
#include "serial.h"

// Create serial port
serial serial;
//fingerPrint Finger;

int main(int argc, char** argv)
{
	uint8_t buffer[256] = {0};
	int i, length = 0;
	
	// Open serial port ("COM3", "/dev/ttyUSB0")
	int opened = serial.Open("/dev/ttyUSB0", 57600, 8, NO, 1);

	//EF01 FFFFFFFF 01 00 07 13 00 00 00 00 00 1B
	uint8_t serialBuffer[] = {0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x07, 0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1B };

	// Send data
	serial.Write(serialBuffer, sizeof(serialBuffer));

	//Finger.verifyPassword();

	while(1)
	{
		// Wait character
		length = serial.Read(buffer);
		
		if(length)
		{
			for(i = 0; i < length; i++)
			{
				printf("%X ", buffer[i]);
				//printf("%c", buffer[i]);
			}
			
			printf("\n");
		}
	}
	
	//printf("%d\n" , n);
	// Close serial port
	//serial.Close();
	
	return 0;
}