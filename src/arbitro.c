/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "defines.h"
#include "game.h"
#include "syscall.h"
#include "screen.h"

void imprimir_tablero();
void imprimir_puntaje();
void imprimir_ganador();
int  juego_terminado(unsigned char * tablero);
void actualizar_pantalla();
void calcular_puntajes();
void imprimir_tablero_inicial();

void task() {
	imprimir_tablero_inicial();
	syscall_iniciar();

	while(1) {
		calcular_puntajes();
		actualizar_pantalla();
	}
}

void calcular_puntajes() {
	/*
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
	*/
}

void actualizar_pantalla() {
	imprimir_puntaje();
	//imprimir_tablero();
}

int juego_terminado(unsigned char * tablero) {
	return FALSE;
}

void imprimir_ganador() {
}

void imprimir_puntaje() {
	aprintf(10, 50, C_FG_LIGHT_BLUE,  "Jugador 1: %d", 0); // puntajesJugadores[0]);
	aprintf(11, 50, C_FG_LIGHT_GREEN, "Jugador 2: %d", 0); // puntajesJugadores[1]);
	aprintf(12, 50, C_FG_LIGHT_CYAN,  "Jugador 3: %d", 0); // puntajesJugadores[2]);
	aprintf(13, 50, C_FG_LIGHT_RED,   "Jugador 4: %d", 0); // puntajesJugadores[3]);
}

void imprimir_tablero() {
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

void imprimir_tablero_inicial() {
	screen_pintar(C_BG_LIGHT_GREY,0,15,40,VIDEO_COLS);
	screen_pintar(C_BG_BROWN,15,VIDEO_FILS,0,VIDEO_COLS);
}

// El árbitro se compila como un binario independiente del kernel, linkeado
// estáticamente. Para poder usar código objeto de otras unidades de
// compilación, hace falta linkear task5.ctsk con el archivo con el código
// objeto correspondiente. Esto implica modificar el Makefile provisto por la
// cátedra.
//
// En lugar de modificar el Makefile, optamos por incluir el código necesario
// dentro de esta misma unidad de compilación.

#include "screen.c"