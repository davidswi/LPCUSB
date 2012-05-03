#include "errno.h"
#include "type.h"
#include "hidi2c.h"
#include "lpc214x.h"
#include "usbapi.h"
#include "usbdebug.h"
#include "hidi2c_descriptors.h"
#include "i2cdriver.h"

#include "../examples/startup.h"
#include "../examples/console.h"
#include "../examples/armVIC.h"
#include "../examples/Debug_LEDS.h"

// PCLK and default I2C bus frequencies
#define PCLK_FREQ_MHZ		60.0f
#define I2C_FREQ_KHZ		100.0f

// Debug UART
#define BAUD_RATE			115200
// The USB controller interrupt handler gets IRQ0
#define USB_VECT_NUM		0
// The tick timer gets IRQ1
#define TIMER_VECT_NUM		1
// Last available vector
#define LAST_VECT_NUM		15

// Upper limit on tick callback functions
#define MAX_TICK_CALLBACKS	8

// Tick callback function table
static tick_task tickFnTbl[MAX_TICK_CALLBACKS];

// module state variables
static BOOL bIRQEnabled = FALSE;	// Set to TRUE once enableIRQ() is called
static U8 uNextFreeVect = 0;		// Next available IRQ slot
static int	_iIdleRate = 0;
static U8 abClassReqData[MAX_PACKET_SIZE0];

// Forward declaration of timer interrupt handler
static void TimerIntHandler(void) __attribute__ ((interrupt("IRQ")));
// Forward declaration of USB interrupt handler
static void USBIntHandler(void) __attribute__ ((interrupt("IRQ")));

// Helper functions
void RegisterTimerInt(void);
void RegisterUSBInt(void);

static void TimerIntHandler(void)
{
	U8 uTickCB = 0;

	// Clear timer 0 interrupt
	T0_IR = T_IR_MR0;

	// Call the tick callback functions in order of registration
	while (uTickCB < MAX_TICK_CALLBACKS && tickFnTbl[uTickCB] != NULL)
	{
		(tickFnTbl[uTickCB])();
		uTickCB++;
	}

	// Signal end of ISR
	VIC_VectAddr = 0x00000000;
}

static void USBIntHandler(void)
{
	// Call USB stack ISR
	USBHwISR();
	// Signal end of ISR
	VIC_VectAddr = 0x00000000;	
}

void RegisterTimerInt(void)
{
	U8 uTickCB;

	// Initialize the tick callback table
	for (uTickCB = 0; uTickCB < MAX_TICK_CALLBACKS; uTickCB++)
	{
		tickFnTbl[uTickCB] = NULL;
	}

	// Set up timer0 to fire every 10ms @ 60MHz
	// Maximum counting frequency for timer is 60MHz / 4
	// == 15MHz. Therefore matching count for 10ms = 1.5*10^7 * 10^-2
	// = 150000 - 1
	T0_MR0 = 149999;
	// Interrupt and reset on MR0 
	T0_MCR = T_MCR_MR0I | T_MCR_MR0R;
	// Enable timer 0
	T0_TCR = T_EMR_EM0;

	// Select timer0 for IRQ (not FIQ)
	VIC_IntSelect &= ~(VIC_IntSelect_Timer0);

	// Enable IRQ for timer0
	*((&VIC_VectCntl0) + TIMER_VECT_NUM) |= VIC_VectCntl_ENABLE | VIC_Channel_Timer0;
	// Set ISR address
	*((&VIC_VectAddr0) + TIMER_VECT_NUM) = (unsigned long)TimerIntHandler;
	// Enable timer 0 interrupts
	VIC_IntEnable |= VIC_IntEnable_Timer0;

	uNextFreeVect = TIMER_VECT_NUM + 1;
}

void RegisterUSBInt(void)
{
	// Select USB interrupt for IRQ (not FIQ)
	VIC_IntSelect &= ~(VIC_IntSelect_USB);              

	// Enable IRQ for USB
	(*(&VIC_VectCntl0 + USB_VECT_NUM)) |= VIC_VectCntl_ENABLE | VIC_Channel_USB;  	
	// Set ISR address
	(*(&VIC_VectAddr0 + USB_VECT_NUM)) = (int)USBIntHandler;
	// Enable USB interrupts
	VIC_IntEnable |= VIC_IntEnable_USB;

	uNextFreeVect = USB_VECT_NUM + 1;
}

