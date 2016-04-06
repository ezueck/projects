; Print.s
; Student names: change this to your names or look very silly
; Last modification date: change this to the last modification date or look very silly
; Runs on LM4F120 or TM4C123
; EE319K lab 7 device driver for any LCD
;
; As part of Lab 7, students need to implement these LCD_OutDec and LCD_OutFix
; This driver assumes two low-level LCD functions
; ST7735_OutChar   outputs a single 8-bit ASCII character
; ST7735_OutString outputs a null-terminated string 

    IMPORT   ST7735_OutChar
    IMPORT   ST7735_OutString
    EXPORT   LCD_OutDec
    EXPORT   LCD_OutFix
	PRESERVE8
	AREA Variables, READWRITE, ALIGN=2
CounterA DCD 0x0000
CounterB DCD 0x0000
    AREA    |.text|, CODE, READONLY, ALIGN=2	
    THUMB



;-----------------------LCD_OutDec-----------------------
; Output a 32-bit number in unsigned decimal format
; Input: R0 (call by value) 32-bit unsigned number
; Output: none
; Invariables: This function must not permanently modify registers R4 to R11
LCD_OutDec
	  PUSH {LR}
	  LDR R1, =CounterA ;Add 1 to our counter
	  MOV R12, #1
	  STR R12, [R1] 	;We start our counter
	  MOV R2, #10     	;Save our link register 
	  MOV R12, #10
Div	  UDIV R3, R0, R2 	;We do R0/10^n to get remainder 
	  ADDS R3, R3, #0 
	  BEQ Done			;Check if we're in last digit 
	  MUL R3, R3, R2
	  SUB R3, R0, R3
	  SUB R0, R0, R3
	  MOV R12, #10
	  UDIV R12, R2, R12
	  UDIV R3, R3, R12 	;We get the the actual digit 
	  PUSH{R3}			;Save it in the stack
	  LDR R1, =CounterA ;Add 1 to our counter
	  LDR R12, [R1]
	  ADD R12, R12, #1
	  STR R12, [R1]
	  MOV R12, #10
	  MUL R2, R12 
	  B Div
Done  UDIV R3, R2, R12
	  UDIV R0, R0, R3
	  PUSH {R0}			;Get the last digit and push it into stack
Rep   POP{R0}
      ADD R0, R0, #0x30	;Get the ASCII value of our number 
	  BL ST7735_OutChar	;Output the character
	  LDR R1, =CounterA ;Check the counter
	  LDR R12, [R1]
	  SUBS R12, R12, #1
	  STR R12, [R1]
	  BNE Rep			;Loop back for more digits
	  MOV R0, #0x20
	  BL ST7735_OutChar
	  POP {LR}
	  BX LR

;* * * * * * * * End of LCD_OutDec * * * * * * * *

; -----------------------LCD _OutFix----------------------
; Output characters to LCD display in fixed-point format
; unsigned decimal, resolution 0.001, range 0.000 to 9.999
; Inputs:  R0 is an unsigned 32-bit number
; Outputs: none
; E.g., R0=0,    then output "0.000 "
;       R0=3,    then output "0.003 "
;       R0=89,   then output "0.089 "
;       R0=123,  then output "0.123 "
;       R0=9999, then output "9.999 "
;       R0>9999, then output "*.*** "
; Invariables: This function must not permanently modify registers R4 to R11
LCD_OutFix

		PUSH {LR}
;We check for R0>9999
		MOV R1, #9
		MOV R2, #1111
		MUL R1, R2, R1
		CMP R0, R1
		BHI TooLarge
		
;Start our counter and registers 
		LDR R1, =CounterB ;Add 1 to our counter
		MOV R12, #1
		STR R12, [R1] 	;We start our counter
		MOV R2, #10     	 
		MOV R12, #10
		
;Get the digits for our number
DivB	UDIV R3, R0, R2 	;We do R0/10^n to get remainder 
		ADDS R3, R3, #0 
		BEQ DoneB			;Check if we're in last digit 
		MUL R3, R3, R2
		SUB R3, R0, R3
		SUB R0, R0, R3
		MOV R12, #10
		UDIV R12, R2, R12
		UDIV R3, R3, R12 	;We get the the actual digit 
		PUSH{R3}			;Save it in the stack
		LDR R1, =CounterB ;Add 1 to our counter
		LDR R12, [R1]
		ADD R12, R12, #1
		STR R12, [R1]
		MOV R12, #10
		MUL R2, R12 
		B DivB
DoneB  	UDIV R3, R2, R12
		UDIV R0, R0, R3
		PUSH {R0}			;Get the last digit and push it into stack
		
;We print our number
;Calculate how many zeroes we have
		LDR R3, =CounterB
		LDR R1, [R3]
		MOV R3, #4 		;Number of digits 
		MOV R2, #0
		SUBS R3, R3, R1
Check	BEQ RepB
		MOV R0, #0x30
		PUSH {R0, R3, R2}
		BL ST7735_OutChar	;Output a 0
		POP {R0, R3, R2}
		ADD R2, #1
		CMP R2, #1
		BEQ DotA
Return	SUBS R3, R3, #1
		B Check
		
;Print our digits		
RepB   	POP{R0}
		ADD R0, R0, #0x30	;Get the ASCII value of our number 
		PUSH {R0, R3, R2}
		BL ST7735_OutChar	;Output the character
		POP {R0, R3, R2}
		ADD R2, #1
		CMP R2, #1
		BEQ DotB
Cont	LDR R1, =CounterB ;Check the counter
		LDR R12, [R1]
		SUBS R12, R12, #1
		STR R12, [R1]
		BNE RepB		;Loop back for more digits
		POP {LR}
Leave   BX   LR
		
TooLarge 
		MOV R0, #0x2A
		BL ST7735_OutChar
		MOV R0, #0x2E
		BL ST7735_OutChar
		MOV R0, #0x2A
		BL ST7735_OutChar
		MOV R0, #0x2A
		BL ST7735_OutChar
		MOV R0, #0x2A
		BL ST7735_OutChar
		MOV R0, #0x2A
		POP {LR}
		BX LR	
DotA
	MOV R0, #0x2E
	PUSH {R0, R3, R2}
	BL ST7735_OutChar	;Output the character
	POP {R0, R3, R2}
	B Return
DotB
	MOV R0, #0x2E
	PUSH {R0, R3, R2}
	BL ST7735_OutChar	;Output the character
	POP {R0, R3, R2}
	B Cont
	
ALIGN
;* * * * * * * * End of LCD_OutFix * * * * * * * *

     ALIGN                           ; make sure the end of this section is aligned
     END                             ; end of file
