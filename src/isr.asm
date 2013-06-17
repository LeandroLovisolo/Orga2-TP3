; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
; definicion de rutinas de atencion de interrupciones

%include "imprimir.mac"

borrar_pantalla:
	mov ecx, 0x7FFF
	push ecx
	.limpiar:
		mov byte [fs:ecx], 0 ;pongo la pantalla limpa con 0's
		loop .limpiar
	pop ecx
	ret

%macro imprimir_excepcion 2
	;Primer parámetro, puntero a mensaje
	;Segundo parámetros, longitud de mensaje
	call borrar_pantalla
	imprimir_texto_mp excepcion_msg, excepcion_msg_len ,0x07, 0, 0
	imprimir_texto_mp %1, %2, 0x06, 1 ,0
%endmacro

BITS 32

extern game_iniciar
extern game_terminar
extern game_migrar
extern game_duplicar
extern sched_proximo_indice
extern sched_remover_tarea

;;
;;Extern de tareas
;;
extern pausado
extern pausarReanudar
extern sched
extern jugador_actual
extern asignarMemoria


%define TAREA_QUANTUM		2
excepcion_msg db		'Ch, ch, que andas haciendo? Toma una excepcion pebete!'
excepcion_msg_len equ	$ - excepcion_msg

excepcion_division_msg db		'Exepcion, division por 0'
excepcion_division_msg_len equ	$ - excepcion_division_msg

excepcion_debug_msg db		'Exepcion de debug'
excepcion_debug_msg_len equ	$ - excepcion_debug_msg

excepcion_nmi_msg db		'Exepcion NMI'
excepcion_nmi_msg_len equ	$ - excepcion_nmi_msg

excepcion_bp_msg db			'Breakpoint Exception'
excepcion_bp_msg_len equ	$ - excepcion_bp_msg

excepcion_of_msg db		'Overflow Exception'
excepcion_of_msg_len equ	$ - excepcion_of_msg

excepcion_br_msg db		'BOUND Range Exceeded Exception'
excepcion_br_msg_len equ	$ - excepcion_br_msg

excepcion_ud_msg db		'Invalid Opcode Exception'
excepcion_ud_msg_len equ	$ - excepcion_ud_msg

excepcion_nm_msg db		'Device Not Available Exception'
excepcion_nm_msg_len equ	$ - excepcion_nm_msg

excepcion_df_msg db		'Double Fault Exception'
excepcion_df_msg_len equ	$ - excepcion_df_msg

excepcion_cpso_msg db		'Coprocesor Segment Overrun'
excepcion_cpso_msg_len equ	$ - excepcion_cpso_msg

excepcion_ts_msg db		'Invalid TSS Exception'
excepcion_ts_msg_len equ	$ - excepcion_ts_msg

excepcion_np_msg db		'Segment Not Present'
excepcion_np_msg_len equ	$ - excepcion_np_msg

excepcion_ss_msg db		'Stack Fault Exception'
excepcion_ss_msg_len equ	$ - excepcion_ss_msg

excepcion_gp_msg db		'General Protection Exception'
excepcion_gp_msg_len equ	$ - excepcion_gp_msg

excepcion_pf_msg db		'Page Fault Exception'
excepcion_pf_msg_len equ	$ - excepcion_pf_msg

excepcion_mf_msg db		'FPU Floating point Error'
excepcion_mf_msg_len equ	$ - excepcion_mf_msg

excepcion_ac_msg db		'Alignment Check Exception'
excepcion_ac_msg_len equ	$ - excepcion_ac_msg

excepcion_mc_msg db		'Maching Check Exception'
excepcion_mc_msg_len equ	$ - excepcion_mc_msg

excepcion_xm_msg db		'SIMD Floating point Exception'
excepcion_xm_msg_len equ	$ - excepcion_xm_msg

excepcion_ve_msg db		'Virtualization Exception'
excepcion_ve_msg_len equ	$ - excepcion_ve_msg

soltarR_ve_msg db		'Tecla R'
soltarR_ve_msg_len equ	$ - soltarR_ve_msg

soltarP_ve_msg db		'Tecla P'
soltarP_ve_msg_len equ	$ - soltarP_ve_msg

;; PIC
extern fin_intr_pic1


;;
;; Definición de MACROS
;;

