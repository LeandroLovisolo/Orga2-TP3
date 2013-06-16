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
extern tareaActiva
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
imprimir_excepcion excepcion_division_msg, excepcion_division_msg_len
jmp $

;Rutina de atención de Debug Exception
ISR 1
imprimir_excepcion excepcion_debug_msg, excepcion_debug_msg_len
jmp $

;Rutina de atención de NMI
ISR 2
imprimir_excepcion excepcion_nmi_msg, excepcion_nmi_msg_len
jmp $

;Rutina de atención de Breakpoint
ISR 3
imprimir_excepcion excepcion_bp_msg, excepcion_bp_msg_len
jmp $

;Rutina de atención de Overflow
ISR 4
imprimir_excepcion excepcion_of_msg, excepcion_of_msg_len
jmp $

;Rutina de atención de Bound range exceeded
ISR 5
imprimir_excepcion excepcion_br_msg, excepcion_br_msg_len
jmp $

;Rutina de atención de Invalid Opcode
ISR 6
imprimir_excepcion excepcion_ud_msg, excepcion_ud_msg_len
jmp $

;Rutina de atención de Device not available
ISR 7
imprimir_excepcion excepcion_nm_msg, excepcion_nm_msg_len
jmp $

;Rutina de atención de Double Foult
ISR 8
imprimir_excepcion excepcion_df_msg, excepcion_df_msg_len
jmp $

;Rutina de atención de Coprocessor Segment Overrun
ISR 9
imprimir_excepcion excepcion_cpso_msg, excepcion_cpso_msg_len
jmp $

;Rutina de atención de Invalid TSS
ISR 10
imprimir_excepcion excepcion_ts_msg, excepcion_ts_msg_len
jmp $

;Rutina de atención de Segment not present
ISR 11
imprimir_excepcion excepcion_np_msg, excepcion_np_msg_len
jmp $

;Rutina de atención de Stack Fault
ISR 12
imprimir_excepcion excepcion_ss_msg, excepcion_ss_msg_len
jmp $

;Rutina de atención de General protection
ISR 13
imprimir_excepcion excepcion_gp_msg, excepcion_gp_msg_len
jmp $

;Rutina de atención de Page Fault
;The contents of the CR2 register. The processor loads the CR2 register with the 32-bit linear address that
;generated the exception. The page-fault handler can use this address to locate the corresponding page
;directory and page-table entries. Another page fault can potentially occur during execution of the page-fault
;handler; the handler should save the contents of the CR2 register before a second page fault can occur.

ISR 14
pushfd
mov eax, cr2 ;Pusheo cr2 para tenerlo como parámetro
push eax
call asignarMemoria
add esp, 4 ;Ver si está bien
cmp ax, 0 ;Veo si el resultado es 0
jne .fin14
;Borro la tarea
call tareaActiva ;Me deja en ax la tarea
sub ax, 10d ;Le resto 10 para tener el indice en tareas[]
push ax ;Pusheo el parámetro para borrar la tarea
call sched_remover_tarea
add esp, 4 ;Restauro la pila
;Imprimo el mensaje correspondiente
;imprimir_excepcion excepcion_pf_msg, excepcion_pf_msg_len
;Tal vez hay que saltar directamente al sched, ver esto (tal vez se puede llamar a la int del reloj)
.fin14:
popfd
iret

;Rutina de atención de INTERRUPCION 15
ISR 15
;imprimir_excepcion excepcion_mr_msg, excepcion_mr_msg_len
jmp $

;Rutina de atención de FPU Floating point error
ISR 16
imprimir_excepcion excepcion_mf_msg, excepcion_mf_msg_len
jmp $

;Rutina de atención de Alignment Check
ISR 17
imprimir_excepcion excepcion_ac_msg, excepcion_ac_msg_len
jmp $

;Rutina de atención de Machine Check
ISR 18
imprimir_excepcion excepcion_mc_msg, excepcion_mc_msg_len
jmp $

;Rutina de atención de SIMD Floating Point
ISR 19
imprimir_excepcion excepcion_xm_msg, excepcion_xm_msg_len
jmp $

;Rutina de atención de Virtualization
ISR 20
imprimir_excepcion excepcion_ve_msg, excepcion_ve_msg_len
jmp $

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
	push eax
	call fin_intr_pic1 		; le comunica al pic que ya se atendio la interrupción
	xchg bx, bx
	pushad
	xchg bx, bx
	call sched
	popad

	;Comentario candidato a ser eliminado!
	; se fija si se le acabó el cuantum a la tarea actual
