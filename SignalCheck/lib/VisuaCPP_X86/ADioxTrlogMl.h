	// �ݒ�\���́i�}���`�����[�gIO�j
	struct ADIOX_TREND_SETUP
		{
		RECT	pPpbRect;					// �v���r���[�T�C�Y
		char	*FolderName;					// �ۑ��ꏊ �t���p�X�� (��)ADIOX_CSV_FORMAT_EX.FolderName�ŗǂ�
		DWORD	dwCHMAX[MAX_MFIO];				// �ő�`�����l����
		double	dDivScall[MAX_AI_CH][MAX_MFIO];			// ���̓����W(double)���v���r���[�T�C�Y�Ƀt�B�b�e�B���O������X�P�[�����O
		double	dOffsetScall[MAX_AI_CH][MAX_MFIO];		// ���̓����W(double)���v���r���[�T�C�Y�Ƀt�B�b�e�B���O������I�t�Z�b�g
		double	dLimitter[MAX_AI_CH][MAX_MFIO];			// ���̓����W(double)�̃��~�b�^�ʒu
		BOOL	bMultifunctionIO_Enable[MAX_MFIO];		// �g�p����IO���j�b�g(���g�p�̏ꍇFALSE)
		//////////////////////////////////////////////
		BOOL	bCsvLogEnable[MAX_AI_CH][MAX_MFIO];		// �L���`�����l��
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

	// �f�[�^�������ݍ\���́i�}���`�����[�gIO�j
	struct ADIOX_SET_TREND_DATA
		{
		double	dAnalogData[MAX_AI_CH][MAX_MFIO];
		BOOL	bBurnOut[MAX_AI_CH][MAX_MFIO];
		};

	//////////////////////
	// ��ʃo�b�t�@������
	//////////////////////
	extern "C" BOOL __stdcall bADioxInitializeGraphicBufferMl_EX ( struct ADIOX_TREND_SETUP sADIOX_TREND_SETUP,
								char *lpcKoujimei,char *lpcGenbamei,char *lpcSagyousya );

	////////////////////////////////////////
	// ��ʃo�b�t�@�ɏ������݂��s��
	// �@��ʂɍ��킹�ẴX�P�[�����O���s��
	// �A�����l�Ƒœ_�ŕۑ�
	// �B��ʕ��܂ōs����FIFO������s��
	////////////////////////////////////////
	extern "C" BOOL __stdcall bADioxWriteGraphicBufferMl ( struct ADIOX_SET_TREND_DATA sADIOX_SET_TREND_DATA,BOOL bSetpixelOfTIME );

	////////////////////////////////////////
	// ��ʃo�b�t�@����œ_�̓Ǐo�����s��
	////////////////////////////////////////
	extern "C" BOOL __stdcall bADioxReadGraphicBufferMl ( int iXlocation,int *lpiSetpixel,LPBOOL lpbBurnOut,LPBOOL lpbSetpixelOfTIME,SYSTEMTIME *lpsSysTime,BYTE bCardId  );
	extern "C" BOOL __stdcall bADioxReadGraphicBufferAutoMl ( int *lpiSetpixel,LPBOOL lpbBurnOut,LPBOOL lpbSetpixelOfTIME,SYSTEMTIME *lpsSysTime,int *iXlocation,BYTE bCardId );
	extern "C" BOOL __stdcall bADioxGetBufferDepthMl ( int *iXlocation );
	extern "C" BOOL __stdcall bADioxReadGraphicDataMl ( int iXlocation,double *lpdAnalogData,LPBOOL lpbBurnOut,BYTE bCardId );

	/////////////////////////////////////////////
	// ��~�R�}���h�I��
	// ��ʏ����͍s�킸�t�@�C���t���b�V���̂�
	/////////////////////////////////////////////
	extern "C" BOOL __stdcall bADioxExitGraphicBufferMl ( );

	///////////////////////
	// �q�X�g���J���g�����h
	///////////////////////
	extern "C" BOOL __stdcall bADioxOpenTrendLogMl_EX ( char * szFile,BOOL bOpenDialog, RECT pPpbRect,int *lpiBlockSize,char *lpcKoujimei,char *lpcGenbamei,char *lpcSagyousya );
	extern "C" BOOL __stdcall bADioxReadTrendLogMl ( int iSeekPoint,LPBOOL lpbFinish,BYTE bCardId  );
	extern "C" BOOL __stdcall bADioxCloseTrendLogMl ( );
	extern "C" BOOL __stdcall bADioxCheckValidChMl ( BYTE bCH,BYTE bCardId	);
	extern "C" BOOL __stdcall bADioxCheckValidIoMl ( BYTE bCardId  );

	//////////////////////////////
	// ���O�t�@�C���̐�����on/off
	//////////////////////////////
	extern "C" BOOL __stdcall bADioxTrendLogModeMl ( BOOL bLogMode );

	//////////////////////////////
	// Open�_�C�A���O�f�B���N�g��
	//////////////////////////////
	extern "C" BOOL __stdcall bADioxHistDirMl ( DWORD dwSetInitialDir,char *FolderName );

