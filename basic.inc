	ifndef	_BASIC_INC_
	#define	_BASIC_INC_
;macros
;***********************************************************************************************************
; _IF			- begin conditional code
; _ELSEIF		- new condition, if previous conditions have failed
; _ELSE			- default code, if previous conditions have failed
; _ENDIF		- end conditional code
;
; macro:	_IF
; syntax:	_IF instruction, operand1, operand2, condition
; cycl	:
; 
; macro:	_ELSEIF
; syntax:	_ELSEIF instruction, operand1, operand2, condition
;
; macro:	_ELSE
; syntax:	_ELSE
;
; macro:	_ENDIF
; syntax:	_ENDIF
;
;***********************************************************************************************************
		#define	_BTF	1		;TEST BIT IN F
		#define _S		1		;BIT SET
		#define	_C		2		;BIT NOT SET

		#define _CPF	2		;COMPARE F AND W
		#define _EQ		3		;F==W
		#define	_NEQ	4		;F!=W
		#define	_GT		5		;F>W
		#define	_LT		6		;F<W
		#define	_NGT	7		;F<=W
		#define	_NLT	8		;F>=W

		#define _TSTF	3
		#define	_Z		9
		#define	_NZ		10
		#define	_INCF	4
		#define	_DECF	5

		variable if_level=0
		while if_level < 64
			variable if_count#v(if_level)=0
			variable else_count#v(if_level)=0
if_level += 1
		endw
if_level = 0


;***********************************************************************************************************
;
;	Compare F AND W
;	Skip next instruction, if F equals W (F == W)
;	7(6) cycl
;
;***********************************************************************************************************
_CPFSEQ		macro f1
			subwf f1,1 		;f=f-w		1 cycl
			bz	$+3			;			3(2) cycl
			addwf f1,1		;f=f+w		1 cycl
			b	$+3			;			2 cycl
			addwf f1,1		;			1 cycl
			b	$+2			;			2 cycl
			endm

;***********************************************************************************************************
;
;	Compare F AND W
;	Skip next instruction, if F not equal W (F != W)
;	7(6) cycl
;
;***********************************************************************************************************
_CPFSNEQ	macro f1
			subwf f1,1 		;f=f-w
			bnz	$+3
			addwf f1,1		;f=f+w
			b	$+3
			addwf f1,1
			b	$+2
			endm

;***********************************************************************************************************
;
;	Compare F AND W
;	Skip next instruction, if F greater than W (F > W)
;	7 cycl (F==W)
;	9 cycl (F>W)
;	8 cycl (F<W)
;
;***********************************************************************************************************
_CPFSGT		macro f1
			subwf f1,f 		;f=f-w
			bz	$+3
			bc	$+3
			addwf f1,f		;f=f+w
			b	$+3
			addwf f1,f
			b	$+2
			endm

;***********************************************************************************************************
;
;	Compare F AND W
;	Skip next instruction, if F lower than W (F < W)
;	7(6) cycl
;
;***********************************************************************************************************
_CPFSLT		macro f1
			subwf f1,1 		;f=f-w
			bnc	$+3
			addwf f1,1		;f=f+w
			b	$+3
			addwf f1,1
			b	$+2
			endm
;***********************************************************************************************************
;
;	Compare F AND W
;	Skip next instruction, if F not greater than W (F <= W)
;	7 cycl (F==W)
;	9 cycl (F<W)
;	8 cycl (F>W)
;
;***********************************************************************************************************
_CPFSNGT	macro f1
			subwf f1,1 		;f=f-w		1 cycl
			bz	$+5			;			3(2) cycl
			bnc	$+3			;			3(2) cycl
			addwf f1,1		;f=f+w		1 cycl
			b	$+3			;			2 cycl
			addwf f1,1		;			1 cycl
			b	$+2			;			2 cycl
			endm

;***********************************************************************************************************
;
;	Compare F AND W
;	Skip next instruction, if F not lower than W (F >= W)
;	7(6) cycl
;
;***********************************************************************************************************
_CPFSNLT	macro f1
			subwf f1,1 		;f=f-w
			bc	$+3
			addwf f1,1		;f=f+w
			b	$+3
			addwf f1,1
			b	$+2
			endm

