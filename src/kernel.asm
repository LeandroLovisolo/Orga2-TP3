; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "imprimir.mac"

global start

%macro borrar_pantalla 0
	mov ecx, 0x7FFF
	limpar:
		mov byte [fs:ecx], 0 ;pongo la pantalla limpa con 0's
		loop limpar
%endmacro


;; GDT
extern GDT_DESC

;; IDT
extern IDT_DESC
extern idt_inicializar

;; MMU
extern mmu_inicializar
extern mmu_inicializar_dir_kernel

;; PIC
extern deshabilitar_pic
extern resetear_pic
extern habilitar_pic


;; Saltear seccion de datos
jmp start

;;
;; Seccion de datos.
;;
iniciando_mr_msg db		'Iniciando kernel (Modo Real)...'
iniciando_mr_len equ	$ - iniciando_mr_msg

bienvenida_msg db		'So I said to him, thats not a walrus, thats my wife!'
bienvenida_len equ		$ - bienvenida_msg

iniciando_mp_msg db		'Iniciando kernel (Modo Protegido)...'
iniciando_mp_len equ	$ - iniciando_mp_msg

;;
;; Seccion de código.
;;

;; Punto de entrada del kernel.
BITS 16
start:
	; Deshabilitar interrupciones
	cli

	; Imprimir mensaje de bienvenida
	; imprimir_texto_mr iniciando_mr_msg, iniciando_mr_len, 0x07, 0, 0

	; habilitar A20
	call habilitar_A20

	; cargar la GDT
	lgdt [GDT_DESC]

	; setear el bit PE del registro CR0
	mov eax, cr0
	or eax, 1
	mov cr0, eax

	; pasar a modo protegido
	jmp 0x8:modo_protegido
	BITS 32
modo_protegido:

	; acomodar los segmentos
	mov ax, 32			; Muevo al stack segment el índice 4 
	mov ss, ax 			; de la GDT relacionado a datos de
	mov ds, ax			; privilegio 0
	mov es, ax
	mov gs, ax
	mov ax, 56	
	mov fs, ax			; Declaro a fs como mi segmento de video
	
	; seteo la pila
	mov EBP, 0x00020000	
	mov ESP, 0x00020000

	; limpio la pantalla
	borrar_pantalla

	; inicializar el manejador de memoria

	; inicializar el directorio de paginas
	call mmu_inicializar_dir_kernel

	; inicializar memoria de tareas
	call mmu_inicializar

	; habilitar paginacion
	mov eax, 0x00021000 ; KERNEL_PAGE_DIR = 0x00021000
	mov cr3, eax
	mov eax, cr0
	or eax, 0x80000000 ; habilitamos paginacion
	mov cr0, eax

	; pintar pantalla, todos los colores, que bonito!
	imprimir_texto_mp bienvenida_msg, bienvenida_len, 0x06, 0, 0

	; inicializar tarea idle

	; inicializar todas las tsss

	; inicializar entradas de la gdt de tss

	; inicializar el scheduler

	; inicializar la IDT
	call idt_inicializar
	lidt [IDT_DESC]

	; Para testear divido por 0
	; mov eax, 12d
	; mov ebx, 0d
	; div ebx

	; configurar controlador de interrupciones
	call deshabilitar_pic
	call resetear_pic
	call habilitar_pic
	;sti
	; cargo la primer tarea null

	; aca salto a la primer tarea

	; Ciclar infinitamente (por si algo sale mal)
	jmp $

%include "a20.asm"
