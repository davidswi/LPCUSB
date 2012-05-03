#include "errno.h"
#include "type.h"
#include "i2cdriver.h"
#include "lpc214x.h"
#include "usbdebug.h"

#include "../examples/Debug_LEDs.h"
#include "../examples/console.h"

enum i2c_state
{
	UNINITIALIZED,
	IDLE,
	START,
	WRITING,
	READING,
	STOP,
	ERROR
};

struct i2c_driver
{
	U32						uFreqHz;
	U32						uTicksElapsed;
	enum i2c_state			state;
	struct i2c_transaction	curr_trans;
	int						last_error;
};

// global driver instance
struct i2c_driver driver;

// forward declaration of I2C0 interrupt handler
static void I2C0IntHandler(void) __attribute__ ((interrupt("IRQ")));

// Helper function declarations
static int i2c_driver_start_IO(void);

static void I2C0IntHandler(void)
{
	// Clear the interrupt
	I2C0_CONCLR = I2C_CONSET_SI;

	// Mark the interrupt as handled
	VIC_VectAddr = 0;
}

static int i2c_driver_start_IO(void)
{
	return I2C_ERR_NONE;
}

// public driver functions

int i2c_driver_init(float fPCLKMHz, float fI2CKHz)
{
	U32 uI2CHz,
		uPCLK_Hz;

	U16	uSCLTot;

	U8	uIRQ;

	// Validate PCLK and I2C bus frequencies
	if (fPCLKMHz < 1.00f || fPCLKMHz > 60.0f)
		return EINVAL;

	if (fI2CKHz < 1.25f || fI2CKHz > 400.0f)
		return EINVAL;

	// Convert I2C frequency to integer Hz
	uI2CHz = (U32)(fI2CKHz * 1000.0f);

	// Convert PCLK to Hz
	uPCLK_Hz = (U32)(fPCLKMHz * 1000000.0f);

	// Calculate SCL sum for desired I2C frequency
	uSCLTot = (U16)(uPCLK_Hz / uI2CHz);

	// And divide in half for SCL registers
	uSCLTot >>= 1;

	DBG("i2c_driver_init: Setting SCL registers to %d\n", uSCLTot);

	if (uSCLTot < 4)
		return EINVAL;
	
	// Set up P0.2 as SCL and P0.3 as SDA
	PCB_PINSEL0 &= ~(PCB_PINSEL0_P02_MASK | PCB_PINSEL0_P03_MASK);
	PCB_PINSEL0 |= (PCB_PINSEL0_P02_SCL0 | PCB_PINSEL0_P03_SDA0);

	// Enable power to I2C0 controller
	SCB_PCONP |= SCB_PCONP_PCI2C0;

	// Set SCLH and SCLL to calculated value
	I2C0_SCLH = uSCLTot;
	I2C0_SCLL = uSCLTot;

	// Get free IRQ for I2C0
	if (GetNextFreeIRQ(&uIRQ) != I2C_ERR_NONE)
		return ENOMEM;

	// Set up IRQ for I2C0
	VIC_IntSelect &= ~VIC_IntSelect_I2C0;
	*((&VIC_VectCntl0) + uIRQ) |= VIC_VectCntl_ENABLE | VIC_Channel_I2C0;
	*((&VIC_VectAddr0) + uIRQ) = (int)I2C0IntHandler;
	// Enable I2C0 interrupts
	VIC_IntEnable |= VIC_IntEnable_I2C0;

	return I2C_ERR_NONE;
}

int i2c_driver_write(U8 addr, U8 *pData, U16 uDataCount, U16 *puDataWritten, U32 uWaitTicks)
{
	DBG("i2c_driver_write: Writing %d bytes to addr %X\n", uDataCount, addr);

	return I2C_ERR_NONE;
}

int i2c_driver_read(U8 addr, U8 *pData, U16 uDataReq, U16 *puDataRead, U32 uWaitTicks)
{
	DBG("i2c_driver_read: Reading %d bytes from addr %X\n", uDataReq, addr);

	return I2C_ERR_NONE;
}

int i2c_driver_transaction(U8 addr, struct i2c_transaction *pTrans, U32 uWaitTicks)
{
	DBG("i2c_driver_transaction: addr=%X, write=%d, read=%d\n", addr, pTrans->uWriteCount, pTrans->uReadCount);

	return I2C_ERR_NONE;
}

// end of file
