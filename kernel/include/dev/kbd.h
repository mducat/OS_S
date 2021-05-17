
#ifndef _KBD_H
#define _KBD_H

#define KBD_DATA (0x60)
#define KBD_STATUS (0x64)

static const unsigned char scan_code_set_2[] =
{
    0,     /* 0x00 - undef */
    0,     /* 0x00 - undef */
    0,     /* 0x01 - F9 */
    0,     /* 0x02 - undef */
    0,     /* 0x03 - F5 */
    0,     /* 0x04 - F3 */
 	0,     /* 0x05 - F1 */ 
    0,     /* 0x06 - F2 */
 	0,     /* 0x07 - F12 */
    0,     /* 0x08 - undef */
    0,     /* 0x09 - F10 */
 	0,     /* 0x0A - F8 */
    0,     /* 0x0B - F6 */
    0,     /* 0x0C - F4 */
 	'\t',  /* 0x0D - tab */
 	'`',   /* 0x0E - ` (back tick) */
    0,     /* 0x0F - undef */
    0,     /* 0x11 - left alt */
 	0,     /* 0x12 - left shift */
    0,     /* 0x13 - undef */
    0,     /* 0x14 - left control */
    'q',   /* 0x15 - Q */
 	'1',   /* 0x16 - 1 */
    0,     /* 0x17 - undef */
    0,     /* 0x18 - undef */
    0,     /* 0x19 - undef */
    'z',   /* 0x1A - Z */
 	's',   /* 0x1B - S */
    'a',   /* 0x1C - A */
    'w',   /* 0x1D - W */
    '2',   /* 0x1E - 2 */
    0,     /* 0x1F - undef */
 	0,     /* 0x20 - undef */
    'c',   /* 0x21 - C */
    'x',   /* 0x22 - X */
    'd',   /* 0x23 - D */
    'e',   /* 0x24 - E */
 	'4',   /* 0x25 - 4 */
    '3',   /* 0x26 - 3 */
    0,     /* 0x27 - undef */
    0,     /* 0x28 - undef */
    ' ',   /* 0x29 - space */
 	'v',   /* 0x2A - V */
    'f',   /* 0x2B - F */
    't',   /* 0x2C - T */
    'r',   /* 0x2D - R */
    '5',   /* 0x2E - 5 */
 	0,     /* 0x2F - undef */
    0,     /* 0x30 - undef */
    'n',   /* 0x31 - N */
    'b',   /* 0x32 - B */
    'h',   /* 0x33 - H */
 	'g',   /* 0x34 - G */
    'y',   /* 0x35 - Y */
    '6',   /* 0x36 - 6 */
    0,     /* 0x37 - undef */
 	0,     /* 0x38 - undef */
    0,     /* 0x39 - undef */
    'm',     /* 0x3A - M */
    'j',   /* 0x3B - J */
 	'u',   /* 0x3C - U */	
 	'7',   /* 0x3D - 7 */
 	'8',   /* 0x3E - 8 */
    0,     /* 0x3F - undef */
    0,     /* 0x40 - undef */
	',',   /* 0x41 - , */
 	'k',   /* 0x42 - K */	
 	'i',   /* 0x43 - I */
 	'o',   /* 0x44 - O */
 	'0',   /* 0x45 - 0 */
 	'9',   /* 0x46 - 9 */
    0,     /* 0x47 - undef */
    0,     /* 0x48 - undef */
	'.',   /* 0x49 - . */
 	'/',   /* 0x4A - / */	
 	'l',   /* 0x4B - L */
 	';',   /* 0x4C - ; */
 	'p',   /* 0x4D - P */
    '-',   /* 0x4E - - */
    0,     /* 0x4F - undef */
    0,     /* 0x50 - undef */
    0,     /* 0x51 - undef */
 	'\'',  /* 0x52 - ' */
    0,     /* 0x53 - undef */
 	'[',   /* 0x54 - [ */
 	'=',   /* 0x55 - = */
    0,     /* 0x56 - undef */
    0,     /* 0x57 - undef */
    0,     /* 0x58 - caps lock */
    0,     /* 0x59 - right shift */
 	'\n',  /* 0x5A - enter */
 	']',   /* 0x5B - ] */
    0,     /* 0x5C - undef */
 	'\\',  /* 0x5D - \ */
    0,     /* 0x5E - undef */
    0,     /* 0x5F - undef */
    0,     /* 0x60 - undef */
    0,     /* 0x61 - undef */
    0,     /* 0x62 - undef */
    0,     /* 0x63 - undef */
    0,     /* 0x64 - undef */
    0,     /* 0x65 - undef */
 	'\b',  /* 0x66 - backspace */
};

