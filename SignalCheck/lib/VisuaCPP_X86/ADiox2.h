/*======================================================================================================*/
/* MultifunctionI/O symmetry driver file								*/
/*======================================================================================================*/
/*							Ver.1.00	：2002.12.9			*/
/*							Designer	：Akihiro Yokota		*/
/*							Copyright  2000-2002  SAYA Akihiro Yokota	*/
/*======================================================================================================*/

/*======================================================================================================*/
/*	割り込みﾒｯｾｰｼﾞ											*/
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
/*	各種定義											*/
/*======================================================================================================*/

	#define DMA_BURST_SPLEX		0x400000	// ADX14大容量リングバッファサイズ
	#define DMA_BURST_SPLEX_DW	0x100000	// ADX14大容量リングバッファサイズ
	#define	ADX42_LAN_BUFFER_SIZE	256		// ADX42リングバッファサイズ
	#define	ADX42_LAN_BUFFER_SIZE_L	1024		// ADX42B-FULLリングバッファサイズ
	#define	ADX85_PCI_BUFFER_SIZE	3328		// ADX85プライマリリングバッファサイズ

	// 複数ﾃﾞﾊﾞｲｽの識別定数
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
	#define	MAX_AI_CH		32		// 今のところ16chであるが将来拡張のために32ch確保

