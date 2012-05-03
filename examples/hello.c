#include "startup.h"
#include "console.h"
#include "armVIC.h"
#include "../lpc214x.h"
#include "Debug_LEDS.h"

#define DEBUG
#include "../usbdebug.h"
#define BAUD_RATE			115200

#define TOTAL_BTN_PRESSES	8

// Button press flag
BOOL bBtnPressed = FALSE;

// forward declaration of interrupt handler
static void BtnIntHandler(void) __attribute__ ((interrupt("IRQ")));

static void BtnIntHandler(void)
{
	DEBUG_LED_ON(0);

	// Clear the EINT1 bit
	SCB_EXTINT |= SCB_EXTINT_EINT1;

	// Set the flag
	bBtnPressed = TRUE;
	VIC_VectAddr = 0x00000000; // Signal end of ISR
}

void RegisterButtonInt(void)
{
	// Program P0.14 for EINT1
	PCB_PINSEL0 = (PCB_PINSEL0 & ~PCB_PINSEL0_P014_MASK) | PCB_PINSEL0_P014_EINT1;   
	// Set interrupt polarity in System Control Block
	SCB_EXTPOLAR &= ~SCB_EXTPOLAR_EINT1;
	// Set external interrupt mode to EINT1
	SCB_EXTMODE |=  SCB_EXTMODE_EINT1;
	// Set external interrupt to EINT1
	SCB_EXTINT |=  SCB_EXTINT_EINT1;  
  
	// Set the VIC interrupt select to EINT1
	VIC_IntSelect &= ~VIC_IntSelect_EINT1;
	// Enable EINT1
	VIC_IntEnable |= VIC_IntEnable_EINT1; 
	// Program IRQ slot 8 to handle EINT1
	VIC_VectCntl8 = VIC_VectCntl_ENABLE | VIC_Channel_EINT1;
	// Set IRQ8 ISR address
	VIC_VectAddr8 = (int)BtnIntHandler; 
}

int main(void)
{
	int i = 0;
	Initialize();
	ConsoleInit(60000000 / (16 * BAUD_RATE));

	DBG("Registering EINT1 button handler\n");
	RegisterButtonInt();
	enableIRQ();
	
	// Initialize/clear LEDs
	DEBUG_LED_INIT;

	while (i < TOTAL_BTN_PRESSES)
	{
		if (bBtnPressed)
		{
			i++;
			DBG("Button pressed. Total = %d\n", i);
			bBtnPressed = FALSE;
		}
	}

	DEBUG_LED_OFF(0);

	return 0;
}
