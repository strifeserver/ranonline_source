#include "stdafx.h"

#include "./G-Logic/GLCharDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

WORD school2index ( const EMSCHOOLFLAG emSchool )
{
	switch ( emSchool )
	{
	case GLSCHOOL_00:	return 0;
	case GLSCHOOL_01:	return 1;
	case GLSCHOOL_02:	return 2;
	};

	return 0;
}

EMSCHOOLFLAG index2school ( const WORD wSchool )
{
	switch ( wSchool )
	{
	case 0:	return GLSCHOOL_00;
	case 1:	return GLSCHOOL_01;
	case 2:	return GLSCHOOL_02;
	};

	return GLSCHOOL_00;
}

EMCHARINDEX CharClassToIndex ( const EMCHARCLASS emClass )
{
	switch (emClass)
	{
	case GLCC_FIGHTER_M:		return GLCI_FIGHTER_M;
	case GLCC_ARMS_M:			return GLCI_ARMS_M;
	case GLCC_ARCHER_W:		return GLCI_ARCHER_W;
	case GLCC_SPIRIT_W:		return GLCI_SPIRIT_W;
	case GLCC_EXTREME_M:	return GLCI_EXTREME_M;
	case GLCC_EXTREME_W:	return GLCI_EXTREME_W;
	
	case GLCC_FIGHTER_W:	return GLCI_FIGHTER_W;
	case GLCC_ARMS_W:		return GLCI_ARMS_W;
	case GLCC_ARCHER_M:		return GLCI_ARCHER_M;
	case GLCC_SPIRIT_M:		return GLCI_SPIRIT_M;

		//add class
	case GLCC_SCIENCE_M:		return GLCI_SCIENCE_M;
	case GLCC_SCIENCE_W:		return GLCI_SCIENCE_W;
	case GLCC_ASSASIN_M:		return GLCI_ASSASIN_M;
	case GLCC_ASSASIN_W:		return GLCI_ASSASIN_W;
	case GLCC_TESTING_M:		return GLCI_TESTING_M;
	case GLCC_TESTING_W:		return GLCI_TESTING_W;
	case GLCC_SHAPER_M:		return GLCI_SHAPER_M;
	case GLCC_SHAPER_W:		return GLCI_SHAPER_W;

		//
	};

	GASSERT(0&&"잘못된 케릭터 클래스입니다.");
	return GLCI_FIGHTER_M;
}

EMCHARCLASS CharClassGetSex ( const EMCHARINDEX emIndex ) //add abl
{
	switch (emIndex)
	{
	case GLCI_FIGHTER_M:
	case GLCI_ARMS_M:
	case GLCI_ARCHER_M:
	case GLCI_SPIRIT_M:
	case GLCI_EXTREME_M:
		case GLCI_SCIENCE_M:
		case GLCI_ASSASIN_M:
		case GLCI_TESTING_M:
		case GLCI_SHAPER_M:	return GLCC_MAN;
	case GLCI_FIGHTER_W:
	case GLCI_ARMS_W:
	case GLCI_ARCHER_W:
	case GLCI_SPIRIT_W:
	case GLCI_EXTREME_W:
		case GLCI_SCIENCE_W:
		case GLCI_ASSASIN_W:
		case GLCI_TESTING_W:
		case GLCI_SHAPER_W:			return GLCC_WOMAN;
	};

	GASSERT(0&&"잘못된 케릭터 인덱스입니다.");
	return GLCC_MAN;
}

