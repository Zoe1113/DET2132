ifndef __SN8P2988__INC__H
SN8P2988__INC__H equ 1
	L	EQU	0x80
	H	EQU	0x81
	R	EQU	0x82
	Z	EQU	0x83
	Y	EQU	0x84
	VREG2	EQU	0x85
		FDUMMY5	EQU	VREG2.7
		FDUMMY4	EQU	VREG2.6
		FDUMMY3	EQU	VREG2.5
		FDUMMY2	EQU	VREG2.4
		FDUMMY1	EQU	VREG2.3
		FDUMMY0	EQU	VREG2.2
		FACMSEL	EQU	VREG2.1
		FACMEN	EQU	VREG2.0
	PFLAG	EQU	0x86
		FC	EQU	PFLAG.2
		FDC	EQU	PFLAG.1
		FZ	EQU	PFLAG.0
	RBANK	EQU	0x87
		FRBNKS3	EQU	RBANK.3
		FRBNKS2	EQU	RBANK.2
		FRBNKS1	EQU	RBANK.1
		FRBNKS0	EQU	RBANK.0
	WR0	EQU	0x88
	WR1	EQU	0x89
	WR2	EQU	0x8a
	URTX1	EQU	0x8b
		FUTX1EN	EQU	URTX1.7
		FUTX1PEN	EQU	URTX1.6
		FUTX1PS	EQU	URTX1.5
		FUTX1BRK	EQU	URTX1.4
		FURX1BZ	EQU	URTX1.3
		FUTX1BZ	EQU	URTX1.2
	URRX1	EQU	0x8c
		FURX1EN	EQU	URRX1.7
		FURX1PEN	EQU	URRX1.6
		FURX1PS	EQU	URRX1.5
		FURX1PC	EQU	URRX1.4
		FUFMER1	EQU	URRX1.3
		FURS12	EQU	URRX1.2
		FURS11	EQU	URRX1.1
		FURS10	EQU	URRX1.0
	URCR1	EQU	0x8d
	UTX1D	EQU	0x8e
	URX1D	EQU	0x8f
	VREG	EQU	0x90
		FBGREN	EQU	VREG.7
		FBGCHP	EQU	VREG.6
		FAVEN	EQU	VREG.5
		FAVESEL	EQU	VREG.4
		FAVDDREN	EQU	VREG.2
		FAVDDRSEL1	EQU	VREG.1
		FAVDDRSEL0	EQU	VREG.0
	CHS	EQU	0x91
		FMUXP3	EQU	CHS.7
		FMUXP2	EQU	CHS.6
		FMUXP1	EQU	CHS.5
		FMUXP0	EQU	CHS.4
		FMUXN3	EQU	CHS.3
		FMUXN2	EQU	CHS.2
		FMUXN1	EQU	CHS.1
		FMUXN0	EQU	CHS.0
	AMPM	EQU	0x92
		FAMPCKS2	EQU	AMPM.7
		FAMPCKS1	EQU	AMPM.6
		FAMPCKS0	EQU	AMPM.5
		FPCHPEN	EQU	AMPM.4
		FGS2	EQU	AMPM.3
		FGS1	EQU	AMPM.2
		FGS0	EQU	AMPM.1
		FAMPEN	EQU	AMPM.0
	ADCM1	EQU	0x93
		FRVS3	EQU	ADCM1.7
		FRVS2	EQU	ADCM1.6
		FRVS1	EQU	ADCM1.5
		FRVS0	EQU	ADCM1.4
		FACHPEN	EQU	ADCM1.3
		FGX	EQU	ADCM1.2
		FADGN	EQU	ADCM1.1
		FADC1EN	EQU	ADCM1.0
	ADCM2	EQU	0x94
		FADCKS	EQU	ADCM2.7
		FOSR2	EQU	ADCM2.6
		FOSR1	EQU	ADCM2.5
		FOSR0	EQU	ADCM2.4
		FGR	EQU	ADCM2.3
		FOFSEL1	EQU	ADCM2.2
		FOFSEL0	EQU	ADCM2.1
		FDRDY	EQU	ADCM2.0
	LBTM	EQU	0x95
		FLBTSEL3	EQU	LBTM.5
		FLBTSEL2	EQU	LBTM.4
		FLBTSEL1	EQU	LBTM.3
		FLBTSEL0	EQU	LBTM.2
		FLBTO	EQU	LBTM.1
		FLBTEN	EQU	LBTM.0
	ADCDH	EQU	0x96
	ADCDM	EQU	0x97
	ADCDL	EQU	0x98
	TC0CH	EQU	0x99
	TC0RH	EQU	0x9a
	TC0DL	EQU	0x9b
	TC0DH	EQU	0x9c
	TC1CH	EQU	0x9d
	TC1RH	EQU	0x9e
	TC1DL	EQU	0x9f
	ROMADRH	EQU	0xa0
		FROMADR15	EQU	ROMADRH.7
		FROMADR13	EQU	ROMADRH.5
		FROMADR12	EQU	ROMADRH.4
		FROMADR11	EQU	ROMADRH.3
		FROMADR10	EQU	ROMADRH.2
		FROMADR9	EQU	ROMADRH.1
		FROMADR8	EQU	ROMADRH.0
	ROMADRL	EQU	0xa1
		FROMADR7	EQU	ROMADRL.7
		FROMADR6	EQU	ROMADRL.6
		FROMADR5	EQU	ROMADRL.5
		FROMADR4	EQU	ROMADRL.4
		FROMADR3	EQU	ROMADRL.3
		FROMADR2	EQU	ROMADRL.2
		FROMADR1	EQU	ROMADRL.1
		FROMADR0	EQU	ROMADRL.0
	ROMDAH	EQU	0xa2
		FROMDA15	EQU	ROMDAH.7
		FROMDA14	EQU	ROMDAH.6
		FROMDA13	EQU	ROMDAH.5
		FROMDA12	EQU	ROMDAH.4
		FROMDA11	EQU	ROMDAH.3
		FROMDA10	EQU	ROMDAH.2
		FROMDA9	EQU	ROMDAH.1
		FROMDA8	EQU	ROMDAH.0
	ROMDAL	EQU	0xa3
		FROMDA7	EQU	ROMDAL.7
		FROMDA6	EQU	ROMDAL.6
		FROMDA5	EQU	ROMDAL.5
		FROMDA4	EQU	ROMDAL.4
		FROMDA3	EQU	ROMDAL.3
		FROMDA2	EQU	ROMDAL.2
		FROMDA1	EQU	ROMDAL.1
		FROMDA0	EQU	ROMDAL.0
	DA1M	EQU	0xa4
		FDA1EN	EQU	DA1M.7
		FDA1REF1	EQU	DA1M.5
		FDA1REF0	EQU	DA1M.4
		FDA1B11	EQU	DA1M.3
		FDA1B10	EQU	DA1M.2
		FDA1B9	EQU	DA1M.1
		FDA1B8	EQU	DA1M.0
	DA1BL	EQU	0xa5
		FDA1B7	EQU	DA1BL.7
		FDA1B6	EQU	DA1BL.6
		FDA1B5	EQU	DA1BL.5
		FDA1B4	EQU	DA1BL.4
		FDA1B3	EQU	DA1BL.3
		FDA1B2	EQU	DA1BL.2
		FDA1B1	EQU	DA1BL.1
		FDA1B0	EQU	DA1BL.0
	MSPSTAT	EQU	0xa6
		FCKE	EQU	MSPSTAT.6
		FD_A	EQU	MSPSTAT.5
		FP	EQU	MSPSTAT.4
		FS	EQU	MSPSTAT.3
		FRED_WRT	EQU	MSPSTAT.2
		FBF	EQU	MSPSTAT.0
	MSPM1	EQU	0xa7
		FWCOL	EQU	MSPM1.7
		FMSPOV	EQU	MSPM1.6
		FMSPENB	EQU	MSPM1.5
		FCKP	EQU	MSPM1.4
		FSLRXCKP	EQU	MSPM1.3
		FMSPWK	EQU	MSPM1.2
		FMSPC	EQU	MSPM1.0
	MSPM2	EQU	0xa8
		FGCEN	EQU	MSPM2.7
		FACKSTAT	EQU	MSPM2.6
		FACKDT	EQU	MSPM2.5
		FACKEN	EQU	MSPM2.4
		FRCEN	EQU	MSPM2.3
		FPEN	EQU	MSPM2.2
		FRSEN	EQU	MSPM2.1
		FSEN	EQU	MSPM2.0
	MSPBUF	EQU	0xa9
	MSPADR	EQU	0xaa
	TC1DH	EQU	0xab
	TC2CH	EQU	0xac
	TC2RH	EQU	0xad
	TC2DL0	EQU	0xae
	TC2DH0	EQU	0xaf
	LCDM1	EQU	0xb0
		FDUTY1	EQU	LCDM1.7
		FDUTY0	EQU	LCDM1.6
		FLCDMOD1	EQU	LCDM1.5
		FLCDMOD0	EQU	LCDM1.4
		FLCDEN	EQU	LCDM1.3
		FLCDBIAS	EQU	LCDM1.2
		FLCDRATE	EQU	LCDM1.1
		FLCDCLK	EQU	LCDM1.0
	LCDM2	EQU	0xb1
		FVAR1	EQU	LCDM2.7
		FVAR0	EQU	LCDM2.6
		FDISQ	EQU	LCDM2.5
		FLCDBNK	EQU	LCDM2.4
		FVPPINTL	EQU	LCDM2.3
		FVCP2	EQU	LCDM2.2
		FVCP1	EQU	LCDM2.1
		FVCP0	EQU	LCDM2.0
	P2SEG	EQU	0xb2
	P3SEG	EQU	0xb3
	BZRM	EQU	0xb4
		FBZREN	EQU	BZRM.2
		FBZRCKS1	EQU	BZRM.1
		FBZRCKS0	EQU	BZRM.0
	P1SEG	EQU	0xb5
	P4SEG	EQU	0xb6
	P5SEG	EQU	0xb7
	OPM1	EQU	0xb8
		FSW3	EQU	OPM1.7
		FSW2	EQU	OPM1.6
		FSW1	EQU	OPM1.5
		FSW0	EQU	OPM1.4
		FOPA2CHP	EQU	OPM1.3
		FOPA2EN	EQU	OPM1.2
		FOPA1CHP	EQU	OPM1.1
		FOPA1EN	EQU	OPM1.0
	OPM2	EQU	0xb9
		FSW11	EQU	OPM2.7
		FSW10	EQU	OPM2.6
		FSW9	EQU	OPM2.5
		FSW8	EQU	OPM2.4
		FSW7	EQU	OPM2.3
		FSW6	EQU	OPM2.2
		FSW5	EQU	OPM2.1
		FSW4	EQU	OPM2.0
	ADM	EQU	0xba
		FADC2EN	EQU	ADM.7
		FADS	EQU	ADM.6
		FEOC	EQU	ADM.5
		FCHS3	EQU	ADM.3
		FCHS2	EQU	ADM.2
		FCHS1	EQU	ADM.1
		FCHS0	EQU	ADM.0
	ADB	EQU	0xbb
		FADB11	EQU	ADB.7
		FADB10	EQU	ADB.6
		FADB9	EQU	ADB.5
		FADB8	EQU	ADB.4
		FADB7	EQU	ADB.3
		FADB6	EQU	ADB.2
		FADB5	EQU	ADB.1
		FADB4	EQU	ADB.0
	ADR	EQU	0xbc
		FLVBAT	EQU	ADR.7
		FGCHS	EQU	ADR.6
		FADCKS1	EQU	ADR.5
		FADCKS0	EQU	ADR.4
		FADB3	EQU	ADR.3
		FADB2	EQU	ADR.2
		FADB1	EQU	ADR.1
		FADB0	EQU	ADR.0
	LCDM3	EQU	0xbd
		FBGM	EQU	LCDM3.1
		FLCDMODE	EQU	LCDM3.0
	ADT	EQU	0xbe
		FAD2VREF	EQU	ADT.5
		FADT4	EQU	ADT.4
		FADT3	EQU	ADT.3
		FADT2	EQU	ADT.2
		FADT1	EQU	ADT.1
		FADT0	EQU	ADT.0
	PEDGE	EQU	0xbf
		FP01G1	EQU	PEDGE.6
		FP01G0	EQU	PEDGE.5
		FP00G1	EQU	PEDGE.4
		FP00G0	EQU	PEDGE.3
	P0M	EQU	0xc0
		FP07M	EQU	P0M.7
		FP06M	EQU	P0M.6
		FP05M	EQU	P0M.5
		FP04M	EQU	P0M.4
		FP03M	EQU	P0M.3
		FP02M	EQU	P0M.2
		FP01M	EQU	P0M.1
		FP00M	EQU	P0M.0
	P1M	EQU	0xc1
		FP17M	EQU	P1M.7
		FP16M	EQU	P1M.6
		FP15M	EQU	P1M.5
		FP14M	EQU	P1M.4
		FP13M	EQU	P1M.3
		FP12M	EQU	P1M.2
		FP11M	EQU	P1M.1
		FP10M	EQU	P1M.0
	P2M	EQU	0xc2
		FP27M	EQU	P2M.7
		FP26M	EQU	P2M.6
		FP25M	EQU	P2M.5
		FP24M	EQU	P2M.4
		FP23M	EQU	P2M.3
		FP22M	EQU	P2M.2
		FP21M	EQU	P2M.1
		FP20M	EQU	P2M.0
	P3M	EQU	0xc3
		FP37M	EQU	P3M.7
		FP36M	EQU	P3M.6
		FP35M	EQU	P3M.5
		FP34M	EQU	P3M.4
		FP33M	EQU	P3M.3
		FP32M	EQU	P3M.2
		FP31M	EQU	P3M.1
		FP30M	EQU	P3M.0
	P4M	EQU	0xc4
		FP47M	EQU	P4M.7
		FP46M	EQU	P4M.6
		FP45M	EQU	P4M.5
		FP44M	EQU	P4M.4
		FP43M	EQU	P4M.3
		FP42M	EQU	P4M.2
		FP41M	EQU	P4M.1
		FP40M	EQU	P4M.0
	P5M	EQU	0xc5
		FP57M	EQU	P5M.7
		FP56M	EQU	P5M.6
		FP55M	EQU	P5M.5
		FP54M	EQU	P5M.4
		FP53M	EQU	P5M.3
		FP52M	EQU	P5M.2
		FP51M	EQU	P5M.1
		FP50M	EQU	P5M.0
	INTRQ1	EQU	0xc6
		FMSPIRQ	EQU	INTRQ1.4
		FTC2IRQ	EQU	INTRQ1.3
		FTC1IRQ	EQU	INTRQ1.2
		FTC0IRQ	EQU	INTRQ1.1
		FT0IRQ	EQU	INTRQ1.0
	INTEN1	EQU	0xc7
		FMSPIEN	EQU	INTEN1.4
		FTC2IEN	EQU	INTEN1.3
		FTC1IEN	EQU	INTEN1.2
		FTC0IEN	EQU	INTEN1.1
		FT0IEN	EQU	INTEN1.0
	INTRQ0	EQU	0xc8
		FADC2IRQ	EQU	INTRQ0.7
		FADC1IRQ	EQU	INTRQ0.6
		FURX1IRQ	EQU	INTRQ0.5
		FUTX1IRQ	EQU	INTRQ0.4
		FURX0IRQ	EQU	INTRQ0.3
		FUTX0IRQ	EQU	INTRQ0.2
		FP01IRQ	EQU	INTRQ0.1
		FP00IRQ	EQU	INTRQ0.0
	INTEN0	EQU	0xc9
		FADC2IEN	EQU	INTEN0.7
		FADC1IEN	EQU	INTEN0.6
		FURX1IEN	EQU	INTEN0.5
		FUTX1IEN	EQU	INTEN0.4
		FURX0IEN	EQU	INTEN0.3
		FUTX0IEN	EQU	INTEN0.2
		FP01IEN	EQU	INTEN0.1
		FP00IEN	EQU	INTEN0.0
	OSCM	EQU	0xca
		FCPUM1	EQU	OSCM.4
		FCPUM0	EQU	OSCM.3
		FCLKMD	EQU	OSCM.2
		FSTPHX	EQU	OSCM.1
	FRQH	EQU	0xcb
		FCKEN	EQU	FRQH.7
		FFRQ9	EQU	FRQH.1
		FFRQ8	EQU	FRQH.0
	WDTR	EQU	0xcc
	FRQL	EQU	0xcd
		FFRQ7	EQU	FRQL.7
		FFRQ6	EQU	FRQL.6
		FFRQ5	EQU	FRQL.5
		FFRQ4	EQU	FRQL.4
		FFRQ3	EQU	FRQL.3
		FFRQ2	EQU	FRQL.2
		FFRQ1	EQU	FRQL.1
		FFRQ0	EQU	FRQL.0
	PCL	EQU	0xce
	PCH	EQU	0xcf
		FPC13	EQU	PCH.5
		FPC12	EQU	PCH.4
		FPC11	EQU	PCH.3
		FPC10	EQU	PCH.2
		FPC9	EQU	PCH.1
		FPC8	EQU	PCH.0
	P0	EQU	0xd0
		FP07	EQU	P0.7
		FP06	EQU	P0.6
		FP05	EQU	P0.5
		FP04	EQU	P0.4
		FP03	EQU	P0.3
		FP02	EQU	P0.2
		FP01	EQU	P0.1
		Port_On	EQU	P0.0
	P1	EQU	0xd1
		FP17	EQU	P1.7
		FP16	EQU	P1.6
		FP15	EQU	P1.5
		FP14	EQU	P1.4
		FP13	EQU	P1.3
		FP12	EQU	P1.2
		FP11	EQU	P1.1
		FP10	EQU	P1.0
	P2	EQU	0xd2
		FP27	EQU	P2.7
		FP26	EQU	P2.6
		FP25	EQU	P2.5
		FP24	EQU	P2.4
		FP23	EQU	P2.3
		FP22	EQU	P2.2
		FP21	EQU	P2.1
		FP20	EQU	P2.0
	P3	EQU	0xd3
		FP37	EQU	P3.7
		FP36	EQU	P3.6
		FP35	EQU	P3.5
		FP34	EQU	P3.4
		FP33	EQU	P3.3
		FP32	EQU	P3.2
		FP31	EQU	P3.1
		FP30	EQU	P3.0
	P4	EQU	0xd4
		FP47	EQU	P4.7
		FP46	EQU	P4.6
		FP45	EQU	P4.5
		FP44	EQU	P4.4
		FP43	EQU	P4.3
		FP42	EQU	P4.2
		FP41	EQU	P4.1
		FP40	EQU	P4.0
	P5	EQU	0xd5
		FP57	EQU	P5.7
		FP56	EQU	P5.6
		FP55	EQU	P5.5
		FP54	EQU	P5.4
		FP53	EQU	P5.3
		FP52	EQU	P5.2
		FP51	EQU	P5.1
		FP50	EQU	P5.0
	P4UR	EQU	0xd6
		FP47R	EQU	P4UR.7
		FP46R	EQU	P4UR.6
		FP45R	EQU	P4UR.5
		FP44R	EQU	P4UR.4
		FP43R	EQU	P4UR.3
		FP42R	EQU	P4UR.2
		FP41R	EQU	P4UR.1
		FP40R	EQU	P4UR.0
	P5UR	EQU	0xd7
		FP57R	EQU	P5UR.7
		FP56R	EQU	P5UR.6
		FP55R	EQU	P5UR.5
		FP54R	EQU	P5UR.4
		FP53R	EQU	P5UR.3
		FP52R	EQU	P5UR.2
		FP51R	EQU	P5UR.1
		FP50R	EQU	P5UR.0
	T0M	EQU	0xd8
		FT0EN	EQU	T0M.7
		FT0RATE2	EQU	T0M.6
		FT0RATE1	EQU	T0M.5
		FT0RATE0	EQU	T0M.4
		FTC0GN	EQU	T0M.1
		FT0TB	EQU	T0M.0
	T0C	EQU	0xd9
	TC0M	EQU	0xda
		FTC0ENB	EQU	TC0M.7
		FTC0RATE2	EQU	TC0M.6
		FTC0RATE1	EQU	TC0M.5
		FTC0RATE0	EQU	TC0M.4
		FTC0CKS1	EQU	TC0M.3
		FTC0CKS0	EQU	TC0M.2
		FPWM0OUT	EQU	TC0M.1
	TC0CL	EQU	0xdb
	TC0RL	EQU	0xdc
	P2UR	EQU	0xdd
		FP27R	EQU	P2UR.7
		FP26R	EQU	P2UR.6
		FP25R	EQU	P2UR.5
		FP24R	EQU	P2UR.4
		FP23R	EQU	P2UR.3
		FP22R	EQU	P2UR.2
		FP21R	EQU	P2UR.1
		FP20R	EQU	P2UR.0
	P3UR	EQU	0xde
		FP37R	EQU	P3UR.7
		FP36R	EQU	P3UR.6
		FP35R	EQU	P3UR.5
		FP34R	EQU	P3UR.4
		FP33R	EQU	P3UR.3
		FP32R	EQU	P3UR.2
		FP31R	EQU	P3UR.1
		FP30R	EQU	P3UR.0
	STKP	EQU	0xdf
		FGIE	EQU	STKP.7
		FSTKPB2	EQU	STKP.2
		FSTKPB1	EQU	STKP.1
		FSTKPB0	EQU	STKP.0
	P0UR	EQU	0xe0
		FP07R	EQU	P0UR.7
		FP06R	EQU	P0UR.6
		FP05R	EQU	P0UR.5
		FP04R	EQU	P0UR.4
		FP03R	EQU	P0UR.3
		FP02R	EQU	P0UR.2
		FP01R	EQU	P0UR.1
		FP00R	EQU	P0UR.0
	P1UR	EQU	0xe1
		FP17R	EQU	P1UR.7
		FP16R	EQU	P1UR.6
		FP15R	EQU	P1UR.5
		FP14R	EQU	P1UR.4
		FP13R	EQU	P1UR.3
		FP12R	EQU	P1UR.2
		FP11R	EQU	P1UR.1
		FP10R	EQU	P1UR.0
	URTX0	EQU	0xe2
		FUTX0EN	EQU	URTX0.7
		FUTX0PEN	EQU	URTX0.6
		FUTX0PS	EQU	URTX0.5
		FUTX0BRK	EQU	URTX0.4
		FURX0BZ	EQU	URTX0.3
		FUTX0BZ	EQU	URTX0.2
	URRX0	EQU	0xe3
		FURX0EN	EQU	URRX0.7
		FURX0PEN	EQU	URRX0.6
		FURX0PS	EQU	URRX0.5
		FURX0PC	EQU	URRX0.4
		FUFMER0	EQU	URRX0.3
		FURS02	EQU	URRX0.2
		FURS01	EQU	URRX0.1
		FURS00	EQU	URRX0.0
	URCR0	EQU	0xe4
	UTX0D	EQU	0xe5
	URX0D	EQU	0xe6
	@YZ	EQU	0xe7
	@HL	EQU	0xe8
	TC1M	EQU	0xe9
		FTC1ENB	EQU	TC1M.7
		FTC1RATE2	EQU	TC1M.6
		FTC1RATE1	EQU	TC1M.5
		FTC1RATE0	EQU	TC1M.4
		FTC1CKS1	EQU	TC1M.3
		FTC1CKS0	EQU	TC1M.2
		FPWM1OUT	EQU	TC1M.1
	TC1CL	EQU	0xea
	TC1RL	EQU	0xeb
	TC2M	EQU	0xec
		FTC2ENB	EQU	TC2M.7
		FTC2RATE2	EQU	TC2M.6
		FTC2RATE1	EQU	TC2M.5
		FTC2RATE0	EQU	TC2M.4
		FTC2CKS1	EQU	TC2M.3
		FTC2CKS0	EQU	TC2M.2
		FTC2MOD1	EQU	TC2M.1
		FTC2MOD0	EQU	TC2M.0
	TC2CL	EQU	0xed
	TC2RL	EQU	0xee
	PWMCTL	EQU	0xef
		FPWM9OUT	EQU	PWMCTL.7
		FPWM8OUT	EQU	PWMCTL.6
		FPWM7OUT	EQU	PWMCTL.5
		FPWM6OUT	EQU	PWMCTL.4
		FPWM5OUT	EQU	PWMCTL.3
		FPWM4OUT	EQU	PWMCTL.2
		FPWM3OUT	EQU	PWMCTL.1
		FPWM2OUT	EQU	PWMCTL.0
	STK7L	EQU	0xf0
	STK7H	EQU	0xf1
	STK6L	EQU	0xf2
	STK6H	EQU	0xf3
	STK5L	EQU	0xf4
	STK5H	EQU	0xf5
	STK4L	EQU	0xf6
	STK4H	EQU	0xf7
	STK3L	EQU	0xf8
	STK3H	EQU	0xf9
	STK2L	EQU	0xfa
	STK2H	EQU	0xfb
	STK1L	EQU	0xfc
	STK1H	EQU	0xfd
	STK0L	EQU	0xfe
	STK0H	EQU	0xff
	TC2DL1	EQU	0xe00
		FTC2DL17	EQU	TC2DL1.7
		FTC2DL16	EQU	TC2DL1.6
		FTC2DL15	EQU	TC2DL1.5
		FTC2DL14	EQU	TC2DL1.4
		FTC2DL13	EQU	TC2DL1.3
		FTC2DL12	EQU	TC2DL1.2
		FTC2DL11	EQU	TC2DL1.1
		FTC2DL10	EQU	TC2DL1.0
	TC2DH1	EQU	0xe01
		FTC2DH17	EQU	TC2DH1.7
		FTC2DH16	EQU	TC2DH1.6
		FTC2DH15	EQU	TC2DH1.5
		FTC2DH14	EQU	TC2DH1.4
		FTC2DH13	EQU	TC2DH1.3
		FTC2DH12	EQU	TC2DH1.2
		FTC2DH11	EQU	TC2DH1.1
		FTC2DH10	EQU	TC2DH1.0
	TC2DL2	EQU	0xe02
		FTC2DL27	EQU	TC2DL2.7
		FTC2DL26	EQU	TC2DL2.6
		FTC2DL25	EQU	TC2DL2.5
		FTC2DL24	EQU	TC2DL2.4
		FTC2DL23	EQU	TC2DL2.3
		FTC2DL22	EQU	TC2DL2.2
		FTC2DL21	EQU	TC2DL2.1
		FTC2DL20	EQU	TC2DL2.0
	TC2DH2	EQU	0xe03
		FTC2DH27	EQU	TC2DH2.7
		FTC2DH26	EQU	TC2DH2.6
		FTC2DH25	EQU	TC2DH2.5
		FTC2DH24	EQU	TC2DH2.4
		FTC2DH23	EQU	TC2DH2.3
		FTC2DH22	EQU	TC2DH2.2
		FTC2DH21	EQU	TC2DH2.1
		FTC2DH20	EQU	TC2DH2.0
	TC2DL3	EQU	0xe04
		FTC2DL37	EQU	TC2DL3.7
		FTC2DL36	EQU	TC2DL3.6
		FTC2DL35	EQU	TC2DL3.5
		FTC2DL34	EQU	TC2DL3.4
		FTC2DL33	EQU	TC2DL3.3
		FTC2DL32	EQU	TC2DL3.2
		FTC2DL31	EQU	TC2DL3.1
		FTC2DL30	EQU	TC2DL3.0
	TC2DH3	EQU	0xe05
		FTC2DH37	EQU	TC2DH3.7
		FTC2DH36	EQU	TC2DH3.6
		FTC2DH35	EQU	TC2DH3.5
		FTC2DH34	EQU	TC2DH3.4
		FTC2DH33	EQU	TC2DH3.3
		FTC2DH32	EQU	TC2DH3.2
		FTC2DH31	EQU	TC2DH3.1
		FTC2DH30	EQU	TC2DH3.0
	TC2DL4	EQU	0xe06
		FTC2DL47	EQU	TC2DL4.7
		FTC2DL46	EQU	TC2DL4.6
		FTC2DL45	EQU	TC2DL4.5
		FTC2DL44	EQU	TC2DL4.4
		FTC2DL43	EQU	TC2DL4.3
		FTC2DL42	EQU	TC2DL4.2
		FTC2DL41	EQU	TC2DL4.1
		FTC2DL40	EQU	TC2DL4.0
	TC2DH4	EQU	0xe07
		FTC2DH47	EQU	TC2DH4.7
		FTC2DH46	EQU	TC2DH4.6
		FTC2DH45	EQU	TC2DH4.5
		FTC2DH44	EQU	TC2DH4.4
		FTC2DH43	EQU	TC2DH4.3
		FTC2DH42	EQU	TC2DH4.2
		FTC2DH41	EQU	TC2DH4.1
		FTC2DH40	EQU	TC2DH4.0
	TC2DL5	EQU	0xe08
		FTC2DL57	EQU	TC2DL5.7
		FTC2DL56	EQU	TC2DL5.6
		FTC2DL55	EQU	TC2DL5.5
		FTC2DL54	EQU	TC2DL5.4
		FTC2DL53	EQU	TC2DL5.3
		FTC2DL52	EQU	TC2DL5.2
		FTC2DL51	EQU	TC2DL5.1
		FTC2DL50	EQU	TC2DL5.0
	TC2DH5	EQU	0xe09
		FTC2DH57	EQU	TC2DH5.7
		FTC2DH56	EQU	TC2DH5.6
		FTC2DH55	EQU	TC2DH5.5
		FTC2DH54	EQU	TC2DH5.4
		FTC2DH53	EQU	TC2DH5.3
		FTC2DH52	EQU	TC2DH5.2
		FTC2DH51	EQU	TC2DH5.1
		FTC2DH50	EQU	TC2DH5.0
	TC2DL6	EQU	0xe0a
		FTC2DL67	EQU	TC2DL6.7
		FTC2DL66	EQU	TC2DL6.6
		FTC2DL65	EQU	TC2DL6.5
		FTC2DL64	EQU	TC2DL6.4
		FTC2DL63	EQU	TC2DL6.3
		FTC2DL62	EQU	TC2DL6.2
		FTC2DL61	EQU	TC2DL6.1
		FTC2DL60	EQU	TC2DL6.0
	TC2DH6	EQU	0xe0b
		FTC2DH67	EQU	TC2DH6.7
		FTC2DH66	EQU	TC2DH6.6
		FTC2DH65	EQU	TC2DH6.5
		FTC2DH64	EQU	TC2DH6.4
		FTC2DH63	EQU	TC2DH6.3
		FTC2DH62	EQU	TC2DH6.2
		FTC2DH61	EQU	TC2DH6.1
		FTC2DH60	EQU	TC2DH6.0
	TC2DL7	EQU	0xe0c
		FTC2DL77	EQU	TC2DL7.7
		FTC2DL76	EQU	TC2DL7.6
		FTC2DL75	EQU	TC2DL7.5
		FTC2DL74	EQU	TC2DL7.4
		FTC2DL73	EQU	TC2DL7.3
		FTC2DL72	EQU	TC2DL7.2
		FTC2DL71	EQU	TC2DL7.1
		FTC2DL70	EQU	TC2DL7.0
	TC2DH7	EQU	0xe0d
		FTC2DH77	EQU	TC2DH7.7
		FTC2DH76	EQU	TC2DH7.6
		FTC2DH75	EQU	TC2DH7.5
		FTC2DH74	EQU	TC2DH7.4
		FTC2DH73	EQU	TC2DH7.3
		FTC2DH72	EQU	TC2DH7.2
		FTC2DH71	EQU	TC2DH7.1
		FTC2DH70	EQU	TC2DH7.0
	DA2M	EQU	0xe0e
		FDA2EN	EQU	DA2M.7
		FDA2REF1	EQU	DA2M.5
		FDA2REF0	EQU	DA2M.4
		FDA2B11	EQU	DA2M.3
		FDA2B10	EQU	DA2M.2
		FDA2B9	EQU	DA2M.1
		FDA2B8	EQU	DA2M.0
	DA2BL	EQU	0xe0f
		FDA2B7	EQU	DA2BL.7
		FDA2B6	EQU	DA2BL.6
		FDA2B5	EQU	DA2BL.5
		FDA2B4	EQU	DA2BL.4
		FDA2B3	EQU	DA2BL.3
		FDA2B2	EQU	DA2BL.2
		FDA2B1	EQU	DA2BL.1
		FDA2B0	EQU	DA2BL.0
	OPM3	EQU	0xe10
		FOPCKS1	EQU	OPM3.1
		FOPCKS0	EQU	OPM3.0
