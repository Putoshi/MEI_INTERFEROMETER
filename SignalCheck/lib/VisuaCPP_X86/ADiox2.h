/*======================================================================================================*/
/* MultifunctionI/O symmetry driver file								*/
/*======================================================================================================*/
/*							Ver.1.00	�F2002.12.9			*/
/*							Designer	�FAkihiro Yokota		*/
/*							Copyright  2000-2002  SAYA Akihiro Yokota	*/
/*======================================================================================================*/

/*======================================================================================================*/
/*	���荞��ү����											*/
/*======================================================================================================*/
	#define WM_HWINTR0		(WM_USER+2004)
	#define WM_HWINTR1		(WM_USER+2005)
	#define WM_HWINTR2		(WM_USER+2006)
	#define WM_HWINTR3		(WM_USER+2007)
	#define WM_HWINTR4		(WM_USER+2008)
	#define WM_HWINTR5		(WM_USER+2009)
	#define WM_HWINTR6		(WM_USER+2010)
	#define WM_HWINTR7		(WM_USER+2011)
	#define WM_HWINTR8		(WM_USER+2012)
	#define WM_HWINTR9		(WM_USER+2013)
	#define WM_HWINTRA		(WM_USER+2014)
	#define WM_HWINTRB		(WM_USER+2015)
	#define WM_HWINTRC		(WM_USER+2016)
	#define WM_HWINTRD		(WM_USER+2017)
	#define WM_HWINTRE		(WM_USER+2018)
	#define WM_HWINTRF		(WM_USER+2019)
	#define WM_HWINTRG		(WM_USER+2020)

/*======================================================================================================*/
/*	�e���`											*/
/*======================================================================================================*/

	#define DMA_BURST_SPLEX		0x400000	// ADX14��e�ʃ����O�o�b�t�@�T�C�Y
	#define DMA_BURST_SPLEX_DW	0x100000	// ADX14��e�ʃ����O�o�b�t�@�T�C�Y
	#define	ADX42_LAN_BUFFER_SIZE	256		// ADX42�����O�o�b�t�@�T�C�Y
	#define	ADX42_LAN_BUFFER_SIZE_L	1024		// ADX42B-FULL�����O�o�b�t�@�T�C�Y
	#define	ADX85_PCI_BUFFER_SIZE	3328		// ADX85�v���C�}�������O�o�b�t�@�T�C�Y

	// �������޲��̎��ʒ萔
	#define	MAX_MFIO		32		// 0-3 PCI,4-99 COM
	#define	PCI_MAX			3
	#define	CARD_A			0
	#define	CARD_B			1
	#define	CARD_C			2
	#define	CARD_D			3
	#define	CARD_E			28
	#define	CARD_F			29
	#define	CARD_G			30
	#define	CARD_H			31
	#define	MAX_AI_CH		32		// ���̂Ƃ���16ch�ł��邪�����g���̂��߂�32ch�m��

/*======================================================================================================*/
/*	�\���̒�`											*/
/*======================================================================================================*/

	// �C�x���g�E���b�Z�[�W���(���荞��)
	struct IRQ_BUFFER
		{
		DWORD	dwADO_underrun;
		DWORD	dwADI_overrun;
		DWORD	dwADO_wr_addr_over;
		DWORD	dwADI_rd_addr_over;
		DWORD	dwPIO_mode_not_support;
		DWORD	dwTimming_error;
		DWORD	dwRequestPostmessage;
		DWORD	dwDI_CT_interrupt;
		DWORD	dwIrqst;
		DWORD	dwTheCallCount;
		};

	// �g���K�E�����O�o�b�t�@
	struct TXBUFSETUP2
		{
		DWORD	dwClockScall;
		// AIO TRIG VALUE
		DWORD	dwStartTrigDelay;
		DWORD	dwStartTrigLevel1;
		DWORD	dwStartTrigLevel2;
		DWORD	dwStopTrigDelay;
		DWORD	dwStopTrigLevel1;
		DWORD	dwStopTrigLevel2;
		// DIO TRIG VALUE
		DWORD	dwStartMask;
		DWORD	dwStartDiPattern;
		DWORD	dwStartTrigSourceDI_ch;
		DWORD	dwStopMask;
		DWORD	dwStopDiPattern;
		DWORD	dwStopTrigSourceDI_ch;
		// TRIG MODE
		DWORD	dwTrigStopMode;
		DWORD	dwTrigStartMode;

		// TRIG MISC
		DWORD	dwPreTrigger;
		DWORD	dwIamSlave;
		DWORD	dwAnalogTrigSourceStart;
		DWORD	dwDigitalTrigSourceStart;
		DWORD	dwAnalogTrigSourceStop;
		DWORD	dwDigitalTrigSourceStop;
		DWORD	dwIntrruptMode;
		DWORD	dwDeadTime;
		// STOP COUNTER
		DWORD	dwStopCounterValue;
		// �`�����l���V�[�N�G���X
		DWORD	dwMuxSeqenceAuto;
		// AO,DO�������O�o�b�t�@�o�R�ɂ��邩�H
		DWORD	dwAoHspBufferd;
		DWORD	dwDoHspBufferd;

		// ADX14�ɂ����āAADI�������O�o�b�t�@�o�R�ɂ��邩
		DWORD	dwAdiBufferOn;

		// �J�E���^�������O�o�b�t�@�o�R�ɂ��邩
		BYTE	bConnectBuffer;	
		// �g���K�C�l�[�u��
		BOOL	bTrigEnable;
		};

	// IO�ݒ�
	struct IOGEOSETUP2
		{
		DWORD	dwAo3Mode;
		DWORD	dwAo2Mode;
		DWORD	dwAo1Mode;
		DWORD	dwAo0Mode;
		DWORD	dwInputShort;
		DWORD	dwCOB;
		DWORD	dwFilterEnable;
		DWORD	dwDifferential;
		DWORD	dwMux;
		DWORD	dwAI_Range;	// dwAI_UnipoleBipole = BIT2(1Unipole:0Bipole) / Range = BIT1:0
		DWORD	dwAO_Range;	// dwAO_UnipoleBipole = BIT2(1Unipole:0Bipole) / Range = BIT1:0
		DWORD	dwChatCan;
		DWORD	dwNoiseShaper;
		};

	// DIO�G���R�[�_�J�E���^�E���g���J�E���^�E�X�g���[�u�EPWM�}�b�s���O�E���荞��
	struct	TDIO_MISC
		{
		// Latch strobe source
		DWORD	dwStrobeInternal;
		// PWM_MISC
		DWORD	dwSetFreq;
		DWORD	dwLinkPwmToDO;
		DWORD	dwDinIntMode16;
		DWORD	dwDinIntMode17;
		DWORD	dwDinIntMode18;
		DWORD	dwDinIntMode19;
		DWORD	dwDinIntMode20;
		DWORD	dwDinIntMode21;
		DWORD	dwDinIntMode22;
		DWORD	dwDinIntMode23;
		DWORD	dwDinIntMode24;
		DWORD	dwDinIntMode25;
		DWORD	dwDinIntMode26;
		DWORD	dwDinIntMode27;
		DWORD	dwDinIntMode28;
		DWORD	dwDinIntMode29;
		DWORD	dwDinIntMode30;
		DWORD	dwDinIntMode31;
		// 4Phase ABZY-Encoder counter
		DWORD	dwCounterMode_A;
		DWORD	dwCounterMode_B;
		DWORD	dwCounterMode_C;
		DWORD	dwCounterMode_D;
		DWORD	dwLatchMode_A;
		DWORD	dwLatchMode_B;
		DWORD	dwLatchMode_C;
		DWORD	dwLatchMode_D;
		DWORD	dwZ_CENTER_A;
		DWORD	dwZ_CENTER_B;
		DWORD	dwZ_CENTER_C;
		DWORD	dwZ_CENTER_D;
		DWORD	dwSoftwareClear_A;
		DWORD	dwSoftwareClear_B;
		DWORD	dwSoftwareClear_C;
		DWORD	dwSoftwareClear_D;
		// Counter compare output (Counter DO link)
		DWORD	dwCompareMode;
		DWORD	dwLinkCounterToDO_A;
		DWORD	dwLinkCounterToDO_B;
		DWORD	dwLinkCounterToDO_C;
		DWORD	dwLinkCounterToDO_D;
		// Counter compare interrupt (Counter IRQ link)
		DWORD	dwCounterIntMode_A;
		DWORD	dwCounterIntMode_B;
		DWORD	dwCounterIntMode_C;
		DWORD	dwCounterIntMode_D;
		// Counter compare reference
		DWORD	dwReferenceLow_A;
		DWORD	dwReferenceLow_B;
		DWORD	dwReferenceLow_C;
		DWORD	dwReferenceLow_D;
		DWORD	dwReferenceHigh_A;
		DWORD	dwReferenceHigh_B;
		DWORD	dwReferenceHigh_C;
		DWORD	dwReferenceHigh_D;
		// Freqency counter
		DWORD	dwSetGate;
		};

	// AI,DI,AO,DO,COUNTER,���x�̃|�[�����O����ѕ����萔�ϊ��𓝍�
	struct	TADIO2
		{
		DWORD	dwAi0;
		DWORD	dwAi1;
		DWORD	dwAi2;
		DWORD	dwAi3;
		DWORD	dwAi4;
		DWORD	dwAi5;
		DWORD	dwAi6;
		DWORD	dwAi7;
		DWORD	dwAo0;
		DWORD	dwAo1;
		DWORD	dwAo2;
		DWORD	dwAo3;
		DWORD	dwAo4;
		DWORD	dwAo5;
		DWORD	dwAo6;
		DWORD	dwAo7;
		DWORD	dwDOS;
		DWORD	dwDI;
		DWORD	dwDI_Latch;
		double	dAi0;
		double	dAi1;
		double	dAi2;
		double	dAi3;
		double	dAi4;
		double	dAi5;
		double	dAi6;
		double	dAi7;
		double	dAo0;
		double	dAo1;
		double	dAo2;
		double	dAo3;
		double	dAo4;
		double	dAo5;
		double	dAo6;
		double	dAo7;
		DWORD	dwCounterA;
		DWORD	dwCounterB;
		DWORD	dwCounterC;
		DWORD	dwCounterD;
		DWORD	dwLatchA;
		DWORD	dwLatchB;
		DWORD	dwLatchC;
		DWORD	dwLatchD;
		DWORD	dwFreqLatch_A;
		DWORD	dwFreqLatch_B;
		DWORD	dwFreqLatch_C;
		DWORD	dwFreqLatch_D;
		double	dTemp;
		DWORD	dwMode;
		};

	struct	TADIO2_P
		{
		DWORD	dwAi8;
		DWORD	dwAi9;
		DWORD	dwAi10;
		DWORD	dwAi11;
		DWORD	dwAi12;
		DWORD	dwAi13;
		DWORD	dwAi14;
		DWORD	dwAi15;
		double	dAi8;
		double	dAi9;
		double	dAi10;
		double	dAi11;
		double	dAi12;
		double	dAi13;
		double	dAi14;
		double	dAi15;
		};

	// PWM�p�����^
	struct TConfigPWM
		{
		DWORD	dwCycleMax0s;
		DWORD	dwCycleMax1s;
		DWORD	dwCycleMax2s;
		DWORD	dwCycleMax3s;
		DWORD	dwCycleMax4s;
		DWORD	dwCycleMax5s;
		DWORD	dwCycleMax6s;
		DWORD	dwCycleMax7s;
		DWORD	dwCycleMax8s;
		DWORD	dwCycleMax9s;
		DWORD	dwCycleMax10s;
		DWORD	dwCycleMax11s;
		DWORD	dwCycleMax12s;
		DWORD	dwCycleMax13s;
		DWORD	dwCycleMax14s;
		DWORD	dwCycleMax15s;
		DWORD	dwPwmPhase0s;
		DWORD	dwPwmPhase1s;
		DWORD	dwPwmPhase2s;
		DWORD	dwPwmPhase3s;
		DWORD	dwPwmPhase4s;
		DWORD	dwPwmPhase5s;
		DWORD	dwPwmPhase6s;
		DWORD	dwPwmPhase7s;
		DWORD	dwPwmPhase8s;
		DWORD	dwPwmPhase9s;
		DWORD	dwPwmPhase10s;
		DWORD	dwPwmPhase11s;
		DWORD	dwPwmPhase12s;
		DWORD	dwPwmPhase13s;
		DWORD	dwPwmPhase14s;
		DWORD	dwPwmPhase15s;
		DWORD	dwStart;
		};

	// PWM�f�B�e�B�[
	struct TSetupPWM
		{
		DWORD	dwPwm0Value;
		DWORD	dwPwm1Value;
		DWORD	dwPwm2Value;
		DWORD	dwPwm3Value;
		DWORD	dwPwm4Value;
		DWORD	dwPwm5Value;
		DWORD	dwPwm6Value;
		DWORD	dwPwm7Value;
		DWORD	dwPwm8Value;
		DWORD	dwPwm9Value;
		DWORD	dwPwm10Value;
		DWORD	dwPwm11Value;
		DWORD	dwPwm12Value;
		DWORD	dwPwm13Value;
		DWORD	dwPwm14Value;
		DWORD	dwPwm15Value;
		};

	// �X�e�[�^�X
	struct TStatusPack2
		{
		DWORD	dwBurstMax;
		DWORD	dwADO_underrun;
		DWORD	dwADI_overrun;
		DWORD	dwWriteAddress;
		DWORD	dwBusmasterOn;
		DWORD	dwDaqInit;
		DWORD	dwDaqEnable;
		DWORD	dwTrigSens2;
		DWORD	dwTrigSens1;
		DWORD	dwTrigSens0;
		DWORD	dwTrigSeq;
		};

	// �f�o�C�X���
	struct	SAYA_DEVICE_INFO
		{
		DWORD	dwDeviceType;
		DWORD	dwBufferSizeOfByte;
		DWORD	dwBufferSizeOfDWORD;
		int	iDIO_TRIG_SOURCE_MAX;
		int	iAIO_TRIG_SOURCE_MAX;
		int	iTRIG_MODE_MAX;
		DWORD	dwSAMPLE_FAST;
		DWORD	dwSAMPLE_SLOW;
		int	iPRE_TRIG_MAX;
		};

	struct	SAYA_DEVICE_INFO_EX
		{
		DWORD	dwDeviceType;
		DWORD	dwBufferSizeOfByte;
		DWORD	dwBufferSizeOfDWORD;
		int	iDIO_TRIG_SOURCE_MAX;
		int	iAIO_TRIG_SOURCE_MAX;
		int	iTRIG_MODE_MAX;
		DWORD	dwSAMPLE_FAST;
		DWORD	dwSAMPLE_SLOW;
		int	iPRE_TRIG_MAX;
		DWORD	dwSAMPLE_SLOW2;
		DWORD	dwTempSensor;
		DWORD	dwOnboardCAL;
		DWORD	dwCounterToRingbuf;
		DWORD	dwClockOut;
		DWORD	dwPreTrigSize;
		DWORD	dwBankSize;
		DWORD	dwBankSizeUnit;
		DWORD	dwClockIn;
		DWORD	dwChseqMax;
		DWORD	dwChseqMin;
		DWORD	dwCyclicTrig;
		DWORD	dwChseqDfType;
		DWORD	dwCoreAirange;
		DWORD	dwDoMap;
		DWORD	dwAiMap;
		DWORD	dwDiMap;
		DWORD	dwAoMap;
		DWORD	dwCounterMap;
		DWORD	dwFreqCounterMap;
		DWORD	dwPwmMap;
		DWORD	dwProgramableScp;
		DWORD	dwAdaptiveFreqUnit;
		double	dAoLsbLevel;
		double	dAoLsbLevelEx;
		double	dAoLsbOffset;
		double	dAoLsbOffsetEx;
		DWORD	dwDI[16];
		DWORD	dwDO[16];
		DWORD	dwBufferSizeOfBytesr;
		DWORD	dwBufferSizeOfDWORDsr;
		DWORD	dwPreWriteSizeOfByte;
		DWORD	dwPreWriteSizeOfDWORD;
		DWORD	dwAdcStyle;
		DWORD	dwAdoBufMode;
		};

	// �����[�gIO�p��IP�A�h���X�E�|�[�g�ԍ�
	struct ADR_IP_CONF
		{
		int		iIP1[MAX_MFIO];
		int		iIP2[MAX_MFIO];
		int		iIP3[MAX_MFIO];
		int		iIP4[MAX_MFIO];
		int		iPORT[MAX_MFIO];
		BOOL		bEnable[MAX_MFIO];
		};

	// ������IO�{�[�h�̐ݒ�A�V�O�i���R���f�B�V�����ɑΉ�
	struct SCP_SETUP2
		{
		// �C�l�[�u��
		BOOL	bMultifunctionIO_Enable[MAX_MFIO];
		// X-API�������̂��߂̍\����
		TXBUFSETUP2	sTXBUFSETUP[MAX_MFIO];
		IOGEOSETUP2	sIOGEOSETUP[MAX_MFIO];
		TDIO_MISC	sTDIO_MISC[MAX_MFIO];
		TConfigPWM	sTConfigPWM[MAX_MFIO];
		TSetupPWM	sTSetupPWM[MAX_MFIO];
		// �V�O�i���R���f�B�V�������W�X�^�ւ̐ݒ�
		DWORD		dwLDO1[MAX_MFIO];
		DWORD		dwLDO2[MAX_MFIO];
		// �Z���T�[���
		DWORD		dwSensorMode[MAX_MFIO][MAX_AI_CH];
		// �[���X�p��
		double		doZeroPos[MAX_MFIO][MAX_AI_CH];
		double		doSpanPos[MAX_MFIO][MAX_AI_CH];
		double		doZero_Coefficient[MAX_MFIO][MAX_AI_CH];
		double		doSpan_Coefficient[MAX_MFIO][MAX_AI_CH];
		// �X�P�[�����O
		BOOL		bScalling[MAX_MFIO][MAX_AI_CH];
		double		dScallingRatio[MAX_MFIO][MAX_AI_CH];
		double		dOutTopScall[MAX_MFIO][MAX_AI_CH];
		double		dOutBottomScall[MAX_MFIO][MAX_AI_CH];
		double		dInTopScall[MAX_MFIO][MAX_AI_CH];
		double		dInBottomScall[MAX_MFIO][MAX_AI_CH];
		// �A���[��
		DWORD		bAlarmMode[MAX_MFIO][MAX_AI_CH];
		double		dAlarmUpper[MAX_MFIO][MAX_AI_CH];
		double		dAlarmLower[MAX_MFIO][MAX_AI_CH];
		};

	struct SCP_SETUP_CS2	// C#�p
		{
		BOOL		*bMultifunctionIO_Enable;
		TXBUFSETUP2	*sTXBUFSETUP;
		IOGEOSETUP2	*sIOGEOSETUP;
		TDIO_MISC	*sTDIO_MISC;
		TConfigPWM	*sTConfigPWM;
		TSetupPWM	*sTSetupPWM;
		DWORD		*dwLDO1;
		DWORD		*dwLDO2;
		DWORD		*dwSensorMode;
		double		*doZeroPos;
		double		*doSpanPos;
		double		*doZero_Coefficient;
		double		*doSpan_Coefficient;
		BOOL		*bScalling;
		double		*dScallingRatio;
		double		*dOutTopScall;
		double		*dOutBottomScall;
		double		*dInTopScall;
		double		*dInBottomScall;
		DWORD		*bAlarmMode;
		double		*dAlarmUpper;
		double		*dAlarmLower;
		};

	// �P��`�����l���̃V�O�i���R���f�B�V�����ݒ�l�̎���̂��߂̍\���́i�Z���W���܂܂��j
	struct SCP_SETUP_AICH
		{
		DWORD		dwSensorMode[MAX_AI_CH];
		DWORD		dwLDO[MAX_AI_CH];
		double		doZeroPos[MAX_AI_CH];
		double		doSpanPos[MAX_AI_CH];
		BOOL		bScalling[MAX_AI_CH];
		double		dOutTopScall[MAX_AI_CH];
		double		dOutBottomScall[MAX_AI_CH];
		double		dInTopScall[MAX_AI_CH];
		double		dInBottomScall[MAX_AI_CH];
		DWORD		bAlarmMode[MAX_AI_CH];
		double		dAlarmUpper[MAX_AI_CH];
		double		dAlarmLower[MAX_AI_CH];
		};

	// �P��`�����l���̃V�O�i���R���f�B�V�����ݒ�l�̎���̂��߂̍\���́i�Z���W���܂ށj
	struct SCP_SETUP_AIALL
		{
		DWORD		dwSensorMode[MAX_AI_CH];
		DWORD		dwLDO[MAX_AI_CH];
		double		doZeroPos[MAX_AI_CH];
		double		doSpanPos[MAX_AI_CH];
		double		doZero_Coefficient[MAX_AI_CH];
		double		doSpan_Coefficient[MAX_AI_CH];
		BOOL		bScalling[MAX_AI_CH];
		double		dOutTopScall[MAX_AI_CH];
		double		dOutBottomScall[MAX_AI_CH];
		double		dInTopScall[MAX_AI_CH];
		double		dInBottomScall[MAX_AI_CH];
		DWORD		bAlarmMode[MAX_AI_CH];
		double		dAlarmUpper[MAX_AI_CH];
		double		dAlarmLower[MAX_AI_CH];
		};

	// �t�@�C���ۑ��@�\�Ȃǂ̊g���\����C/C++/VC++/VC#�p
	struct ADIOX_EXTENTION2
		{
		// AI
		char		lpcAdiFileName[256];
		char		lpcDmyFileName[256];
		BOOL		bDoubleSave;
		DWORD		dwADI_style;
		// AO
		int		iAO_Gain0;
		int		iAO_Gain1;
		int		iAO_Offset0;
		int		iAO_Offset1;
		int		iAO_SamplePerCycle0;	// 1�T�C�N��������̃T���v����1�X�^�[�g
		int		iAO_SamplePerCycle1;	// 1�T�C�N��������̃T���v����1�X�^�[�g
		DWORD		dwADO_style0;
		DWORD		dwADO_style1;
		char		lpcAdoFileName[256];
		// RESERVED
		DWORD		dwReserverd[16];
		};

	struct ADIOX_EXTENTION2B
		{
		// AI
		char		*lpcAdiFileName;
		char		*lpcDmyFileName;
		BOOL		bDoubleSave;
		DWORD		dwADI_style;
		// AO
		int		iAO_Gain0;
		int		iAO_Gain1;
		int		iAO_Offset0;
		int		iAO_Offset1;
		int		iAO_SamplePerCycle0;	// 1�T�C�N��������̃T���v����1�X�^�[�g
		int		iAO_SamplePerCycle1;	// 1�T�C�N��������̃T���v����1�X�^�[�g
		DWORD		dwADO_style0;
		DWORD		dwADO_style1;
		char		*lpcAdoFileName;
		// RESERVED
		DWORD		dwReserverd[16];
		};

	// ��������i�[����\����C/C++/VC++/VC#�p
	struct CharPayloadC
		{
		char	lpcInitialDir[256];
		char	lpcConfigFileName[256];
		};

	// ��������i�[����\����C/C++/VC++/VB�p
	struct CharPayloadB
		{
		char	*lpcInitialDir;
		char	*lpcConfigFileName;
		};

	// �t�@�C���ۑ��w�b�_
	struct LOG_FRONTEND
		{
		DWORD	dwHeaderCode;  		// 0x41594154
		DWORD	dwDeviceName;  		// �@��R�[�h
		DWORD	dwBuffaSize;  		// �Z�J���_��PC�����O�o�b�t�@�T�C�Y
		double	dClockScall;  		// �T���v�����O���[�g(Hz)
		BYTE	bBitScall;  		// �ʎq���r�b�g��
		BYTE	bAI_ChannelScall; 	// AI�`�����l����
		BYTE	bDI_ChannelScall; 	// DI�`�����l���� �������g�p
		BYTE	DataType;  		// DATA�^��ʔ��f DWORD=0,double=1
		DWORD	dwGetYear;  		// �v���J�n�̔N
		DWORD	dwGetMonth;  		// �v���J�n�̌�
		DWORD	dwGetDay;  		// �v���J�n�̓�
		DWORD	dwGetHour;  		// �v���J�n�̎�
		DWORD	dwGetMinute;  		// �v���J�n�̕�
		DWORD	dwGetSecond;  		// �v���J�n�̕b
		DWORD	dwGetMilliseconds; 	// �v���J�n�̃~���b
		DWORD	dwInrange;	 	// ���̓����W(ADX85��AI�����W���̂܂܁AADX14��1�Ń��j�|�[���^0�Ńo�C�|�[��)
		DWORD	dwReserved1;	 	// 
		DWORD	dwReserved2;	 	// 
		};

	/*==============================================================================================*/
	/* ���������[�gIO��EEPROM�ݒ�									*/
	/*==============================================================================================*/

	// IP�A�h���X�ƃ|�[�g
	struct TCP_IP_SETTING
		{
		int Port;
		int IP1;
		int IP2;
		int IP3;
		int IP4;
		};

	// IP�A�h���X�ƃ|�[�g�A�Q�[�g�E�F�C�A�T�u�l�b�g
	struct NETWORK_SETUP
		{
		WORD	Port;
		BYTE	IpAddress0;
		BYTE	IpAddress1;
		BYTE	IpAddress2;
		BYTE	IpAddress3;
		BYTE	GatewayAddress0;
		BYTE	GatewayAddress1;
		BYTE	GatewayAddress2;
		BYTE	GatewayAddress3;
		BYTE	SubnetMask0;
		BYTE	SubnetMask1;
		BYTE	SubnetMask2;
		BYTE	SubnetMask3;
		};

	// MAC�A�h���X
	struct MAC_ADDRESS
		{
		BYTE	MacAddress0;
		BYTE	MacAddress1;
		BYTE	MacAddress2;
		BYTE	MacAddress3;
		BYTE	MacAddress4;
		BYTE	MacAddress5;
		};