// Public functions
int RegisterTickTask(tick_task tickFn)
{
	U8 uTickCB = 0;

	if (bIRQEnabled)
		// Not allowed to register tasks after IRQs enabled
		return EBUSY;

	// Find first open slot in callback table
	while (uTickCB < MAX_TICK_CALLBACKS && tickFnTbl[uTickCB] != NULL)
	{
		uTickCB++;
	}

	if (uTickCB == MAX_TICK_CALLBACKS)
	{
		// Out of slots in the table
		return ENOMEM;
	}
	else
	{
		tickFnTbl[uTickCB] = tickFn;
		return 0;
	}
}

int GetNextFreeIRQ(U8 *IRQ)
{
	if (uNextFreeVect > LAST_VECT_NUM)
		return ENOMEM;

	*IRQ = uNextFreeVect;
	uNextFreeVect++;
	
	return 0;
}

/*************************************************************************
	HandleClassRequest
	==================
		HID class request handler
		
**************************************************************************/
static BOOL HandleClassRequest(TSetupPacket *pSetup, int *piLen, U8 **ppbData)
{
	U8	*pbData = *ppbData;

	switch (pSetup->bRequest) {
	
   	// get_idle
	case HID_GET_IDLE:
		DBG("GET IDLE, val=%X, idx=%X\n", pSetup->wValue, pSetup->wIndex);
		pbData[0] = (_iIdleRate / 4) & 0xFF;
		*piLen = 1;
		break;

	// set_idle:
	case HID_SET_IDLE:
		DBG("SET IDLE, val=%X, idx=%X\n", pSetup->wValue, pSetup->wIndex);
		_iIdleRate = ((pSetup->wValue >> 8) & 0xFF) * 4;
		break;
	case HID_GET_REPORT:
		DBG("GET_REPORT, val=%X, idx=%X\n", pSetup->wValue, pSetup->wIndex);
		// Send last available response report to host
		break;
	case HID_SET_REPORT:
		DBG("SET_REPORT, val=%X, idx=%X, data=%X\n", pSetup->wValue, pSetup->wIndex, *ppbData[0]);
		// Process new request report
		break;
	default:
		DBG("Unhandled class %X\n", pSetup->bRequest);
		return FALSE;
	}
	return TRUE;
}

/*************************************************************************
	HIDHandleStdReq
	===============
		Standard request handler for HID devices.
		
	This function tries to service any HID specific requests.
		
**************************************************************************/
static BOOL HIDHandleStdReq(TSetupPacket *pSetup, int *piLen, U8 **ppbData)
{
	U8	bType, bIndex;

	if ((pSetup->bmRequestType == 0x81) &&			// standard IN request for interface
		(pSetup->bRequest == REQ_GET_DESCRIPTOR)) {	// get descriptor
		
		bType = GET_DESC_TYPE(pSetup->wValue);
		bIndex = GET_DESC_INDEX(pSetup->wValue);
		switch (bType) {

		case DESC_HID_REPORT:
			// report
			*ppbData = GetReportDescriptor(piLen);
			break;

		case DESC_HID_HID:
		case DESC_HID_PHYSICAL:
		default:
		    // search descriptor space
		    return USBGetDescriptor(pSetup->wValue, pSetup->wIndex, piLen, ppbData);
		}
		
		return TRUE;
	}
	return FALSE;
}

/*************************************************************************
	main
	====
**************************************************************************/
int main(void)
{	
	int err;

	// PLL, PCLK and MAM
	Initialize();

	// init DBG
	ConsoleInit(60000000 / (16 * BAUD_RATE));
	DEBUG_LED_INIT

	DBG("Initialising USB stack\n");
	
	// initialise stack
	USBInit();
	
	// register device descriptors
	USBRegisterDescriptors(GetDeviceDescriptors());

	// register HID standard request handler
	USBRegisterCustomReqHandler(HIDHandleStdReq);

	// register class request handler
	USBRegisterRequestHandler(REQTYPE_TYPE_CLASS, HandleClassRequest, abClassReqData);

	// register endpoint
	USBHwRegisterEPIntHandler(INTR_IN_EP, NULL);

	// Set up USB interrupt
	RegisterUSBInt();

	// Set up timer interrupt
	RegisterTimerInt();

	// Initialize i2c driver
	if ((err = i2c_driver_init(PCLK_FREQ_MHZ, I2C_FREQ_KHZ)) != I2C_ERR_NONE)
	{
		DBG("main: i2c driver init failed %d\n", err);
		return err;
	}

	enableIRQ();
	bIRQEnabled = TRUE;

	DBG("Starting USB communication\n");

	// connect to bus
	USBHwConnect(TRUE);

	while (TRUE);

	return EIO;
}

// end of file
