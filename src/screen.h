/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCREEN_H__
#define __SCREEN_H__


/* Definicion de la pantalla */
#define VIDEO_FILS 25
#define VIDEO_COLS 80

/* Definicion de colores */
#define C_FG_BLACK			(0x0)
#define C_FG_BLUE			(0x1)
#define C_FG_GREEN			(0x2)
#define C_FG_CYAN			(0x3)
#define C_FG_RED			(0x4)
#define C_FG_MAGENTA		(0x5)
#define C_FG_BROWN			(0x6)
#define C_FG_LIGHT_GREY		(0x7)
#define C_FG_DARK_GREY		(0x8)
#define C_FG_LIGHT_BLUE		(0x9)
#define C_FG_LIGHT_GREEN	(0xA)
#define C_FG_LIGHT_CYAN		(0xB)
#define C_FG_LIGHT_RED		(0xC)
#define C_FG_LIGHT_MAGENTA	(0xD)
#define C_FG_LIGHT_BROWN	(0xE)
#define C_FG_WHITE			(0xF)

#define C_BG_BLACK			(0x0 << 4)
#define C_BG_BLUE			(0x1 << 4)
#define C_BG_GREEN			(0x2 << 4)
#define C_BG_CYAN			(0x3 << 4)
#define C_BG_RED			(0x4 << 4)
#define C_BG_MAGENTA		(0x5 << 4)
#define C_BG_BROWN			(0x6 << 4)
#define C_BG_LIGHT_GREY		(0x7 << 4)

#define C_BLINK				(0x8 << 4)

void screen_print(const char* str, unsigned int fil, unsigned int col, unsigned short attr);
void screen_pintar(unsigned char formato, unsigned short desdeFil, unsigned short hastaFil,
                                          unsigned short desdeCol, unsigned short hastaCol);

// Formato de la familia de funciones *printf:
//
// %[flag][width][mod][conv]
// flag:   -      left justify, pad right w/ blanks
//         0      pad left w/ 0 for numerics
// width:         (field width)
// mod:    N      near ptr
//         h      short (16-bit) int
//         l      long (32-bit) int
// conv:   d,i    decimal int
//         u      decimal unsigned
//         o      octal
//         x,X    hex
//         c      char
//         s      string
//         p      ptr

void  printf(unsigned char fila, unsigned char columna, const char *fmt, ...);
void aprintf(unsigned char fila, unsigned char columna, unsigned char attr, const char *fmt, ...);
int  sprintf(char *buffer, const char *fmt, ...);

// Ejemplos de uso de sprintf:
//
// char buf[64];
// sprintf(buf, "%u score and %i years ago...\n", 4, -7);
// sprintf(buf, "-1L == 0x%lX == octal %lo\n", -1L, -1L);

#endif	/* !__SCREEN_H__ */