/*======================================================================================================*/
/*	構造体定義											*/
/*======================================================================================================*/

	// イベント・メッセージ種類(割り込み)
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

	// トリガ・リングバッファ
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
		// チャンネルシークエンス
		DWORD	dwMuxSeqenceAuto;
		// AO,DOをリングバッファ経由にするか？
		DWORD	dwAoHspBufferd;
		DWORD	dwDoHspBufferd;

		// ADX14において、ADIをリングバッファ経由にするか
		DWORD	dwAdiBufferOn;

		// カウンタをリングバッファ経由にするか
		BYTE	bConnectBuffer;	
		// トリガイネーブル
		BOOL	bTrigEnable;
		};

	// IO設定
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

	// DIOエンコーダカウンタ・周波数カウンタ・ストローブ・PWMマッピング・割り込み
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

	// AI,DI,AO,DO,COUNTER,温度のポーリングおよび物理定数変換を統合
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

	// PWMパラメタ
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

	// PWMディティー
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

	// ステータス
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

	// デバイス情報
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

	// リモートIO用のIPアドレス・ポート番号
	struct ADR_IP_CONF
		{
		int		iIP1[MAX_MFIO];
		int		iIP2[MAX_MFIO];
		int		iIP3[MAX_MFIO];
		int		iIP4[MAX_MFIO];
		int		iPORT[MAX_MFIO];
		BOOL		bEnable[MAX_MFIO];
		};

	// 多数のIOボードの設定、シグナルコンディションに対応
	struct SCP_SETUP2
		{
		// イネーブル
		BOOL	bMultifunctionIO_Enable[MAX_MFIO];
		// X-API初期化のための構造体
		TXBUFSETUP2	sTXBUFSETUP[MAX_MFIO];
		IOGEOSETUP2	sIOGEOSETUP[MAX_MFIO];
		TDIO_MISC	sTDIO_MISC[MAX_MFIO];
		TConfigPWM	sTConfigPWM[MAX_MFIO];
		TSetupPWM	sTSetupPWM[MAX_MFIO];
		// シグナルコンディションレジスタへの設定
		DWORD		dwLDO1[MAX_MFIO];
		DWORD		dwLDO2[MAX_MFIO];
		// センサー種別
		DWORD		dwSensorMode[MAX_MFIO][MAX_AI_CH];
		// ゼロスパン
		double		doZeroPos[MAX_MFIO][MAX_AI_CH];
		double		doSpanPos[MAX_MFIO][MAX_AI_CH];
		double		doZero_Coefficient[MAX_MFIO][MAX_AI_CH];
		double		doSpan_Coefficient[MAX_MFIO][MAX_AI_CH];
		// スケーリング
		BOOL		bScalling[MAX_MFIO][MAX_AI_CH];
		double		dScallingRatio[MAX_MFIO][MAX_AI_CH];
		double		dOutTopScall[MAX_MFIO][MAX_AI_CH];
		double		dOutBottomScall[MAX_MFIO][MAX_AI_CH];
		double		dInTopScall[MAX_MFIO][MAX_AI_CH];
		double		dInBottomScall[MAX_MFIO][MAX_AI_CH];
		// アラーム
		DWORD		bAlarmMode[MAX_MFIO][MAX_AI_CH];
		double		dAlarmUpper[MAX_MFIO][MAX_AI_CH];
		double		dAlarmLower[MAX_MFIO][MAX_AI_CH];
		};

	struct SCP_SETUP_CS2	// C#用
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

	// 単一チャンネルのシグナルコンディション設定値の授受のための構造体（校正係数含まず）
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

	// 単一チャンネルのシグナルコンディション設定値の授受のための構造体（校正係数含む）
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

	// ファイル保存機能などの拡張構造体C/C++/VC++/VC#用
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
		int		iAO_SamplePerCycle0;	// 1サイクルあたりのサンプル数1スタート
		int		iAO_SamplePerCycle1;	// 1サイクルあたりのサンプル数1スタート
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
		int		iAO_SamplePerCycle0;	// 1サイクルあたりのサンプル数1スタート
		int		iAO_SamplePerCycle1;	// 1サイクルあたりのサンプル数1スタート
		DWORD		dwADO_style0;
		DWORD		dwADO_style1;
		char		*lpcAdoFileName;
		// RESERVED
		DWORD		dwReserverd[16];
		};

	// 文字列を格納する構造体C/C++/VC++/VC#用
	struct CharPayloadC
		{
		char	lpcInitialDir[256];
		char	lpcConfigFileName[256];
		};

	// 文字列を格納する構造体C/C++/VC++/VB用
	struct CharPayloadB
		{
		char	*lpcInitialDir;
		char	*lpcConfigFileName;
		};

	// ファイル保存ヘッダ
	struct LOG_FRONTEND
		{
		DWORD	dwHeaderCode;  		// 0x41594154
		DWORD	dwDeviceName;  		// 機種コード
		DWORD	dwBuffaSize;  		// セカンダリPCリングバッファサイズ
		double	dClockScall;  		// サンプリングレート(Hz)
		BYTE	bBitScall;  		// 量子化ビット数
		BYTE	bAI_ChannelScall; 	// AIチャンネル数
		BYTE	bDI_ChannelScall; 	// DIチャンネル数 実質未使用
		BYTE	DataType;  		// DATA型種別判断 DWORD=0,double=1
		DWORD	dwGetYear;  		// 計測開始の年
		DWORD	dwGetMonth;  		// 計測開始の月
		DWORD	dwGetDay;  		// 計測開始の日
		DWORD	dwGetHour;  		// 計測開始の時
		DWORD	dwGetMinute;  		// 計測開始の分
		DWORD	dwGetSecond;  		// 計測開始の秒
		DWORD	dwGetMilliseconds; 	// 計測開始のミリ秒
		DWORD	dwInrange;	 	// 入力レンジ(ADX85はAIレンジそのまま、ADX14は1でユニポーラ／0でバイポーラ)
		DWORD	dwReserved1;	 	// 
		DWORD	dwReserved2;	 	// 
		};

	/*==============================================================================================*/
	/* 高速リモートIOのEEPROM設定									*/
	/*==============================================================================================*/

	// IPアドレスとポート
	struct TCP_IP_SETTING
		{
		int Port;
		int IP1;
		int IP2;
		int IP3;
		int IP4;
		};

	// IPアドレスとポート、ゲートウェイ、サブネット
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

	// MACアドレス
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
/*	引数の定義											*/
/*======================================================================================================*/
	// リングバッファ割り込みモード
	#define	NOT_INT		0
	#define	DMA_INT		1

	// トリガモード5-10番は11-12番からハードウェア内部で生成される
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

	// AO0-3モード
	#define	NO_LINK		0
	#define	LIVE_CTC	1
	#define	LATCH_CTC	2

	// DI割り込みモード
	#define	NO_INT		0
	#define	POSEDGE_INT	1
	#define	NEGEDGE_INT	2
	#define	DUALEDGE_INT	3

	// カウンターラッチモード
	#define	SOFT		0
	#define	Z_PHASE		1
	#define	DI_SEL		2

	// リングバッファーステータス(割り込みは発生しない)
	#define	NO_TRANSFER	0
	#define	EMPTY_BUFFER	1
	#define	FULL_BUFFER	2
	#define	INVALID_BUFFER	3
	#define	OVERRUN_BUFFER	4
	#define	UNDERRUN_BUFFER	4
	#define	ERR0_BUFFER	5
	#define	ERR1_BUFFER	6
	#define	ERR2_BUFFER	7

	// 割り込みポストメッセージのメッセージコード
	#define	ERROR_DETECT		0x1
	#define	DMA_BUFFER_HALF_EMPTY	0x2
	#define	DMA_BUFFER_FULL_EMPTY	0x4
	#define	DI_EVENT		0x8
	#define	COUNTER_EVENT		0x10
	#define	DMA_SIGNAL		0x20
	#define	ADO_FIFO_HALF_EMPTY	0x40
	#define	BANK_CHANGE		0x80

	// デバイスの種類
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

	// センサー種別
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
	#define D16BIT		27	// 0-65535のデジタル値のこと
	#define RION_DC80	28	// リオン騒音計振動計
	#define RION_DC90	29	// リオン騒音計振動計
	#define RION_DC100	30	// リオン騒音計振動計
	#define RION_DC110	31	// リオン騒音計振動計
	#define RION_DC120	32	// リオン騒音計振動計
	#define RION_DC130	33	// リオン騒音計振動計
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
/*	エクスポートしている関数									*/
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
	// これらはC/C++/VC++用
	extern "C" void __stdcall vADioxScpCopy2    ( struct SCP_SETUP2 * lpsSCP_SETUP );
	extern "C" void __stdcall vADioxScpCopy_CS2 ( struct SCP_SETUP_CS2 * lpsSCP_SETUP_CS );
	extern "C" BOOL __stdcall bADioxScpDefault2    ( DWORD dwSensorMode,DWORD dwDeviceNumber, DWORD dwCH,struct SCP_SETUP2 * lpsSCP_SETUP );
	extern "C" BOOL __stdcall bADioxScpDefaultCS2  ( DWORD dwSensorMode,DWORD dwDeviceNumber, DWORD dwCH,struct SCP_SETUP_CS2 * lpsSCP_SETUP_CS );

	// これはVC#用
	extern "C" BOOL __stdcall bADioxScpDefaultReconvert2 ( struct SCP_SETUP_CS2 sSCP_SETUP_CS, struct SCP_SETUP2 * lpsSCP_SETUP );

	// これはC/C++/VC++/VC#用
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

	// SAYA予約：一般には使わないでください//////////////////////////////////////////////////////////////////////////////////////////////////////////
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
/* 波形描画アシスト											*/
/*======================================================================================================*/

	//////////
	// 初期化
	//////////
	extern "C" BOOL __stdcall vADioxInitializeViewArea
		(
		DWORD	wDiViewerSize_Y		// 波形描画領域サイズY(DI)
		);

	/////////////////
	// 実行AI,DWORD
	/////////////////
	extern "C" BOOL __stdcall bADioxGenerateViewAreaAI
		(
		/* INPUT */
		DWORD	dwDeviceType,		// 機種コード
		LPDWORD	dwReadBuffa,		// 入力データバッファ
		DWORD	dwBufferSize,		// 入力データバッファサイズ
		BYTE	bMaxAiCh,		// 最大AIチャンネル
		DWORD	dwViewerSize_X,		// 波形描画領域サイズX
		DWORD	dwViewerSize_Y,		// 波形描画領域サイズY
		double	dY_Offset,		// Y方向オフセット	：1.0～0.0
		double	dY_Span,		// Y方向スケール	：1.0以上
		DWORD	dwX_Offset,		// X方向オフセット	：0以上 (バッファ読み出し位置先頭)
		DWORD	dwX_Span,		// X方向スケール	：0以上、0でスキップなし(読み出し位置スキップ量)
		DWORD	dwFftPoint,		// FFTポイント数
		DWORD	dwFFTSize_X,		// FFT描画領域サイズX
		DWORD	dwFFTSize_Y,		// FFT描画領域サイズY
		/* OUTPUT */
		LPDWORD	lpdwPlotStart,		// 波形描画開始位置
		LPDWORD	lpdwPlotStop,		// 波形描画終了位置
		LPBOOL	lpbFftAnalizeComplete,	// FFT計算が成功
		LPDWORD	dwCh_00_Data,		// 波形の画面打点位置を格納したバッファ
		LPDWORD	dwCh_01_Data,		// 波形の画面打点位置を格納したバッファ
		LPDWORD	dwCh_02_Data,		// 波形の画面打点位置を格納したバッファ
		LPDWORD	dwCh_03_Data,		// 波形の画面打点位置を格納したバッファ
		LPDWORD	dwCh_04_Data,		// 波形の画面打点位置を格納したバッファ
		LPDWORD	dwCh_05_Data,		// 波形の画面打点位置を格納したバッファ
		LPDWORD	dwCh_06_Data,		// 波形の画面打点位置を格納したバッファ
		LPDWORD	dwCh_07_Data,		// 波形の画面打点位置を格納したバッファ
		LPDWORD	dwCh_08_Data,		// 波形の画面打点位置を格納したバッファ
		LPDWORD	dwCh_09_Data,		// 波形の画面打点位置を格納したバッファ
		LPDWORD	dwCh_10_Data,		// 波形の画面打点位置を格納したバッファ
		LPDWORD	dwCh_11_Data,		// 波形の画面打点位置を格納したバッファ
		LPDWORD	dwCh_12_Data,		// 波形の画面打点位置を格納したバッファ
		LPDWORD	dwCh_13_Data,		// 波形の画面打点位置を格納したバッファ
		LPDWORD	dwCh_14_Data,		// 波形の画面打点位置を格納したバッファ
		LPDWORD	dwCh_15_Data,		// 波形の画面打点位置を格納したバッファ
		int	*iCh_00_Fft,		// FFT の画面打点位置を格納したバッファ
		int	*iCh_01_Fft,		// FFT の画面打点位置を格納したバッファ
		int	*iCh_02_Fft,		// FFT の画面打点位置を格納したバッファ
		int	*iCh_03_Fft,		// FFT の画面打点位置を格納したバッファ
		int	*iCh_04_Fft,		// FFT の画面打点位置を格納したバッファ
		int	*iCh_05_Fft,		// FFT の画面打点位置を格納したバッファ
		int	*iCh_06_Fft,		// FFT の画面打点位置を格納したバッファ
		int	*iCh_07_Fft,		// FFT の画面打点位置を格納したバッファ
		int	*iCh_08_Fft,		// FFT の画面打点位置を格納したバッファ
		int	*iCh_09_Fft,		// FFT の画面打点位置を格納したバッファ
		int	*iCh_10_Fft,		// FFT の画面打点位置を格納したバッファ
		int	*iCh_11_Fft,		// FFT の画面打点位置を格納したバッファ
		int	*iCh_12_Fft,		// FFT の画面打点位置を格納したバッファ
		int	*iCh_13_Fft,		// FFT の画面打点位置を格納したバッファ
		int	*iCh_14_Fft,		// FFT の画面打点位置を格納したバッファ
		int	*iCh_15_Fft		// FFT の画面打点位置を格納したバッファ
		);

	/////////////////
	// 実行DI,DWORD
	/////////////////
	extern "C" BOOL __stdcall bADioxGenerateViewAreaDI
		(
		/* INPUT */
		DWORD	dwDeviceType,		// 機種コード
		LPDWORD	dwReadBuffa,		// 入力データバッファ
		DWORD	dwBufferSize,		// 入力データバッファサイズ
		BYTE	bMaxAiCh,		// 最大AIチャンネル
		DWORD	dwViewerSize_X,		// 波形描画領域サイズX
		DWORD	dwX_Offset,		// X方向オフセット	：0以上 (バッファ読み出し位置先頭)
		DWORD	dwX_Span,		// X方向スケール	：0以上、0でスキップなし(読み出し位置スキップ量)
		/* OUTPUT */
		LPBYTE	lpbDiPaintMode,		// 0で無描画、1で0-3ch描画、2で0-15ch描画
		LPDWORD	lpdwPlotStart,		// 波形描画開始位置
		LPDWORD	lpdwPlotStop,		// 波形描画終了位置

		LPDWORD	dwCh_00_Data,		// 波形の画面打点位置を格納したバッファ
		LPDWORD	dwCh_01_Data,		// 波形の画面打点位置を格納したバッファ
		LPDWORD	dwCh_02_Data,		// 波形の画面打点位置を格納したバッファ
		LPDWORD	dwCh_03_Data,		// 波形の画面打点位置を格納したバッファ
		LPDWORD	dwCh_04_Data,		// 波形の画面打点位置を格納したバッファ
		LPDWORD	dwCh_05_Data,		// 波形の画面打点位置を格納したバッファ
		LPDWORD	dwCh_06_Data,		// 波形の画面打点位置を格納したバッファ
		LPDWORD	dwCh_07_Data,		// 波形の画面打点位置を格納したバッファ
		LPDWORD	dwCh_08_Data,		// 波形の画面打点位置を格納したバッファ
		LPDWORD	dwCh_09_Data,		// 波形の画面打点位置を格納したバッファ
		LPDWORD	dwCh_10_Data,		// 波形の画面打点位置を格納したバッファ
		LPDWORD	dwCh_11_Data,		// 波形の画面打点位置を格納したバッファ
		LPDWORD	dwCh_12_Data,		// 波形の画面打点位置を格納したバッファ
		LPDWORD	dwCh_13_Data,		// 波形の画面打点位置を格納したバッファ
		LPDWORD	dwCh_14_Data,		// 波形の画面打点位置を格納したバッファ
		LPDWORD	dwCh_15_Data		// 波形の画面打点位置を格納したバッファ
		);


	////////////////////////////////////
	// 読み出し位置先頭を計算するヘルパ
	////////////////////////////////////
	extern "C" DWORDLONG __stdcall dwADioxGetBufferStartAddress
		(
		/* INPUT */
		DWORD		dwDeviceType,		// 機種コード
		BYTE		bMaxAiCh,		// 最大AIチャンネル
		DWORDLONG	dwlDataSize,		// 最大データサイズ（ファイルならファイルサイズ、ライブならリングバッファサイズ）
		DWORD		dwViewerSize_X,		// 波形描画領域サイズX
		DWORD		dwX_Span,		// X方向スケール	：0以上、0でスキップなし(読み出し位置スキップ量)
		DWORD		dwFileOffset,		// ファイルの場合の、データ先頭位置
		DWORD		dwHposCurrent,		// 水平位置
		DWORD		dwHposMax		// 水平最大値
		);

	////////////////////////
	// グリッド位置の指示値
	////////////////////////
	extern "C" double __stdcall dADioxGetGridText
		(
		/* INPUT */
		DWORD	dwDeviceType,		// 機種コード
		DWORD	dwAiRange85,		// 入力レンジADX85
		BOOL	bAiRange14,		// 入力レンジADX14(TRUEでユニポーラ＝( bADX14input_type ( )の値そのもの)
		double	dY_Offset,		// Y方向オフセット	：1.0～0.0
		double	dY_Span,			// Y方向スケール	：1.0以上
		double	dLinePosition		// グリッドの位置	：1.0～0.0
		);


/*======================================================================================================*/
/* 波形ジェネレート											*/
/*======================================================================================================*/
	extern "C" void __stdcall vADioxWaveInit ( struct ADIOX_EXTENTION2 *lpsADIOX_EXTENTION2,double *dFrequency0,double *dFrequency1,BYTE bCardID );
	extern "C" BOOL __stdcall bADioxWaveGenerate ( LPDWORD dwWriteBuffa,DWORD dwBufferSize,BYTE bCardID, struct ADIOX_EXTENTION2 *lpsADIOX_EXTENTION2 );