@RST_WDT	macro		
 		   MOV  A, #0x5A
		   B0MOV  WDTR, A
	endm

SleepMode	macro	
		B0BSET	FCPUM0
	endm

GreenMode	macro	
		B0BSET	FCPUM1
		NOP
		NOP
	endm

SLowMode	macro	
		B0BSET	FCLKMD
		B0BSET	FSTPHX
	endm

Slow2Normal	macro	
		B0BCLR	FSTPHX
		NOP
		NOP
		NOP
		B0BCLR	FCLKMD
	endm

RomwrtVpp	macro
	local		ISPP_End,ISP_Delay10ms	

	mov	a,#5AH	;clear WDT
	b0mov	WDTR,a

	;==== [ VPP Set 7.5V ] ===================
	MOV	A,#00100011b	; [ LCDBNK, -, LCDMOD1, LCDMOD0. LCDENB, LCDBIAS, LCDRATE, LCDCLK]
	B0XCH	a,LCDM1		; ChargePump start-up setting (ISP Mode, LCD Disable, 1/3 bias)
 	B0mov	Y,a		; Original setting of LCDM1 save in "Y" register

	mov	a,#5AH	;clear WDT
	b0mov	WDTR,a

	;==== [ ISP Dlay 150ms wait VPP 7.5V stable ] ===================	
	Call	ISP_Delay10ms	; for VPP 7.5V stable 
	Call	ISP_Delay10ms	; for VPP 7.5V stable 
	Call	ISP_Delay10ms	; for VPP 7.5V stable 
	Call	ISP_Delay10ms	; for VPP 7.5V stable 
	Call	ISP_Delay10ms	; for VPP 7.5V stable 
	Call	ISP_Delay10ms	; for VPP 7.5V stable 
	Call	ISP_Delay10ms	; for VPP 7.5V stable 
	Call	ISP_Delay10ms	; for VPP 7.5V stable 
	Call	ISP_Delay10ms	; for VPP 7.5V stable 
	Call	ISP_Delay10ms	; for VPP 7.5V stable 
	Call	ISP_Delay10ms	; for VPP 7.5V stable 
	Call	ISP_Delay10ms	; for VPP 7.5V stable 
	Call	ISP_Delay10ms	; for VPP 7.5V stable 
	Call	ISP_Delay10ms	; for VPP 7.5V stable 
	Call	ISP_Delay10ms	; for VPP 7.5V stable 


	Mov	a,#00001000b	; [ VAR1, VAR0, DISQ, -, VPPINTL, VCP2, VCP1, VCP0]
	B0XCH	a,LCDM2		; Set VCP=001 and VPPINTL=1. (VPP set 7.5V and VPP-VLCD short), others set default
	B0mov	z,a		; Original setting of LCDM2 save in "Z" register

	mov	a,#5AH	;clear WDT
	b0mov	WDTR,a

	;==== [ ISP ROM Write Command ] ===========
	nop
	nop
	nop
	ROMWRT			; ISP ROM Write Instruction
	nop			; must nop

	b0bclr	FVPPINTL

	;==== [VLCD drop to 2.6V ] ================
	b0bclr	FLCDBIAS	; Set 1/3 Bias
	mov	a,#00100000b	;[ VAR1, VAR0, DISQ, x, VPPINTL, VCP2, VCP1, VCP0 ]
	b0mov	LCDM2,a		; VLCD discharge to VLCD=2.6V, VPPINTL=0

	;-- Delay 10ms ---------- 
	Call	ISP_Delay10ms

	b0bclr	FDISQ

	;===== [ Original LCD seting ] ============
	B0mov	a,z
	b0mov	LCDM2,a		
	B0mov	a,y
	b0mov	LCDM1,a
	jmp	ISPP_End

