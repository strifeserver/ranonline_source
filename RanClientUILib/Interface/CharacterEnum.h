#pragma	once


enum
{
	SELECT_SCHOOL_STAGE,
	SELECT_SEX_STAGE,
	SELECT_CLASS_STAGE,
	SELECT_STYLE_STAGE
};

enum 
{
	MAX_SELSCHOOL = 3,
	MAX_SEX = 2,
	MAX_CLASS = 9,
};

enum
{
	SUNGMUN_SCHOOL,
	BONGHWANG_SCHOOL,
	HYUNAM_SCHOOL,
};

enum 
{
	GENDER_MAN,
	GENDER_WOMAN,
};

enum 
{
	EXTREME_CLASS,
	FIGHTER_CLASS,
	ARMS_CLASS,
	ARCHER_CLASS,
	SPIRIT_CLASS,
	//add science
	SCIENCE_CLASS,
	ASSASIN_CLASS,
	TESTING_CLASS,
	SHAPER_CLASS,
};

static const float fFontRate = 0.8f;

static const char ClassImg[MAX_SEX][MAX_PATH] = { "class_image_m.dds", 
												  "class_image_w.dds" }; 

static const char jszBackImg1[] = { "SCHOOL_GUI_01.dds" };
static const char jszBackImg2[] = { "SCHOOL_GUI_02.dds" };
static const char jszBackImg3[] = { "SCHOOL_GUI_03.dds" };