EMCHARCLASS CharIndexToClass ( const EMCHARINDEX emIndex )
{
	switch (emIndex)
	{
	case GLCI_FIGHTER_M:	return GLCC_FIGHTER_M;
	case GLCI_ARMS_M:		return GLCC_ARMS_M;
	case GLCI_ARCHER_W:		return GLCC_ARCHER_W;
	case GLCI_SPIRIT_W:		return GLCC_SPIRIT_W;

	case GLCI_EXTREME_M:	return GLCC_EXTREME_M;
	case GLCI_EXTREME_W:	return GLCC_EXTREME_W;

	case GLCI_FIGHTER_W:	return GLCC_FIGHTER_W;
	case GLCI_ARMS_W:		return GLCC_ARMS_W;
	case GLCI_ARCHER_M:		return GLCC_ARCHER_M;
	case GLCI_SPIRIT_M:		return GLCC_SPIRIT_M;

		//add class
	case GLCI_SCIENCE_M:		return GLCC_SCIENCE_M;
	case GLCI_SCIENCE_W:		return GLCC_SCIENCE_W;
	case GLCI_ASSASIN_M:		return GLCC_ASSASIN_M;
	case GLCI_ASSASIN_W:		return GLCC_ASSASIN_W;
	case GLCI_TESTING_M:		return GLCC_TESTING_M;
	case GLCI_TESTING_W:		return GLCC_TESTING_W;

	case GLCI_SHAPER_M:		return GLCC_SHAPER_M;
	case GLCI_SHAPER_W:		return GLCC_SHAPER_W;
		//
	};

	GASSERT(0&&"잘못된 케릭터 인덱스입니다.");
	return GLCC_FIGHTER_M;
}


EMCHARCLASS CharClassToSex ( const EMCHARCLASS emClass )
{
	switch (emClass)
	{
	case GLCC_FIGHTER_M:		return GLCC_FIGHTER_W;
	case GLCC_ARMS_M:			return GLCC_ARMS_W;
	case GLCC_ARCHER_W:		return GLCC_ARCHER_M;
	case GLCC_SPIRIT_W:		return GLCC_SPIRIT_M;
	case GLCC_EXTREME_M:	return GLCC_EXTREME_W;
	case GLCC_EXTREME_W:	return GLCC_EXTREME_M;
	
	case GLCC_FIGHTER_W:	return GLCC_FIGHTER_M;
	case GLCC_ARMS_W:		return GLCC_ARMS_M;
	case GLCC_ARCHER_M:		return GLCC_ARCHER_W;
	case GLCC_SPIRIT_M:		return GLCC_SPIRIT_W;

		//add class
	case GLCC_SCIENCE_M:		return GLCC_SCIENCE_W;
	case GLCC_SCIENCE_W:		return GLCC_SCIENCE_M;
	case GLCC_ASSASIN_M:		return GLCC_ASSASIN_W;
	case GLCC_ASSASIN_W:		return GLCC_ASSASIN_M;
	case GLCC_TESTING_M:		return GLCC_TESTING_W;
	case GLCC_TESTING_W:		return GLCC_TESTING_M;
	case GLCC_SHAPER_M:		return GLCC_SHAPER_W;
	case GLCC_SHAPER_W:		return GLCC_SHAPER_M;
		//
	};

	GASSERT(0&&"잘못된 케릭터 클래스입니다.");
	return GLCC_FIGHTER_M;
}

EMCHARINDEX CharIndexToSex ( const EMCHARINDEX emIndex )
{
	switch (emIndex)
	{
	case GLCI_FIGHTER_M:	return GLCI_FIGHTER_W;
	case GLCI_ARMS_M:		return GLCI_ARMS_W;
	case GLCI_ARCHER_W:		return GLCI_ARCHER_M;
	case GLCI_SPIRIT_W:		return GLCI_SPIRIT_M;

	case GLCI_EXTREME_M:	return GLCI_EXTREME_W;
	case GLCI_EXTREME_W:	return GLCI_EXTREME_M;

	case GLCI_FIGHTER_W:	return GLCI_SPIRIT_W;
	case GLCI_ARMS_W:		return GLCI_ARCHER_W;
	case GLCI_ARCHER_M:		return GLCI_ARMS_M;
	case GLCI_SPIRIT_M:		return GLCI_FIGHTER_M;

		//add class
	case GLCI_SCIENCE_M:		return GLCI_SCIENCE_W;
	case GLCI_SCIENCE_W:		return GLCI_SCIENCE_M;
	case GLCI_ASSASIN_M:		return GLCI_ASSASIN_W;
	case GLCI_ASSASIN_W:		return GLCI_ASSASIN_M;
	case GLCI_TESTING_M:		return GLCI_TESTING_W;
	case GLCI_TESTING_W:		return GLCI_TESTING_M;
	case GLCI_SHAPER_M:		return GLCI_SHAPER_W;
	case GLCI_SHAPER_W:		return GLCI_SHAPER_M;
		//
	};

	GASSERT(0&&"잘못된 케릭터 인덱스입니다.");
	return GLCI_FIGHTER_M;
}

