/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#ifndef __MMU_H__
#define __MMU_H__

typedef struct str_pd_entry {
    unsigned char present:1;
    unsigned char read_write:1;
    unsigned char user_supervisor:1;
    unsigned char page_level_write_through:1;
    unsigned char page_level_cache_disable:1;
    unsigned char accessed:1;
    unsigned char ignored_6:1;
    unsigned char page_size:1;
    unsigned char global:1;
    unsigned char ignored_9_11:3;
    unsigned int  address:20;
} __attribute__((__packed__, aligned (4))) pd_entry;

typedef struct str_pt_entry {
    unsigned char present:1;
    unsigned char read_write:1;
    unsigned char user_supervisor:1;
    unsigned char page_level_write_through:1;
    unsigned char page_level_cache_disable:1;
    unsigned char accessed:1;
    unsigned char dirty:1;
    unsigned char page_table_attribute_index:1;
    unsigned char global:1;
    unsigned char ignored_9_11:3;
    unsigned int  address:20;
} __attribute__((__packed__, aligned (4))) pt_entry;


// inicializa el directorio de páginas del kernel
void mmu_inicializar_dir_kernel();

// inicializa el mmu
void mmu_inicializar();
pd_entry* directorioDeTareas[5];

// mapea direcciones de memoria
void mmu_mappear_pagina(unsigned int virtual,
                       unsigned int fisica,
                       pd_entry* page_directory,
                       unsigned char user_supervisor,
                       unsigned char read_write);

#endif	/* !__MMU_H__ */
