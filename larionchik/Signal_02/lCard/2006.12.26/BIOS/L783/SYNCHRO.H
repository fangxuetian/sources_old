{ ---------------------------------------------------------------------	}
{ � ������ 䠩�� ��室���� �㭪樨 ����᪠ ᨭ�஭���樨, � ������:		}
{ - ��஢�� ᨭ�஭���樨 ���� ��� �����஢��                    		}
{ - ���������� ᨭ�஭���樨 �� �஢�� ��� ���室�							}
{ ---------------------------------------------------------------------	}

{ *********************************************************************	}
{ �室 � ��ࠡ��稪 ������� ᨭ�஭���樨											}
{ *********************************************************************	}
Synchro_Config_cmd:
{ ��⠭���� ��� �몫�稢 �����, �.�. ᤥ���� SCLK1 ���譨�					}
	AR = 0x3F1F; 						{ 0011 1111 0001 1111 						}
	DM(Sport1_Ctrl_Reg) = AR;		{ 0x3FF2 - SPORT1 Control Register		}

{ ����⠭���� ����� ࠡ��� ���														}
	AR=DM(ADC_Rate);				{ ���� ����᪠ ���							}
	DM(Sport1_Sclkdiv) = AR;	{ 0x3FF5 - Serial Clock Divide Modulus		}

{ ����⠭���� BDMA, �᫨ �� �⮣� �뫨 � ��-���஢�� ᨭ�஭���樨		}
	CALL ResetBdmaHandler;

{ �஢�ਬ ⨯ ᨭ�஭���樨															}
	AR=DM(SynchroType);
	AR= ABS AR;
{ ��६����� SynchroType ������ ���� 0, 1 ��� 2									}
	AR=AR-2;
	IF LE JUMP SynchroEntry;

{ �᫨ ᨭ�஭����� �� �㦭�, � ��୥��� �� �᭮���� ��楤���			}
	JUMP Restart_ADC;

SynchroEntry:
	AR=DM(ADC_Rate);		{ ���� ����᪠ ���									}
	AY0=19;           	{ �᫮ 19 ᮮ⢥����� ���� 1000 ���			}
	AF=AR-AY0;
	IF LT CALL SetHighFreqLoop;
	AF= PASS AF;
	IF GE	CALL SetLowFreqLoop;

	DIS INTS;	 				{ ����⨬ �� ���뢠���							}

{ ���⨬ �� �⥪� 																		}
ClearStacksSynchroEntry:
	POP STS, POP PC, POP CNTR, POP LOOP;
	ENA SEC_REG;			{ ��३��� �� ��ன ���� ॣ���஢					}
	AR=SSTAT;
	AY0=0x55;
	AR=AR AND AY0;
	AR=AR XOR AY0;
	IF NE JUMP ClearStacksSynchroEntry;	{ �᫨ �� �� �⥪� ���⨫���, 	}
													{ � ����ਬ							}
	IMASK=0x0; NOP;      { ����᪨�㥬 �� ���뢠���							}
	ENA INTS;  				{ ࠧ�訬 �� ���뢠���								}

	AR=^Synchronization;	{ ���室�� �� ��ࠡ��稪 �������					}
	TOPPCSTACK=AR;			{ ����ᥬ ���� �᭮���� ��楤��� 					}
	DIS SEC_REG;			{ ��३��� �� ���� ���� ॣ���஢					}
	PUSH STS;				{ �������� � �⥪�										}

{ ���⨬ ������ �� ���뢠���, �஬� SPORT0 Transmit (���)				}
	IFC=0xBF; NOP;

	RTI;

{ *********************************************************************	}
{ ��⠭���� ⨯� ᨭ�஭���樨														}
{ *********************************************************************	}
Synchronization:
{ ᮮ�騬 � �����襭�� �������														}
	AR=0x0; DM(Command)=AR;

{ ����প� �� 0.4 ���, �⮡� ���ਫ�� ��᫥���� �����						}
	CNTR=16;
	DO SynchronizationLoop UNTIL CE;
SynchronizationLoop: NOP;

{ ���⨬ ������ �� ���뢠���, �஬� SPORT0 Transmit (���)				}
	IFC=0xBF; NOP;

