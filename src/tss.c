/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "defines.h"
#include "gdt.h"
#include "tss.h"
#include "i386.h"

#define SEGMENT_SELECTOR(index, rpl) ((index << 3) | rpl)
#define SEG_CODIGO_0 SEGMENT_SELECTOR(1, 0)
#define SEG_CODIGO_2 SEGMENT_SELECTOR(2, 2)
#define SEG_CODIGO_3 SEGMENT_SELECTOR(3, 3)
#define SEG_DATOS_0  SEGMENT_SELECTOR(4, 0)
#define SEG_DATOS_2  SEGMENT_SELECTOR(5, 2)
#define SEG_DATOS_3  SEGMENT_SELECTOR(6, 3)


tss tss_inicial;
tss tarea_idle;
tss tarea_dibujar;

tss tsss[CANT_TAREAS];

void tss_inicializar() {
	// tss de la tarea idle
	tarea_idle.esp 		= 0x0003F000;
	tarea_idle.ebp 		= 0x0003F000;
	tarea_idle.eip  	= TASK_CODE;
	tarea_idle.cr3 		= KERNEL_PAGE_DIR;
	tarea_idle.es 		= SEG_DATOS_0;
	tarea_idle.cs 		= SEG_CODIGO_0;
	tarea_idle.ss 		= SEG_DATOS_0;
	tarea_idle.ds 		= SEG_DATOS_0;
	tarea_idle.fs 		= SEG_DATOS_0;
	tarea_idle.gs 		= SEG_DATOS_0;
	tarea_idle.eflags 	= 0x00000202;
	tarea_idle.iomap 	= 0xFFFF;
	tarea_idle.esp0     = TASK_IDLE_STACK_RING_0;

	//Tarea 1
	tsss[0].esp 	= TASK_STACK + 0x1000;
	tsss[0].ebp 	= TASK_STACK + 0x1000;
	tsss[0].eip    	= TASK_CODE;
	tsss[0].cr3 	= TASK_1_PAGE_DIR;
	tsss[0].es 		= SEG_DATOS_3;
	tsss[0].cs 		= SEG_CODIGO_3;
	tsss[0].ss 		= SEG_DATOS_3;
	tsss[0].ds 		= SEG_DATOS_3;
	tsss[0].fs 		= SEG_DATOS_3;
	tsss[0].gs 		= SEG_DATOS_3;
	tsss[0].eflags 	= 0x00000202;
	tsss[0].iomap 	= 0xFFFF;
	tsss[1].esp0 	= TASK_1_STACK_RING_0;

	//Tarea 2
	tsss[1].esp 	= TASK_STACK + 0x1000;
	tsss[1].ebp 	= TASK_STACK + 0x1000;
	tsss[1].eip    	= TASK_CODE;
	tsss[1].cr3 	= TASK_2_PAGE_DIR;
	tsss[1].es 		= SEG_DATOS_3;
	tsss[1].cs 		= SEG_CODIGO_3;
	tsss[1].ss 		= SEG_DATOS_3;
	tsss[1].ds 		= SEG_DATOS_3;
	tsss[1].fs 		= SEG_DATOS_3;
	tsss[1].gs 		= SEG_DATOS_3;
	tsss[1].eflags 	= 0x00000202;
	tsss[1].iomap 	= 0xFFFF;
	tsss[1].esp0 	= TASK_2_STACK_RING_0;

	//Tarea 3
	tsss[2].esp 	= TASK_STACK + 0x1000;
	tsss[2].ebp 	= TASK_STACK + 0x1000;
	tsss[2].eip    	= TASK_CODE;
	tsss[2].cr3 	= TASK_3_PAGE_DIR;
	tsss[2].es 		= SEG_DATOS_3;
	tsss[2].cs 		= SEG_CODIGO_3;
	tsss[2].ss 		= SEG_DATOS_3;
	tsss[2].ds 		= SEG_DATOS_3;
	tsss[2].fs 		= SEG_DATOS_3;
	tsss[2].gs 		= SEG_DATOS_3;
	tsss[2].eflags 	= 0x00000202;
	tsss[2].iomap 	= 0xFFFF;
	tsss[2].esp0 	= TASK_3_STACK_RING_0;

	//Tarea 4
	tsss[3].esp 	= TASK_STACK + 0x1000;
	tsss[3].ebp 	= TASK_STACK + 0x1000;
	tsss[3].eip    	= TASK_CODE;
	tsss[3].cr3 	= TASK_4_PAGE_DIR;
	tsss[3].es 		= SEG_DATOS_3;
	tsss[3].cs 		= SEG_CODIGO_3;
	tsss[3].ss 		= SEG_DATOS_3;
	tsss[3].ds 		= SEG_DATOS_3;
	tsss[3].fs 		= SEG_DATOS_3;
	tsss[3].gs 		= SEG_DATOS_3;
	tsss[3].eflags 	= 0x00000202;
	tsss[3].iomap 	= 0xFFFF;
	tsss[3].esp0 	= TASK_4_STACK_RING_0;

	//Tarea 5 (Arbitro)
	tsss[4].esp 	= TASK_STACK + 0x1000;
	tsss[4].ebp 	= TASK_STACK + 0x1000;
	tsss[4].eip    	= TASK_CODE;
	tsss[4].cr3 	= TASK_5_PAGE_DIR;
	tsss[4].es 		= SEG_DATOS_2; 	// no lo usamos
	tsss[4].cs 		= SEG_CODIGO_2; 	// segmento de codigo privilegio 2
	tsss[4].ss 		= SEG_DATOS_2; 	// segmento de datos privilegio 2
	tsss[4].ds 		= SEG_DATOS_2; 	// segmento de datos privilegio 2
	tsss[4].fs 		= SEG_DATOS_2; 	// segmento de video
	tsss[4].gs 		= SEG_DATOS_2; 	// no lo usamos
	tsss[4].eflags 	= 0x00000202;
	tsss[4].iomap 	= 0xFFFF;
	tsss[4].esp0 	= TASK_5_STACK_RING_0;
}
