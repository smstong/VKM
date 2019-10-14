#include <linux/uinput.h>
#include "key.h"

static int map[][2] = {
	{0x08, KEY_BACKSPACE },	/* VK_BACK */
	{0x09, KEY_TAB },	/* VK_TAB */
	/*0x0A - 0B */		/* Reserved */
	{0x0C, KEY_CLEAR },	/* VK_CLEAR */
	{0x0D, KEY_ENTER },	/* VK_RETURN */
	/*0x0E - 0F */		/* Undefined */
	{0x10, KEY_LEFTSHIFT },	/* VK_SHIFT */
	{0x11, KEY_LEFTCTRL },	/* VK_CONTROL */
	{0x12, KEY_LEFTALT },	/* VK_MENU */
	{0x13, KEY_PAUSE },	/* VK_PAUSE */
	{0x14, KEY_CAPSLOCK },	/* VK_CAPITAL */
	{0x15, KEY_KATAKANA },	/* VK_KANA */
	{0x15, KEY_HANGUEL },	/* VK_HANGUEL */
	/*0x16 */		/* Undefined */
	{0x1B, KEY_ESC },	/* VK_ESCAPE */
	{0x20, KEY_SPACE },	/* VK_SPACE */
	{0x21, KEY_PAGEUP },	/* VK_PRIOR */
	{0x22, KEY_PAGEDOWN },	/* VK_NEXT */
	{0x23, KEY_END },	/* VK_END */
	{0x24, KEY_HOME },	/* VK_HOME */
	{0x25, KEY_LEFT },	/* VK_LEFT */
	{0x26, KEY_UP },	/* VK_UP */
	{0x27, KEY_RIGHT },	/* VK_RIGHT */
	{0x28, KEY_DOWN },	/* VK_DOWN */
	{0x29, KEY_SELECT },	/* VK_SELECT */
	{0x2A, KEY_PRINT },	/* VK_PRINT */
	{0x2C, KEY_SCREEN },	/* VK_SNAPSHOT */
	{0x2D, KEY_INSERT },	/* VK_INSERT */
	{0x2E, KEY_DELETE },	/* VK_DELETE */
	{0x2F, KEY_HELP },	/* VK_HELP */
	{0x30, KEY_0 },
	{0x31, KEY_1 },
	{0x32, KEY_2 },
	{0x33, KEY_3 },
	{0x34, KEY_4 },
	{0x35, KEY_5 },
	{0x36, KEY_6 },
	{0x37, KEY_7 },
	{0x38, KEY_8 },
	{0x39, KEY_9 },
	{0x41, KEY_A },
	{0x42, KEY_B },
	{0x43, KEY_C },
	{0x44, KEY_D },
	{0x45, KEY_E },
	{0x46, KEY_F },
	{0x47, KEY_G },
	{0x48, KEY_H },
	{0x49, KEY_I },
	{0x4A, KEY_J },
	{0x4B, KEY_K },
	{0x4C, KEY_L },
	{0x4D, KEY_M },
	{0x4E, KEY_N },
	{0x4F, KEY_O },
	{0x50, KEY_P },
	{0x51, KEY_Q },
	{0x52, KEY_R },
	{0x53, KEY_S },
	{0x54, KEY_T },
	{0x55, KEY_U },
	{0x56, KEY_V },
	{0x57, KEY_W },
	{0x58, KEY_X },
	{0x59, KEY_Y },
	{0x5A, KEY_Z },
	{0x5B, KEY_LEFTMETA },/* VK_LWIN */
	{0x5C, KEY_RIGHTMETA },/* VK_RWIN */
	{0x5D, KEY_APPSELECT },/* VK_APPS */
	{0x5F, KEY_SLEEP },	/* VK_SLEEP */
	{0x60, KEY_KP0 },	/* VK_NUMPAD0 */
	{0x61, KEY_KP1 },	/* VK_NUMPAD1 */
	{0x62, KEY_KP2 },	/* VK_NUMPAD2 */
	{0x63, KEY_KP3 },	/* VK_NUMPAD3 */
	{0x64, KEY_KP4 },	/* VK_NUMPAD4 */
	{0x65, KEY_KP5 },	/* VK_NUMPAD5 */
	{0x66, KEY_KP6 },	/* VK_NUMPAD6 */
	{0x67, KEY_KP7 },	/* VK_NUMPAD7 */
	{0x68, KEY_KP8 },	/* VK_NUMPAD8 */
	{0x69, KEY_KP9 },	/* VK_NUMPAD9 */
	{0x6A, KEY_KPASTERISK },	/* VK_MULTIPLY */
	{0x6B, KEY_KPPLUS },		/* VK_ADD */
/*	{0x6C, KEY_ },*/		/* VK_SEPARATOR */
	{0x6D, KEY_KPMINUS },		/* VK_SUBSTRACT */
	{0x6E, KEY_KPDOT },		/* VK_DECIMAL */
	{0x6F, KEY_KPSLASH },		/* VK_DIVIDE */
	{0x70, KEY_F1 },	/* VK_F1 */
	{0x71, KEY_F2 },	/* VK_F2 */
	{0x72, KEY_F3 },	/* VK_F3 */
	{0x73, KEY_F4 },	/* VK_F4 */
	{0x74, KEY_F5 },	/* VK_F5 */
	{0x75, KEY_F6 },	/* VK_F6 */
	{0x76, KEY_F7 },	/* VK_F7 */
	{0x77, KEY_F8 },	/* VK_F8 */
	{0x78, KEY_F9 },	/* VK_F9 */
	{0x79, KEY_F10 },	/* VK_F10 */
	{0x7A, KEY_F11 },	/* VK_F11 */
	{0x7B, KEY_F12 },	/* VK_F12 */
	{0x7C, KEY_F13 },	/* VK_F13 */
	{0x7D, KEY_F14 },	/* VK_F14 */
	{0x7E, KEY_F15 },	/* VK_F15 */
	{0x7F, KEY_F16 },	/* VK_F16 */
	{0x80, KEY_F17 },	/* VK_F17 */
	{0x81, KEY_F18 },	/* VK_F18 */
	{0x82, KEY_F19 },	/* VK_F19 */
	{0x83, KEY_F20 },	/* VK_F20 */
	{0x84, KEY_F21 },	/* VK_F21 */
	{0x85, KEY_F22 },	/* VK_F22 */
	{0x86, KEY_F23 },	/* VK_F23 */
	{0x87, KEY_F24 },	/* VK_F24 */
	{0x90, KEY_NUMLOCK },	/* VK_NUMLOCK */
	{0x91, KEY_SCROLLLOCK },/* VK_SCROLL */
	{0xA0, KEY_LEFTSHIFT },	/* VK_LSHIFT */
	{0xA1, KEY_RIGHTSHIFT },/* VK_RSHIFT */
	{0xA2, KEY_LEFTCTRL },		/* VK_LCONTROL */
	{0xA3, KEY_RIGHTCTRL },		/* VK_RCONTROL */
	{0xA4, KEY_LEFTALT },	/* VK_LMENU? */
	{0xA5, KEY_RIGHTALT },	/* VK_RMENU? */
	{0xAD, KEY_MUTE },	/* VK_VOLUME_MUTE */
	{0xAE, KEY_VOLUMEDOWN },/* VK_VOLUME_DOWN */
	{0xAF, KEY_VOLUMEUP },	/* VK_VOLUME_UP */
	{0xBA, KEY_SEMICOLON },	/* VK_OEM_1 */
	{0xBB, KEY_EQUAL },	/* VK_OEM_PLUS */
	{0xBC, KEY_COMMA },	/* VK_OEM_COMMA */
	{0xBD, KEY_MINUS },	/* VK_OEM_MINUS */
	{0xBE, KEY_DOT },	/* VK_OEM_PERIOD */
	{0xBF, KEY_SLASH },	/* VK_OEM_2 */
	{0xC0, KEY_GRAVE },	/* VK_OEM_2 */
	{0xDB, KEY_LEFTBRACE },	/* VK_OEM_4 */
	{0xDC, KEY_BACKSLASH },	/* VK_OEM_5 */
	{0xDD, KEY_RIGHTBRACE },/* VK_OEM_6 */
	{0xDE, KEY_APOSTROPHE },/* VK_OEM_7 */
	{0xFA, KEY_PLAY },	/* VK_PLAY */
	{0xFB, KEY_ZOOM },	/* VK_ZOOM */
};

int win_to_linux_key(int winkey)
{
	int i=0;
	for(i=0; i<sizeof(map)/sizeof(map[0]); i++) {
		if(winkey == map[i][0]) {
			return map[i][1];
		}
	}
	return -1;
}
