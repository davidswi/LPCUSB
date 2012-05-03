#ifndef HID_USB_DESC_H
#define HID_USB_DESC_H

#define INTR_IN_EP		0x81
#define MAX_PACKET_SIZE	64
#define LE_WORD(x)		((x)&0xFF),((x)>>8)
#define REPORT_SIZE		4

const U8 *GetDeviceDescriptors(void);
const U8 *GetReportDescriptor(int *piDescLen);

#endif // HID_USB_DESC_H
