#ifndef GLUPGRADECOLOR_
#define GLUPGRADECOLOR_

class GLUpgradeColor
{
	enum
	{
		COLOR_ARRAY = 47,
	};

public:
	BYTE	G_A[ COLOR_ARRAY ];
	BYTE	G_R[ COLOR_ARRAY ];
	BYTE	G_G[ COLOR_ARRAY ];
	BYTE	G_B[ COLOR_ARRAY ];
	BYTE	A_A[ COLOR_ARRAY ];
	BYTE	A_R[ COLOR_ARRAY ];
	BYTE	A_G[ COLOR_ARRAY ];
	BYTE	A_B[ COLOR_ARRAY ];
	BYTE	R_A[ COLOR_ARRAY ];
	BYTE	R_R[ COLOR_ARRAY ];
	BYTE	R_G[ COLOR_ARRAY ];
	BYTE	R_B[ COLOR_ARRAY ];	
	bool	bEnable[ COLOR_ARRAY ];	

public:
	BOOL	LoadUpgradeColor();

public:
	GLUpgradeColor();
	~GLUpgradeColor();

public:
	static GLUpgradeColor& GetInstance ();

};

#endif