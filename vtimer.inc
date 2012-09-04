	ifndef	_VTIMER_INC_
	#define	_VTIMER_INC_

	#include	"basic.inc"

VTMR00ID	EQU		0
VTMR01ID	EQU		1
VTMR02ID	EQU		2
VTMR03ID	EQU		3
VTMR04ID	EQU		4
VTMR05ID	EQU		5
VTMR06ID	EQU		6
VTMR07ID	EQU		7
VTMR00IF	EQU		0
VTMR01IF	EQU		1
VTMR02IF	EQU		2
VTMR03IF	EQU		3
VTMR04IF	EQU		4
VTMR05IF	EQU		5
VTMR06IF	EQU		6
VTMR07IF	EQU		7

VTMR10ID	EQU		0
VTMR11ID	EQU		1
VTMR12ID	EQU		2
VTMR13ID	EQU		3
VTMR14ID	EQU		4
VTMR15ID	EQU		5
VTMR16ID	EQU		6
VTMR17ID	EQU		7
VTMR10IF	EQU		0
VTMR11IF	EQU		1
VTMR12IF	EQU		2
VTMR13IF	EQU		3
VTMR14IF	EQU		4
VTMR15IF	EQU		5
VTMR16IF	EQU		6
VTMR17IF	EQU		7

;***********************************************************************************************************
;
;	VTIMER0 memory allocation
;	Creates memory allocation section for VTIMER0
;	Parameter
;		count			-	VTIMER0 count (min 1, max 8)
;	Requires
;	Changes
;	0(0) cycl
;
;***********************************************************************************************************
VTIMER0			macro	count
	ifdef			_VTIMER0_MEMORY_ALLOC_DONE_
				error "VTIMER0 memory section already created."
	else
		if			count < 1
				error "VTIMER0 count must be greater than 0."
		else
			if		count > 8
				error "VTIMER0 count must not be greater than 8"
			else
				constant		vtimer0_count=count
VTMR0UDATA		UDATA_SHR
VTMR0CON		RES		1
VTMR0INT		RES		1
VTMR0CONST_FF	RES		1
				local ii = 0
				while ii < count
VTMR0#v(ii)L		RES		1
ii += 1
				endw
				#define	_VTIMER0_MEMORY_ALLOC_DONE_
			endif
		endif
	endif
				endm

;***********************************************************************************************************
;
;	VTIMER0 initialization
;	Initializes VTIMER0 memory
;	Parameter
;	Requires
;		VTIMER0
;	Changes
;		bank
;
;***********************************************************************************************************
VTIMER0_INIT	macro
	ifndef	_VTIMER0_MEMORY_ALLOC_DONE_
		error "No memory for VTIMER0 available. Call VTIMER0, first."
	else
		ifdef	_VTIMER0_INIT_DONE_
			error "VTIMER0 initialization already done"
		else
			clrf	VTMR0CON
			clrf	VTMR0INT
			movlw	H'FF'
			movwf	VTMR0CONST_FF
			local ii = 0
			while ii < vtimer0_count
				clrf VTMR0#v(ii)L
ii += 1
				endw
			#define	_VTIMER0_INIT_DONE_	
		endif
	endif
				endm

;***********************************************************************************************************
;
;	VTIMER0 ON
;	Enables one specific VTIMER0 and initializes it with a timer value.
;	Parameter
;		TIMER_ID			-	identifier of VTIMER0 that has to be enabled (min 0, max VTIMER0 count - 1)
;		TMRL				-	address to the timer value that has to be used for the virtual timer
;	Requires
;		BANK 0
;		VTIMER0
;		VTIMER0_INIT
;	Changes
;		W
;
;***********************************************************************************************************
VTIMER0_ON		macro	TIMER_ID,TMRL
	ifndef	_VTIMER0_INIT_DONE_
		error "VTIMER0 not initialized, yet. Call VTIMER0_INIT, first."
	else
		if TIMER_ID >= vtimer0_count
			error	"TIMER_ID " #v(TIMER_ID) " not available. VTIMER0 TIMER_ID must be lower than "#v(vtimer0_count)"."
		else
			if TIMER_ID < 0
				error "VTIMER0 TIMER_ID must be greater than or equal 0."
			else
				movf	TMRL,w
				movwf	VTMR0#v(TIMER_ID)L
				bsf		VTMR0CON,#v(TIMER_ID)
			endif
		endif
	endif
				endm