%macro ISR 1

global _isr%1

_isr%1:
	; To Infinity And Beyond!!
	;jmp $

xchg bx, bx
push eax
mov eax, %1
pop eax

%endmacro

;;
;; Datos
;;
; Scheduler
reloj_numero:		 	dd 0x00000000
reloj:  				db '|/-\'

;;
;; Rutina de atención de las EXCEPCIONES
;;

;Rutina división por 0
ISR 0
;imprimir_excepcion excepcion_division_msg, excepcion_division_msg_len
pushfd
pushad
call jugador_actual 	; Me deja en ax el jugador actual
sub ax, 1d	 			; Le resto 1 para tener el indice en tareas[]
push ax ;Pusheo el parámetro para borrar la tarea
call sched_remover_tarea
add esp, 4 ;Restauro la pila
;Imprimo el mensaje correspondiente
pushad
pushfd
iret

;Rutina de atención de Debug Exception
ISR 1
;imprimir_excepcion excepcion_debug_msg, excepcion_debug_msg_len
pushfd
pushad
call jugador_actual 	; Me deja en ax el jugador actual
sub ax, 1d	 			; Le resto 1 para tener el indice en tareas[]
push ax ;Pusheo el parámetro para borrar la tarea
call sched_remover_tarea
add esp, 4 ;Restauro la pila
;Imprimo el mensaje correspondiente
pushad
pushfd
iret

;Rutina de atención de NMI
ISR 2
;imprimir_excepcion excepcion_nmi_msg, excepcion_nmi_msg_len
pushfd
pushad
call jugador_actual 	; Me deja en ax el jugador actual
sub ax, 1d	 			; Le resto 1 para tener el indice en tareas[]
push ax ;Pusheo el parámetro para borrar la tarea
call sched_remover_tarea
add esp, 4 ;Restauro la pila
;Imprimo el mensaje correspondiente
pushad
pushfd
iret

;Rutina de atención de Breakpoint
ISR 3
;imprimir_excepcion excepcion_bp_msg, excepcion_bp_msg_len
pushfd
pushad
call jugador_actual 	; Me deja en ax el jugador actual
sub ax, 1d	 			; Le resto 1 para tener el indice en tareas[]
push ax ;Pusheo el parámetro para borrar la tarea
call sched_remover_tarea
add esp, 4 ;Restauro la pila
;Imprimo el mensaje correspondiente
pushad
pushfd
iret

;Rutina de atención de Overflow
ISR 4
;imprimir_excepcion excepcion_of_msg, excepcion_of_msg_len
pushfd
pushad
call jugador_actual 	; Me deja en ax el jugador actual
sub ax, 1d	 			; Le resto 1 para tener el indice en tareas[]
push ax ;Pusheo el parámetro para borrar la tarea
call sched_remover_tarea
add esp, 4 ;Restauro la pila
;Imprimo el mensaje correspondiente
pushad
pushfd
iret

;Rutina de atención de Bound range exceeded
ISR 5
;imprimir_excepcion excepcion_br_msg, excepcion_br_msg_len
pushfd
pushad
call jugador_actual 	; Me deja en ax el jugador actual
sub ax, 1d	 			; Le resto 1 para tener el indice en tareas[]
push ax ;Pusheo el parámetro para borrar la tarea
call sched_remover_tarea
add esp, 4 ;Restauro la pila
;Imprimo el mensaje correspondiente
pushad
pushfd
iret

;Rutina de atención de Invalid Opcode
ISR 6
;imprimir_excepcion excepcion_ud_msg, excepcion_ud_msg_len
pushfd
pushad
call jugador_actual 	; Me deja en ax el jugador actual
sub ax, 1d	 			; Le resto 1 para tener el indice en tareas[]
push ax ;Pusheo el parámetro para borrar la tarea
call sched_remover_tarea
add esp, 4 ;Restauro la pila
;Imprimo el mensaje correspondiente
pushad
pushfd
iret

;Rutina de atención de Device not available
ISR 7
;imprimir_excepcion excepcion_nm_msg, excepcion_nm_msg_len
pushfd
pushad
call jugador_actual 	; Me deja en ax el jugador actual
sub ax, 1d	 			; Le resto 1 para tener el indice en tareas[]
push ax ;Pusheo el parámetro para borrar la tarea
call sched_remover_tarea
add esp, 4 ;Restauro la pila
;Imprimo el mensaje correspondiente
pushad
pushfd
iret

