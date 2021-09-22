/*
 * main.c
 */
int ftn(int *data);
int ftn0(int *data);
int ftn1(int *data);
int ftn2(int *data);
int ftn3(int *data);

#pragma DATA_ALIGN   (data1, 0x80);

#define BUFLEN 64
int data1[BUFLEN] = {  1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,
				 	  33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64	};

//---------------------------------------------------------------------------------------------
int main(void)
{
	int i = 0;
	i += ftn(data1);		// 1879 pointer 1659 array
	i += ftn0(data1);		// 1879         1623
	i += ftn1(data1);		// 1884         1559
	i += ftn2(data1);		// 670          672
	i += ftn3(data1);		// 665          662
	return i;
} // main
//---------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------
int ftn(int *data)
{
int i;
long jlong = 0;

_nassert((int)(data) & 0x7F == 0);		// it didn't know this yet
#pragma MUST_ITERATE(BUFLEN) 			// it already knew this, actually

for(i=BUFLEN-1; i>=0; i--)
	jlong = _lsadd(data[i], jlong);		// or *data++ for pointer testing
return _sat(jlong);
} // int ftn(int *data);
//---------------------------------------------------------------------------------------------


#pragma FUNCTION_OPTIONS(ftn0, "-o0")
//---------------------------------------------------------------------------------------------
int ftn0(int *data)
{
	int i;
	long jlong = 0;

	_nassert((int)(data) & 0x7F == 0);		// it didn't know this yet
	#pragma MUST_ITERATE(BUFLEN) 			// it already knew this, actually

	for(i=BUFLEN-1; i>=0; i--)
		jlong = _lsadd(data[i], jlong);		// or *data++ for pointer testing
	return _sat(jlong);
} // int ftn0(int *data);
//---------------------------------------------------------------------------------------------


#pragma FUNCTION_OPTIONS(ftn1, "-o1")
//---------------------------------------------------------------------------------------------
int ftn1(int *data)
{
	int i;
	long jlong = 0;

	_nassert((int)(data) & 0x7F == 0);		// it didn't know this yet
	#pragma MUST_ITERATE(BUFLEN) 			// it already knew this, actually

	for(i=BUFLEN-1; i>=0; i--)
		jlong = _lsadd(data[i], jlong);		// or *data++ for pointer testing
	return _sat(jlong);
} // int ftn1(int *data);
//---------------------------------------------------------------------------------------------


#pragma FUNCTION_OPTIONS(ftn2, "-o2")
//---------------------------------------------------------------------------------------------
int ftn2(int *data)
{
	int i;
	long jlong = 0;

	_nassert((int)(data) & 0x7F == 0);		// it didn't know this yet
	#pragma MUST_ITERATE(BUFLEN) 			// it already knew this, actually

	for(i=BUFLEN-1; i>=0; i--)
		jlong = _lsadd(data[i], jlong);		// or *data++ for pointer testing
	return _sat(jlong);

} // int ftn2(int *data);
//---------------------------------------------------------------------------------------------


#pragma FUNCTION_OPTIONS(ftn3, "-o3")
//---------------------------------------------------------------------------------------------
int ftn3(int *data)
{
	int i;
	long jlong = 0;

	_nassert((int)(data) & 0x7F == 0);		// it didn't know this yet
	#pragma MUST_ITERATE(BUFLEN) 	// it already knew this, actually

	for(i=BUFLEN-1; i>=0; i--)
		jlong = _lsadd(data[i], jlong);		// or *data++ for pointer testing
	return _sat(jlong);

} // int ftn3(int *data);
//---------------------------------------------------------------------------------------------

