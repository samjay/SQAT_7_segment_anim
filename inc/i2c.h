/*
 * i2c.h
 *
 *  Created on: Jun 6, 2016
 *      Author: timppa
 */

#ifndef I2C_H_
#define I2C_H_

//#define I2C_CFG_MSTEN            (0x1)
//#define I2C_CFG_SLVEN            (0x2)
//#define I2C_STAT_MSTPENDING      (0x1)
//#define I2C_STAT_MSTSTATE        (0xe)
#define I2C_STAT_MSTST_IDLE      (0x0)
#define I2C_STAT_MSTST_RX        (0x2)
#define I2C_STAT_MSTST_TX        (0x4)
//#define I2C_STAT_MSTST_NACK_ADDR (0x6)
//#define I2C_STAT_MSTST_NACK_TX   (0x8)
//#define I2C_STAT_SLVPENDING      (0x100)
//#define I2C_STAT_SLVSTATE        (0x600)
//#define I2C_STAT_SLVST_ADDR      (0x000)
//#define I2C_STAT_SLVST_RX        (0x200)
//#define I2C_STAT_SLVST_TX        (0x400)
#define I2C_MSTCTL_CONTINUE      (0x1)
//#define I2C_MSTCTL_MSTSTART      (0x2)
//#define I2C_MSTCTL_MSTSTOP       (0x4)
//#define I2C_SLVCTL_SLVCONTINUE   (0x1)
//#define I2C_SLVCTL_SLVNACK       (0x2)
#define I2C_ENABLE_SLAVE_ADDRESS	~(0x1)

typedef enum {
	I2C_ERR_READ_IDLE = -1,
	I2C_ERR_READ_RX   = -2,
	I2C_ERR_READ_STOP = -3
} i2c_read_err_e;

typedef enum {
	I2C_ERR_WRITE_IDLE = -4,
	I2C_ERR_WRITE_TX   = -5,
	I2C_ERR_WRITE_STOP = -6
} i2c_write_err_e;

typedef struct i2c_data_s {
	int address;
	char *reg;
	int reglen;
	char* buffer;
	int length;
} i2c_data_t;

#ifdef __cplusplus
extern "C" {
#endif

void I2C_init();
int I2C_write(int address,const char* buffer,int length);
int I2C_read(int address, char* reg,int reglen, char* buffer, int length);

#ifdef __cplusplus
}
#endif

#endif /* I2C_H_ */