VOID GetCharSkillClassIndex( const EMCHARCLASS emClass, WORD & wBeginIndex, WORD & wEndIndex )
{
	switch ( emClass )
	{
	case GLCC_FIGHTER_M:
	case GLCC_FIGHTER_W:
		{
			wBeginIndex = EMSKILL_FIGHTER_01;
			wEndIndex = EMSKILL_FIGHTER_04;
		}
		break;

	case GLCC_ARMS_M:
	case GLCC_ARMS_W:
		{
			wBeginIndex = EMSKILL_ARMS_01;
			wEndIndex = EMSKILL_ARMS_04;
		}
		break;

	case GLCC_ARCHER_M:
	case GLCC_ARCHER_W:
		{
			wBeginIndex = EMSKILL_ARCHER_01;
			wEndIndex = EMSKILL_ARCHER_04;
		}
		break;

	case GLCC_SPIRIT_M:
	case GLCC_SPIRIT_W:
		{
			wBeginIndex = EMSKILL_SPIRIT_01;
			wEndIndex = EMSKILL_SPIRIT_04;
		}
		break;	

	case GLCC_EXTREME_M:
	case GLCC_EXTREME_W:
		{
			wBeginIndex = EMSKILL_EXTREME_01;
			wEndIndex = EMSKILL_EXTREME_04;
		}
		break;

		//add class
	case GLCC_SCIENCE_M:
	case GLCC_SCIENCE_W:
		{
			wBeginIndex = EMSKILL_SCIENCE_01;
			wEndIndex = EMSKILL_SCIENCE_04;
		}
		break;
	case GLCC_ASSASIN_M:
	case GLCC_ASSASIN_W:
		{
			wBeginIndex = EMSKILL_ASSASIN_01;
			wEndIndex = EMSKILL_ASSASIN_04;
		}
		break;
	case GLCC_TESTING_M:
	case GLCC_TESTING_W:
		{
			wBeginIndex = EMSKILL_TESTING_01;
			wEndIndex = EMSKILL_TESTING_04;
		}
		break;
	case GLCC_SHAPER_M:
	case GLCC_SHAPER_W:
		{
			wBeginIndex = EMSKILL_SHAPER_01;
			wEndIndex = EMSKILL_SHAPER_04;
		}
		break;

	default:
		GASSERT(0&&"잘못된 케릭터 인덱스입니다.");
		break;
	};
}

namespace COMMENT
{
	std::string ATIONTYPE[GLAT_SIZE] =
	{
		"GLAT_IDLE",
		"GLAT_MOVE",
		"GLAT_ATTACK",
		"GLAT_SKILL",
		"GLAT_SHOCK",
		"GLAT_PUSHPULL",
		"GLAT_TALK",
		"GLAT_CONFT_END",
		"GLAT_FALLING",
		"GLAT_DIE"
	};

	std::string TRIBE[TRIBE_NSIZE] = 
	{
		"Human",
		"Monster",
		"Angel",
		"Devil",
		"Fairy",
		"Genie",
		"Dragon"
	};

	std::string MOVETYPE[MOVETYPE_NSIZE] = 
	{
		"Ground movement",
		"Air Moverment",
		"Aqua movement"
	};

	std::string ELEMENT[EMELEMENT_MAXNUM2] = 
	{
		"Vital Energy",
		"Fire",
		"Ice",
		"Eletrical energy",
		"Poison",
		"Petrified",
		"Confusion",
		"Fainting",
		"Malediction",
		"All belong to item"
	};

	std::string BRIGHT[BRIGHT_SIZE] =
	{
		"Good",
		"Evil",
		"(Neutral)"
	};

