/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "defines.h"
#include "game.h"
#include "syscall.h"
#include "screen.h"
#include "i386.h"

#define COLOR_V C_BG_BLACK
#define COLOR_1 C_FG_WHITE + C_BG_RED
#define COLOR_2 C_FG_WHITE + C_BG_CYAN
#define COLOR_3 C_FG_WHITE + C_BG_GREEN
#define COLOR_4 C_FG_WHITE + C_BG_BLUE
#define COLOR_A C_FG_WHITE + C_BG_MAGENTA

#define TABLERO_POS_FIL 3
#define TABLERO_POS_COL 3

void dibujar_interfaz();
void imprimir_tablero();
void imprimir_puntaje();
void imprimir_ganador();
int  juego_terminado(unsigned char * tablero);
void actualizar_pantalla();
void calcular_puntajes();

int puntaje[4];

void task() {
	dibujar_interfaz();
	syscall_iniciar();

	while(1) {
		// calcular_puntajes();
		actualizar_pantalla();
	}
}

void dibujar_interfaz() {
	rect(C_BG_BLACK,      1,  1, 25, 80);
	rect(C_BG_LIGHT_GREY, 2,  1, 19, 80);
	rect(C_BG_BROWN,      20, 1, 24, 80);
	aprintf(20, 2, COLOR_1, "1");
	aprintf(21, 2, COLOR_2, "2");
	aprintf(22, 2, COLOR_3, "3");
	aprintf(23, 2, COLOR_4, "4");
	aprintf(24, 2, COLOR_A, "A");
}

void calcular_puntajes() {
	int fil, col;

	unsigned char (*tablero)[TABLERO_COLS] = (unsigned char (*)[TABLERO_COLS]) (TABLERO_ADDR);

	breakpoint();
	printf(19, 1, "antes de iniciar array");
	breakpoint();

	puntaje[0] = 0;
	puntaje[1] = 0;
	puntaje[2] = 0;
	puntaje[3] = 0;

	printf(19, 1, "antes del for           ");
	breakpoint();

	for(fil = 0; fil < TABLERO_FILS; fil++) {
		for(col = 0; col < TABLERO_COLS; col++) {
			printf(19, 1, "antes del switch (%d, %d)        ", fil, col);
			breakpoint();			
			switch(tablero[fil][col]) {
				case JUG_1:
					puntaje[0]++;
					break;
				case JUG_2:
					puntaje[1]++;
					break;
				case JUG_3:
					puntaje[2]++;
					break;
				case JUG_4:
					puntaje[3]++;
					break;
				default:
					break;					
			}
		}
	}
}

void actualizar_pantalla() {
	imprimir_puntaje();
	imprimir_tablero();
}

int juego_terminado(unsigned char * tablero) {
	return FALSE;
}

void imprimir_ganador() {
}

void imprimir_puntaje() {
	aprintf(10, 50, COLOR_1, "Jugador 1: %d", 0); // puntajesJugadores[0]);
	aprintf(11, 50, COLOR_2, "Jugador 2: %d", 0); // puntajesJugadores[1]);
	aprintf(12, 50, COLOR_3, "Jugador 3: %d", 0); // puntajesJugadores[2]);
	aprintf(13, 50, COLOR_4, "Jugador 4: %d", 0); // puntajesJugadores[3]);
}

void imprimir_tablero() {
	int fil, col;

	unsigned char (*tablero)[TABLERO_COLS] = (unsigned char (*)[TABLERO_COLS]) (TABLERO_ADDR);

	for(fil = 0; fil < TABLERO_FILS; fil++) {
		for(col = 0; col < TABLERO_COLS; col++) {
			switch(tablero[fil][col]) {
				case JUG_1:
					rect(COLOR_1, TABLERO_POS_FIL + fil, TABLERO_POS_COL + col,
								  TABLERO_POS_FIL + fil, TABLERO_POS_COL + col);
					break;
				case JUG_2:
					rect(COLOR_2, TABLERO_POS_FIL + fil, TABLERO_POS_COL + col,
								  TABLERO_POS_FIL + fil, TABLERO_POS_COL + col);
					break;
				case JUG_3:
					rect(COLOR_3, TABLERO_POS_FIL + fil, TABLERO_POS_COL + col,
								  TABLERO_POS_FIL + fil, TABLERO_POS_COL + col);
					break;
				case JUG_4:
					rect(COLOR_4, TABLERO_POS_FIL + fil, TABLERO_POS_COL + col,
								  TABLERO_POS_FIL + fil, TABLERO_POS_COL + col);
					break;
				case TABLERO_CELDA_VACIA:
					rect(COLOR_V, TABLERO_POS_FIL + fil, TABLERO_POS_COL + col,
								  TABLERO_POS_FIL + fil, TABLERO_POS_COL + col);				
					break;					
			}
		}
	}
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