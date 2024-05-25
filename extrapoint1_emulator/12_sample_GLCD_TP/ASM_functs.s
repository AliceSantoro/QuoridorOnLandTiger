				
				AREA asm_functions, CODE, READONLY				
                EXPORT  save_move
save_move
				; save current SP for a faster access 
				; to parameters in the stack
				MOV   r12, sp
				; save volatile registers
				STMFD sp!,{r2-r8,r10-r11,lr}				
				; extract argument 4 and 5 into R4 and R5

				; arguments 
				; R0 - Vett_in[]
				; R1 - Vett_out[]
				; R2 - valore in costruzione
				; R3 - cnt elementi nel valore attuale
				; R4 - valore letto
				; R5 - conta simboli
				; R6 - valori in tabella
				; R7 - pointer a tabella
				; R8 - dato_tabella
			
				
				

				

				

                END
					
					