	std::string CHARCLASS[GLCI_NUM_2015] =
	{
		"Brawler(M)",
		"Swordsman(M)",
		"Archer(W)",
		"Shaman(W)",
		"Extreme(M)",
		"Extreme(W)",
		"Brawler(W)",
		"Swordsman(W)",
		"Archer(M)",
		"Shaman(M)",
		"Science(M)",//add class
		"Science(W)",
		"Assassin(M)",
		"Assassin(W)",
		"Magician(M)",
		"Magician(W)",
		//added by CNDev
		"Shaper(M)",
		"Shaper(W)",
	};

	std::string EMSCHOOL[GLSCHOOL_NUM] =
	{
		"school 1",
		"school 2",
		"school 3",
	};

	std::string CHARCOMMENT[GLCI_NUM_2015] = 
	{
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		//add class
		"",
		"",
		"",
		"",
		"",
		"",
		//added by CNDev
		"",
		""
	};

	std::string BLOW[EMBLOW_SIZE] =
	{
		"Without",
		"Paralysis",
		"Faint",
		"Petrochemical",
		"Burn",	
		"Freeze",
		"Chaos",
		"Poison",
		"Curse",
	};

	std::string BLOW_VAR1[EMBLOW_SIZE] =
	{
		"No",
		"Movement Speed Eate",
		"No",
		"Movement Speed Rate",
		"No",
		"Movement Speed Rate",
		"No",
		"No",
		"No",
	};

	float BLOW_VAR1_SCALE[EMBLOW_SIZE] =
	{
		1.0f,
		100.0f,	
		1.0f,
		100.0f,
		1.0f,
		100.0f,
		1.0f,
		1.0f,
		1.0f,
	};

	std::string BLOW_VAR2[EMBLOW_SIZE] =
	{
		"No",
		"Delay Rate",
		"No",
		"No",	
		"Continue Damage",
		"Attack Add Rate",
		"No",	
		"Continue Damage",
		"Continue Damage",
	};

	float BLOW_VAR2_SCALE[EMBLOW_SIZE] =
	{
		1.0f,
		100.0f,	
		1.0f,
		1.0f,
		1.0f,
		100.0f,	
		1.0f,
		1.0f,
		1.0f,
	};

	std::string IMPACT_ADDON[EIMPACTA_SIZE] =
	{
		"Without",
		"Hit Percentage Transformation Rate",
		"Avoid Change Rate",
		"Attack Change Amount",
		"Defend Change Amount",
		"HP Change Percentage",
		"MP Change Percentage",
		"SP Change Percentage",
		"HP+MP+SP Change Percentage",
		"Attack Change Percentage",
		"Defend Change Percentage",
		"CloseAttack Rate",
		"Shooting Rate",
		"Energy",
		"HP increasing Percentage",
		"MP Increasing Percentage",
		"SP Increasing Percentage",
		"Change Resistance",
		"Change Stats",
		"HP recovery amount +-",
		"MP recovery amount +-",
		"SP recovery amount +-",
		"CP recovery amount +-",
		"CP Auto +-",
		"CP Add",
		"CP Rate Increase",
	};

	float IMPACT_ADDON_SCALE[EIMPACTA_SIZE] =
	{
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		100.0f,
		100.0f,
		100.0f,
		100.0f,
		100.0f,
		100.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
	};

	std::string IMPACT_TAR[TAR_SIZE] =
	{
		"Self",
		"Target",
		"From Self until the Target",
		"Specified location",
		"Specified"
	};

	std::string IMPACT_REALM[REALM_SIZE] =
	{
		"Target Self",
		"Target Area",
		"Continuous Area Target",
		"Cone Target"
	};

	std::string IMPACT_SIDE[SIDE_SIZE] =
	{
		"Towards my side",
		"Towards Enemy",
		"Towards Everyone"
	};

