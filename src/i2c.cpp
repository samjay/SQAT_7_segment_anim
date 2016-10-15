/*
 * i2c.c
 *
 *  Created on: Jun 6, 2016
 *      Author: timppa
 */

#include "chip.h"
#include "i2c.h"

void I2C_init()
{
	LPC_SYSCON -> SYSAHBCLKCTRL |=  (1 << 5);   // Enable clock for I2C (p. 36)
	LPC_SYSCON -> PRESETCTRL &= ~(1 << 6);		// Assert I2C reset (p. 30)
	LPC_SYSCON -> PRESETCTRL |= (1 << 6);		// Clear I2C reset (p. 30)
	LPC_I2C -> CLKDIV = 15;						// I2C clock divider (p. 222)
	LPC_I2C->CFG = I2C_CFG_MSTEN;				// Enable master (p. 213)
}

static int i2c_master_pending()
{
	// LPC_I2C->STAT has bit 0 with value 0 when an operation is in progress
	// According to I2C terminology, "Pending" means there is nothing to do with I2C at this point
	return LPC_I2C->STAT & I2C_STAT_MSTPENDING; // Check if the I2C master has an operation in progress (p. 215)
}

// Protocol for reading/writing available in p. 208
// data -> reg contains the information to be sent to the slave device (e.g., register number to read)
static int i2c_read_master( i2c_data_t* data )
{
	// Enable master
	LPC_I2C->CFG = I2C_CFG_MSTEN;
	// Wait until master comes to pending state
	// Notice that in this first stage, we need the I2C to have finished all previous R/W operations.
	while( !i2c_master_pending() );
	// I2C_STAT_MSTST_IDLE = 0x00
	// I2C_STAT_MSTSTATE = 0x07 << 1. It is used to mask all bits except the relevant ones (MSTSTATE, bits 3:1)
	// The master can be in various situation. 0x00 is "idle"
	if ( (LPC_I2C->STAT & I2C_STAT_MSTSTATE) != I2C_STAT_MSTST_IDLE ){ // Check that the master is in idle state (p. 215)
		return I2C_ERR_READ_IDLE;
	}

	// Checks that reglen > 0 and reg is a valid pointer
	// It means that before the read operation, we need to send some value to the slave I2C
	if ( data->reglen && data->reg ){
		// LPC_I2C->MSTDAT --> Data register
		// Address is a 7-bit number, located in 7:1
		// Bit 0 should be set to 0 to enable writing
		LPC_I2C->MSTDAT = (data->address << 1) | 0; // Write device address to master data register (p. 225)
		// LPC_I2C->MSTCTL --> Control register
		// Bit 1 should be set to 1 to start transmission
		LPC_I2C->MSTCTL = I2C_MSTCTL_MSTSTART;		// Start transmission (p. 224)
		// Wait until pending
		while( !i2c_master_pending() );
		// I2C_STAT_MSTST_TX = 0x04 = 0x02 << 1
		// I2C_STAT_MSTSTATE = 0x07 << 1
		// The master can be in various situation. 0x02 is "transmit ready"
		if ( (LPC_I2C->STAT & I2C_STAT_MSTSTATE) != I2C_STAT_MSTST_TX ){ // Check that the master is in transmit ready (p. 215)
			return I2C_ERR_WRITE_TX;
		}

		// This is device-specific.
		// In the case of the MPL3115A2, reg[0] contains the register number to be read
		LPC_I2C->MSTDAT = data->reg[0];
		//I2C_MSTCTL_CONTINUE = 0x01
		// Bit 0 should be set to 1 to repeat the write operation
		LPC_I2C->MSTCTL = I2C_MSTCTL_CONTINUE;		// Continue transmission (p. 223)
		// wait until pending
		while(!i2c_master_pending() );
	}

	// Bit 0 should be set to 0 to enable writing
	LPC_I2C->MSTDAT = (data->address << 1) | 1;		// Write device address to master data register (p. 225)
	LPC_I2C->MSTCTL = I2C_MSTCTL_MSTSTART;			// Start transmission (p. 224)

	// wait until pending
	while( !i2c_master_pending() );

	// I2C_STAT_MSTST_RX = 0x02 = 0x01 << 1
	// I2C_STAT_MSTSTATE = 0x07 << 1
	// The master can be in various situation. 0x01 is "receive ready"
	if ( (LPC_I2C->STAT & I2C_STAT_MSTSTATE) != I2C_STAT_MSTST_RX ){ // Check that the master is in receive ready (p. 215)
		return I2C_ERR_READ_RX;
	}

	int count = 0;
	while ( count < data->length ){
		data->buffer[count] = LPC_I2C->MSTDAT;
		count++;
		if ( count < data->length ){
			LPC_I2C->MSTCTL = I2C_MSTCTL_CONTINUE;
			// wait until pending
			while( !i2c_master_pending() );
			if ( (LPC_I2C->STAT & I2C_STAT_MSTSTATE) != I2C_STAT_MSTST_RX ){
				return I2C_ERR_READ_RX;
			}
		} else {
			LPC_I2C->MSTCTL = I2C_MSTCTL_MSTSTOP;
			// wait until pending
			while( !i2c_master_pending() );
			if ( (LPC_I2C->STAT & I2C_STAT_MSTSTATE) != I2C_STAT_MSTST_IDLE ){
				return I2C_ERR_READ_STOP;
			}
		}
	}
	//Number of bytes read
	return count;
}