;***********************************************************************************************************
;
;	VTIMER0 OFF
;	Disables one specific VTIMER0.
;	Parameter
;		TIMER_ID			-	identifier of VTIMER0 that has to be disabled (min 0, max VTIMER0 count - 1)
;	Requires
;		BANK 0
;		VTIMER0
;		VTIMER0_INIT
;	Changes
;
;***********************************************************************************************************
VTIMER0_OFF		macro	TIMER_ID
	ifndef	_VTIMER0_INIT_DONE_
		error "VTIMER0 not initialized, yet. Call VTIMER0_INIT, first."
	else
		if TIMER_ID >= vtimer0_count
			error	"TIMER_ID " #v(TIMER_ID) " not available. VTIMER0 TIMER_ID must be lower than "#v(vtimer0_count)"."
		else
			if TIMER_ID < 0
				error "VTIMER0 TIMER_ID must be greater than or equal 0."
			else
				bcf	VTMR0CON,TIMER_ID
			endif
		endif
	endif
				endm


;***********************************************************************************************************
;
;	VTIMER0 CHECK
;	Checks an overflow off one or more VTIMER0.
;	Parameter
;	Requires
;		BANK 0
;		VTIMER0
;		VTIMER0_INIT
;	Changes
;		VTMR0INT				-	interrupt bit of the coresponding vtimer is set of  	
;
;***********************************************************************************************************
VTIMER0_CHECK		macro
	ifndef	_VTIMER0_INIT_DONE_
		error "VTIMER0 not initialized, yet. Call VTIMER0_INIT, first."
	else
		movlw	H'FF'
		local	ii = 0
		while	ii < vtimer0_count
			_if _BTF,VTMR0CON,#v(ii),_S
;				_if _TSTF,VTMR0#v(ii)L,,_Z
				_if	_CPF,VTMR0#v(ii)L,,_EQ		;f==w
					bsf	VTMR0INT,#v(ii)
				_endif
			_endif
ii += 1
		endw
	endif
				endm

;***********************************************************************************************************
;
;	VTIMER0 START
;	Starts VTIMER0 processing.
;	The lowest VTIMER0 value is used for calculating the real timer value (TMR0).
;	Every enabled VTIMER0 is reduced by this value. So at least one VTIMER0 is set to zero. 
;	Parameter
;	Requires
;		BANK 0
;		VTIMER0
;		VTIMER0_INIT
;	Changes
;		W
;
;***********************************************************************************************************
VTIMER0_START		macro
	ifndef	_VTIMER0_INIT_DONE_
		error "VTIMER0 not initialized, yet. Call VTIMER0_INIT, first."
	else
		_if		_BTF,INTCON,T0IF,_C
 			movlw	H'00'
			local	ii = 0
ii = 0
			while	ii < vtimer0_count
				_if _BTF,VTMR0CON,#v(ii),_S			;if VTIMER(ii) is on?
					_if _CPF,VTMR0#v(ii)L,,_GT		;if biggest timer value?
						movf	VTMR0#v(ii)L,w		;take it
					_endif
				_endif
ii += 1
			endw
			movwf	TMR0							;insert biggest timer value
			subwf	VTMR0CONST_FF,w					;w=f-w
ii = 0
			while	ii < vtimer0_count
				_if _BTF,VTMR0CON,#v(ii),_S			;if VTIMER(ii) is on?
					addwf	VTMR0#v(ii)L,f			;calculate new value
				_endif
ii += 1
			endw
		_endif
	endif
				endm


