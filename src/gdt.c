/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de la tabla de descriptores globales
*/
#include "defines.h"
#include "gdt.h"
#include "tss.h"

gdt_entry gdt[GDT_COUNT] = {
	/* Descriptor nulo*/
	/* Offset = 0x00 */
	[GDT_IDX_NULL_DESC] = (gdt_entry) {
		(unsigned short)	0x0000,			/* limit[0:15]  */
		(unsigned short)	0x0000,			/* base[0:15]   */
		(unsigned char)		0x00,			/* base[23:16]  */
		(unsigned char)		0x00,			/* type         */
		(unsigned char)		0x00,			/* s            */
		(unsigned char)		0x00,			/* dpl          */
		(unsigned char)		0x00,			/* p            */
		(unsigned char)		0x00,			/* limit[16:19] */
		(unsigned char)		0x00,			/* avl          */
		(unsigned char)		0x00,			/* l            */
		(unsigned char)		0x00,			/* db           */
		(unsigned char)		0x00,			/* g            */
		(unsigned char)		0x00,			/* base[31:24]  */
	},

	// Código 1, privilegio 0
	[1] = {
		//Límite: máximo offset que puede tener un byte en un segmento
		.limit_0_15  = 0xFFFF,
		//Base: dirección a partir de la cuál se tiene el segmento
		.base_0_15   = 0,
		.base_23_16  = 0,
		/*Tipo: Si s = 1,
		1° bit => Accessed: 0
		2° bit => código = 1
			3° bit => Conforming: Estos segmentos de codigo “ajustan” 
				su nivel de privilegio al del c odigo que los ha invocado.
				0 en este caso.
			4° bit => Readable: si está en 1, se puede leer
		2° bit => datos = 0
			3° bit => Expand Down: para utilizar el segmento de datos como pila
			4° bit => Writable: para poder escribir los datos
		*/
		.type        = 10, //101
		//System: s = 0 => recurso del kernel, s=1 => codigo o datos
		.s           = 1,
		//Descriptor de privilegios.
		.dpl         = 0,
		//Present: p = 1 => segmento presente en la ram
		.p           = 1,
		.limit_16_19 = 0x7,
		//Available: bit sin uso
		.avl         = 0,
		//L siempre 0, => 32 bits, L=1 y db = 0 => 64 bits de segmento
		.l           = 0,
		//Default Big: configura el tamaño del segmento, si db = 1, 32 bits
		//si db = 0, 16 bits
		.db          = 1,
		//Granularidad: establece la unidad de medida de "límite",
		//Si g = 1, el límite es multiplicado por 4096
		//Si g = 0, el límite es tal cuál está
		.g           = 1,
		.base_31_24  = 0,
	},

	// Código 2, privilegio 2
	[2] = {
		.limit_0_15  = 0xFFFF,
		.base_0_15   = 0,
		.base_23_16  = 0,
		.type        = 10,
		.s           = 1,
		.dpl         = 2,
		.p           = 1,
		.limit_16_19 = 0x7,
		.avl         = 0,
		.l           = 0,
		.db          = 1,
		.g           = 1,
		.base_31_24  = 0,
	},

	// Código 3, privilegio 3
	[3] = {
		.limit_0_15  = 0xFFFF,
		.base_0_15   = 0,
		.base_23_16  = 0,
		.type        = 10,
		.s           = 1,
		.dpl         = 3,
		.p           = 1,
		.limit_16_19 = 0x7,
		.avl         = 0,
		.l           = 0,
		.db          = 1,
		.g           = 1,
		.base_31_24  = 0,
	},

	// Datos 1, privilegio 0
	[4] = {
		.limit_0_15  = 0xFFFF,
		.base_0_15   = 0,
		.base_23_16  = 0,
		.type        = 2, //0010
		.s           = 1,
		.dpl         = 0,
		.p           = 1,
		.limit_16_19 = 0x7,
		.avl         = 0,
		.l           = 0,
		.db          = 1,
		.g           = 1,
		.base_31_24  = 0,
	},

	// Datos 2, privilegio 2
	[5] = {
		.limit_0_15  = 0xFFFF,
		.base_0_15   = 0,
		.base_23_16  = 0,
		.type        = 2,
		.s           = 1,
		.dpl         = 2,
		.p           = 1,
		.limit_16_19 = 0x7,
		.avl         = 0,
		.l           = 0,
		.db          = 1,
		.g           = 1,
		.base_31_24  = 0,
	},

	// Datos 3, privilegio 3
	[6] = {
		.limit_0_15  = 0xFFFF,
		.base_0_15   = 0,
		.base_23_16  = 0,
		.type        = 2,
		.s           = 1,
		.dpl         = 3,
		.p           = 1,
		.limit_16_19 = 0x7,
		.avl         = 0,
		.l           = 0,
		.db          = 1,
		.g           = 1,
		.base_31_24  = 0,
	},

	// Memoria en pantalla, privilegio 0
	[7] = {
		//Memoria de pantalla de 0xB8000 a 0xB8FA0
		.limit_0_15  = 0xFA0,
		.base_0_15   = 0x8000,
		.base_23_16  = 0xB,
		.type        = 2,
		.s           = 1,
		.dpl         = 3,
		.p           = 1,
		.limit_16_19 = 0,
		.avl         = 0,
		.l           = 0,
		.db          = 1,
		.g           = 0,
		.base_31_24  = 0,
	},

	// SEGEMENTOS PARA LAS TAREAS
};

