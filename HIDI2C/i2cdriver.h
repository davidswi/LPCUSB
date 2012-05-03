#ifndef I2CDRIVER_H
#define I2CDRIVER_H

#define I2C_ERR_NONE		0x00
#define I2_ERR_BUS			0x01
#define I2C_ERR_BUSY		0x02
#define I2C_ERR_TIMEOUT		0x03
#define I2C_ERR_BADADDR		0x04
#define I2C_ERR_WRITEFAIL	0x05
#define I2C_ERR_READFAIL	0x06

struct i2c_transaction
{
	U8	addr;
	U8	*pOutData;
	U16 uWriteCount;
	U16 uWritten;
	U8	*pInData;
	U16 uReadCount;
	U16 uRead;
};

int i2c_driver_init(float fPCLKMHz, float fI2CKHz);
int i2c_driver_write(U8 addr, U8 *pData, U16 uDataCount, U16 *puDataWritten, U32 uWaitTicks);
int i2c_driver_read(U8 addr, U8 *pData, U16 uDataReq, U16 *puDataRead, U32 uWaitTicks);
int i2c_driver_transaction(U8 addr, struct i2c_transaction *pTrans, U32 uWaitTicks);

#endif // I2C_DRIVER_H
