; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
; definicion de rutinas de atencion de interrupciones

%include "imprimir.mac"

BITS 32

extern fin_intr_pic1
extern game_iniciar
extern game_terminar
extern game_migrar
extern game_duplicar
extern sched_proximo_indice
extern sched_remover_tarea
extern pausado
extern pausarReanudar
extern sched
extern tarea_actual
extern asignarMemoria
extern printf
extern aprintf
extern pasar_turno

;;
;; Definición de MACROS
;;


mensaje_vacio db '                                                                      ', 0

%macro limpiar_mensaje_tarea 1 	; limpiar_mensaje_tarea num_tarea
	push eax

	; Calculo fila
	mov eax, %1
	and eax, 0x000000FF
	add eax, 19

	; Limpio línea
	pushad
	push mensaje_vacio			; Mensaje
	push 0x6F 					; Atributos
	push 4 						; Columna
	push eax					; Fila
	call aprintf
	add esp, 16
	popad

	pop eax
%endmacro


%macro mensaje_tarea 2 			; mensaje_tarea num_tarea, mensaje
	push eax

	limpiar_mensaje_tarea %1

	; Calculo fila
	mov eax, %1
	and eax, 0x000000FF
	add eax, 19

	; Imprimo mensaje
	pushad
	push %2						; Mensaje
	push 0x6F 					; Atributos
	push 4 						; Columna
	push eax					; Fila
	call aprintf
	add esp, 16
	popad

	pop eax
%endmacro


%macro eliminar_tarea_actual 1
	push eax
	mov eax, %1

	; Verifico que no se trate del árbitro
	cmp ax, 5
	je %%no_eliminar_tarea_actual

	; Quito el jugador actual del scheduler
	dec ax		 				; Le resto 1 para tener el indice en tareas[]
	push eax 					; Pusheo el parámetro para borrar la tarea
	call sched_remover_tarea
	add esp, 4 					; Restauro la pila

	%%no_eliminar_tarea_actual:

	pop eax
%endmacro


%macro ISR 1
	global _isr%1

	_isr%1:
	; xchg bx, bx
	; push eax
	; mov eax, %1
	; pop eax
%endmacro


%macro ISR_GENERICO 2
	global _isr%1

	%defstr intr_num_%1 %1
	intr_msg_%1     db  %2, ' (', intr_num_%1, ')', 0
	intr_msg_len_%1 equ $ - intr_msg_%1

	_isr%1:

	; xchg bx, bx
	; push eax
	; mov eax, %1
	; pop eax

	pushfd
	pushad

	call tarea_actual 			; ax = tarea actual

	eliminar_tarea_actual eax

	; Muestro excepción en pantalla
 	mensaje_tarea eax, intr_msg_%1

	popad
	popfd

	;sti
	;int 32
	call pasar_turno
%endmacro


;;
;; Rutina de atención de las EXCEPCIONES
;;

ISR_GENERICO  0, '#DE Divide Error'
ISR_GENERICO  1, '#DB RESERVED'
ISR_GENERICO  2, 'NMI Interrupt'
ISR_GENERICO  3, '#BP Breakpoint'
ISR_GENERICO  4, '#OF Overflow'
ISR_GENERICO  5, '#BR BOUND Range Exceeded'
ISR_GENERICO  6, '#UD Invalid Opcode (Undefined Opcode)'
ISR_GENERICO  7, '#NM Device Not Available (No Math Coprocessor)'
ISR_GENERICO  8, '#DF Double Fault'
ISR_GENERICO  9, 'Coprocessor Segment Overrun (reserved)'
ISR_GENERICO 10, '#TS Invalid TSS'
ISR_GENERICO 11, '#NP Segment Not Present'
ISR_GENERICO 12, '#SS Stack-Segment Fault'
ISR_GENERICO 13, '#GP General Protection'
; ISR_GENERICO 14, '#PF Page Fault'
ISR_GENERICO 15, '(Intel reserved. Do not use.)'
ISR_GENERICO 16, '#MF x87 FPU Floating-Point Error (Math Fault)'
ISR_GENERICO 17, '#AC Alignment Check'
ISR_GENERICO 18, '#MC Machine Check'
ISR_GENERICO 19, '#XM SIMD Floating-Point Exception'
ISR_GENERICO 20, '#VE Virtualization Exception'