ISP_Delay10ms:
	;-- Delay 10ms -- 
	b0bset	FCLKMD	; Slow Mode
	NOP

	mov	a,#00
	add	a,#1
	cmprs	a,#18
	jmp	$-2
	
	b0bclr	FCLKMD	; Normal Mode	
ret
ISPP_End:
	endm

Temp_calibration	macro	addr1,addr2,addr3
	local		Temp_calibration_END,ISP_command,ADC_Temp_set,ISP_Delay150ms_Temp,ISP_Delay10ms_Temp,Temp_error,clr_rdata_Temp_bit1,set_rdata_Temp_bit1,clr_rdata_Temp_bit0,bit_sent_routine,bit_sent_routine_function,bit_sent_routine_shift,Clr_command_bit1,Set_command_bit1,Clr_command_bit0,bit_accept_routine,bit_accept_routine_shift_1,bit_accept_routine_shift,Temp_change_addr_3,Temp_change_addr_2,Temp_change_addr,Temp_calibration_END_pr,Temp_DATA_to_59e90_2,Temp_addr_3,second_run_reload_TempAD,Start_reloat_float,Temp_DATA_to_59e90,Check_reload_data_method_1,Temp_addr_2,auto_temp_reload,ADC_Wait_Temp_isp,ROMADRL_PASS,Temp_convert_program,Temp_addr,Temp_value_isp_program,auto_temp_clication,selection_mode,state_command_3,state_command_2,state_selection,Password_command_bit0                                 



	Call	ISP_Delay10ms_Temp
