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
char 		   arbitro			= FALSE;
unsigned short posArbitro 		= 112;
unsigned short posicion 		= 0;

void sched_inicializar() {
	tareas[0] = 80;
	tareas[1] = 88;
	tareas[2] = 96;
	tareas[3] = 104;
}

unsigned short sched_proximo_indice() {
	/* Implementación alternativa: Ejecutar sólo la tarea 1 */
	return 80;

	/* Implementación alternativa: Excluye tarea árbitro
	posicion++;
	if(posicion >= 4) posicion = 0;
	return tareas[posicion];
	*/

	/* Implementación real
	unsigned short result;
	int cant = 0;

	if (!arbitro) {
		result = posArbitro;
		arbitro = TRUE;
	} else {
		while (tareas[posicion] == 0 && cant < 4) {
			++posicion;
			++cant;
		}

		if(cant == 4) {
			return posArbitro;
		}

		result 	= tareas[posicion];
		arbitro = FALSE;
		++posicion;
	} 

	return result;
	*/
}

void sched_remover_tarea(unsigned int process_id) {
	tareas[process_id] = 0;
}

