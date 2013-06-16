/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "defines.h"
#include "game.h"
#include "syscall.h"
#include "screen.h"

void print(const char* str, unsigned int fil, unsigned int col, unsigned short attr);

void imprimir_tablero(unsigned char * tablero);
void imprimir_puntaje(int * puntajes);
void imprimir_ganador(int * puntajes);
int  juego_terminado(unsigned char * tablero);
void actualizar_pantalla(unsigned char * tablero, int * puntajes);
void calcular_puntajes(unsigned char * tablero, int * puntajes);
void imprimir_tablero_inicial();
void screen_pintar(unsigned char formato, unsigned short desdeFil, 
	unsigned short hastaFil, unsigned short desdeCol, unsigned short hastaCol);

void task() {
	//screen_pintar_pantalla(); //Pinto la pantalla por primera vez
	/* Task 5 : Tarea arbitro */
	imprimir_tablero_inicial();
	while(1) {

	}
}

void calcular_puntajes(unsigned char * tablero, int * puntajes) {
	int f,c;
	int espaciosLibres = 0;
	for(f = 0; f < TABLERO_FILS; f++) {
		for(c = 0; c < TABLERO_COLS; c++) {
			if(tablero[f][c] == TABLERO_CELDA_VACIA) {
				espaciosLibres++;
			}
			else {
				puntajes[(int)tablero[f][c] - 1]++;
			}
		}
	}
}

void actualizar_pantalla(unsigned char * tablero, int * puntajes) {
}

int juego_terminado(unsigned char * tablero) {
	return FALSE;
}

void imprimir_ganador(int * puntajes) {
}

void imprimir_puntaje(int * puntajes) {
}

void imprimir_tablero(unsigned char * tablero) {
	int f,c;
	//int espaciosLibres = 0;
	for(f = 0; f < TABLERO_FILS; f++) {
		for(c = 0; c < TABLERO_COLS; c++) {
			if(tablero[f][c] == TABLERO_CELDA_VACIA) {
				screen_pintar(C_BG_BLACK,f,f+1,c,c+1);
			}
			else {
				screen_pintar(tablero[f][c] << 4,f,f+1,c,c+1);
			}
		}
	}
}


void print(const char* str, unsigned int fil, unsigned int col, unsigned short attr) {
}

void imprimir_tablero_inicial() {
	screen_pintar(C_BG_LIGHT_GREY,0,15,40,VIDEO_COLS);
	screen_pintar(C_BG_BROWN,15,VIDEO_FILS,0,VIDEO_COLS);
}

void screen_pintar(unsigned char formato, unsigned short desdeFil, 
	unsigned short hastaFil, unsigned short desdeCol, unsigned short hastaCol) {
	unsigned char* ptr_pantalla = (unsigned char*)VIDEO_ADDR;	
	int i,j;
	for(j = desdeFil; j < hastaFil; j++) {
		for(i = desdeCol; i < hastaCol; i++) {
			ptr_pantalla[(j*VIDEO_COLS*2) + i*2] = 0;
			ptr_pantalla[(j*VIDEO_COLS*2) + (i*2) + 1] = formato;
		}
	}
}