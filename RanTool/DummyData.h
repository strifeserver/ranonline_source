#pragma once
#include "stdafx.h"
#include "../../EngineLib/Meshs/DxPieceDefine.h"
#include "../../EngineLib/Meshs/DxAniKeys.h"
#include "../../EngineLib/Meshs/SAnimation.h"


enum	EFFCHARTYPES
{
	EFFCHAR_SINGLE			= 0,
	EFFCHAR_BLUR			= 1,
	EFFCHAR_CLONEBLUR		= 2,
	EFFCHAR_DUST			= 3,
	EFFCHAR_SHOCK			= 4,
	EFFCHAR_ATTRIBUTE		= 5,
	EFFCHAR_ARROW			= 6,
	EFFCHAR_SPECULAR		= 7,
	EFFCHAR_LEVEL			= 8,
	EFFCHAR_EMIT			= 9,
	EFFCHAR_ALPHA			= 10,
	EFFCHAR_NEON			= 11,
	EFFCHAR_MARK			= 12,
	EFFCHAR_NOALPHA			= 13,
	EFFCHAR_REFLECTION2		= 14,
	EFFCHAR_AMBIENT			= 15,
	EFFCHAR_DOT3			= 16,
	EFFCHAR_MULTITEX		= 17,
	EFFCHAR_GHOSTING		= 18,
	EFFCHAR_SPECULAR2		= 19,
	EFFCHAR_TOON			= 20,
	EFFCHAR_TEXDIFF			= 21,
	EFFCHAR_PARTICLE		= 22,
	EFFCHAR_BONEPOSEFF		= 23,
	EFFCHAR_BONELISTEFF		= 24,
	EFFCHAR_USERCOLOR		= 25,
	EFFCHAR_NORMALMAP		= 26,
	EFFCHAR_LINE2BONE		= 27,
	EFFCHAR_AROUNDEFFECT	= 28,

	EFFCHAR_SIZE			= 29,
};

enum DCRDT_TYPE		
{
	DCRDT_DEFAULT		= 0,
	DCRDT_SPECULAR		= 1,
	DCRDT_DOT3_SPECULAR	= 2,
	DCRDT_DOT3_REFLECT	= 3,
	DCRDT_GLOW			= 4,
	DCRDT_SIZE			= 5,
};

enum	DCOLOROP
{
	XD3DTOP_NONE						= 0, 
	XD3DTOP_DISABLE						= 1, 
    XD3DTOP_SELECTARG1					= 2,
    XD3DTOP_SELECTARG2					= 3,
    XD3DTOP_MODULATE					= 4, 
    XD3DTOP_MODULATE2X					= 5,
    XD3DTOP_MODULATE4X					= 6, 
    XD3DTOP_ADD							= 7,
    XD3DTOP_ADDSIGNED					= 8,
    XD3DTOP_ADDSIGNED2X					= 9,
    XD3DTOP_SUBTRACT					= 10, 
    XD3DTOP_ADDSMOOTH					= 11,
    XD3DTOP_BLENDDIFFUSEALPHA			= 12,
    XD3DTOP_BLENDTEXTUREALPHA			= 13,
    XD3DTOP_BLENDFACTORALPHA			= 14,
    XD3DTOP_BLENDTEXTUREALPHAPM			= 15,
    XD3DTOP_BLENDCURRENTALPHA			= 16,
    XD3DTOP_PREMODULATE					= 17,
    XD3DTOP_MODULATEALPHA_ADDCOLOR		= 18,
    XD3DTOP_MODULATECOLOR_ADDALPHA		= 19,
    XD3DTOP_MODULATEINVALPHA_ADDCOLOR	= 20,
    XD3DTOP_MODULATEINVCOLOR_ADDALPHA	= 21,
    XD3DTOP_BUMPENVMAP					= 22,
    XD3DTOP_BUMPENVMAPLUMINANCE			= 23,
    XD3DTOP_DOTPRODUCT3					= 24,
    XD3DTOP_MULTIPLYADD					= 25,
    XD3DTOP_LERP						= 26,

	XD3DTOP_SIZE						= 27,
};

enum EFFANITYPES
{
	EFFANI_SINGLE	= 0,
	EFFANI_GHOSTING	= 1,
	EFFANI_TRACE	= 2,
	EFFANI_FACEOFF	= 3,

	EFFANI_SIZE		= 4,
};

enum	WEATHER
{
	WNONE = 0,
	NORMAL = 1,
	RAIN = 2,

	WESIZE = 3
};

enum	ANIMATION
{
	ANIMNONE = 0,
	ATTACKANIM = 1,
	NORMALANIM = 2,

	ANSIZE = 3
};

namespace DUMMYDATA
{
	extern std::string m_CharType[PIECE_SIZE];
	extern std::string m_SlotType[EMPEACE_WEAPON_SIZE];
	extern std::string m_StrikeEff[EMSF_SIZE];
	extern std::string m_AniMain[AN_TYPE_SIZE];
	extern std::string m_AniSub[AN_SUB_00_SIZE];
	extern std::string m_DColorOp[XD3DTOP_SIZE];
	extern std::string m_CRDT_Type[DCRDT_SIZE];
	extern std::string m_EffCharType[EFFCHAR_SIZE];
	extern std::string m_EffAniType[EFFANI_SIZE];

	extern std::string m_WeatherType[WESIZE];
	extern std::string m_AnimType[ANSIZE];
};