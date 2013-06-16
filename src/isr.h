/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de las rutinas de atencion de interrupciones
*/

#ifndef __ISR_H__
#define __ISR_H__
#include "sched.h"
#include "mmu.h"

void _isr0();
void _isr1();
void _isr2();
void _isr3();
void _isr4();
void _isr5();
void _isr6();
void _isr7();
void _isr8();
void _isr9();
void _isr10();
void _isr11();
void _isr12();
void _isr13();
void _isr14();
void _isr15();
void _isr16();
void _isr17();
void _isr18();
void _isr19();
void _isr20();
void _isr32();
void _isr33();
void _isr128();
void _isr144();
void jmpToTask(unsigned short tarea);

unsigned int ultimaDirfisica = 0x164000;
unsigned short cantPaginas[5];

cantPaginas[0] = 0;
cantPaginas[1] = 0;
cantPaginas[2] = 0;
cantPaginas[3] = 0;
cantPaginas[4] = 0;
/*
La idea de asignar memoria es que si piden una dirección virtual entre las del enunciado
le demos una página de memoria fisica. Cada tarea puede tener a lo sumo 5 páginas
No es necesario llevar el rastro de a que tarea se le asigna cual dirección fisica o algo similar
Ya que no se necesita liberar memoria y las direcciones que ya se mapean no vuelven a tirar PF*/
char asignarMemoria(unsigned int direccion) {
	unsigned short tarea = tareaActiva()-10; //Me da el valor del indice de la tarea
	if((direccion >= 0x003D0000) && (direccion < 0x003F0000) && (cantPaginas[tarea] <= 5)) {
		cantPaginas[tarea]++;
		mmu_mappear_pagina(direccion, ultimaDirfisica, directorioDeTareas[tarea], 0, 1);
		ultimaDirfisica += 4096; //Ver si hay que sumarle uno también
		return 1;
	}
	else {
		return 0;
	}
} 



#endif	/* !__ISR_H__ */