static int i2c_write_master(i2c_data_t* data)
{
	LPC_I2C->CFG = I2C_CFG_MSTEN;
	// wait until pending
	while( !i2c_master_pending() );
	if ( (LPC_I2C->STAT & I2C_STAT_MSTSTATE) != I2C_STAT_MSTST_IDLE ){
		return I2C_ERR_WRITE_IDLE;
	}

	LPC_I2C->MSTDAT = (data->address << 1) | 0;
	LPC_I2C->MSTCTL = I2C_MSTCTL_MSTSTART;
	// wait until pending
	while( !i2c_master_pending() );
	if ( (LPC_I2C->STAT & I2C_STAT_MSTSTATE) != I2C_STAT_MSTST_TX ){
		return I2C_ERR_WRITE_TX;
	}
	int count=0;
	while ( count<data->length ){
		LPC_I2C->MSTDAT = data->buffer[count];
		count++;
		LPC_I2C->MSTCTL = I2C_MSTCTL_CONTINUE;
		// wait until pending
		while( !i2c_master_pending() );
		if ( (LPC_I2C->STAT & I2C_STAT_MSTSTATE) != I2C_STAT_MSTST_TX ){
			return I2C_ERR_WRITE_TX;
		}
	}
	LPC_I2C->MSTCTL = I2C_MSTCTL_MSTSTOP;
	// wait until pending
	while( !i2c_master_pending() );
	if ( (LPC_I2C->STAT & I2C_STAT_MSTSTATE) != I2C_STAT_MSTST_IDLE ){
		return I2C_ERR_WRITE_STOP;
	}
	return 1;
}

int I2C_write(int address,const char* buffer,int length)
{
	int rc;
	i2c_data_t i2c_data;
	i2c_data.address = address;
	i2c_data.reg = 0;
	i2c_data.reglen = 0;
	i2c_data.buffer = (char*)buffer;
	i2c_data.length = length;

	rc = i2c_write_master( &i2c_data );
	return rc;

}

int I2C_read(int address, char* reg, int reglen, char* buffer,int length)
{
	int rc;
	i2c_data_t i2c_data;
	i2c_data.address = address;
	i2c_data.reg = (char*)reg;
	i2c_data.reglen = reglen;
	i2c_data.buffer = (char*)buffer;
	i2c_data.length = length;

	rc = i2c_read_master( &i2c_data );
	return rc;
}

