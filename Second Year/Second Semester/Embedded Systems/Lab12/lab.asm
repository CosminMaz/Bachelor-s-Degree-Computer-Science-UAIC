;--------------------------------------------------------
; File Created by SDCC : free open source ISO C Compiler
; Version 4.5.0 #15242 (MINGW64)
;--------------------------------------------------------
	.module lab
	
	.optsdcc -mmcs51 --model-small
;--------------------------------------------------------
; Public variables in this module
;--------------------------------------------------------
	.globl _main
	.globl _CY
	.globl _AC
	.globl _F0
	.globl _RS1
	.globl _RS0
	.globl _OV
	.globl _F1
	.globl _P
	.globl _PS
	.globl _PT1
	.globl _PX1
	.globl _PT0
	.globl _PX0
	.globl _RD
	.globl _WR
	.globl _T1
	.globl _T0
	.globl _INT1
	.globl _INT0
	.globl _TXD
	.globl _RXD
	.globl _P3_7
	.globl _P3_6
	.globl _P3_5
	.globl _P3_4
	.globl _P3_3
	.globl _P3_2
	.globl _P3_1
	.globl _P3_0
	.globl _EA
	.globl _ES
	.globl _ET1
	.globl _EX1
	.globl _ET0
	.globl _EX0
	.globl _P2_7
	.globl _P2_6
	.globl _P2_5
	.globl _P2_4
	.globl _P2_3
	.globl _P2_2
	.globl _P2_1
	.globl _P2_0
	.globl _SM0
	.globl _SM1
	.globl _SM2
	.globl _REN
	.globl _TB8
	.globl _RB8
	.globl _TI
	.globl _RI
	.globl _P1_7
	.globl _P1_6
	.globl _P1_5
	.globl _P1_4
	.globl _P1_3
	.globl _P1_2
	.globl _P1_1
	.globl _P1_0
	.globl _TF1
	.globl _TR1
	.globl _TF0
	.globl _TR0
	.globl _IE1
	.globl _IT1
	.globl _IE0
	.globl _IT0
	.globl _P0_7
	.globl _P0_6
	.globl _P0_5
	.globl _P0_4
	.globl _P0_3
	.globl _P0_2
	.globl _P0_1
	.globl _P0_0
	.globl _B
	.globl _ACC
	.globl _PSW
	.globl _IP
	.globl _P3
	.globl _IE
	.globl _P2
	.globl _SBUF
	.globl _SCON
	.globl _P1
	.globl _TH1
	.globl _TH0
	.globl _TL1
	.globl _TL0
	.globl _TMOD
	.globl _TCON
	.globl _PCON
	.globl _DPH
	.globl _DPL
	.globl _SP
	.globl _P0
	.globl _i
	.globl _x
	.globl _c
;--------------------------------------------------------
; special function registers
;--------------------------------------------------------
	.area RSEG    (ABS,DATA)
	.org 0x0000
_P0	=	0x0080
_SP	=	0x0081
_DPL	=	0x0082
_DPH	=	0x0083
_PCON	=	0x0087
_TCON	=	0x0088
_TMOD	=	0x0089
_TL0	=	0x008a
_TL1	=	0x008b
_TH0	=	0x008c
_TH1	=	0x008d
_P1	=	0x0090
_SCON	=	0x0098
_SBUF	=	0x0099
_P2	=	0x00a0
_IE	=	0x00a8
_P3	=	0x00b0
_IP	=	0x00b8
_PSW	=	0x00d0
_ACC	=	0x00e0
_B	=	0x00f0
;--------------------------------------------------------
; special function bits
;--------------------------------------------------------
	.area RSEG    (ABS,DATA)
	.org 0x0000