	std::string SPEC_ADDON[EMSPECA_NSIZE] =
	{
		"None",
		"Pull/Push",
		"Reflex Damage",
		"Revive",
		"Steal Hp",
		"Steal MP",
		"Steal SP",
		"Share HP",
		"Share MP",
		"Share SP",
		"Prevent Abnormal Condition",
		"Recover Abnormal Condition",
		"Penetration",
		"Shooting Range",
		"Moving Speed",
		"Dash Attack",
		"Invisible",
		"Detect Invisible",
		"Attack Speed",
		"Skill Delay",
		"Crushing Blow Rate",
		"Melee Damage Absorption",
		"Magic Damage Absorption",
		"Reflex Melee Damage",
		"Reflex Magic Damage",
		"Remove Skill Effect",
		"Stun",
		"Active Type Skill",
		"Pull",
		"Push",
		"Continuous Damage",
		"(X)Curse",
		"(X)Change Attack Range",
		"(X)Change Coverage Area",
		"Prohibit Using Potion",
		"Prohibit Using Skill",
		"(X)Damage Absorption (ignore)",
		"(X)Item Drop Rate",
		"(X)Money Drop Rate",
		"(X)Get Exp Rate",
		"(X)Talk to Specific NPC",
		"(X)Specific Item Drop",
		"Teleport",
		"Position Shift", 
		"(X)Reinforcement",
		"(X)Virtual Image",
		"(X)Bus Stop",
		"(X)Duration-Increase",
		"(X)Stigma",
		"(X)Hwandok",
		"(X)When the shot off effect",
		"(X)Ring",
		"(X)Sudok",
		"(X)Immune",
		"(X)Combat",
		"(X)Combat Off",
		"(X)Fixed Target",
		"(X)Governance",
		"(X)Random Experience Acquisition Rate",
		"(X)Skill Link",
		"Attack Increase",
		"Attack Rate Increase",
		"Defense Increase",
		"Defense Rate Increase",
		"Accuracy Increase",
		"Accuracy Rate Increase",
		"Avoid Increase",
		"Avoid Rate Increase",
		"HP Increase",
		"MP Increase",
		"SP Increase",
		"CP Increase",
		"HP Recovery Rate",
		"MP Recovery Rate",
		"SP Recovery Rate",
		"HP/MP/SP Recovery Rate",
		"(X)attack surface",
		"(X)sp consumption",
		"(X)resistance",
		"(X)force (integer)",
		"(X)stamina (integer)",
		"(X)agility (integer)",
		"(X)strength (integer)",
		"(X)fighting value (integer)",
		"(X)shot value (integer)",
		"(X)horsepower value (integer)",
		"(X)potion increase hp (integer)",
		"(X)mp potion increase (integer)",
		"(X)sp potion increase (integer)",
		"(X)cp increasing (integer)",
		"(X)movement speed (integer)",
		"(X)damage (percentage)",
		"(X)limited field of view (percentage)",
		"(X)airborne",
		"(X)pet (page change)",
		"(X)pet (skill)",
		"(X)pet (target)",
		"(X)pet (retreat)",
		"(X)activate delay",
		"(X)replication effect",
		"(X)movement speed limit",
		"Crusing Blow Damage",
		"Critical Rate",
		"Critical Damage",
		"Remove Debuff",
		"(X)CombatOff",
		"(X)GetExp",
	};

	std::string SPEC_ADDON_VAR1[EMSPECA_NSIZE] =
	{
			"None",
	"Range",
	"None", 
	"Recovery Rate",
	"Absorb Rate", 
	"Absorb Rate", 
	"Absorb Rate", 
	"None", 
	"None", 
	"None", 
	"None", 
	"None", 
	"Increase Penetration", 
	"Attack Range", 
	"Moving Rate",
	"Range", 
	"Hiding Lv", 
	"Detect Lv",
	"Att-Speed Rate", 
	"Change Rate", 
	"Range", 
	"Absorb Rate", 
	"Absorb Rate", 
	"Reflex Rate",
	"Reflex Rate", 
	"None", 
	"None", 
	"Rate",
	"Rate",
	"Rate", 
	"Duration",
	"Reflect Rate",
	"Range",
	"Range",
	"None", 
	"None", 
	"Number of times", 
	"Increase Rate", 
	"Increase Rate",
	"Increase Rate",
	"None", 
	"None",
	"Probability",
	"Probability", 
	"Bang Probability",
	"Distance" 
	"Success", 
	"Decrease Rate 1", 
	"Distance effect", 
	"Transforming ID", 
	"Probability", 
	"None", 
	"Probability", 
	"Application type",
	"Probability", 
	"No", 
	"Probability",
	"Probability",
	"Index", 
	"Probability", 
	"Value", 
	"Rate", 
	"Value", 
	"Rate", 
	"Value", 
	"Rate", 
	"Value", 
	"Rate", 
	"Value", 
	"Value", 
	"Value", 
	"Value", 
	"Rate", 
	"Rate", 
	"Rate",
	"Rate", 
	"Value", 
	"Value", 
	"Value", 
	"Value", 
	"Value", 
	"Value", 
	"Value", 
	"Value", 
	"Value", 
	"Value", 
	"Value", 
	"Value", 
	"Value", 
	"Value", 
	"Value", 
	"Rate", 
	"Rate", 
	"time dependent", 
	"Default slot", 
	"Default slot", 
	"No", 
	"No", 
	"Skill Mid", 
	"Replicate", 
	"Minimum movement speed",
	"Value",
	"Rate",
	"Value",
	"None",
	"None",
	"None",
	};

