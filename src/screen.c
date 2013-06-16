/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/
#include "defines.h"
#include "i386.h"
#include "screen.h"

void screen_pintar(unsigned char* mensaje, unsigned char formato, 
	unsigned short longitud, unsigned short fil, unsigned short col) {
	unsigned char* ptr_pantalla = (unsigned char*)VIDEO_ADDR;	
	int i;
	//escribo desde la fil y col especificada
	for(i = fil*col;i<longitud;i+=2) {
		ptr_pantalla[i] = mensaje[i*2-fil*col];
		ptr_pantalla[i+1] = formato;
	}
}

void screen_pintar_pantalla() {
}