_P0_0	=	0x0080
_P0_1	=	0x0081
_P0_2	=	0x0082
_P0_3	=	0x0083
_P0_4	=	0x0084
_P0_5	=	0x0085
_P0_6	=	0x0086
_P0_7	=	0x0087
_IT0	=	0x0088
_IE0	=	0x0089
_IT1	=	0x008a
_IE1	=	0x008b
_TR0	=	0x008c
_TF0	=	0x008d
_TR1	=	0x008e
_TF1	=	0x008f
_P1_0	=	0x0090
_P1_1	=	0x0091
_P1_2	=	0x0092
_P1_3	=	0x0093
_P1_4	=	0x0094
_P1_5	=	0x0095
_P1_6	=	0x0096
_P1_7	=	0x0097
_RI	=	0x0098
_TI	=	0x0099
_RB8	=	0x009a
_TB8	=	0x009b
_REN	=	0x009c
_SM2	=	0x009d
_SM1	=	0x009e
_SM0	=	0x009f
_P2_0	=	0x00a0
_P2_1	=	0x00a1
_P2_2	=	0x00a2
_P2_3	=	0x00a3
_P2_4	=	0x00a4
_P2_5	=	0x00a5
_P2_6	=	0x00a6
_P2_7	=	0x00a7
_EX0	=	0x00a8
_ET0	=	0x00a9
_EX1	=	0x00aa
_ET1	=	0x00ab
_ES	=	0x00ac
_EA	=	0x00af
_P3_0	=	0x00b0
_P3_1	=	0x00b1
_P3_2	=	0x00b2
_P3_3	=	0x00b3
_P3_4	=	0x00b4
_P3_5	=	0x00b5
_P3_6	=	0x00b6
_P3_7	=	0x00b7
_RXD	=	0x00b0
_TXD	=	0x00b1
_INT0	=	0x00b2
_INT1	=	0x00b3
_T0	=	0x00b4
_T1	=	0x00b5
_WR	=	0x00b6
_RD	=	0x00b7
_PX0	=	0x00b8
_PT0	=	0x00b9
_PX1	=	0x00ba
_PT1	=	0x00bb
_PS	=	0x00bc
_P	=	0x00d0
_F1	=	0x00d1
_OV	=	0x00d2
_RS0	=	0x00d3
_RS1	=	0x00d4
_F0	=	0x00d5
_AC	=	0x00d6
_CY	=	0x00d7
;--------------------------------------------------------
; overlayable register banks
;--------------------------------------------------------
	.area REG_BANK_0	(REL,OVR,DATA)
	.ds 8
;--------------------------------------------------------
; internal ram data
;--------------------------------------------------------
	.area DSEG    (DATA)
_c::
	.ds 10
_x::
	.ds 8
_i::
	.ds 2
;--------------------------------------------------------
; overlayable items in internal ram
;--------------------------------------------------------
	.area	OSEG    (OVR,DATA)
;--------------------------------------------------------
; Stack segment in internal ram
;--------------------------------------------------------
	.area SSEG
__start__stack:
	.ds	1

;--------------------------------------------------------
; indirectly addressable internal ram data
;--------------------------------------------------------
	.area ISEG    (DATA)
;--------------------------------------------------------
; absolute internal ram data
;--------------------------------------------------------
	.area IABS    (ABS,DATA)
	.area IABS    (ABS,DATA)
;--------------------------------------------------------
; bit data
;--------------------------------------------------------
	.area BSEG    (BIT)
;--------------------------------------------------------
; paged external ram data
;--------------------------------------------------------
	.area PSEG    (PAG,XDATA)
;--------------------------------------------------------
; uninitialized external ram data
;--------------------------------------------------------
	.area XSEG    (XDATA)
;--------------------------------------------------------
; absolute external ram data
;--------------------------------------------------------
	.area XABS    (ABS,XDATA)
;--------------------------------------------------------
; initialized external ram data
;--------------------------------------------------------
	.area XISEG   (XDATA)
	.area HOME    (CODE)
	.area GSINIT0 (CODE)
	.area GSINIT1 (CODE)
	.area GSINIT2 (CODE)
	.area GSINIT3 (CODE)
	.area GSINIT4 (CODE)
	.area GSINIT5 (CODE)
	.area GSINIT  (CODE)
	.area GSFINAL (CODE)
	.area CSEG    (CODE)
;--------------------------------------------------------
; interrupt vector
;--------------------------------------------------------
	.area HOME    (CODE)
__interrupt_vect:
	ljmp	__sdcc_gsinit_startup
; restartable atomic support routines
	.ds	5
sdcc_atomic_exchange_rollback_start::
	nop
	nop
sdcc_atomic_exchange_pdata_impl:
	movx	a, @r0
	mov	r3, a
	mov	a, r2
	movx	@r0, a
	sjmp	sdcc_atomic_exchange_exit
	nop
	nop
sdcc_atomic_exchange_xdata_impl:
	movx	a, @dptr
	mov	r3, a
	mov	a, r2
	movx	@dptr, a
	sjmp	sdcc_atomic_exchange_exit
sdcc_atomic_compare_exchange_idata_impl:
	mov	a, @r0
	cjne	a, ar2, .+#5
	mov	a, r3
	mov	@r0, a
	ret
	nop
sdcc_atomic_compare_exchange_pdata_impl:
	movx	a, @r0
	cjne	a, ar2, .+#5
	mov	a, r3
	movx	@r0, a
	ret
	nop