;Rutina de atención de Page Fault
;The contents of the CR2 register. The processor loads the CR2 register with the 32-bit linear address that
;generated the exception. The page-fault handler can use this address to locate the corresponding page
;directory and page-table entries. Another page fault can potentially occur during execution of the page-fault
;handler; the handler should save the contents of the CR2 register before a second page fault can occur.

; ISR 14
; pushfd
; pushad
; mov eax, cr2 				;Pusheo cr2 para tenerlo como parámetro
; push eax
; call asignarMemoria
; add esp, 4 ;Ver si está bien
; cmp ax, 0 ;Veo si el resultado es 0
; jne .fin14
; ; Borro la tarea
; call jugador_actual 		; Me deja en ax el jugador actual
; sub ax, 1d	 				; Le resto 1 para tener el indice en tareas[]
; push ax 					; Pusheo el parámetro para borrar la tarea
; call sched_remover_tarea
; add esp, 4 ;Restauro la pila
; ;Imprimo el mensaje correspondiente
; ;imprimir_excepcion excepcion_pf_msg, excepcion_pf_msg_len
; ;Tal vez hay que saltar directamente al sched, ver esto (tal vez se puede llamar a la int del reloj)
; .fin14:
; popad
; popfd
; iret

intr_msg_14 db '#PF Page Fault (14) - CR2: %x ', 0
intr_malloc_valido db 'Malloc valido!', 0

ISR 14
	;mov ebx, [esp] 			; Cargo el error code
	add esp, 4 ;Chau error code
	pushfd
	pushad
	; Obtengo en ax la tarea actual (valor entre 1 y 5, o 0 si no se está ejecutando ninguna.)
	call tarea_actual 			; ax = tarea actual
	mov ecx, eax ;Muevo la tarea actual
	push ecx ;Guardo tarea actual
	mov ebx, cr2				; CR2
	push ebx
	call asignarMemoria
	add esp, 4
	pop ecx
	cmp eax, 1
	mov eax, ecx ;Muevo la tarea actual
	jne .elminar14
	;No se elimina
	; pushad
	; push intr_malloc_valido		; Mensaje
	; push 0x6F 					; Atributos
	; push 45 						; Columna
	; push 20					; Fila
	; call aprintf
	; add esp, 16
	; popad
	;xchg bx, bx
	jmp .fin14
	.elminar14:

	eliminar_tarea_actual eax

	limpiar_mensaje_tarea eax

	; Calculo fila
	and eax, 0x000000FF
	add eax, 19


	; Imprimo mensaje
	pushad
	mov ebx, cr2				; CR2
	push ebx
	push intr_msg_14			; Mensaje
	push 0x6F 					; Atributos
	push 4 						; Columna
	push eax					; Fila
	call aprintf
	add esp, 20
	popad

	;popad
	;popfd
	call pasar_turno

.fin14:
	popad
	popfd
	iret




;;
;; variables del handler del RELOJ
;;

offset:      			dd 0
proximaTarea:			dw 0 

global jmpToTask

jmpToTask:
	push ebp
	mov ebp, esp
	pushad
	mov eax, [ebp+8]
	mov [proximaTarea], ax
	; xchg bx, bx
    jmp far [offset]
    popad
   	pop ebp
    ret

;;
;; Rutina de atención del RELOJ
;;

reloj_numero:		 	dd 0x00000000
reloj:  				db '|/-\'

ISR 32
	pushfd 					; guarda del valor de los flags
	pushad
	call fin_intr_pic1 		; le comunica al pic que ya se atendio la interrupción
	call proximo_reloj
	call sched			
	popad
	popfd 				; restablece el valor de los flags
	iret 				; retorna de la interrupción