;***********************************************************************************************************
;
;	VTIMER0 SUSPEND
;	Suspends all enabled VTIMER0. If the real TMR0 hasn't expired, yet, the remaining time is added to every
;	enabled VTIMER0. After this procedure VTIMER0 can be altered (start a new tiner or change a running timer).
;	A VTIMER0_START must be issued before real TMR0 is started, again.
;	
;	Parameter
;	Requires
;		BANK 0
;		VTIMER0
;		VTIMER0_INIT
;	Changes
;		W
;
;***********************************************************************************************************
VTIMER0_SUSPEND		macro
	ifndef	_VTIMER0_INIT_DONE_
		error "VTIMER0 not initialized, yet. Call VTIMER0_INIT, first."
	else
		_if		_BTF,INTCON,T0IF,_C
			movf	TMR0,w
			subwf	VTMR0CONST_FF,w					;calculate remaining time (w=f-w)
			local	ii = 0
			while	ii < vtimer0_count	
				subwf VTMR0#v(ii)L,f				;calculate new value	
ii += 1
			endw
		_endif
	endif
				endm




VTIMER1			macro	count
	ifdef			_VTIMER1_MEMORY_ALLOC_DONE_
				error "VTIMER1 memory section already created."
	else
		if			count < 1
				error "VTIMER1 count must be greater than 0."
		else
			if		count > 8
				error "VTIMER1 count must not be greater than 8"
			else
				constant		vtimer1_count=count
VTMR1UDATA		UDATA_SHR
VTMR1CON		RES		1
VTMR1INT		RES		1
VTMR1LWORK		RES		1
VTMR1HWORK		RES		1
				local ii = 0
				while ii < count
VTMR1#v(ii)L		RES		1
VTMR1#v(ii)H		RES		1
ii += 1
				endw
				#define	_VTIMER1_MEMORY_ALLOC_DONE_
			endif
		endif
	endif
				endm

VTIMER1_INIT	macro
	ifndef	_VTIMER1_MEMORY_ALLOC_DONE_
		error "No memory for VTIMER1 available. Call VTIMER1, first."
	else
		ifdef	_VTIMER1_INIT_DONE_
			error "VTIMER1 initialization already done"
		else
			banksel	VTMR1CON
			clrf	VTMR1CON
			clrf	VTMR1INT
			local ii = 0
			while ii < vtimer1_count
				clrf VTMR1#v(ii)L
				clrf VTMR1#v(ii)H
ii += 1
				endw
			#define	_VTIMER1_INIT_DONE_	
		endif
	endif
				endm

VTIMER1_ON		macro	TIMER_ID,TMRL,TMRH
	ifndef	_VTIMER1_MEMORY_ALLOC_DONE_
		error "No memory for VTIMER1 available. Call VTIMER1, first."
	else
		if TIMER_ID >= vtimer1_count
			error	"TIMER_ID " #v(TIMER_ID) " not available. VTIMER1 TIMER_ID must be lower than "#v(vtimer1_count)"."
		else
			if TIMER_ID < 0
				error "VTIMER1 TIMER_ID must be greater than or equal 0."
			else
				movf	TMRL,w
				movwf	VTMR1#v(TIMER_ID)L
				movf	TMRH,w
				movwf	VTMR1#v(TIMER_ID)H
				bsf		VTMR1CON,#v(TIMER_ID)
			endif
		endif
	endif
				endm

VTIMER1_OFF		macro	TIMER_ID
	ifndef	_VTIMER1_MEMORY_ALLOC_DONE_
		error "No memory for VTIMER1 available. Call VTIMER1, first."
	else
		if TIMER_ID >= vtimer1_count
			error	"TIMER_ID " #v(TIMER_ID) " not available. VTIMER0 TIMER_ID must be lower than "#v(vtimer1_count)"."
		else
			if TIMER_ID < 0
				error "VTIMER1 TIMER_ID must be greater than or equal 0."
			else
				bcf	VTMR1CON,TIMER_ID
			endif
		endif
	endif
				endm