/*
	mov	a,#00H
	b0mov	P1,a
	mov	a,#0FFH
	b0mov	P1M,a
	b0mov	P1,a
	mov	a,#00H
	b0mov	P1,a
	mov	a,#0FFH
	b0mov	P1,a
*/
	mov	a,p0
	nop
	nop
	and	a,#00011110B
	cmprs	a,#00011000B
	jmp	Temp_calibration_END	


	;===== [ Compare Password ] ============

	;====test io=======
	/*
	mov	a,#00H
	b0mov	P1,a
	mov	a,#0FFH
	b0mov	P1M,a
	*/
	;==================
	mov	a,#00001000b
	b0mov	P0m,a

//	call	ADC_Temp_set




Password_command_bit0:

	call 	bit_accept_routine_shift

	;========select_mode==========

state_selection:
	b0bclr	P0.3
	Call	ISP_Delay10ms_Temp
	b0bset	P0.3
	b0mov	a,Y
	cmprs	a,#5AH
	jmp	state_command_2
	mov	a,#00H
	jmp	selection_mode		

state_command_2:
	b0mov	a,Y
	cmprs	a,#0A5H
	jmp	state_command_3
	mov	a,#01H
	jmp	selection_mode	

state_command_3:
	;========Reseved==========
	jmp	Temp_calibration_END	