{ �᫨ ���� ��� ����� 1000.0 ���, ⮣�� �몫�稬 ���						}
	AR=^LowFreqLoop;
	DM(TopStack)=AR;

	AR=DM(ADC_Rate);		{ ���� ����᪠ ���									}
	AY0=19;              { �᫮ 19 ᮮ⢥����� ���� 1000.0 ���		}
	NONE=AR-AY0;
	IF GE JUMP Skip_DAC_Off;

	AR=^HighFreqLoop;
	DM(TopStack)=AR;

{ ��ᨬ 䫠��� ࠧ�襭�� �뢮�� �� ���� �� ���							}
	AR=0x0; DM(EnableDacStream)=AR;

{ ������� � IMASK_VALUE ����� ���祭��												}
	AR=DM(IMASK_VALUE);
	AR= CLRBIT 6 OF AR;	{ ����⨬ ���뢠��� SPORT0 Transmit (���)		}
	DM(IMASK_VALUE)=AR;

Skip_DAC_Off:
	AR=DM(SynchroType);				{ �஢�ਬ ⨯ ᨭ�஭���樨				}
	AF=PASS AR;
	IF EQ JUMP TtlSynchroStart;	{ �᫨ 0, � ��஢�� 						}
											{						ᨭ�஭����� ����	}
	AF=AF-0x1;
	IF EQ JUMP TtlSynchroKadr;		{ �᫨ 1, � �����஢��        			}
											{ 					��஢�� ᨭ�஭�����	}
	AF=AF-0x1;
	IF EQ JUMP AdcSynchroStart;	{ �᫨ 2, � ᨭ�஭����� 				}
											{								�� ������ ���	}

	IMASK=DM(IMASK_VALUE); NOP;
	I6=DM(TopStack);
	JUMP (I6); 							{ �������� ��ଠ��� ������ ������		}

{ *********************************************************************	}
{ ��஢�� ᨭ�஭����� ����														}
{ *********************************************************************	}
TtlSynchroStart:
{ �ந��樠�����㥬 ����室��� 㪠��⥫� � ��६����						}
	CALL InitAdcPointers;

{ ������� �ᨫ���� � ����� ������ ��� ᫥���饣� �����						}
	AR=DM(I2, M2);
	SET FL0;
	IO(SetChannel)=AR;
	RESET FL0;
	AR=DM(I2, M2);
	IO(SetChannel)=AR;

{ ��������쭠� ����প�, �⮡� ��⠭������ ��������� �ࠪ�					}
	AR=DM(FirstSampleDelay);
	CNTR=AR;
	DO TtlStartLoop0 UNTIL CE;
TtlStartLoop0: NOP;

{ ���⨬ ������ �� ���뢠���, �஬� SPORT0 Transmit (���)				}
	IFC=0x3F; NOP;

{ ࠧ�訬 ���뢠��� IRQ2 � IRQ1(������ ᨭ�஭�����)						}
	AR=DM(IMASK_VALUE);
	AR= SETBIT 2 OF AR;			{ ࠧ�訬 ���뢠��� IRQ1						}
	IMASK=AR; NOP;

{ ��ᨬ 䫠� ᨭ�஭���樨 IRQ1													}
/*	AR=0x0; DM(TtlSynchroFlag)=AR;*/

{ ���� ���뢠��� IRQ1																	}
/*WaitForStartSynchroPulse:
	AR=DM(TtlSynchroFlag);
	AR= PASS AR;
	IF EQ JUMP WaitForStartSynchroPulse;*/

{ ��⠭���� 䫠���, �� ࠧ�襭� ࠡ�� ���										}
	AR=0x1; DM(ADC_Enable)=AR;

	I6=DM(TopStack);
	JUMP (I6); 						{ �������� ��ଠ��� ������ ������			}

{ *********************************************************************	}
{ �����஢�� ��஢�� ᨭ�஭�����													}
{ �᫨ ࠡ�⠥� �� ���� ��� ��� 1000.0 ���, � �ନ�㥬 ᫥���騩 	}
{ ��ࠡ��稪 ���뢠��� IRQE (��� ������� �� ���뢠��� �� BDMA):			}
{	AF=AF-1, MR1=DM(I2, M2);  ��� AF 蠣 ���뢠���								}
{	IO(SetChannel)=MR1;																	}
{	MR1=IO(ReadADC);																		}
{	DM(I3, M3)=MR1, AR=AR-AY0;	��� AR ⥪�騩 ����� ����� � ����		}
{  IF EQ JUMP ReturnToAnotherKadr;													}
{  RTI;																						}
{ *********************************************************************	}
TtlSynchroKadr:
{ �ந��樠�����㥬 ����室��� 㪠��⥫� � ��६����						}
	CALL InitAdcPointers;

