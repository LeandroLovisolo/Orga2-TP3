/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/
#include "defines.h"
#include "mmu.h"
#include "i386.h"

// Declaración de funciones
void inicializar_kernel_page_directory();
void inicializar_kernel_table_directory();

void mmu_inicializar_dir_kernel() {
	inicializar_kernel_page_directory();
	inicializar_kernel_table_directory();
}

void mmu_inicializar_tarea_arbitro();

// Implementación de funciones
void inicializar_kernel_page_directory() {
	pd_entry* page_directory = (pd_entry*) KERNEL_PAGE_DIR;
	int i;
	for (i = 0; i < 1024; i++) {
		*((unsigned int*) &page_directory[i]) = 0;
	}
	page_directory[0].user_supervisor = 0;
	page_directory[0].address = KERNEL_PAGE_TABLE >> 12;
	page_directory[0].read_write = 1;
	page_directory[0].present = 1;
}

void inicializar_kernel_table_directory() {
	pt_entry* page_table = (pt_entry*) KERNEL_PAGE_TABLE;
	int i;
	for (i = 0; i < 1024; i++) {
		*((unsigned int*) &(page_table[i])) = 0;
	}
	for (i = 0; i < 0x164; i++) {
		page_table[i].user_supervisor = 0;
		page_table[i].address = i;
		page_table[i].read_write = 1;
		page_table[i].present = 1;
	}
}

void mmu_inicializar_tarea_jugador(
	pd_entry* page_directory,
	pt_entry* page_table,
	unsigned int code_address,
	unsigned int stack_address);

void mmu_inicializar() {
	// Jugador 1
    mmu_inicializar_tarea_jugador(
    	(pd_entry*) TASK_1_PAGE_DIR,
    	(pt_entry*) TASK_1_PAGE_TABLE,
    	TASK_1_CODE_PA,
    	TASK_1_STACK_PA);

	// Jugador 2
    mmu_inicializar_tarea_jugador(
    	(pd_entry*) TASK_2_PAGE_DIR,
    	(pt_entry*) TASK_2_PAGE_TABLE,
    	TASK_2_CODE_PA,
    	TASK_2_STACK_PA);

	// Jugador 3
    mmu_inicializar_tarea_jugador(
    	(pd_entry*) TASK_3_PAGE_DIR,
    	(pt_entry*) TASK_3_PAGE_TABLE,
    	TASK_3_CODE_PA,
    	TASK_3_STACK_PA);

	// Jugador 4
    mmu_inicializar_tarea_jugador(
    	(pd_entry*) TASK_4_PAGE_DIR,
    	(pt_entry*) TASK_4_PAGE_TABLE,
    	TASK_4_CODE_PA,
    	TASK_4_STACK_PA);

    // Árbitro
    mmu_inicializar_tarea_arbitro();
}

void mmu_inicializar_tarea_jugador(
		pd_entry* page_directory,
		pt_entry* page_table,
		unsigned int code_address,
		unsigned int stack_address) {
	int i;

	// Creo el directorio con todas las entradas vacías
	for (i = 0; i < 1024; i++) {
		*((unsigned int*) &page_directory[i]) = 0;
	}
	page_directory[0].user_supervisor = 1;
	page_directory[0].address = ((unsigned int) page_table) >> 12;
	page_directory[0].read_write = 1;
	page_directory[0].present = 1;

	// Creo una única entrada en el directorio apuntando a la tabla
	for (i = 0; i < 1024; i++) {
		*((unsigned int*) &(page_table[i])) = 0;
	}

	// Task code
	// Dirección virtual: 0x003A0000
	// Directory    | Table        | Offset
	// 0000 0000 00 | 11 1010 0000 | 0000 0000 0000 | Binary 
	//            0 |          928 |              0 | Decimal
	page_table[928].user_supervisor = 1;
	page_table[928].address = code_address >> 12;
	page_table[928].read_write = 1;
	page_table[928].present = 1;

	// Stack code
	// Dirección virtual: 0x003B0000
	// Directory    | Table        | Offset
	// 0000 0000 00 | 11 1011 0000 | 0000 0000 0000 | Binary 
	//            0 |          944 |              0 | Decimal
	page_table[944].user_supervisor = 1;
	page_table[944].address = stack_address >> 12;
	page_table[944].read_write = 1;
	page_table[944].present = 1;

	// Tablero
	// Dirección virtual: 0x003C0000
	// Directory    | Table        | Offset
	// 0000 0000 00 | 11 1100 0000 | 0000 0000 0000 | Binary 
	//            0 |          960 |              0 | Decimal
	page_table[960].user_supervisor = 1;
	page_table[960].address = TABLERO_ADDR_PA >> 12;
	page_table[960].read_write = 0;
	page_table[960].present = 1;	
}

void mmu_inicializar_tarea_arbitro() {
    mmu_inicializar_tarea_jugador(
    	(pd_entry*) TASK_5_PAGE_DIR,
    	(pt_entry*) TASK_5_PAGE_TABLE,
    	TASK_5_CODE_PA,
    	TASK_5_STACK_PA);

    pt_entry* page_table = (pt_entry*) TASK_5_PAGE_TABLE;

	// Memoria de video con identity mapping
	// Dirección virtual: 0x000B8000
	// Directory    | Table        | Offset
	// 0000 0000 00 | 00 1011 1000 | 0000 0000 0000 | Binary 
	//            0 |          184 |              0 | Decimal
	page_table[184].user_supervisor = 1;
	page_table[184].address = VIDEO_ADDR >> 12;
	page_table[184].read_write = 1;
	page_table[184].present = 1;	
}