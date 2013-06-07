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

tss tarea_inicial;
tss tarea_idle;
tss tarea_dibujar;

tss tsss[CANT_TAREAS];

void tss_inicializar() {

	// tss de la tarea inicial
	tarea_inicial.eip = 0;
	tarea_inicial.cr3 = 0;
	tarea_inicial.

	// tss de la tarea idle
	

	/*
	mov edi,tsss
	add edi,104*<indice>
	mov eax,cr3
	mov [edi+28],eax
	mov dword [edi+32],<eip>
	mov dword [edi+36],<flags>
	mov dword [edi+56],<pila>
	mov dword [edi+60],<pila>
	mov word [edi+72],<seg.dat>
	mov word [edi+76],<seg.cod>
	mov word [edi+80],<seg.dat>
	mov word [edi+84],<seg.dat>
	mov word [edi+88],<seg.dat>
	mov word [edi+92],<seg.dat>
	mov word [edi+102],0xFFFF
	*/
}