sdcc_atomic_compare_exchange_xdata_impl:
	movx	a, @dptr
	cjne	a, ar2, .+#5
	mov	a, r3
	movx	@dptr, a
	ret
sdcc_atomic_exchange_rollback_end::

sdcc_atomic_exchange_gptr_impl::
	jnb	b.6, sdcc_atomic_exchange_xdata_impl
	mov	r0, dpl
	jb	b.5, sdcc_atomic_exchange_pdata_impl
sdcc_atomic_exchange_idata_impl:
	mov	a, r2
	xch	a, @r0
	mov	dpl, a
	ret
sdcc_atomic_exchange_exit:
	mov	dpl, r3
	ret
sdcc_atomic_compare_exchange_gptr_impl::
	jnb	b.6, sdcc_atomic_compare_exchange_xdata_impl
	mov	r0, dpl
	jb	b.5, sdcc_atomic_compare_exchange_pdata_impl
	sjmp	sdcc_atomic_compare_exchange_idata_impl
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area HOME    (CODE)
	.area GSINIT  (CODE)
	.area GSFINAL (CODE)
	.area GSINIT  (CODE)
	.globl __sdcc_gsinit_startup
	.globl __sdcc_program_startup
	.globl __start__stack
	.globl __mcs51_genXINIT
	.globl __mcs51_genXRAMCLEAR
	.globl __mcs51_genRAMCLEAR
	.area GSFINAL (CODE)
	ljmp	__sdcc_program_startup
;--------------------------------------------------------
; Home
;--------------------------------------------------------
	.area HOME    (CODE)
	.area HOME    (CODE)
__sdcc_program_startup:
	ljmp	_main