/*
                                ftn0():
0000016c:  ftn0()+0               07BF09C2            SUB.D2        SP,0x18,SP
00000170:                         023C22F4            STW.D2T1      A4,*+SP[1]
 50                             long jlong = 0;
00000174:                         020004FA            ZERO.L2       B5:B4
00000178:                         023C82F6            STW.D2T2      B4,*+SP[4]
0000017c:                         02BCA2F6            STW.D2T2      B5,*+SP[5]
 54                             for(i=BUFLEN; i>0;i--)
00000180:                         0200202A            MVK.S2        0x0040,B4
00000184:                         023C42F6            STW.D2T2      B4,*+SP[2]
00000188:                         00100ADA            CMPLT.L2      0,B4,B0
0000018c:                         30000B10     [!B0]  B.S1          C$DW$L$_ftn0$2$E (PC+88 = 0x000001d8)
00000190:                         00008000            NOP           5
 55                             	jlong = _lsadd(jlong, *data++);
                                C$DW$L$_ftn0$2$B, C$L3:
00000194:  ..._ftn0$2$B, C$L3+0   02BC22E6            LDW.D2T2      *+SP[1],B5
00000198:                         033C82E6            LDW.D2T2      *+SP[4],B6
0000019c:                         00004000            NOP           3
000001a0:                         021436E6            LDW.D2T2      *B5++[1],B4
000001a4:                         02BC22F6            STW.D2T2      B5,*+SP[1]
000001a8:                         00004000            NOP           3
000001ac:                         0293EDA2            SHR.S2        B4,0x1f,B5
000001b0:                         0210C63A            SADD.L2       B6,B5:B4,B5:B4
000001b4:                         02BCA2F6            STW.D2T2      B5,*+SP[5]
000001b8:                         023C82F6            STW.D2T2      B4,*+SP[4]
 54                             for(i=BUFLEN; i>0;i--)
000001bc:                         023C42E6            LDW.D2T2      *+SP[2],B4
000001c0:                         00006000            NOP           4
000001c4:                         0213E05A            SUB.L2        B4,1,B4
000001c8:                         023C42F6            STW.D2T2      B4,*+SP[2]
000001cc:                         00100ADA            CMPLT.L2      0,B4,B0
000001d0:                         2FFFFA90     [ B0]  B.S1          C$L3 (PC-44 = 0x00000194)
000001d4:                         00008000            NOP           5
 56                             return _sat(jlong);
                                C$DW$L$_ftn0$2$E, C$L4:
000001d8:  ..._ftn0$2$E, C$L4+0   023C43E4            LDDW.D2T1     *+SP[2],A5:A4
000001dc:                         00006000            NOP           4
000001e0:                         02100818            SAT.L1        A5:A4,A4
 57                             } // int ftn0(int *data);
000001e4:                         07800C52            ADDK.S2       24,SP
000001e8:                         000C0362            B.S2          B3
000001ec:                         00008000            NOP           5

                                ftn1():
000001f0:  ftn1()+0               07BF09C2            SUB.D2        SP,0x18,SP
000001f4:                         023C22F4            STW.D2T1      A4,*+SP[1]
 65                             long jlong = 0;
000001f8:                         020004FA            ZERO.L2       B5:B4
000001fc:                         023C82F6            STW.D2T2      B4,*+SP[4]
00000200:                         02BCA2F6            STW.D2T2      B5,*+SP[5]
 69                             for(i=BUFLEN; i>0;i--)
00000204:                         01802028            MVK.S1        0x0040,A3
00000208:                         01BC42F4            STW.D2T1      A3,*+SP[2]
0000020c:                         008C0AD8            CMPLT.L1      0,A3,A1
00000210:                         90000B90     [!A1]  B.S1          C$DW$L$_ftn1$3$E (PC+92 = 0x0000025c)
00000214:                         00008000            NOP           5
 70                             	jlong = _lsadd(jlong, *data++);
                                C$DW$L$_ftn1$2$B, C$L5:
00000218:  ..._ftn1$2$B, C$L5+0   02BC22E6            LDW.D2T2      *+SP[1],B5
0000021c:                         033C82E6            LDW.D2T2      *+SP[4],B6
00000220:                         00004000            NOP           3
00000224:                         021436E6            LDW.D2T2      *B5++[1],B4
00000228:                         00006000            NOP           4
                                C$DW$L$_ftn1$2$E, C$DW$L$_ftn1$3$B:
0000022c:  ...$DW$L$_ftn1$3$B+0   02BC22F7            STW.D2T2      B5,*+SP[1]
00000230:                         0293EDA2 ||         SHR.S2        B4,0x1f,B5
00000234:                         0210C63A            SADD.L2       B6,B5:B4,B5:B4
00000238:                         02BCA2F6            STW.D2T2      B5,*+SP[5]
0000023c:                         023C82F6            STW.D2T2      B4,*+SP[4]
00000240:                         023C42E6            LDW.D2T2      *+SP[2],B4
00000244:                         00006000            NOP           4
00000248:                         0213E05A            SUB.L2        B4,1,B4
0000024c:                         023C42F6            STW.D2T2      B4,*+SP[2]
00000250:                         00100ADA            CMPLT.L2      0,B4,B0
00000254:                         2FFFFB10     [ B0]  B.S1          C$L5 (PC-40 = 0x00000218)
00000258:                         00008000            NOP           5
 71                             return _sat(jlong);
                                C$DW$L$_ftn1$3$E, C$L6:
0000025c:  ..._ftn1$3$E, C$L6+0   023C43E4            LDDW.D2T1     *+SP[2],A5:A4
00000260:                         00006000            NOP           4
00000264:                         02100818            SAT.L1        A5:A4,A4
 72                             } // int ftn1(int *data);
00000268:                         07800C52            ADDK.S2       24,SP
0000026c:                         000C0362            B.S2          B3
00000270:                         00008000            NOP           5

                                ftn2():
00000274:  ftn2()+0               07BF09C2            SUB.D2        SP,0x18,SP
00000278:                         023C22F4            STW.D2T1      A4,*+SP[1]
 80                             	long jlong = 0;
0000027c:                         020004FA            ZERO.L2       B5:B4
00000280:                         023C82F6            STW.D2T2      B4,*+SP[4]
00000284:                         02BCA2F6            STW.D2T2      B5,*+SP[5]
 84                             	for(i=BUFLEN; i>0;i--)
00000288:                         01802028            MVK.S1        0x0040,A3
0000028c:                         01BC42F4            STW.D2T1      A3,*+SP[2]
00000290:                         008C0AD8            CMPLT.L1      0,A3,A1
00000294:                         90000D90     [!A1]  B.S1          C$L10 (PC+108 = 0x000002ec)
00000298:                         00008000            NOP           5
0000029c:                         030403E2            MVC.S2        CSR,B6
000002a0:                         021BCF5A            AND.L2        -2,B6,B4
000002a4:                         009003A3            MVC.S2        B4,CSR
000002a8:                         023C42E6 ||         LDW.D2T2      *+SP[2],B4
000002ac:                         0290105A            MV.L2X        A4,B5
000002b0:                         00002000            NOP           2
                                C$DW$L$_ftn2$4$B, C$L7, C$L8:
000002b4:  ...4$B, C$L7, C$L8+0   021436E4            LDW.D2T1      *B5++[1],A4
000002b8:                         0213E05B            SUB.L2        B4,1,B4
000002bc:                         01BC82E4 ||         LDW.D2T1      *+SP[4],A3
000002c0:                         00100ADB            CMPLT.L2      0,B4,B0
000002c4:                         02BC22F6 ||         STW.D2T2      B5,*+SP[1]
000002c8:                         023C42F7            STW.D2T2      B4,*+SP[2]
000002cc:                         2FFFFE92 ||  [ B0]  B.S2          C$L7 (PC-12 = 0x000002b4)
000002d0:                         02BC22E6            LDW.D2T2      *+SP[1],B5
000002d4:                         0293EDA1            SHR.S1        A4,0x1f,A5
000002d8:                         023C42E6 ||         LDW.D2T2      *+SP[2],B4
000002dc:                         02106638            SADD.L1       A3,A5:A4,A5:A4
000002e0:                         023C82F4            STW.D2T1      A4,*+SP[4]
000002e4:                         02BCA2F4            STW.D2T1      A5,*+SP[5]
                                C$DW$L$_ftn2$4$E, C$L9:
000002e8:  ..._ftn2$4$E, C$L9+0   009803A2            MVC.S2        B6,CSR
 86                             	return _sat(jlong);
                                C$L10:
000002ec:  C$L10+0                023C43E4            LDDW.D2T1     *+SP[2],A5:A4
000002f0:                         00006000            NOP           4
000002f4:                         02100818            SAT.L1        A5:A4,A4
 88                             } // int ftn2(int *data);
000002f8:                         07800C52            ADDK.S2       24,SP
000002fc:                         000C0362            B.S2          B3
00000300:                         00008000            NOP           5

                                ftn3():
00000304:  ftn3()+0               07BF09C2            SUB.D2        SP,0x18,SP
00000308:                         023C22F4            STW.D2T1      A4,*+SP[1]
 97                             	long jlong = 0;
0000030c:                         020004FA            ZERO.L2       B5:B4
00000310:                         023C82F6            STW.D2T2      B4,*+SP[4]
00000314:                         02BCA2F6            STW.D2T2      B5,*+SP[5]
101                             	for(i=BUFLEN; i>0;i--)
00000318:                         01802028            MVK.S1        0x0040,A3
0000031c:                         01BC42F4            STW.D2T1      A3,*+SP[2]
00000320:                         008C0AD8            CMPLT.L1      0,A3,A1
00000324:                         90000B90     [!A1]  B.S1          C$L14 (PC+92 = 0x0000037c)
00000328:                         00008000            NOP           5
0000032c:                         030403E2            MVC.S2        CSR,B6
00000330:                         021BCF5A            AND.L2        -2,B6,B4
00000334:                         009003A3            MVC.S2        B4,CSR
00000338:                         023C42E6 ||         LDW.D2T2      *+SP[2],B4
0000033c:                         0290105A            MV.L2X        A4,B5
00000340:                         00002000            NOP           2
                                C$DW$L$_ftn3$4$B, C$L11, C$L12:
00000344:  ...B, C$L11, C$L12+0   021436E4            LDW.D2T1      *B5++[1],A4
00000348:                         0213E05B            SUB.L2        B4,1,B4
0000034c:                         01BC82E4 ||         LDW.D2T1      *+SP[4],A3
00000350:                         00100ADB            CMPLT.L2      0,B4,B0
00000354:                         02BC22F6 ||         STW.D2T2      B5,*+SP[1]
00000358:                         023C42F7            STW.D2T2      B4,*+SP[2]
0000035c:                         20000092 ||  [ B0]  B.S2          C$L11 (PC+4 = 0x00000344)
00000360:                         02BC22E6            LDW.D2T2      *+SP[1],B5
00000364:                         0293EDA1            SHR.S1        A4,0x1f,A5
00000368:                         023C42E6 ||         LDW.D2T2      *+SP[2],B4
0000036c:                         02106638            SADD.L1       A3,A5:A4,A5:A4
00000370:                         023C82F4            STW.D2T1      A4,*+SP[4]
00000374:                         02BCA2F4            STW.D2T1      A5,*+SP[5]
                                C$DW$L$_ftn3$4$E, C$L13:
00000378:  ...ftn3$4$E, C$L13+0   009803A2            MVC.S2        B6,CSR
103                             	return _sat(jlong);
                                C$L14:
0000037c:  C$L14+0                023C43E4            LDDW.D2T1     *+SP[2],A5:A4
00000380:                         00006000            NOP           4
00000384:                         02100818            SAT.L1        A5:A4,A4
105                             } // int ftn3(int *data);
00000388:                         07800C52            ADDK.S2       24,SP
0000038c:                         000C0362            B.S2          B3
00000390:                         00008000            NOP           5
00000394:                         00000000            NOP
00000398:                         00000000            NOP
0000039c:                         00000000            NOP

 */
