/*
 * sd.c
 *
 *  Created on: 18 May 2020
 *      Author: lvisagie
 */


#include "main.h"//Allows us to use the HAL functions

extern SPI_HandleTypeDef hspi2;

uint8_t SD_ReceiveByte(void)
{
	uint8_t dummy, data;
	dummy = 0xFF;

	HAL_SPI_TransmitReceive(&hspi2, &dummy, &data, 1, 100);

	return data;
}

uint8_t SD_SendCommand(uint8_t cmd, uint32_t arg)//This is like a template for the bytes we send to the SD command
{
	// wait for SD to become ready
	//SPI is full duplex therefore we can send and receive at the same time
	uint32_t ticksstart = HAL_GetTick();
	uint8_t res = 0;
	do
	{
		res = SD_ReceiveByte();
	} while (((HAL_GetTick() - ticksstart) >= 500) && (res != 0xff));
	//either the SD_ReceiveByte return 255 (0xff) or if tick-counter increased by 500 ms since start of the loop
	//If he SD card does not return 255 (0xff) the send command will give up and exit

	uint8_t cmdbuffer[6];//6 bytes to send
	cmdbuffer[0] = 0x40 | cmd;// This is the command ID using bit-wise AND
	//Next we send the = arguments using 32 bytes using bit shifting
	cmdbuffer[1] = (uint8_t)(arg >> 24);
	cmdbuffer[2] = (uint8_t)(arg >> 16);
	cmdbuffer[3] = (uint8_t)(arg >> 8);
	cmdbuffer[4] = (uint8_t)arg;

	// crc-Cyclic redundancy code (Only need to CRC the following commands since they have fixed arguments)
	if(cmd == 0)
		cmdbuffer[5] = 0x95;	// CRC for CMD0 (Initialize Command)
	else if (cmd == 8)
		cmdbuffer[5] = 0x87;	// CRC for CMD8 (with 32-bit argument = 0x1AA)
	else
		cmdbuffer[5] = 1;

	// transmit command
	HAL_SPI_Transmit(&hspi2, cmdbuffer, 6, 100);

	// read response code
	uint8_t n = 10;
	//need to check multiple times for the result
	do
	{
		res = SD_ReceiveByte();
		n--;
	} while ((res & 0x80) && (n > 0));//loop till MSBit from response is 0
	//The result we obtain is also the return
	return res;
}

void Deselect()
{
	// set chipselect high
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 1);
	HAL_Delay(1);
}

void Select()
{
	// set chipselect low
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 0);
	HAL_Delay(1);
}

uint8_t SD_Init()
{
	Deselect();//set Chip Select high

	uint8_t dummy = 0xff;
	for (int i = 0; i < 10; i++)
	{
	  HAL_SPI_Transmit(&hspi2, &dummy, 1, 100);
	}

	Select();//set Chip Select low


	// send command 0
	if (SD_SendCommand(0, 0) != 1)//this is error checking for if we don't receive the correct response of 1
		return 0;

	// send command 8
	//Send it our voltage(0b0001->Convert to hex is 0X1) and we chose (0x00AA) for our unique check pattern
	//Therefore we send 0x1aa
	if (SD_SendCommand(8, 0x1aa) != 1)
		return 0;

	// read R7 response, after CMD8
	uint8_t cmd8return[4];
	for (int i = 0; i < 4; i++)
		cmd8return[i] = SD_ReceiveByte();

	if ((cmd8return[2] != 1) || (cmd8return[3] != 0xaa))//If SD card acknowledges our voltage we will get '1' and our unique 'aa' back
		return 0;

	uint32_t ticksstart = HAL_GetTick();
	do
	{
		if (SD_SendCommand(55, 0) <= 1)//CMD55 returns 0 or 1
		{
			if (SD_SendCommand(41, 1<< 30) == 0)//bit in position 30 set to 1, 'HCS' bit shows we support high capacity cards and MUST return 0
			{
				//ok!
				break;
			}
		}
	} while ((HAL_GetTick() - ticksstart) < 1000);//1 second timer as digram 7-2 indicates we must loop till we get a '0' returned

	//Can use CMD58 to check if standard or high capacity card used but we will skip this part (Use in proper implementation)


	Deselect();

	return 1;
}


uint8_t SD_RxDataBlock(uint8_t* buff)
{
	uint8_t token;

	uint32_t ticksstart = HAL_GetTick();

	// loop until receive a response or timeout
	do
	{
		token = SD_ReceiveByte();
	} while ((token == 0xFF) && ((HAL_GetTick() - ticksstart) < 200));//exit when we get our needed token or 200 milliseconds expires

	// check for invalid response
	if(token != 0xFE)
		return 0;

	// receive data
	for (int i = 0; i < 512; i++)
	{
		buff[i] = SD_ReceiveByte();
	}

	// read and discard CRC
	SD_ReceiveByte();
	SD_ReceiveByte();

	// return success
	return 1;

}

uint8_t SD_Read(uint8_t* rxbuffer, uint32_t address, uint32_t numblocks)//(buffer to store into,address of SD card to read from,number of blocks to read)
{
	//We use commands 17 and 18 to read
	uint32_t sector = address >> 9;//divide by 512 (2^9) using bit-shifting
	//If standard SD card used we don't shift because we do not need to divide.

	Select();

	if (numblocks == 1)//Use CMD 17 when accessing a single block
	{
		// read single block
		if (SD_SendCommand(17, sector) != 0)
			return 0;

		SD_RxDataBlock(rxbuffer);//read 512 byte block

		return 1;
	}
	else
	{
		// read multiple blocks using CMD 18
		if (SD_SendCommand(18, sector) != 0)
			return 0;

		for (int i = 0; i < numblocks; i++)
		{
			if (!SD_RxDataBlock(rxbuffer))
				break;

			rxbuffer += 512;
		}

		// stop receiving asper timing diagrams in specification
		SD_SendCommand(12, 0);
	}

	Deselect();

	return 1;
}

uint8_t SD_TxDataBlock(uint8_t *buff, uint8_t token)
{
	uint8_t resp = 0;
	uint8_t crc[2] = {0, 0};

	// transmit token
	HAL_SPI_Transmit(&hspi2, &token, 1, 100);

	// transmit data bytes
	HAL_SPI_Transmit(&hspi2, buff, 512, 100);

	// transmit dummy crc since CRC not enabled
	HAL_SPI_Transmit(&hspi2, crc, 2, 100);

	// wait for data response token
	for (int i = 0; i < 64; i++)
	{
		resp = SD_ReceiveByte();

		if ((resp & 0x1F) == 0x05)
			break;
	}

	// now wait for programming to finish. TO DO: add a timeout here...
	while (SD_ReceiveByte() == 0);

	// return success if data was accepted
	if ((resp & 0x1F) == 0x05)
		return 1;

	return 0;
}

uint8_t SD_Write(uint8_t* txbuffer, uint32_t address, uint32_t numblocks)
{
	Select();

	uint32_t sector = address >> 9;

	if (numblocks == 1)
	{
		if (SD_SendCommand(24, sector) == 0)
		{
			SD_TxDataBlock(txbuffer, 0xFE);//FE for one block
		}
	}
	else
	{
		if (SD_SendCommand(25, sector) == 0)
		{
			for (int i = 0; i < numblocks; i++)
			{
				if (!SD_TxDataBlock(txbuffer, 0xFC))//FE for more than 1 block
					break;

				txbuffer += 512;
			}

			uint8_t token = 0xFD;
			HAL_SPI_Transmit(&hspi2, &token, 1, 100);
		}
	}

	Deselect();

	return 1;
}
