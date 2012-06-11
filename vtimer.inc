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

VTIMER0_INIT	macro
	ifndef	_VTIMER0_MEMORY_ALLOC_DONE_
		error "No memory for VTIMER0 available. Call VTIMER0, first."
	else
		ifdef	_VTIMER0_INIT_DONE_
			error "VTIMER0 initialization already done"
		else
			banksel	VTMR0CON
			clrf	VTMR0CON
			clrf	VTMR0INT
			local ii = 0
			while ii < vtimer0_count
				clrf VTMR0#v(ii)L
ii += 1
				endw
			#define	_VTIMER0_INIT_DONE_	
		endif
	endif
				endm

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

VTIMER0_CHECK		macro
	ifndef	_VTIMER0_INIT_DONE_
		error "VTIMER0 not initialized, yet. Call VTIMER0_INIT, first."
	else
		local	ii = 0
		while	ii < vtimer0_count
			_if _BTF,VTMR0CON,#v(ii),_S
				_if _TSTF,VTMR0#v(ii)L,,_Z
					bsf	VTMR0INT,#v(ii)
				_endif
			_endif
ii += 1
		endw
	endif
				endm

VTIMER0_START		macro
	ifndef	_VTIMER0_INIT_DONE_
		error "VTIMER0 not initialized, yet. Call VTIMER0_INIT, first."
	else
		movlw	H'FF'
		local	ii = 0
ii = 0
		while	ii < vtimer0_count
			_if _BTF,VTMR0CON,#v(ii),_S
				_if _CPF,VTMR0#v(ii)L,,_LT
					movf	VTMR0#v(ii)L,w
				_endif
			_endif
ii += 1
		endw
		banksel	TMR0
		movwf	TMR0
		movlw	H'FF'
		SWAPWF	TMR0
		subwf	TMR0,f
ii = 0
		while	ii < vtimer0_count
			_if _BTF,VTMR0CON,#v(ii),_S
				subwf	VTMR0#v(ii)L,f
			_endif
ii += 1
		endw
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
			_if _BTF,VTMR1CON,#v(ii),_S
				_if	_TSTF,VTMR1#v(ii)H,,_Z
					_if _TSTF,VTMR1#v(ii)L,,_Z
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
		movlw	H'FF'
		local	ii = 0
ii = 0
		while	ii < vtimer1_count
			_if _BTF,VTMR1CON,#v(ii),_S
				_if	_CPF,VTMR1#v(ii)H,,_LT			;f<w
					movf	VTMR1#v(ii)L,w			;save value
					movwf	VTMR1LWORK				;in vtmr1 working register
					movf	VTMR1#v(ii)H,w
				_elseif _CPF,VTMR1#v(ii)H,,_EQ		;f==w
					movf	VTMR1LWORK,w
					_if _CPF,VTMR1#v(ii)L,,_LT		;f<w
						movf	VTMR1#v(ii)L,w		;save value
						movwf	VTMR1LWORK			;in vtmr1 working register
					_endif
					movf VTMR1#v(ii)H,w
				_endif
			_endif
ii += 1
		endw
		banksel	TMR1H
		movwf	TMR1H
		movlw	H'FF'
		SWAPWF	TMR1H
		subwf	TMR1H,f
		
		SWAPWF	VTMR1LWORK		;save TMR1H and load TMR1L to w	
		movwf	TMR1L
		movlw	H'FF'
		SWAPWF	TMR1L			;FF to TMR1L and TMR1L to w
		subwf	TMR1L,f			;FF-w

ii = 0
		while	ii < vtimer1_count
			_if _BTF,VTMR1CON,#v(ii),_S
				subwf	VTMR1#v(ii)L,f		;f=f-w
				_if _BTF,STATUS,C,_C
					decf	VTMR1#v(ii)H,f
				_endif
				SWAPWF	VTMR1LWORK			;save w to f and get TMR1H
				subwf	VTMR1#v(ii)H,f		;f=f-w
				SWAPWF	VTMR1LWORK
			_endif
ii += 1
		endw
	endif
				endm



	endif