selection_mode:
	b0add	PCL,a
	jmp	auto_temp_clication
	jmp	auto_temp_reload	



auto_temp_clication:
	;===== [ Set Temp_AD Convertor] ============
	//call	ADC_Temp_set
	;===== [ Accept Temp and program this value by isp] ============

	call bit_accept_routine_shift
	b0mov	A,Y
	b0mov	ROMDAH,A
	call bit_accept_routine_shift
	b0mov	A,Y
	b0mov	ROMDAL,A



Temp_value_isp_program:

	;==== [ Check & set address ] ===================
/*
	B0MOV	Z, #00H
//	B0MOV	Y, #0FH
	B0MOV	Y, #07H
*/

	MOV	A,addr2
	B0MOV	Z,A
	MOV	A,addr1
	B0MOV	Y,A

Temp_addr:
	MOVC	
	CMPRS	A,#0FFH
	JMP	Temp_change_addr
	B0MOV	A, R	
	CMPRS	A,#0FFH	
	JMP	Temp_change_addr
	B0MOV	A,Z
	B0MOV	ROMADRL, A		;Move Low Byte Address to ROMADRL
	B0MOV	A,Y
	B0MOV	ROMADRH, A		;Move High Byte Address to ROMADRH

	CALL	ISP_command
	
	MOV	a,#00H
	B0MOV	R,A

