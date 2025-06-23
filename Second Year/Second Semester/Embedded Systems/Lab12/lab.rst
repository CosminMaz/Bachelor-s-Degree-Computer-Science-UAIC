                                      1 ;--------------------------------------------------------
                                      2 ; File Created by SDCC : free open source ISO C Compiler
                                      3 ; Version 4.5.0 #15242 (MINGW64)
                                      4 ;--------------------------------------------------------
                                      5 	.module lab
                                      6 	
                                      7 	.optsdcc -mmcs51 --model-small
                                      8 ;--------------------------------------------------------
                                      9 ; Public variables in this module
                                     10 ;--------------------------------------------------------
                                     11 	.globl _main
                                     12 	.globl _CY
                                     13 	.globl _AC
                                     14 	.globl _F0
                                     15 	.globl _RS1
                                     16 	.globl _RS0
                                     17 	.globl _OV
                                     18 	.globl _F1
                                     19 	.globl _P
                                     20 	.globl _PS
                                     21 	.globl _PT1
                                     22 	.globl _PX1
                                     23 	.globl _PT0
                                     24 	.globl _PX0
                                     25 	.globl _RD
                                     26 	.globl _WR
                                     27 	.globl _T1
                                     28 	.globl _T0
                                     29 	.globl _INT1
                                     30 	.globl _INT0
                                     31 	.globl _TXD
                                     32 	.globl _RXD
                                     33 	.globl _P3_7
                                     34 	.globl _P3_6
                                     35 	.globl _P3_5
                                     36 	.globl _P3_4
                                     37 	.globl _P3_3
                                     38 	.globl _P3_2
                                     39 	.globl _P3_1
                                     40 	.globl _P3_0
                                     41 	.globl _EA
                                     42 	.globl _ES
                                     43 	.globl _ET1
                                     44 	.globl _EX1
                                     45 	.globl _ET0
                                     46 	.globl _EX0
                                     47 	.globl _P2_7
                                     48 	.globl _P2_6
                                     49 	.globl _P2_5
                                     50 	.globl _P2_4
                                     51 	.globl _P2_3
                                     52 	.globl _P2_2
                                     53 	.globl _P2_1
                                     54 	.globl _P2_0
                                     55 	.globl _SM0
                                     56 	.globl _SM1
                                     57 	.globl _SM2
                                     58 	.globl _REN
                                     59 	.globl _TB8
                                     60 	.globl _RB8
                                     61 	.globl _TI
                                     62 	.globl _RI
                                     63 	.globl _P1_7
                                     64 	.globl _P1_6
                                     65 	.globl _P1_5
                                     66 	.globl _P1_4
                                     67 	.globl _P1_3
                                     68 	.globl _P1_2
                                     69 	.globl _P1_1
                                     70 	.globl _P1_0
                                     71 	.globl _TF1
                                     72 	.globl _TR1
                                     73 	.globl _TF0
                                     74 	.globl _TR0
                                     75 	.globl _IE1
                                     76 	.globl _IT1
                                     77 	.globl _IE0
                                     78 	.globl _IT0
                                     79 	.globl _P0_7
                                     80 	.globl _P0_6
                                     81 	.globl _P0_5
                                     82 	.globl _P0_4
                                     83 	.globl _P0_3
                                     84 	.globl _P0_2
                                     85 	.globl _P0_1
                                     86 	.globl _P0_0
                                     87 	.globl _B
                                     88 	.globl _ACC
                                     89 	.globl _PSW
                                     90 	.globl _IP
                                     91 	.globl _P3
                                     92 	.globl _IE
                                     93 	.globl _P2
                                     94 	.globl _SBUF
                                     95 	.globl _SCON
                                     96 	.globl _P1
                                     97 	.globl _TH1
                                     98 	.globl _TH0
                                     99 	.globl _TL1
                                    100 	.globl _TL0
                                    101 	.globl _TMOD
                                    102 	.globl _TCON
                                    103 	.globl _PCON
                                    104 	.globl _DPH
                                    105 	.globl _DPL
                                    106 	.globl _SP
                                    107 	.globl _P0
                                    108 	.globl _i
                                    109 	.globl _x
                                    110 	.globl _c
                                    111 ;--------------------------------------------------------
                                    112 ; special function registers
                                    113 ;--------------------------------------------------------
                                    114 	.area RSEG    (ABS,DATA)
      000000                        115 	.org 0x0000
                           000080   116 _P0	=	0x0080
                           000081   117 _SP	=	0x0081
                           000082   118 _DPL	=	0x0082
                           000083   119 _DPH	=	0x0083
                           000087   120 _PCON	=	0x0087
                           000088   121 _TCON	=	0x0088
                           000089   122 _TMOD	=	0x0089
                           00008A   123 _TL0	=	0x008a
                           00008B   124 _TL1	=	0x008b
                           00008C   125 _TH0	=	0x008c
                           00008D   126 _TH1	=	0x008d
                           000090   127 _P1	=	0x0090
                           000098   128 _SCON	=	0x0098
                           000099   129 _SBUF	=	0x0099
                           0000A0   130 _P2	=	0x00a0
                           0000A8   131 _IE	=	0x00a8
                           0000B0   132 _P3	=	0x00b0
                           0000B8   133 _IP	=	0x00b8
                           0000D0   134 _PSW	=	0x00d0
                           0000E0   135 _ACC	=	0x00e0
                           0000F0   136 _B	=	0x00f0
                                    137 ;--------------------------------------------------------
                                    138 ; special function bits
                                    139 ;--------------------------------------------------------
                                    140 	.area RSEG    (ABS,DATA)
      000000                        141 	.org 0x0000
                           000080   142 _P0_0	=	0x0080
                           000081   143 _P0_1	=	0x0081
                           000082   144 _P0_2	=	0x0082
                           000083   145 _P0_3	=	0x0083
                           000084   146 _P0_4	=	0x0084
                           000085   147 _P0_5	=	0x0085
                           000086   148 _P0_6	=	0x0086
                           000087   149 _P0_7	=	0x0087
                           000088   150 _IT0	=	0x0088
                           000089   151 _IE0	=	0x0089
                           00008A   152 _IT1	=	0x008a
                           00008B   153 _IE1	=	0x008b
                           00008C   154 _TR0	=	0x008c
                           00008D   155 _TF0	=	0x008d
                           00008E   156 _TR1	=	0x008e
                           00008F   157 _TF1	=	0x008f
                           000090   158 _P1_0	=	0x0090
                           000091   159 _P1_1	=	0x0091
                           000092   160 _P1_2	=	0x0092
                           000093   161 _P1_3	=	0x0093
                           000094   162 _P1_4	=	0x0094
                           000095   163 _P1_5	=	0x0095
                           000096   164 _P1_6	=	0x0096
                           000097   165 _P1_7	=	0x0097
                           000098   166 _RI	=	0x0098
                           000099   167 _TI	=	0x0099
                           00009A   168 _RB8	=	0x009a
                           00009B   169 _TB8	=	0x009b
                           00009C   170 _REN	=	0x009c
                           00009D   171 _SM2	=	0x009d
                           00009E   172 _SM1	=	0x009e
                           00009F   173 _SM0	=	0x009f
                           0000A0   174 _P2_0	=	0x00a0
                           0000A1   175 _P2_1	=	0x00a1
                           0000A2   176 _P2_2	=	0x00a2
                           0000A3   177 _P2_3	=	0x00a3
                           0000A4   178 _P2_4	=	0x00a4
                           0000A5   179 _P2_5	=	0x00a5
                           0000A6   180 _P2_6	=	0x00a6
                           0000A7   181 _P2_7	=	0x00a7
                           0000A8   182 _EX0	=	0x00a8
                           0000A9   183 _ET0	=	0x00a9
                           0000AA   184 _EX1	=	0x00aa
                           0000AB   185 _ET1	=	0x00ab
                           0000AC   186 _ES	=	0x00ac
                           0000AF   187 _EA	=	0x00af
                           0000B0   188 _P3_0	=	0x00b0
                           0000B1   189 _P3_1	=	0x00b1
                           0000B2   190 _P3_2	=	0x00b2
                           0000B3   191 _P3_3	=	0x00b3
                           0000B4   192 _P3_4	=	0x00b4
                           0000B5   193 _P3_5	=	0x00b5
                           0000B6   194 _P3_6	=	0x00b6
                           0000B7   195 _P3_7	=	0x00b7
                           0000B0   196 _RXD	=	0x00b0
                           0000B1   197 _TXD	=	0x00b1
                           0000B2   198 _INT0	=	0x00b2
                           0000B3   199 _INT1	=	0x00b3
                           0000B4   200 _T0	=	0x00b4
                           0000B5   201 _T1	=	0x00b5
                           0000B6   202 _WR	=	0x00b6
                           0000B7   203 _RD	=	0x00b7
                           0000B8   204 _PX0	=	0x00b8
                           0000B9   205 _PT0	=	0x00b9
                           0000BA   206 _PX1	=	0x00ba
                           0000BB   207 _PT1	=	0x00bb
                           0000BC   208 _PS	=	0x00bc
                           0000D0   209 _P	=	0x00d0
                           0000D1   210 _F1	=	0x00d1
                           0000D2   211 _OV	=	0x00d2
                           0000D3   212 _RS0	=	0x00d3
                           0000D4   213 _RS1	=	0x00d4
                           0000D5   214 _F0	=	0x00d5
                           0000D6   215 _AC	=	0x00d6
                           0000D7   216 _CY	=	0x00d7
                                    217 ;--------------------------------------------------------
                                    218 ; overlayable register banks
                                    219 ;--------------------------------------------------------
                                    220 	.area REG_BANK_0	(REL,OVR,DATA)
      000000                        221 	.ds 8
                                    222 ;--------------------------------------------------------
                                    223 ; internal ram data
                                    224 ;--------------------------------------------------------
                                    225 	.area DSEG    (DATA)
      000008                        226 _c::
      000008                        227 	.ds 10
      000012                        228 _x::
      000012                        229 	.ds 8
      00001A                        230 _i::
      00001A                        231 	.ds 2
                                    232 ;--------------------------------------------------------
                                    233 ; overlayable items in internal ram
                                    234 ;--------------------------------------------------------
                                    235 	.area	OSEG    (OVR,DATA)
                                    236 ;--------------------------------------------------------
                                    237 ; Stack segment in internal ram
                                    238 ;--------------------------------------------------------
                                    239 	.area SSEG
      00001C                        240 __start__stack:
      00001C                        241 	.ds	1
                                    242 
                                    243 ;--------------------------------------------------------
                                    244 ; indirectly addressable internal ram data
                                    245 ;--------------------------------------------------------
                                    246 	.area ISEG    (DATA)
                                    247 ;--------------------------------------------------------
                                    248 ; absolute internal ram data
                                    249 ;--------------------------------------------------------
                                    250 	.area IABS    (ABS,DATA)
                                    251 	.area IABS    (ABS,DATA)
                                    252 ;--------------------------------------------------------
                                    253 ; bit data
                                    254 ;--------------------------------------------------------
                                    255 	.area BSEG    (BIT)
                                    256 ;--------------------------------------------------------
                                    257 ; paged external ram data
                                    258 ;--------------------------------------------------------
                                    259 	.area PSEG    (PAG,XDATA)
                                    260 ;--------------------------------------------------------
                                    261 ; uninitialized external ram data
                                    262 ;--------------------------------------------------------
                                    263 	.area XSEG    (XDATA)
                                    264 ;--------------------------------------------------------
                                    265 ; absolute external ram data
                                    266 ;--------------------------------------------------------
                                    267 	.area XABS    (ABS,XDATA)
                                    268 ;--------------------------------------------------------
                                    269 ; initialized external ram data
                                    270 ;--------------------------------------------------------
                                    271 	.area XISEG   (XDATA)
                                    272 	.area HOME    (CODE)
                                    273 	.area GSINIT0 (CODE)
                                    274 	.area GSINIT1 (CODE)
                                    275 	.area GSINIT2 (CODE)
                                    276 	.area GSINIT3 (CODE)
                                    277 	.area GSINIT4 (CODE)
                                    278 	.area GSINIT5 (CODE)
                                    279 	.area GSINIT  (CODE)
                                    280 	.area GSFINAL (CODE)
                                    281 	.area CSEG    (CODE)
                                    282 ;--------------------------------------------------------
                                    283 ; interrupt vector
                                    284 ;--------------------------------------------------------
                                    285 	.area HOME    (CODE)
      000000                        286 __interrupt_vect:
      000000 02 00 4C         [24]  287 	ljmp	__sdcc_gsinit_startup
                                    288 ; restartable atomic support routines
      000003                        289 	.ds	5
      000008                        290 sdcc_atomic_exchange_rollback_start::
      000008 00               [12]  291 	nop
      000009 00               [12]  292 	nop
      00000A                        293 sdcc_atomic_exchange_pdata_impl:
      00000A E2               [24]  294 	movx	a, @r0
      00000B FB               [12]  295 	mov	r3, a
      00000C EA               [12]  296 	mov	a, r2
      00000D F2               [24]  297 	movx	@r0, a
      00000E 80 2C            [24]  298 	sjmp	sdcc_atomic_exchange_exit
      000010 00               [12]  299 	nop
      000011 00               [12]  300 	nop
      000012                        301 sdcc_atomic_exchange_xdata_impl:
      000012 E0               [24]  302 	movx	a, @dptr
      000013 FB               [12]  303 	mov	r3, a
      000014 EA               [12]  304 	mov	a, r2
      000015 F0               [24]  305 	movx	@dptr, a
      000016 80 24            [24]  306 	sjmp	sdcc_atomic_exchange_exit
      000018                        307 sdcc_atomic_compare_exchange_idata_impl:
      000018 E6               [12]  308 	mov	a, @r0
      000019 B5 02 02         [24]  309 	cjne	a, ar2, .+#5
      00001C EB               [12]  310 	mov	a, r3
      00001D F6               [12]  311 	mov	@r0, a
      00001E 22               [24]  312 	ret
      00001F 00               [12]  313 	nop
      000020                        314 sdcc_atomic_compare_exchange_pdata_impl:
      000020 E2               [24]  315 	movx	a, @r0
      000021 B5 02 02         [24]  316 	cjne	a, ar2, .+#5
      000024 EB               [12]  317 	mov	a, r3
      000025 F2               [24]  318 	movx	@r0, a
      000026 22               [24]  319 	ret
      000027 00               [12]  320 	nop
      000028                        321 sdcc_atomic_compare_exchange_xdata_impl:
      000028 E0               [24]  322 	movx	a, @dptr
      000029 B5 02 02         [24]  323 	cjne	a, ar2, .+#5
      00002C EB               [12]  324 	mov	a, r3
      00002D F0               [24]  325 	movx	@dptr, a
      00002E 22               [24]  326 	ret
      00002F                        327 sdcc_atomic_exchange_rollback_end::
                                    328 
      00002F                        329 sdcc_atomic_exchange_gptr_impl::
      00002F 30 F6 E0         [24]  330 	jnb	b.6, sdcc_atomic_exchange_xdata_impl
      000032 A8 82            [24]  331 	mov	r0, dpl
      000034 20 F5 D3         [24]  332 	jb	b.5, sdcc_atomic_exchange_pdata_impl
      000037                        333 sdcc_atomic_exchange_idata_impl:
      000037 EA               [12]  334 	mov	a, r2
      000038 C6               [12]  335 	xch	a, @r0
      000039 F5 82            [12]  336 	mov	dpl, a
      00003B 22               [24]  337 	ret
      00003C                        338 sdcc_atomic_exchange_exit:
      00003C 8B 82            [24]  339 	mov	dpl, r3
      00003E 22               [24]  340 	ret
      00003F                        341 sdcc_atomic_compare_exchange_gptr_impl::
      00003F 30 F6 E6         [24]  342 	jnb	b.6, sdcc_atomic_compare_exchange_xdata_impl
      000042 A8 82            [24]  343 	mov	r0, dpl
      000044 20 F5 D9         [24]  344 	jb	b.5, sdcc_atomic_compare_exchange_pdata_impl
      000047 80 CF            [24]  345 	sjmp	sdcc_atomic_compare_exchange_idata_impl
                                    346 ;--------------------------------------------------------
                                    347 ; global & static initialisations
                                    348 ;--------------------------------------------------------
                                    349 	.area HOME    (CODE)
                                    350 	.area GSINIT  (CODE)
                                    351 	.area GSFINAL (CODE)
                                    352 	.area GSINIT  (CODE)
                                    353 	.globl __sdcc_gsinit_startup
                                    354 	.globl __sdcc_program_startup
                                    355 	.globl __start__stack
                                    356 	.globl __mcs51_genXINIT
                                    357 	.globl __mcs51_genXRAMCLEAR
                                    358 	.globl __mcs51_genRAMCLEAR
                                    359 	.area GSFINAL (CODE)
      0000A5 02 00 49         [24]  360 	ljmp	__sdcc_program_startup
                                    361 ;--------------------------------------------------------
                                    362 ; Home
                                    363 ;--------------------------------------------------------
                                    364 	.area HOME    (CODE)
                                    365 	.area HOME    (CODE)
      000049                        366 __sdcc_program_startup:
      000049 02 00 A8         [24]  367 	ljmp	_main
                                    368 ;	return from main will return to caller
                                    369 ;--------------------------------------------------------
                                    370 ; code
                                    371 ;--------------------------------------------------------
                                    372 	.area CSEG    (CODE)
                                    373 ;------------------------------------------------------------
                                    374 ;Allocation info for local variables in function 'main'
                                    375 ;------------------------------------------------------------
                                    376 ;n             Allocated to registers r6 r7 
                                    377 ;------------------------------------------------------------
                                    378 ;	lab.c:8: void main(void) {
                                    379 ;	-----------------------------------------
                                    380 ;	 function main
                                    381 ;	-----------------------------------------
      0000A8                        382 _main:
                           000007   383 	ar7 = 0x07
                           000006   384 	ar6 = 0x06
                           000005   385 	ar5 = 0x05
                           000004   386 	ar4 = 0x04
                           000003   387 	ar3 = 0x03
                           000002   388 	ar2 = 0x02
                           000001   389 	ar1 = 0x01
                           000000   390 	ar0 = 0x00
                                    391 ;	lab.c:9: c[0] = 0b11000000;
      0000A8 75 08 C0         [24]  392 	mov	_c,#0xc0
                                    393 ;	lab.c:10: c[1] = 0b11111001;
      0000AB 75 09 F9         [24]  394 	mov	(_c + 0x0001),#0xf9
                                    395 ;	lab.c:11: c[2] = 0b11011011;
      0000AE 75 0A DB         [24]  396 	mov	(_c + 0x0002),#0xdb
                                    397 ;	lab.c:12: c[3] = 0b10110000;
      0000B1 75 0B B0         [24]  398 	mov	(_c + 0x0003),#0xb0
                                    399 ;	lab.c:13: c[4] = 0b10011010;
      0000B4 75 0C 9A         [24]  400 	mov	(_c + 0x0004),#0x9a
                                    401 ;	lab.c:14: c[5] = 0b10010010;
      0000B7 75 0D 92         [24]  402 	mov	(_c + 0x0005),#0x92
                                    403 ;	lab.c:15: c[6] = 0b10000010;
      0000BA 75 0E 82         [24]  404 	mov	(_c + 0x0006),#0x82
                                    405 ;	lab.c:16: c[7] = 0b11111000;
      0000BD 75 0F F8         [24]  406 	mov	(_c + 0x0007),#0xf8
                                    407 ;	lab.c:17: c[8] = 0b10000000;
      0000C0 75 10 80         [24]  408 	mov	(_c + 0x0008),#0x80
                                    409 ;	lab.c:18: c[9] = 0b10010000;
      0000C3 75 11 90         [24]  410 	mov	(_c + 0x0009),#0x90
                                    411 ;	lab.c:21: i = 0;
      0000C6 E4               [12]  412 	clr	a
      0000C7 F5 1A            [12]  413 	mov	_i,a
      0000C9 F5 1B            [12]  414 	mov	(_i + 1),a
                                    415 ;	lab.c:23: x[0] = 0;
      0000CB F5 12            [12]  416 	mov	(_x + 0),a
      0000CD F5 13            [12]  417 	mov	(_x + 1),a
                                    418 ;	lab.c:24: x[1] = 0;
      0000CF F5 14            [12]  419 	mov	((_x + 0x0002) + 0),a
      0000D1 F5 15            [12]  420 	mov	((_x + 0x0002) + 1),a
                                    421 ;	lab.c:25: x[2] = 0;
      0000D3 F5 16            [12]  422 	mov	((_x + 0x0004) + 0),a
      0000D5 F5 17            [12]  423 	mov	((_x + 0x0004) + 1),a
                                    424 ;	lab.c:26: x[3] = 0;
      0000D7 F5 18            [12]  425 	mov	((_x + 0x0006) + 0),a
      0000D9 F5 19            [12]  426 	mov	((_x + 0x0006) + 1),a
                                    427 ;	lab.c:28: while(n > 1000) {
      0000DB 7E D7            [12]  428 	mov	r6,#0xd7
      0000DD 7F 09            [12]  429 	mov	r7,#0x09
      0000DF                        430 00101$:
      0000DF C3               [12]  431 	clr	c
      0000E0 74 E8            [12]  432 	mov	a,#0xe8
      0000E2 9E               [12]  433 	subb	a,r6
      0000E3 74 83            [12]  434 	mov	a,#(0x03 ^ 0x80)
      0000E5 8F F0            [24]  435 	mov	b,r7
      0000E7 63 F0 80         [24]  436 	xrl	b,#0x80
      0000EA 95 F0            [12]  437 	subb	a,b
      0000EC 50 17            [24]  438 	jnc	00121$
                                    439 ;	lab.c:29: x[0] = x[0] + 1;
      0000EE 74 01            [12]  440 	mov	a,#0x01
      0000F0 25 12            [12]  441 	add	a, _x
      0000F2 FC               [12]  442 	mov	r4,a
      0000F3 E4               [12]  443 	clr	a
      0000F4 35 13            [12]  444 	addc	a, (_x + 1)
      0000F6 FD               [12]  445 	mov	r5,a
      0000F7 8C 12            [24]  446 	mov	(_x + 0),r4
      0000F9 8D 13            [24]  447 	mov	(_x + 1),r5
                                    448 ;	lab.c:30: n = n - 1000;
      0000FB EE               [12]  449 	mov	a,r6
      0000FC 24 18            [12]  450 	add	a,#0x18
      0000FE FE               [12]  451 	mov	r6,a
      0000FF EF               [12]  452 	mov	a,r7
      000100 34 FC            [12]  453 	addc	a,#0xfc
      000102 FF               [12]  454 	mov	r7,a
                                    455 ;	lab.c:32: while(n > 100) {
      000103 80 DA            [24]  456 	sjmp	00101$
      000105                        457 00121$:
      000105                        458 00104$:
      000105 C3               [12]  459 	clr	c
      000106 74 64            [12]  460 	mov	a,#0x64
      000108 9E               [12]  461 	subb	a,r6
      000109 74 80            [12]  462 	mov	a,#(0x00 ^ 0x80)
      00010B 8F F0            [24]  463 	mov	b,r7
      00010D 63 F0 80         [24]  464 	xrl	b,#0x80
      000110 95 F0            [12]  465 	subb	a,b
      000112 50 17            [24]  466 	jnc	00123$
                                    467 ;	lab.c:33: x[1] = x[1] + 1;
      000114 74 01            [12]  468 	mov	a,#0x01
      000116 25 14            [12]  469 	add	a, (_x + 0x0002)
      000118 FC               [12]  470 	mov	r4,a
      000119 E4               [12]  471 	clr	a
      00011A 35 15            [12]  472 	addc	a, ((_x + 0x0002) + 1)
      00011C FD               [12]  473 	mov	r5,a
      00011D 8C 14            [24]  474 	mov	((_x + 0x0002) + 0),r4
      00011F 8D 15            [24]  475 	mov	((_x + 0x0002) + 1),r5
                                    476 ;	lab.c:34: n = n - 100;
      000121 EE               [12]  477 	mov	a,r6
      000122 24 9C            [12]  478 	add	a,#0x9c
      000124 FE               [12]  479 	mov	r6,a
      000125 EF               [12]  480 	mov	a,r7
      000126 34 FF            [12]  481 	addc	a,#0xff
      000128 FF               [12]  482 	mov	r7,a
                                    483 ;	lab.c:36: while(n > 10) {
      000129 80 DA            [24]  484 	sjmp	00104$
      00012B                        485 00123$:
      00012B                        486 00107$:
      00012B C3               [12]  487 	clr	c
      00012C 74 0A            [12]  488 	mov	a,#0x0a
      00012E 9E               [12]  489 	subb	a,r6
      00012F 74 80            [12]  490 	mov	a,#(0x00 ^ 0x80)
      000131 8F F0            [24]  491 	mov	b,r7
      000133 63 F0 80         [24]  492 	xrl	b,#0x80
      000136 95 F0            [12]  493 	subb	a,b
      000138 50 17            [24]  494 	jnc	00125$
                                    495 ;	lab.c:37: x[2] = x[2] + 1;
      00013A 74 01            [12]  496 	mov	a,#0x01
      00013C 25 16            [12]  497 	add	a, (_x + 0x0004)
      00013E FC               [12]  498 	mov	r4,a
      00013F E4               [12]  499 	clr	a
      000140 35 17            [12]  500 	addc	a, ((_x + 0x0004) + 1)
      000142 FD               [12]  501 	mov	r5,a
      000143 8C 16            [24]  502 	mov	((_x + 0x0004) + 0),r4
      000145 8D 17            [24]  503 	mov	((_x + 0x0004) + 1),r5
                                    504 ;	lab.c:38: n = n - 10;
      000147 EE               [12]  505 	mov	a,r6
      000148 24 F6            [12]  506 	add	a,#0xf6
      00014A FE               [12]  507 	mov	r6,a
      00014B EF               [12]  508 	mov	a,r7
      00014C 34 FF            [12]  509 	addc	a,#0xff
      00014E FF               [12]  510 	mov	r7,a
                                    511 ;	lab.c:40: while(n > 1) {
      00014F 80 DA            [24]  512 	sjmp	00107$
      000151                        513 00125$:
      000151                        514 00110$:
      000151 C3               [12]  515 	clr	c
      000152 74 01            [12]  516 	mov	a,#0x01
      000154 9E               [12]  517 	subb	a,r6
      000155 74 80            [12]  518 	mov	a,#(0x00 ^ 0x80)
      000157 8F F0            [24]  519 	mov	b,r7
      000159 63 F0 80         [24]  520 	xrl	b,#0x80
      00015C 95 F0            [12]  521 	subb	a,b
      00015E 50 14            [24]  522 	jnc	00114$
                                    523 ;	lab.c:41: x[3] = x[3] + 1;
      000160 74 01            [12]  524 	mov	a,#0x01
      000162 25 18            [12]  525 	add	a, (_x + 0x0006)
      000164 FC               [12]  526 	mov	r4,a
      000165 E4               [12]  527 	clr	a
      000166 35 19            [12]  528 	addc	a, ((_x + 0x0006) + 1)
      000168 FD               [12]  529 	mov	r5,a
      000169 8C 18            [24]  530 	mov	((_x + 0x0006) + 0),r4
      00016B 8D 19            [24]  531 	mov	((_x + 0x0006) + 1),r5
                                    532 ;	lab.c:42: n = n - 1;
      00016D 1E               [12]  533 	dec	r6
      00016E BE FF 01         [24]  534 	cjne	r6,#0xff,00180$
      000171 1F               [12]  535 	dec	r7
      000172                        536 00180$:
                                    537 ;	lab.c:45: while(1) {
      000172 80 DD            [24]  538 	sjmp	00110$
      000174                        539 00114$:
                                    540 ;	lab.c:46: P0_7 = 1;
                                    541 ;	assignBit
      000174 D2 87            [12]  542 	setb	_P0_7
                                    543 ;	lab.c:47: P3_3 = 0;
                                    544 ;	assignBit
      000176 C2 B3            [12]  545 	clr	_P3_3
                                    546 ;	lab.c:48: P3_4 = 0;
                                    547 ;	assignBit
      000178 C2 B4            [12]  548 	clr	_P3_4
                                    549 ;	lab.c:49: P1 = c[x[1]];
      00017A E5 14            [12]  550 	mov	a,(_x + 0x0002)
      00017C 24 08            [12]  551 	add	a, #_c
      00017E F9               [12]  552 	mov	r1,a
      00017F 87 90            [24]  553 	mov	_P1,@r1
                                    554 ;	lab.c:50: P3_3 = 0;
                                    555 ;	assignBit
      000181 C2 B3            [12]  556 	clr	_P3_3
                                    557 ;	lab.c:51: P3_4 = 0;
                                    558 ;	assignBit
      000183 C2 B4            [12]  559 	clr	_P3_4
                                    560 ;	lab.c:52: P1 = c[x[1]];
      000185 E5 14            [12]  561 	mov	a,(_x + 0x0002)
      000187 24 08            [12]  562 	add	a, #_c
      000189 F9               [12]  563 	mov	r1,a
      00018A 87 90            [24]  564 	mov	_P1,@r1
                                    565 ;	lab.c:53: P3_3 = 1;
                                    566 ;	assignBit
      00018C D2 B3            [12]  567 	setb	_P3_3
                                    568 ;	lab.c:54: P3_4 = 0;
                                    569 ;	assignBit
      00018E C2 B4            [12]  570 	clr	_P3_4
                                    571 ;	lab.c:55: P1 = c[x[1]];
      000190 E5 14            [12]  572 	mov	a,(_x + 0x0002)
      000192 24 08            [12]  573 	add	a, #_c
      000194 F9               [12]  574 	mov	r1,a
      000195 87 90            [24]  575 	mov	_P1,@r1
                                    576 ;	lab.c:56: P3_3 = 0;
                                    577 ;	assignBit
      000197 C2 B3            [12]  578 	clr	_P3_3
                                    579 ;	lab.c:57: P3_4 = 1;
                                    580 ;	assignBit
      000199 D2 B4            [12]  581 	setb	_P3_4
                                    582 ;	lab.c:58: P1 = c[x[1]];
      00019B E5 14            [12]  583 	mov	a,(_x + 0x0002)
      00019D 24 08            [12]  584 	add	a, #_c
      00019F F9               [12]  585 	mov	r1,a
      0001A0 87 90            [24]  586 	mov	_P1,@r1
                                    587 ;	lab.c:59: P3_3 = 1;
                                    588 ;	assignBit
      0001A2 D2 B3            [12]  589 	setb	_P3_3
                                    590 ;	lab.c:60: P3_4 = 1;
                                    591 ;	assignBit
      0001A4 D2 B4            [12]  592 	setb	_P3_4
                                    593 ;	lab.c:62: }
      0001A6 80 CC            [24]  594 	sjmp	00114$
                                    595 	.area CSEG    (CODE)
                                    596 	.area CONST   (CODE)
                                    597 	.area XINIT   (CODE)
                                    598 	.area CABS    (ABS,CODE)