{ ������� �ᨫ���� � ����� ������ ��� ��ࢮ�� � ��ண� �����			}
	I2=^Control_Table; AR=DM(I2, M2);
	SET FL0; IO(SetChannel)=AR; RESET FL0;
	AR=DM(I2, M2);	IO(SetChannel)=AR;
{ ��������쭠� ����প�, �⮡� ��⠭������ ��������� �ࠪ�					}
	CNTR=DM(FirstSampleDelay);
	DO TtlSynchroKadrLoop1 UNTIL CE;
TtlSynchroKadrLoop1: NOP;

{ ࠧ�訬 ࠡ��� ���																	}
	AR=0x1; DM(ADC_Enable)=AR;
{ ����⠭� ࠧ�襭�� ����७��� ������ SCLK1 									}
	MY0 = 0x7F1F; 						{ 0111 1111 0001 1111 						}

{ �᫨ ���� ��� ����� 1000.0 ���, ⮣�� ��३��� 							}
{																�� TtlKadrHighFreqLoop	}
	AR=DM(ADC_Rate);		{ ���� ����᪠ ���									}
	AY0=19;              { �᫮ 19 ᮮ⢥����� ���� 1000.0 ���		}
	NONE=AR-AY0;
	IF GE JUMP TtlKadrLowFreqLoop;

{ ����ࠢ�� ��ࠡ��稪 ���뢠��� ��� ���											}
	CALL SetBdmaHandler;
{ ��� ��᮪����⭮� ᨭ�஭���樨 � AY0 ������ ���� ������				}
	AY0=0x1;
{ ����⠭� ����饭�� ����७��� ������ SCLK1 									}
	MY1 = 0x3F1F; 						{ 0011 1111 0001 1111 						}
{ ����㧨� � AF ���稪 ����⮢ �� ࠧ�襭��� �����஢���� IRQ � PC	}
	AR= AX1 + 0x1;
	AF= PASS AR;
{ ��� ��᮪����⭮� ᨭ�஭���樨 � AR ������ ���� ࠧ��� ����			}
	AR=DM(Control_Table_Lenght);
{ ���⨬ ������ �� ���뢠���, �஬� IRQ2 (������� �� ��)					}
	IFC=0x7F; NOP;
{ ࠧ�訬 ���뢠��� IRQ1(��.ᨭ��.), IRQ2(�������) � IRQE(���)			}
	IMASK=0x214; NOP;

	JUMP TtlKadrHighFreqLoop; 	{ �������� ��-���஢� ������ ������		}

{ *********************************************************************	}
{ ���������� 横� ��� ��-���஢�� ᨭ�஭���樨							}
{ *********************************************************************	}
TtlKadrLowFreqLoop:
{ ���⨬ ������ �� ���뢠���, �஬� IRQ2 (������� �� ��)					}
	IFC=0x7F; NOP;
{ ࠧ�訬 ���뢠��� IRQ1(��.ᨭ��.) � IRQ2(�������)							}
	AR=DM(IMASK_VALUE);
	AR= SETBIT 2 OF AR;	{ �ਭ㤨⥫쭮 ࠧ�訬 ���뢠��� IRQ1			}
	IMASK=AR; NOP;

{	�ய��⨬ ���� ����� ��⮬�, �� �� ����									}
TtlKadrLowFreq1:
	IF FLAG_IN JUMP TtlKadrLowFreq1;
	AR=DM(I2, M2);
TtlKadrLowFreq2:
	IF NOT FLAG_IN JUMP TtlKadrLowFreq2;
	IO(SetChannel)=AR;

{ ���⨬ ������ �� ���뢠���, �஬� IRQ2 (������� �� ��)					}
	IFC=0x7F;

AnotherLowFreqKadr:
{ ��⠥� ���� ����⮢ � ���															}
	CNTR=DM(Control_Table_Lenght);
	DO TtlKadrLowFreq UNTIL CE;
TtlKadrLowFreq3:
		IF FLAG_IN JUMP TtlKadrLowFreq3;
		AR=DM(I2, M2);
