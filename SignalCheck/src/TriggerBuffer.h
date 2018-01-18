#pragma once

//class TriggerBuffer
//{
//	typedef unsigned long DWORD;
//	typedef unsigned char BYTE;
//
//public:
//	TriggerBuffer();
//	~TriggerBuffer();
//
//	// �t�@�C���ۑ��w�b�_
//	struct LOG_FRONTEND
//	{
//		DWORD	dwHeaderCode;  		// 0x41594154
//		DWORD	dwDeviceName;  		// �@��R�[�h
//		DWORD	dwBuffaSize;  		// �Z�J���_��PC�����O�o�b�t�@�T�C�Y
//		double	dClockScall;  		// �T���v�����O���[�g(Hz)
//		BYTE	bBitScall;  		// �ʎq���r�b�g��
//		BYTE	bAI_ChannelScall; 	// AI�`�����l����
//		BYTE	bDI_ChannelScall; 	// DI�`�����l���� �������g�p
//		BYTE	DataType;  		// DATA�^��ʔ��f DWORD=0,double=1
//		DWORD	dwGetYear;  		// �v���J�n�̔N
//		DWORD	dwGetMonth;  		// �v���J�n�̌�
//		DWORD	dwGetDay;  		// �v���J�n�̓�
//		DWORD	dwGetHour;  		// �v���J�n�̎�
//		DWORD	dwGetMinute;  		// �v���J�n�̕�
//		DWORD	dwGetSecond;  		// �v���J�n�̕b
//		DWORD	dwGetMilliseconds; 	// �v���J�n�̃~���b
//		DWORD	dwInrange;	 	// ���̓����W(ADX85��AI�����W���̂܂܁AADX14��1�Ń��j�|�[���^0�Ńo�C�|�[��)
//		DWORD	dwReserved1;	 	// 
//		DWORD	dwReserved2;	 	// 
//	};
//};


typedef unsigned long DWORD;
typedef unsigned char BYTE;
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

