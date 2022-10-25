#ifndef NEOEASYCRYPTION_TEST_
#define NEOEASYCRYPTION_TEST_

#pragma once

namespace CCrypt
{
	enum
	{

#if defined( RELEASE_A )
		EN	= 0x03,
		EN2 = 0x31,
		//EN	= 0x08,
		//EN2 = 0x25,
#elif defined( RELEASE_B )
		EN	= 0x43,
		EN2 = 0x78,
#elif defined( RELEASE_C )
		EN	= 0x55,
		EN2 = 0x80,
#elif defined( RELEASE_D )
		EN	= 0x67,
		EN2 = 0x92,
#else
		EN	= 0x12,
		EN2 = 0x27,	
#endif
		END = 0x03,
	};
	enum
	{
		ENTYPE_ALL,
	};
	enum
	{
		ENCRY_VER = 0x100,
		ENCRY_HEADER_SIZE = 12,
	};

	enum 
	{
		TEXTURE_DDS,
		TEXTURE_TGA,

		TEXTURE_TYPE,
	};

	enum 
	{
		TEX_HEADER_SIZE = 12,
		TEX_VERSION		= 0x100,
#if defined( RELEASE_A )
		TEX_XOR_DATA	= 0x53,
		TEX_DIFF_DATA	= 0x07,
#elif defined( RELEASE_B )
		TEX_XOR_DATA	= 0x55,
		TEX_DIFF_DATA	= 0x09,
#elif defined( RELEASE_C )
		TEX_XOR_DATA	= 0x57,
		TEX_DIFF_DATA	= 0x11,
#elif defined( RELEASE_D )
		TEX_XOR_DATA	= 0x59,
		TEX_DIFF_DATA	= 0x13,
#else
		TEX_XOR_DATA	= 0x48,
		TEX_DIFF_DATA	= 0x05,
#endif
	};

	extern BOOL	bCryptRCC;
	extern BOOL	bEngineDebug;
	extern BOOL	bEngineDebugLog;

	void Encryption(BYTE* B,int dwSize);
	void Decryption(BYTE* B,int dwSize);
	void LoadSetting( const TCHAR *szRootPath );

};

#endif