;***********************************************************************************************************
;	common if macro, used by _if and _elseif macro.
;	implements the allowed instructions
; 
;
;***********************************************************************************************************
__common_if macro instr,op1,op2,con
		if instr == _BTF
			if con == _S
				btfss op1,op2
				goto _if_l#v(if_level)_c#v(if_count#v(if_level))_e#v(else_count#v(if_level))
			else
				if con == _C
					btfsc op1,op2
					goto _if_l#v(if_level)_c#v(if_count#v(if_level))_e#v(else_count#v(if_level))
				else
					error "unknown condition used for instruction _BTF"
				endif
			endif
		else
			if instr == _CPF
				if con == _EQ
					_CPFSEQ op1
					goto _if_l#v(if_level)_c#v(if_count#v(if_level))_e#v(else_count#v(if_level))
				else
					if con == _NEQ
						_CPFSNEQ op1
						goto _if_l#v(if_level)_c#v(if_count#v(if_level))_e#v(else_count#v(if_level))
					else
						if con == _GT
							_CPFSGT op1
							goto _if_l#v(if_level)_c#v(if_count#v(if_level))_e#v(else_count#v(if_level))
						else
							if con == _LT
								_CPFSLT op1
								goto _if_l#v(if_level)_c#v(if_count#v(if_level))_e#v(else_count#v(if_level))
							else
								if con == _NGT
									_CPFSNGT op1
									goto _if_l#v(if_level)_c#v(if_count#v(if_level))_e#v(else_count#v(if_level))
								else
									if con == _NLT
										_CPFSNLT op1
										goto _if_l#v(if_level)_c#v(if_count#v(if_level))_e#v(else_count#v(if_level))
									else
										error "unknown condition used for instruction _CPF"
									endif
								endif
							endif
						endif
					endif
				endif
			else
				if instr == _TSTF
					TSTF op1
					if con == _Z
						bnz _if_l#v(if_level)_c#v(if_count#v(if_level))_e#v(else_count#v(if_level))
					else
						if con == _NZ
							bz _if_l#v(if_level)_c#v(if_count#v(if_level))_e#v(else_count#v(if_level))
						else
							error "unknown condition used for instruction _TSTF"
						endif
					endif
				else
					if instr == _INCF
						INCFSZ	op1,op2
						if	con == _NZ
							goto	$+2
							goto	_if_l#v(if_level)_c#v(if_count#v(if_level))_e#v(else_count#v(if_level))
						else
							if con == _Z
								goto	_if_l#v(if_level)_c#v(if_count#v(if_level))_e#v(else_count#v(if_level))
							else
								error "unknown condition used for instruction _INCF"
							endif
						endif
					else

						if instr == _DECF
							DECFSZ	op1,op2
							if	con == _NZ
								goto	$+2
								goto	_if_l#v(if_level)_c#v(if_count#v(if_level))_e#v(else_count#v(if_level))
							else
								if con == _Z
									goto	_if_l#v(if_level)_c#v(if_count#v(if_level))_e#v(else_count#v(if_level))
								else
									error "unknown condition used for instruction _DECF"
								endif
							endif
						else
							error "unknown instruction used for _if"
						endif
					endif
				endif
			endif
		endif
		endm


;***********************************************************************************************************
;
;
;***********************************************************************************************************
_if		macro	instr,op1,op2,con
if_level += 1
		__common_if instr,op1,op2,con
		endm


;***********************************************************************************************************
;
;
;***********************************************************************************************************
_elseif	macro	instr,op1,op2,con
		goto _endif_l#v(if_level)_c#v(if_count#v(if_level))
_if_l#v(if_level)_c#v(if_count#v(if_level))_e#v(else_count#v(if_level))
else_count#v(if_level) += 1
		__common_if instr,op1,op2,con
		endm


;***********************************************************************************************************
;
;
;***********************************************************************************************************
_else	macro
		goto _endif_l#v(if_level)_c#v(if_count#v(if_level))
_if_l#v(if_level)_c#v(if_count#v(if_level))_e#v(else_count#v(if_level))
else_count#v(if_level) += 1
		endm