; 	cmp byte [quantum], 0
; 	jne .finYDec32 			; si no se le acabo salta a la sección donde decrementa el cuantum y sale
	
; 	; si llego aca es porque el cuantum de la tarea actual se acabo
; 	mov byte [quantum], 2 	; Reestablezco quantum
; 	; se fija si el programa no está actualmente pausado
; 	cmp byte [pausado], 0 
; 	jne .noPausar32 		; si no es 0 es porque esta pausado, en ese caso hay que preguntar si se debe despausarlo

; 	; si no salto es porque no esta pausado y en ese caso me fijo si hay que pausarlo
; 	cmp byte [pausarReanudar], 1
; 	jne .cambiarTarea32 	; si salta es porque no hay que pausar

; 	; si no salta es porque hay que pausarlo, en ese caso seteamos el bit de pausado a 1 para informar que pasa a ser pausado
; 	mov byte [pausado], 1
; 	;Salto a idle
; 	jmp 72:00
; 	jmp .fin32 ; Al volver a la tarea quiero que se siga ejecutando

; 	; si entra en esta etiqueta es porque ya esta pausado, en este caso hay que ver si hay que despausarlo
; .noPausar32:
; 	cmp byte [pausarReanudar], 0 ; Veo si tengo que despausar
; 	jne .fin32 					 ; si salta es porque no hay que reanudar

; 	; si no salta es porque hay que reanudar el hilo de las tareas 
; 	mov byte [pausado], 0 		 ; informamos que ya dejara de estar pausado seteando en 0 pausado
; 	.cambiarTarea32: 			 ; paso a la proxima tarea

; 	pushad 						; Push EAX, ECX, EDX, EBX, original ESP, EBP, ESI, and EDI
; 	call sched_proximo_indice

; 	xchg bx, bx

; 	; Sacado de http://forum.osdev.org/viewtopic.php?f=1&t=17813
;     push    eax              	; ax contains your TSS selector
;     push    0         			; offset is ignored
;     jmp     far [esp+0]
;     add     esp, 8              ; remove the dwords pushed onto the stack

; 	popad 						; Pop EAX, ECX, EDX, EBX, original ESP, EBP, ESI, and EDI
; 	jmp .fin32

; .finYDec32:

; 	;Decremento el quantum
; 	mov al, [quantum]
; 	dec al
; 	mov byte [quantum], al

; .fin32:
; 	call proximo_reloj 	; llama al handler del reloj
	pop eax 			
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

;;
;; Rutina de atencion x80
;;
ISR 128
pushfd 				; pushea el estado de los flags
call fin_intr_pic1 	; comunica al PIC que la interrupción fue atendida

; se fija que acción debe llevar a cabo, esto depende del valor que pasaron en eax
cmp eax,111
Je .duplicar_128
; si no saltó puede asumir que el valor de eax es 222, en otro caso no le importa ya que no dice que hacer

; esta función devuelve en eax el número del jugagor al que pertence la tarea HACER
call obtener_id_jugador 	

; le paso los parámetros a traves de la pila
push esi
push edx
push ecx
push ebx
push eax
call game_migrar
add esp,20 	 		; pongo el puntero de la pila en la posición correcta		

jmp .salir_128

.duplicar_128:
; si entro aca es porque eax es 1 y tiene que llamar a duplicar
; esta función devuelve en eax el número del jugagor al que pertence la tarea HACER
call obtener_id_jugador 	
							
; le paso los parámetros a traves de la pila y llama a la función que debe ejecutar
push ecx
push ebx
push eax
call game_duplicar 	; me devuelve en eax el valor de si se pudo llevar a cabo o no
add esp,8 			; pongo el puntero de la pila en la posición correcta

.salir_128:

; restablece los registros pusheados
popfd 				; restablece el estado de los flags
iret 				; retorna de las interrupciones

ISR 144
pushfd
cmp eax, 200 ;Veo si hay que terminar el juego
jne .iniciarJuego
;Termino el juego
call game_terminar
jmp .fin144
.iniciarJuego:
cmp eax, 300 ;Veo si hay que iniciar el juego
jne .fin144
;Inicializar juego
call game_iniciar
.fin144:
popfd

; --------------------  funciones auxiliares ------------------------------
obtener_id_jugador:
ret

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
