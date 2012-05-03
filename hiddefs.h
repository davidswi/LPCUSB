#ifndef HIDDEFS_H
#define HIDDEFS_H

// HID class requests are already defined in usbstruct.h

// HID subclass types
#define HIDSUB_NONE			0
#define HIDSUB_BOOT			1

// HID protocol types
#define HIDPRO_NONE			0
#define HIDPRO_KEYBOARD		1
#define HIDPRO_MOUSE		2

// HID descriptor types
#define HIDDSC_HID			0x21
#define HIDDSC_REPORT		0x22
#define HIDDSC_PHYSICAL		0x23

// HID report types
#define HIDRPT_INPUT		1
#define HIDRPT_OUTPUT		2
#define HIDRPT_FEATURE		3

// HID pages
#define HIDPAGE_UNDEFINED	0
#define HIDPAGE_GENERIC		1
#define HIDPAGE_SIMULATION	2
#define HIDPAGE_VR			3
#define HIDPAGE_SPORT		4
#define HIDPAGE_GAME		5 
#define HIDPAGE_DEVICE		6 
#define HIDPAGE_KEYBOARD	7 
#define HIDPAGE_LED			8 
#define HIDPAGE_BUTTON		9
#define HIDPAGE_ORDINAL		0x0A
#define HIDPAGE_TELEPHONY	0x0B
#define HIDPAGE_CONSUMER	0x0C
#define HIDPAGE_DIGITIZER	0x0D
#define HIDPAGE_UNICODE		0x10
#define HIDPAGE_ALPHA		0x14
#define HIDPAGE_BARCODE		0x8C 
#define HIDPAGE_CAMERA		0x90 
#define HIDPAGE_VENDOR		0xFF 

// Generic page usages (0x01)
#define HIDGEN_POINTER				 0x01
#define HIDGEN_MOUSE                 0x02
#define HIDGEN_JOYSTICK              0x04
#define HIDGEN_GAMEPAD               0x05
#define HIDGEN_KEYBOARD              0x06
#define HIDGEN_KEYPAD                0x07
#define HIDGEN_X                     0x30
#define HIDGEN_Y                     0x31
#define HIDGEN_Z                     0x32
#define HIDGEN_RX                    0x33
#define HIDGEN_RY                    0x34
#define HIDGEN_RZ                    0x35
#define HIDGEN_SLIDER                0x36
#define HIDGEN_DIAL                  0x37
#define HIDGEN_WHEEL                 0x38
#define HIDGEN_HATSWITCH             0x39
#define HIDGEN_COUNTED_BUFFER        0x3A
#define HIDGEN_BYTE_COUNT            0x3B
#define HIDGEN_MOTION_WAKEUP         0x3C
#define HIDGEN_VX                    0x40
#define HIDGEN_VY                    0x41
#define HIDGEN_VZ                    0x42
#define HIDGEN_VBRX                  0x43
#define HIDGEN_VBRY                  0x44
#define HIDGEN_VBRZ                  0x45
#define HIDGEN_VNO                   0x46
#define HIDGEN_SYSTEM_CTL            0x80
#define HIDGEN_SYSCTL_POWER          0x81
#define HIDGEN_SYSCTL_SLEEP          0x82
#define HIDGEN_SYSCTL_WAKE           0x83
#define HIDGEN_SYSCTL_CONTEXT_MENU   0x84
#define HIDGEN_SYSCTL_MAIN_MENU      0x85
#define HIDGEN_SYSCTL_APP_MENU       0x86
#define HIDGEN_SYSCTL_HELP_MENU      0x87
#define HIDGEN_SYSCTL_MENU_EXIT      0x88
#define HIDGEN_SYSCTL_MENU_SELECT    0x89
#define HIDGEN_SYSCTL_MENU_RIGHT     0x8A
#define HIDGEN_SYSCTL_MENU_LEFT      0x8B
#define HIDGEN_SYSCTL_MENU_UP        0x8C
#define HIDGEN_SYSCTL_MENU_DOWN      0x8D