gdt_descriptor GDT_DESC = {
	sizeof(gdt) - 1,
	(unsigned int) &gdt
};

void gdt_tareas_ii(){
	// Tarea inicial
	gdt[8].limit_0_15  = 0x67;
	gdt[8].base_0_15   = (unsigned int)&tss_inicial;
	gdt[8].base_23_16  = (unsigned int)&tss_inicial >> 16;
	gdt[8].type        = 9;
	gdt[8].s           = 0;
	gdt[8].dpl         = 0;
	gdt[8].p           = 1;
	gdt[8].limit_16_19 = 0;
	gdt[8].avl         = 0;
	gdt[8].l           = 0;
	gdt[8].db          = 1;
	gdt[8].g           = 0;
	gdt[8].base_31_24  = (unsigned int)&tss_inicial >> 24;

	// Tarea Idle
	gdt[9].limit_0_15  = 0x67;
	gdt[9].base_0_15   = (unsigned int)&tarea_idle;
	gdt[9].base_23_16  = (unsigned int)&tarea_idle >> 16;
	gdt[9].type        = 9;
	gdt[9].s           = 0;
	gdt[9].dpl         = 0;
	gdt[9].p           = 1;
	gdt[9].limit_16_19 = 0;
	gdt[9].avl         = 0;
	gdt[9].l           = 0;
	gdt[9].db          = 1;
	gdt[9].g           = 0;
	gdt[9].base_31_24  = (unsigned int)&tarea_idle >> 24;

	// Tarea 1 (DPLS de tareas 0)
	gdt[10].limit_0_15  = 0x67;
	gdt[10].base_0_15   = (unsigned int)&tsss[0];
	gdt[10].base_23_16  = (unsigned int)&tsss[0] >> 16;
	gdt[10].type        = 9;
	gdt[10].s           = 0;
	gdt[10].dpl         = 0;
	gdt[10].p           = 1;
	gdt[10].limit_16_19 = 0;
	gdt[10].avl         = 0;
	gdt[10].l           = 0;
	gdt[10].db          = 1;
	gdt[10].g           = 0;
	gdt[10].base_31_24  = (unsigned int)&tsss[0] >> 24;

	// Tarea 2
	gdt[11].limit_0_15  = 0x67;
	gdt[11].base_0_15   = (unsigned int)&tsss[1];
	gdt[11].base_23_16  = (unsigned int)&tsss[1] >> 16;
	gdt[11].type        = 9;
	gdt[11].s           = 0;
	gdt[11].dpl         = 0;
	gdt[11].p           = 1;
	gdt[11].limit_16_19 = 0;
	gdt[11].avl         = 0;
	gdt[11].l           = 0;
	gdt[11].db          = 1;
	gdt[11].g           = 0;
	gdt[11].base_31_24  = (unsigned int)&tsss[1] >> 24;

	// Tarea 3
	gdt[12].limit_0_15  = 0x67;
	gdt[12].base_0_15   = (unsigned int)&tsss[2];
	gdt[12].base_23_16  = (unsigned int)&tsss[2] >> 16;
	gdt[12].type        = 9;
	gdt[12].s           = 0;
	gdt[12].dpl         = 0;
	gdt[12].p           = 1;
	gdt[12].limit_16_19 = 0;
	gdt[12].avl         = 0;
	gdt[12].l           = 0;
	gdt[12].db          = 1;
	gdt[12].g           = 0;
	gdt[12].base_31_24  = (unsigned int)&tsss[2] >> 24;

	// Tarea 4
	gdt[13].limit_0_15  = 0x67;
	gdt[13].base_0_15   = (unsigned int)&tsss[3];
	gdt[13].base_23_16  = (unsigned int)&tsss[3] >> 16;
	gdt[13].type        = 9;
	gdt[13].s           = 0;
	gdt[13].dpl         = 0;
	gdt[13].p           = 1;
	gdt[13].limit_16_19 = 0;
	gdt[13].avl         = 0;
	gdt[13].l           = 0;
	gdt[13].db          = 1;
	gdt[13].g           = 0;
	gdt[13].base_31_24  = (unsigned int)&tsss[3] >> 24;

	// Tarea 5 (arbitro)
	gdt[14].limit_0_15  = 0x67;
	gdt[14].base_0_15   = (unsigned int)&tsss[4];
	gdt[14].base_23_16  = (unsigned int)&tsss[4] >> 16;
	gdt[14].type        = 9;
	gdt[14].s           = 0;
	gdt[14].dpl         = 0;
	gdt[14].p           = 1;
	gdt[14].limit_16_19 = 0;
	gdt[14].avl         = 0;
	gdt[14].l           = 0;
	gdt[14].db          = 1;
	gdt[14].g           = 0;
	gdt[14].base_31_24  = (unsigned int)&tsss[4] >> 24;
}