Temp_convert_program:
	;===== [ Set Temp_AD Convertor] ============
	call	ADC_Temp_set
	CALL	ISP_Delay150ms_Temp	
	CALL	ISP_Delay150ms_Temp
	CALL	ISP_Delay150ms_Temp

	;===== [ Program Temp ADC Code ] ============
	INCMS	ROMADRL
	JMP	ROMADRL_PASS
	JMP	Temp_error
ROMADRL_PASS:	
	B0BCLR	FDRDY
ADC_Wait_Temp_isp:
	B0BTS1	FDRDY		;ADC Conversion Ready bit
	JMP	ADC_Wait_Temp_isp
	B0BCLR	FDRDY
	;===== [ FOR "Temp_convert_output" FUNCTION  ] ============	
	B0MOV	A,R
	CMPRS	A,#00h
	RET
	;==========================================================
	B0MOV	A,ADCDH
	B0MOV	ROMDAH,A		
	B0MOV	A,ADCDM
	B0MOV	ROMDAL,A	

	call	ISP_command


auto_temp_reload:
	b0bset	P0M.1
	b0bset	P0M.2	

	;==== [ Check_method_1:check from end addr ] ===================
/*
	B0MOV	Z, #50H
//	B0MOV	Y, #0FH
	B0MOV	Y, #07H
*/

	MOV	A,addr3	
	B0MOV	Z,A
	MOV	A,addr1
	B0MOV	Y,A


	;==== [ Check & set address ] ===================
/*
	;==== [ Check_method_2:check from start addr ] ===================	
	B0MOV	Z, #00H
	B0MOV	Y, #07H
*/
Temp_addr_2:
	MOVC	
	;==== [ Check_method_1:check from end addr ] ===================
	CMPRS	A,#0FFH
	JMP	Check_reload_data_method_1
	B0MOV	A, R	
	CMPRS	A,#0FFH	
	JMP	Check_reload_data_method_1
	JMP	Temp_change_addr_2	




Check_reload_data_method_1:
//	DECMS	Z			;if the Z=1,this condition will fail
//	JMP	Temp_DATA_to_59e90
//	JMP	Temp_error
	b0mov	a,Z
	SUB	a,#01H
	b0mov	Z,a

	
	
;========Temp_Data_to_sh59e90========
Temp_DATA_to_59e90:
	MOVC
	B0MOV	Z,A
	B0MOV	A,R
	B0MOV	Y,A
	;------init R=0 to determine that float_A was ended by R=1 -------
	MOV	A,#00H
	B0MOV	R,A
	;-----------------------------------------------------------------
Start_reloat_float:	
	CALL	bit_sent_routine_shift
	MOV	A,Z
	b0mov	Y,A
	CALL	bit_sent_routine_shift




second_run_reload_TempAD:
	b0mov	a,R
	cmprs	a,#00H
	jmp	Temp_calibration_END_pr
	;==== [ Check_method:check from end addr ] ===================
/*
	B0MOV	Z, #50H
//	B0MOV	Y, #0FH
	B0MOV	Y, #07H
*/

	MOV	A,addr3	
	B0MOV	Z,A
	MOV	A,addr1
	B0MOV	Y,A

	;==== [ Check & set address ] ===================
Temp_addr_3:
	MOVC	
	;==== [ Check_method:check from end addr ] ===================
	CMPRS	A,#0FFH
	JMP	Temp_DATA_to_59e90_2
	B0MOV	A, R	
	CMPRS	A,#0FFH	
	JMP	Temp_DATA_to_59e90_2
	JMP	Temp_change_addr_3

Temp_DATA_to_59e90_2:
	MOVC
	B0MOV	Z,A
	B0MOV	A,R
	B0MOV	Y,A
	;------init R=0 to determine that float_A was ended by R=1 -------
	MOV	A,#01H
	B0MOV	R,A
/*
	b0bclr	P0.3
	Call	ISP_Delay10ms_Temp	
	b0bset	P0.3
*/
	jmp	Start_reloat_float


Temp_calibration_END_pr:
/*
	b0bclr	P0.3
	Call	ISP_Delay10ms_Temp	
	b0bset	P0.3
*/
	JMP	Temp_calibration_END



Temp_change_addr:
	INCMS	Z
	JMP	Temp_addr
	JMP	Temp_error	