/*======================================================================================================*/
/*	�����̒�`											*/
/*======================================================================================================*/
	// �����O�o�b�t�@���荞�݃��[�h
	#define	NOT_INT		0
	#define	DMA_INT		1

	// �g���K���[�h5-10�Ԃ�11-12�Ԃ���n�[�h�E�F�A�����Ő��������
	#define	RESET		0
	#define	BURST		1
	#define	DI_POSEDGE	2
	#define	DI_NEGEDGE	3
	#define	DI_PATTERN	4
	#define	AI_POSEDGE	5
	#define	AI_NEGEDGE	6
	#define	AI_DUALEDGE	7
	#define	AI_INTERENAL	8
	#define	AI_EXTERNAL	9
	#define	INVALID		10
	#define	AI_LEVEL	11
	#define	AI_AREA		12

	// AO0-3���[�h
	#define	NO_LINK		0
	#define	LIVE_CTC	1
	#define	LATCH_CTC	2

	// DI���荞�݃��[�h
	#define	NO_INT		0
	#define	POSEDGE_INT	1
	#define	NEGEDGE_INT	2
	#define	DUALEDGE_INT	3

	// �J�E���^�[���b�`���[�h
	#define	SOFT		0
	#define	Z_PHASE		1
	#define	DI_SEL		2

	// �����O�o�b�t�@�[�X�e�[�^�X(���荞�݂͔������Ȃ�)
	#define	NO_TRANSFER	0
	#define	EMPTY_BUFFER	1
	#define	FULL_BUFFER	2
	#define	INVALID_BUFFER	3
	#define	OVERRUN_BUFFER	4
	#define	UNDERRUN_BUFFER	4
	#define	ERR0_BUFFER	5
	#define	ERR1_BUFFER	6
	#define	ERR2_BUFFER	7

	// ���荞�݃|�X�g���b�Z�[�W�̃��b�Z�[�W�R�[�h
	#define	ERROR_DETECT		0x1
	#define	DMA_BUFFER_HALF_EMPTY	0x2
	#define	DMA_BUFFER_FULL_EMPTY	0x4
	#define	DI_EVENT		0x8
	#define	COUNTER_EVENT		0x10
	#define	DMA_SIGNAL		0x20
	#define	ADO_FIFO_HALF_EMPTY	0x40
	#define	BANK_CHANGE		0x80

	// �f�o�C�X�̎��
	#define	ADX85_PCI		0
	#define	ADX42_LAN		4
	#define	ADX14_PCI		5
	#define	ADX52_LAN		6
	#define	MFIO_NOT_FIND		0xFFFFFFFF

	// ADI MODE
	#define	NO_SAVE		0
	#define	DIRECT_FILE	1

	// ADO MODE
	#define	ADX_Sin		0
	#define	ADX_Cos		1
	#define	ADX_Exp		2
	#define	ADX_Sqrt	3
	#define	ADX_Triangle	4
	#define	ADX_Lamp	5
	#define	ADX_Square	6
	#define	ADX_DC0		7
	#define	ADX_DC1		8
	#define	ADX_File	9
	#define	ADX_LargeStep	10

	// �Z���T�[���
	#define NOT_USE		0
	#define CA_K		1
	#define CA_J		2
	#define CA_E		3
	#define CA_T		4
	#define CA_R		5
	#define CA_S		6
	#define CA_N		7
	#define CA_B		8
	#define PT100		9
	#define JPT100		10
	#define VBP_10mV	11
	#define VBP_100mV	12
	#define VBP_1V		13
	#define VBP_1_25V	14
	#define VBP_2_5V	15
	#define VBP_5V		16
	#define VBP_10V		17
	#define VUP_1_25V	18
	#define VUP_2_5V	19
	#define VUP_5V		20
	#define VUP_10V		21
	#define I_4_20		22
	#define I_4_20EX	23
	#define VBP_30mV	24
	#define VBP_3V		25
	#define I_4_20EX2	26
	#define D16BIT		27	// 0-65535�̃f�W�^���l�̂���
	#define RION_DC80	28	// ���I�������v�U���v
	#define RION_DC90	29	// ���I�������v�U���v
	#define RION_DC100	30	// ���I�������v�U���v
	#define RION_DC110	31	// ���I�������v�U���v
	#define RION_DC120	32	// ���I�������v�U���v
	#define RION_DC130	33	// ���I�������v�U���v
	#define DUST10000	34
	#define DUST1000	35
	#define	MT321			36
	#define	AKW4802C_100OHM		37
	#define	AKW4803_4804C_10OHM	38
	#define	AKW4808C_10OHM		39

	#define EC_4X		40
	#define	EC_4XZ		41
	#define	EC_2X		42
	#define	EC_2XZ		43
	#define	EC_1X		44
	#define	EC_1XZ		45
	#define	UPC		46
	#define	UPC_Z		47
	#define	WIND		48
	#define	INFRS_FA	49
	#define	INFRS_FB	50
	#define	INFRS_TA	51
	#define	INFRS_TB	52
	#define	INFRS_CA	53
	#define	INFRS_CB	54
	#define	INFRS_DIF	55
	#define	INFRS_DIF2	56
	#define	INFRS_DIF3	57
	#define	INFRS_FB2	58
	#define	INFRS_FB3	59

	// Ifrs
	#define VIB		100
	#define	SPL		101
	#define	APL		102

