// @File		MX25L4005.H		 		
// @Author		JOSIMAR PEREIRA LEITE
// @country		Brazil
// @Date		26/03/24
//
//
// Copyright (C) 2024  JOSIMAR PEREIRA LEITE
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
#ifndef MX25L4005_H
#define MX25L4005_H

#include <xc.h>

#define MX25L4005_SCK PORTCbits.RC7
#define MX25L4005_SDI PORTCbits.RC6
#define MX25L4005_SDO PORTCbits.RC5
#define MX25L4005_CS PORTCbits.RC4

#define MX25L4005_SCK_TRIS TRISCbits.RC7
#define MX25L4005_SDI_TRIS TRISCbits.RC6
#define MX25L4005_SDO_TRIS TRISCbits.RC5
#define MX25L4005_CS_TRIS TRISCbits.RC4
//----------------------------------------------
//
//
//
#define MX25L4005_COMMAND_WRITE_ENABLE 0x06
#define MX25L4005_COMMAND_WRITE_DISABLE 0x04
#define MX25L4005_COMMAND_READ_INDENTIFICATION 0x9F 
#define MX25L4005_COMMAND_WRITE_STATUS 0x01
#define MX25L4005_COMMAND_READ_STATUS 0x05
#define MX25L4005_COMMAND_READ_DATA 0x03
#define MX25L4005_COMMAND_PAGE_PROGRAM 0x02
#define MX25L4005_COMMAND_CHIP_ERASE 0x60
#define MX25L4005_COMMAND_BLOCK_ERASE 0x52
#define MX25L4005_COMMAND_SETOR_ERASE 0x20
#define MX25L4005_COMMAND_DEEP_POWER_DOWN 0xB9
#define MX25L4005_COMMAND_RELEASE_FROM_DEEP_POWER_DOWN 0xAB
#define MX25L4005_COMMAND_READ_ELETRONIC_ID 0xAB
#define MX25L4005_COMMAND_READ_ELETRONIC_MANUFACTURER 0x90

unsigned char MX25L4005_READ_BYTE(unsigned long address);
unsigned char MX25L4005_RDSR(void);
unsigned char SPI_read(void);
void SPI_write(unsigned char data);
void MX25L4005_WREN(void);
void MX25L4005_WRDI(void);
void MX25L4005_CHIP_ERASE(void);
void MX25L4005_SECTOR_ERASE(unsigned long address);
void MX25L4005_BLOCK_ERASE(unsigned long address);
void MX25L4005_WRSR(unsigned char data);
void MX25L4005_RDID(unsigned char *manufacturer, unsigned char *memory_type, unsigned char *memory_density);
void MX25L4005_REMS(unsigned char *manufacturer, unsigned char *device_id);
void MX25L4005_READ_BUF(unsigned long address, unsigned long size, unsigned char buf[]);
void MX25L4005_PROGRAM_BYTE(unsigned long address, unsigned char data);
void MX25L4005_PROGRAM_BUF(unsigned long address, unsigned long size, unsigned char buf[]);
void MX25L4005_Init(void);
#endif 

