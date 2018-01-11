	// 設定構造体（マルチリモートIO）
	struct ADIOX_TREND_SETUP
		{
		RECT	pPpbRect;					// プレビューサイズ
		char	*FolderName;					// 保存場所 フルパス名 (例)ADIOX_CSV_FORMAT_EX.FolderNameで良い
		DWORD	dwCHMAX[MAX_MFIO];				// 最大チャンネル数
		double	dDivScall[MAX_AI_CH][MAX_MFIO];			// 入力レンジ(double)をプレビューサイズにフィッティングさせるスケーリング
		double	dOffsetScall[MAX_AI_CH][MAX_MFIO];		// 入力レンジ(double)をプレビューサイズにフィッティングさせるオフセット
		double	dLimitter[MAX_AI_CH][MAX_MFIO];			// 入力レンジ(double)のリミッタ位置
		BOOL	bMultifunctionIO_Enable[MAX_MFIO];		// 使用するIOユニット(未使用の場合FALSE)
		//////////////////////////////////////////////
		BOOL	bCsvLogEnable[MAX_AI_CH][MAX_MFIO];		// 有効チャンネル
		//////////////////////////////////////////////
		DWORD	dwReserve0;
		DWORD	dwReserve1;
		DWORD	dwReserve2;
		DWORD	dwReserve3;
		double	dReserve0;
		double	dReserve1;
		double	dReserve2;
		double	dReserve3;
		};

	// データ書き込み構造体（マルチリモートIO）
	struct ADIOX_SET_TREND_DATA
		{
		double	dAnalogData[MAX_AI_CH][MAX_MFIO];
		BOOL	bBurnOut[MAX_AI_CH][MAX_MFIO];
		};

	//////////////////////
	// 画面バッファ初期化
	//////////////////////
	extern "C" BOOL __stdcall bADioxInitializeGraphicBufferMl_EX ( struct ADIOX_TREND_SETUP sADIOX_TREND_SETUP,
								char *lpcKoujimei,char *lpcGenbamei,char *lpcSagyousya );

	////////////////////////////////////////
	// 画面バッファに書き込みを行う
	// ①画面に合わせてのスケーリングを行う
	// ②実測値と打点で保存
	// ③画面幅まで行くとFIFO動作を行う
	////////////////////////////////////////
	extern "C" BOOL __stdcall bADioxWriteGraphicBufferMl ( struct ADIOX_SET_TREND_DATA sADIOX_SET_TREND_DATA,BOOL bSetpixelOfTIME );

	////////////////////////////////////////
	// 画面バッファから打点の読出しを行う
	////////////////////////////////////////
	extern "C" BOOL __stdcall bADioxReadGraphicBufferMl ( int iXlocation,int *lpiSetpixel,LPBOOL lpbBurnOut,LPBOOL lpbSetpixelOfTIME,SYSTEMTIME *lpsSysTime,BYTE bCardId  );
	extern "C" BOOL __stdcall bADioxReadGraphicBufferAutoMl ( int *lpiSetpixel,LPBOOL lpbBurnOut,LPBOOL lpbSetpixelOfTIME,SYSTEMTIME *lpsSysTime,int *iXlocation,BYTE bCardId );
	extern "C" BOOL __stdcall bADioxGetBufferDepthMl ( int *iXlocation );
	extern "C" BOOL __stdcall bADioxReadGraphicDataMl ( int iXlocation,double *lpdAnalogData,LPBOOL lpbBurnOut,BYTE bCardId );

	/////////////////////////////////////////////
	// 停止コマンド終了
	// 画面処理は行わずファイルフラッシュのみ
	/////////////////////////////////////////////
	extern "C" BOOL __stdcall bADioxExitGraphicBufferMl ( );

	///////////////////////
	// ヒストリカルトレンド
	///////////////////////
	extern "C" BOOL __stdcall bADioxOpenTrendLogMl_EX ( char * szFile,BOOL bOpenDialog, RECT pPpbRect,int *lpiBlockSize,char *lpcKoujimei,char *lpcGenbamei,char *lpcSagyousya );
	extern "C" BOOL __stdcall bADioxReadTrendLogMl ( int iSeekPoint,LPBOOL lpbFinish,BYTE bCardId  );
	extern "C" BOOL __stdcall bADioxCloseTrendLogMl ( );
	extern "C" BOOL __stdcall bADioxCheckValidChMl ( BYTE bCH,BYTE bCardId	);
	extern "C" BOOL __stdcall bADioxCheckValidIoMl ( BYTE bCardId  );

	//////////////////////////////
	// ログファイルの生成のon/off
	//////////////////////////////
	extern "C" BOOL __stdcall bADioxTrendLogModeMl ( BOOL bLogMode );

	//////////////////////////////
	// Openダイアログディレクトリ
	//////////////////////////////
	extern "C" BOOL __stdcall bADioxHistDirMl ( DWORD dwSetInitialDir,char *FolderName );

