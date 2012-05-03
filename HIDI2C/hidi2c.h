#ifndef HIDI2C_H
#define HIDI2C_H

// The firmware tick timer is defined inside HIDI2C, but
// other modules can register tick tasks. NOTE: This function
// cannot be called after enableIRQ() is called
typedef void (*tick_task)(void);

int RegisterTickTask(tick_task tickFn);

// Other modules can determine the next available free IRQ
// slot by calling this function
int GetNextFreeIRQ(U8 *IRQ);

#endif // HIDI2C_H
