/*==============================================================================================*/
/* MultifunctionI/O ADioxLog.dll�p�w�b�_							*/
/*==============================================================================================*/
/*							Ver.1.00	�F2014.6.18		*/
/*							Designer	�FAkihiro Yokota	*/
/*							Copyright  2000-2014  SAYA Inc.		*/
/*==============================================================================================*/

	// �ݒ�p�\����
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
		BOOL	bMultifunctionIO_Enable[MAX_MFIO];	// �g�p����IO���j�b�g(���g�p�̏ꍇFALSE)
		double	dClockScall;				// �T���v�����O���[�g(Hz)
		BYTE	bAI_ChannelScall;			// AI�`�����l����
		BYTE	bDI_ChannelScall;			// DI�`�����l����
		char	*FolderName;				// CSVfile�̕ۑ��ꏊ �t���p�X�� (��)"D:\\MFIO_Files" 
		char	*unit_Title;				// �^�C�g���i�s�v�ȏꍇ��NULL���w�肵�Ă��������j
		BOOL	bCsvLogEnable[MAX_AI_CH][MAX_MFIO];	// �L�^����`�����l���̃t���O
		ADIOX_CSV_NAME	sDIOX_CSV_NAME[MAX_MFIO];	// ����
		};

	extern "C" int __stdcall iADioxSpaceCheckEX ( char *NAME );
	extern "C" BOOL __stdcall bADioxLoggerInit ( struct ADIOX_CSV_FORMAT_EX sADIOX_CSV_FORMAT_EX );
	extern "C" BOOL __stdcall bADioxLoggerWrite ( double *dAI_Buffa,WORD wDI_Buffa,BOOL *bBO_Buffa,BYTE bCardId, BOOL bRefreshFile );
	extern "C" void __stdcall vADioxAddProjyect ( char *lpcProjyect,char *lpcOwner,char *lpcComment );
	extern "C" void __stdcall vGetLogFilename ( char * cName,BYTE bCardId );
	extern "C" void __stdcall vADioxLoggerExit ( );