Temp_change_addr_2:
	;==== [ Check_method:check from end addr ] ===================
	DECMS	Z
	JMP	Temp_addr_2
	JMP	Temp_error
Temp_change_addr_3:
	;==== [ Check_method:check from end addr ] ===================
	DECMS	Z
	JMP	Temp_addr_3
	JMP	Temp_error



;============ [ Accept_data_function ] ===================
bit_accept_routine_shift:
	mov	a,#00H
	b0mov	Y,a
	b0mov	Z,a
	b0mov	R,a	
bit_accept_routine_shift_1:
	b0bclr	P0.3
	call 	bit_accept_routine
	RLCM	Y
	RLCM	Y
	RLCM	Y
	RLCM	Y
	RLCM	Y
	RLCM	Y
	MOV	A,#11000000b
	AND	A,Y
	OR	R,A
	b0bset	P0.3
	call 	bit_accept_routine
	RLCM	Y
	RLCM	Y
	RLCM	Y
	RLCM	Y
	MOV	A,#00110000b
	AND	A,Y
	OR	R,A
	b0bclr	P0.3
	call 	bit_accept_routine
	RLCM	Y
	RLCM	Y
	MOV	A,#00001100b
	AND	A,Y
	OR	R,A
	b0bset	P0.3
	call 	bit_accept_routine
	MOV	A,#00000011b
	AND	A,Y
	OR	R,A
	b0mov	A,R
	b0mov	Y,A
ret

bit_accept_routine:
//	b0bclr	P0.3
	Call	ISP_Delay10ms_Temp
	b0mov	a,P0
	and	a,#00000010B
	cmprs	a,#00000010B
	jmp	Clr_command_bit0
	b0bset	Y.0
	jmp	Set_command_bit1
Clr_command_bit0:
	b0bclr	Y.0

Set_command_bit1:
	b0mov	a,P0
	and	a,#00000100B
	cmprs	a,#00000100B
	jmp	Clr_command_bit1
	b0bset	Y.1
	ret

Clr_command_bit1:
	b0bclr	Y.1


ret
	
;=======================================================


;============ [ Sent_data_function ] ===================
bit_sent_routine_shift:
	
	
	call	bit_sent_routine
	Call	ISP_Delay10ms_Temp
	Call	ISP_Delay10ms_Temp
	b0bclr	P0.3
	call	bit_sent_routine_function
/*
	Call	ISP_Delay10ms_Temp
	RRCM	Y
	RRCM	Y
	call	bit_sent_routine
	Call	ISP_Delay10ms_Temp
	Call	ISP_Delay10ms_Temp
*/
	b0bset	P0.3
	call	bit_sent_routine_function
	b0bclr	P0.3
	call	bit_sent_routine_function
	b0bset	P0.3
	Call	ISP_Delay10ms_Temp


ret

bit_sent_routine_function:
	Call	ISP_Delay10ms_Temp
	RRCM	Y
	RRCM	Y
	call	bit_sent_routine
	Call	ISP_Delay10ms_Temp
	Call	ISP_Delay10ms_Temp

ret


bit_sent_routine:
	B0MOV	A,Y
	AND	A,#00000001B
	cmprs	a,#00000001B
	jmp	clr_rdata_Temp_bit0
	b0bset	P0.1
	jmp	set_rdata_Temp_bit1
clr_rdata_Temp_bit0:
	b0bclr	P0.1
	
set_rdata_Temp_bit1:
	B0MOV	A,Y
	AND	A,#00000010B
	cmprs	a,#00000010B
	jmp	clr_rdata_Temp_bit1
	b0bset	P0.2
	ret
clr_rdata_Temp_bit1:
	b0bclr	P0.2

ret
;=======================================================

Temp_error:
	b0bset	P0M.4
	b0bclr	P0.4
	JMP	$

ISP_Delay10ms_Temp:
	;-- Delay 10ms -- 
	b0bset	FCLKMD	; Slow Mode
	NOP

	mov	a,#00
	add	a,#1
	cmprs	a,#18
	jmp	$-2
	
	b0bclr	FCLKMD	; Normal Mode	
ret



ISP_Delay150ms_Temp:

	Call	ISP_Delay10ms_Temp
	Call	ISP_Delay10ms_Temp
	Call	ISP_Delay10ms_Temp
	Call	ISP_Delay10ms_Temp
	Call	ISP_Delay10ms_Temp
	Call	ISP_Delay10ms_Temp
	Call	ISP_Delay10ms_Temp
	Call	ISP_Delay10ms_Temp
	Call	ISP_Delay10ms_Temp
	Call	ISP_Delay10ms_Temp
	Call	ISP_Delay10ms_Temp
	Call	ISP_Delay10ms_Temp
	Call	ISP_Delay10ms_Temp
	Call	ISP_Delay10ms_Temp
	Call	ISP_Delay10ms_Temp
	
ret

ADC_Temp_set:
/*
	;===== [ Setting ADC_Temp ] ============

	;===== [ CPREG_Init ] ============
	B0BSET	FBGREN		; Enable Band Gap Reference voltage
;	B0BCLR	FBGREN		; Disable Band Gap Reference voltage	

	;===== [ AVDDR_Selection ] ============
;	B0BSET	FAVDDRENS	; Enable AVDDR Slow Turn On Output voltage. (@CR2032 Battery Case)
;	B0BCLR	FAVDDRENS	; AVDDR Normal Turn On Output voltage.

	;===== [ AVDDR_Enable ] ============
	B0BCLR	FAVDDRSEL1	; Set AVDDR output 2.4V ,AVDDRSEL[1:0]:01 = AVDDR 2.4V.
	B0BSET	FAVDDRSEL0	; Set AVDDR output 2.4V

	B0BSET	FAVDDREN	; Enable Regulator and AVDDR Output voltage

	;===== [ PGIA_Init ] ============
	MOV	A, #10101000B	; PGIA channel (Temperature sensor) and PGIA Gain x 1
	B0MOV	AMPM1, A	
	MOV	A, #00011100B 	;PGIA Chopper frequency enable amd PGIA chopper frequency selection(31.25KHz@ADC_CLK=250k)
	B0MOV	AMPM2, A	
	B0BSET	FAMPEN		; Enable PGIA function
				

	;===== [ ADC_Init ] ============
	MOV	A, #01000000B	;ADC Internal Reference Voltage(0.84V@AVDDR=2.4V)
	B0MOV	ADCM1, A	
 	B0BSET	FACHPEN		;ADC Chopper Enable
;	MOV	A, #01110110B	;ADC OSR Select 32768 and ADC Offset Slect 00% Vref
	MOV	A, #01000110B	;ADC OSR Select 4096 and ADC Offset Slect 00% Vref		
	B0MOV	ADCM2, A
;	B0BSET	FADCKS		;ADC clock source set 333kHz	
	B0BSET	FADCEN
*/
	;***********[Channel selection]*********************************
;	mov	a,#00000001B		;AI1/AI2	
;	mov	a,#00100011B		;AI3/AI4	
;	mov	a,#01010101B		;VDD_DET
	mov	a,#01100110B		;Temp
	mov	CHS,a
	;***********[Regulator setting]*********************************	
;	mov	a,#10111100b		;AVE=2V¡BAVDDR=2.2V
	mov	a,#10111101b		;AVE=2V¡BAVDDR=2.4V
