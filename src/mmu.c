/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/
#include "defines.h"
#include "mmu.h"
#include "i386.h"
#include "sched.h"
#include "screen.h"

void inicializar_kernel_page_directory();
void inicializar_kernel_table_directory();
void mmu_mappear_pagina(unsigned int virtual,
                       unsigned int fisica,
                       pd_entry* page_directory,
                       unsigned char user_supervisor,
                       unsigned char read_write);
void mmu_inicializar_tarea_idle();

void mmu_inicializar_dir_kernel() {
    inicializar_kernel_page_directory();
    inicializar_kernel_table_directory();
    mmu_inicializar_tarea_idle();
}

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

void mmu_inicializar_tarea_idle() {
    mmu_mappear_pagina(TASK_CODE, 0x00010000, (pd_entry*) KERNEL_PAGE_DIR, 0, 1);
}

void mmu_inicializar_tarea_arbitro();

void mmu_inicializar_tarea_jugador(
    pd_entry* page_directory,
    pt_entry* page_table,
    unsigned int code_address,
    unsigned int stack_address);

void mmu_inicializar() {
    directorioDeTareas[0] = (pd_entry*)TASK_1_PAGE_DIR;
    directorioDeTareas[1] = (pd_entry*)TASK_2_PAGE_DIR;
    directorioDeTareas[2] = (pd_entry*)TASK_3_PAGE_DIR;
    directorioDeTareas[3] = (pd_entry*)TASK_4_PAGE_DIR;
    directorioDeTareas[4] = (pd_entry*)TASK_5_PAGE_DIR;
    ultimaDirfisica = 0x164000;
    cantPaginas[0] = 0;
    cantPaginas[1] = 0;
    cantPaginas[2] = 0;
    cantPaginas[3] = 0;
    cantPaginas[4] = 0;
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

void mmu_mappear_pagina(unsigned int virtual,
                       unsigned int fisica,
                       pd_entry* page_directory,
                       unsigned char user_supervisor,
                       unsigned char read_write) {
    
    // Dirección virtual: 
    //
    // | Directorio   | Table        | Offset         |
    // | XXXX XXXX XX | XX XXXX XXXX | XXXX XXXX XXXX |
    // | 31        22 | 21        12 | 11           0 |

    unsigned int directory = (virtual >> 22) & 0x3FF;
    unsigned int table     = (virtual & 0x003FF000) >> 12;

    pt_entry* page_table = (pt_entry*) (page_directory[directory].address << 12);

    page_table[table].user_supervisor = user_supervisor;
    page_table[table].address = fisica >> 12;
    page_table[table].read_write = read_write;
    page_table[table].present = 1;
}

void mmu_inicializar_tarea_jugador(
        pd_entry* page_directory,
        pt_entry* page_table,
        unsigned int code_address,
        unsigned int stack_address) {
    int i;

    // Inicializo el directorio con todas las entradas vacías
    for (i = 0; i < 1024; i++) {
        *((unsigned int*) &page_directory[i]) = 0;
    }

    // Asigno una única entrada en el directorio apuntando a la tabla
    page_directory[0].user_supervisor = 1;
    page_directory[0].address = ((unsigned int) page_table) >> 12;
    page_directory[0].read_write = 1;
    page_directory[0].present = 1;

    // Inicializo la tabla con todas las entradas vacías
    for (i = 0; i < 1024; i++) {
        *((unsigned int*) &(page_table[i])) = 0;
    }

    // Mappeo memoria del kernel
    for (i = 0; i < 0x164; i++) {
        page_table[i].user_supervisor = 0;
        page_table[i].address = i;
        page_table[i].read_write = 0;
        page_table[i].present = 1;
    }
    
    // Mappeo la memoria
    mmu_mappear_pagina(TASK_CODE,    code_address, page_directory,    1, 1);
    mmu_mappear_pagina(TASK_STACK,   stack_address, page_directory,   1, 1);
    mmu_mappear_pagina(TABLERO_ADDR, TABLERO_ADDR_PA, page_directory, 1, 0);
}

void mmu_inicializar_tarea_arbitro() {
    mmu_inicializar_tarea_jugador(
        (pd_entry*) TASK_5_PAGE_DIR,
        (pt_entry*) TASK_5_PAGE_TABLE,
        TASK_5_CODE_PA,
        TASK_5_STACK_PA);

    mmu_mappear_pagina(VIDEO_ADDR, VIDEO_ADDR, (pd_entry*) TASK_5_PAGE_DIR, 1, 1);

}

/*
La idea de asignar memoria es que si piden una dirección virtual entre las del enunciado
le demos una página de memoria fisica. Cada tarea puede tener a lo sumo 5 páginas
No es necesario llevar el rastro de a que tarea se le asigna cual dirección fisica o algo similar
Ya que no se necesita liberar memoria y las direcciones que ya se mapean no vuelven a tirar PF*/
char asignarMemoria(unsigned int direccion) {
    unsigned short tarea = tarea_actual() - 1; //Me da el valor del indice de la tarea
    //breakpoint();
    //printf(15, 45, "Tarea %u -> dir %x veces %u", tarea, direccion, cantPaginas[tarea]);
    if((direccion >= 0x003D0000) && (direccion < 0x003F0000) && (cantPaginas[tarea] <= 5)) {
        //printf(16, 45, "Pasa el if");
        cantPaginas[tarea]++;
        //printf(17, 45, "Incremento contador");
        mmu_mappear_pagina(direccion, ultimaDirfisica, (pd_entry*)directorioDeTareas[tarea], 1, 1);
        //printf(18, 45, "Hago el mapeo");
        ultimaDirfisica += 4096; //Ver si hay que sumarle uno también
        //printf(19, 45, "Incremento dir");
        //breakpoint();
        return 1;
    }
    else {
        return 0;
    }
} 