	float SPEC_ADDON_VAR1_SCALE[EMSPECA_NSIZE] =
	{
		1.00, 
		0.10,
		1.00, 
		100.00, 
		100.00, 
		100.00, 
		100.00, 
		1.00, 
		1.00, 
		1.00, 
		1.00, 
		1.00, 
		1.00, 
		1.00, 
		1.00, 
		1.00, 
		1.00, 
		1.00, 
		1.00, 
		1.00,
		0.10, 
		1.00, 
		1.00, 
		1.00, 
		1.00, 
		0.00, 
		0.00, 
		1.00, 
		1.00,
		1.00, 
		1.00,
		100.00, 
		0.10, 
		1.00, 
		1.00, 
		1.00, 
		1.00, 
		100.00, 
		100.00, 
		100.00, 
		1.00, 
		1.00, 
		1.00, 
		1.00,
		100.00, 
		1, 
		100.00, 
		1.00, 
		1.00,
		0.00,
		100.00, 
		0.00, 
		100.00, 
		1.00,
		100.00, 
		0.00, 
		1.00, 
		100.00, 
		1.00, 
		1.00, 
		0.00, 
		1.00, 
		0.00, 
		1.00, 
		0.00,
		1.00, 
		0.00, 
		1.00, 
		0.00, 
		0.00,
		0.00, 
		0.00, 
		1.00, 
		1.00, 
		1.00,
		1.00, 
		0.00, 
		0.00, 
		0.00, 
		0.00,
		0.00, 
		0.00, 
		0.00, 
		0.00,
		0.00,
		0.00, 
		0.00,
		0.00, 
		0.00, 
		0.00, 
		0.00, 
		1.00, 
		1.00, 
		1.00,
		10.00, 
		10.00, 
		0.00, 
		0.00, 
		0.00, 
		1.00, 
		100.00, 
		1.00,
		0.10,
		1.00,
		0.00,
		0.00,
		1.00,
	};

	std::string SPEC_ADDON_VAR2[EMSPECA_NSIZE] =
	{
		"None",
"Chance",
"None",
"None",
"None",
"None",
"None",
"None",
"None",
"None",
"None",
"None",
"None",
"None",
"None",
"Speed",
"Animation",
"Range",
"None",
"Rate",
"Chance",
"None",
"None",
"Chance",
"Chance",
"None",
"None",
"None",
"Moving Speed",
"Range",
"Damage",
"None",
"None",
"None",
"None",
"None",
"absorption",
"None",
"None",
"None",
"None",
"None",
"No",
"Speed",
"Damage Increase",
"number of times", 
"continue",
"increase and decrease ratio 2",
"Value ratio",
"emoticons ID", 
"additional release conditions",
"No",
"No",
"type behavior",
"duration",
"No",
"Effective range",
"range",
"overlapping count",
"No",
"No",
"No",
"No",
"No",
"No", 
"No",
"No",
"range",
"No",
"No", 
"No",
"No",
"No",
"No",
"No",
"No",
"No",
"range",
"No",
"No", 
"No",
"No",
"No",
"No", 
"No",
"No",
"range",
"No",
"No",
"No",
"No",
"No",
"No",
"Height",
"No",
"No",
"No",
"No",
"skill Sid",
"Attack succession", 
"highest movement speed",
"None",
"None",
"None",
"None",
"None",
"None",
	};

