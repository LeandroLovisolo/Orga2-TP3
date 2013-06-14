/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/
#include "defines.h"
#include "screen.h"
#include "sched.h"

unsigned short tareas[CANT_TAREAS];
bool 		   arbitro			= false;
unsigned short posArbitro 		= 112;
unsigned short posicion 		= 0;

void sched_inicializar() {
	tareas[0] = 80;
	tareas[1] = 88;
	tareas[2] = 96;
	tareas[3] = 104;
}

unsigned short sched_proximo_indice() {
	
	unsigned short 	result;
	int 			cant = 0;

	if (!abitro) {

		result 	= posArbitro;
		arbitro = true;
	} else {

		while (tarea[posicion] == 0 && cant < 4) {

			++posicion;
			++cant;
		}

		if(cant == 4) {
			return posArbitro;
		}

		result 	= tarea[posicion];
		arbitro = false;
		++posicion;
	} 

	return result;
}

void sched_remover_tarea(unsigned int process_id) {
	tareas[process_id] = 0;
}

