#include "type.h"
#include "usbapi.h"
#include "hidi2c_descriptors.h"

static const U8 abReportDesc[] = {
    0x06, 0x00, 0xff,              // USAGE_PAGE (Vendor Defined Page 1)
    0x09, 0x01,                    // USAGE (Vendor Usage 1)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x85, REQUEST_REPORT_ID,       //   REPORT_ID (1)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, REQUEST_REPORT_SIZE,     //   REPORT_COUNT (64)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x09, 0x01,                    //   USAGE (Vendor Usage 1)
    0x91, 0x02,                    //   OUTPUT (Data,Var,Abs)
    0x85, RESPONSE_REPORT_ID,      //   REPORT_ID (2)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, RESPONSE_REPORT_SIZE,    //   REPORT_COUNT (64)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x09, 0x01,                    //   USAGE (Vendor Usage 1)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0xc0                           // END_COLLECTION
};

static const U8 abDescriptors[] = {

/* Device descriptor */
	0x12,              		
	DESC_DEVICE,       		
	LE_WORD(0x0110),		// bcdUSB	
	0x00,              		// bDeviceClass
	0x00,              		// bDeviceSubClass
	0x00,              		// bDeviceProtocol
	MAX_PACKET_SIZE0,  		// bMaxPacketSize
	LE_WORD(0xFFFF),		// idVendor
	LE_WORD(0x0001),		// idProduct
	LE_WORD(0x0100),		// bcdDevice
	0x01,              		// iManufacturer
	0x02,              		// iProduct
	0x03,              		// iSerialNumber
	0x01,              		// bNumConfigurations

// configuration
	0x09,
	DESC_CONFIGURATION,
	LE_WORD(0x22),  		// wTotalLength
	0x01,  					// bNumInterfaces
	0x01,  					// bConfigurationValue
	0x00,  					// iConfiguration
	0x80,  					// bmAttributes
	0x32,  					// bMaxPower

// interface
	0x09,   				
	DESC_INTERFACE, 
	0x00,  		 			// bInterfaceNumber
	0x00,   				// bAlternateSetting
	0x01,   				// bNumEndPoints
	0x03,   				// bInterfaceClass = HID
	0x00,   				// bInterfaceSubClass
	0x00,   				// bInterfaceProtocol
	0x00,   				// iInterface

// HID descriptor
	0x09, 
	DESC_HID_HID, 			// bDescriptorType = HID
	LE_WORD(0x0110),		// bcdHID
	0x00,   				// bCountryCode
	0x01,   				// bNumDescriptors = report
	DESC_HID_REPORT,   		// bDescriptorType
	LE_WORD(sizeof(abReportDesc)),

// EP descriptor
	0x07,   		
	DESC_ENDPOINT,   		
	INTR_IN_EP,				// bEndpointAddress
	0x03,   				// bmAttributes = INT
	LE_WORD(MAX_PACKET_SIZE),// wMaxPacketSize
	10,						// bInterval   		

	// string descriptors
	0x04,					// bLength
	DESC_STRING,			// bType
	LE_WORD(0x0409),

	// manufacturer string
	0x18,					// bLength
	DESC_STRING,			// bType
	'H', 0, 'O', 0, 'T', 0, 'P', 0, 'L', 0, 'U', 0, 'G', 0, 'L', 0, 'A', 0, 'B', 0, 'S', 0, 

	// product string
	0x10,
	DESC_STRING,
	'R', 0, 'a', 0, 'l', 0, 'e', 0, 'i', 0, 'g', 0, 'h', 0,

	// serial number string
	0x0E,
	DESC_STRING,
	'1', 0, '0', 0, '0', 0, 'R', 0, 'A', 0, '0', 0,
	
	// terminator
	0
};

// Descriptor accessors
const U8 *GetDeviceDescriptors(void)
{
	return abDescriptors;
}

const U8 *GetReportDescriptor(int *piDescLen)
{
	*piDescLen = sizeof(abReportDesc);
	return abReportDesc;
}