TtlKadrLowFreq4:
		IF NOT FLAG_IN JUMP TtlKadrLowFreq4;
{ 		������� �ᨫ���� � ����� ������ ��� ᫥���饣� �����				}
		IO(SetChannel)=AR;
{ 		������� ���஢����� ���祭�� � ���� ���									}
		AR=IO(ReadADC);
		AR=AR+AY1, AY1=DM(I1,M1);
		MR=AR*MY1(SU), MY1=DM(I5,M5);
		DM(I3, M3)=MR1;

TtlKadrLowFreq: AF=AF+1;

{ ���⨬ ������ �� ���뢠���, �஬� IRQ2 (������� �� ��)					}
	IFC=0x7F;
{ ��⠭���� ��� �몫�稢 �����, �.�. ᤥ���� SCLK1 ���譨�					}
	AR = 0x3F1F; DM(Sport1_Ctrl_Reg) = AR;

{ �᫨ �㦭� ᣥ��ਬ ���뢠��� � ��											}
	AR=AX1-AF;
	IF EQ CALL MakeIrqPC;

	DM(AdcFifoPointer)=I3;

	JUMP AnotherLowFreqKadr;

{ *********************************************************************	}
{ ��᮪������ 横� ��� ��-���஢�� ᨭ�஭���樨							}
{ *********************************************************************	}
TtlKadrHighFreqLoop:
	DM(AdcFifoPointer)=I3;
	CALL MakePcIrqForTtlKadrMode;
	JUMP TtlKadrHighFreqLoop;

{ *********************************************************************	}
{ ���室�� �� ����祭�� ᫥���饣� ����											}
{ *********************************************************************	}
ReturnToAnotherKadr:
{ ��⠭���� ��� �몫�稢 �����, �.�. ᤥ���� SCLK1 ���譨�					}
	DM(Sport1_Ctrl_Reg) = MY1;
{ ���⨬ ������ �� ���뢠���, �஬� IRQ2 (������� �� ��)					}
	IFC=0x7F;
{ ��� ��᮪����⭮� ᨭ�஭���樨 � AR ������ ���� ࠧ��� ����			}
	AR=DM(Control_Table_Lenght);

	RTI;

{ *********************************************************************	}
{ ����ਬ ���뢠��� � �� �᫨ ����㯨� ⠪�� ������							}
{ *********************************************************************	}
MakePcIrqForTtlKadrMode:
{ �᫨ ���뢠��� � PC ����饭�, � ���� �멤��								}
	SR0=DM(EnableIrq); NONE= PASS SR0; IF EQ RTS;
{ ⥯��� �஢�ਬ ��� �� ������� ���뢠��� � ��								}
	NONE= PASS AF;	IF GT RTS;
{ ����㧨� ���稪 ����⮢ �� ࠧ�襭��� �����஢���� IRQ � PC		}
	AF= AX1 + AF;
{ ��⠭���� ����, � ���ண� ����� ���뢠�� ����� ����祭�� � ���	}
	DM(IrqDataAddress)=I0;	MODIFY(I0, M0);
{ ⥯��� ����ਬ ���뢠��� � PC														}
	RESET FL2; SET FL2;

	RTS;

{ *********************************************************************	}
{ ���ᨬ ��������� � �⠭����� ��ࠡ��稪 ᯥ樠�쭮 ��� 					}
{ ࠡ��� � ��-���஢�� ᨭ�࠭���樥�												}
{ *********************************************************************	}
SetBdmaHandler:
{ ��ନ�㥬 ��� ������� AF=AF-1, MR1=DM(I2, M2) 								}
{ �� ���� ��⪨ IrqE_Handler															}
	AR=^IrqE_Handler;						{ ����稬 ���� ��⪨ IrqE_Handler	}
	I6=AR; M6=0x1; L6=0x0;

	SR0=0xCA;
	PX=SR0;									{ ����稫� 8 ������ ��� ������樨	}
	SR0=0x6710;
	PM(I6,M6)=SR0;							{ ���࠭��� �� 24-��⮢�� ᫮�� 	}