;	mov	a,#10111110b		;AVE=2V¡BAVDDR=2.8V
;	mov	a,#10111111b		;AVE=2V¡BAVDDR=3.2V
;	mov	a,#10110100b		;AVE=1.5V¡BAVDDR=2.2V
;	mov	a,#10110101b		;AVE=1.5V¡BAVDDR=2.4V
;	mov	a,#10110110b		;AVE=1.5V¡BAVDDR=2.8V
;	mov	a,#10110111b		;AVE=1.5V¡BAVDDR=3.2V
	b0mov	VREG,a
	;***********[PGIA setting]**************************************	
;	mov	a,#00000111b
;	b0mov	AMPM1,a
;	mov	a,#00011100b
;	b0mov	AMPM2,a

;	mov	a,#00011101b

	mov	a,#00010000b		;Chopper=31.25K¡B Disable PGIA function. (PGIA input signal by pass to ADC)
;	mov	a,#00010001b		;Chopper=31.25K¡BPGIA=1x
;	mov	a,#00010011b		;Chopper=31.25K¡BPGIA=4x
;	mov	a,#00010101b		;Chopper=31.25K¡BPGIA=8x
;	mov	a,#00010111b		;Chopper=31.25K¡BPGIA=16x
;	mov	a,#00011001b		;Chopper=31.25K¡BPGIA=32x
;	mov	a,#00011011b		;Chopper=31.25K¡BPGIA=64x
;	mov	a,#00011101b		;Chopper=31.25K¡BPGIA=128x
;	mov	a,#00011111b		;Chopper=31.25K¡BPGIA=192x

	b0mov	AMPM,a	
	;**********[ADC setting]*****************************************	
;	mov	a,#01001001b		;ADC_G=1x¡BVref=0.36v
;	mov	a,#01011001b		;ADC_G=1x¡BVref=0.6v
;	mov	a,#01101001b		;ADC_G=1x¡BVref=0.84v
;	mov	a,#01111001b		;ADC_G=1x¡BVref=1.2v
;	mov	a,#01001011b		;ADC_G=2x¡BVref=0.36v
;	mov	a,#01011011b		;ADC_G=2x¡BVref=0.6v
;	mov	a,#01101011b		;ADC_G=2x¡BVref=0.84v
;	mov	a,#01111011b		;ADC_G=2x¡BVref=1.2v
	mov	a,#10001001b		;ADC_G=1x¡BVref=0.8VBG
	b0mov	ADCM1,a

	mov	a,#01000110b		;OSR=4096¡BOFFset=0*Vref
;	mov	a,#01110110b		;OSR=32768¡BOFFset=0*Vref
;	mov	a,#00100100b		;OSR=256¡BOFFset=-1/4*Vref
;	mov	a,#00100010b		;OSR=256¡BOFFset=-1/2*Vref
;	mov	a,#00100000b		;OSR=256¡BOFFset=-3/4*Vref
	b0mov	ADCM2,a
ret


ISP_command:
	;==== [ VPP Set 7.5V ] ===================
	MOV	A,#00100011b	; [ LCDBNK, -, LCDMOD1, LCDMOD0. LCDENB, LCDBIAS, LCDRATE, LCDCLK]
	XCH	a,LCDM1		; ChargePump start-up setting (ISP Mode, LCD Disable, 1/3 bias)
 	B0mov	Y,a		; Original setting of LCDM1 save in "Y" register

	;==== [ ISP Dlay 150ms wait VPP 7.5V stable ] ===================	

	CALL	ISP_Delay150ms_Temp

	Mov	a,#00001000b	; [ VAR1, VAR0, DISQ, -, VPPINTL, VCP2, VCP1, VCP0]
	XCH	a,LCDM2		; Set VCP=001 and VPPINTL=1. (VPP set 7.5V and VPP-VLCD short), others set default
	B0mov	z,a		; Original setting of LCDM2 save in "Z" register

	;==== [ ISP ROM Write Command ] ===========
	nop
	nop
	nop
	ROMWRT			; ISP ROM Write Instruction
	nop			; must nop

	b0bclr	FVPPINTL


	;==== [VLCD drop to 2.6V ] ================
	b0bclr	FLCDBIAS	; Set 1/3 Bias
	mov	a,#00100000b	;[ VAR1, VAR0, DISQ, x, VPPINTL, VCP2, VCP1, VCP0 ]
	b0mov	LCDM2,a		; VLCD discharge to VLCD=2.6V, VPPINTL=0

	;-- Delay 10ms ---------- 
	Call	ISP_Delay10ms_Temp

	b0bclr	FDISQ


	;===== [ Original LCD seting ] ============
	B0mov	a,z
	b0mov	LCDM2,a		
	B0mov	a,y
	b0mov	LCDM1,a
ret


/*
Temp_convert_output:
	call	ADC_Temp_set
	;==== [ Check from end addr ] ===================
	B0MOV	Z, #50H
//	B0MOV	Y, #0FH
	B0MOV	Y, #07H
Temp_out_addr_Check:
	MOVC	
	;==== [ Check_method_1:check from end addr ] ===================
	CMPRS	A,#0FFH
	JMP	Check_reload_data_method_1
	B0MOV	A, R	
	CMPRS	A,#0FFH	
	JMP	Check_reload_data_method_1
	DECMS	Z
	JMP	Temp_out_addr_Check	

;	b0mov	a,Z
;	SUB	a,#01H
;	b0mov	Z,a

	MOV	A,#01H
	B0MOV	R,A
	
	CALL	Temp_convert_program

	MOVC
	B0MOV	Z,A
	B0MOV	A,R
	B0MOV	Y,A


	MOV	A,ADCDM
	SUB	A,Z
	B0MOV	Z,A
	MOV	A,ADCDH
	SBC	A,Y
	B0MOV	Y,A	





	
ret

*/


Temp_calibration_END:
;	mov	a,#0FFH
;	b0mov	P1,a
//	jmp	main
//	jmp	main_loop
/*
org	0x700
RAM_TAB:
  DW 00FFFFH 0FFFFH 0FFFFH 0FFFFH 0FFFFH 0FFFFH 0FFFFH 0FFFFH 
  DW 00FFFFH 0FFFFH 0FFFFH 0FFFFH 0FFFFH 0FFFFH 0FFFFH 0FFFFH 
  DW 00FFFFH 0FFFFH 0FFFFH 0FFFFH 0FFFFH 0FFFFH 0FFFFH 0FFFFH 
  DW 00FFFFH 0FFFFH 0FFFFH 0FFFFH 0FFFFH 0FFFFH 0FFFFH 0FFFFH 
  DW 00FFFFH 0FFFFH 0FFFFH 0FFFFH 0FFFFH 0FFFFH 0FFFFH 0FFFFH 
  DW 00FFFFH 0FFFFH 0FFFFH 0FFFFH 0FFFFH 0FFFFH 0FFFFH 0FFFFH 
  DW 00FFFFH 0FFFFH 0FFFFH 0FFFFH 0FFFFH 0FFFFH 0FFFFH 0FFFFH 
  DW 00FFFFH 0FFFFH 0FFFFH 0FFFFH 0FFFFH 0FFFFH 0FFFFH 0FFFFH 
  DW 00FFFFH 0FFFFH 0FFFFH 0FFFFH 0FFFFH 0FFFFH 0FFFFH 0FFFFH 
  DW 00FFFFH 0FFFFH 0FFFFH 0FFFFH 0FFFFH 0FFFFH 0FFFFH 0FFFFH 
  DW 00FFFFH 0FFFFH 0FFFFH 0FFFFH 0FFFFH 0FFFFH 0FFFFH 0FFFFH 
  DW 00FFFFH 0FFFFH 0FFFFH 0FFFFH 0FFFFH 0FFFFH 0FFFFH 0FFFFH
*/


	endm

endif
