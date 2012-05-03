#include "type.h"
#include "usbdebug.h"
#include "lpc214x.h"
#include "Debug_LEDs.h"

#include "console.h"
#include "usbapi.h"
#include "startup.h"
#include "armVIC.h"

#include "hid_usb_desc.h"

#define CAPS_LOCK_ON		1
#define SCROLL_LOCK_ON		2

#define LEDMSK			0x00FF0000
#define LOWER_CASE_A	0x04

#define BAUD_RATE		115200

// interrupts
#define VICIntSelect   *((volatile unsigned int *) 0xFFFFF00C)
#define VICIntEnable   *((volatile unsigned int *) 0xFFFFF010)
#define VICVectAddr    *((volatile unsigned int *) 0xFFFFF030)
#define VICVectAddr0   *((volatile unsigned int *) 0xFFFFF100)
#define VICVectCntl0   *((volatile unsigned int *) 0xFFFFF200)

#define	INT_VECT_NUM	1

#define IRQ_MASK 0x00000080

struct kbd_report
{
	U8 modifier;    
	U8 reserved;    
	U8 keycode[6];
};

static U8 abClassReqData[4];
static int	_iIdleRate = 0;

// Output report
U8 led_report;
// Input report
struct kbd_report key_states = {0, 0, {0, 0, 0, 0, 0, 0}};
// Flag indicating whether to send a keyboard report to host
BOOL bReport = FALSE;
// Flag indicating whether next event is debounced
BOOL bDebounced;

// Forward declaration of timer interrupt handler
static void TimerIntHandler(void) __attribute__ ((interrupt("IRQ")));
// Forward declaration of USB interrupt handler
static void USBIntHandler(void) __attribute__ ((interrupt("IRQ")));

static void TimerIntHandler(void)
{
	static U8 uDnCnt = 0;
	static U8 uUpCnt = 0;
	
	// Clear timer 0 interrupt
	T0_IR = T_IR_MR0;

	// Read P014
	if ((GPIO0_IOPIN & 0x00004000) == 0x00000000)
	{
		// Pin is low -- add to down count
		uDnCnt++;
	}
	else
	{
		// Add to up count
		uUpCnt++;
	}

	if (uDnCnt + uUpCnt == 5)
	{
		// Debouncing over 5 consecutive intervals complete
		bDebounced = TRUE;
		if (uDnCnt > uUpCnt)
		{
			key_states.keycode[2] = LOWER_CASE_A;
			// We always report down for repeats
			bReport = TRUE;
		}
		else
		{
			if (key_states.keycode[2] != 0)
			{
				// We were down, but just went up
				key_states.keycode[2] = 0;
				bReport = TRUE;
			}
			else
			{
				// We were up already
				bReport = FALSE;
			}
		}
		// Reset counters
		uDnCnt = 0;
		uUpCnt = 0;
	}
	else
	{
		// Still debouncing
		bDebounced = FALSE;
	}

	// Signal end of ISR
	VIC_VectAddr = 0x00000000; 
}

BOOL SendReport()
{
	BOOL bRet;
	
	// Disable timer interrupts while checking the report and debounced flags
	VIC_IntEnable &= ~(0x00000010);
	
	bRet = (bReport && bDebounced);
	
	// Re-enable timer interrups
	VIC_IntEnable |= 0x00000010;

	return bRet;
}

void ReadKbdReport(struct kbd_report *report)
{
	// Disable timer interrupts while reading the keycode
	VIC_IntEnable &= ~(0x00000010);

	// Copy in the report data
	report->modifier = key_states.modifier;
	report->reserved = key_states.reserved;
	report->keycode[0] = key_states.keycode[0];
	report->keycode[1] = key_states.keycode[1];
	report->keycode[2] = key_states.keycode[2];
	report->keycode[3] = key_states.keycode[3];
	report->keycode[4] = key_states.keycode[4];
	report->keycode[5] = key_states.keycode[5];

	// Re-enable timer interrupts
	VIC_IntEnable |= 0x00000010;
}

/**
	Interrupt handler
	
	Simply calls the USB ISR, then signals end of interrupt to VIC
 */
static void USBIntHandler(void)
{
	USBHwISR();
	VICVectAddr = 0x00000000;    // dummy write to VIC to signal end of ISR 	
}

void RegisterTimerInt(void)
{
	// Program P0.14 for GPIO
	PCB_PINSEL0 = (PCB_PINSEL0 & ~PCB_PINSEL0_P014_MASK) | PCB_PINSEL0_P014_GPIO;   
	// Set direction to input
	GPIO0_IODIR &= 0xFFFFDFFF;
  
	// Set up timer0 to fire every 10ms @ 60MHz
	// Maximum counting frequency for timer is 60MHz / 4
	// == 15MHz. Therefore matching count for 10ms = 1.5*10^7 * 10^-2
	// = 150000 - 1
	T0_MR0 = 149999;
	// Interrupt and reset on MR0 
	T0_MCR = T_MCR_MR0I | T_MCR_MR0R;
	// Enable timer 0
	T0_TCR = T_EMR_EM0;

	// Enable IRQ0 for timer
	VIC_VectCntl0 = 0x20 | 0x04;
	// Set ISR address
	VIC_VectAddr0 = (unsigned long)TimerIntHandler;
	// Enable timer 0 interrupts
	VIC_IntEnable |= 0x00000010;
}

static void SetOutReport()
{
	if (led_report & CAPS_LOCK_ON)
	{
		DEBUG_LED_ON(6);
	}
	else
	{
		DEBUG_LED_OFF(6);
	}
	if (led_report & SCROLL_LOCK_ON)
	{
		DEBUG_LED_ON(7);
	}
	else
	{
		DEBUG_LED_OFF(7);
	}
}


/*************************************************************************
	HandleClassRequest
	==================
		HID class request handler
		
**************************************************************************/
static BOOL HandleClassRequest(TSetupPacket *pSetup, int *piLen, U8 **ppbData)
{
	U8	*pbData = *ppbData;
	struct kbd_report rpt;

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
		// Get a copy of the report
		ReadKbdReport(&rpt);
		// Send it to the host
		USBHwEPWrite(INTR_IN_EP, &rpt, sizeof(rpt));
		break;
	case HID_SET_REPORT:
		DBG("SET_REPORT, val=%X, idx=%X, data=%X\n", pSetup->wValue, pSetup->wIndex, *ppbData[0]);
		led_report = *ppbData[0] >> 1;
		SetOutReport();
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
	struct kbd_report rpt;

	// PLL and MAM
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

	// set up USB interrupt
	VICIntSelect &= ~(1<<22);               // select IRQ for USB
	VICIntEnable |= (1<<22);

	(*(&VICVectCntl0+INT_VECT_NUM)) = 0x20 | 22; // choose highest priority ISR slot 	
	(*(&VICVectAddr0+INT_VECT_NUM)) = (int)USBIntHandler;
	
	RegisterTimerInt();
	enableIRQ();

	DBG("Starting USB communication\n");

	// connect to bus
	USBHwConnect(TRUE);

	while (TRUE)
	{
		if (SendReport())
		{
			ReadKbdReport(&rpt);
			// Send it to the host
			USBHwEPWrite(INTR_IN_EP, &rpt, sizeof(rpt));
		}
	}

	return 0;
}

// end of file