{ ��ନ�㥬 ��� ������� DM(I3, M3)=MR1, AR=AR-AY0								}
{ �� ����� ^BDMA_Handler-1															}
	AR=^BDMA_Handler;						{ ����稬 ���� ��⪨ BDMA_Handler	}
	I6=AR;
	M6=-1;
	L6=0x0;
	MODIFY(I6, M6);
	M6=0x1;

	SR0=0xCF;
	PX=SR0;									{ ����稫� 8 ������ ��� ������樨	}
	SR0=0x6AE2;
	PM(I6,M6)=SR0;							{ ���࠭��� �� 24-��⮢�� ᫮�� 	}

{ ����� �ନ�㥬 ��� ������� IF EQ JUMP ReturnToAnotherKadr				}
	AR=^ReturnToAnotherKadr;	{ ����稬 ���� ��⪨ ReturnToAnotherKadr	}
	SR1=0x0018;								{ ---- ---- 0001 10xx - ��� �����.	}
	SR0=0x0000;								{ xxxx xxxx xxxx 1111 -     JUMP		}
	SR=SR OR LSHIFT AR BY 4(LO);
	PX=SR0;									{ ����稫� 8 ������ ��� ������樨	}
	AR=SR1;
	SR=LSHIFT SR0 BY -8(LO);
	SR=SR OR LSHIFT AR BY -8(HI);		{ ����稫� ���訥 16 ��� 				}
	PM(I6,M6)=SR0;							{ ���࠭��� �� 24-��⮢�� ᫮�� 	}

{ ����� �ନ�㥬 ��� ������� RTI 													}
	AR=0x1F;
	PX=AR;									{ ����稫� 8 ������ ��� ������樨	}
	AR=0x0A00;
	PM(I6,M6)=AR;							{ ���࠭��� �� 24-��⮢�� ᫮�� 	}

	RTS;

{ *********************************************************************	}
{ ����⠭�������� �⠭����� ��ࠡ��稪 											}
{ *********************************************************************	}
ResetBdmaHandler:
{ ��ନ�㥬 ��� ������� AF=AF+1, MR1=DM(I2, M2) 								}
{ �� ���� ��⪨ IrqE_Handler															}
	AR=^IrqE_Handler;						{ ����稬 ���� ��⪨ IrqE_Handler	}
	I6=AR;
	M6=0x1;
	L6=0x0;

	SR0=0xCA;
	PX=SR0;									{ ����稫� 8 ������ ��� ������樨	}
	SR0=0x6630;
	PM(I6,M6)=SR0;							{ ���࠭��� �� 24-��⮢�� ᫮�� 	}

{ ��ନ�㥬 ��� ������� DM(I3, M3)=MR1	�� ����� ^BDMA_Handler-1			}
	AR=^BDMA_Handler;						{ ����稬 ���� ��⪨ BDMA_Handler	}
	I6=AR;
	M6=-1;
	L6=0x0;
	MODIFY(I6, M6);
	M6=0x1;

	SR0=0xCF;
	PX=SR0;									{ ����稫� 8 ������ ��� ������樨	}
	SR0=0x6800;
	PM(I6,M6)=SR0;							{ ���࠭��� �� 24-��⮢�� ᫮�� 	}

{ ��ନ�㥬 ��� ������� RTI �� ���� ��⪨ BDMA_Handler						}
	AR=0x1F;
	PX=AR;									{ ����稫� 8 ������ ��� ������樨	}
	AR=0x0A00;
	PM(I6,M6)=AR;							{ ���࠭��� �� 24-��⮢�� ᫮�� 	}

{ ����� ������ �ନ�㥬 ��� ������� NOP 			 								}
	AR=0x0;
	PX=AR;									{ ����稫� 8 ������ ��� ������樨	}
	PM(I6,M6)=AR;							{ ���࠭��� �� 24-��⮢�� ᫮�� 	}

	PX=AR;									{ ����稫� 8 ������ ��� ������樨	}
	PM(I6,M6)=AR;							{ ���࠭��� �� 24-��⮢�� ᫮�� 	}

	RTS;

{ *********************************************************************	}
{ ᨭ�஭����� �� ����������� ������ ���											}
{ *********************************************************************	}
AdcSynchroStart:
{ �ந��樠�����㥬 ����室��� 㪠��⥫� � ��६����						}
	CALL InitAdcPointers;