;Rutina de atención de Double Foult
ISR 8
;imprimir_excepcion excepcion_df_msg, excepcion_df_msg_len
pushfd
pushad
call jugador_actual 	; Me deja en ax el jugador actual
sub ax, 1d	 			; Le resto 1 para tener el indice en tareas[]
push ax ;Pusheo el parámetro para borrar la tarea
call sched_remover_tarea
add esp, 4 ;Restauro la pila
;Imprimo el mensaje correspondiente
pushad
pushfd
iret

;Rutina de atención de Coprocessor Segment Overrun
ISR 9
;imprimir_excepcion excepcion_cpso_msg, excepcion_cpso_msg_len
pushfd
pushad
call jugador_actual 	; Me deja en ax el jugador actual
sub ax, 1d	 			; Le resto 1 para tener el indice en tareas[]
push ax ;Pusheo el parámetro para borrar la tarea
call sched_remover_tarea
add esp, 4 ;Restauro la pila
;Imprimo el mensaje correspondiente
pushad
pushfd
iret

;Rutina de atención de Invalid TSS
ISR 10
;imprimir_excepcion excepcion_ts_msg, excepcion_ts_msg_len
pushfd
pushad
call jugador_actual 	; Me deja en ax el jugador actual
sub ax, 1d	 			; Le resto 1 para tener el indice en tareas[]
push ax ;Pusheo el parámetro para borrar la tarea
call sched_remover_tarea
add esp, 4 ;Restauro la pila
;Imprimo el mensaje correspondiente
pushad
pushfd
iret

;Rutina de atención de Segment not present
ISR 11
;imprimir_excepcion excepcion_np_msg, excepcion_np_msg_len
pushfd
pushad
call jugador_actual 	; Me deja en ax el jugador actual
sub ax, 1d	 			; Le resto 1 para tener el indice en tareas[]
push ax ;Pusheo el parámetro para borrar la tarea
call sched_remover_tarea
add esp, 4 ;Restauro la pila
;Imprimo el mensaje correspondiente
pushad
pushfd
iret

;Rutina de atención de Stack Fault
ISR 12
;imprimir_excepcion excepcion_ss_msg, excepcion_ss_msg_len
pushfd
pushad
call jugador_actual 	; Me deja en ax el jugador actual
sub ax, 1d	 			; Le resto 1 para tener el indice en tareas[]
push ax ;Pusheo el parámetro para borrar la tarea
call sched_remover_tarea
add esp, 4 ;Restauro la pila
;Imprimo el mensaje correspondiente
pushad
pushfd
iret

;Rutina de atención de General protection
ISR 13
;imprimir_excepcion excepcion_gp_msg, excepcion_gp_msg_len
pushfd
pushad
call jugador_actual 	; Me deja en ax el jugador actual

xchg bx, bx
xchg eax, eax

sub ax, 1d	 			; Le resto 1 para tener el indice en tareas[]
push ax ;Pusheo el parámetro para borrar la tarea
call sched_remover_tarea

xchg bx, bx
xchg ebx, ebx

add esp, 4 ;Restauro la pila
;Imprimo el mensaje correspondiente
pushad
pushfd
iret

;Rutina de atención de Page Fault
;The contents of the CR2 register. The processor loads the CR2 register with the 32-bit linear address that
;generated the exception. The page-fault handler can use this address to locate the corresponding page
;directory and page-table entries. Another page fault can potentially occur during execution of the page-fault
;handler; the handler should save the contents of the CR2 register before a second page fault can occur.

ISR 14
pushfd
pushad
mov eax, cr2 ;Pusheo cr2 para tenerlo como parámetro
push eax
call asignarMemoria
add esp, 4 ;Ver si está bien
cmp ax, 0 ;Veo si el resultado es 0
jne .fin14
;Borro la tarea
call jugador_actual 	; Me deja en ax el jugador actual
sub ax, 1d	 			; Le resto 1 para tener el indice en tareas[]
push ax ;Pusheo el parámetro para borrar la tarea
call sched_remover_tarea
add esp, 4 ;Restauro la pila
;Imprimo el mensaje correspondiente
;imprimir_excepcion excepcion_pf_msg, excepcion_pf_msg_len
;Tal vez hay que saltar directamente al sched, ver esto (tal vez se puede llamar a la int del reloj)
.fin14:
popad
popfd
iret