/*======================================================================================================*/
/*	�G�N�X�|�[�g���Ă���֐�									*/
/*======================================================================================================*/
	extern "C" BOOL __stdcall bADioxOpen2 ( HWND hWnd,BYTE bHwintr,BYTE bCardID );
	extern "C" BOOL __stdcall bADioxClose ( BYTE bCardID );
	extern "C" BOOL __stdcall bADioxInterruptStart ( BOOL bStart,BYTE bCardID  );
	extern "C" BOOL __stdcall bADioxAnalogConfigration2 ( struct IOGEOSETUP2 *sIoGeoSetup,BYTE bCardID );	//***
	extern "C" BOOL __stdcall bADioxDioMisc2 ( struct TDIO_MISC *lpsDIO_MISC,BYTE bCardID  );		//***
	extern "C" BOOL __stdcall bADioxADIO2 ( struct TADIO2 * lpsTADio,BYTE bCardID  );
	extern "C" BOOL __stdcall bADioxADIO2P ( struct TADIO2 * sTADio,struct TADIO2_P * sTADiop,BYTE bCardID );
	extern "C" void __stdcall vADioxDeviceInfoEx ( struct SAYA_DEVICE_INFO_EX * lpsSAYA_DEVICE_INFO ,BYTE bCardID );
	extern "C"  BOOL __stdcall bADioxDmaReadEX3 ( LPDWORD lpdwBuffa,double  * lpdAiBuffa,TStatusPack2 *lpsTStatus,BYTE bCardID );
	extern "C" BOOL __stdcall bADioxConfigPWM2 ( struct TConfigPWM *lpsTConfigPWM,BYTE bCardID );		//***
	extern "C" BOOL __stdcall bADioxSetupPWM2 ( struct TSetupPWM *lpsTSetupPWM,BYTE bCardID  );		//***
	extern "C" BOOL __stdcall bADioxStatus2 ( struct TStatusPack2 * sTStatus,BYTE bCardID  );
	extern "C" void __stdcall vADioxDeviceInfo ( struct SAYA_DEVICE_INFO * lpsSAYA_DEVICE_INFO ,BYTE bCardID );
	extern "C" double __stdcall dADioxCalcFreq ( DWORD dwSampling,BYTE bCardID );
	extern "C" BOOL __stdcall bADX14input_type ( BYTE bCardID );
	extern "C" BOOL __stdcall bADioxChannelAIw2 ( struct IOGEOSETUP2 *lpsIoGeoSetup,LPDWORD lpwData,int iInterval,BYTE bCardID );		//***
	extern "C" int __stdcall iADioxCheckBits ( DWORD dwDATA,BYTE bChannel );
	extern "C" BOOL __stdcall bADioxDI ( LPDWORD lpdwDI,BYTE bCardID );
	extern "C" BOOL __stdcall bADioxDO ( DWORD dwDO,BYTE bCardID );
	extern "C" BOOL __stdcall bADioxRingBufferMode ( DWORD dwRingBufferMode,BYTE bCardID );
	extern "C"  BOOL __stdcall bADioxDmaReadEX2 ( LPDWORD lpdwBuffa,TStatusPack2 *lpsTStatus,BYTE bCardID );
	extern "C"  BOOL __stdcall bADioxReadScpBuf2 ( LPDWORD lpdwBuffa,double * lpdAiBuffa,LPDWORD lpdwAlarmBuffa,LPBOOL bBurnOut,TStatusPack2 *lpsTStatus,BYTE bCardID );
	extern "C" BOOL __stdcall bADioxWriteMemoryEX ( BYTE bAccessMode,LPDWORD lpdwBuffa,BYTE bCardID );
	extern "C" BOOL __stdcall bADioxScpSetup2 ( struct SCP_SETUP_AICH *lpsScpSetupAich,BYTE bCardID );	//***
	extern "C" BOOL __stdcall bADioxScpSetupEX2 ( struct SCP_SETUP_AIALL *lpsScpSetupAich,BYTE bCardID );	//***
	extern "C" double __stdcall dADioxLinCoef ( DWORD dwANALOG, LPBOOL lpbBurnOut,LPDWORD lpdwAlarm,BYTE bCardID,BYTE bChannel );
	extern "C" BOOL __stdcall bADioxZeroAdj ( BYTE bCardID,BYTE bChannel );
	extern "C" BOOL __stdcall bADioxSpanAdj ( BYTE bCardID,BYTE bChannel );
	extern "C" void __stdcall vADioxFreeAdj ( BYTE bCardID,BYTE bChannel );
	extern "C" BOOL __stdcall bADioxAutoZero ( BYTE bCardID );
	extern "C" void __stdcall vSetupTcpIp ( int IP1,int IP2,int IP3,int IP4,int Port,BYTE bCardID );
	extern "C" BOOL __stdcall bADioxWriteIp2 ( struct ADR_IP_CONF *lpsADR_IP_CONF  );			//***
	extern "C" BOOL __stdcall bADioxReadIp ( struct ADR_IP_CONF *lpsADR_IP_CONF );
	extern "C" BOOL __stdcall bADioxStopPoint2 ( LPDWORD lpdwStopAddress,struct TStatusPack2 *lpsTStatus,BYTE bCardID );
	extern "C" DWORD __stdcall dwADioxSrcStatus ( DWORD dwCommand );
	extern "C" DWORD __stdcall dwADioxDebugIf ( DWORD dwDeviceNumber,DWORD dwCH );
	extern "C" DWORD __stdcall dwADioxNetError ( LPDWORD lpdwWriteRetry,LPDWORD lpdwReadRetry,LPDWORD lpdwReadFlameError,LPDWORD lpdwReadAddressError );
	extern "C" BOOL __stdcall bADioxWaveOut ( LPCTSTR lpszSoundNam );
	extern "C" BOOL __stdcall bADioxMessageCount ( LPDWORD lpdwMessageCount,BYTE bCardID );
	extern "C" BOOL __stdcall bADioxFft_Config ( int iPoint,int iWindow );
	extern "C" double __stdcall dADioxFft_PreScall ( DWORD dwGetSample,BOOL bCOB );
	extern "C" BOOL __stdcall bADioxFft_Analyze ( double * dFreqOut,double * dDataIn );
	extern "C" BOOL __stdcall bADioxAutoCal2 ( struct IOGEOSETUP2 *lpsIOGEOSETUP,struct TXBUFSETUP2 *lpsTBUFSETUP,BYTE bCardID );			// NEW
	extern "C" DWORD __stdcall dwADioxAutoCal_Status1 ( LPBOOL lpbInAutoCal );
	extern "C" BOOL __stdcall bADioxDefaultInit ( TXBUFSETUP2 * lpsTBUFSETUP,IOGEOSETUP2 * lpsIOGEOSETUP,TDIO_MISC * lpsTDIO_MISC,TConfigPWM * lpsTConfigPWM,TSetupPWM * lpsTSetupPWM,TADIO2 * lpsTADIO,BYTE bCARD_ID );
	extern "C" double __stdcall dADiox_Measurement_Fs_detection ( BYTE bCardID );
	extern "C" double __stdcall dADioxAdxFullScall ( BYTE bCardID,BOOL bFiles,DWORD dwFileRange,DWORD dwFileHardwareType );
	extern "C" BYTE __stdcall bADioxAdxMaxCH ( BYTE bCardID );
	extern "C" BYTE __stdcall bADioxAdxMaxCH2 ( BYTE bCardID );
	extern "C" void __stdcall vADioxIrqEmuration ( BOOL bEnableIrqEmuration );
	extern "C"  BOOL __stdcall bADioxScpRetry ( HWND hWnd,BYTE bWintr,BYTE bCardID );
	extern "C" void __stdcall vADioxErrorMessageStop ( BOOL bStop );
	extern "C"  BOOL __stdcall bADioxGetBootStatus ( BYTE bCardID, LPBYTE lpbErrorType );
	extern "C" double __stdcall dADioxAdxTriggerPosition ( double dPercent,BYTE bCardID );
	extern "C" BOOL __stdcall bADioxClockMode ( BOOL bClockOutEnable,BYTE bCardID );
	extern "C" BOOL __stdcall bADioxCyclicTrigger ( BOOL bCyclicTrigger,DWORD dwSetupBufferSize,BYTE bCardID );
	extern "C" BOOL __stdcall bADioxCyclicRestart ( BYTE bCardID );
	extern "C" BOOL __stdcall bADioxWindSpeed ( double * lpdCounter,BYTE bCardID );
	extern "C" BOOL __stdcall bADioxFastPolling ( DWORD dwClock,BYTE bCardID );

	extern "C"  BOOL __stdcall bADioxDR_RB ( LPDWORD lpdwBuffa,BYTE bCardID );
	extern "C"  double  __stdcall dADioxLinCoefItl ( DWORD dwANALOG, LPBOOL lpbBurnOut,LPDWORD lpdwAlarm,BYTE bCardID,BYTE bChannel );
	extern "C"  BOOL __stdcall bADioxScpRetryRB ( HWND hWnd,BYTE bWintr,BYTE bCardID );
	extern "C" BOOL __stdcall bADioxInterruptStatus ( struct IRQ_BUFFER *lpIrqbuf,BYTE bCardID );
	extern "C" void __stdcall vADioxNetworkQuarity( LPDWORD p_error_rx_par,LPDWORD p_error_tx_par,LPDWORD p_error_tx_hdr,LPDWORD p_error_tx_size,LPDWORD p_error_rx_hdr,LPDWORD p_complete );
	extern "C" BOOL  __stdcall  bADioxEEPROM_INIT ( MAC_ADDRESS sMAC_ADDRESS );
	extern "C" BOOL  __stdcall  bADioxEEPROM_READ ( MAC_ADDRESS *psMAC_ADDRESS , NETWORK_SETUP *psNS );
	extern "C" BOOL  __stdcall  bADioxEEPROM_WRITE ( TCP_IP_SETTING sTCP_IP_SETTING , NETWORK_SETUP sNS );
	extern "C" BOOL __stdcall bADioxReadWriteEX ( BYTE bAccessMode,LPDWORD lpdwBuffa,LPDWORD lpdwReadBuf,double * lpdAiBuffa,BYTE bCardID, LPBOOL lpbStopNow,TStatusPack2 *lpsTStatus );
	extern "C" BOOL __stdcall bADioxRestartIrqThread ( );
	extern "C" BOOL __stdcall bADioxPauseIrqThread ( );
	extern "C" void __stdcall vADioxGenerateViewAreaAI_EX ( BOOL bCyclicAdx2_42 );
	extern "C" DWORD __stdcall dwADioxGenerateViewAreaAI_FFT_size ( );
	extern "C" BOOL __stdcall bADioxEepromWrite ( BYTE bAddress , WORD wData , BYTE bCardID );
	extern "C" BOOL __stdcall bADioxEepromRead ( BYTE bAddress , LPWORD lpwData , BYTE bCardID );
	extern "C" BOOL __stdcall bADioxChannelAIw2_fx ( struct IOGEOSETUP2 *lpsIoGeoSetup,LPDWORD lpwData,int iInterval,BYTE bCardID );
	extern "C" void __stdcall vStartTimerIRQ ( UINT uiPacerDelay,HWND hmWnd );
	extern "C" void __stdcall vStopMtimerIRQ ( );
	extern "C" void __stdcall vReserMtimerIRQ ( );
	extern "C" BOOL __stdcall bADioxGetIrqThreadErr ( BYTE bCardID );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ������C/C++/VC++�p
	extern "C" void __stdcall vADioxScpCopy2    ( struct SCP_SETUP2 * lpsSCP_SETUP );
	extern "C" void __stdcall vADioxScpCopy_CS2 ( struct SCP_SETUP_CS2 * lpsSCP_SETUP_CS );
	extern "C" BOOL __stdcall bADioxScpDefault2    ( DWORD dwSensorMode,DWORD dwDeviceNumber, DWORD dwCH,struct SCP_SETUP2 * lpsSCP_SETUP );
	extern "C" BOOL __stdcall bADioxScpDefaultCS2  ( DWORD dwSensorMode,DWORD dwDeviceNumber, DWORD dwCH,struct SCP_SETUP_CS2 * lpsSCP_SETUP_CS );

	// �����VC#�p
	extern "C" BOOL __stdcall bADioxScpDefaultReconvert2 ( struct SCP_SETUP_CS2 sSCP_SETUP_CS, struct SCP_SETUP2 * lpsSCP_SETUP );

	// �����C/C++/VC++/VC#�p
	extern "C" BOOL __stdcall bADioxSetupSymmetryEngine2 ( struct TXBUFSETUP2 *lpsTBufSetup,struct ADIOX_EXTENTION2 *lpsADIOX_EXTENTION2 ,BYTE bCardID );	//***
	extern "C" BOOL __stdcall bADioxScpLoad2 ( HWND hWnd,BYTE bWintr,struct CharPayloadC *lpsCharPayload,BOOL bOpenDialog );
	extern "C" BOOL __stdcall bADioxScpStore2 ( struct CharPayloadC *lpsCharPayload,BOOL bOpenDialog );
	extern "C" BOOL __stdcall bADioxLoad_EX3													// NEW
		( 
		HWND hWnd,BYTE bWintr,BOOL bOpenDialog,struct SAYA_DEVICE_INFO *lpsSAYA_DEVICE_INFO,struct TXBUFSETUP2 *lpsTBUFSETUP, struct IOGEOSETUP2 *lpsIOGEOSETUP,struct TDIO_MISC *lpsTDIO_MISC,
		struct TADIO2 *lpsTADIO,struct TConfigPWM *lpsTConfigPWM,struct TSetupPWM *lpsTSetupPWM,struct SCP_SETUP_AIALL * lpsScpSetupAich,struct ADIOX_EXTENTION2 * lpsEXTENTION,
		DWORD dwRingbufferSize,BYTE CARD_ID,struct CharPayloadC *lpsCharPayload
		);
	extern "C"  BOOL __stdcall bADioxStore_EX3													// NEW
		( 
		BOOL bOpenDialog,struct TXBUFSETUP2 *lpsTBUFSETUP,struct IOGEOSETUP2 *sIOGEOSETUP,struct TDIO_MISC *lpsTDIO_MISC,struct TADIO2 *lpsTADIO,struct TConfigPWM *lpsTConfigPWM,
		struct TSetupPWM *lpsTSetupPWM,	struct SCP_SETUP_AIALL *lpsScpSetupAich,struct ADIOX_EXTENTION2 *lpsEXTENTION,BYTE CARD_ID,struct CharPayloadC *lpsCharPayload
		);

	// SAYA�\��F��ʂɂ͎g��Ȃ��ł�������//////////////////////////////////////////////////////////////////////////////////////////////////////////
	struct TRomWrite
		{
		DWORD	dwADDRESS;
		DWORD	dwDATA;
		};

	extern "C" BOOL  __stdcall  bADioxRomWrite ( struct TRomWrite sTRomWrite,BYTE bCardID );
	extern "C" BOOL  __stdcall  bADioxRomRead ( DWORD dwADDRESS,LPDWORD lpdwDATA,BYTE bCardID );
	extern "C" BOOL __stdcall bADioxAutoCal_GetTune1_B ( struct IOGEOSETUP2 sIOGEOSETUP,struct TXBUFSETUP2 sTBUFSETUP,BYTE bCardID );
	extern "C" BOOL __stdcall bADioxAutoCal_GetTune2_B ( struct IOGEOSETUP2 sIOGEOSETUP,struct TXBUFSETUP2 sTBUFSETUP,BYTE bCardID );
	extern "C" BOOL __stdcall bADioxAutoCal_GetTune3_B ( struct IOGEOSETUP2 sIOGEOSETUP,struct TXBUFSETUP2 sTBUFSETUP,BYTE bCardID );
	extern "C" void __stdcall vADioxAutoCal_Status3 ( short *lpsgOffset,double *lpdsGain,LPDWORD lpdwProcess );
	extern "C" BOOL __stdcall bADioxDR_R ( DWORD dwIndex,LPDWORD lpdwData,BYTE bCardID );
	extern "C" BOOL __stdcall bADioxDR_W ( DWORD dwIndex,DWORD dwData,DWORD dwCompareMask,BYTE bCardID  );
	extern "C" BOOL __stdcall bADioxADO_PID ( DWORD dwAo0,DWORD dwAo1,DWORD dwAo2,DWORD dwAo3,DWORD dwAoHsp,BYTE bCardID );
	extern "C" BOOL __stdcall  bADioxFastDaq ( BYTE ch , LPDWORD lpdwAdcLow, LPDWORD lpdwAdcHigh, double * lpdAdData, BYTE bCardID );
	extern "C" BOOL __stdcall bADioxBattsens ( int * lpiBatteryLevel, BYTE bCardID );
	extern "C" BOOL __stdcall bADioxSetPeakHold ( DWORD dwPeakholdCh, BYTE bCardID );
	extern "C"  BOOL __stdcall bADioxDmaReadEX2 ( LPDWORD lpdwBuffa,TStatusPack2 *lpsTStatus,BYTE bCardID );
	extern "C"  void __stdcall vADioxSetInf01Coef ( double dSetCoef );
	extern "C" BOOL __stdcall bADioxInfrsPack ( LPDWORD lpdwTmp,BYTE bCardID,BOOL bInit );