	float SPEC_ADDON_VAR2_SCALE[EMSPECA_NSIZE] =
	{
1.00,
1.00,
1.00,
1.00,
1.00,
1.00,
1.00,
1.00,
1.00,
1.00,
1.00,
1.00,
1.00,
1.00,
1.00,
1.00,
1.00,
1.00,
1.00,
1.00,
1.00,
1.00,
1.00,
1.00,
1.00,
1.00,
1.00,
1.00,
1.00,
1.00,
1.00,
1.00,
1.00,
1.00,
1.00,
1.00,
1.00,
1.00,
1.00,
1.00,
1.00,
1.00,
1.00,
1.00,
100.00,
1,
1.00,
1.00,
100.00,
0.00,
0.00,
0.00,
0.00,
1.00,
1.00,
0.00,
1.00,
1.00,
1.00,
0.00,
0.00,
0.00,
0.00,
0.00,
0.00,
0.00,
0.00,
0.00,
0.00,
0.00,
0.00,
0.00,
0.00,
0.00,
0.00,
0.00,
0.00,
0.00,
0.00,
0.00,
0.00,
0.00,
0.00,
0.00,
0.00,
0.00,
0.00,
0.00,
0.00,
0.00,
0.00,
0.00,
0.00,
10.00,
0.00,
0.00,
0.00,
0.00,
0.00,
10.00,
100.00,
1.00,
1.00,
1.00,
1.00,
0.00,
0.00,
	};

	std::string SKILLCLASS[EMSKILLCLASS_NSIZE] =
	{
			"Brawler-class1",
			"Brawler-class2",
			"Brawler-class3",
			"Brawler-class4",
			"Sword-class1",
			"Sword-class2",
			"Sword-class3",
			"Sword -class4",
			"Archery-class1",
			"Archery-class2",
			"Archery-class3",
			"Archery-class4",
			"Shaman-class1",
			"Shaman-class2",
			"Shaman-class3",
			"Shaman-class4",
			"NPC_01",
			"NPC_02",
			"NPC_03",
			"NPC_04",
			"NPC_05",
			"NPC_06",
			"NPC_07",
			"NPC_08",
			"NPC_09",
			"NPC_10",
			"NPC_11",
			"NPC_12",
			"NPC_13",
			"NPC_14",
			"New-class1",
			"New-class2",
			"New-class3",
			"New-class4",
			"NPC_19",
			"NPC_20",
			"Science-class1",
			"Science-class2",
			"Science-class3",
			"Science-class4",
			"Food(Lunch)",
			"Summon",
			"Magician's Animal",
			"Assassin-class1",
			"Assassin-class2",
			"Assassin-class3",
			"Assassin-class4",
			"ETC-class1",
			"ETC-class2",
			"ETC-class3",
			"ETC-class4",
			"CaptureTheField",
			"Event",
			"ETC",
			"Test",
			"Magician-Multi",
			"Magician-class1",
			"Magician-class2",
			"Magician-class3",
			"Magician-class4",
			//added by CNDev
			"Shaper-Multi",
			"Shaper-class1",
			"Shaper-class2",
			"Shaper-class3",
			"Shaper-class4"
	};

	std::string LANDEFFECT_TYPE[EMLANDEFFECT_SIZE] =
	{
		"Attackspeed",
		"Movespeed",
		"HPChangerate",
		"MPChangerate",
		"Recoveryrate",
		"DamageChangerate",
		"DefenseChangerate",
		"Resistance",
		"CancelAllBuff",
		"SafeZone"
	};

	std::string CONFT_BET_TYPE[EMCONFT_BET_SIZE] = //add duel bet
	{
		"None",
		"Use Gold",
		"Use Vote Point",
		"Use Prem. Point"
	};
};