{ ��⠭���� ���� ��ࠡ��稪 ���뢠��� IRQE 									}
{										ᯥ樠�쭮 ��� ���������� ᨭ�஭���樨	}
	AR=^IrqE_Handler;
	I6=AR;
	M6=0x0;
	AR=PM(I6, M6);
	DM(PM_Data)=AR;
	DM(PX_Reg)=PX;
	AR=^IrqEAdcSynchro;
	CALL ModifyJump;

{ ������� �ᨫ���� � ����� ᨭ�ப����� ��� ᫥���饣� �����				}
	SET FL0;
	AR=DM(SynchroAdChannel);
	IO(SetChannel)=AR;
	RESET FL0;
	IO(SetChannel)=AR;

{ ��������쭠� ����প�, �⮡� ��⠭������ ��������� �ࠪ�					}
	AR=DM(FirstSampleDelay);
	CNTR=AR;
	DO AdSynchroLoop0 UNTIL CE;
AdSynchroLoop0: NOP;

{ ��⠭���� ����� ��� ��� ���������� ᨭ�஭���樨 �� ����� 1000.0 ���	}
	AR=DM(ADC_Rate);		{ ���� ����᪠ ���									}
	AY0=19;              { �᫮ 19 ᮮ⢥����� ���� 1000 ���			}
	NONE=AR-AY0;
	IF GE JUMP SkipChangeAdcRate;
	AR = 19;							{ ���� ����᪠ ���							}
	DM(Sport1_Sclkdiv) = AR;	{ 0x3FF5 - Serial Clock Divide Modulus		}

SkipChangeAdcRate:
{ ���� 蠣 ���������� ᨭ�஭���樨												}
	AR=0x0;
	DM(AdSynchroStep)=AR;

{ ࠧ�訬 ���뢠��� IRQ2, IRQE �, ��������, SPORT1 Transmit				}
	AR=DM(IMASK_VALUE);
	AR= SETBIT 4 OF AR;	{ ࠧ�訬 ���뢠��� IRQE(���)						}
	IMASK=AR; NOP;

{ ࠧ�訬 ��� ����稢 �����, �.�. ᤥ���� SCLK1 ����७���					}
	AR = 0x7F1F;						{ 0111 1101 1100 1111						}
	DM(Sport1_Ctrl_Reg) = AR;		{ 0x3FF2 - SPORT1 Control Register		}

{ ��ࢮ� ���뢠���� �� ��� - ������ � ��� �ய�᪠��							}
AdSynchroIdle0:
	IDLE;
	AY0=DM(IsAdcSample);
	AF= PASS AY0;
	IF EQ JUMP AdSynchroIdle0;
	AY0=0x0;
	DM(IsAdcSample)=AY0;

{ � ��஥ �, �� �㦭� � ��稭�� � ���� ࠡ�⠥�								}
AdSynchroIdle1:
	IDLE;
	AY0=DM(IsAdcSample);
	AF= PASS AY0;
	IF EQ JUMP AdSynchroIdle1;
	AY0=0x0;
	DM(IsAdcSample)=AY0;

{ � ॣ���� AR ����稫� ��� � ������ ᨭ�஭���樨 							}
	AY0=DM(SynchroAdSensitivity);	{ ᨭ�஭����� �� �஢�� ��� ���室�	}
	AF=PASS AY0;
	IF NE JUMP StepSynchro;

LastAdSynchro:
	AY0=DM(SynchroAdPorog);
	AF=AR-AY0;
	AR=DM(SynchroAdMode);
	AR=PASS AR;
	IF EQ AF=-AF;
	AF=PASS AF;
	IF LT JUMP AdSynchroDetected;
	JUMP AdSynchroIdle1;

StepSynchro:
	AY0=DM(AdSynchroStep);
	AF=PASS AY0;
	IF NE JUMP LastAdSynchro;
	AY0=DM(SynchroAdPorog);
	AF=AR-AY0;
	AR=DM(SynchroAdMode);
	AR=PASS AR;
	IF NE AF=-AF;
	AF=PASS AF;
	IF GE JUMP AdSynchroIdle1;
	AR=0x1;
	DM(AdSynchroStep)=AR;
	JUMP AdSynchroIdle1;

AdSynchroDetected:
{ ��⠭���� ��� �몫�稢 �����, �.�. ᤥ���� SCLK1 ���譨�					}
	AR = 0x3F1F; 						{ 0011 1111 0001 1111 						}
	DM(Sport1_Ctrl_Reg) = AR;		{ 0x3FF2 - SPORT1 Control Register		}

