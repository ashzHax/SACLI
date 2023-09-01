// By: ashz
// Ex: terminial text output with colors, using ANSI escape code

#ifndef __AXC_COLOR__
#define __AXC_COLOR__

#include <stdio.h>

// General
#define ESC_START       "\033["
#define ESC_END   "m"

#define _RESET     ESC_START"0"ESC_END
#define _BOLD      ESC_START"1"ESC_END
#define _DIM       ESC_START"2"ESC_END
#define _UNDERLINE ESC_START"4"ESC_END
#define _BLINK     ESC_START"5"ESC_END
#define _INVERTED  ESC_START"7"ESC_END
#define _HIDDEN    ESC_START"8"ESC_END

//////////////////// 4-Bit ////////////////////

/*
 * Color Types
 * 4 Bit =  8(Colors) * 2(Brightness) * 2(FG/BG)
 * 8 Bit =  [ 8(Colors) * 2(Brightness) * 2(FG/BG) ] + 6^3(Color Cube) + 24(Gray Scale)
 * 24 Bit = RGB
 */

/*
#define _F_BLACK  ESC_START"30"ESC_END
#define _F_RED    ESC_START"31"ESC_END
#define _F_GREEN  ESC_START"32"ESC_END
#define _F_YELLOW ESC_START"33"ESC_END
#define _F_BLUE   ESC_START"34"ESC_END
#define _F_PURPLE ESC_START"35"ESC_END
#define _F_CYAN   ESC_START"36"ESC_END
#define _F_WHITE  ESC_START"37"ESC_END

#define _F_BRIGHT_BLACK  ESC_START"90"ESC_END
#define _F_BRIGHT_RED    ESC_START"91"ESC_END
#define _F_BRIGHT_GREEN  ESC_START"92"ESC_END
#define _F_BRIGHT_YELLOW ESC_START"93"ESC_END
#define _F_BRIGHT_BLUE   ESC_START"94"ESC_END
#define _F_BRIGHT_PURPLE ESC_START"95"ESC_END
#define _F_BRIGHT_CYAN   ESC_START"96"ESC_END
#define _F_BRIGHT_WHITE  ESC_START"97"ESC_END

#define _B_BLACK  ESC_START"40"ESC_END
#define _B_RED    ESC_START"41"ESC_END
#define _B_GREEN  ESC_START"42"ESC_END
#define _B_YELLOW ESC_START"43"ESC_END
#define _B_BLUE   ESC_START"44"ESC_END
#define _B_PURPLE ESC_START"45"ESC_END
#define _B_CYAN   ESC_START"46"ESC_END
#define _B_WHITE  ESC_START"47"ESC_END

#define _B_BRIGHT_BLACK  ESC_START"100"ESC_END
#define _B_BRIGHT_RED    ESC_START"101"ESC_END
#define _B_BRIGHT_GREEN  ESC_START"102"ESC_END
#define _B_BRIGHT_YELLOW ESC_START"103"ESC_END
#define _B_BRIGHT_BLUE   ESC_START"104"ESC_END
#define _B_BRIGHT_PURPLE ESC_START"105"ESC_END
#define _B_BRIGHT_CYAN   ESC_START"106"ESC_END
#define _B_BRIGHT_WHITE  ESC_START"107"ESC_END
*/

//////////////////// 4-Bit ////////////////////

//////////////////// 8-Bit ////////////////////

#define FG "38"
#define BG "48"

/* 
	Default Colors (0~15)
	8 * 2 * 2 
*/

#define	_F_BLACK         ESC_START""FG";5;0"ESC_END
#define	_F_RED           ESC_START""FG";5;1"ESC_END
#define	_F_GREEN         ESC_START""FG";5;2"ESC_END
#define	_F_YELLOW        ESC_START""FG";5;3"ESC_END
#define	_F_BLUE          ESC_START""FG";5;4"ESC_END
#define	_F_PURPLE        ESC_START""FG";5;5"ESC_END
#define	_F_CYAN          ESC_START""FG";5;6"ESC_END
#define	_F_WHITE         ESC_START""FG";5;7"ESC_END
#define	_F_BRIGHT_BLACK  ESC_START""FG";5;8"ESC_END
#define	_F_BRIGHT_RED    ESC_START""FG";5;9"ESC_END
#define	_F_BRIGHT_GREEN  ESC_START""FG";5;10"ESC_END
#define	_F_BRIGHT_YELLOW ESC_START""FG";5;11"ESC_END
#define	_F_BRIGHT_BLUE   ESC_START""FG";5;12"ESC_END
#define	_F_BRIGHT_PURPLE ESC_START""FG";5;13"ESC_END
#define	_F_BRIGHT_CYAN   ESC_START""FG";5;14"ESC_END
#define	_F_BRIGHT_WHITE  ESC_START""FG";5;15"ESC_END

#define	_B_BLACK         ESC_START""FG";5;0"ESC_END
#define	_B_RED           ESC_START""FG";5;1"ESC_END
#define	_B_GREEN         ESC_START""FG";5;2"ESC_END
#define	_B_YELLOW        ESC_START""FG";5;3"ESC_END
#define	_B_BLUE          ESC_START""FG";5;4"ESC_END
#define	_B_PURPLE        ESC_START""FG";5;5"ESC_END
#define	_B_CYAN          ESC_START""FG";5;6"ESC_END
#define	_B_WHITE         ESC_START""FG";5;7"ESC_END
#define	_B_BRIGHT_BLACK  ESC_START""FG";5;8"ESC_END
#define	_B_BRIGHT_RED    ESC_START""FG";5;9"ESC_END
#define	_B_BRIGHT_GREEN  ESC_START""FG";5;10"ESC_END
#define	_B_BRIGHT_YELLOW ESC_START""FG";5;11"ESC_END
#define	_B_BRIGHT_BLUE   ESC_START""FG";5;12"ESC_END
#define	_B_BRIGHT_PURPLE ESC_START""FG";5;13"ESC_END
#define	_B_BRIGHT_CYAN   ESC_START""FG";5;14"ESC_END
#define	_B_BRIGHT_WHITE  ESC_START""FG";5;15"ESC_END

/* 
	216 Colors (RGB) (16~231)
	RGB([FG/BG], {0~5}, {0~5}, {0~5});
	16 + 36 * r + 6 * g + b
*/

/*
static inline void rgb(const char *mode, short red, short green, short blue)
{
	printf("%s%s;5;%d%s", ESC_START, mode, (16+36*red+6*green+blue), ESC_END);
}
*/

/* 
	Grayscale Colors (232 ~ 255)
	GRAY([FG/BG], {0~24});
	Black(0) -> Bright Gray(23)
*/

#define GRAY_SCALE 232
static inline void gray(const char *mode, short brightness)
{
	printf("%s%s;5;%d%s", ESC_START, mode, GRAY_SCALE+(brightness%24), ESC_END);
}

//////////////////// 8-Bit ////////////////////

//////////////////// 24-Bit ////////////////////

/* 
	255^3 Colors (RGB) (16~231)
	RGB([FG/BG], {0~5}, {0~5}, {0~5});
	16 + 36 * r + 6 * g + b
*/

static inline void rgb(const char *mode, short red, short green, short blue)
{
	printf("%s%s;5;%d%s", ESC_START, mode, (16+36*red+6*green+blue), ESC_END);
}

//////////////////// 24-Bit ////////////////////

#endif /* __AXC_COLOR__ */