/*======================================================================================================*/
/* �g�`�`��A�V�X�g											*/
/*======================================================================================================*/

	//////////
	// ������
	//////////
	extern "C" BOOL __stdcall vADioxInitializeViewArea
		(
		DWORD	wDiViewerSize_Y		// �g�`�`��̈�T�C�YY(DI)
		);

	/////////////////
	// ���sAI,DWORD
	/////////////////
	extern "C" BOOL __stdcall bADioxGenerateViewAreaAI
		(
		/* INPUT */
		DWORD	dwDeviceType,		// �@��R�[�h
		LPDWORD	dwReadBuffa,		// ���̓f�[�^�o�b�t�@
		DWORD	dwBufferSize,		// ���̓f�[�^�o�b�t�@�T�C�Y
		BYTE	bMaxAiCh,		// �ő�AI�`�����l��
		DWORD	dwViewerSize_X,		// �g�`�`��̈�T�C�YX
		DWORD	dwViewerSize_Y,		// �g�`�`��̈�T�C�YY
		double	dY_Offset,		// Y�����I�t�Z�b�g	�F1.0�`0.0
		double	dY_Span,		// Y�����X�P�[��	�F1.0�ȏ�
		DWORD	dwX_Offset,		// X�����I�t�Z�b�g	�F0�ȏ� (�o�b�t�@�ǂݏo���ʒu�擪)
		DWORD	dwX_Span,		// X�����X�P�[��	�F0�ȏ�A0�ŃX�L�b�v�Ȃ�(�ǂݏo���ʒu�X�L�b�v��)
		DWORD	dwFftPoint,		// FFT�|�C���g��
		DWORD	dwFFTSize_X,		// FFT�`��̈�T�C�YX
		DWORD	dwFFTSize_Y,		// FFT�`��̈�T�C�YY
		/* OUTPUT */
		LPDWORD	lpdwPlotStart,		// �g�`�`��J�n�ʒu
		LPDWORD	lpdwPlotStop,		// �g�`�`��I���ʒu
		LPBOOL	lpbFftAnalizeComplete,	// FFT�v�Z������
		LPDWORD	dwCh_00_Data,		// �g�`�̉�ʑœ_�ʒu���i�[�����o�b�t�@
		LPDWORD	dwCh_01_Data,		// �g�`�̉�ʑœ_�ʒu���i�[�����o�b�t�@
		LPDWORD	dwCh_02_Data,		// �g�`�̉�ʑœ_�ʒu���i�[�����o�b�t�@
		LPDWORD	dwCh_03_Data,		// �g�`�̉�ʑœ_�ʒu���i�[�����o�b�t�@
		LPDWORD	dwCh_04_Data,		// �g�`�̉�ʑœ_�ʒu���i�[�����o�b�t�@
		LPDWORD	dwCh_05_Data,		// �g�`�̉�ʑœ_�ʒu���i�[�����o�b�t�@
		LPDWORD	dwCh_06_Data,		// �g�`�̉�ʑœ_�ʒu���i�[�����o�b�t�@
		LPDWORD	dwCh_07_Data,		// �g�`�̉�ʑœ_�ʒu���i�[�����o�b�t�@
		LPDWORD	dwCh_08_Data,		// �g�`�̉�ʑœ_�ʒu���i�[�����o�b�t�@
		LPDWORD	dwCh_09_Data,		// �g�`�̉�ʑœ_�ʒu���i�[�����o�b�t�@
		LPDWORD	dwCh_10_Data,		// �g�`�̉�ʑœ_�ʒu���i�[�����o�b�t�@
		LPDWORD	dwCh_11_Data,		// �g�`�̉�ʑœ_�ʒu���i�[�����o�b�t�@
		LPDWORD	dwCh_12_Data,		// �g�`�̉�ʑœ_�ʒu���i�[�����o�b�t�@
		LPDWORD	dwCh_13_Data,		// �g�`�̉�ʑœ_�ʒu���i�[�����o�b�t�@
		LPDWORD	dwCh_14_Data,		// �g�`�̉�ʑœ_�ʒu���i�[�����o�b�t�@
		LPDWORD	dwCh_15_Data,		// �g�`�̉�ʑœ_�ʒu���i�[�����o�b�t�@
		int	*iCh_00_Fft,		// FFT �̉�ʑœ_�ʒu���i�[�����o�b�t�@
		int	*iCh_01_Fft,		// FFT �̉�ʑœ_�ʒu���i�[�����o�b�t�@
		int	*iCh_02_Fft,		// FFT �̉�ʑœ_�ʒu���i�[�����o�b�t�@
		int	*iCh_03_Fft,		// FFT �̉�ʑœ_�ʒu���i�[�����o�b�t�@
		int	*iCh_04_Fft,		// FFT �̉�ʑœ_�ʒu���i�[�����o�b�t�@
		int	*iCh_05_Fft,		// FFT �̉�ʑœ_�ʒu���i�[�����o�b�t�@
		int	*iCh_06_Fft,		// FFT �̉�ʑœ_�ʒu���i�[�����o�b�t�@
		int	*iCh_07_Fft,		// FFT �̉�ʑœ_�ʒu���i�[�����o�b�t�@
		int	*iCh_08_Fft,		// FFT �̉�ʑœ_�ʒu���i�[�����o�b�t�@
		int	*iCh_09_Fft,		// FFT �̉�ʑœ_�ʒu���i�[�����o�b�t�@
		int	*iCh_10_Fft,		// FFT �̉�ʑœ_�ʒu���i�[�����o�b�t�@
		int	*iCh_11_Fft,		// FFT �̉�ʑœ_�ʒu���i�[�����o�b�t�@
		int	*iCh_12_Fft,		// FFT �̉�ʑœ_�ʒu���i�[�����o�b�t�@
		int	*iCh_13_Fft,		// FFT �̉�ʑœ_�ʒu���i�[�����o�b�t�@
		int	*iCh_14_Fft,		// FFT �̉�ʑœ_�ʒu���i�[�����o�b�t�@
		int	*iCh_15_Fft		// FFT �̉�ʑœ_�ʒu���i�[�����o�b�t�@
		);

	/////////////////
	// ���sDI,DWORD
	/////////////////
	extern "C" BOOL __stdcall bADioxGenerateViewAreaDI
		(
		/* INPUT */
		DWORD	dwDeviceType,		// �@��R�[�h
		LPDWORD	dwReadBuffa,		// ���̓f�[�^�o�b�t�@
		DWORD	dwBufferSize,		// ���̓f�[�^�o�b�t�@�T�C�Y
		BYTE	bMaxAiCh,		// �ő�AI�`�����l��
		DWORD	dwViewerSize_X,		// �g�`�`��̈�T�C�YX
		DWORD	dwX_Offset,		// X�����I�t�Z�b�g	�F0�ȏ� (�o�b�t�@�ǂݏo���ʒu�擪)
		DWORD	dwX_Span,		// X�����X�P�[��	�F0�ȏ�A0�ŃX�L�b�v�Ȃ�(�ǂݏo���ʒu�X�L�b�v��)
		/* OUTPUT */
		LPBYTE	lpbDiPaintMode,		// 0�Ŗ��`��A1��0-3ch�`��A2��0-15ch�`��
		LPDWORD	lpdwPlotStart,		// �g�`�`��J�n�ʒu
		LPDWORD	lpdwPlotStop,		// �g�`�`��I���ʒu

		LPDWORD	dwCh_00_Data,		// �g�`�̉�ʑœ_�ʒu���i�[�����o�b�t�@
		LPDWORD	dwCh_01_Data,		// �g�`�̉�ʑœ_�ʒu���i�[�����o�b�t�@
		LPDWORD	dwCh_02_Data,		// �g�`�̉�ʑœ_�ʒu���i�[�����o�b�t�@
		LPDWORD	dwCh_03_Data,		// �g�`�̉�ʑœ_�ʒu���i�[�����o�b�t�@
		LPDWORD	dwCh_04_Data,		// �g�`�̉�ʑœ_�ʒu���i�[�����o�b�t�@
		LPDWORD	dwCh_05_Data,		// �g�`�̉�ʑœ_�ʒu���i�[�����o�b�t�@
		LPDWORD	dwCh_06_Data,		// �g�`�̉�ʑœ_�ʒu���i�[�����o�b�t�@
		LPDWORD	dwCh_07_Data,		// �g�`�̉�ʑœ_�ʒu���i�[�����o�b�t�@
		LPDWORD	dwCh_08_Data,		// �g�`�̉�ʑœ_�ʒu���i�[�����o�b�t�@
		LPDWORD	dwCh_09_Data,		// �g�`�̉�ʑœ_�ʒu���i�[�����o�b�t�@
		LPDWORD	dwCh_10_Data,		// �g�`�̉�ʑœ_�ʒu���i�[�����o�b�t�@
		LPDWORD	dwCh_11_Data,		// �g�`�̉�ʑœ_�ʒu���i�[�����o�b�t�@
		LPDWORD	dwCh_12_Data,		// �g�`�̉�ʑœ_�ʒu���i�[�����o�b�t�@
		LPDWORD	dwCh_13_Data,		// �g�`�̉�ʑœ_�ʒu���i�[�����o�b�t�@
		LPDWORD	dwCh_14_Data,		// �g�`�̉�ʑœ_�ʒu���i�[�����o�b�t�@
		LPDWORD	dwCh_15_Data		// �g�`�̉�ʑœ_�ʒu���i�[�����o�b�t�@
		);


	////////////////////////////////////
	// �ǂݏo���ʒu�擪���v�Z����w���p
	////////////////////////////////////
	extern "C" DWORDLONG __stdcall dwADioxGetBufferStartAddress
		(
		/* INPUT */
		DWORD		dwDeviceType,		// �@��R�[�h
		BYTE		bMaxAiCh,		// �ő�AI�`�����l��
		DWORDLONG	dwlDataSize,		// �ő�f�[�^�T�C�Y�i�t�@�C���Ȃ�t�@�C���T�C�Y�A���C�u�Ȃ烊���O�o�b�t�@�T�C�Y�j
		DWORD		dwViewerSize_X,		// �g�`�`��̈�T�C�YX
		DWORD		dwX_Span,		// X�����X�P�[��	�F0�ȏ�A0�ŃX�L�b�v�Ȃ�(�ǂݏo���ʒu�X�L�b�v��)
		DWORD		dwFileOffset,		// �t�@�C���̏ꍇ�́A�f�[�^�擪�ʒu
		DWORD		dwHposCurrent,		// �����ʒu
		DWORD		dwHposMax		// �����ő�l
		);

	////////////////////////
	// �O���b�h�ʒu�̎w���l
	////////////////////////
	extern "C" double __stdcall dADioxGetGridText
		(
		/* INPUT */
		DWORD	dwDeviceType,		// �@��R�[�h
		DWORD	dwAiRange85,		// ���̓����WADX85
		BOOL	bAiRange14,		// ���̓����WADX14(TRUE�Ń��j�|�[����( bADX14input_type ( )�̒l���̂���)
		double	dY_Offset,		// Y�����I�t�Z�b�g	�F1.0�`0.0
		double	dY_Span,			// Y�����X�P�[��	�F1.0�ȏ�
		double	dLinePosition		// �O���b�h�̈ʒu	�F1.0�`0.0
		);


/*======================================================================================================*/
/* �g�`�W�F�l���[�g											*/
/*======================================================================================================*/
	extern "C" void __stdcall vADioxWaveInit ( struct ADIOX_EXTENTION2 *lpsADIOX_EXTENTION2,double *dFrequency0,double *dFrequency1,BYTE bCardID );
	extern "C" BOOL __stdcall bADioxWaveGenerate ( LPDWORD dwWriteBuffa,DWORD dwBufferSize,BYTE bCardID, struct ADIOX_EXTENTION2 *lpsADIOX_EXTENTION2 );


