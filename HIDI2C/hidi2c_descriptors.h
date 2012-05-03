#ifndef HIDI2C_DESC_H
#define HIDI2C_DESC_H

#define INTR_IN_EP		0x81
#define MAX_PACKET_SIZE	64
#define LE_WORD(x)		((x)&0xFF),((x)>>8)

#define	REQUEST_REPORT_ID		1
#define REQUEST_REPORT_SIZE		0x40 // 64
#define RESPONSE_REPORT_ID		2
#define RESPONSE_REPORT_SIZE	0x40 // 64

const U8 *GetDeviceDescriptors(void);
const U8 *GetReportDescriptor(int *piDescLen);

#endif // HIDI2C_DESC_H
