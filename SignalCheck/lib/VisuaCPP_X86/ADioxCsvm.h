/*==============================================================================================*/
/* MultifunctionI/O ADioxLog.dll用ヘッダ							*/
/*==============================================================================================*/
/*							Ver.1.00	：2014.6.18		*/
/*							Designer	：Akihiro Yokota	*/
/*							Copyright  2000-2014  SAYA Inc.		*/
/*==============================================================================================*/

	// 設定用構造体
	struct ADIOX_IO_NAME
		{
		char	csStr[50];
		};

	struct ADIOX_CSV_NAME
		{
		ADIOX_IO_NAME	sCH[MAX_AI_CH];
		};

	struct	ADIOX_CSV_FORMAT_EX
		{
		BOOL	bMultifunctionIO_Enable[MAX_MFIO];	// 使用するIOユニット(未使用の場合FALSE)
		double	dClockScall;				// サンプリングレート(Hz)
		BYTE	bAI_ChannelScall;			// AIチャンネル数
		BYTE	bDI_ChannelScall;			// DIチャンネル数
		char	*FolderName;				// CSVfileの保存場所 フルパス名 (例)"D:\\MFIO_Files" 
		char	*unit_Title;				// タイトル（不要な場合はNULLを指定してください）
		BOOL	bCsvLogEnable[MAX_AI_CH][MAX_MFIO];	// 記録するチャンネルのフラグ
		ADIOX_CSV_NAME	sDIOX_CSV_NAME[MAX_MFIO];	// 名称
		};

	extern "C" int __stdcall iADioxSpaceCheckEX ( char *NAME );
	extern "C" BOOL __stdcall bADioxLoggerInit ( struct ADIOX_CSV_FORMAT_EX sADIOX_CSV_FORMAT_EX );
	extern "C" BOOL __stdcall bADioxLoggerWrite ( double *dAI_Buffa,WORD wDI_Buffa,BOOL *bBO_Buffa,BYTE bCardId, BOOL bRefreshFile );
	extern "C" void __stdcall vADioxAddProjyect ( char *lpcProjyect,char *lpcOwner,char *lpcComment );
	extern "C" void __stdcall vGetLogFilename ( char * cName,BYTE bCardId );
	extern "C" void __stdcall vADioxLoggerExit ( );