;***********************************************************************************************************
;
;
;***********************************************************************************************************	
_endif	macro
_endif_l#v(if_level)_c#v(if_count#v(if_level))
_if_l#v(if_level)_c#v(if_count#v(if_level))_e#v(else_count#v(if_level))
if_count#v(if_level) += 1
if_level -= 1
		endm

;***********************************************************************************************************
;
; Content of w is stored into f
; Content of f is restored into w
; 3 cycl
;
;***********************************************************************************************************	
SWAPWF  MACRO  REG
        XORWF  REG,F
        XORWF  REG,W
        XORWF  REG,F
        ENDM

		variable while_level=0
		while while_level < 64
			variable while_count#v(while_level)=0
			variable do_count#v(while_level)=0
while_level += 1
		endw
while_level = 0

;***********************************************************************************************************
;	common while macro, used by _while macro.
;	implements the allowed instructions
; 
;
;***********************************************************************************************************
__common_while macro instr,op1,op2,con
		if instr == _BTF
			if con == _S
				btfss op1,op2
				goto _enddo_while_l#v(while_level)_c#v(while_count#v(while_level))
			else
				if con == _C
					btfsc op1,op2
					goto _enddo_while_l#v(while_level)_c#v(while_count#v(while_level))
				else
					error "unknown condition used for instruction _BTF"
				endif
			endif
		else
			if instr == _CPF
				if con == _EQ
					_CPFSEQ op1
					goto _enddo_while_l#v(while_level)_c#v(while_count#v(while_level))
				else
					if con == _NEQ
						_CPFSNEQ op1
						goto _enddo_while_l#v(while_level)_c#v(while_count#v(while_level))
					else
						if con == _GT
							_CPFSGT op1
							goto _enddo_while_l#v(while_level)_c#v(while_count#v(while_level))
						else
							if con == _LT
								_CPFSLT op1
								goto _enddo_while_l#v(while_level)_c#v(while_count#v(while_level))
							else
								if con == _NGT
									_CPFSNGT op1
									goto _enddo_while_l#v(while_level)_c#v(while_count#v(while_level))
								else
									if con == _NLT
										_CPFSNLT op1
										goto _enddo_while_l#v(while_level)_c#v(while_count#v(while_level))
									else
										error "unknown condition used for instruction _CPF"
									endif
								endif
							endif
						endif
					endif
				endif
			else
				if instr == _TSTF
					TSTF op1
					if con == _Z
						bnz _enddo_while_l#v(while_level)_c#v(while_count#v(while_level))
					else
						if con == _NZ
							bz _enddo_while_l#v(while_level)_c#v(while_count#v(while_level))
						else
							error "unknown condition used for instruction _TSTF"
						endif
					endif
				else
					if instr == _INCF
						INCFSZ	op1,op2
						if	con == _NZ
							goto	$+2
							goto	_enddo_while_l#v(while_level)_c#v(while_count#v(while_level))
						else
							if con == _Z
								goto	_enddo_while_l#v(while_level)_c#v(while_count#v(while_level))
							else
								error "unknown condition used for instruction _INCF"
							endif
						endif
					else
						if instr == _DECF
							DECFSZ	op1,op2
							if	con == _NZ
								goto	$+2
								goto	_enddo_while_l#v(while_level)_c#v(while_count#v(while_level))
							else
								if con == _Z
									goto	_enddo_while_l#v(while_level)_c#v(while_count#v(while_level))
								else
									error "unknown condition used for instruction _DECF"
								endif
							endif
						else
							error "unknown instruction used for _while"
						endif
					endif
				endif
			endif
		endif
		endm


_while 		macro	instr,op1,op2,con
while_level += 1
_while_l#v(while_level)_c#v(while_count#v(while_level))
			__common_while instr,op1,op2,con
			endm

_exitdo		macro
			goto	_enddo_while_l#v(while_level)_c#v(while_count#v(while_level))
			endm

_exitdoif	macro	instr,op1,op2,con
			_if instr,op1,op2,con
				goto	_enddo_while_l#v(while_level)_c#v(while_count#v(while_level))
			_endif
			endm

_enddo		macro
			goto	_while_l#v(while_level)_c#v(while_count#v(while_level))
_enddo_while_l#v(while_level)_c#v(while_count#v(while_level))
while_count#v(while_level) += 1
while_level -= 1
			endm

	endif