// Keyboard page (7)
// Adapted from Wixel SDK
// http://pololu.github.com/wixel-sdk/usb__hid__constants_8h_source.html
#define KEY_NONE               0x00
#define KEY_A                  0x04
#define KEY_B                  0x05
#define KEY_C                  0x06
#define KEY_D                  0x07
#define KEY_E                  0x08
#define KEY_F                  0x09
#define KEY_G                  0x0A
#define KEY_H                  0x0B
#define KEY_I                  0x0C
#define KEY_J                  0x0D
#define KEY_K                  0x0E
#define KEY_L                  0x0F
#define KEY_M                  0x10
#define KEY_N                  0x11
#define KEY_O                  0x12
#define KEY_P                  0x13
#define KEY_Q                  0x14
#define KEY_R                  0x15
#define KEY_S                  0x16
#define KEY_T                  0x17
#define KEY_U                  0x18
#define KEY_V                  0x19
#define KEY_W                  0x1A
#define KEY_X                  0x1B
#define KEY_Y                  0x1C
#define KEY_Z                  0x1D
#define KEY_1                  0x1E
#define KEY_2                  0x1F
#define KEY_3                  0x20
#define KEY_4                  0x21
#define KEY_5                  0x22
#define KEY_6                  0x23
#define KEY_7                  0x24
#define KEY_8                  0x25
#define KEY_9                  0x26
#define KEY_0                  0x27
#define KEY_RETURN             0x28
#define KEY_ESCAPE             0x29
#define KEY_BACKSPACE          0x2A
#define KEY_TAB                0x2B
#define KEY_SPACE              0x2C
#define KEY_MINUS              0x2D
#define KEY_EQUAL              0x2E
#define KEY_BRACKET_LEFT       0x2F
#define KEY_BRACKET_RIGHT      0x30
#define KEY_BACKSLASH          0x31
#define KEY_EUROPE_1           0x32
#define KEY_SEMICOLON          0x33
#define KEY_APOSTROPHE         0x34
#define KEY_GRAVE              0x35
#define KEY_COMMA              0x36
#define KEY_PERIOD             0x37
#define KEY_SLASH              0x38
#define KEY_CAPS_LOCK          0x39
#define KEY_F1                 0x3A
#define KEY_F2                 0x3B
#define KEY_F3                 0x3C
#define KEY_F4                 0x3D
#define KEY_F5                 0x3E
#define KEY_F6                 0x3F
#define KEY_F7                 0x40
#define KEY_F8                 0x41
#define KEY_F9                 0x42
#define KEY_F10                0x43
#define KEY_F11                0x44
#define KEY_F12                0x45
#define KEY_PRINT_SCREEN       0x46
#define KEY_SCROLL_LOCK        0x47
#define KEY_PAUSE              0x48
#define KEY_INSERT             0x49
#define KEY_HOME               0x4A
#define KEY_PAGE_UP            0x4B
#define KEY_DELETE             0x4C
#define KEY_END                0x4D
#define KEY_PAGE_DOWN          0x4E
#define KEY_ARROW_RIGHT        0x4F
#define KEY_ARROW_LEFT         0x50
#define KEY_ARROW_DOWN         0x51
#define KEY_ARROW_UP           0x52
#define KEY_NUM_LOCK           0x53
#define KEY_KEYPAD_DIVIDE      0x54
#define KEY_KEYPAD_MULTIPLY    0x55
#define KEY_KEYPAD_SUBTRACT    0x56
#define KEY_KEYPAD_ADD         0x57
#define KEY_KEYPAD_ENTER       0x58
#define KEY_KEYPAD_1           0x59
#define KEY_KEYPAD_2           0x5A
#define KEY_KEYPAD_3           0x5B
#define KEY_KEYPAD_4           0x5C
#define KEY_KEYPAD_5           0x5D
#define KEY_KEYPAD_6           0x5E
#define KEY_KEYPAD_7           0x5F
#define KEY_KEYPAD_8           0x60
#define KEY_KEYPAD_9           0x61
#define KEY_KEYPAD_0           0x62
#define KEY_KEYPAD_DECIMAL     0x63
#define KEY_EUROPE_2           0x64
#define KEY_APPLICATION        0x65
#define KEY_POWER              0x66
#define KEY_KEYPAD_EQUAL       0x67
#define KEY_F13                0x68
#define KEY_F14                0x69
#define KEY_F15                0x6A
#define KEY_CONTROL_LEFT       0xE0
#define KEY_SHIFT_LEFT         0xE1
#define KEY_ALT_LEFT           0xE2
#define KEY_GUI_LEFT           0xE3
#define KEY_CONTROL_RIGHT      0xE4
#define KEY_SHIFT_RIGHT        0xE5
#define KEY_ALT_RIGHT          0xE6
#define KEY_GUI_RIGHT          0xE7

// Keyboard modifiers
#define MODIFIER_CONTROL_LEFT  0xE0
#define MODIFIER_SHIFT_LEFT    0xE1
#define MODIFIER_ALT_LEFT      0xE2
#define MODIFIER_GUI_LEFT      0xE3
#define MODIFIER_CONTROL_RIGHT 0xE4
#define MODIFIER_SHIFT_RIGHT   0xE5
#define MODIFIER_ALT_RIGHT     0xE6
#define MODIFIER_GUI_RIGHT     0xE7