static const unsigned char scan_code_set_2_fr[] =
{
    0,     /* 0x00 - undef */
    0,     /* 0x00 - undef */
    0,     /* 0x01 - F9 */
    0,     /* 0x02 - undef */
    0,     /* 0x03 - F5 */
    0,     /* 0x04 - F3 */
 	0,     /* 0x05 - F1 */ 
    0,     /* 0x06 - F2 */
 	0,     /* 0x07 - F12 */
    0,     /* 0x08 - undef */
    0,     /* 0x09 - F10 */
 	0,     /* 0x0A - F8 */
    0,     /* 0x0B - F6 */
    0,     /* 0x0C - F4 */
 	'\t',  /* 0x0D - tab */
 	'`',   /* 0x0E - ` (back tick) */
    0,     /* 0x0F - undef */
    0,     /* 0x11 - left alt */
 	0,     /* 0x12 - left shift */
    0,     /* 0x13 - undef */
    0,     /* 0x14 - left control */
    'a',   /* 0x15 - A */
 	'1',   /* 0x16 - 1 */
    0,     /* 0x17 - undef */
    0,     /* 0x18 - undef */
    0,     /* 0x19 - undef */
    'w',   /* 0x1A - W */
 	's',   /* 0x1B - S */
    'q',   /* 0x1C - Q */
    'z',   /* 0x1D - Z */
    '2',   /* 0x1E - 2 */
    0,     /* 0x1F - undef */
 	0,     /* 0x20 - undef */
    'c',   /* 0x21 - C */
    'x',   /* 0x22 - X */
    'd',   /* 0x23 - D */
    'e',   /* 0x24 - E */
 	'4',   /* 0x25 - 4 */
    '3',   /* 0x26 - 3 */
    0,     /* 0x27 - undef */
    0,     /* 0x28 - undef */
    ' ',   /* 0x29 - space */
 	'v',   /* 0x2A - V */
    'f',   /* 0x2B - F */
    't',   /* 0x2C - T */
    'r',   /* 0x2D - R */
    '5',   /* 0x2E - 5 */
 	0,     /* 0x2F - undef */
    0,     /* 0x30 - undef */
    'n',   /* 0x31 - N */
    'b',   /* 0x32 - B */
    'h',   /* 0x33 - H */
 	'g',   /* 0x34 - G */
    'y',   /* 0x35 - Y */
    '6',   /* 0x36 - 6 */
    0,     /* 0x37 - undef */
 	0,     /* 0x38 - undef */
    0,     /* 0x39 - undef */
    ',',   /* 0x3A - , */
    'j',   /* 0x3B - J */
 	'u',   /* 0x3C - U */	
 	'7',   /* 0x3D - 7 */
 	'8',   /* 0x3E - 8 */
    0,     /* 0x3F - undef */
    0,     /* 0x40 - undef */
	';',   /* 0x41 - ; */
 	'k',   /* 0x42 - K */	
 	'i',   /* 0x43 - I */
 	'o',   /* 0x44 - O */
 	'0',   /* 0x45 - 0 */
 	'9',   /* 0x46 - 9 */
    0,     /* 0x47 - undef */
    0,     /* 0x48 - undef */
	'.',   /* 0x49 - . */
 	'/',   /* 0x4A - / */	
 	'l',   /* 0x4B - L */
 	'm',   /* 0x4C - M */
 	'p',   /* 0x4D - P */
    '-',   /* 0x4E - - */
    0,     /* 0x4F - undef */
    0,     /* 0x50 - undef */
    0,     /* 0x51 - undef */
 	'\'',  /* 0x52 - ' */
    0,     /* 0x53 - undef */
 	'[',   /* 0x54 - [ */
 	'=',   /* 0x55 - = */
    0,     /* 0x56 - undef */
    0,     /* 0x57 - undef */
    0,     /* 0x58 - caps lock */
    0,     /* 0x59 - right shift */
 	'\n',  /* 0x5A - enter */
 	']',   /* 0x5B - ] */
    0,     /* 0x5C - undef */
 	'\\',  /* 0x5D - \ */
    0,     /* 0x5E - undef */
    0,     /* 0x5F - undef */
    0,     /* 0x60 - undef */
    0,     /* 0x61 - undef */
    0,     /* 0x62 - undef */
    0,     /* 0x63 - undef */
    0,     /* 0x64 - undef */
    0,     /* 0x65 - undef */
 	'\b',  /* 0x66 - backspace */
};

#endif
