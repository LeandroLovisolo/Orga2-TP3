; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "imprimir.mac"

global start


;; GDT
extern GDT_DESC

;; IDT
extern IDT_DESC
extern idt_inicializar

;; PIC
extern resetear_pic
extern habilitar_pic


;; Saltear seccion de datos
jmp start

;;
;; Seccion de datos.
;;
iniciando_mr_msg db		'Iniciando kernel (Modo Real)...'
iniciando_mr_len equ	$ - iniciando_mr_msg

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
	imprimir_texto_mr iniciando_mr_msg, iniciando_mr_len, 0x07, 0, 0

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
	;Muevo al stack segment el índice 4 de la GDT
	;relacionado a datos de privilegio 0
	mov ax, 32
	mov ss, ax 
	mov ds, ax
	mov es, ax
	mov gs, ax
	mov fs, ax
	; seteo la pila
	mov EBP, 0x20000
	mov ESP, 0x20000
	; pintar pantalla, todos los colores, que bonito!

	; inicializar el manejador de memoria

	; inicializar el directorio de paginas

	; inicializar memoria de tareas

	; habilitar paginacion

	; inicializar tarea idle

	; inicializar todas las tsss

	; inicializar entradas de la gdt de tss

	; inicializar el scheduler

	; inicializar la IDT

	; configurar controlador de interrupciones

	; cargo la primer tarea null

	; aca salto a la primer tarea

	; Ciclar infinitamente (por si algo sale mal)
	jmp $

%include "a20.asm"