;	return from main will return to caller
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG    (CODE)
;------------------------------------------------------------
;Allocation info for local variables in function 'main'
;------------------------------------------------------------
;n             Allocated to registers r6 r7 
;------------------------------------------------------------
;	lab.c:8: void main(void) {
;	-----------------------------------------
;	 function main
;	-----------------------------------------
_main:
	ar7 = 0x07
	ar6 = 0x06
	ar5 = 0x05
	ar4 = 0x04
	ar3 = 0x03
	ar2 = 0x02
	ar1 = 0x01
	ar0 = 0x00
;	lab.c:9: c[0] = 0b11000000;
	mov	_c,#0xc0
;	lab.c:10: c[1] = 0b11111001;
	mov	(_c + 0x0001),#0xf9
;	lab.c:11: c[2] = 0b11011011;
	mov	(_c + 0x0002),#0xdb
;	lab.c:12: c[3] = 0b10110000;
	mov	(_c + 0x0003),#0xb0
;	lab.c:13: c[4] = 0b10011010;
	mov	(_c + 0x0004),#0x9a
;	lab.c:14: c[5] = 0b10010010;
	mov	(_c + 0x0005),#0x92
;	lab.c:15: c[6] = 0b10000010;
	mov	(_c + 0x0006),#0x82
;	lab.c:16: c[7] = 0b11111000;
	mov	(_c + 0x0007),#0xf8
;	lab.c:17: c[8] = 0b10000000;
	mov	(_c + 0x0008),#0x80
;	lab.c:18: c[9] = 0b10010000;
	mov	(_c + 0x0009),#0x90
;	lab.c:21: i = 0;
	clr	a
	mov	_i,a
	mov	(_i + 1),a
;	lab.c:23: x[0] = 0;
	mov	(_x + 0),a
	mov	(_x + 1),a
;	lab.c:24: x[1] = 0;
	mov	((_x + 0x0002) + 0),a
	mov	((_x + 0x0002) + 1),a
;	lab.c:25: x[2] = 0;
	mov	((_x + 0x0004) + 0),a
	mov	((_x + 0x0004) + 1),a
;	lab.c:26: x[3] = 0;
	mov	((_x + 0x0006) + 0),a
	mov	((_x + 0x0006) + 1),a
;	lab.c:28: while(n > 1000) {
	mov	r6,#0xd7
	mov	r7,#0x09
00101$:
	clr	c
	mov	a,#0xe8
	subb	a,r6
	mov	a,#(0x03 ^ 0x80)
	mov	b,r7
	xrl	b,#0x80
	subb	a,b
	jnc	00121$
;	lab.c:29: x[0] = x[0] + 1;
	mov	a,#0x01
	add	a, _x
	mov	r4,a
	clr	a
	addc	a, (_x + 1)
	mov	r5,a
	mov	(_x + 0),r4
	mov	(_x + 1),r5
;	lab.c:30: n = n - 1000;
	mov	a,r6
	add	a,#0x18
	mov	r6,a
	mov	a,r7
	addc	a,#0xfc
	mov	r7,a
;	lab.c:32: while(n > 100) {
	sjmp	00101$
00121$:
00104$:
	clr	c
	mov	a,#0x64
	subb	a,r6
	mov	a,#(0x00 ^ 0x80)
	mov	b,r7
	xrl	b,#0x80
	subb	a,b
	jnc	00123$
;	lab.c:33: x[1] = x[1] + 1;
	mov	a,#0x01
	add	a, (_x + 0x0002)
	mov	r4,a
	clr	a
	addc	a, ((_x + 0x0002) + 1)
	mov	r5,a
	mov	((_x + 0x0002) + 0),r4
	mov	((_x + 0x0002) + 1),r5
;	lab.c:34: n = n - 100;
	mov	a,r6
	add	a,#0x9c
	mov	r6,a
	mov	a,r7
	addc	a,#0xff
	mov	r7,a
;	lab.c:36: while(n > 10) {
	sjmp	00104$
00123$:
00107$:
	clr	c
	mov	a,#0x0a
	subb	a,r6
	mov	a,#(0x00 ^ 0x80)
	mov	b,r7
	xrl	b,#0x80
	subb	a,b
	jnc	00125$
;	lab.c:37: x[2] = x[2] + 1;
	mov	a,#0x01
	add	a, (_x + 0x0004)
	mov	r4,a
	clr	a
	addc	a, ((_x + 0x0004) + 1)
	mov	r5,a
	mov	((_x + 0x0004) + 0),r4
	mov	((_x + 0x0004) + 1),r5
;	lab.c:38: n = n - 10;
	mov	a,r6
	add	a,#0xf6
	mov	r6,a
	mov	a,r7
	addc	a,#0xff
	mov	r7,a
;	lab.c:40: while(n > 1) {
	sjmp	00107$
00125$:
00110$:
	clr	c
	mov	a,#0x01
	subb	a,r6
	mov	a,#(0x00 ^ 0x80)
	mov	b,r7
	xrl	b,#0x80
	subb	a,b
	jnc	00114$
;	lab.c:41: x[3] = x[3] + 1;
	mov	a,#0x01
	add	a, (_x + 0x0006)
	mov	r4,a
	clr	a
	addc	a, ((_x + 0x0006) + 1)
	mov	r5,a
	mov	((_x + 0x0006) + 0),r4
	mov	((_x + 0x0006) + 1),r5
;	lab.c:42: n = n - 1;
	dec	r6
	cjne	r6,#0xff,00180$
	dec	r7
00180$:
;	lab.c:45: while(1) {
	sjmp	00110$
00114$:
;	lab.c:46: P0_7 = 1;
;	assignBit
	setb	_P0_7
;	lab.c:47: P3_3 = 0;
;	assignBit
	clr	_P3_3
;	lab.c:48: P3_4 = 0;
;	assignBit
	clr	_P3_4
;	lab.c:49: P1 = c[x[1]];
	mov	a,(_x + 0x0002)
	add	a, #_c
	mov	r1,a
	mov	_P1,@r1
;	lab.c:50: P3_3 = 0;
;	assignBit
	clr	_P3_3
;	lab.c:51: P3_4 = 0;
;	assignBit
	clr	_P3_4
;	lab.c:52: P1 = c[x[1]];
	mov	a,(_x + 0x0002)
	add	a, #_c
	mov	r1,a
	mov	_P1,@r1
;	lab.c:53: P3_3 = 1;
;	assignBit
	setb	_P3_3
;	lab.c:54: P3_4 = 0;
;	assignBit
	clr	_P3_4
;	lab.c:55: P1 = c[x[1]];
	mov	a,(_x + 0x0002)
	add	a, #_c
	mov	r1,a
	mov	_P1,@r1
;	lab.c:56: P3_3 = 0;
;	assignBit
	clr	_P3_3
;	lab.c:57: P3_4 = 1;
;	assignBit
	setb	_P3_4
;	lab.c:58: P1 = c[x[1]];
	mov	a,(_x + 0x0002)
	add	a, #_c
	mov	r1,a
	mov	_P1,@r1
;	lab.c:59: P3_3 = 1;
;	assignBit
	setb	_P3_3
;	lab.c:60: P3_4 = 1;
;	assignBit
	setb	_P3_4
;	lab.c:62: }
	sjmp	00114$
	.area CSEG    (CODE)
	.area CONST   (CODE)
	.area XINIT   (CODE)
	.area CABS    (ABS,CODE)
