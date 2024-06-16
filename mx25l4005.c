// @File		MX25L4005.C		 		
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
#include "mx25l4005.h"

void SPI_write(unsigned char data)
{
    for(unsigned char mask = 0x80; mask; mask >>= 1)
    {            
        if(data & mask) MX25L4005_SDI = 1; else MX25L4005_SDI = 0;  
        
        __asm__ __volatile__("nop"); 
        MX25L4005_SCK = 1;
        __asm__ __volatile__("nop");        
        MX25L4005_SCK = 0;    
        __asm__ __volatile__("nop");
    }    
}
//----------------------------------------------
//
//
//
unsigned char SPI_read(void)
{
    unsigned char data = 0;
    
    for(unsigned char mask = 0x80; mask; mask >>=1)
    {
        data <<= 1;
        if(MX25L4005_SDO) data |= 1;
        
        __asm__ __volatile__("nop");
        MX25L4005_SCK = 1;
        __asm__ __volatile__("nop");
        MX25L4005_SCK = 0;        
        __asm__ __volatile__("nop");
    }
    
    return ((unsigned char) data);
}
//----------------------------------------------
//
//
//
void MX25L4005_WREN(void)
{
    MX25L4005_CS = 0;    
    SPI_write(MX25L4005_COMMAND_WRITE_ENABLE);
    MX25L4005_CS = 1;
}
//----------------------------------------------
//
//
//
void MX25L4005_WRDI(void)
{
    MX25L4005_CS = 0;    
    SPI_write(MX25L4005_COMMAND_WRITE_DISABLE);
    MX25L4005_CS = 1;
}
//----------------------------------------------
//
//
//
unsigned char MX25L4005_RDSR(void)
{                     
    unsigned char data = 0;
    
    MX25L4005_CS = 0;    
    SPI_write(MX25L4005_COMMAND_READ_STATUS);
    data = SPI_read();
    MX25L4005_CS = 1;        
    return ((unsigned char) data);
}
//----------------------------------------------
//
//
//
void MX25L4005_CHIP_ERASE(void)
{
    MX25L4005_WREN();
    
    MX25L4005_CS = 0;
    SPI_write(MX25L4005_COMMAND_CHIP_ERASE);
    MX25L4005_CS = 1;
    
    MX25L4005_WRDI();
    
    // CHECK BUSY
    unsigned char status = 1;
    while((status & 0x01) == 1) status = MX25L4005_RDSR();
}
//----------------------------------------------
//
//
//
void MX25L4005_SECTOR_ERASE(unsigned long address)
{
    MX25L4005_WREN();
    
    MX25L4005_CS = 0;
    SPI_write(MX25L4005_COMMAND_SETOR_ERASE); 
    SPI_write((unsigned char)((address >> 16) & 0xFF));
    SPI_write((unsigned char)((address >> 8) & 0xFF));
    SPI_write((unsigned char)(address & 0xFF));
    MX25L4005_CS = 1;
    
    MX25L4005_WRDI();
    
    // CHECK BUSY
    unsigned char status = 1;
    while((status & 0x01) == 1) status = MX25L4005_RDSR();
}
//----------------------------------------------
//
//
//
void MX25L4005_BLOCK_ERASE(unsigned long address)
{
    MX25L4005_WREN();
    
    MX25L4005_CS = 0;
    SPI_write(MX25L4005_COMMAND_BLOCK_ERASE); 
    SPI_write((unsigned char)((address >> 16) & 0xFF));
    SPI_write((unsigned char)((address >> 8) & 0xFF));
    SPI_write((unsigned char)(address & 0xFF));
    MX25L4005_CS = 1;
    
    MX25L4005_WRDI();
    
    // CHECK BUSY
    unsigned char status = 1;
    while((status & 0x01) == 1) status = MX25L4005_RDSR();
}
//----------------------------------------------
//
//
//
void MX25L4005_WRSR(unsigned char data)
{
    MX25L4005_WREN();
    
    MX25L4005_CS = 0;
    SPI_write(MX25L4005_COMMAND_WRITE_STATUS);
    SPI_write((unsigned char)data);
    MX25L4005_CS = 1;
    
    MX25L4005_WRDI();
    
    // CHECK BUSY
    unsigned char status = 1;
    while((status & 0x01) == 1) status = MX25L4005_RDSR();
}
//----------------------------------------------
//
//
//
// DEVICE ID 2 BYTE; MANUFACTURER: C2H, MEMORY TYPE: 20H
// DEVICE INDIVIDUAL  13H for IDMX25L4005
void MX25L4005_RDID(unsigned char *manufacturer,
unsigned char *memory_type, unsigned char *memory_density)
{                 
    MX25L4005_CS = 0;    
    SPI_write(MX25L4005_COMMAND_READ_INDENTIFICATION);
    *manufacturer = SPI_read();
    *memory_type = SPI_read();
    *memory_density = SPI_read();
    MX25L4005_CS = 1;
}
//----------------------------------------------
//
//
//
//
void MX25L4005_REMS(unsigned char *manufacturer, unsigned char *device_id)
{                 
    MX25L4005_CS = 0;    
    SPI_write(MX25L4005_COMMAND_READ_ELETRONIC_MANUFACTURER);
    SPI_read(); // 1 DUMMY BYTES
    SPI_read(); // 1 DUMMY BYTES
    // ADD: ADD=00H will output the manufacturer's 
    // ID first and ADD=01H will output device ID first
    SPI_read(); // ADD
    *manufacturer = SPI_read();
    *device_id = SPI_read();
    MX25L4005_CS = 1;
}
//----------------------------------------------
//
//
//
unsigned char MX25L4005_READ_BYTE(unsigned long address)
{         
    unsigned char byte = 0;
    
    MX25L4005_CS = 0;    
    SPI_write(MX25L4005_COMMAND_READ_DATA);
    SPI_write((unsigned char)((address >> 16) & 0xFF));
    SPI_write((unsigned char)((address >> 8) & 0xFF));
    SPI_write((unsigned char)(address & 0xFF));
    byte = SPI_read();
    MX25L4005_CS = 1;
    
    // CHECK BUSY
    unsigned char status = 1;
    while((status & 0x01) == 1) status = MX25L4005_RDSR();
    
    return ((unsigned char) byte);        
}
//----------------------------------------------
//
//
//
void MX25L4005_READ_BUF(unsigned long address, unsigned long size, unsigned char buf[])
{                     
    MX25L4005_CS = 0;    
    SPI_write(MX25L4005_COMMAND_READ_DATA);    
    
    SPI_write((unsigned char)((address >> 16) & 0xFF));
	SPI_write((unsigned char)((address >> 8) & 0xFF));
	SPI_write((unsigned char)(address & 0xFF));
	
	for(unsigned long i = 0; i < size; i++)
    {            
        buf[i] = SPI_read();
    }    
    
    MX25L4005_CS = 1;
    
    // CHECK BUSY
    unsigned char status = 1;
    while((status & 0x01) == 1) status = MX25L4005_RDSR();
}
//----------------------------------------------
//
//
//
void MX25L4005_PROGRAM_BYTE(unsigned long address, unsigned char data)
{
    MX25L4005_WREN();
    
    
    MX25L4005_CS = 0;        
    SPI_write(MX25L4005_COMMAND_PAGE_PROGRAM);
    SPI_write((unsigned char)((address >> 16) & 0xFF));
    SPI_write((unsigned char)((address >> 8) & 0xFF));
    SPI_write((unsigned char)(address & 0xFF));   
    SPI_write(data);
    MX25L4005_CS = 1; 
        
    MX25L4005_WRDI();
    
    // CHECK BUSY
    unsigned char status = 1;
    while((status & 0x01) == 1) status = MX25L4005_RDSR();
}
//----------------------------------------------
//
//
//
void MX25L4005_PROGRAM_BUF(unsigned long address, unsigned long size, unsigned char buf[])
{
    MX25L4005_WREN();
    
    
    MX25L4005_CS = 0;     
    SPI_write(MX25L4005_COMMAND_PAGE_PROGRAM);    
    
    SPI_write((unsigned char)((address >> 16) & 0xFF));
	SPI_write((unsigned char)((address >> 8) & 0xFF));
	SPI_write((unsigned char)(address & 0xFF));
    
	for(unsigned long i = 0; i < size; i++)
    {                
        SPI_write(buf[i]);        
    }    
    MX25L4005_CS = 1; 
    
    MX25L4005_WRDI();
    
    // CHECK BUSY
    unsigned char status = 1;
    while((status & 0x01) == 1) status = MX25L4005_RDSR();    
}

//----------------------------------------------
//
//
//
void MX25L4005_Init(void)
{
    MX25L4005_SCK_TRIS = 0;
    MX25L4005_SDI_TRIS = 0;
    MX25L4005_SDO_TRIS = 1;
    MX25L4005_CS_TRIS = 0;
    
    MX25L4005_CS = 1;
}