// LED page (8)
#define LED_NUM_LOCK				0x01
#define LED_CAPS_LOCK				0x02
#define LED_SCROLL_LOCK				0x03
#define LED_COMPOSE					0x04
#define LED_KANA					0x05
#define LED_POWER					0x06
#define LED_SHIFT					0x07
#define LED_DO_NOT_DISTURB			0x08
#define LED_MUTE					0x09
#define LED_TONE_ENABLE             0x0A
#define LED_HIGH_CUT_FILTER         0x0B
#define LED_LOW_CUT_FILTER          0x0C
#define LED_EQUALIZER_ENABLE        0x0D
#define LED_SOUND_FIELD_ON          0x0E
#define LED_SURROUND_FIELD_ON		0x0F
#define LED_REPEAT                  0x10
#define LED_STEREO                  0x11
#define LED_SAMPLING_RATE_DETECT	0x12
#define LED_SPINNING                0x13
#define LED_CAV                     0x14
#define LED_CLV                     0x15
#define LED_RECORDING_FORMAT_DET    0x16
#define LED_OFF_HOOK                0x17
#define LED_RING                    0x18
#define LED_MESSAGE_WAITING         0x19
#define LED_DATA_MODE               0x1A
#define LED_BATTERY_OPERATION       0x1B
#define LED_BATTERY_OK              0x1C
#define LED_BATTERY_LOW             0x1D
#define LED_SPEAKER                 0x1E
#define LED_HEAD_SET                0x1F
#define LED_HOLD                    0x20
#define LED_MICROPHONE              0x21
#define LED_COVERAGE                0x22
#define LED_NIGHT_MODE              0x23
#define LED_SEND_CALLS              0x24
#define LED_CALL_PICKUP             0x25
#define LED_CONFERENCE              0x26
#define LED_STAND_BY                0x27
#define LED_CAMERA_ON               0x28
#define LED_CAMERA_OFF              0x29
#define LED_ON_LINE                 0x2A
#define LED_OFF_LINE                0x2B
#define LED_BUSY                    0x2C
#define LED_READY                   0x2D
#define LED_PAPER_OUT               0x2E
#define LED_PAPER_JAM               0x2F
#define LED_REMOTE                  0x30
#define LED_FORWARD                 0x31
#define LED_REVERSE                 0x32
#define LED_STOP                    0x33
#define LED_REWIND                  0x34
#define LED_FAST_FORWARD            0x35
#define LED_PLAY                    0x36
#define LED_PAUSE                   0x37
#define LED_RECORD                  0x38
#define LED_ERROR                   0x39
#define LED_SELECTED_INDICATOR      0x3A
#define LED_IN_USE_INDICATOR        0x3B
#define LED_MULTI_MODE_INDICATOR    0x3C
#define LED_INDICATOR_ON            0x3D
#define LED_INDICATOR_FLASH         0x3E
#define LED_INDICATOR_SLOW_BLINK    0x3F
#define LED_INDICATOR_FAST_BLINK    0x40
#define LED_INDICATOR_OFF           0x41
#define LED_FLASH_ON_TIME           0x42
#define LED_SLOW_BLINK_ON_TIME      0x43
#define LED_SLOW_BLINK_OFF_TIME     0x44
#define LED_FAST_BLINK_ON_TIME      0x45
#define LED_FAST_BLINK_OFF_TIME     0x46
#define LED_INDICATOR_COLOR         0x47
#define LED_RED                     0x48
#define LED_GREEN                   0x49
#define LED_AMBER                   0x4A
#define LED_GENERIC_INDICATOR       0x4B

// Telephony usages
#define TELEPHONY_PHONE               0x01
#define TELEPHONY_ANSWERING_MACHINE   0x02
#define TELEPHONY_MESSAGE_CONTROLS    0x03
#define TELEPHONY_HANDSET             0x04
#define TELEPHONY_HEADSET             0x05
#define TELEPHONY_KEYPAD              0x06
#define TELEPHONY_PROGRAMMABLE_BUTTON 0x07

// Report item macros
// Main
#define INPUT(i)			0x81, (i)
#define OUTPUT(o)			0x91, (o)
#define FEATURE(f)			0xB1, (f)
#define COLLECTION(c)		0xA1, (c)

// Feature Data
#define DATA				0
#define CONSTANT			1
#define ARRAY				0
#define VARIABLE			2
#define ABSOLUTE			0
#define RELATIVE			4
#define NOWRAP				0
#define WRAP				8
#define LINEAR				0
#define NON_LINEAR			16
#define PREFERRED			0
#define NO_PREFERRED		32
#define NO_NULLPOS			0
#define NULLSTATE			64
#define NONVOLATILE			0
#define VOLATILE			128

// Collection Data
#define PHYSICAL           0x00
#define APPLICATION        0x01
#define LOGICAL            0x02
#define REPORT             0x03
#define NAMEDARRAY         0x04
#define USAGESWITCH        0x05
#define USAGEMODIFIER      0x06

// Globals
#define USAGEPAGEVENDOR(v)	0x06, (v), 0xFF
#define USAGEPAGE(p)		0x05, (p)
#define REPORTSIZE(s)		0x75, (s)
#define REPORTCOUNT(c)		0x95, (c)
#define LOGICALMIN(m)		0x15, (m)
#define LOGICALMAX(M)		0x25, (M)
#define ENDCOLLECTION		0xC0

// Locals
#define USAGE(u)			0x09, (u)
#define USAGEMIN(m)			0x19, (m)
#define USAGEMAX(M)			0x29, (M)

#endif // HIDDEFS_H