proximo_reloj:
	pushad
	inc DWORD [reloj_numero]
	mov ebx, [reloj_numero]
	cmp ebx, 0x4
	jl .ok
		mov DWORD [reloj_numero], 0x0
		mov ebx, 0
	.ok:
		add ebx, reloj
		imprimir_texto_mp ebx, 1, 0x0f, 24, 79

	popad
	ret


;;
;; Rutina de atención del TECLADO
;;

soltarR_ve_msg db		'Tecla R'
soltarR_ve_msg_len equ	$ - soltarR_ve_msg

soltarP_ve_msg db		'Tecla P'
soltarP_ve_msg_len equ	$ - soltarP_ve_msg

ISR 33
pushfd
call fin_intr_pic1
push eax
in al, 0x60 ;Lectura del teclado
cmp al, 0x93 ;Veo si soltó R
jne .verTeclaP
;imprimir_excepcion soltarR_ve_msg, soltarR_ve_msg_len
;Reanudo tarea
mov byte [pausarReanudar], 1
jmp .fin33
.verTeclaP:
cmp al, 0x99 ;Veo si soltó P
jne .fin33
;imprimir_excepcion soltarP_ve_msg, soltarP_ve_msg_len
;Pauso tarea
mov byte [pausarReanudar], 0
.fin33:
pop eax
popfd
iret


;;
;; Rutinas de atención de las SYSCALLS
;;


ISR 128

pushfd 				; pushea el estado de los flags
push edi
push esi
push ebp
push esp
push ebx
push edx
push ecx

pushad
call fin_intr_pic1 	; comunica al PIC que la interrupción fue atendida
popad
; Verifico si solicita la operación 'duplicar'
cmp eax, 111
je .duplicar_128

; Verifico si solicita la operación 'migrar'
cmp eax, 222
je .migrar_128

; La operación solicitada es inválida (eax no es ni 111 ni 222). Devolvemos 0 en eax (error).
mov eax, 0
jmp .salir_128

.duplicar_128:
; Obtengo en eax el número de jugador actual

push ecx 			; col
push ebx 			; fila
call tarea_actual 
pop ebx
pop ecx


; eax = unsigned int game_duplicar(int nro_jugador, int fila, int col);
push ecx 			; col
push ebx 			; fila
push eax 			; nro_jugador
call game_duplicar 	; me devuelve en eax el valor de si se pudo llevar a cabo o no
add esp, 12

; Terminamos la syscall, con el valor de retorno en eax devuelto por game_duplicar
jmp .salir_128

.migrar_128:
; Obtengo en eax el número de jugador actual
push ecx 			; col
push ebx 			; fila
call tarea_actual 	
pop ebx				; recupero col
pop ecx				; recupero fila
; eax = unsigned int game_migrar(int nro_jugador, int fil_src, int col_src,
;                                                 int fil_dst, int col_dst);
push esi 			; col_dst
push edx 			; fila_dst
push ecx 			; col_src
push ebx 			; fila_src
push eax 			; nro_jugador
call game_migrar
add esp, 20

; Terminamos la syscall, con el valor de retorno en eax devuelto por game_duplicar

.salir_128:
cmp eax, 0
je .fin_128
pushad
call pasar_turno
popad
.fin_128:
pop ecx
pop edx
pop ebx
pop esp
pop ebp
pop esi
pop edi
popfd 				; restablece el estado de los flags
iret 				; retorna de las interrupciones







ISR 144

pushfd
push edi
push esi
push ebp
push esp
push ebx
push edx
push ecx

; Verifico si se solicita la operación 'terminar'
cmp eax, 200
je .terminar_144

; Verifico si se solicita la operación 'iniciar'
cmp eax, 300
je .iniciar_144

; La operación solicitada es inválida
jmp .salir_144

.terminar_144:
call game_terminar
jmp .salir_144

.iniciar_144:
call game_iniciar

.salir_144:
pop ecx
pop edx
pop ebx
pop esp
pop ebp
pop esi
pop edi
popfd
iret