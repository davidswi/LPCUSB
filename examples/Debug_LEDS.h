#ifndef DEBUG_LEDS_H
#define DEBUG_LEDS_H

// On MCB2140, LEDs are connected to Port1, pins 16..23

#define DEBUG_LED_ON(x)		GPIO1_IOSET = (1 << ((x) + 16));
#define DEBUG_LED_OFF(x)	GPIO1_IOCLR = (1 << ((x) + 16));
// PCB_PINSEL1 &= 0x0100 removed from DEBUG_LED_INIT...
#define DEBUG_LED_INIT		GPIO1_IODIR = 0x00FF0000; GPIO1_IOCLR = 0x00FF0000;

#endif // DEBUG_LEDS_H