VTIMER1_CHECK		macro
	ifndef	_VTIMER1_MEMORY_ALLOC_DONE_
		error "No memory for VTIMER1 available. Call VTIMER1, first."
	else
		local	ii = 0
		while	ii < vtimer1_count
			movlw	H'FF'
			_if _BTF,VTMR1CON,#v(ii),_S
				_if _CPF,VTMR1#v(ii)H,,_EQ		;f==w
					_if _CPF,VTMR1#v(ii)L,,_EQ		;f==w
						bsf	VTMR1INT,#v(ii)
					_endif
				_endif
			_endif
ii += 1
		endw
	endif
				endm

VTIMER1_START		macro
	ifndef	_VTIMER1_MEMORY_ALLOC_DONE_
		error "No memory for VTIMER1 available. Call VTIMER1, first."
	else
		banksel	PIR1
		_if		_BTF,PIR1,TMR1IF,_C						;only, if no timer1 interrput
			
			movlw	H'FF'
			movwf	VTMR1LWORK
			movwf	VTMR1HWORK

			movlw	H'00'
			movwf	TMR1L
			local	ii = 0
ii = 0
			while	ii < vtimer1_count
				_if _BTF,VTMR1CON,#v(ii),_S
					_if	_CPF,VTMR1#v(ii)H,,_GT			;f>w
						movf	VTMR1#v(ii)L,w			;save value
						movwf	TMR1L					;in TMR1L
						movf	VTMR1#v(ii)H,w
					_elseif _CPF,VTMR1#v(ii)H,,_EQ		;f==w
						movf	TMR1L,w
						_if _CPF,VTMR1#v(ii)L,,_GT		;f>w
							movf	VTMR1#v(ii)L,w		;save value
							movwf	TMR1L				;in TMR1L
						_endif
						movf VTMR1#v(ii)H,w
					_endif
				_endif
ii += 1
			endw
			movwf	TMR1H			;VTIMERH ->TMR1H
			subwf	VTMR1HWORK,f	;f=f-w	=> VTMR1HWORK=FF-VTIMERH
			movf	TMR1L,w
			subwf	VTMR1LWORK,f	;f=f-w	=> VTMR1LWORK=FF-VTIMERL

ii = 0
			while	ii < vtimer1_count
				_if _BTF,VTMR1CON,#v(ii),_S
					movf	VTMR1LWORK,w
					addwf	VTMR1#v(ii)L,f		;f=f+w
					_if _BTF,STATUS,C,_S
						incf	VTMR1#v(ii)H,f
					_endif
					movf	VTMR1HWORK,w
					addwf	VTMR1#v(ii)H,f		;f=f+w
				_endif
ii += 1
			endw
		_endif
	endif
				endm


VTIMER1_SUSPEND		macro
	ifndef	_VTIMER1_INIT_DONE_
		error "VTIMER1 not initialized, yet. Call VTIMER1_INIT, first."
	else
		banksel	PIR1
		_if		_BTF,PIR1,TMR1IF,_C						;only, if no timer1 interrput
			movlw	H'FF'								;save max. value FF
			movwf	VTMR1HWORK							;in work fields
			movwf	VTMR1LWORK							;

			movf	TMR1L,w								;get content of TMR1L
			subwf	VTMR1LWORK,f						;f=f-w (calculate remaining: VTMR1LWORK = FF - TMR1L)
			movf	TMR1H,w								;get content of TMR1H
			subwf	VTMR1HWORK,f						;f=f-w (calculate remaining: VTMR1HWORK = FF - TMR1H)
			local	ii = 0
ii = 0
			while	ii < vtimer1_count
				_if _BTF,VTMR1CON,#v(ii),_S				;if vtimer is enabled
					movf	VTMR1LWORK,w
					subwf	VTMR1#v(ii)L,f
					_if _BTF,STATUS,C,_C
						decf	VTMR1#v(ii)H,f
					_endif
					movf	VTMR1HWORK,w
					subwf	VTMR1#v(ii)H,f
				_endif
ii += 1
			endw
		_endif
	endif
				endm

	endif