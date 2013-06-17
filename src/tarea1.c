/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "defines.h"
#include "game.h"
#include "syscall.h"

void task() {
	while(1) {
        // Ejecuta algunos movimientos inicialmente válidos
        syscall_duplicar(1, 1);
        syscall_duplicar(2, 2);
        syscall_duplicar(3, 3);

        // Ejecuta un movimiento inválido		
        syscall_duplicar(10, 10);

        // Los siguientes movimientos nunca se deberían ejecutar
        syscall_duplicar(4, 4);
        syscall_duplicar(5, 5);
        syscall_duplicar(6, 6);        
	};
}
