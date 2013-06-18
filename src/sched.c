/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/
#include "defines.h"
#include "screen.h"
#include "sched.h"
#include "isr.h"
#include "gdt.h"
#include "game.h"
//extern jmpToTask
#define COLOR_V C_BG_BLACK
#define COLOR_1 C_FG_WHITE + C_BG_RED
#define COLOR_2 C_FG_WHITE + C_BG_CYAN
#define COLOR_3 C_FG_WHITE + C_BG_GREEN
#define COLOR_4 C_FG_WHITE + C_BG_BLUE
#define COLOR_A C_FG_WHITE + C_BG_MAGENTA

unsigned short tareas[CANT_TAREAS];
char 		   arbitro			= FALSE;
unsigned short posArbitro 		= 112;
unsigned short posicion 		= 0;

char		   pausarReanudar	= 1;
char		   pausado			= 0;
char		   quantum			= 2;
char 		   contador			= 0;
char		   reloj[]			= "|/-\\";
int			   posRelojTarea[5];
void sched_inicializar() {
	tareas[0] = 80;
	tareas[1] = 88;
	tareas[2] = 96;
	tareas[3] = 104;
	posRelojTarea[0] = 0;
	posRelojTarea[1] = 0;
	posRelojTarea[2] = 0;
	posRelojTarea[3] = 0;
	posRelojTarea[4] = 0;
}

//pausado TRUE & pausarReanudar TRUE -> pausado FALSE
//pausado TRUE & pausarReanudar FALSE -> pausado TRUE
//pausado FALSE & pausarReanudar TRUE -> pausado FALSE
//pausado FALSE & pausarReanudar FALSE -> pausado TRUE

void sched() {
	if(game_terminado() == TRUE) { //Veo si se terminó el juego
		if(tarea_actual() == 0) return; //Si ya estaba antes en idle salgo
		jmpToTask(72); //Terminó juego, salto a idle
	}
	if(quantum == 0) {
		quantum = 2;
		if(pausado == 0 && pausarReanudar == 0) { // hay que pausar 
			pausado 		= 1;
			jmpToTask(72); 	// Salto a la tarea idle
		} else if((pausado == 1 && pausarReanudar == 1) || 
					(pausado == 0 && pausarReanudar == 1)) { // hay que reanudar 
			pausado = 0;
			unsigned short proxTarea = sched_proximo_indice();
			if(proxTarea != 0)	{
				jmpToTask(proxTarea); // salto a la proxima tarea
			} else { //Si no quedan tareas por ejecutar porque todas murieron
				game_terminar();
				jmpToTask(72); 	// Se terminó todo, salto a idle
			} 
		}
	}
	else {
		quantum--;
	}
	reloj_tarea();
}

unsigned short sched_proximo_indice() {
	unsigned short 	result;
	unsigned short	cant 	= 0;

	if (arbitro == FALSE) {
		result 	= posArbitro;
		arbitro = TRUE;
	} else {
		//printf(2, 50, "Posicion 1: %d", posicion);

		while (tareas[posicion] == 0 && cant < 4) {
			++posicion;
			++cant;
			if(posicion > 3) posicion = 0;
		}

		//printf(3, 50, "posicion = %u, cant = %u", posicion, cant);

		if(cant == 4) {
			return 0;
		}
		result 	= tareas[posicion];
		arbitro = FALSE;
		posicion++;
		if(posicion == 4) posicion = 0;
	} 

	//printf(5, 50, "Prox Ind: %d", result);

	return result;
}

void sched_remover_tarea(unsigned int process_id) {
	//printf(6, 50, "Borrar: %d", process_id);

	tareas[process_id] = 0;
	quantum = 0;
}

// Devuelve un numero entre 1 y 5 representando la tarea actual,
// o 0 si no se está ejecutando ninguna tarea.
unsigned short tarea_actual() {
	unsigned short tarea = 0;
	int i;
	for(i = 10; i <= 14; i++) {
		unsigned short busy = (gdt[i].type & 0x0002);
		if(busy == 2) tarea = i - 9;
	}

	//printf(7, 50, "Tarea actual: %d", tarea);

	return tarea;
}

void pasar_turno() {
	quantum = 0;
	sched();
}

void reloj_tarea() {
	char buf[2];
	buf[1] = '\0';
	unsigned short tarea = tarea_actual();
	if(tarea == 0) return;
	posRelojTarea[tarea-1]++;
	if(posRelojTarea[tarea-1] == 4) posRelojTarea[tarea-1] = 0;
	buf[0] = reloj[posRelojTarea[tarea-1]];
	switch(tarea) {
		case 1:
			aprintf(20, 1, COLOR_1, buf);
			break;
		case 2:
			aprintf(21, 1, COLOR_2, buf);
			break;
		case 3:
			aprintf(22, 1, COLOR_3, buf);
			break;
		case 4:
			aprintf(23, 1, COLOR_4, buf);
			break;
		case 5:
			aprintf(24, 1, COLOR_A, buf);
			break;
	}
	printf(15, 50, "Reloj tarea actual: %d", tarea);
}