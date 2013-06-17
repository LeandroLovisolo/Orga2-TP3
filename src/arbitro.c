/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "defines.h"
#include "game.h"
#include "syscall.h"
#include "screen.h"
#define INT_DIGITS 19		/* enough for 64 bit integer */

void print(const char* str, unsigned int fil, unsigned int col, unsigned short attr);

void imprimir_tablero();
void imprimir_puntaje(int * puntajes);
void imprimir_ganador(int * puntajes);
int  juego_terminado(unsigned char * tablero);
void actualizar_pantalla(int * puntajes);
void calcular_puntajes(int * puntajes);
void imprimir_tablero_inicial();
void screen_pintar(unsigned char formato, unsigned short desdeFil, 
	unsigned short hastaFil, unsigned short desdeCol, unsigned short hastaCol);
char *itoa(int i);

void task() {
	//screen_pintar_pantalla(); //Pinto la pantalla por primera vez
	/* Task 5 : Tarea arbitro */
	imprimir_tablero_inicial();
	syscall_iniciar();
	while(1) {
		//calcular_puntajes(puntajesJugadores);
		//actualizar_pantalla(puntajesJugadores);
	}
}

void calcular_puntajes(int * puntajes) {
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

void actualizar_pantalla(int * puntajes) {
	imprimir_puntaje(puntajes);
	//imprimir_tablero();
}

int juego_terminado(unsigned char * tablero) {
	return FALSE;
}

void imprimir_ganador(int * puntajes) {
}

void imprimir_puntaje(int * puntajes) {
	//char* puntajeJugador1 = itoa(puntajes[0]);
	//print(puntajeJugador1, 5, 16, C_FG_BLUE);
	/*
	char* puntajeJugador2 = itoa(puntajes[1]);
	//print(puntajeJugador2, 7, 16, C_FG_GREEN);
	char* puntajeJugador3 = itoa(puntajes[2]);
	//print(puntajeJugador3, 9, 16, C_FG_CYAN);
	char* puntajeJugador4 = itoa(puntajes[3]);
	//print(puntajeJugador4, 12, 16, C_FG_RED);
	puntajeJugador1++;
	puntajeJugador2++;
	puntajeJugador3++;
	puntajeJugador4++;
	*/
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

//Imprime una string bien formada
void print(const char* str, unsigned int fil, unsigned int col, unsigned short attr) {
	unsigned char* ptr_pantalla = (unsigned char*)VIDEO_ADDR;
	int i = 0;
	while(str[i] != '\0') {
		ptr_pantalla[i + col*2+ fil*VIDEO_COL*2] = str[i];
		ptr_pantalla[i + 1 + col*2+ fil*VIDEO_COL*2] = attr;
		i+=2;
	}
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



char *itoa(int i) {
  /* Room for INT_DIGITS digits, - and '\0' */
  static char buf[INT_DIGITS + 2];
  char *p = buf + INT_DIGITS + 1;	/* points to terminating '\0' */
  if (i >= 0) {
    do {
      *--p = '0' + (i % 10);
      i /= 10;
    } while (i != 0);
    return p;
  }
  else {			/* i < 0 */
    do {
      *--p = '0' - (i % 10);
      i /= 10;
    } while (i != 0);
    *--p = '-';
  }
  return p;
}