{ ����᪨�㥬 ���뢠��� �� ��� IRQE												}
	AR=DM(IMASK_VALUE);
	AR=CLRBIT 4 OF AR;
	IMASK=AR; NOP;

{ ����প� �� 0.4 ���, �⮡� ���ਫ�� ��᫥���� �����						}
	cntr=16;
	DO AdSynchroLoop1 UNTIL CE;
AdSynchroLoop1: NOP;

{ ������� �ᨫ���� � ����� ������ ��� ᫥���饣� �����						}
	I2=^Control_Table;
	SET FL0;
	AR=DM(I2, M2);
	IO(SetChannel)=AR;
	RESET FL0;
	AR=DM(I2, M2);
	IO(SetChannel)=AR;

{ ����⠭���� ���� ��ࠡ��稪 ���뢠��� IRQE 								}
	AR=^IrqE_Handler;
	I6=AR;
	M6=0x0;
	AR=DM(PM_Data);
	PX=DM(PX_Reg);
	PM(I6, M6)=AR;

{ ��������쭠� ����প�, �⮡� ��⠭������ ��������� �ࠪ�					}
	AR=DM(FirstSampleDelay);
	CNTR=AR;
	DO AdSynchroLoop2 UNTIL CE;
AdSynchroLoop2: NOP;

{ ����⠭���� ����� ࠡ��� ���														}
	AR=DM(ADC_Rate);				{ ���� ����᪠ ���							}
	DM(Sport1_Sclkdiv) = AR;	{ 0x3FF5 - Serial Clock Divide Modulus		}

{ �� ������⨫� ॣ���� AF																}
	AF= PASS 0;						{ AF - ���稪 ����⮢ � ���					}

{ ��⠭���� 䫠���, �� ࠧ�襭� ࠡ�� ���										}
	AR=0x1;
	DM(ADC_Enable)=AR;

{ ���⨬ ������ �� ���뢠���, �஬� SPORT0 Transmit (���) � IRQ2		}
	IFC=0x3F; NOP;

{ ࠧ�訬 ���뢠��� IRQ2 �, ��������, SPORT1 Transmit � IRQE				}
	IMASK=DM(IMASK_VALUE); NOP;

{ ࠧ�訬 ��� ����稢 �����, �.�. ᤥ���� SCLK1 ����७���					}
	AR = 0x7F1F;						{ 0111 1101 1100 1111						}
	DM(Sport1_Ctrl_Reg) = AR;		{ 0x3FF2 - SPORT1 Control Register		}

	I6=DM(TopStack);
	JUMP (I6); 							{ �������� ��ଠ��� ������ ������		}

{ *********************************************************************	}
{ ��ࠡ��稪 ���뢠��� IRQL0 �� ���������� 									}
{														ᨭ�஭���樨 �� ������ ���	}
{ *********************************************************************	}
IrqEAdcSynchro:
{ ��⠥� ���祭�� ��� � ᨭ�ப�����												}
	AR=IO(ReadADC);

{ ��⠭���� 䫠���, �� �뫮 ���뢠��� �� ���									}
	AY0=1;
	DM(IsAdcSample)=AY0;

	RTI;

{ *********************************************************************	}
{ ������� � PM �� ��������� ����� ������� JUMP xxx			    			}
{ �室:                                                     	  			}
{      I6 - ����, �㤠 �����뢠���� �������             					}
{      AR - ���� ���室� xxx														}
{ *********************************************************************	}
ModifyJump:
{ ����頥� �� ���뢠���																}
	DIS INTS;

{ ��ନ�㥬 ��� ������� 																}
	SR1=0x0018;								{ 00000000 00011000 										}
	SR0=0x000F;								{ 00000000 00001111 						}
	SR=SR OR LSHIFT AR BY 4(LO);
	PX=SR0;									{ ����稫� 8 ������ ��� 				}
	AR=SR1;
	SR=LSHIFT SR0 BY -8(LO);
	SR=SR OR LSHIFT AR BY -8(HI);		{ ����稫� ���訥 16 ��� 				}
	M6=0x0;
	PM(I6,M6)=SR0;							{ ���࠭��� �� 24-��⮢�� ᫮�� 	}

{ ࠧ�蠥� �� ���뢠���																}
	ENA INTS;

	RTS;