;Rutina de atención de INTERRUPCION 15
ISR 15
;imprimir_excepcion excepcion_mr_msg, excepcion_mr_msg_len
pushfd
pushad
call jugador_actual 	; Me deja en ax el jugador actual
sub ax, 1d	 			; Le resto 1 para tener el indice en tareas[]
push ax ;Pusheo el parámetro para borrar la tarea
call sched_remover_tarea
add esp, 4 ;Restauro la pila
;Imprimo el mensaje correspondiente
pushad
pushfd
iret

;Rutina de atención de FPU Floating point error
ISR 16
;imprimir_excepcion excepcion_mf_msg, excepcion_mf_msg_len
pushfd
pushad
call jugador_actual 	; Me deja en ax el jugador actual
sub ax, 1d	 			; Le resto 1 para tener el indice en tareas[]
push ax ;Pusheo el parámetro para borrar la tarea
call sched_remover_tarea
add esp, 4 ;Restauro la pila
;Imprimo el mensaje correspondiente
pushad
pushfd
iret

;Rutina de atención de Alignment Check
ISR 17
;imprimir_excepcion excepcion_ac_msg, excepcion_ac_msg_len
pushfd
pushad
call jugador_actual 	; Me deja en ax el jugador actual
sub ax, 1d	 			; Le resto 1 para tener el indice en tareas[]
push ax ;Pusheo el parámetro para borrar la tarea
call sched_remover_tarea
add esp, 4 ;Restauro la pila
;Imprimo el mensaje correspondiente
pushad
pushfd
iret

;Rutina de atención de Machine Check
ISR 18
;imprimir_excepcion excepcion_mc_msg, excepcion_mc_msg_len
pushfd
pushad
call jugador_actual 	; Me deja en ax el jugador actual
sub ax, 1d	 			; Le resto 1 para tener el indice en tareas[]
push ax ;Pusheo el parámetro para borrar la tarea
call sched_remover_tarea
add esp, 4 ;Restauro la pila
;Imprimo el mensaje correspondiente
pushad
pushfd
iret

;Rutina de atención de SIMD Floating Point
ISR 19
;imprimir_excepcion excepcion_xm_msg, excepcion_xm_msg_len
pushfd
pushad
call jugador_actual 	; Me deja en ax el jugador actual
sub ax, 1d	 			; Le resto 1 para tener el indice en tareas[]
push ax ;Pusheo el parámetro para borrar la tarea
call sched_remover_tarea
add esp, 4 ;Restauro la pila
;Imprimo el mensaje correspondiente
pushad
pushfd
iret

;Rutina de atención de Virtualization
ISR 20
;imprimir_excepcion excepcion_ve_msg, excepcion_ve_msg_len
pushfd
pushad
call jugador_actual 	; Me deja en ax el jugador actual
sub ax, 1d	 			; Le resto 1 para tener el indice en tareas[]
push ax ;Pusheo el parámetro para borrar la tarea
call sched_remover_tarea
add esp, 4 ;Restauro la pila
;Imprimo el mensaje correspondiente
pushad
pushfd
iret

;;
;; variables del handler del RELOJ
;;

offset:      dd 0
proximaTarea:   dw 0 

;;
;;	Función jmpToTask para saltar de tareas
;;

global jmpToTask

jmpToTask:
	push ebp
	mov ebp, esp
	pushad
	mov eax, [ebp+8]
	mov [proximaTarea], ax
	xchg bx, bx
    jmp far [offset]
    popad
   	pop ebp
    ret

;;
;; Rutina de atención del RELOJ
;;

ISR 32
	pushfd 					; guarda del valor de los flags
	pushad
	call fin_intr_pic1 		; le comunica al pic que ya se atendio la interrupción
	call proximo_reloj
	call sched			
	popad
	popfd 				; restablece el valor de los flags
	iret 				; retorna de la interrupción

;;
;; Rutina de atención del TECLADO
;;
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
call jugador_actual 
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
call jugador_actual 	
pop ebx
push ecx
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





; --------------------  funciones auxiliares ------------------------------

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
