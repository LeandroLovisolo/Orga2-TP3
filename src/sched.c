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

//extern jmpToTask

unsigned short tareas[CANT_TAREAS];
char 		   arbitro			= FALSE;
unsigned short posArbitro 		= 112;
unsigned short posicion 		= 0;

char		   pausarReanudar	= 0;
char		   pausado			= 0;
char		   quantum			= 2;
char		   finalizado		= 0;
char 		   contador			= 0;

void sched_inicializar() {
	tareas[0] = 80;
	tareas[1] = 88;
	tareas[2] = 96;
	tareas[3] = 104;
}

void sched() {
	if(finalizado == 1) return;
	if(quantum == 0) {
		quantum = 2;
		if(pausado == 0 && pausarReanudar == 1) {
			pausado = 1;
			jmpToTask(72); //Salto a la tarea idle
			return;
		}
		else if(pausado == 1 && pausarReanudar == 0) {
			pausado = 0;
		}
		unsigned short proxTarea = sched_proximo_indice();
		if(proxTarea != -1) { //Si hay tareas activas
			if(contador != 1) { //Esto es solamente por ahora porque tenemos una sola tarea corriendo
				contador = 1;
				jmpToTask(proxTarea);
			}
		}
		else {
			finalizado = 1;
			jmpToTask(72); //Salto a la tarea idle
		}
	}
	quantum--;
	return;
}

unsigned short sched_proximo_indice() {
	/* Implementación alternativa: Ejecutar sólo la tarea 1 */
	return posArbitro;

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

