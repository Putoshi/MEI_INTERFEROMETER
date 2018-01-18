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
//	// ファイル保存ヘッダ
//	struct LOG_FRONTEND
//	{
//		DWORD	dwHeaderCode;  		// 0x41594154
//		DWORD	dwDeviceName;  		// 機種コード
//		DWORD	dwBuffaSize;  		// セカンダリPCリングバッファサイズ
//		double	dClockScall;  		// サンプリングレート(Hz)
//		BYTE	bBitScall;  		// 量子化ビット数
//		BYTE	bAI_ChannelScall; 	// AIチャンネル数
//		BYTE	bDI_ChannelScall; 	// DIチャンネル数 実質未使用
//		BYTE	DataType;  		// DATA型種別判断 DWORD=0,double=1
//		DWORD	dwGetYear;  		// 計測開始の年
//		DWORD	dwGetMonth;  		// 計測開始の月
//		DWORD	dwGetDay;  		// 計測開始の日
//		DWORD	dwGetHour;  		// 計測開始の時
//		DWORD	dwGetMinute;  		// 計測開始の分
//		DWORD	dwGetSecond;  		// 計測開始の秒
//		DWORD	dwGetMilliseconds; 	// 計測開始のミリ秒
//		DWORD	dwInrange;	 	// 入力レンジ(ADX85はAIレンジそのまま、ADX14は1でユニポーラ／0でバイポーラ)
//		DWORD	dwReserved1;	 	// 
//		DWORD	dwReserved2;	 	// 
//	};
//};


typedef unsigned long DWORD;
typedef unsigned char BYTE;
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

