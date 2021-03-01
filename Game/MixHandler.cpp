/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "MixHandler.cpp"
*
*/

ChaosMixHandler ChaosMixTable[] =
{
	{1,	"[ Chaos Weapon ]",			InterfaceData::ChaosMachine,		&MixHandler::ChaosMixWeapon				, false, 0 }, //1
	{2,	"[ Devil Square ]",			InterfaceData::ChaosMachine,		&MixHandler::ChaosMixDevilSquare		, true, 8 }, //2
	{3,	"[ Upgrade +10 ]",			InterfaceData::ChaosMachine,		&MixHandler::ChaosMixUpgrade10			, false, 0 }, //3
	{4,	"[ Upgrade +11 ]",			InterfaceData::ChaosMachine,		&MixHandler::ChaosMixUpgrade11			, false, 0 }, //4
	{5,	"[ Dinorant ]",				InterfaceData::ChaosMachine,		&MixHandler::ChaosMixDinorant			, true, 2 }, //5
	{6,	"[ Stat Fruit ]",			InterfaceData::ChaosMachine,		&MixHandler::ChaosMixStatFruit			, true, 16 }, //6
	{7,	"[ Wing Second ]",			InterfaceData::ChaosMachine,		&MixHandler::ChaosMixWingSecond			, false, 0 }, //7
	{8,	"[ Blood Castle ]",			InterfaceData::ChaosMachine,		&MixHandler::ChaosMixBloodCastle		, true, 6 }, //8
	{11,	"[ Wing First ]",			InterfaceData::ChaosMachine,		&MixHandler::ChaosMixWingFirst			, false, 0 }, //11
	{12,	"[ Unknown_4 ]",			InterfaceData::None,				&MixHandler::ChaosMixNone				, false, 0 }, //12
	{13,	"[ Dark Horse ]",			InterfaceData::PetTrainer,			&MixHandler::ChaosMixDarkHorse			, false, 0 }, //13
	{14,	"[ Dark Spirit ]",			InterfaceData::PetTrainer,			&MixHandler::ChaosMixDarkSpirit			, false, 0 }, //14
	{15,	"[ Bless Potion ]",			InterfaceData::ChaosMachine,		&MixHandler::ChaosMixBlessPotion		, true, 25 }, //15
	{16,	"[ Soul Potion ]",			InterfaceData::ChaosMachine,		&MixHandler::ChaosMixSoulPotion			, true, 25 }, //16
	{17,	"[ Life Stone ]",			InterfaceData::ChaosMachine,		&MixHandler::ChaosMixLifeStone			, false, 0 }, //17
	{18,	"[ Siege Lord ]",			InterfaceData::LordMix,				&MixHandler::ChaosMixSiegeLord			, false, 0 }, //18
	{22,	"[ Upgrade +12 ]",			InterfaceData::ChaosMachine,		&MixHandler::ChaosMixUpgrade12			, false, 0 }, //22
	{23,	"[ Upgrade +13 ]",			InterfaceData::ChaosMachine,		&MixHandler::ChaosMixUpgrade13			, false, 0 }, //23
	{24,	"[ Cloak ]",				InterfaceData::ChaosMachine,		&MixHandler::ChaosMixCloak				, false, 0 }, //24
	{25,	"[ Fenrir Fragment ]",		InterfaceData::ChaosMachine,		&MixHandler::ChaosMixFenrirFragment		, false, 0 }, //25
	{26,	"[ Fenrir Broken Horn ]",	InterfaceData::ChaosMachine,		&MixHandler::ChaosMixFenrirBrokenHorn	, false, 0 }, //26
	{27,	"[ Fenrir ]",				InterfaceData::ChaosMachine,		&MixHandler::ChaosMixFenrir				, false, 0 }, //27
	{28,	"[ Fenrir Upgrade ]",		InterfaceData::ChaosMachine,		&MixHandler::ChaosMixFenrirUpgrade		, false, 0 }, //28
	{30,	"[ SD Lvl1 ]",				InterfaceData::ChaosMachine,		&MixHandler::ChaosMixSDLvl1				, true, 32 }, //30
	{31,	"[ SD Lvl2 ]",				InterfaceData::ChaosMachine,		&MixHandler::ChaosMixSDLvl2				, true, 32 }, //31
	{32,	"[ SD Lvl3 ]",				InterfaceData::ChaosMachine,		&MixHandler::ChaosMixSDLvl3				, true, 32 }, //32
	{33,	"[ Harmony Refine ]",		InterfaceData::HarmonyRefine,		&MixHandler::ChaosMixHarmonyRefine		, true, 32 }, //33
	{34,	"[ Harmony Refine Item ]",	InterfaceData::HarmonySmelt,		&MixHandler::ChaosMixHarmonyRefineItem	, true, 8 }, //34
	{35,	"[ Harmony Restore Item ]",	InterfaceData::HarmonyRestore,		&MixHandler::ChaosMixHarmonyRestoreItem	, false, 0 }, //35
	{36,	"[ 380 Refine ]",			InterfaceData::ChaosMachine,		&MixHandler::ChaosMix380				, false, 0 }, //36
	{37,	"[ Illusion Temple ]",		InterfaceData::ChaosMachine,		&MixHandler::ChaosMixIllusionTemple		, false, 0 }, //37
	{38,	"[ Condor Feather ]",		InterfaceData::ChaosMachine,		&MixHandler::ChaosMixCondorFeather		, false, 0 }, //38
	{39,	"[ Wing Third ]",			InterfaceData::ChaosMachine,		&MixHandler::ChaosMixWingThird			, false, 0 }, //39
	{40,	"[ Chaos Card ]",			InterfaceData::ChaosCard,			&MixHandler::ChaosMixChaosCard			, false, 0 }, //40
	{41,	"[ Cherry Blossom ]",		InterfaceData::ChaosMachine,		&MixHandler::ChaosMixCherryBlossom		, false, 0 }, //41
	{46,	"[ Secromicon ]",			InterfaceData::ChaosMachine,		&MixHandler::ChaosMixSecromicon			, false, 0 }, //46
	{47,	"[ Sealed Box ]",			InterfaceData::ChaosMachine,		&MixHandler::ChaosMixSealedBox			, true, 16 }, //47
	{48,	"[ Summon Scroll ]",		InterfaceData::ChaosMachine,		&MixHandler::ChaosMixSummonScroll		, false, 0 }, //48
	{49,	"[ Upgrade +14 ]",			InterfaceData::ChaosMachine,		&MixHandler::ChaosMixUpgrade14			, false, 0 }, //49
	{50,	"[ Upgrade +15 ]",			InterfaceData::ChaosMachine,		&MixHandler::ChaosMixUpgrade15			, false, 0 }, //50
	{51,	"[ Lucky Ticket ]",			InterfaceData::Lucky,				&MixHandler::ChaosMixLuckyTicket		, false, 0 }, //51
	{52,	"[ Extension Jewel ]",		InterfaceData::Lucky,				&MixHandler::ChaosMixExtensionJewel		, false, 0 }, //52
	{54,	"[ Arka War Booty ]",		InterfaceData::ArkaWarExchange,		&MixHandler::ChaosMixArkaWarBooty		, false, 0 }, //54
	{55,	"[ Arka War Sign ]",		InterfaceData::ArkaWarSignRegister,	&MixHandler::ChaosMixArkaWarSign		, false, 0 }, //55
	{56,	"[ Monster Wing ]",			InterfaceData::ChaosMachine,		&MixHandler::ChaosMixMonsterWing		, false, 0 }, //56
	{75,	"[ Conqueror Wings ]",			InterfaceData::ChaosMachine,		&MixHandler::ChaosMixConquerorWings		, false, 0 }, //75
	{76,	"[ Dark Transformation Ring ]",	InterfaceData::ChaosMachine,		&MixHandler::ChaosMixDarkTransformationRing		, false, 0 }, //76
	{77,	"[ Ancient Hero Soul ]",			InterfaceData::ChaosMachine,		&MixHandler::ChaosMixAncientHeroSoul		, false, 0 }, //77
	{78,	"[ Sealed Armor Fury ]",			InterfaceData::ChaosMachine,		&MixHandler::ChaosMixSealedMasteryFinal	, false, 0 }, //78
	{79,	"[ Sealed Helm Fury ]",				InterfaceData::ChaosMachine,		&MixHandler::ChaosMixSealedMasteryFinal	, false, 0 }, //79
	{80,	"[ Sealed Pants Fury ]",			InterfaceData::ChaosMachine,		&MixHandler::ChaosMixSealedMasteryFinal	, false, 0 }, //80
	{81,	"[ Sealed Boots Fury ]",			InterfaceData::ChaosMachine,		&MixHandler::ChaosMixSealedMasteryFinal	, false, 0 }, //81
	{ 82, "[ Sealed Rune Wizard ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixSealedMasteryFinal, false, 0 }, //82
	{ 83, "[ Sealed Rune Wizard ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixSealedMasteryFinal, false, 0 }, //83
	{ 84, "[ Sealed Rune Wizard ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixSealedMasteryFinal, false, 0 }, //84
	{ 85, "[ Sealed Rune Wizard ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixSealedMasteryFinal, false, 0 }, //85
	{86,	"[ Sealed Armor Trascendence ]",	InterfaceData::ChaosMachine,		&MixHandler::ChaosMixSealedMasteryFinal	, false, 0 }, //86
	{87,	"[ Sealed Helm Trascendence ]",		InterfaceData::ChaosMachine,		&MixHandler::ChaosMixSealedMasteryFinal	, false, 0 }, //87
	{88,	"[ Sealed Pants Trascendence ]",	InterfaceData::ChaosMachine,		&MixHandler::ChaosMixSealedMasteryFinal	, false, 0 }, //88
	{89,	"[ Sealed Gloves Trascendence ]",	InterfaceData::ChaosMachine,		&MixHandler::ChaosMixSealedMasteryFinal	, false, 0 }, //89
	{90,	"[ Sealed Armor Flurry ]",			InterfaceData::ChaosMachine,		&MixHandler::ChaosMixSealedMasteryFinal	, false, 0 }, //90
	{91,	"[ Sealed Helm Flurry ]",			InterfaceData::ChaosMachine,		&MixHandler::ChaosMixSealedMasteryFinal	, false, 0 }, //91
	{92,	"[ Sealed Pants Flurry ]",			InterfaceData::ChaosMachine,		&MixHandler::ChaosMixSealedMasteryFinal	, false, 0 }, //92
	{93,	"[ Sealed Gloves Flurry ]",			InterfaceData::ChaosMachine,		&MixHandler::ChaosMixSealedMasteryFinal	, false, 0 }, //93
	{98,	"[ Sealed Armor Extremity ]",		InterfaceData::ChaosMachine,		&MixHandler::ChaosMixSealedMasteryFinal	, false, 0 }, //98
	{99,	"[ Sealed Boots Extremity ]",		InterfaceData::ChaosMachine,		&MixHandler::ChaosMixSealedMasteryFinal	, false, 0 }, //99
	{100,	"[ Sealed Pants Extremity ]",		InterfaceData::ChaosMachine,		&MixHandler::ChaosMixSealedMasteryFinal	, false, 0 }, //100
	{101,	"[ Sealed Gloves Extremity ]",		InterfaceData::ChaosMachine,		&MixHandler::ChaosMixSealedMasteryFinal	, false, 0 }, //101
	{106,	"[ Sealed Armor Conquest ]",		InterfaceData::ChaosMachine,		&MixHandler::ChaosMixSealedMasteryFinal	, false, 0 }, //106
	{107,	"[ Sealed Helm Conquest ]",			InterfaceData::ChaosMachine,		&MixHandler::ChaosMixSealedMasteryFinal	, false, 0 }, //107
	{108,	"[ Sealed Pants Conquest ]",		InterfaceData::ChaosMachine,		&MixHandler::ChaosMixSealedMasteryFinal	, false, 0 }, //108
	{109,	"[ Sealed Gloves Conquest ]",		InterfaceData::ChaosMachine,		&MixHandler::ChaosMixSealedMasteryFinal	, false, 0 }, //109
	{ 110, "[ Sealed Slayer ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixSealedMasteryFinal, false, 0 }, //110
	{ 111, "[ Sealed Slayer ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixSealedMasteryFinal, false, 0 }, //111
	{ 112, "[ Sealed Slayer ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixSealedMasteryFinal, false, 0 }, //112
	{ 113, "[ Sealed Slayer ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixSealedMasteryFinal, false, 0 }, //113
	{114,	"[ Sealed Armor Honor ]",			InterfaceData::ChaosMachine,		&MixHandler::ChaosMixSealedMasteryFinal	, false, 0 }, //114
	{115,	"[ Sealed Helm Honor ]",			InterfaceData::ChaosMachine,		&MixHandler::ChaosMixSealedMasteryFinal	, false, 0 }, //115
	{116,	"[ Sealed Pants Honor ]",			InterfaceData::ChaosMachine,		&MixHandler::ChaosMixSealedMasteryFinal	, false, 0 }, //116
	{117,	"[ Sealed Gloves Honor ]",			InterfaceData::ChaosMachine,		&MixHandler::ChaosMixSealedMasteryFinal	, false, 0 }, //117
	{118,	"[ Sealed Armor Destruction ]",		InterfaceData::ChaosMachine,		&MixHandler::ChaosMixSealedMasteryFinal	, false, 0 }, //118
	{119,	"[ Sealed Helm Destruction ]",		InterfaceData::ChaosMachine,		&MixHandler::ChaosMixSealedMasteryFinal	, false, 0 }, //119
	{120,	"[ Sealed Pants Destruction ]",		InterfaceData::ChaosMachine,		&MixHandler::ChaosMixSealedMasteryFinal	, false, 0 }, //120
	{121,	"[ Sealed Boots Destruction ]",		InterfaceData::ChaosMachine,		&MixHandler::ChaosMixSealedMasteryFinal	, false, 0 }, //121
	{126,	"[ Sealed Armor Tenacity ]",		InterfaceData::ChaosMachine,		&MixHandler::ChaosMixSealedMasteryFinal	, false, 0 }, //126
	{127,	"[ Sealed Helm Tenacity ]",			InterfaceData::ChaosMachine,		&MixHandler::ChaosMixSealedMasteryFinal	, false, 0 }, //127
	{128,	"[ Sealed Pants Tenacity ]",		InterfaceData::ChaosMachine,		&MixHandler::ChaosMixSealedMasteryFinal	, false, 0 }, //128
	{129,	"[ Sealed Gloves Tenacity ]",		InterfaceData::ChaosMachine,		&MixHandler::ChaosMixSealedMasteryFinal	, false, 0 }, //129
	{134,	"[ Archangels Hammer ]",					InterfaceData::ChaosMachine,	&MixHandler::ChaosMixArchangelsHammer , false, 0 }, //134
	{135,	"[ Upgrade Divine Archangel Weapon ]",		InterfaceData::ChaosMachine,	&MixHandler::ChaosMixUpgradeDivineArchangelWeapon , false, 0 }, //135
	{136,	"[ Upgrade Bloodangel Helm Fury ]",			InterfaceData::ChaosMachine,	&MixHandler::ChaosMixBloodangelToDarkangelSet , false, 0 },
	{137,	"[ Upgrade Bloodangel Helm Trascendence ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixBloodangelToDarkangelSet , false, 0 }, //137
	{138,	"[ Upgrade Bloodangel Helm Flurry ]",		InterfaceData::ChaosMachine,	&MixHandler::ChaosMixBloodangelToDarkangelSet , false, 0 }, //138
	{139,	"[ Upgrade Bloodangel Helm Conquest ]",		InterfaceData::ChaosMachine,	&MixHandler::ChaosMixBloodangelToDarkangelSet , false, 0 }, //139
	{140,	"[ Upgrade Bloodangel Helm Honor ]",		InterfaceData::ChaosMachine,	&MixHandler::ChaosMixBloodangelToDarkangelSet , false, 0 }, //140
	{141,	"[ Upgrade Bloodangel Helm Destruction ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixBloodangelToDarkangelSet , false, 0 }, //141
	{142,	"[ Upgrade Bloodangel Helm Tenacity ]",		InterfaceData::ChaosMachine,	&MixHandler::ChaosMixBloodangelToDarkangelSet , false, 0 }, //142
	{143,	"[ Upgrade Bloodangel Armor Fury ]",		InterfaceData::ChaosMachine,	&MixHandler::ChaosMixBloodangelToDarkangelSet , false, 0 }, //143
	{144,	"[ Upgrade Bloodangel Armor Trascendence ]",InterfaceData::ChaosMachine,	&MixHandler::ChaosMixBloodangelToDarkangelSet , false, 0 }, //144
	{145,	"[ Upgrade Bloodangel Armor Flurry ]",		InterfaceData::ChaosMachine,	&MixHandler::ChaosMixBloodangelToDarkangelSet , false, 0 }, //145
	{146,	"[ Upgrade Bloodangel Armor Extremity ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixBloodangelToDarkangelSet , false, 0 }, //146
	{147,	"[ Upgrade Bloodangel Armor Conquest ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixBloodangelToDarkangelSet , false, 0 }, //147
	{148,	"[ Upgrade Bloodangel Armor Honor ]",		InterfaceData::ChaosMachine,	&MixHandler::ChaosMixBloodangelToDarkangelSet , false, 0 }, //148
	{149,	"[ Upgrade Bloodangel Armor Destruction ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixBloodangelToDarkangelSet , false, 0 }, //149
	{150,	"[ Upgrade Bloodangel Armor Tenacity ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixBloodangelToDarkangelSet , false, 0 }, //150
	{151,	"[ Upgrade Bloodangel Pants Fury ]",		InterfaceData::ChaosMachine,	&MixHandler::ChaosMixBloodangelToDarkangelSet , false, 0 }, //151
	{152,	"[ Upgrade Bloodangel Pants Trascendence ]",InterfaceData::ChaosMachine,	&MixHandler::ChaosMixBloodangelToDarkangelSet , false, 0 }, //152
	{153,	"[ Upgrade Bloodangel Pants Flurry ]",		InterfaceData::ChaosMachine,	&MixHandler::ChaosMixBloodangelToDarkangelSet , false, 0 }, //153
	{154,	"[ Upgrade Bloodangel Pants Extremity ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixBloodangelToDarkangelSet , false, 0 }, //154
	{155,	"[ Upgrade Bloodangel Pants Conquest ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixBloodangelToDarkangelSet , false, 0 }, //155
	{156,	"[ Upgrade Bloodangel Pants Honor ]",		InterfaceData::ChaosMachine,	&MixHandler::ChaosMixBloodangelToDarkangelSet , false, 0 }, //156
	{157,	"[ Upgrade Bloodangel Pants Destruction ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixBloodangelToDarkangelSet , false, 0 }, //157
	{158,	"[ Upgrade Bloodangel Pants Tenacity ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixBloodangelToDarkangelSet , false, 0 }, //158
	{159,	"[ Upgrade Bloodangel Gloves Trascendence ]",InterfaceData::ChaosMachine,	&MixHandler::ChaosMixBloodangelToDarkangelSet , false, 0 }, //159
	{160,	"[ Upgrade Bloodangel Gloves Flurry ]",		InterfaceData::ChaosMachine,	&MixHandler::ChaosMixBloodangelToDarkangelSet , false, 0 }, //160
	{161,	"[ Upgrade Bloodangel Gloves Extremity ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixBloodangelToDarkangelSet , false, 0 }, //161
	{162,	"[ Upgrade Bloodangel Gloves Conquest ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixBloodangelToDarkangelSet , false, 0 }, //162
	{163,	"[ Upgrade Bloodangel Gloves Honor ]",		InterfaceData::ChaosMachine,	&MixHandler::ChaosMixBloodangelToDarkangelSet , false, 0 }, //163
	{164,	"[ Upgrade Bloodangel Gloves Tenacity ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixBloodangelToDarkangelSet , false, 0 }, //164
	{165,	"[ Upgrade Bloodangel Boots Fury ]",		InterfaceData::ChaosMachine,	&MixHandler::ChaosMixBloodangelToDarkangelSet , false, 0 }, //165
	{166,	"[ Upgrade Bloodangel Boots Extremity ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixBloodangelToDarkangelSet , false, 0 }, //166
	{167,	"[ Upgrade Bloodangel Boots Destruction ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixBloodangelToDarkangelSet , false, 0 }, //167
	{168,	"[ Upgrade Darkangel to Holyangel Set ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixUpgradeDarkangelToHolyangelSet , false, 0 }, //168
	{169,	"[ Upgrade Darkangel to Holyangel Set ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixUpgradeDarkangelToHolyangelSet , false, 0 }, //169
	{170,	"[ Upgrade Darkangel to Holyangel Set ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixUpgradeDarkangelToHolyangelSet , false, 0 }, //170
	{171,	"[ Upgrade Darkangel to Holyangel Set ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixUpgradeDarkangelToHolyangelSet , false, 0 }, //171
	{172,	"[ Upgrade Darkangel to Holyangel Set ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixUpgradeDarkangelToHolyangelSet , false, 0 }, //172
	{173,	"[ Upgrade Darkangel to Holyangel Set ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixUpgradeDarkangelToHolyangelSet , false, 0 }, //173
	{174,	"[ Upgrade Darkangel to Holyangel Set ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixUpgradeDarkangelToHolyangelSet , false, 0 }, //174
	{175,	"[ Upgrade Darkangel to Holyangel Set ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixUpgradeDarkangelToHolyangelSet , false, 0 }, //175
	{176,	"[ Upgrade Darkangel to Holyangel Set ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixUpgradeDarkangelToHolyangelSet , false, 0 }, //176
	{177,	"[ Upgrade Darkangel to Holyangel Set ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixUpgradeDarkangelToHolyangelSet , false, 0 }, //177
	{178,	"[ Upgrade Darkangel to Holyangel Set ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixUpgradeDarkangelToHolyangelSet , false, 0 }, //178
	{179,	"[ Upgrade Darkangel to Holyangel Set ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixUpgradeDarkangelToHolyangelSet , false, 0 }, //179
	{180,	"[ Upgrade Darkangel to Holyangel Set ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixUpgradeDarkangelToHolyangelSet , false, 0 }, //180
	{181,	"[ Upgrade Darkangel to Holyangel Set ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixUpgradeDarkangelToHolyangelSet , false, 0 }, //181
	{182,	"[ Upgrade Darkangel to Holyangel Set ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixUpgradeDarkangelToHolyangelSet , false, 0 }, //182
	{183,	"[ Upgrade Darkangel to Holyangel Set ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixUpgradeDarkangelToHolyangelSet , false, 0 }, //183
	{184,	"[ Upgrade Darkangel to Holyangel Set ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixUpgradeDarkangelToHolyangelSet , false, 0 }, //184
	{185,	"[ Upgrade Darkangel to Holyangel Set ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixUpgradeDarkangelToHolyangelSet , false, 0 }, //185
	{186,	"[ Upgrade Darkangel to Holyangel Set ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixUpgradeDarkangelToHolyangelSet , false, 0 }, //186
	{187,	"[ Upgrade Darkangel to Holyangel Set ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixUpgradeDarkangelToHolyangelSet , false, 0 }, //187
	{188,	"[ Upgrade Darkangel to Holyangel Set ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixUpgradeDarkangelToHolyangelSet , false, 0 }, //188
	{189,	"[ Upgrade Darkangel to Holyangel Set ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixUpgradeDarkangelToHolyangelSet , false, 0 }, //189
	{190,	"[ Upgrade Darkangel to Holyangel Set ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixUpgradeDarkangelToHolyangelSet , false, 0 }, //190
	{191,	"[ Upgrade Darkangel to Holyangel Set ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixUpgradeDarkangelToHolyangelSet , false, 0 }, //191
	{192,	"[ Upgrade Darkangel to Holyangel Set ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixUpgradeDarkangelToHolyangelSet , false, 0 }, //192
	{193,	"[ Upgrade Darkangel to Holyangel Set ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixUpgradeDarkangelToHolyangelSet , false, 0 }, //193
	{194,	"[ Upgrade Darkangel to Holyangel Set ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixUpgradeDarkangelToHolyangelSet , false, 0 }, //194
	{195,	"[ Upgrade Darkangel to Holyangel Set ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixUpgradeDarkangelToHolyangelSet , false, 0 }, //195
	{196,	"[ Upgrade Darkangel to Holyangel Set ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixUpgradeDarkangelToHolyangelSet , false, 0 }, //196
	{197,	"[ Upgrade Darkangel to Holyangel Set ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixUpgradeDarkangelToHolyangelSet , false, 0 }, //197
	{198,	"[ Upgrade Darkangel to Holyangel Set ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixUpgradeDarkangelToHolyangelSet , false, 0 }, //198
	{199,	"[ Upgrade Darkangel to Holyangel Set ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixUpgradeDarkangelToHolyangelSet , false, 0 }, //199
	{200,	"[ Upgrade Bloodangel to Darkangel Weapon ]",		InterfaceData::ChaosMachine,	&MixHandler::ChaosMixUpgradeBloodangelToDarkangelWeapon , false, 0 }, //200
	{201,	"[ Upgrade DarkAngel Weapon Magic Sword ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixUpgradeBloodangelToDarkangelWeapon , false, 0 }, //201
	{202,	"[ Upgrade DarkAngel Weapon Claw ]",		InterfaceData::ChaosMachine,	&MixHandler::ChaosMixUpgradeBloodangelToDarkangelWeapon , false, 0 }, //202
	{203,	"[ Upgrade DarkAngel Weapon Scepter ]",		InterfaceData::ChaosMachine,	&MixHandler::ChaosMixUpgradeBloodangelToDarkangelWeapon , false, 0 }, //203
	{204,	"[ Upgrade DarkAngel Weapon Lance ]",		InterfaceData::ChaosMachine,	&MixHandler::ChaosMixUpgradeBloodangelToDarkangelWeapon , false, 0 }, //204
	{205,	"[ Upgrade DarkAngel Weapon Bow ]",			InterfaceData::ChaosMachine,	&MixHandler::ChaosMixUpgradeBloodangelToDarkangelWeapon , false, 0 }, //205
	{206,	"[ Upgrade DarkAngel Weapon Staff ]",		InterfaceData::ChaosMachine,	&MixHandler::ChaosMixUpgradeBloodangelToDarkangelWeapon , false, 0 }, //206
	{207,	"[ Upgrade DarkAngel Weapon Stick ]",		InterfaceData::ChaosMachine,	&MixHandler::ChaosMixUpgradeBloodangelToDarkangelWeapon , false, 0 }, //207
	{208,	"[ Sealed BloodAngel Hero ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixSealedBloodangelHero , false, 0 }, //208
	{209,	"[ Sealed BloodAngel Hero ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixSealedBloodangelHero , false, 0 }, //209
	{210,	"[ Sealed BloodAngel Hero ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixSealedBloodangelHero , false, 0 }, //210
	{211,	"[ Sealed BloodAngel Hero ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixSealedBloodangelHero , false, 0 }, //211
	{212,	"[ Sealed BloodAngel Hero ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixSealedBloodangelHero , false, 0 }, //212
	{213,	"[ Sealed BloodAngel Hero ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixSealedBloodangelHero , false, 0 }, //213
	{214,	"[ Sealed BloodAngel Hero ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixSealedBloodangelHero , false, 0 }, //214
	{215,	"[ Sealed BloodAngel Hero ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixSealedBloodangelHero , false, 0 }, //215
	{216,	"[ Sealed BloodAngel Hero ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixSealedBloodangelHero , false, 0 }, //216
	{217,	"[ Sealed BloodAngel Hero ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixSealedBloodangelHero , false, 0 }, //217
	{218,	"[ Sealed BloodAngel Hero ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixSealedBloodangelHero , false, 0 }, //218
	{219,	"[ Sealed BloodAngel Hero ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixSealedBloodangelHero , false, 0 }, //219
	{220,	"[ Sealed BloodAngel Hero ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixSealedBloodangelHero , false, 0 }, //220
	{221,	"[ Sealed BloodAngel Hero ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixSealedBloodangelHero , false, 0 }, //221
	{222,	"[ Sealed BloodAngel Hero ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixSealedBloodangelHero , false, 0 }, //222
	{223,	"[ Sealed BloodAngel Hero ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixSealedBloodangelHero , false, 0 }, //223
	{224,	"[ Sealed BloodAngel Hero ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixSealedBloodangelHero , false, 0 }, //224
	{225,	"[ Sealed BloodAngel Hero ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixSealedBloodangelHero , false, 0 }, //225
	{226,	"[ Sealed BloodAngel Hero ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixSealedBloodangelHero , false, 0 }, //226
	{227,	"[ Sealed BloodAngel Hero ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixSealedBloodangelHero , false, 0 }, //227
	{228,	"[ Sealed BloodAngel Hero ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixSealedBloodangelHero , false, 0 }, //228
	{229,	"[ Sealed BloodAngel Hero ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixSealedBloodangelHero , false, 0 }, //229
	{230,	"[ Sealed BloodAngel Hero ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixSealedBloodangelHero , false, 0 }, //230
	{231,	"[ Sealed BloodAngel Hero ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixSealedBloodangelHero , false, 0 }, //231
	{232,	"[ Sealed BloodAngel Hero ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixSealedBloodangelHero , false, 0 }, //232
	{233,	"[ Sealed BloodAngel Hero ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixSealedBloodangelHero , false, 0 }, //233
	{234,	"[ Sealed BloodAngel Hero ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixSealedBloodangelHero , false, 0 }, //234
	{235,	"[ Sealed BloodAngel Hero ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixSealedBloodangelHero , false, 0 }, //235
	{236,	"[ Sealed BloodAngel Hero ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixSealedBloodangelHero , false, 0 }, //236
	{237,	"[ Sealed BloodAngel Hero ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixSealedBloodangelHero , false, 0 }, //237
	{238,	"[ Sealed BloodAngel Hero ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixSealedBloodangelHero , false, 0 }, //238
	{239,	"[ Sealed BloodAngel Hero ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixSealedBloodangelHero , false, 0 }, //239
	{240,	"[ Sealed BloodAngel Hero ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixSealedBloodangelHero , false, 0 }, //240
	{241,	"[ Sealed BloodAngel Hero ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixSealedBloodangelHero , false, 0 }, //241
	{242,	"[ Sealed BloodAngel Hero ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixSealedBloodangelHero , false, 0 }, //242
	{243,	"[ Sealed BloodAngel Hero ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixSealedBloodangelHero , false, 0 }, //243
	{244,	"[ Sealed BloodAngel Hero ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixSealedBloodangelHero , false, 0 }, //244
	{245,	"[ Sealed BloodAngel Hero ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixSealedBloodangelHero , false, 0 }, //245
	{246,	"[ Sealed BloodAngel Hero ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixSealedBloodangelHero , false, 0 }, //246
	{247,	"[ Sealed BloodAngel Hero ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixSealedBloodangelHero , false, 0 }, //247
	{248,	"[ Sealed BloodAngel Salvation ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixSealedMasteryFinal , false, 0 }, //248
	{249,	"[ Sealed BloodAngel Salvation ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixSealedMasteryFinal , false, 0 }, //249
	{250,	"[ Sealed BloodAngel Salvation ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixSealedMasteryFinal , false, 0 }, //250
	{251,	"[ Sealed BloodAngel Salvation ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixSealedMasteryFinal , false, 0 }, //251
	{252,	"[ Sealed BloodAngel Magical ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixSealedMasteryFinal , false, 0 }, //252
	{253,	"[ Sealed BloodAngel Magical ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixSealedMasteryFinal , false, 0 }, //253
	{254,	"[ Sealed BloodAngel Magical ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixSealedMasteryFinal , false, 0 }, //254
	{255,	"[ Sealed BloodAngel Magical ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixSealedMasteryFinal , false, 0 }, //255
	{256,	"[ Upgrade Bloodangel Armor Salvation ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixBloodangelToDarkangelSet , false, 0 },
	{257,	"[ Upgrade Bloodangel Helm Salvation ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixBloodangelToDarkangelSet , false, 0 },
	{258,	"[ Upgrade Bloodangel Pants Salvation ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixBloodangelToDarkangelSet , false, 0 },
	{259,	"[ Upgrade Bloodangel Gloves Salvation ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixBloodangelToDarkangelSet , false, 0 },
	{260,	"[ Upgrade Bloodangel Armor Magical ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixBloodangelToDarkangelSet , false, 0 },
	{261,	"[ Upgrade Bloodangel Pants Magical ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixBloodangelToDarkangelSet , false, 0 },
	{262,	"[ Upgrade Bloodangel Gloves Magical ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixBloodangelToDarkangelSet , false, 0 },
	{263,	"[ Upgrade Bloodangel Boots Magical ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixBloodangelToDarkangelSet , false, 0 },
	{264,	"[ Upgrade DarkAngel Armor Revival ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixUpgradeDarkangelToHolyangelSet , false, 0 },
	{265,	"[ Upgrade DarkAngel Helm Revival ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixUpgradeDarkangelToHolyangelSet , false, 0 },
	{266,	"[ Upgrade DarkAngel Pants Revival ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixUpgradeDarkangelToHolyangelSet , false, 0 },
	{267,	"[ Upgrade DarkAngel Gloves Revival ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixUpgradeDarkangelToHolyangelSet , false, 0 },
	{268,	"[ Upgrade DarkAngel Armor Fall ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixUpgradeDarkangelToHolyangelSet , false, 0 },
	{269,	"[ Upgrade DarkAngel Pants Fall ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixUpgradeDarkangelToHolyangelSet , false, 0 },
	{270,	"[ Upgrade DarkAngel Gloves Fall ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixUpgradeDarkangelToHolyangelSet , false, 0 },
	{271,	"[ Upgrade DarkAngel Boots Fall ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixUpgradeDarkangelToHolyangelSet , false, 0 },
	{272,	"[ Attack To Support Conversion (BA) ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixElfConversionAttackToSupport , false, 0 }, // Attack -> Support
	{273,	"[ Attack To Support Conversion (BA) ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixElfConversionAttackToSupport , false, 0 }, // Attack -> Support
	{274,	"[ Attack To Support Conversion (BA) ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixElfConversionAttackToSupport , false, 0 }, // Attack -> Support
	{275,	"[ Attack To Support Conversion (BA) ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixElfConversionAttackToSupport , false, 0 }, // Attack -> Support
	{276,	"[ Attack To Support Conversion (DA) ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixElfConversionAttackToSupport , false, 0 }, // Attack -> Support
	{277,	"[ Attack To Support Conversion (DA) ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixElfConversionAttackToSupport , false, 0 }, // Attack -> Support
	{278,	"[ Attack To Support Conversion (DA) ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixElfConversionAttackToSupport , false, 0 }, // Attack -> Support
	{279,	"[ Attack To Support Conversion (DA) ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixElfConversionAttackToSupport , false, 0 }, // Attack -> Support
	{280,	"[ Attack To Support Conversion (HA) ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixElfConversionAttackToSupport , false, 0 }, // Attack -> Support
	{281,	"[ Attack To Support Conversion (HA) ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixElfConversionAttackToSupport , false, 0 }, // Attack -> Support
	{282,	"[ Attack To Support Conversion (HA) ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixElfConversionAttackToSupport , false, 0 }, // Attack -> Support
	{283,	"[ Attack To Support Conversion (HA) ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixElfConversionAttackToSupport , false, 0 }, // Attack -> Support
	{284,	"[ Attack To Magic Conversion (BA) ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixMGConversionAttackToMagic , false, 0 }, // Attack -> Magic
	{285,	"[ Attack To Magic Conversion (BA) ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixMGConversionAttackToMagic , false, 0 }, // Attack -> Magic
	{286,	"[ Attack To Magic Conversion (BA) ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixMGConversionAttackToMagic , false, 0 }, // Attack -> Magic
	{287,	"[ Attack To Magic Conversion (BA) ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixMGConversionAttackToMagic , false, 0 }, // Attack -> Magic
	{288,	"[ Attack To Magic Conversion (DA) ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixMGConversionAttackToMagic , false, 0 }, // Attack -> Magic
	{289,	"[ Attack To Magic Conversion (DA) ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixMGConversionAttackToMagic , false, 0 }, // Attack -> Magic
	{290,	"[ Attack To Magic Conversion (DA) ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixMGConversionAttackToMagic , false, 0 }, // Attack -> Magic
	{291,	"[ Attack To Magic Conversion (DA) ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixMGConversionAttackToMagic , false, 0 }, // Attack -> Magic
	{292,	"[ Attack To Magic Conversion (HA) ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixMGConversionAttackToMagic , false, 0 }, // Attack -> Magic
	{293,	"[ Attack To Magic Conversion (HA) ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixMGConversionAttackToMagic , false, 0 }, // Attack -> Magic
	{294,	"[ Attack To Magic Conversion (HA) ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixMGConversionAttackToMagic , false, 0 }, // Attack -> Magic
	{295,	"[ Attack To Magic Conversion (HA) ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixMGConversionAttackToMagic , false, 0 }, // Attack -> Magic
	{296,	"[ Support to Attack Conversion (BA) ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixElfConversionSupportToAttack , false, 0 }, // Support -> Attack
	{297,	"[ Support to Attack Conversion (BA) ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixElfConversionSupportToAttack , false, 0 }, // Support -> Attack
	{298,	"[ Support to Attack Conversion (BA) ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixElfConversionSupportToAttack , false, 0 }, // Support -> Attack
	{299,	"[ Support to Attack Conversion (BA) ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixElfConversionSupportToAttack , false, 0 }, // Support -> Attack
	{300,	"[ Support to Attack Conversion (DA) ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixElfConversionSupportToAttack , false, 0 }, // Support -> Attack
	{301,	"[ Support to Attack Conversion (DA) ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixElfConversionSupportToAttack , false, 0 }, // Support -> Attack
	{302,	"[ Support to Attack Conversion (DA) ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixElfConversionSupportToAttack , false, 0 }, // Support -> Attack
	{303,	"[ Support to Attack Conversion (DA) ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixElfConversionSupportToAttack , false, 0 }, // Support -> Attack
	{304,	"[ Support to Attack Conversion (HA) ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixElfConversionSupportToAttack , false, 0 }, // Support -> Attack
	{305,	"[ Support to Attack Conversion (HA) ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixElfConversionSupportToAttack , false, 0 }, // Support -> Attack
	{306,	"[ Support to Attack Conversion (HA) ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixElfConversionSupportToAttack , false, 0 }, // Support -> Attack
	{307,	"[ Support to Attack Conversion (HA) ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixElfConversionSupportToAttack , false, 0 }, // Support -> Attack
	{308,	"[ Magic to Attack Conversion (BA) ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixMGConversionMagicToAttack , false, 0 }, // Magic -> Attack
	{309,	"[ Magic to Attack Conversion (BA) ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixMGConversionMagicToAttack , false, 0 }, // Magic -> Attack
	{310,	"[ Magic to Attack Conversion (BA) ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixMGConversionMagicToAttack , false, 0 }, // Magic -> Attack
	{311,	"[ Magic to Attack Conversion (BA) ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixMGConversionMagicToAttack , false, 0 }, // Magic -> Attack
	{312,	"[ Magic to Attack Conversion (DA) ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixMGConversionMagicToAttack , false, 0 }, // Magic -> Attack
	{313,	"[ Magic to Attack Conversion (DA) ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixMGConversionMagicToAttack , false, 0 }, // Magic -> Attack
	{314,	"[ Magic to Attack Conversion (DA) ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixMGConversionMagicToAttack , false, 0 }, // Magic -> Attack
	{315,	"[ Magic to Attack Conversion (DA) ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixMGConversionMagicToAttack , false, 0 }, // Magic -> Attack
	{316,	"[ Magic to Attack Conversion (HA) ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixMGConversionMagicToAttack , false, 0 }, // Magic -> Attack
	{317,	"[ Magic to Attack Conversion (HA) ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixMGConversionMagicToAttack , false, 0 }, // Magic -> Attack
	{318,	"[ Magic to Attack Conversion (HA) ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixMGConversionMagicToAttack , false, 0 }, // Magic -> Attack
	{319,	"[ Magic to Attack Conversion (HA) ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixMGConversionMagicToAttack , false, 0 }, // Magic -> Attack
	{320,	"[ Upgrade DarkAngel Weapon Quiver ]",	InterfaceData::ChaosMachine,	&MixHandler::ChaosMixUpgradeBloodangelToDarkangelWeapon , false, 0 }, //320
	{ 321, "[ Upgrade Holyangel to Awakening Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToAwakeningSet, false, 0 }, //321
	{ 322, "[ Upgrade Holyangel to Awakening Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToAwakeningSet, false, 0 }, //321
	{ 323, "[ Upgrade Holyangel to Awakening Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToAwakeningSet, false, 0 }, //321
	{ 324, "[ Upgrade Holyangel to Awakening Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToAwakeningSet, false, 0 }, //321
	{ 325, "[ Upgrade Holyangel to Awakening Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToAwakeningSet, false, 0 }, //321
	{ 326, "[ Upgrade Holyangel to Awakening Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToAwakeningSet, false, 0 }, //321
	{ 327, "[ Upgrade Holyangel to Awakening Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToAwakeningSet, false, 0 }, //321
	{ 328, "[ Upgrade Holyangel to Awakening Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToAwakeningSet, false, 0 }, //321
	{ 329, "[ Upgrade Holyangel to Awakening Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToAwakeningSet, false, 0 }, //321
	{ 330, "[ Upgrade Holyangel to Awakening Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToAwakeningSet, false, 0 }, //321
	{ 331, "[ Upgrade Holyangel to Awakening Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToAwakeningSet, false, 0 }, //321
	{ 332, "[ Upgrade Holyangel to Awakening Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToAwakeningSet, false, 0 }, //321
	{ 333, "[ Upgrade Holyangel to Awakening Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToAwakeningSet, false, 0 }, //321
	{ 334, "[ Upgrade Holyangel to Awakening Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToAwakeningSet, false, 0 }, //321
	{ 335, "[ Upgrade Holyangel to Awakening Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToAwakeningSet, false, 0 }, //321
	{ 336, "[ Upgrade Holyangel to Awakening Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToAwakeningSet, false, 0 }, //321
	{ 337, "[ Upgrade Holyangel to Awakening Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToAwakeningSet, false, 0 }, //321
	{ 338, "[ Upgrade Holyangel to Awakening Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToAwakeningSet, false, 0 }, //321
	{ 339, "[ Upgrade Holyangel to Awakening Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToAwakeningSet, false, 0 }, //321
	{ 340, "[ Upgrade Holyangel to Awakening Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToAwakeningSet, false, 0 }, //321
	{ 341, "[ Upgrade Holyangel to Awakening Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToAwakeningSet, false, 0 }, //321
	{ 342, "[ Upgrade Holyangel to Awakening Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToAwakeningSet, false, 0 }, //321
	{ 343, "[ Upgrade Holyangel to Awakening Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToAwakeningSet, false, 0 }, //321
	{ 344, "[ Upgrade Holyangel to Awakening Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToAwakeningSet, false, 0 }, //321
	{ 345, "[ Upgrade Holyangel to Awakening Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToAwakeningSet, false, 0 }, //321
	{ 346, "[ Upgrade Holyangel to Awakening Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToAwakeningSet, false, 0 }, //321
	{ 347, "[ Upgrade Holyangel to Awakening Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToAwakeningSet, false, 0 }, //321
	{ 348, "[ Upgrade Holyangel to Awakening Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToAwakeningSet, false, 0 }, //321
	{ 349, "[ Upgrade Holyangel to Awakening Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToAwakeningSet, false, 0 }, //321
	{ 350, "[ Upgrade Holyangel to Awakening Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToAwakeningSet, false, 0 }, //321
	{ 351, "[ Upgrade Holyangel to Awakening Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToAwakeningSet, false, 0 }, //321
	{ 352, "[ Upgrade Holyangel to Awakening Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToAwakeningSet, false, 0 }, //321
	{ 353, "[ Upgrade Holyangel to Awakening Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToAwakeningSet, false, 0 }, //321
	{ 354, "[ Upgrade Holyangel to Awakening Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToAwakeningSet, false, 0 }, //321
	{ 355, "[ Upgrade Holyangel to Awakening Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToAwakeningSet, false, 0 }, //321
	{ 356, "[ Upgrade Holyangel to Awakening Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToAwakeningSet, false, 0 }, //321
	{ 357, "[ Upgrade Holyangel to Awakening Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToAwakeningSet, false, 0 }, //321
	{ 358, "[ Upgrade Holyangel to Awakening Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToAwakeningSet, false, 0 }, //321
	{ 359, "[ Upgrade Holyangel to Awakening Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToAwakeningSet, false, 0 }, //321
	{ 360, "[ Upgrade Holyangel to Awakening Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToAwakeningSet, false, 0 }, //321
	{ 361, "[ Upgrade Darkangel to Holyangel Weapon ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeDarkangelToHolyangelWeapon, false, 0 }, //361
	{ 362, "[ Upgrade Darkangel to Holyangel Weapon ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeDarkangelToHolyangelWeapon, false, 0 }, //361
	{ 363, "[ Upgrade Darkangel to Holyangel Weapon ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeDarkangelToHolyangelWeapon, false, 0 }, //361
	{ 364, "[ Upgrade Darkangel to Holyangel Weapon ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeDarkangelToHolyangelWeapon, false, 0 }, //361
	{ 365, "[ Upgrade Darkangel to Holyangel Weapon ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeDarkangelToHolyangelWeapon, false, 0 }, //361
	{ 366, "[ Upgrade Darkangel to Holyangel Weapon ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeDarkangelToHolyangelWeapon, false, 0 }, //361
	{ 367, "[ Upgrade Darkangel to Holyangel Weapon ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeDarkangelToHolyangelWeapon, false, 0 }, //361
	{ 368, "[ Upgrade Darkangel to Holyangel Weapon ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeDarkangelToHolyangelWeapon, false, 0 }, //361
	{ 369, "[ Upgrade Darkangel to Holyangel Weapon ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeDarkangelToHolyangelWeapon, false, 0 }, //361
	{ 370, "[ Attack to Support Conversion (AS) ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixElfConversionAttackToSupport, false, 0 },
	{ 371, "[ Attack to Support Conversion (AS) ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixElfConversionAttackToSupport, false, 0 },
	{ 372, "[ Attack to Support Conversion (AS) ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixElfConversionAttackToSupport, false, 0 },
	{ 373, "[ Attack to Support Conversion (AS) ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixElfConversionAttackToSupport, false, 0 },
	{ 374, "[ Support to Attack Conversion (AS) ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixElfConversionSupportToAttack, false, 0 },
	{ 375, "[ Support to Attack Conversion (AS) ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixElfConversionSupportToAttack, false, 0 },
	{ 376, "[ Support to Attack Conversion (AS) ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixElfConversionSupportToAttack, false, 0 },
	{ 377, "[ Support to Attack Conversion (AS) ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixElfConversionSupportToAttack, false, 0 },
	{ 378, "[ Attack to Magic Conversion (AS) ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixMGConversionAttackToMagic, false, 0 },
	{ 379, "[ Attack to Magic Conversion (AS) ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixMGConversionAttackToMagic, false, 0 },
	{ 380, "[ Attack to Magic Conversion (AS) ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixMGConversionAttackToMagic, false, 0 },
	{ 381, "[ Attack to Magic Conversion (AS) ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixMGConversionAttackToMagic, false, 0 },
	{ 382, "[ Magic to Attack Conversion (AS) ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixMGConversionMagicToAttack, false, 0 },
	{ 383, "[ Magic to Attack Conversion (AS) ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixMGConversionMagicToAttack, false, 0 },
	{ 384, "[ Magic to Attack Conversion (AS) ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixMGConversionMagicToAttack, false, 0 },
	{ 385, "[ Magic to Attack Conversion (AS) ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixMGConversionMagicToAttack, false, 0 },
	{ 386, "[ Upgrade Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 }, //386
	{ 387, "[ Upgrade Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 }, //387
	{ 388, "[ Upgrade Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 }, //388
	{ 389, "[ Upgrade Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 }, //389
	{ 390, "[ Upgrade Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 }, //390
	{ 391, "[ Upgrade Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 }, //391
	{ 392, "[ Upgrade Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 }, //392
	{ 393, "[ Upgrade Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 }, //393
	{ 394, "[ Upgrade Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 }, //394
	{ 395, "[ Upgrade Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 }, //395
	{ 396, "[ Upgrade Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 }, //396
	{ 397, "[ Upgrade Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 }, //397
	{ 398, "[ Upgrade Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 }, //398
	{ 399, "[ Upgrade Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 }, //399
	{ 400, "[ Upgrade Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 }, //400
	{ 401, "[ Upgrade Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 }, //401
	{ 402, "[ Upgrade Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 }, //402
	{ 403, "[ Upgrade Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 }, //403
	{ 404, "[ Upgrade Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 }, //404
	{ 405, "[ Upgrade Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 }, //405
	{ 406, "[ Upgrade Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 }, //406
	{ 407, "[ Upgrade Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 }, //407
	{ 408, "[ Upgrade Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 }, //408
	{ 409, "[ Upgrade Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 }, //409
	{ 410, "[ Upgrade Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 }, //410
	{ 411, "[ Upgrade Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 }, //411
	{ 412, "[ Upgrade Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 }, //412
	{ 413, "[ Upgrade Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 }, //413
	{ 414, "[ Upgrade Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 }, //414
	{ 415, "[ Upgrade Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 }, //415
	{ 416, "[ Upgrade Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 }, //416
	{ 417, "[ Upgrade Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 }, //417
	{ 418, "[ Upgrade Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 }, //418
	{ 419, "[ Upgrade Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 }, //419
	{ 420, "[ Upgrade Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 }, //420
	{ 421, "[ Upgrade Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 }, //421
	{ 422, "[ Upgrade Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 }, //422
	{ 423, "[ Upgrade Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 }, //423
	{ 424, "[ Upgrade Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 }, //424
	{ 425, "[ Upgrade Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 }, //425
	{ 426, "[ Upgrade Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 }, //426
	{ 427, "[ Upgrade Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 }, //427
	{ 428, "[ Upgrade Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 }, //428
	{ 429, "[ Upgrade Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 }, //429
	{ 430, "[ Upgrade Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixSealedBloodangelHero, false, 0 },
	{ 431, "[ Upgrade Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixSealedBloodangelHero, false, 0 },
	{ 432, "[ Upgrade Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixSealedBloodangelHero, false, 0 },
	{ 433, "[ Upgrade Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixSealedBloodangelHero, false, 0 },
	{ 434, "[ Upgrade Bloodangel To Darkangel Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 }, //434
	{ 435, "[ Upgrade Bloodangel To Darkangel Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 }, //435
	{ 436, "[ Upgrade Bloodangel To Darkangel Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 }, //436
	{ 437, "[ Upgrade Bloodangel To Darkangel Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 }, //437
	{ 438, "[ Upgrade DarkAngel Weapon Mace ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBloodangelToDarkangelWeapon, false, 0 }, //438
	{ 439, "[ Upgrade Darkangel to Holyangel Weapon ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeDarkangelToHolyangelWeapon, false, 0 }, //439
	{ 440, "[ Upgrade Holyangel To Soul Weapon ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToSoulWeapon, false, 0 }, //440
	{ 441, "[ Upgrade Darkangel To Holyangel Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeDarkangelToHolyangelSet, false, 0 }, //441
	{ 442, "[ Upgrade Darkangel To Holyangel Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeDarkangelToHolyangelSet, false, 0 }, //442
	{ 443, "[ Upgrade Darkangel To Holyangel Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeDarkangelToHolyangelSet, false, 0 }, //443
	{ 444, "[ Upgrade Darkangel To Holyangel Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeDarkangelToHolyangelSet, false, 0 }, //444
	{ 445, "[ Upgrade Holyangel To Awakening Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToAwakeningSet, false, 0 }, //445
	{ 446, "[ Upgrade Holyangel To Awakening Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToAwakeningSet, false, 0 }, //446
	{ 447, "[ Upgrade Holyangel To Awakening Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToAwakeningSet, false, 0 }, //447
	{ 448, "[ Upgrade Holyangel To Awakening Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToAwakeningSet, false, 0 }, //448
	{ 449, "[ Upgrade Awakening to Blue Eye Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeAwakeningToBlueEyeSet, false, 0 }, //449
	{ 450, "[ Upgrade Awakening to Blue Eye Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeAwakeningToBlueEyeSet, false, 0 }, //450
	{ 451, "[ Upgrade Awakening to Blue Eye Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeAwakeningToBlueEyeSet, false, 0 }, //451
	{ 452, "[ Upgrade Awakening to Blue Eye Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeAwakeningToBlueEyeSet, false, 0 }, //452
	{ 453, "[ Upgrade Awakening to Blue Eye Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeAwakeningToBlueEyeSet, false, 0 }, //453
	{ 454, "[ Upgrade Awakening to Blue Eye Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeAwakeningToBlueEyeSet, false, 0 }, //454
	{ 455, "[ Upgrade Awakening to Blue Eye Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeAwakeningToBlueEyeSet, false, 0 }, //455
	{ 456, "[ Upgrade Awakening to Blue Eye Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeAwakeningToBlueEyeSet, false, 0 }, //456
	{ 457, "[ Upgrade Awakening to Blue Eye Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeAwakeningToBlueEyeSet, false, 0 }, //457
	{ 458, "[ Upgrade Awakening to Blue Eye Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeAwakeningToBlueEyeSet, false, 0 }, //458
	{ 459, "[ Upgrade Awakening to Blue Eye Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeAwakeningToBlueEyeSet, false, 0 }, //459
	{ 460, "[ Upgrade Awakening to Blue Eye Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeAwakeningToBlueEyeSet, false, 0 }, //460
	{ 461, "[ Upgrade Awakening to Blue Eye Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeAwakeningToBlueEyeSet, false, 0 }, //461
	{ 462, "[ Upgrade Awakening to Blue Eye Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeAwakeningToBlueEyeSet, false, 0 }, //462
	{ 463, "[ Upgrade Awakening to Blue Eye Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeAwakeningToBlueEyeSet, false, 0 }, //463
	{ 464, "[ Upgrade Awakening to Blue Eye Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeAwakeningToBlueEyeSet, false, 0 }, //464
	{ 465, "[ Upgrade Awakening to Blue Eye Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeAwakeningToBlueEyeSet, false, 0 }, //465
	{ 466, "[ Upgrade Awakening to Blue Eye Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeAwakeningToBlueEyeSet, false, 0 }, //466
	{ 467, "[ Upgrade Awakening to Blue Eye Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeAwakeningToBlueEyeSet, false, 0 }, //467
	{ 468, "[ Upgrade Awakening to Blue Eye Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeAwakeningToBlueEyeSet, false, 0 }, //468
	{ 469, "[ Upgrade Awakening to Blue Eye Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeAwakeningToBlueEyeSet, false, 0 }, //469
	{ 470, "[ Upgrade Awakening to Blue Eye Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeAwakeningToBlueEyeSet, false, 0 }, //470
	{ 471, "[ Upgrade Awakening to Blue Eye Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeAwakeningToBlueEyeSet, false, 0 }, //471
	{ 472, "[ Upgrade Awakening to Blue Eye Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeAwakeningToBlueEyeSet, false, 0 }, //472
	{ 473, "[ Upgrade Awakening to Blue Eye Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeAwakeningToBlueEyeSet, false, 0 }, //473
	{ 474, "[ Upgrade Awakening to Blue Eye Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeAwakeningToBlueEyeSet, false, 0 }, //474
	{ 475, "[ Upgrade Awakening to Blue Eye Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeAwakeningToBlueEyeSet, false, 0 }, //475
	{ 476, "[ Upgrade Awakening to Blue Eye Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeAwakeningToBlueEyeSet, false, 0 }, //476
	{ 477, "[ Upgrade Awakening to Blue Eye Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeAwakeningToBlueEyeSet, false, 0 }, //477
	{ 478, "[ Upgrade Awakening to Blue Eye Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeAwakeningToBlueEyeSet, false, 0 }, //478
	{ 479, "[ Upgrade Awakening to Blue Eye Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeAwakeningToBlueEyeSet, false, 0 }, //479
	{ 480, "[ Upgrade Awakening to Blue Eye Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeAwakeningToBlueEyeSet, false, 0 }, //480
	{ 481, "[ Upgrade Awakening to Blue Eye Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeAwakeningToBlueEyeSet, false, 0 }, //481
	{ 482, "[ Upgrade Awakening to Blue Eye Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeAwakeningToBlueEyeSet, false, 0 }, //482
	{ 483, "[ Upgrade Awakening to Blue Eye Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeAwakeningToBlueEyeSet, false, 0 }, //483
	{ 484, "[ Upgrade Awakening to Blue Eye Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeAwakeningToBlueEyeSet, false, 0 }, //484
	{ 485, "[ Upgrade Awakening to Blue Eye Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeAwakeningToBlueEyeSet, false, 0 }, //485
	{ 486, "[ Upgrade Awakening to Blue Eye Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeAwakeningToBlueEyeSet, false, 0 }, //486
	{ 487, "[ Upgrade Awakening to Blue Eye Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeAwakeningToBlueEyeSet, false, 0 }, //487
	{ 488, "[ Upgrade Awakening to Blue Eye Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeAwakeningToBlueEyeSet, false, 0 }, //488
	{ 489, "[ Upgrade Soul Weapon ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToSoulWeapon, false, 0 }, //489
	{ 490, "[ Upgrade Soul Weapon ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToSoulWeapon, false, 0 }, //490
	{ 491, "[ Upgrade Soul Weapon ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToSoulWeapon, false, 0 }, //491
	{ 492, "[ Upgrade Soul Weapon ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToSoulWeapon, false, 0 }, //492
	{ 493, "[ Upgrade Soul Weapon ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToSoulWeapon, false, 0 }, //493
	{ 494, "[ Upgrade Soul Weapon ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToSoulWeapon, false, 0 }, //494
	{ 495, "[ Upgrade Soul Weapon ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToSoulWeapon, false, 0 }, //495
	{ 496, "[ Upgrade Soul Weapon ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToSoulWeapon, false, 0 }, //496
	{ 497, "[ Upgrade Soul Weapon ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToSoulWeapon, false, 0 }, //497
	{ 498, "[ Attack To Support Conversion (BE) ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixElfConversionAttackToSupport, false, 0 }, // 498
	{ 499, "[ Attack To Support Conversion (BE) ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixElfConversionAttackToSupport, false, 0 }, // 499
	{ 500, "[ Attack To Support Conversion (BE) ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixElfConversionAttackToSupport, false, 0 }, // 500
	{ 509, "[ Attack To Support Conversion (BE) ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixElfConversionAttackToSupport, false, 0 }, // 509
	{ 510, "[ Support To Attack Conversion (BE) ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixElfConversionSupportToAttack, false, 0 }, // 510
	{ 511, "[ Support To Attack Conversion (BE) ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixElfConversionSupportToAttack, false, 0 }, // 511
	{ 512, "[ Support To Attack Conversion (BE) ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixElfConversionSupportToAttack, false, 0 }, // 512
	{ 513, "[ Support To Attack Conversion (BE) ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixElfConversionSupportToAttack, false, 0 }, // 513
	{ 514, "[ Attack To Magic Conversion (BE) ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixMGConversionAttackToMagic, false, 0 }, // 514
	{ 515, "[ Attack To Magic Conversion (BE) ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixMGConversionAttackToMagic, false, 0 }, // 515
	{ 516, "[ Attack To Magic Conversion (BE) ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixMGConversionAttackToMagic, false, 0 }, // 516
	{ 517, "[ Attack To Magic Conversion (BE) ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixMGConversionAttackToMagic, false, 0 }, // 517
	{ 518, "[ Magic To Attack Conversion (BE) ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixMGConversionMagicToAttack, false, 0 }, // 518
	{ 519, "[ Magic To Attack Conversion (BE) ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixMGConversionMagicToAttack, false, 0 }, // 519
	{ 520, "[ Magic To Attack Conversion (BE) ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixMGConversionMagicToAttack, false, 0 }, // 520
	{ 521, "[ Magic To Attack Conversion (BE) ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixMGConversionMagicToAttack, false, 0 }, // 521
	{ 502, "[ Create SpellStone ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixCreateSpellStone, true, 8 }, //502
	{ 503, "[ Bind Wings Create Lvl 2 ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBindWingsCreateLvl2, false, 0 }, //503
	{ 504, "[ Bind Wings Create Lvl 3 ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBindWingsCreateLvl3, false, 0 }, //504
	//{ 505, "[ Bind Wings Create Lvl 4 ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBindWingsCreateLvl4, false, 0 }, //505
	{ 506, "[ Garuda Feather ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixGarudaFeather, false, 0 }, //506
	{ 507, "[ Fourth Wings ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixFourthWings, false, 0 }, //507
	{ 508, "[ Handcraft Stones ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixHandcraftStones, true, 16 }, //508
	{ 524, "[ Upgrade Awakening to Blue Eye Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeAwakeningToBlueEyeSet, false, 0 }, //524
	{ 525, "[ Upgrade Awakening to Blue Eye Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeAwakeningToBlueEyeSet, false, 0 }, //525
	{ 526, "[ Upgrade Awakening to Blue Eye Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeAwakeningToBlueEyeSet, false, 0 }, //526
	{ 527, "[ Upgrade Awakening to Blue Eye Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeAwakeningToBlueEyeSet, false, 0 }, //527
	{ 528, "[ Upgrade Bloodangel To Darkangel Weapon ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBloodangelToDarkangelWeapon, false, 0 }, //528
	{ 529, "[ Upgrade Darkangel To Holyangel Weapon ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeDarkangelToHolyangelWeapon, false, 0 }, //529
	{ 530, "[ Upgrade Holyangel To Soul Weapon ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToSoulWeapon, false, 0 }, //530

	/*
	Upgrade Earring
	*/
	{ 522, "[ Upgrade Earring Rage To Ancestors ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeEarringRageToAncestors, false, 0 }, //522 -> Left +15
	{ 523, "[ Upgrade Earring Rage To Ancestors ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeEarringRageToAncestors, false, 0 }, //523 -> Right +15
	{ 531, "[ Upgrade Earring Rage To Ancestors ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeEarringRageToAncestors, false, 0 }, //531 -> Left +14
	{ 532, "[ Upgrade Earring Rage To Ancestors ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeEarringRageToAncestors, false, 0 }, //532 -> Right +14
	{ 533, "[ Upgrade Earring Rage To Ancestors ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeEarringRageToAncestors, false, 0 }, //533 -> Left +13
	{ 534, "[ Upgrade Earring Rage To Ancestors ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeEarringRageToAncestors, false, 0 }, //534 -> Right +13
	{ 535, "[ Upgrade Earring Rage To Ancestors ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeEarringRageToAncestors, false, 0 }, //535 -> Left +12
	{ 536, "[ Upgrade Earring Rage To Ancestors ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeEarringRageToAncestors, false, 0 }, //536 -> Right +12
	{ 537, "[ Upgrade Earring Rage To Ancestors ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeEarringRageToAncestors, false, 0 }, //537 -> Left +11
	{ 538, "[ Upgrade Earring Rage To Ancestors ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeEarringRageToAncestors, false, 0 }, //538 -> Right +11

	{ 547, "[ Upgrade Bloodangel To Darkangel Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 }, //547
	{ 548, "[ Upgrade Bloodangel To Darkangel Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 }, //548
	{ 549, "[ Upgrade Bloodangel To Darkangel Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 }, //549
	{ 550, "[ Upgrade Bloodangel To Darkangel Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 }, //550
	{ 551, "[ Upgrade Bloodangel to Darkangel Weapon ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBloodangelToDarkangelWeapon, false, 0 }, //551
	{ 552, "[ Upgrade Darkangel to Holyangel Weapon ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeDarkangelToHolyangelWeapon, false, 0 }, //552
	{ 553, "[ Upgrade Holyangel To Soul Weapon ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToSoulWeapon, false, 0 }, //553
	{ 554, "[ Upgrade Darkangel to Holyangel Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeDarkangelToHolyangelSet, false, 0 }, //554
	{ 555, "[ Upgrade Darkangel to Holyangel Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeDarkangelToHolyangelSet, false, 0 }, //555
	{ 556, "[ Upgrade Darkangel to Holyangel Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeDarkangelToHolyangelSet, false, 0 }, //556
	{ 557, "[ Upgrade Darkangel to Holyangel Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeDarkangelToHolyangelSet, false, 0 }, //557
	{ 558, "[ Upgrade Holyangel to Awakening Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToAwakeningSet, false, 0 }, //558
	{ 559, "[ Upgrade Holyangel to Awakening Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToAwakeningSet, false, 0 }, //559
	{ 560, "[ Upgrade Holyangel to Awakening Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToAwakeningSet, false, 0 }, //560
	{ 561, "[ Upgrade Holyangel to Awakening Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToAwakeningSet, false, 0 }, //561
	{ 562, "[ Upgrade Awakening to Blue Eye Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeAwakeningToBlueEyeSet, false, 0 }, //562
	{ 563, "[ Upgrade Awakening to Blue Eye Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeAwakeningToBlueEyeSet, false, 0 }, //563
	{ 564, "[ Upgrade Awakening to Blue Eye Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeAwakeningToBlueEyeSet, false, 0 }, //564
	{ 565, "[ Upgrade Awakening to Blue Eye Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeAwakeningToBlueEyeSet, false, 0 }, //565

	{ 601, "[ Seal Armor Piece ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixSealArmorPiece, false, 0 }, //601
	{ 602, "[ Scratched Sealed Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixScratchedSealedBloodangel, false, 0 }, //602
	{ 603, "[ Scratched Sealed Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixScratchedSealedBloodangel, false, 0 }, //603
	{ 604, "[ Scratched Sealed Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixScratchedSealedBloodangel, false, 0 }, //604
	{ 605, "[ Scratched Sealed Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixScratchedSealedBloodangel, false, 0 }, //605
	{ 606, "[ Scratched Sealed Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixScratchedSealedBloodangel, false, 0 }, //606


	{ 566, "[ Upgrade Blue Eye to Silver Heart Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBlueEyeToSilverHeartSet, false, 0 }, //
	{ 567, "[ Upgrade Blue Eye to Silver Heart Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBlueEyeToSilverHeartSet, false, 0 }, //
	{ 568, "[ Upgrade Blue Eye to Silver Heart Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBlueEyeToSilverHeartSet, false, 0 }, //
	{ 569, "[ Upgrade Blue Eye to Silver Heart Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBlueEyeToSilverHeartSet, false, 0 }, //
	{ 570, "[ Upgrade Blue Eye to Silver Heart Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBlueEyeToSilverHeartSet, false, 0 }, //
	{ 571, "[ Upgrade Blue Eye to Silver Heart Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBlueEyeToSilverHeartSet, false, 0 }, //
	{ 572, "[ Upgrade Blue Eye to Silver Heart Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBlueEyeToSilverHeartSet, false, 0 }, //
	{ 573, "[ Upgrade Blue Eye to Silver Heart Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBlueEyeToSilverHeartSet, false, 0 }, //
	{ 574, "[ Upgrade Blue Eye to Silver Heart Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBlueEyeToSilverHeartSet, false, 0 }, //
	{ 575, "[ Upgrade Blue Eye to Silver Heart Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBlueEyeToSilverHeartSet, false, 0 }, //
	{ 576, "[ Upgrade Blue Eye to Silver Heart Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBlueEyeToSilverHeartSet, false, 0 }, //
	{ 577, "[ Upgrade Blue Eye to Silver Heart Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBlueEyeToSilverHeartSet, false, 0 }, //
	{ 578, "[ Upgrade Blue Eye to Silver Heart Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBlueEyeToSilverHeartSet, false, 0 }, //
	{ 579, "[ Upgrade Blue Eye to Silver Heart Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBlueEyeToSilverHeartSet, false, 0 }, //
	{ 580, "[ Upgrade Blue Eye to Silver Heart Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBlueEyeToSilverHeartSet, false, 0 }, //
	{ 581, "[ Upgrade Blue Eye to Silver Heart Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBlueEyeToSilverHeartSet, false, 0 }, //
	{ 582, "[ Upgrade Blue Eye to Silver Heart Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBlueEyeToSilverHeartSet, false, 0 }, //
	{ 583, "[ Upgrade Blue Eye to Silver Heart Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBlueEyeToSilverHeartSet, false, 0 }, //
	{ 584, "[ Upgrade Blue Eye to Silver Heart Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBlueEyeToSilverHeartSet, false, 0 }, //
	{ 585, "[ Upgrade Blue Eye to Silver Heart Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBlueEyeToSilverHeartSet, false, 0 }, //
	{ 586, "[ Upgrade Blue Eye to Silver Heart Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBlueEyeToSilverHeartSet, false, 0 }, //
	{ 587, "[ Upgrade Blue Eye to Silver Heart Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBlueEyeToSilverHeartSet, false, 0 }, //
	{ 588, "[ Upgrade Blue Eye to Silver Heart Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBlueEyeToSilverHeartSet, false, 0 }, //
	{ 589, "[ Upgrade Blue Eye to Silver Heart Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBlueEyeToSilverHeartSet, false, 0 }, //
	{ 590, "[ Upgrade Blue Eye to Silver Heart Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBlueEyeToSilverHeartSet, false, 0 }, //
	{ 591, "[ Upgrade Blue Eye to Silver Heart Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBlueEyeToSilverHeartSet, false, 0 }, //
	{ 592, "[ Upgrade Blue Eye to Silver Heart Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBlueEyeToSilverHeartSet, false, 0 }, //
	{ 593, "[ Upgrade Blue Eye to Silver Heart Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBlueEyeToSilverHeartSet, false, 0 }, //
	{ 594, "[ Upgrade Blue Eye to Silver Heart Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBlueEyeToSilverHeartSet, false, 0 }, //
	{ 595, "[ Upgrade Blue Eye to Silver Heart Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBlueEyeToSilverHeartSet, false, 0 }, //
	{ 596, "[ Upgrade Blue Eye to Silver Heart Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBlueEyeToSilverHeartSet, false, 0 }, //
	{ 597, "[ Upgrade Blue Eye to Silver Heart Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBlueEyeToSilverHeartSet, false, 0 }, //
	{ 598, "[ Upgrade Blue Eye to Silver Heart Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBlueEyeToSilverHeartSet, false, 0 }, //
	{ 599, "[ Upgrade Blue Eye to Silver Heart Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBlueEyeToSilverHeartSet, false, 0 }, //
	{ 600, "[ Upgrade Blue Eye to Silver Heart Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBlueEyeToSilverHeartSet, false, 0 }, //
	{ 1000, "[ Upgrade Blue Eye to Silver Heart Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBlueEyeToSilverHeartSet, false, 0 }, //
	{ 1001, "[ Upgrade Blue Eye to Silver Heart Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBlueEyeToSilverHeartSet, false, 0 }, //
	{ 1002, "[ Upgrade Blue Eye to Silver Heart Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBlueEyeToSilverHeartSet, false, 0 }, //
	{ 1003, "[ Upgrade Blue Eye to Silver Heart Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBlueEyeToSilverHeartSet, false, 0 }, //
	{ 1004, "[ Upgrade Blue Eye to Silver Heart Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBlueEyeToSilverHeartSet, false, 0 }, //
	{ 1005, "[ Upgrade Blue Eye to Silver Heart Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBlueEyeToSilverHeartSet, false, 0 }, //
	{ 1006, "[ Upgrade Blue Eye to Silver Heart Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBlueEyeToSilverHeartSet, false, 0 }, //
	{ 1007, "[ Upgrade Blue Eye to Silver Heart Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBlueEyeToSilverHeartSet, false, 0 }, //
	{ 1008, "[ Upgrade Blue Eye to Silver Heart Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBlueEyeToSilverHeartSet, false, 0 }, //
	{ 1009, "[ Upgrade Blue Eye to Silver Heart Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBlueEyeToSilverHeartSet, false, 0 }, //
	{ 1010, "[ Upgrade Blue Eye to Silver Heart Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBlueEyeToSilverHeartSet, false, 0 }, //
	{ 1011, "[ Upgrade Blue Eye to Silver Heart Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBlueEyeToSilverHeartSet, false, 0 }, //
	{ 1012, "[ Upgrade Blue Eye to Silver Heart Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBlueEyeToSilverHeartSet, false, 0 }, //
	{ 1013, "[ Upgrade Soul to Blue Eye Weapon ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeSoulToBlueEyeWeapon, false, 0 },
	{ 1014, "[ Upgrade Soul to Blue Eye Weapon ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeSoulToBlueEyeWeapon, false, 0 },
	{ 1015, "[ Upgrade Soul to Blue Eye Weapon ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeSoulToBlueEyeWeapon, false, 0 },
	{ 1016, "[ Upgrade Soul to Blue Eye Weapon ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeSoulToBlueEyeWeapon, false, 0 },
	{ 1017, "[ Upgrade Soul to Blue Eye Weapon ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeSoulToBlueEyeWeapon, false, 0 },
	{ 1018, "[ Upgrade Soul to Blue Eye Weapon ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeSoulToBlueEyeWeapon, false, 0 },
	{ 1019, "[ Upgrade Soul to Blue Eye Weapon ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeSoulToBlueEyeWeapon, false, 0 },
	{ 1020, "[ Upgrade Soul to Blue Eye Weapon ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeSoulToBlueEyeWeapon, false, 0 },
	{ 1021, "[ Upgrade Soul to Blue Eye Weapon ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeSoulToBlueEyeWeapon, false, 0 },
	{ 1022, "[ Upgrade Soul to Blue Eye Weapon ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeSoulToBlueEyeWeapon, false, 0 },
	{ 1023, "[ Upgrade Soul to Blue Eye Weapon ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeSoulToBlueEyeWeapon, false, 0 },
	{ 1024, "[ Upgrade Soul to Blue Eye Weapon ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeSoulToBlueEyeWeapon, false, 0 },
	{ 1025, "[ Attack To Support Conversion (SH) ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixElfConversionAttackToSupport, false, 0 },
	{ 1026, "[ Attack To Support Conversion (SH) ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixElfConversionAttackToSupport, false, 0 },
	{ 1027, "[ Attack To Support Conversion (SH) ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixElfConversionAttackToSupport, false, 0 },
	{ 1028, "[ Attack To Support Conversion (SH) ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixElfConversionAttackToSupport, false, 0 },
	{ 1029, "[ Support To Attack Conversion (SH) ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixElfConversionSupportToAttack, false, 0 },
	{ 1030, "[ Support To Attack Conversion (SH) ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixElfConversionSupportToAttack, false, 0 },
	{ 1031, "[ Support To Attack Conversion (SH) ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixElfConversionSupportToAttack, false, 0 },
	{ 1032, "[ Support To Attack Conversion (SH) ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixElfConversionSupportToAttack, false, 0 },
	{ 1033, "[ Attack To Magic Conversion (SH) ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixMGConversionAttackToMagic, false, 0 },
	{ 1034, "[ Attack To Magic Conversion (SH) ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixMGConversionAttackToMagic, false, 0 },
	{ 1035, "[ Attack To Magic Conversion (SH) ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixMGConversionAttackToMagic, false, 0 },
	{ 1036, "[ Attack To Magic Conversion (SH) ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixMGConversionAttackToMagic, false, 0 },
	{ 1037, "[ Magic To Attack Conversion (SH) ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixMGConversionMagicToAttack, false, 0 },
	{ 1038, "[ Magic To Attack Conversion (SH) ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixMGConversionMagicToAttack, false, 0 },
	{ 1039, "[ Magic To Attack Conversion (SH) ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixMGConversionMagicToAttack, false, 0 },
	{ 1040, "[ Magic To Attack Conversion (SH) ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixMGConversionMagicToAttack, false, 0 },
	{ 1041, "[ Upgrade Earring Ancestors To Honor ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeEarringAncestorsToHonor, false, 0 },
	{ 1042, "[ Upgrade Earring Ancestors To Honor ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeEarringAncestorsToHonor, false, 0 },
	{ 1043, "[ Upgrade Earring Ancestors To Honor ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeEarringAncestorsToHonor, false, 0 },
	{ 1044, "[ Upgrade Earring Ancestors To Honor ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeEarringAncestorsToHonor, false, 0 },
	{ 1045, "[ Upgrade Earring Ancestors To Honor ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeEarringAncestorsToHonor, false, 0 },
	{ 1046, "[ Upgrade Earring Ancestors To Honor ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeEarringAncestorsToHonor, false, 0 },
	{ 1047, "[ Upgrade Earring Ancestors To Honor ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeEarringAncestorsToHonor, false, 0 },
	{ 1048, "[ Upgrade Earring Ancestors To Honor ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeEarringAncestorsToHonor, false, 0 },
	{ 1049, "[ Upgrade Earring Ancestors To Honor ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeEarringAncestorsToHonor, false, 0 },
	{ 1050, "[ Upgrade Earring Ancestors To Honor ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeEarringAncestorsToHonor, false, 0 },

	{ 1059, "[ Upgrade Bloodangel To Darkangel Weapon ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBloodangelToDarkangelWeapon, false, 0 },
	{ 1060, "[ Upgrade Darkangel To Holyangel Weapon ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeDarkangelToHolyangelWeapon, false, 0 },
	{ 1061, "[ Upgrade Holyangel To Soul Weapon ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToSoulWeapon, false, 0 },
	{ 1062, "[ Upgrade Soul To Blue Eye Weapon ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeSoulToBlueEyeWeapon, false, 0 },
	{ 1063, "[ Upgrade Bloodangel To Darkangel Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 },
	{ 1064, "[ Upgrade Bloodangel To Darkangel Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 },
	{ 1065, "[ Upgrade Bloodangel To Darkangel Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 },
	{ 1066, "[ Upgrade Bloodangel To Darkangel Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixBloodangelToDarkangelSet, false, 0 },
	{ 1067, "[ Upgrade Darkangel To Holyangel Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeDarkangelToHolyangelSet, false, 0 },
	{ 1068, "[ Upgrade Darkangel To Holyangel Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeDarkangelToHolyangelSet, false, 0 },
	{ 1069, "[ Upgrade Darkangel To Holyangel Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeDarkangelToHolyangelSet, false, 0 },
	{ 1070, "[ Upgrade Darkangel To Holyangel Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeDarkangelToHolyangelSet, false, 0 },
	{ 1071, "[ Upgrade Holyangel To Soul Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToAwakeningSet, false, 0 },
	{ 1072, "[ Upgrade Holyangel To Soul Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToAwakeningSet, false, 0 },
	{ 1073, "[ Upgrade Holyangel To Soul Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToAwakeningSet, false, 0 },
	{ 1074, "[ Upgrade Holyangel To Soul Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeHolyangelToAwakeningSet, false, 0 },
	{ 1075, "[ Upgrade Soul To Blue Eye Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeAwakeningToBlueEyeSet, false, 0 },
	{ 1076, "[ Upgrade Soul To Blue Eye Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeAwakeningToBlueEyeSet, false, 0 },
	{ 1077, "[ Upgrade Soul To Blue Eye Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeAwakeningToBlueEyeSet, false, 0 },
	{ 1078, "[ Upgrade Soul To Blue Eye Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeAwakeningToBlueEyeSet, false, 0 },
	{ 1079, "[ Upgrade Blue Eye To Silver Heart Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBlueEyeToSilverHeartSet, false, 0 },
	{ 1080, "[ Upgrade Blue Eye To Silver Heart Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBlueEyeToSilverHeartSet, false, 0 },
	{ 1081, "[ Upgrade Blue Eye To Silver Heart Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBlueEyeToSilverHeartSet, false, 0 },
	{ 1082, "[ Upgrade Blue Eye To Silver Heart Set ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixUpgradeBlueEyeToSilverHeartSet, false, 0 },
	{ 1083, "[ Sealed Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixSealedMasteryFinal, false, 0 },
	{ 1084, "[ Sealed Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixSealedMasteryFinal, false, 0 },
	{ 1085, "[ Sealed Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixSealedMasteryFinal, false, 0 },
	{ 1086, "[ Sealed Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixSealedMasteryFinal, false, 0 },
	{ 1087, "[ E To S Conversion Knight ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixKnightConversionEnergyToStrength, false, 0 },
	{ 1088, "[ E To S Conversion Knight ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixKnightConversionEnergyToStrength, false, 0 },
	{ 1089, "[ E To S Conversion Knight ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixKnightConversionEnergyToStrength, false, 0 },
	{ 1090, "[ E To S Conversion Knight ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixKnightConversionEnergyToStrength, false, 0 },
	{ 1091, "[ S To E Conversion Knight ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixKnightConversionStrengthToEnergy, false, 0 },
	{ 1092, "[ S To E Conversion Knight ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixKnightConversionStrengthToEnergy, false, 0 },
	{ 1093, "[ S To E Conversion Knight ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixKnightConversionStrengthToEnergy, false, 0 },
	{ 1094, "[ S To E Conversion Knight ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixKnightConversionStrengthToEnergy, false, 0 },
	{ 1095, "[ E To S Conversion Knight ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixKnightConversionEnergyToStrength, false, 0 },
	{ 1096, "[ E To S Conversion Knight ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixKnightConversionEnergyToStrength, false, 0 },
	{ 1097, "[ E To S Conversion Knight ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixKnightConversionEnergyToStrength, false, 0 },
	{ 1098, "[ E To S Conversion Knight ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixKnightConversionEnergyToStrength, false, 0 },
	{ 1099, "[ S To E Conversion Knight ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixKnightConversionStrengthToEnergy, false, 0 },
	{ 1100, "[ S To E Conversion Knight ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixKnightConversionStrengthToEnergy, false, 0 },
	{ 1101, "[ S To E Conversion Knight ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixKnightConversionStrengthToEnergy, false, 0 },
	{ 1102, "[ S To E Conversion Knight ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixKnightConversionStrengthToEnergy, false, 0 },
	{ 1103, "[ E To S Conversion Knight ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixKnightConversionEnergyToStrength, false, 0 },
	{ 1104, "[ E To S Conversion Knight ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixKnightConversionEnergyToStrength, false, 0 },
	{ 1105, "[ E To S Conversion Knight ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixKnightConversionEnergyToStrength, false, 0 },
	{ 1106, "[ E To S Conversion Knight ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixKnightConversionEnergyToStrength, false, 0 },
	{ 1107, "[ S To E Conversion Knight ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixKnightConversionStrengthToEnergy, false, 0 },
	{ 1108, "[ S To E Conversion Knight ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixKnightConversionStrengthToEnergy, false, 0 },
	{ 1109, "[ S To E Conversion Knight ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixKnightConversionStrengthToEnergy, false, 0 },
	{ 1110, "[ S To E Conversion Knight ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixKnightConversionStrengthToEnergy, false, 0 },
	{ 1111, "[ E To S Conversion Knight ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixKnightConversionEnergyToStrength, false, 0 },
	{ 1112, "[ E To S Conversion Knight ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixKnightConversionEnergyToStrength, false, 0 },
	{ 1113, "[ E To S Conversion Knight ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixKnightConversionEnergyToStrength, false, 0 },
	{ 1114, "[ E To S Conversion Knight ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixKnightConversionEnergyToStrength, false, 0 },
	{ 1115, "[ S To E Conversion Knight ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixKnightConversionStrengthToEnergy, false, 0 },
	{ 1116, "[ S To E Conversion Knight ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixKnightConversionStrengthToEnergy, false, 0 },
	{ 1117, "[ S To E Conversion Knight ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixKnightConversionStrengthToEnergy, false, 0 },
	{ 1118, "[ S To E Conversion Knight ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixKnightConversionStrengthToEnergy, false, 0 },
	{ 1119, "[ E To S Conversion Knight ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixKnightConversionEnergyToStrength, false, 0 },
	{ 1120, "[ E To S Conversion Knight ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixKnightConversionEnergyToStrength, false, 0 },
	{ 1121, "[ E To S Conversion Knight ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixKnightConversionEnergyToStrength, false, 0 },
	{ 1122, "[ E To S Conversion Knight ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixKnightConversionEnergyToStrength, false, 0 },
	{ 1123, "[ S To E Conversion Knight ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixKnightConversionStrengthToEnergy, false, 0 },
	{ 1124, "[ S To E Conversion Knight ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixKnightConversionStrengthToEnergy, false, 0 },
	{ 1125, "[ S To E Conversion Knight ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixKnightConversionStrengthToEnergy, false, 0 },
	{ 1126, "[ S To E Conversion Knight ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixKnightConversionStrengthToEnergy, false, 0 },
	{ 1127, "[ E To S Conversion Knight ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixKnightConversionEnergyToStrength, false, 0 },
	{ 1128, "[ E To S Conversion Knight ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixKnightConversionEnergyToStrength, false, 0 },
	{ 1129, "[ E To S Conversion Knight ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixKnightConversionEnergyToStrength, false, 0 },
	{ 1130, "[ E To S Conversion Knight ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixKnightConversionEnergyToStrength, false, 0 },
	{ 1131, "[ S To E Conversion Knight ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixKnightConversionStrengthToEnergy, false, 0 },
	{ 1132, "[ S To E Conversion Knight ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixKnightConversionStrengthToEnergy, false, 0 },
	{ 1133, "[ S To E Conversion Knight ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixKnightConversionStrengthToEnergy, false, 0 },
	{ 1134, "[ S To E Conversion Knight ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixKnightConversionStrengthToEnergy, false, 0 },

	{ 1167, "[ Sealed Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixSealedMasteryFinal, false, 0 },
	{ 1168, "[ Sealed Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixSealedMasteryFinal, false, 0 },
	{ 1169, "[ Sealed Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixSealedMasteryFinal, false, 0 },
	{ 1170, "[ Sealed Bloodangel ]", InterfaceData::ChaosMachine, &MixHandler::ChaosMixSealedMasteryFinal, false, 0 },

	{0xFFFF,	"[ NULL ]",	InterfaceData::None,	&MixHandler::ChaosMixNone , false, 0 },
};

PentagramMixHandler PentagramMixTable[CHAOS_MIX_PENTAGRAM_MAX] =
{
	{	"[ Pentagram_Unknown_1 ]",			InterfaceData::None,			&MixHandler::PentagramMixNone			}, //0
	{	"[ Pentragram Jewel Mix ]",			InterfaceData::Adniel,			&MixHandler::PentagramJewelMix			}, //1
	{	"[ Pentragram Slot of Radiance ]",	InterfaceData::Adniel,			&MixHandler::PentagramSlotOfRadiance	}, //2
	{	"[ Errtel Element Change ]",		InterfaceData::Adniel,			&MixHandler::ErrtelElementChange	}, //3
};

PentagramJewelMixHandler PentagramUpgradeMixTable[CHAOS_MIX_PENTAGRAM_JEWEL_UPGRADE_MAX] =
{
	{	"[ Pentragram Jewel Upgrade Level ]",	InterfaceData::Adniel,		&MixHandler::PentagramJewelUpgradeLevelMix	}, //1
	{	"[ Pentragram Jewel Upgrade Rank ]",	InterfaceData::Adniel,		&MixHandler::PentagramJewelUpgradeRankMix	}, //2
};

SocketMixHandler SocketMixTable[MAX_SOCKET_MIX] = 
{
	{	"[ Socket None ]",						InterfaceData::SeedCreation,		&MixHandler::SocketMixNone	}, //0
	{	"[ Socket Upgrade ]",					InterfaceData::SeedCreation,		&MixHandler::SocketMixSetUpgrade	}, //1
	{	"[ Socket Seed Create ]",				InterfaceData::SeedCreation,		&MixHandler::SocketMixSeedCreate	}, //2
	{	"[ Socket Seed Insert ]",				InterfaceData::SeedCreation,		&MixHandler::SocketMixSeedInsert	}, //3
	{	"[ Socket Seed Remove ]",				InterfaceData::SeedCreation,		&MixHandler::SocketMixSeedRemove	}, //4
	{	"[ Socket Seed Assemble ]",				InterfaceData::SeedCreation,		&MixHandler::SocketMixSeedAssemble	}, //5
	{	"[ Socket Seed Upgrade ]",				InterfaceData::SeedCreation,		&MixHandler::SocketMixSeedUpgrade	}, //6
	{	"[ Socket Slot Reduction ]",			InterfaceData::SeedCreation,		&MixHandler::SocketMixSlotReduction	}, //7
};

WingSocketMixHandler WingSocketMixTable[MAX_WING_SOCKET_MIX] =
{
	{ "[ Wing Socket Add ]", InterfaceData::Adniel, &MixHandler::WingSocketMixAdd }, //0
	{ "[ Wing Socket Upgrade ]", InterfaceData::Adniel, &MixHandler::WingSocketMixUpgrade }, //1
};

EarringMixHandler EarringMixTable[MAX_EARRING_MIX] =
{
	{ "[ Earring Upgrade ]", InterfaceData::None, &MixHandler::EarringUpgrade }, //0
};

bool MixHandler::DuplicatedMixCheck()
{
	for ( size_t i = 0; ChaosMixTable[i].id != 0xFFFF; ++i )
	{
		for ( size_t h = 0; ChaosMixTable[h].id != 0xFFFF; ++h )
		{
			if ( i == h )
			{
				continue;
			}

			if ( ChaosMixTable[i].id == ChaosMixTable[h].id )
			{
				return false;
			}
		}
	}

	return true;
}

ChaosMixHandler* MixHandler::GetMixHandler(uint16 mix_id)
{
	for ( int32 i = 0; ChaosMixTable[i].id != 0xFFFF; ++i )
	{
		if ( ChaosMixTable[i].id == mix_id )
		{
			return &ChaosMixTable[i];
		}
	}

	return nullptr;
}

MixHandler::MixHandler(Player* pPlayer, uint8 type)
{
	this->SetPlayer(pPlayer);
	this->success_rate.set(0);
	this->mix_price = 0;
	this->tax_price = 0;
	this->ResetPrice(0);
	this->SetLuckyCharm(0);
	this->SetCount(0);
	this->SetMaxCount(0);
	this->SetType(type);
	this->SetMaxSuccessRate(100);
	this->ResetMassCombinationList(0);
	this->SetDBFlag(0);
	this->ResetItemDataBefore();
	this->ResetItemDataAfter();
	this->ResetSpiritStone(0);
	this->ResetJewelItem(-1);
	this->ResetJewelCount(0);
	this->ResetMaxJewelCount(0);

	if ( pPlayer )
		pPlayer->SetMixCompleted(false);
}

bool MixHandler::tributeTaxPrice(bool tax, uint8 mix_kind)
{
	if ( this->mix_price == 0 )
		return true;

	if ( tax )
	{
		this->tax_price = this->mix_price * sCastleSiege->GetTaxRateChaos(this->GetPlayer()) / 100;
	}

	this->mix_price += this->tax_price;

	if ( !this->GetPlayer()->MoneyHave(this->mix_price) )
	{
		switch ( mix_kind )
		{
		case 0:
			{
				this->GetPlayer()->ChaosMixResult(CHAOS_MIX_RESULT_NOT_ENOUGH_ZEN);
			} break;

		case 1:
			{
				this->GetPlayer()->ChaosMixPentagramResult(249, 0);
			} break;

		case 2:
			{
				this->GetPlayer()->ChaosMixSocketResult(249, 0);
			} break;

		case 3:
			{
				this->GetPlayer()->ChaosMixPentagramResult(249, 0);
			} break;
		}

		this->GetPlayer()->GetInterfaceState()->SetState(0);
		this->ChaosMixLogNotEnoughMixZen();
		return false;
	}

	if ( tax )
	{
		sCastleSiege->AddTributeMoney(this->tax_price);
	}

	this->GetPlayer()->MoneyReduce(this->mix_price);
	return true;
}

void MixHandler::ExecuteMix(uint8 * packet)
{
	CHAOS_MIX_BUTTON * lpMsg = (CHAOS_MIX_BUTTON*)packet;

	if ( sMixMgr->IsMixDisabled(0, lpMsg->mix) )
	{
		this->GetPlayer()->SendNotice(CUSTOM_MESSAGE_ID_RED, "This mix is disabled.");
		this->ChaosMixNone(0);
		return;
	}

	ChaosMixHandler * mixhandle = MixHandler::GetMixHandler(lpMsg->mix);

	if ( !mixhandle )
	{
		if (this->GetPlayer()->IsAdministrator())
		{
			this->GetPlayer()->SendNotice(CUSTOM_MESSAGE_ID_RED, "Wrong mix ID: %u", lpMsg->mix);
		}
		this->ChaosMixNone(0);
		return;
	}

	if ( mixhandle->type != this->GetPlayer()->GetInterfaceState()->GetID() )
	{
		this->ChaosMixLogInvalidMixItems();
		this->GetPlayer()->SendNotice(CUSTOM_MESSAGE_ID_RED, "Wrong Mix Interface. Contact Administrators");
		sLog->outError(LOG_CHAOS_MIX,"Wrong Mix Interface. %s - Mix: %d [%s], Mix Interface: %s, Current Interface: %s",
			this->GetPlayer()->BuildLog().c_str(), lpMsg->mix, mixhandle->name, mixhandle->type.c_str(), this->GetPlayer()->GetInterfaceState()->GetID().c_str());
		return;
	}

	if ( this->GetPlayer()->GetInterfaceState()->GetState() == 1 )
	{
		return;
	}

	this->GetPlayer()->GetInterfaceState()->SetState(1);
	this->SetMixName(mixhandle->name);
	this->SetMixID(lpMsg->mix);
	this->SetCount(lpMsg->count);
	this->SetMaxCount(mixhandle->count);
	this->SetSuccess(false);
	this->GetPlayer()->SetMixCompleted(false);
	this->SetMaxSuccessRate(100);
	this->ResetMassCombinationList(0);
	this->SetDBFlag(0);
	this->ResetItemDataBefore();
	this->ResetItemDataAfter();
	
	this->CalculateSpiritStone();

	chaos_box_loop(i)
	{
		Item * item = this->GetPlayer()->GetMixInventory()->GetItem(i);

		if ( !item->IsItem() )
			continue;

		this->ChaosMixLogItem(item, i);

		if (this->GetItemDataBefore().empty())
		{
			this->SetItemDataBefore("[");
		}
		else
		{
			this->SetItemDataBefore(this->GetItemDataBefore() + ",");
		}
		
		this->SetItemDataBefore(this->GetItemDataBefore() + item->BuildLogDB(i, false));
	}

	if (!this->GetItemDataBefore().empty())
	{
		this->SetItemDataBefore(this->GetItemDataBefore() + "]");
	}

	if ( this->GetPlayer()->IsAdministrator() )
	{
		this->GetPlayer()->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Mix Name: %s", mixhandle->name);
	}
	
	(this->*mixhandle->handler)(lpMsg->data);

	if ( this->GetPlayer()->IsMixCompleted() )
	{
		chaos_box_loop(i)
		{
			Item * item = this->GetPlayer()->GetMixInventory()->GetItem(i);

			if ( !item->IsItem() )
				continue;

			this->ChaosMixLogFinalItem(item, i);
		}
	}
}

void MixHandler::ExecuteMassCombination(uint8 * packet)
{
	POINTER_PCT_LOG(CHAOS_MIX_MASS_COMBINATION, lpMsg, packet, 0);

	if ( sMixMgr->IsMixDisabled(0, lpMsg->mix) )
	{
		return;
	}

	ChaosMixHandler * mixhandle = MixHandler::GetMixHandler(lpMsg->mix);

	if ( !mixhandle )
	{
		return;
	}

	if ( mixhandle->type != this->GetPlayer()->GetInterfaceState()->GetID() )
	{
		this->ChaosMixLogInvalidMixItems();
		this->GetPlayer()->SendNotice(CUSTOM_MESSAGE_ID_RED, "Wrong Mix Interface. Contact Administrators");
		sLog->outError(LOG_CHAOS_MIX,"Wrong Mix Interface. %s - Mix: %d [%s], Mix Interface: %s, Current Interface: %s",
			this->GetPlayer()->BuildLog().c_str(), lpMsg->mix, mixhandle->name, mixhandle->type.c_str(), this->GetPlayer()->GetInterfaceState()->GetID().c_str());
		return;
	}

	if ( !mixhandle->mass_combination || mixhandle->count <= 0 )
	{
		return;
	}

	if ( this->GetPlayer()->GetInterfaceState()->GetState() == 1 )
	{
		return;
	}
	
	this->GetPlayer()->ChaosMixMassCombinationResult(1);
}

void MixHandler::ChaosMixNone(uint8 data)
{
	this->ChaosMixLogInvalidMixItems();
}

void MixHandler::ChaosMixWeapon(uint8 data)
{
	int32 jewel_chaos = 0;
	int64 price_tmp = 0;
	int32 Material = 0;

	chaos_box_loop(i)
	{
		Item const& item = this->GetPlayer()->GetMixInventory()->item[i];

		if (!item.IsItem())
			continue;

		if (item.GetItem() == JEWEL::CHAOS)
		{
			jewel_chaos += item.GetDurability();
			price_tmp += item.GetOldPrice();
		}
		else if (item.GetItem() == JEWEL::BLESS)
		{
			price_tmp += item.GetOldPrice();
		}
		else if (item.GetItem() == JEWEL::SOUL)
		{
			price_tmp += item.GetOldPrice();
		}
		else if (item.GetItem() >= ITEMGET(0, 0) && item.GetItem() < ITEMGET(12, 0) &&
			item.GetLevel() >= 4 && item.GetOption() >= 1)
		{
			price_tmp += item.GetOldPrice();
			Material++;
		}
	}

	if (jewel_chaos < 1 || Material < 1)
	{
		this->ChaosMixLogLackingMixItems();
		return;
	}

	if (price_tmp > 2000000)
		price_tmp = 2000000;

	this->success_rate.set(price_tmp / 20000);
	this->success_rate.limit_min(0);
	this->success_rate.limit_max(100);

	this->mix_price = this->success_rate.get() * 10000;

	if (!this->tributeTaxPrice())
		return;

	this->ChaosMixDebugMessage();

	if ( this->roll_success() )
	{
		uint8 level = Random<uint8>(5);
		uint8 skill = roll_chance_i(this->success_rate.get() / 5 + 6) ? 1 : 0;
		uint8 luck = roll_chance_i(this->success_rate.get() / 5 + 4) ? 1 : 0;
		uint8 opt_rate = Random<uint8>(100);
		uint8 opt_type = Random<uint8>(3);
		uint8 option = (opt_type == 0 && opt_rate < (this->success_rate.get()/5 + 4))?3:
					   (opt_type == 1 && opt_rate < (this->success_rate.get()/5 + 8))?2:
					   (opt_type == 2 && opt_rate < (this->success_rate.get()/5 + 12))?1:
					   0;

		uint8 chaos_item = Random<uint8>(3);
		uint32 final_item = (chaos_item == 0)?ITEMGET(2,6):
							(chaos_item == 1)?ITEMGET(4,6):
							ITEMGET(5,7);

		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, Item(final_item, level, 0, skill, luck, option), 1);
		this->ChaosMixLogSuccess();
	}
	else
	{
		this->GetPlayer()->GetInterfaceState()->SetState(0);
		this->ChaosMixItemDown();
		this->GetPlayer()->ChaosMixSend(0);
		this->GetPlayer()->ChaosMixResult(CHAOS_MIX_RESULT_FAIL);
		this->ChaosMixLogFail();
	}

	this->ChaosMixDBSave();
}

void MixHandler::ChaosMixDevilSquare(uint8 data)
{
	int32 TicketPart1Count = 0;
	int32 TicketPart2Count = 0;
	int32 TicketPart1Level = 0;
	int32 TicketPart2Level = 0;
	int32 ChaosCount = 0;

	chaos_box_loop(i)
	{
		Item const* item = this->GetPlayer()->GetMixInventory()->GetItem(i);

		if ( !item->IsItem() )
			continue;

		if ( item->GetItem() == ITEMGET(14, 17) )
		{
			TicketPart1Count++;

			if ( TicketPart1Level == 0 )
			{
				TicketPart1Level = item->GetLevel();
			}
			else if ( TicketPart1Level != item->GetLevel() )
			{
				TicketPart1Level = -1;
			}
		}
		else if ( item->GetItem() == ITEMGET(14, 18) )
		{
			TicketPart2Count++;

			if ( TicketPart2Level == 0 )
			{
				TicketPart2Level = item->GetLevel();
			}
			else if ( TicketPart2Level != item->GetLevel() )
			{
				TicketPart2Level = -1;
			}
		}
		else if (item->GetItem() == JEWEL::CHAOS)
		{
			ChaosCount += item->GetDurability();
		}
	}

	if ( TicketPart1Count <= 0 || TicketPart2Count <= 0 || ChaosCount <= 0 )
	{
		this->ChaosMixLogLackingMixItems();
		return;
	}

	if ( TicketPart1Level == 0 || TicketPart1Level == -1 || TicketPart2Level == 0 || TicketPart2Level == -1 )
	{
		this->ChaosMixLogInvalidMixItems();
		return;
	}

	if ( TicketPart1Level != TicketPart2Level )
	{
		this->ChaosMixLogInvalidMixItemLevel();
		return;
	}

	if ( (TicketPart1Level - 1) > MAX_DEVIL_SQUARE_GROUND )
	{
		this->ChaosMixLogInvalidMixItemLevel();
		return;
	}

	if ( TicketPart1Count != TicketPart2Count )
	{
		this->ChaosMixLogInvalidMixItems();
		return;
	}

	int32 count = ChaosCount;
	
	if ( TicketPart1Count != count )
	{
		this->ChaosMixLogInvalidMixItems();
		return;
	}

	if ( count != this->GetCount() || count > this->GetMaxCount() )
	{
		this->ChaosMixLogInvalidMixItems();
		return;
	}

	if ( count == 1 && !this->ChaosMixTalismanOfLuck() )
	{
		this->ChaosMixLogTooManyTalismanOfLuck();
		return;
	}

	this->mix_price = m_DevilSquareMixInfo[TicketPart1Level - 1].price * count;

	if ( !this->tributeTaxPrice() )
		return;

	this->GetPlayer()->GetMixInventory()->Clear();
	
	this->GenerateMassCombinationResult(count, m_DevilSquareMixInfo[TicketPart1Level - 1].rate + this->GetLuckyCharm(), 100);

	for ( int32 i = 0; i < count; ++i )
	{
		this->SetSuccess(this->GetMassCombinationList(i) == 1 ? true: false);

		this->ChaosMixDebugMessage();

		if ( this->IsSuccess() )
		{
			sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, Item(ITEMGET(14, 19), TicketPart1Level));
			this->ChaosMixLogSuccess();
		}
		else
		{
			if ( count == 1 )
			{
				this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
			}

			this->ChaosMixLogFail();
		}

		this->ChaosMixDBSave();
	}

	this->GetPlayer()->GetInterfaceState()->SetState(0);
}

void MixHandler::ChaosMixUpgrade10(uint8 data)
{
	this->ChaosMixUpgradeLevel(data, 0);
}

void MixHandler::ChaosMixUpgrade11(uint8 data)
{
	this->ChaosMixUpgradeLevel(data, 1);
}

void MixHandler::ChaosMixDinorant(uint8 data)
{
	int32 UniriaCount = 0;
	int32 ChaosCount = 0;

	chaos_box_loop(i)
	{
		Item const* item = this->GetPlayer()->GetMixInventory()->GetItem(i);

		if ( !item->IsItem() )
			continue;

		if ( item->GetItem() == ITEMGET(13, 2) && item->GetDurability() == 255 )
		{
			UniriaCount++;
		}
		else if (item->GetItem() == JEWEL::CHAOS)
		{
			ChaosCount += item->GetDurability();
		}
	}

	if ( UniriaCount <= 0 || ChaosCount <= 0 )
	{
		this->ChaosMixLogLackingMixItems();
		return;
	}

	if ( (UniriaCount / 10) != ChaosCount )
	{
		this->ChaosMixLogLackingMixItems();
		return;
	}

	if ( (UniriaCount % 10) != 0 )
	{
		this->ChaosMixLogLackingMixItems();
		return;
	}

	int32 count = ChaosCount;

	if ( count != this->GetCount() || count > this->GetMaxCount() )
	{
		this->ChaosMixLogInvalidMixItems();
		return;
	}

	this->mix_price = 500000 * count;

	if ( !this->tributeTaxPrice() )
		return;

	this->GetPlayer()->GetMixInventory()->Clear();
	
	this->GenerateMassCombinationResult(count, 70, 100);
	
	for ( int32 i = 0; i < count; ++i )
	{
		this->SetSuccess(this->GetMassCombinationList(i) == 1 ? true: false);

		this->ChaosMixDebugMessage();

		if ( this->IsSuccess() )
		{
			uint8 option = 0;

			if (roll_chance_i(30))
			{
				option = 1 << Random(3);

				if ( Random(5) == 0 )
					option |= 1 << Random(3);
			}

			sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, Item(PET_DINORANT, 0, 255, 1, 0, option));
			this->ChaosMixLogSuccess();
		}
		else
		{
			if ( count == 1 )
			{
				this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
			}

			this->ChaosMixLogFail();
		}

		this->ChaosMixDBSave();
	}

	this->GetPlayer()->GetInterfaceState()->SetState(0);
}
	
void MixHandler::ChaosMixStatFruit(uint8 data)
{
	int32 CreationCount = 0;
	int32 ChaosCount = 0;

	chaos_box_loop(i)
	{
		Item const* item = this->GetPlayer()->GetMixInventory()->GetItem(i);

		if ( !item->IsItem() )
			continue;

		if ( item->GetItem() == JEWEL::CREATION)
		{
			CreationCount += item->GetDurability();
		}
		else if (item->GetItem() == JEWEL::CHAOS)
		{
			ChaosCount += item->GetDurability();
		}
	}

	if ( CreationCount <= 0 || ChaosCount <= 0 )
	{
		this->ChaosMixLogLackingMixItems();
		return;
	}

	if ( CreationCount != ChaosCount )
	{
		this->ChaosMixLogInvalidMixItems();
		return;
	}

	int32 count = CreationCount;

	if ( count != this->GetCount() || count > this->GetMaxCount() )
	{
		this->ChaosMixLogInvalidMixItems();
		return;
	}

	if ( count == 1 && !this->ChaosMixTalismanOfLuck() )
	{
		this->ChaosMixLogTooManyTalismanOfLuck();
		return;
	}

	this->mix_price = 3000000 * count;

	if ( !this->tributeTaxPrice() )
		return;

	this->GetPlayer()->GetMixInventory()->Clear();
	
	this->GenerateMassCombinationResult(count, 90 + this->GetLuckyCharm(), 100);

	for ( int32 i = 0; i < count; ++i )
	{
		this->SetSuccess(this->GetMassCombinationList(i) == 1 ? true: false);

		this->ChaosMixDebugMessage();

		if ( this->IsSuccess() )
		{
			sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, Item(ITEMGET(13,15), Random<uint8>(5)));
			this->ChaosMixLogSuccess();
		}
		else
		{
			if ( count == 1 )
			{
				this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
			}

			this->ChaosMixLogFail();
		}

		this->ChaosMixDBSave();
	}

	this->GetPlayer()->GetInterfaceState()->SetState(0);
}

void MixHandler::ChaosMixWingSecond(uint8 data)
{
	uint32 jewel_chaos = 0;
	int64 price_tmp = 0;
	uint32 feather_count = 0;
	uint32 wing_count = 0;
	int64 wing_price = 0;
	uint32 talisman_wing_count = 0;
	uint32 talisman_wing_type = 0;

	chaos_box_loop(i)
	{
		Item const& item = this->GetPlayer()->GetMixInventory()->item[i];

		if ( !item.IsItem() )
			continue;

		if ( item.GetItem() == JEWEL::CHAOS )
		{
			jewel_chaos += item.GetDurability();
		}
		else if ( item.IsWingLvl1() )
		{
			wing_count++;
			wing_price += item.GetBuyPrice();
		}
		else if ( item.GetItem() >= ITEMGET(13,88) && item.GetItem() <= ITEMGET(13,92) )
		{
			talisman_wing_count++;
			talisman_wing_type = item.GetItem() - ITEMGET(13,88);
		}
		else if ( item.GetItem() == ITEMGET(13,14) && item.GetLevel() == 0 )
		{
			feather_count++;
		}
	}

	if ( jewel_chaos < 1 || wing_count < 1 || feather_count < 1 )
	{
		this->ChaosMixLogLackingMixItems();
		return;
	}

	if ( jewel_chaos > 1 || wing_count > 1 || feather_count > 1 || talisman_wing_count > 1 )
	{
		this->ChaosMixLogTooManyMixItems();
		return;
	}

	if ( !this->ChaosMixTalismanOfLuck() )
	{
		this->ChaosMixLogTooManyTalismanOfLuck();
		return;
	}

	this->mix_price = 5000000;

	if ( !this->tributeTaxPrice() )
		return;

	this->success_rate.set(wing_price / 4000000);
	this->success_rate.op_add(price_tmp / 40000);
	this->success_rate.op_add(this->GetSpiritStoneRate(SPIRIT_STONE_FLAG_MEDIUM));

	this->success_rate.limit_min(0);
	this->success_rate.limit_max(90);
	this->success_rate.op_add(this->GetLuckyCharm());
	this->success_rate.limit_max(100);

	this->ChaosMixDebugMessage();

	if ( this->roll_success() )
	{
		uint8 opt_rate = Random<uint8>(100);
		uint8 opt_type = Random<uint8>(3);
		
		uint8 exe = 0;
		uint8 luck = Random<uint8>(5) == 0;
		uint8 option = (opt_type == 0 && opt_rate < 30)?3:
					   (opt_type == 1 && opt_rate < 45)?2:
					   (opt_type == 2 && opt_rate < 60)?1:
					   0;

		if ( Random<uint8>(5) == 0 )
			exe = 1 << Random<uint8>(3);

		if ( Random<uint8>(2) )
			exe |= 0x20;

		uint16 wing = 0;

		if ( talisman_wing_count )
		{
			wing = (talisman_wing_type == 0)?ITEMGET(12,5):
				   (talisman_wing_type == 1)?ITEMGET(12,4):
				   (talisman_wing_type == 2)?ITEMGET(12,3):
				   (talisman_wing_type == 3)?ITEMGET(12,42):
				   (talisman_wing_type == 4)?ITEMGET(12,6):
				   ITEMGET(12,42);
		}
		else
		{
			uint8 wing_rand = Random<uint8>(5);

			wing = (wing_rand == 0)?ITEMGET(12,5):
				   (wing_rand == 1)?ITEMGET(12,4):
				   (wing_rand == 2)?ITEMGET(12,3):
				   (wing_rand == 3)?ITEMGET(12,42):
				   (wing_rand == 4)?ITEMGET(12,6):
				   ITEMGET(12,42);
		}

		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, Item(wing, 0, 0, 0, luck, option, exe), 1);
		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
	}

	this->ChaosMixDBSave();
}
	
void MixHandler::ChaosMixBloodCastle(uint8 data)
{
	int32 TicketPart1Count = 0;
	int32 TicketPart2Count = 0;
	int32 TicketPart1Level = 0;
	int32 TicketPart2Level = 0;
	int32 ChaosCount = 0;

	chaos_box_loop(i)
	{
		Item const* item = this->GetPlayer()->GetMixInventory()->GetItem(i);

		if ( !item->IsItem() )
			continue;

		if ( item->GetItem() == ITEMGET(13, 16) )
		{
			TicketPart1Count++;

			if ( TicketPart1Level == 0 )
			{
				TicketPart1Level = item->GetLevel();
			}
			else if ( TicketPart1Level != item->GetLevel() )
			{
				TicketPart1Level = -1;
			}
		}
		else if ( item->GetItem() == ITEMGET(13, 17) )
		{
			TicketPart2Count++;

			if ( TicketPart2Level == 0 )
			{
				TicketPart2Level = item->GetLevel();
			}
			else if ( TicketPart2Level != item->GetLevel() )
			{
				TicketPart2Level = -1;
			}
		}
		else if (item->GetItem() == JEWEL::CHAOS)
		{
			ChaosCount += item->GetDurability();
		}
	}

	if ( TicketPart1Count <= 0 || TicketPart2Count <= 0 || ChaosCount <= 0 )
	{
		this->ChaosMixLogLackingMixItems();
		return;
	}

	if ( TicketPart1Level == 0 || TicketPart1Level == -1 || TicketPart2Level == 0 || TicketPart2Level == -1 )
	{
		this->ChaosMixLogInvalidMixItems();
		return;
	}

	if ( TicketPart1Level != TicketPart2Level )
	{
		this->ChaosMixLogInvalidMixItemLevel();
		return;
	}

	if ( (TicketPart1Level - 1) > MAX_BLOOD_CASTLE_GROUND )
	{
		this->ChaosMixLogInvalidMixItemLevel();
		return;
	}

	if ( TicketPart1Count != TicketPart2Count )
	{
		this->ChaosMixLogInvalidMixItems();
		return;
	}

	int32 count = ChaosCount;
	
	if ( TicketPart1Count != count )
	{
		this->ChaosMixLogInvalidMixItems();
		return;
	}

	if ( count != this->GetCount() || count > this->GetMaxCount() )
	{
		this->ChaosMixLogInvalidMixItems();
		return;
	}

	if ( count == 1 && !this->ChaosMixTalismanOfLuck() )
	{
		this->ChaosMixLogTooManyTalismanOfLuck();
		return;
	}

	this->mix_price = m_BloodCastleMixInfo[TicketPart1Level - 1].price * count;

	if ( !this->tributeTaxPrice() )
		return;

	this->GetPlayer()->GetMixInventory()->Clear();

	this->GenerateMassCombinationResult(count, m_BloodCastleMixInfo[TicketPart1Level - 1].rate + this->GetLuckyCharm(), 100);

	for ( int32 i = 0; i < count; ++i )
	{
		this->SetSuccess(this->GetMassCombinationList(i) == 1 ? true: false);

		this->ChaosMixDebugMessage();

		if ( this->IsSuccess() )
		{
			sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, Item(ITEMGET(13, 18), TicketPart1Level));
			this->ChaosMixLogSuccess();
		}
		else
		{
			if ( count == 1 )
			{
				this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
			}

			this->ChaosMixLogFail();
		}

		this->ChaosMixDBSave();
	}

	this->GetPlayer()->GetInterfaceState()->SetState(0);
}

void MixHandler::ChaosMixWingFirst(uint8 data)
{
	uint32 jewel_chaos = 0;
	int64 price_tmp = 0;
	uint32 chaos_weapon = 0;
	uint32 talisman_wing_count = 0;
	uint32 talisman_wing_type = 0;

	chaos_box_loop(i)
	{
		Item const& item = this->GetPlayer()->GetMixInventory()->item[i];

		if ( !item.IsItem() )
			continue;

		if ( item.GetItem() == JEWEL::CHAOS )
		{
			jewel_chaos += item.GetDurability();
			price_tmp += item.GetOldPrice();
		}
		else if ( item.GetItem() == JEWEL::BLESS )
		{
			price_tmp += item.GetOldPrice();
		}
		else if ( item.GetItem() == JEWEL::SOUL )
		{
			price_tmp += item.GetOldPrice();
		}
		else if ( item.GetItem() >= ITEMGET(13,83) && item.GetItem() <= ITEMGET(13,86) )
		{
			talisman_wing_count++;
			talisman_wing_type = item.GetItem() - ITEMGET(13,83);
		}
		else if ( item.GetItem() >= ITEMGET(0,0) && item.GetItem() < ITEMGET(12,0) &&
				  item.GetLevel() >= 4 && item.GetOption() >= 1 )
		{
			price_tmp += item.GetOldPrice();

			if ( item.GetItem() == ITEMGET(2, 6) || item.GetItem() == ITEMGET(4, 6) || item.GetItem() == ITEMGET(5, 7) )
			{
				chaos_weapon++;
			}
		}
	}

	if ( jewel_chaos < 1 || chaos_weapon < 1 )
	{
		this->ChaosMixLogLackingMixItems();
		return;
	}

	if ( !this->ChaosMixTalismanOfLuck() )
	{
		this->ChaosMixLogTooManyTalismanOfLuck();
		return;
	}

	if ( price_tmp > 2000000 )
		price_tmp = 2000000;

	this->success_rate.set(price_tmp / 20000);
	this->success_rate.op_add(this->GetSpiritStoneRate(SPIRIT_STONE_FLAG_LESSER));
	this->success_rate.op_add(this->GetLuckyCharm());
	this->success_rate.limit_min(0);
	this->success_rate.limit_max(100);

	this->mix_price = this->success_rate.get() * 10000;

	if ( !this->tributeTaxPrice() )
		return;

	this->ChaosMixDebugMessage();

	if ( this->roll_success() )
	{
		uint8 opt_rate = Random<uint8>(100);
		uint8 opt_type = Random<uint8>(3);
		
		uint8 luck = (RAND%100) < ((this->success_rate.get() / 5) + 4);
		uint8 option = (opt_type == 0 && opt_rate < ((this->success_rate.get() / 5) + 4)) ? 3:
					   (opt_type == 1 && opt_rate < ((this->success_rate.get() / 5) + 8)) ? 2:
					   (opt_type == 2 && opt_rate < ((this->success_rate.get() / 5) + 12)) ? 1:
					   0;

		uint16 wing = 0;

		if ( talisman_wing_count )
		{
			wing = (talisman_wing_type == 0)?ITEMGET(12,2):
				   (talisman_wing_type == 1)?ITEMGET(12,1):
				   (talisman_wing_type == 2)?ITEMGET(12,0):
				   (talisman_wing_type == 3)?ITEMGET(12,41):
				   ITEMGET(12,41);
		}
		else
		{
			uint8 wing_rand = Random<uint8>(4);

			wing = (wing_rand == 0)?ITEMGET(12,2):
				   (wing_rand == 1)?ITEMGET(12,1):
				   (wing_rand == 2)?ITEMGET(12,0):
				   (wing_rand == 3)?ITEMGET(12,41):
				   ITEMGET(12,41);
		}

		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, Item(wing, 0, 0, 0, luck, option), 1);
		this->ChaosMixLogSuccess();
	}
	else
	{
		this->GetPlayer()->GetInterfaceState()->SetState(0);
		this->ChaosMixItemDown();
		this->GetPlayer()->ChaosMixSend(0);
		this->GetPlayer()->ChaosMixResult(CHAOS_MIX_RESULT_FAIL);
		this->ChaosMixLogFail();
	}

	this->ChaosMixDBSave();
}

void MixHandler::ChaosMixDarkHorse(uint8 data)
{
	int32 JewelChaos = 0;
	int32 JewelBless = 0;
	int32 JewelSoul = 0;
	int32 JewelCreation = 0;
	int32 Spirit = 0;

	chaos_box_loop(i)
	{
		Item const& item = this->GetPlayer()->GetMixInventory()->item[i];

		if (!item.IsItem())
			continue;

		if (item.GetItem() == JEWEL::CHAOS)
		{
			JewelChaos += item.GetDurability();
		}
		else if (item.GetItem() == JEWEL::BLESS)
		{
			JewelBless += item.GetDurability();
		}
		else if (item.GetItem() == JEWEL::SOUL)
		{
			JewelSoul += item.GetDurability();
		}
		else if (item.GetItem() == JEWEL::CREATION)
		{
			JewelCreation += item.GetDurability();
		}
		else if (item.GetItem() == ITEMGET(13, 31) && item.GetLevel() == 0)
		{
			Spirit++;
		}
	}

	if (JewelChaos < 1 || JewelBless < 5 || JewelSoul < 5 || JewelCreation < 1 || Spirit < 1)
	{
		this->ChaosMixLogLackingMixItems();
		return;
	}

	if (JewelChaos > 1 || JewelBless > 5 || JewelSoul > 5 || JewelCreation > 1 || Spirit > 1)
	{
		this->ChaosMixLogTooManyMixItems();
		return;
	}

	if (!this->ChaosMixTalismanOfLuck())
	{
		this->ChaosMixLogTooManyTalismanOfLuck();
		return;
	}

	this->success_rate.set(60);
	this->success_rate.op_add(this->GetLuckyCharm());
	this->mix_price = 5000000;

	if (!this->tributeTaxPrice())
	{
		return;
	}

	this->ChaosMixDebugMessage();

	if ( this->roll_success() )
	{
		uint8 skill = ((RAND%100) < (this->success_rate.get()/5 + 6));
		uint8 luck = ((RAND%5) == 0);
		uint8 rand_1 = Random<uint8>(100);
		uint8 rand_2 = Random<uint8>(3);
		uint8 option = (rand_2 == 0 && rand_1 < 4)?3: (rand_2 == 1 && rand_1 < 10)?2: (rand_2 == 2 && rand_1 < 20)?1: 0;

		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, Item(PET_DARK_HORSE, 0, 0, skill, luck, option), 1);
		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL, true, 1);
	}

	this->ChaosMixDBSave();
}
	
void MixHandler::ChaosMixDarkSpirit(uint8 data)
{
	int32 JewelChaos = 0;
	int32 JewelBless = 0;
	int32 JewelSoul = 0;
	int32 JewelCreation = 0;
	int32 Spirit = 0;

	chaos_box_loop(i)
	{
		Item const& item = this->GetPlayer()->GetMixInventory()->item[i];

		if (!item.IsItem())
			continue;

		if (item.GetItem() == JEWEL::CHAOS)
		{
			JewelChaos += item.GetDurability();
		}
		else if (item.GetItem() == JEWEL::BLESS)
		{
			JewelBless += item.GetDurability();
		}
		else if (item.GetItem() == JEWEL::SOUL)
		{
			JewelSoul += item.GetDurability();
		}
		else if (item.GetItem() == JEWEL::CREATION)
		{
			JewelCreation += item.GetDurability();
		}
		else if (item.GetItem() == ITEMGET(13, 31) && item.GetLevel() == 1)
		{
			Spirit++;
		}
	}

	if (JewelChaos < 1 || JewelBless < 2 || JewelSoul < 2 || JewelCreation < 1 || Spirit < 1)
	{
		this->ChaosMixLogLackingMixItems();
		return;
	}

	if (JewelChaos > 1 || JewelBless > 2 || JewelSoul > 2 || JewelCreation > 1 || Spirit > 1)
	{
		this->ChaosMixLogTooManyMixItems();
		return;
	}

	if (!this->ChaosMixTalismanOfLuck())
	{
		this->ChaosMixLogTooManyTalismanOfLuck();
		return;
	}

	this->success_rate.set(60);
	this->success_rate.op_add(this->GetLuckyCharm());
	this->mix_price = 1000000;

	if (!this->tributeTaxPrice())
	{
		return;
	}

	this->ChaosMixDebugMessage();

	if ( this->roll_success() )
	{
		uint8 skill = ((RAND%100) < (this->success_rate.get()/5 + 6));
		uint8 luck = ((RAND%5) == 0);
		uint8 rand_1 = Random<uint8>(100);
		uint8 rand_2 = Random<uint8>(3);
		uint8 option = (rand_2 == 0 && rand_1 < 4)?3: (rand_2 == 1 && rand_1 < 10)?2: (rand_2 == 2 && rand_1 < 20)?1: 0;

		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, Item(ITEMGET(13,5), 0, 0, skill, luck, option), 1);
		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL, true, 1);
	}

	this->ChaosMixDBSave();
}

void MixHandler::ChaosMixBlessPotion(uint8 data)
{
	int32 BlessCount = 0;
	
	chaos_box_loop(i)
	{
		Item const* item = this->GetPlayer()->GetMixInventory()->GetItem(i);

		if ( !item->IsItem() )
			continue;

		if (item->GetItem() == JEWEL::BLESS)
		{
			BlessCount += item->GetDurability();
		}
	}

	if ( BlessCount <= 0 )
	{
		this->ChaosMixLogLackingMixItems();
		return;
	}

	int32 count = BlessCount;

	if ( count != this->GetCount() || count > this->GetMaxCount() )
	{
		this->ChaosMixLogInvalidMixItems();
		return;
	}

	this->mix_price = 100000 * count;

	if ( !this->tributeTaxPrice() )
		return;

	this->GetPlayer()->GetMixInventory()->Clear();
		
	this->GenerateMassCombinationResult(count, 100, 100);

	for ( int32 i = 0; i < count; ++i )
	{
		this->SetSuccess(this->GetMassCombinationList(i) == 1 ? true: false);

		this->ChaosMixDebugMessage();

		if ( this->IsSuccess() )
		{
			sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, Item(ITEMGET(14, 7), 0, 10.0f));
			this->ChaosMixLogSuccess();
		}
		else
		{
			if ( count == 1 )
			{
				this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
			}

			this->ChaosMixLogFail();
		}

		this->ChaosMixDBSave();
	}

	this->GetPlayer()->GetInterfaceState()->SetState(0);
}
	
void MixHandler::ChaosMixSoulPotion(uint8 data)
{
	int32 SoulCount = 0;
	
	chaos_box_loop(i)
	{
		Item const* item = this->GetPlayer()->GetMixInventory()->GetItem(i);

		if ( !item->IsItem() )
			continue;

		if (item->GetItem() == JEWEL::SOUL)
		{
			SoulCount += item->GetDurability();
		}
	}

	if ( SoulCount <= 0 )
	{
		this->ChaosMixLogLackingMixItems();
		return;
	}

	int32 count = SoulCount;

	if ( count != this->GetCount() || count > this->GetMaxCount() )
	{
		this->ChaosMixLogInvalidMixItems();
		return;
	}

	this->mix_price = 50000 * count;

	if ( !this->tributeTaxPrice() )
		return;

	this->GetPlayer()->GetMixInventory()->Clear();
	
	this->GenerateMassCombinationResult(count, 100, 100);

	for ( int32 i = 0; i < count; ++i )
	{
		this->SetSuccess(this->GetMassCombinationList(i) == 1 ? true: false);

		this->ChaosMixDebugMessage();

		if ( this->IsSuccess() )
		{
			sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, Item(ITEMGET(14, 7), 1, 10.0f));
			this->ChaosMixLogSuccess();
		}
		else
		{
			if ( count == 1 )
			{
				this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
			}

			this->ChaosMixLogFail();
		}

		this->ChaosMixDBSave();
	}

	this->GetPlayer()->GetInterfaceState()->SetState(0);
}

void MixHandler::ChaosMixLifeStone(uint8 data)
{
	int32 JewelChaos = 0;
	int32 JewelBless = 0;
	int32 JewelSoul = 0;
	int32 JewelGuardian = 0;

	chaos_box_loop(i)
	{
		Item const& item = this->GetPlayer()->GetMixInventory()->item[i];

		if (!item.IsItem())
			continue;

		if (item.GetItem() == JEWEL::CHAOS)
		{
			JewelChaos += item.GetDurability();
		}
		else if (item.GetItem() == JEWEL::BLESS)
		{
			JewelBless += item.GetDurability();
		}
		else if (item.GetItem() == JEWEL::SOUL)
		{
			JewelSoul += item.GetDurability();
		}
		else if (item.GetItem() == JEWEL::GUARDIAN)
		{
			JewelGuardian += item.GetDurability();
		}
	}

	if (JewelChaos < 1 || JewelBless < 5 || JewelSoul < 5 || JewelGuardian < 1)
	{
		this->ChaosMixLogLackingMixItems();
		return;
	}

	if (JewelChaos > 1 || JewelBless > 5 || JewelSoul > 5 || JewelGuardian > 1)
	{
		this->ChaosMixLogTooManyMixItems();
		return;
	}

	this->success_rate.set(100);
	this->mix_price = 5000000;

	if (!this->tributeTaxPrice())
	{
		return;
	}

	this->ChaosMixDebugMessage();

	if ( this->roll_success() )
	{
		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, Item(ITEMGET(13, 11), 1), 1);
		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
	}

	this->ChaosMixDBSave();
}
	
void MixHandler::ChaosMixSiegeLord(uint8 data)
{
	if ( !sCastleSiege->CastleOwnerMember(this->GetPlayer()) )
	{
		this->GetPlayer()->GetInterfaceState()->SetState(0);
		this->GetPlayer()->ChaosMixResult(CHAOS_MIX_RESULT_FAIL);
		this->ChaosMixLogFail();
		return;
	}

	Guild* pGuild = this->GetPlayer()->GuildGet();

	if ( !pGuild || pGuild->GetMemberRanking(this->GetPlayer()) != GUILD_RANK_MASTER )
	{
		this->GetPlayer()->GetInterfaceState()->SetState(0);
		this->GetPlayer()->ChaosMixResult(CHAOS_MIX_RESULT_FAIL);
		this->ChaosMixLogFail();
		return;
	}

	if ( g_SiegeLordMixCount <= 0 )
	{
		this->GetPlayer()->GetInterfaceState()->SetState(0);
		this->GetPlayer()->ChaosMixResult(CHAOS_MIX_RESULT_FAIL);
		this->ChaosMixLogFail();
		return;
	}

	uint32 jewel_soul = 0;
	uint32 jewel_bless = 0;
	uint32 jewel_guardian = 0;

	chaos_box_loop(i)
	{
		Item const& item = this->GetPlayer()->GetMixInventory()->item[i];

		if ( !item.IsItem() )
			continue;

		if ( item.GetItem() == JEWEL::BLESS)
		{
			jewel_bless += item.GetDurability();
		}
		else if ( item.GetItem() == JEWEL::SOUL)
		{
			jewel_soul += item.GetDurability();
		}
		else if ( item.GetItem() == JEWEL::GUARDIAN )
		{
			jewel_guardian += item.GetDurability();
		}
	}

	if ( jewel_soul < 30 || jewel_bless < 30 || jewel_guardian < 30 )
	{
		this->ChaosMixLogLackingMixItems();
		return;
	}

	if ( jewel_soul > 30 || jewel_bless > 30 || jewel_guardian > 30 )
	{
		this->ChaosMixLogTooManyMixItems();
		return;
	}

	this->mix_price = 1000000000;

	if ( !this->tributeTaxPrice(false) )
	{
		return;
	}

	this->success_rate.set(100);
	this->success_rate.limit_min(0);
	this->success_rate.limit_max(100);

	this->ChaosMixDebugMessage();

	if ( this->roll_success() )
	{
		if ( sItemBagMgr->ExecuteItemBag(ITEM_BAG_MIX, this->GetPlayer(), "", this->GetMixID()) == ITEM_BAG_RESULT_SUCCESS )
		{
			this->ChaosMixLogSuccess();
		}
		else
		{
			this->GetPlayer()->GetInterfaceState()->SetState(0);
			this->GetPlayer()->ChaosMixResult(CHAOS_MIX_RESULT_FAIL);
			this->ChaosMixLogFail();
			return;
		}
	}
	else
	{
		this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
	}

	g_SiegeLordMixCount--;

	this->ChaosMixDBSave();
}

void MixHandler::ChaosMixUpgrade12(uint8 data)
{
	this->ChaosMixUpgradeLevel(data, 2);
}
	
void MixHandler::ChaosMixUpgrade13(uint8 data)
{
	this->ChaosMixUpgradeLevel(data, 3);
}

void MixHandler::ChaosMixCloak(uint8 data)
{
	uint32 jewel_chaos = 0;
	int64 price_tmp = 0;
	uint32 feather_count = 0;
	uint32 wing_count = 0;
	int64 wing_price = 0;
	uint32 talisman_wing_count = 0;
	uint32 talisman_wing_type = 0;

	chaos_box_loop(i)
	{
		Item const& item = this->GetPlayer()->GetMixInventory()->item[i];

		if ( !item.IsItem() )
			continue;

		if ( item.GetItem() == JEWEL::CHAOS )
		{
			jewel_chaos += item.GetDurability();
		}
		else if ( item.IsWingLvl1() )
		{
			wing_count++;
			wing_price += item.GetBuyPrice();
		}
		else if ( item.GetItem() == ITEMGET(13,87) )
		{
			talisman_wing_count++;
			talisman_wing_type = 0;
		}
		else if ( item.GetItem() == ITEMGET(13,14) && item.GetLevel() == 1 )
		{
			feather_count++;
		}
		else if ( item.GetItem() >= ITEMGET(0,0) && item.GetItem() < ITEMGET(12,0) &&
			item.GetLevel() >= 4 && item.GetExe())
		{
			price_tmp += item.GetBuyPrice();
		}
	}

	if ( jewel_chaos < 1 || wing_count < 1 || feather_count < 1 )
	{
		this->ChaosMixLogLackingMixItems();
		return;
	}

	if ( jewel_chaos > 1 || wing_count > 1 || feather_count > 1 || talisman_wing_count > 1 )
	{
		this->ChaosMixLogTooManyMixItems();
		return;
	}

	if ( !this->ChaosMixTalismanOfLuck() )
	{
		this->ChaosMixLogTooManyTalismanOfLuck();
		return;
	}

	this->mix_price = 5000000;

	if ( !this->tributeTaxPrice() )
		return;

	this->success_rate.set(wing_price / 4000000);
	this->success_rate.op_add(price_tmp / 40000);
	this->success_rate.limit_min(0);
	this->success_rate.limit_max(90);
	this->success_rate.op_add(this->GetLuckyCharm());
	this->success_rate.limit_max(100);

	this->ChaosMixDebugMessage();

	if ( this->roll_success() )
	{
		uint8 opt_rate = Random<uint8>(100);
		uint8 opt_type = Random<uint8>(3);
		
		uint8 exe = 0;
		uint8 luck = Random(5) == 0;
		uint8 option = (opt_type == 0 && opt_rate < 4)?3:
					   (opt_type == 1 && opt_rate < 10)?2:
					   (opt_type == 2 && opt_rate < 20)?1:
					   0;

		if ( Random(5) == 0 )
			exe = 1 << Random(3);

		exe |= 0x20;

		uint16 wing = 0;

		if ( talisman_wing_count )
		{
			wing = this->GetPlayer()->GetClass() == Character::RAGE_FIGHTER ? ITEMGET(12, 49):
				   this->GetPlayer()->GetClass() == Character::GROW_LANCER ? ITEMGET(12, 269):
				   ITEMGET(13, 30);
		}
		else
		{
			uint8 wing_rand = Random<uint8>(3);

			wing = (wing_rand == 0) ? ITEMGET(13, 30):
				   (wing_rand == 1) ? ITEMGET(12, 49):
				   ITEMGET(12, 269);
		}

		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, Item(wing, 0, 0, 0, luck, option, exe), 1);
		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
	}

	this->ChaosMixDBSave();
}

void MixHandler::ChaosMixFenrirFragment(uint8 data)
{
	int32 JewelChaos = 0;
	int32 Fragment01 = 0;
	int32 Fragment02 = 0;

	chaos_box_loop(i)
	{
		Item const& item = this->GetPlayer()->GetMixInventory()->item[i];

		if (!item.IsItem())
			continue;

		if (item.GetItem() == JEWEL::CHAOS)
		{
			JewelChaos += item.GetDurability();
		}
		else if (item.GetItem() == ITEMGET(13, 32))
		{
			Fragment01 += item.GetDurability();
		}
		else if (item.GetItem() == ITEMGET(13, 33))
		{
			Fragment02 += item.GetDurability();
		}
	}

	if (JewelChaos < 1 || Fragment01 < 20 || Fragment02 < 20)
	{
		this->ChaosMixLogLackingMixItems();
		return;
	}

	if (JewelChaos > 1 || Fragment01 > 20 || Fragment02 > 20)
	{
		this->ChaosMixLogTooManyMixItems();
		return;
	}

	if (!this->ChaosMixTalismanOfLuck())
	{
		this->ChaosMixLogTooManyTalismanOfLuck();
		return;
	}

	this->success_rate.set(70);
	this->success_rate.op_add(this->GetLuckyCharm());

	this->mix_price = 0;

	if (!this->tributeTaxPrice())
	{
		return;
	}

	this->ChaosMixDebugMessage();

	if ( this->roll_success() )
	{
		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, Item(ITEMGET(13, 35), 0, 1.0f), 1);
		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
	}

	this->ChaosMixDBSave();
}
	
void MixHandler::ChaosMixFenrirBrokenHorn(uint8 data)
{
	int32 JewelChaos = 0;
	int32 ClawBeast = 0;
	int32 Fragment = 0;

	chaos_box_loop(i)
	{
		Item const& item = this->GetPlayer()->GetMixInventory()->item[i];

		if (!item.IsItem())
			continue;

		if (item.GetItem() == JEWEL::CHAOS)
		{
			JewelChaos += item.GetDurability();
		}
		else if (item.GetItem() == ITEMGET(13, 34))
		{
			ClawBeast += item.GetDurability();
		}
		else if (item.GetItem() == ITEMGET(13, 35))
		{
			Fragment++;
		}
	}

	if (JewelChaos < 1 || ClawBeast < 10 || Fragment < 5)
	{
		this->ChaosMixLogLackingMixItems();
		return;
	}

	if (JewelChaos > 1 || ClawBeast > 10 || Fragment > 5)
	{
		this->ChaosMixLogTooManyMixItems();
		return;
	}

	if (!this->ChaosMixTalismanOfLuck())
	{
		this->ChaosMixLogTooManyTalismanOfLuck();
		return;
	}

	this->success_rate.set(50);
	this->success_rate.op_add(this->GetLuckyCharm());

	this->mix_price = 0;

	if (!this->tributeTaxPrice())
	{
		return;
	}

	this->ChaosMixDebugMessage();

	if ( this->roll_success() )
	{
		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, Item(ITEMGET(13, 36), 0, 1.0f), 1);
		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
	}

	this->ChaosMixDBSave();
}
	
void MixHandler::ChaosMixFenrir(uint8 data)
{
	int32 JewelChaos = 0;
	int32 JewelLife = 0;
	int32 BrokenHorn = 0;

	chaos_box_loop(i)
	{
		Item const& item = this->GetPlayer()->GetMixInventory()->item[i];

		if (!item.IsItem())
			continue;

		if (item.GetItem() == JEWEL::CHAOS)
		{
			JewelChaos += item.GetDurability();
		}
		else if (item.GetItem() == JEWEL::LIFE)
		{
			JewelLife += item.GetDurability();
		}
		else if (item.GetItem() == ITEMGET(13, 36))
		{
			BrokenHorn++;
		}
	}

	if (JewelChaos < 1 || JewelLife < 3 || BrokenHorn < 1)
	{
		this->ChaosMixLogLackingMixItems();
		return;
	}

	if (JewelChaos > 1 || JewelLife > 3 || BrokenHorn > 1)
	{
		this->ChaosMixLogTooManyMixItems();
		return;
	}

	if (!this->ChaosMixTalismanOfLuck())
	{
		this->ChaosMixLogTooManyTalismanOfLuck();
		return;
	}

	this->success_rate.set(30);
	this->success_rate.op_add(this->GetLuckyCharm());
	
	this->mix_price = 10000000;

	if (!this->tributeTaxPrice())
	{
		return;
	}

	this->ChaosMixDebugMessage();

	if ( this->roll_success() )
	{
		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, Item(PET_FENRIR, 0, 255.0f), 1);
		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
	}

	this->ChaosMixDBSave();
}
	
void MixHandler::ChaosMixFenrirUpgrade(uint8 data)
{
	int32 JewelChaos = 0;
	int32 JewelLife = 0;
	int32 FenrirHorn = 0;
	int64 Price = 0;
	uint8 FenrirType = -1;

	chaos_box_loop(i)
	{
		Item const& item = this->GetPlayer()->GetMixInventory()->item[i];

		if (!item.IsItem())
			continue;

		if (item.GetItem() == JEWEL::CHAOS)
		{
			JewelChaos += item.GetDurability();
		}
		else if (item.GetItem() == JEWEL::LIFE)
		{
			JewelLife += item.GetDurability();
		}
		else if (item.GetItem() == ITEMGET(13, 37))
		{
			FenrirHorn++;
		}
		else if (item.GetItem() >= ITEMGET(0, 0) && item.GetItem() < ITEMGET(6, 0))
		{
			if (FenrirType == 0 || FenrirType == uint8(-1))
			{
				FenrirType = 0;
				Price += item.GetSellPrice();
			}
		}
		else if (item.GetItem() >= ITEMGET(6, 0) && item.GetItem() < ITEMGET(12, 0))
		{
			if (FenrirType == 1 || FenrirType == uint8(-1))
			{
				FenrirType = 1;
				Price += item.GetSellPrice();
			}
		}
	}

	if (JewelChaos < 1 || JewelLife < 5 || FenrirHorn < 1 || FenrirType == uint8(-1))
	{
		this->ChaosMixLogLackingMixItems();
		return;
	}

	if (JewelChaos > 1 || JewelLife > 5 || FenrirHorn > 1)
	{
		this->ChaosMixLogTooManyMixItems();
		return;
	}

	if (!this->ChaosMixTalismanOfLuck())
	{
		this->ChaosMixLogTooManyTalismanOfLuck();
		return;
	}

	if (Price > 1000000)
	{
		Price = 1000000;
	}

	this->success_rate.set(Price * 100 / 1000000);
	this->success_rate.op_add(this->GetSpiritStoneRate(SPIRIT_STONE_FLAG_LESSER | SPIRIT_STONE_FLAG_MEDIUM | SPIRIT_STONE_FLAG_GREATER));
	this->success_rate.limit_max(79);
	this->success_rate.op_add(this->GetLuckyCharm());
	this->success_rate.limit_max(100);
	this->mix_price = 10000000;

	if (!this->tributeTaxPrice())
	{
		return;
	}

	this->ChaosMixDebugMessage();

	if (this->roll_success())
	{
		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, Item(PET_FENRIR, 0, 255.0f, 0, 0, 0, FenrirType + 1), 1);
		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
	}

	this->ChaosMixDBSave();
}

void MixHandler::ChaosMixSDLvl1(uint8 data)
{
	uint32 large_healing_potion = 0;

	chaos_box_loop(i)
	{
		Item const& item = this->GetPlayer()->GetMixInventory()->item[i];

		if ( !item.IsItem() )
			continue;

		if ( item.GetItem() == ITEMGET(14, 3) )
		{
			large_healing_potion += item.GetDurability();
		}
	}

	this->ChaosMixSD(large_healing_potion, 100000, 50, ITEMGET(14, 35));
}
		
void MixHandler::ChaosMixSDLvl2(uint8 data)
{
	uint32 compound_potion = 0;

	chaos_box_loop(i)
	{
		Item const& item = this->GetPlayer()->GetMixInventory()->item[i];

		if ( !item.IsItem() )
			continue;

		if ( item.GetItem() == ITEMGET(14, 38) )
		{
			compound_potion += item.GetDurability();
		}
	}

	this->ChaosMixSD(compound_potion, 500000, 30, ITEMGET(14, 36));
}
	
void MixHandler::ChaosMixSDLvl3(uint8 data)
{
	uint32 compound_potion = 0;

	chaos_box_loop(i)
	{
		Item const& item = this->GetPlayer()->GetMixInventory()->item[i];

		if ( !item.IsItem() )
			continue;

		if ( item.GetItem() == ITEMGET(14, 39) )
		{
			compound_potion += item.GetDurability();
		}
	}

	this->ChaosMixSD(compound_potion, 1000000, 30, ITEMGET(14, 37));
}

void MixHandler::ChaosMixSD(uint32 potion_count, uint32 price, int32 rate, uint16 item)
{
	if ( potion_count < 3 )
	{
		this->ChaosMixLogLackingMixItems();
		return;
	}

	if ( (potion_count % 3) != 0 )
	{
		this->ChaosMixLogInvalidMixItems();
		return;
	}

	int32 count = potion_count / 3;

	if ( count != this->GetCount() || count > this->GetMaxCount() )
	{
		this->ChaosMixLogInvalidMixItems();
		return;
	}

	if ( count == 1 && !this->ChaosMixTalismanOfLuck() )
	{
		this->ChaosMixLogTooManyTalismanOfLuck();
		return;
	}

	this->mix_price = price * count;

	if ( !this->tributeTaxPrice() )
		return;

	this->GetPlayer()->GetMixInventory()->Clear();
	
	this->GenerateMassCombinationResult(count, rate + this->GetLuckyCharm(), 100);
	
	for ( int32 i = 0; i < count; ++i )
	{
		this->SetSuccess(this->GetMassCombinationList(i) == 1 ? true: false);

		this->ChaosMixDebugMessage();

		if ( this->IsSuccess() )
		{
			sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, Item(item, 0, 1.0f));
			this->ChaosMixLogSuccess();
		}
		else
		{
			if ( count == 1 )
			{
				this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
			}

			this->ChaosMixLogFail();
		}

		this->ChaosMixDBSave();
	}

	this->GetPlayer()->GetInterfaceState()->SetState(0);
}

void MixHandler::ChaosMixHarmonyRefine(uint8 data)
{
	uint32 gem_stone = 0;

	chaos_box_loop(i)
	{
		Item const& item = this->GetPlayer()->GetMixInventory()->item[i];

		if ( !item.IsItem() )
		{
			continue;
		}

		if (item.GetItem() == JEWEL::GEMSTONE)
		{
			gem_stone += item.GetDurability();
		}
	}

	if ( gem_stone < 1 )
	{
		this->ChaosMixLogLackingMixItems();
		return;
	}

	if ( gem_stone > 32 )
	{
		this->ChaosMixLogTooManyMixItems();
		return;
	}

	this->mix_price = 0;

	if ( !this->tributeTaxPrice() )
	{
		return;
	}

	this->GetPlayer()->GetMixInventory()->Clear();
	
	this->GenerateMassCombinationResult(gem_stone, sGameServer->harmony_refine_rate.get(), 100);

	for ( int32 i = 0; i < gem_stone; ++i )
	{
		this->SetSuccess(this->GetMassCombinationList(i) == 1 ? true: false);

		this->ChaosMixDebugMessage();

		if ( this->IsSuccess() )
		{
			sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, Item(JEWEL::HARMONY));
			this->ChaosMixLogSuccess();
		}
		else
		{
			if ( gem_stone == 1 )
			{
				this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
			}

			this->ChaosMixLogFail();
		}

		this->ChaosMixDBSave();
	}

	this->GetPlayer()->GetInterfaceState()->SetState(0);
}
	
void MixHandler::ChaosMixHarmonyRefineItem(uint8 data)
{
	int32 ExcellentItem = 0;
	int32 NormalItem = 0;
	item_template const* item_info = nullptr;

	chaos_box_loop(i)
	{
		Item const& item = this->GetPlayer()->GetMixInventory()->item[i];

		if ( !item.IsItem() )
			continue;

		item_info = sItemMgr->GetItem(item.GetItem());

		if ( !item_info )
		{
			continue;
		}

		if ( item.GetHarmonyItem() == HARMONY_ITEM_NONE )
		{
			continue;
		}

		if ( item.GetLevel() < item_info->GetHarmonySmeltLevel() )
		{
			continue;
		}

		if (item.GetExe())
		{
			ExcellentItem++;
		}
		else
		{
			NormalItem++;
		}
	}

	if ( ExcellentItem <= 0 && NormalItem <= 0 )
	{
		this->ChaosMixLogLackingMixItems();
		return;
	}

	if ( ExcellentItem >= 1 && NormalItem >= 1 )
	{
		this->ChaosMixLogTooManyMixItems();
		return;
	}

	int32 count = ExcellentItem > 0 ? ExcellentItem : NormalItem;

	if ( count != this->GetCount() || count > this->GetMaxCount() )
	{
		this->ChaosMixLogTooManyMixItems();
		return;
	}

	this->mix_price = 0;

	if ( !this->tributeTaxPrice() )
	{
		return;
	}

	this->GetPlayer()->GetMixInventory()->Clear();

	this->GenerateMassCombinationResult(count, ExcellentItem > 0 ? sGameServer->harmony_smelt_exe_rate.get(): sGameServer->harmony_smelt_normal_rate.get(), 100);

	for ( int32 i = 0; i < count; ++i )
	{
		this->SetSuccess(this->GetMassCombinationList(i) == 1 ? true: false);

		this->ChaosMixDebugMessage();

		if ( this->IsSuccess() )
		{
			sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, Item(ExcellentItem > 0 ? JEWEL::HIGH_REFINE_STONE : JEWEL::LOW_REFINE_STONE));
			this->ChaosMixLogSuccess();
		}
		else
		{
			if ( count == 1 )
			{
				this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
			}

			this->ChaosMixLogFail();
		}

		this->ChaosMixDBSave();
	}

	this->GetPlayer()->GetInterfaceState()->SetState(0);
}
	
void MixHandler::ChaosMixHarmonyRestoreItem(uint8 data)
{
	uint32 refined_item = 0;
	uint8 refined_slot = 0;

	chaos_box_loop(i)
	{
		Item const& item = this->GetPlayer()->GetMixInventory()->item[i];

		if ( !item.IsItem() )
			continue;

		if ( item.IsHarmonyUpgrade() )
		{
			refined_item++;
			refined_slot = i;
		}
	}

	if ( refined_item < 1 )
	{
		this->ChaosMixLogLackingMixItems();
		return;
	}

	if ( refined_item > 1 )
	{
		this->ChaosMixLogTooManyMixItems();
		return;
	}

	Item * item = this->GetPlayer()->GetMixInventory()->GetItem(refined_slot);

	uint8 option = item->GetHarmonyOption();
	uint8 level = item->GetHarmonyLevel();
	HarmonyItemType type = item->GetHarmonyItem();

	item_harmony_data const* item_data = sItemMgr->GetHarmony(type, option);

	if ( !item_data )
	{
		this->ChaosMixLogLackingMixItems();
		return;
	}

	if ( !this->GetPlayer()->GetInventory()->IsEmptySpace(item->GetItem()) )
	{
		this->GetPlayer()->SendNotice(CUSTOM_MESSAGE_ID_RED, "You do not have enough space in your inventory.");
		this->ChaosMixLogLackingMixItems();
		return;
	}

	this->mix_price = item_data->GetRequiredZen(level);

	if ( !this->tributeTaxPrice() )
	{
		return;
	}

	this->success_rate.set(100);
	this->success_rate.limit_min(0);
	this->success_rate.limit_max(100);

	this->ChaosMixDebugMessage();

	if ( this->roll_success() )
	{
		item->SetHarmony(HARMONY_EFFECT_NONE);

		this->GetPlayer()->GetInterfaceState()->SetState(0);
		//this->GetPlayer()->ChaosMixSend(0);

		uint8 item_info[MAX_ITEM_INFO];
		item->ConvertToBuffer(item_info);
		this->GetPlayer()->ChaosMixResult(CHAOS_MIX_RESULT_SUCCESS, item_info, refined_slot);

		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
	}

	this->ChaosMixDBSave();
}

void MixHandler::ChaosMix380(uint8 data)
{
	if ( !sGameServer->option_380_enabled )
	{
		this->GetPlayer()->SendNotice(CUSTOM_MESSAGE_ID_RED, "Item 380 mix is disabled");
		this->GetPlayer()->GetInterfaceState()->SetState(0);
		this->GetPlayer()->ChaosMixResult(CHAOS_MIX_RESULT_FAIL);
		this->ChaosMixLogFail();
		return;
	}

	uint32 jewel_harmony = 0;
	uint32 jewel_guardian = 0;
	uint8 upgrade_item_slot = 0;
	uint32 upgrade_item_count = 0;

	chaos_box_loop(i)
	{
		Item const& item = this->GetPlayer()->GetMixInventory()->item[i];

		if ( !item.IsItem() )
			continue;

		item_add_option const* pItemData = sItemMgr->GetItemAddOption(item.GetItem());

		if ( item.GetItem() == JEWEL::HARMONY )
		{
			jewel_harmony += item.GetDurability();
		}
		else if ( item.GetItem() == JEWEL::GUARDIAN )
		{
			jewel_guardian += item.GetDurability();
		}
		else if ( pItemData && item.GetItem() < ITEMGET(12, 0) && !item.Is380Option() && item.GetLevel() >= 4 && item.GetOption() >= 1 )
		{
			upgrade_item_count++;
			upgrade_item_slot = i;
		}
	}

	if ( jewel_harmony < 1 || jewel_guardian < 1 || upgrade_item_count < 1 )
	{
		this->ChaosMixLogLackingMixItems();
		return;
	}

	if ( jewel_harmony > 1 || jewel_guardian > 1 || upgrade_item_count > 1 )
	{
		this->ChaosMixLogTooManyMixItems();
		return;
	}

	if ( !this->ChaosMixTalismanOfLuck() )
	{
		this->ChaosMixLogTooManyTalismanOfLuck();
		return;
	}

	this->mix_price = sGameServer->option_380_price.get();

	if ( !this->tributeTaxPrice() )
		return;

	if ( this->GetPlayer()->GetMixInventory()->GetItem(upgrade_item_slot)->GetLevel() <= 6 )
	{
		this->success_rate.set(sGameServer->option_380_rate_grade_1.get());
	}
	else if ( this->GetPlayer()->GetMixInventory()->GetItem(upgrade_item_slot)->GetLevel() <= 9 )
	{
		this->success_rate.set(sGameServer->option_380_rate_grade_2.get());
	}
	else if ( this->GetPlayer()->GetMixInventory()->GetItem(upgrade_item_slot)->GetLevel() <= 15 )
	{
		this->success_rate.set(sGameServer->option_380_rate_grade_3.get());
	}
	
	this->success_rate.op_add(this->GetLuckyCharm());
	this->success_rate.limit_min(0);
	this->success_rate.limit_max(100);

	this->ChaosMixDebugMessage();
	this->ChaosMix380ItemDelete();

	if ( this->roll_success() )
	{
		this->GetPlayer()->GetMixInventory()->GetItem(upgrade_item_slot)->Or380(0x80);
		this->GetPlayer()->GetMixInventory()->GetItem(upgrade_item_slot)->AlterDurability(3, 0.0f);
		this->GetPlayer()->GetMixInventory()->GetItem(upgrade_item_slot)->Convert();
			
		this->GetPlayer()->GetInterfaceState()->SetState(0);
		this->GetPlayer()->ChaosMixSend(0);
		this->ChaosMixLogSuccess();
	}
	else
	{
		this->GetPlayer()->GetInterfaceState()->SetState(0);
		this->GetPlayer()->ChaosMixSend(0);
		this->GetPlayer()->ChaosMixResult(CHAOS_MIX_RESULT_FAIL);
		this->ChaosMixLogFail();
	}

	this->ChaosMixDBSave();
}

void MixHandler::ChaosMixIllusionTemple(uint8 data)
{
	uint32 jewel_chaos = 0;
	uint32 old_scroll_count = 0;
	uint8 old_scroll_level = 0;
	uint32 sorcerer_covenant_count = 0;
	uint8 sorcerer_covenant_level = 0;

	chaos_box_loop(i)
	{
		Item const& item = this->GetPlayer()->GetMixInventory()->item[i];

		if ( !item.IsItem() )
			continue;

		if ( item.GetItem() == JEWEL::CHAOS )
		{
			jewel_chaos += item.GetDurability();
		}
		else if ( item.GetItem() == ITEMGET(13, 49) )
		{
			old_scroll_count++;
			old_scroll_level = item.GetLevel();
		}
		else if ( item.GetItem() == ITEMGET(13, 50) )
		{
			sorcerer_covenant_count++;
			sorcerer_covenant_level = item.GetLevel();
		}
	}

	if ( !jewel_chaos || !old_scroll_count || !sorcerer_covenant_count )
	{
		this->ChaosMixLogLackingMixItems();
		return;
	}

	if ( jewel_chaos > 1 || old_scroll_count > 1 || sorcerer_covenant_count > 1 )
	{
		this->ChaosMixLogTooManyMixItems();
		return;
	}

	if ( old_scroll_level != sorcerer_covenant_level )
	{
		this->ChaosMixLogInvalidMixItemLevel();
		return;
	}

	if ( !this->ChaosMixTalismanOfLuck() )
	{
		this->ChaosMixLogTooManyTalismanOfLuck();
		return;
	}

	switch ( old_scroll_level )
	{
	case 1:
		{
			this->mix_price = 3000000;
		} break;

	case 2:
		{
			this->mix_price = 5000000;
		} break;

	case 3:
		{
			this->mix_price = 7000000;
		} break;

	case 4:
		{
			this->mix_price = 9000000;
		} break;

	case 5:
		{
			this->mix_price = 11000000;
		} break;

	case 6:
		{
			this->mix_price = 13000000;
		} break;
	}

	if ( !this->tributeTaxPrice() )
		return;

	this->success_rate.set(70);

	if (sCrywolf->GetOccupationState() == CRYWOLF_OCCUPATION_SUCCESS && sGameServer->IsCrywolfBenefitApply())
	{
		this->success_rate.op_add(sGameServer->GetCrywolfBenefitPlusRate());
	}

	this->success_rate.op_add(this->GetLuckyCharm());
	this->success_rate.limit_min(0);
	this->success_rate.limit_max(100);

	this->ChaosMixDebugMessage();

	if ( this->roll_success() )
	{
		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, Item(ITEMGET(13, 51), old_scroll_level), 1);
		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
	}

	this->ChaosMixDBSave();
}

void MixHandler::ChaosMixCondorFeather(uint8 data)
{
	uint32 jewel_chaos = 0;
	uint32 jewel_creation = 0;
	uint32 jewel_soul_pack = 0;
	int64 price_tmp = 0;
	uint32 wing_count = 0;
	uint32 ancient_count = 1;

	chaos_box_loop(i)
	{
		Item const& item = this->GetPlayer()->GetMixInventory()->item[i];

		if ( !item.IsItem() )
			continue;

		if ( item.GetItem() == JEWEL::CHAOS )
		{
			jewel_chaos += item.GetDurability();
		}
		else if ( item.GetItem() == JEWEL::CREATION )
		{
			jewel_creation += item.GetDurability();
		}
		else if ( item.GetItem() == JEWEL::SOUL)
		{
			jewel_soul_pack += item.GetDurability();
		}
		else if ( item.IsWingLvl2() && item.GetLevel() >= 9 && item.GetOption() >= 1 )
		{
			wing_count++;
		}
	}

	if ( jewel_chaos < 1 || jewel_creation < 1 || jewel_soul_pack < 10 || wing_count < 1 || ancient_count < 1 )
	{
		this->ChaosMixLogLackingMixItems();
		return;
	}

	if ( jewel_chaos > 1 || jewel_creation > 1 || jewel_soul_pack > 10 || wing_count > 1 )
	{
		this->ChaosMixLogTooManyMixItems();
		return;
	}

	if ( !this->ChaosMixTalismanOfLuck() )
	{
		this->ChaosMixLogTooManyTalismanOfLuck();
		return;
	}

	this->success_rate.set((price_tmp / 100000) + 1);
	this->success_rate.op_add(this->GetSpiritStoneRate(SPIRIT_STONE_FLAG_GREATER));

	this->success_rate.limit_max(60);

	this->success_rate.op_add(this->GetLuckyCharm());
	this->success_rate.limit_min(0);
	this->success_rate.limit_max(70);

	this->mix_price = 200000 * this->success_rate.get();

	if ( !this->tributeTaxPrice() )
	{
		return;
	}

	this->ChaosMixDebugMessage();

	if ( this->roll_success() )
	{
		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, Item(ITEMGET(13, 53)), 1);
		this->ChaosMixLogSuccess();
	}
	else
	{
		this->GetPlayer()->GetInterfaceState()->SetState(0);
		this->ChaosMixCondorFeatherItemDown();
		this->GetPlayer()->ChaosMixSend(0);
		this->GetPlayer()->ChaosMixResult(CHAOS_MIX_RESULT_FAIL);
		this->ChaosMixLogFail();
	}

	this->ChaosMixDBSave();
}

void MixHandler::ChaosMixWingThird(uint8 data)
{
	uint32 jewel_chaos = 0;
	uint32 flame_count = 0;
	uint32 feather_count = 0;
	uint32 jewel_creation = 0;
	uint32 jewel_bless_pack = 0;
	uint32 jewel_soul_pack = 0;
	int64 price_tmp = 0;
	uint32 item_count = 0;

	chaos_box_loop(i)
	{
		Item const& item = this->GetPlayer()->GetMixInventory()->item[i];

		if ( !item.IsItem() )
			continue;

		if (item.GetItem() == JEWEL::CHAOS)
		{
			jewel_chaos += item.GetDurability();
		}
		else if (item.GetItem() == ITEMGET(13, 52))
		{
			flame_count++;
		}
		else if (item.GetItem() == ITEMGET(13, 53))
		{
			feather_count++;
		}
		else if (item.GetItem() == JEWEL::CREATION)
		{
			jewel_creation += item.GetDurability();
		}
		else if (item.GetItem() == JEWEL::BLESS)
		{
			jewel_bless_pack += item.GetDurability();
		}
		else if (item.GetItem() == JEWEL::SOUL)
		{
			jewel_soul_pack += item.GetDurability();
		}

		item_count = 1;
	}

	if ( jewel_chaos < 30 || flame_count < 1 || feather_count < 1 || jewel_creation < 30 || jewel_bless_pack < 30 || jewel_soul_pack < 30 || item_count < 1 )
	{
		this->ChaosMixLogLackingMixItems();
		return;
	}

	if ( jewel_chaos > 30 || flame_count > 1 || feather_count > 1 || jewel_creation > 30 || jewel_bless_pack > 30 || jewel_soul_pack > 30 )
	{
		this->ChaosMixLogTooManyMixItems();
		return;
	}

	if ( !this->ChaosMixTalismanOfLuck() )
	{
		this->ChaosMixLogTooManyTalismanOfLuck();
		return;
	}

	this->success_rate.set((price_tmp / 3000000) + 1);
	this->success_rate.op_add(this->GetSpiritStoneRate(SPIRIT_STONE_FLAG_MEDIUM));
	this->success_rate.limit_min(1);
	this->success_rate.limit_max(40);

	this->success_rate.op_add(this->GetLuckyCharm());
	this->success_rate.limit_max(50);

	this->mix_price = this->success_rate.get() * 200000;

	if ( !this->tributeTaxPrice() )
		return;

	this->ChaosMixDebugMessage();

	if ( this->roll_success() )
	{
		uint8 opt_rate = Random<uint8>(100);
		uint8 opt_type = Random<uint8>(3);
		
		uint8 exe = 0;
		uint8 luck = roll_chance_i(20) ? 1 : 0;
		uint8 option = (opt_type == 0 && opt_rate < 30)?3:
					   (opt_type == 1 && opt_rate < 45)?2:
					   (opt_type == 2 && opt_rate < 60)?1:
					   0;

		if ( (RAND%2) == 0 )
			exe = 1 << (RAND%4);

		exe |= 0x20;

		RandomValue<uint16> m_RandomValue(ITEMGET(12, 43));
		m_RandomValue.AddValue(ITEMGET(12, 36), 0);
		m_RandomValue.AddValue(ITEMGET(12, 37), 0);
		m_RandomValue.AddValue(ITEMGET(12, 38), 0);
		m_RandomValue.AddValue(ITEMGET(12, 39), 0);
		m_RandomValue.AddValue(ITEMGET(12, 40), 0);
		m_RandomValue.AddValue(ITEMGET(12, 43), 0);
		m_RandomValue.AddValue(ITEMGET(12, 50), 0);
		m_RandomValue.AddValue(ITEMGET(12, 270), 0);
		m_RandomValue.AddValue(ITEMGET(12, 467), 0);
		m_RandomValue.AddValue(ITEMGET(12, 472), 0);

		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, Item(m_RandomValue.GetRandomValue(RANDOM_POOL_RANDOM), 0, 0, 0, luck, option, exe), 1);
		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
	}

	this->ChaosMixDBSave();
}
	
void MixHandler::ChaosMixChaosCard(uint8 data)
{
	uint8 chaos_card_type = -1;

	chaos_box_loop(i)
	{
		Item const* item = this->GetPlayer()->GetMixInventory()->GetItem(i);

		if ( !item->IsItem() )
		{
			continue;
		}

		switch ( item->GetItem() )
		{
		case ITEMGET(14, 54): // Chaos Card
			{
				chaos_card_type = 0;
			} break;

		case ITEMGET(14, 92): // Chaos Card Gold
			{
				chaos_card_type = 1;
			} break;

		case ITEMGET(14, 93): // Chaos Card Rare
			{
				chaos_card_type = 2;
			} break;

		case ITEMGET(14, 95): // Chaos Card Mini
			{
				chaos_card_type = 3;
			} break;
		}
	}

	if ( chaos_card_type == uint8(-1) )
	{
		this->ChaosMixLogInvalidMixItems();
		return;
	}

	this->mix_price = 0;

	if ( !this->tributeTaxPrice(false) )
	{
		return;
	}

	this->success_rate.set(100);
	this->success_rate.limit_min(0);
	this->success_rate.limit_max(100);

	this->ChaosMixDebugMessage();

	if ( this->roll_success() )
	{
		if ( sItemBagMgr->ExecuteItemBag(ITEM_BAG_MIX, this->GetPlayer(), "", this->GetMixID(), chaos_card_type) == ITEM_BAG_RESULT_SUCCESS )
		{
			this->ChaosMixLogSuccess();
		}
		else
		{
			this->GetPlayer()->GetInterfaceState()->SetState(0);
			this->GetPlayer()->ChaosMixResult(CHAOS_MIX_RESULT_FAIL);
			this->ChaosMixLogFail();
		}
	}
	else
	{
		this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
	}

	this->ChaosMixDBSave();
}

void MixHandler::ChaosMixCherryBlossom(uint8 data)
{
	uint32 white_blossom_branch = 0;
	uint32 red_blossom_branch = 0;
	uint32 golden_blossom_branch = 0;
	uint8 blossom_type = 0;

	chaos_box_loop(i)
	{
		Item const& item = this->GetPlayer()->GetMixInventory()->item[i];

		if ( !item.IsItem() )
			continue;

		if ( item.GetItem() == ITEMGET(14, 88) )
		{
			white_blossom_branch += SCAST(uint32, item.GetDurability());
			blossom_type = 0;
		}
		else if (  item.GetItem() == ITEMGET(14, 89) )
		{
			red_blossom_branch += SCAST(uint32, item.GetDurability());
			blossom_type = 1;
		}
		else if ( item.GetItem() == ITEMGET(14, 90) )
		{
			golden_blossom_branch += SCAST(uint32, item.GetDurability());
			blossom_type = 2;
		}
	}

	if ( white_blossom_branch )
	{
		if ( red_blossom_branch || golden_blossom_branch )
		{
			this->ChaosMixLogInvalidMixItems();
			return;
		}

		item_template const* item_info = sItemMgr->GetItem(ITEMGET(14, 88));

		if ( !item_info || item_info->GetStackData()->GetMaxStack() > white_blossom_branch )
		{
			this->ChaosMixLogLackingMixItems();
			return;
		}
	}
	else if ( red_blossom_branch )
	{
		if ( white_blossom_branch || golden_blossom_branch )
		{
			this->ChaosMixLogInvalidMixItems();
			return;
		}

		item_template const* item_info = sItemMgr->GetItem(ITEMGET(14, 89));

		if ( !item_info || item_info->GetStackData()->GetMaxStack() > red_blossom_branch )
		{
			this->ChaosMixLogLackingMixItems();
			return;
		}
	}
	else if ( golden_blossom_branch )
	{
		if ( white_blossom_branch || red_blossom_branch )
		{
			this->ChaosMixLogInvalidMixItems();
			return;
		}

		item_template const* item_info = sItemMgr->GetItem(ITEMGET(14, 90));

		if ( !item_info || item_info->GetStackData()->GetMaxStack() > golden_blossom_branch )
		{
			this->ChaosMixLogLackingMixItems();
			return;
		}
	}
	else
	{
		this->ChaosMixLogLackingMixItems();
		return;
	}

	this->mix_price = 0;

	if ( !this->tributeTaxPrice(false) )
	{
		return;
	}

	this->success_rate.set(100);
	this->success_rate.limit_min(0);
	this->success_rate.limit_max(100);

	this->ChaosMixDebugMessage();

	if ( this->roll_success() )
	{
		if ( sItemBagMgr->ExecuteItemBag(ITEM_BAG_MIX, this->GetPlayer(), "", this->GetMixID(), blossom_type) == ITEM_BAG_RESULT_SUCCESS )
		{
			this->ChaosMixLogSuccess();
		}
		else
		{
			this->GetPlayer()->GetInterfaceState()->SetState(0);
			this->GetPlayer()->ChaosMixResult(CHAOS_MIX_RESULT_FAIL);
			this->ChaosMixLogFail();
			return;
		}
	}
	else
	{
		this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
	}

	this->ChaosMixDBSave();
}

void MixHandler::ChaosMixSecromicon(uint8 data)
{
	uint32 secromicon_part_1 = 0;
	uint32 secromicon_part_2 = 0;
	uint32 secromicon_part_3 = 0;
	uint32 secromicon_part_4 = 0;
	uint32 secromicon_part_5 = 0;
	uint32 secromicon_part_6 = 0;

	chaos_box_loop(i)
	{
		Item const& item = this->GetPlayer()->GetMixInventory()->item[i];

		if ( !item.IsItem() )
			continue;

		if ( item.GetItem() == SECROMICON::PART_1 )
		{
			secromicon_part_1++;
		}
		else if ( item.GetItem() == SECROMICON::PART_2 )
		{
			secromicon_part_2++;
		}
		else if ( item.GetItem() == SECROMICON::PART_3 )
		{
			secromicon_part_3++;
		}
		else if ( item.GetItem() == SECROMICON::PART_4 )
		{
			secromicon_part_4++;
		}
		else if ( item.GetItem() == SECROMICON::PART_5 )
		{
			secromicon_part_5++;
		}
		else if ( item.GetItem() == SECROMICON::PART_6 )
		{
			secromicon_part_6++;
		}
	}

	if ( secromicon_part_1 < 1 || secromicon_part_2 < 1 || secromicon_part_3 < 1 ||
		 secromicon_part_4 < 1 || secromicon_part_5 < 1 || secromicon_part_6 < 1 )
	{
		this->ChaosMixLogLackingMixItems();
		return;
	}

	if ( secromicon_part_1 > 1 || secromicon_part_2 > 1 || secromicon_part_3 > 1 ||
		 secromicon_part_4 > 1 || secromicon_part_5 > 1 || secromicon_part_6 > 1 )
	{
		this->ChaosMixLogTooManyMixItems();
		return;
	}

	this->mix_price = 1000000;

	if ( !this->tributeTaxPrice() )
	{
		return;
	}

	this->success_rate.set(100);
	this->success_rate.limit_min(0);
	this->success_rate.limit_max(100);

	this->ChaosMixDebugMessage();

	if ( this->roll_success() )
	{
		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, Item(ITEMGET(14,109)), 1);
		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
	}

	this->ChaosMixDBSave();
}
	
void MixHandler::ChaosMixSealedBox(uint8 data)
{
	uint32 golden_key = 0;
	uint32 golden_box = 0;
	uint32 silver_key = 0;
	uint32 silver_box = 0;

	chaos_box_loop(i)
	{
		Item const& item = this->GetPlayer()->GetMixInventory()->item[i];

		if ( !item.IsItem() )
			continue;

		if ( item.GetItem() == ITEMGET(14, 113) )
		{
			golden_key++;
		}
		else if ( item.GetItem() == ITEMGET(14, 121) )
		{
			golden_box++;
		}
		else if ( item.GetItem() == ITEMGET(14, 112) )
		{
			silver_key++;
		}
		else if ( item.GetItem() == ITEMGET(14, 122) )
		{
			silver_box++;
		}
	}

	if ( golden_key == 0 && golden_box == 0 && silver_key == 0 && silver_box == 0 )
	{
		this->ChaosMixLogLackingMixItems();
		return;
	}

	uint8 mixtp = 0;
	int32 count = 0;

	if ( silver_key > 0 && silver_box > 0 )
	{
		if ( golden_key || golden_box )
		{
			this->ChaosMixLogInvalidMixItems();
			return;
		}

		if ( silver_key != silver_box )
		{
			this->ChaosMixLogTooManyMixItems();
			return;
		}

		count = silver_key;
		mixtp = 0;
	}
	else if ( golden_key && golden_box )
	{
		if ( silver_key || silver_box )
		{
			this->ChaosMixLogInvalidMixItems();
			return;
		}

		if ( golden_key != golden_box )
		{
			this->ChaosMixLogTooManyMixItems();
			return;
		}

		count = golden_key;
		mixtp = 1;
	}
	else
	{
		this->ChaosMixLogInvalidMixItems();
		return;
	}

	if ( count != this->GetCount() || count > this->GetMaxCount() )
	{
		this->ChaosMixLogInvalidMixItems();
		return;
	}

	this->mix_price = 0;

	if ( !this->tributeTaxPrice() )
	{
		return;
	}

	this->GetPlayer()->GetMixInventory()->Clear();

	this->GenerateMassCombinationResult(count, 100, 100);

	for ( int32 i = 0; i < count; ++i )
	{
		this->SetSuccess(this->GetMassCombinationList(i) == 1 ? true: false);

		this->ChaosMixDebugMessage();

		if ( this->IsSuccess() )
		{
			sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, Item((mixtp == 0 ? ITEMGET(14, 124): ITEMGET(14, 123))));
			this->ChaosMixLogSuccess();
		}
		else
		{
			if ( count == 1 )
			{
				this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
			}

			this->ChaosMixLogFail();
		}

		this->ChaosMixDBSave();
	}

	this->GetPlayer()->GetInterfaceState()->SetState(0);
}

void MixHandler::ChaosMixUpgrade14(uint8 data)
{
	this->ChaosMixUpgradeLevel(data, 4);
}
	
void MixHandler::ChaosMixUpgrade15(uint8 data)
{
	this->ChaosMixUpgradeLevel(data, 5);
}

void MixHandler::ChaosMixLuckyTicket(uint8 data)
{
	uint16 ticket = 0;
	uint8 ticket_level = 0;
	uint32 ticket_count = 0;

	chaos_box_loop(i)
	{
		Item const& item = this->GetPlayer()->GetMixInventory()->item[i];

		if ( !item.IsItem() )
			continue;

		if ( sItemMgr->IsItemMixLucky(item.GetItem(), item.GetLevel()) )
		{
			ticket = item.GetItem();
			ticket_level = item.GetLevel();
			ticket_count++;
		}
	}

	if ( ticket_count < 1 )
	{
		this->ChaosMixLogLackingMixItems();
		return;
	}

	if ( ticket_count > 1 )
	{
		this->ChaosMixLogTooManyMixItems();
		return;
	}

	this->mix_price = 0;

	if ( !this->tributeTaxPrice() )
	{
		return;
	}

	this->success_rate.set(100);
	this->success_rate.limit_min(0);
	this->success_rate.limit_max(100);

	item_mix_lucky_data const* item_lucky = sItemMgr->GetRandomItemMixLucky(ticket, ticket_level, this->GetPlayer());

	if ( !item_lucky )
	{
		this->ChaosMixLogInvalidMixItems();
		return;
	}

	this->ChaosMixDebugMessage();

	if ( this->roll_success() )
	{
		uint8 ancient = sItemMgr->GetRandomOptionAncient(item_lucky->GetItem());

		if ( !IS_ANCIENT(ancient) )
		{
			ancient |= 1;
		}
		
		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, Item(item_lucky->GetItem(), Random(MAX_ITEM_LEVEL + 1), 0.0f, 0, 0, 0, 0, ancient), 1);
		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
	}

	this->ChaosMixDBSave();
}
	
void MixHandler::ChaosMixExtensionJewel(uint8 data)
{
	uint32 item_count = 0;
	float durability = 0.0f;

	chaos_box_loop(i)
	{
		Item const& item = this->GetPlayer()->GetMixInventory()->item[i];

		if ( !item.IsItem() )
			continue;

		if ( sItemMgr->IsLuckyItem(item.GetItem()) )
		{
			durability = item.GetDurability();
			item_count++;
		}
	}

	if ( item_count < 1 )
	{
		this->ChaosMixLogLackingMixItems();
		return;
	}

	if ( item_count > 1 )
	{
		this->ChaosMixLogTooManyMixItems();
		return;
	}

	this->mix_price = 0;

	if ( !this->tributeTaxPrice() )
	{
		return;
	}

	this->success_rate.set(durability == 255.0f ? 60: 10);
	this->success_rate.limit_min(0);
	this->success_rate.limit_max(100);

	this->ChaosMixDebugMessage();

	if ( this->roll_success() )
	{
		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, Item(JEWEL::EXTENSION), 1);
		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
	}

	this->ChaosMixDBSave();
}

void MixHandler::ChaosMixArkaWarSign(uint8 data)
{
	int32 count = 0;

	chaos_box_loop(i)
	{
		Item const& item = this->GetPlayer()->GetMixInventory()->item[i];

		if ( !item.IsItem() )
		{
			continue;
		}

		if ( item.GetItem() == ITEMGET(14, 21) && item.GetLevel() == 3 )
		{
			count += item.GetDurability();
		}
	}

	this->success_rate.set(100);

	this->ChaosMixDebugMessage();

	if ( this->roll_success() )
	{
		this->GetPlayer()->GetMixInventory()->Clear();
		this->GetPlayer()->GetInterfaceState()->SetState(0);
		//this->GetPlayer()->ChaosMixSend(0);
		this->GetPlayer()->ChaosMixResult(193);
						
		sServerLink->ArkaWarSignRegister(this->GetPlayer(), 1, count);

		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
	}
}

void MixHandler::ChaosMixArkaWarBooty(uint8 data)
{
	int32 trophy_count = 0;

	chaos_box_loop(i)
	{
		Item const& item = this->GetPlayer()->GetMixInventory()->item[i];

		if ( !item.IsItem() )
		{
			continue;
		}

		if ( item.GetItem() == ARKA_WAR_KILL_TROPHY )
		{
			trophy_count += item.GetDurability();
		}
	}

	ArkaWarExchangeData const* pArkaWarExchangeData = sArkaWar->GetExchangeData(trophy_count);

	if ( !pArkaWarExchangeData )
	{
		this->ChaosMixLogLackingMixItems();
		return;
	}

	int32 add_count = trophy_count - pArkaWarExchangeData->GetTrophyMin();

	this->success_rate.set(pArkaWarExchangeData->GetSuccessRate());
	this->success_rate.op_add(add_count * pArkaWarExchangeData->GetAddSuccessRate());
	this->success_rate.limit_max(100);

	this->ChaosMixDebugMessage();

	if ( this->roll_success() )
	{
		if ( sItemBagMgr->ExecuteItemBag(ITEM_BAG_MIX, this->GetPlayer(), pArkaWarExchangeData->GetItemBag()) == ITEM_BAG_RESULT_SUCCESS )
		{
			this->ChaosMixLogSuccess();
		}
		else
		{
			this->GetPlayer()->GetInterfaceState()->SetState(0);
			this->GetPlayer()->ChaosMixResult(CHAOS_MIX_RESULT_FAIL);
			this->ChaosMixLogFail();
		}
	}
	else
	{
		this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
	}
}

void MixHandler::ChaosMixSummonScroll(uint8 data)
{
	int32 SummonScroll1Count = 0;
	int32 SummonScroll2Count = 0;
	int32 SummonScroll3Count = 0;
	data = 0;

	chaos_box_loop(i)
	{
		Item const& item = this->GetPlayer()->GetMixInventory()->item[i];

		if ( !item.IsItem() )
			continue;

		if( item.GetItem() == ITEMGET(14, 210) )
		{
			if ( data == 0 ) 
			{
				data = 1;
			}

			SummonScroll1Count++;
		}
		else if( item.GetItem() == ITEMGET(14, 211) )
		{
			data = 2;
			SummonScroll2Count++;
		}
		else if( item.GetItem() == ITEMGET(14, 212) )
		{
			data = 3;
			SummonScroll3Count++;
		}
	}

	if( data == 0 || (data == 1 && SummonScroll1Count != 2) || (data == 2 && (SummonScroll1Count != 1 || SummonScroll2Count != 1)) || (data == 3 && (SummonScroll1Count != 1 || SummonScroll3Count != 1)))
	{
		this->ChaosMixLogInvalidMixItems();
		return;
	}

	switch( data )
	{
	case 1:
		{
			this->success_rate.set(50);
			this->mix_price = 100000;
		} break;

	case 2:
		{
			this->success_rate.set(50);
			this->mix_price = 200000;
		} break;

	case 3:
		{
			this->success_rate.set(40);
			this->mix_price = 300000;
		} break;
	}

	if ( !this->tributeTaxPrice() )
	{
		return;
	}

	this->ChaosMixDebugMessage();
	
	if ( this->roll_success() )
	{
		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, Item(ITEMGET(14, 210) + data, 0, 1), 1);
		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
	}

	this->ChaosMixDBSave();
}
	
void MixHandler::ChaosMixMonsterWing(uint8 data)
{
	int32 ChaosCount = 0;
	int32 CreationCount = 0;
	int32 MaterialType = 0;
	int32 WingItemCount = 0;
	int64 WingItemMoney = 0;
	int32 WingItemType = 0;

	chaos_box_loop(i)
	{
		Item const* item = this->GetPlayer()->GetMixInventory()->GetItem(i);

		if ( !item->IsItem() )
			continue;

		if (item->GetItem() == JEWEL::CHAOS)
		{
			ChaosCount += item->GetDurability();
		}
		else if( item->GetItem() == JEWEL::CREATION)
		{
			CreationCount += item->GetDurability();
		}
		else if( item->GetItem() == ITEMGET(14, 176) )
		{
			MaterialType = 1;
		}
		else if( item->GetItem() == ITEMGET(14, 177) )
		{
			MaterialType = 2;
		}
		else if( item->GetItem() == ITEMGET(14, 178) )
		{
			MaterialType = 3;
		}
		else if( item->GetItem() == ITEMGET(14, 179) )
		{
			MaterialType = 4;
		}
		else if(( item->GetItem() >= ITEMGET(12, 3) && item->GetItem() <= ITEMGET(12, 6)) || item->GetItem() == ITEMGET(12, 42) )
		{
			WingItemCount++;
			WingItemMoney += item->GetBuyPrice();
			WingItemType = 0;
		}
		else if( item->GetItem() == ITEMGET(12, 49) || item->GetItem() == ITEMGET(13, 30) || item->GetItem() == ITEMGET(12, 269) )
		{
			WingItemCount++;
			WingItemMoney += item->GetBuyPrice();
			WingItemType = 1;
		}
	}

	if ( !this->ChaosMixTalismanOfLuck() )
	{
		this->ChaosMixLogTooManyTalismanOfLuck();
		return;
	}

	if( ChaosCount != 1 || CreationCount != 1 || MaterialType == 0 || WingItemCount != 1 )
	{
		this->ChaosMixLogInvalidMixItems();
		return;
	}

	this->success_rate.set(WingItemMoney/((WingItemType==0)?9000000:500000));
	this->success_rate.limit_max(60);

	this->success_rate.op_add(this->GetLuckyCharm());
	
	this->mix_price = this->success_rate.get() * 100000;

	if ( !this->tributeTaxPrice() )
	{
		return;
	}

	this->ChaosMixDebugMessage();

	if ( this->roll_success() )
	{
		uint16 ItemIndex = ITEMGET(12, 262);
		uint8 luck = roll_chance_i(20) ? 1 : 0;
		uint16 option_rate = Random(10000);
		uint8 option = 0;
		uint8 excellent = 0;

		if ( option_rate < 1200 )
		{
			option = 1;
		}

		if ( option_rate < 600 )
		{
			option = 2;
		}

		if ( option_rate < 300 )
		{
			option = 3;
		}

		if (roll_chance_i(12))
		{
			excellent = GetRandomExcOption(2);
		}

		switch( MaterialType )
		{
		case 1:
			{
				ItemIndex = ITEMGET(12, 262);
			} break;

		case 2:
			{
				ItemIndex = ITEMGET(12, 263);
			} break;

		case 3:
			{
				ItemIndex = ITEMGET(12, 264);
			} break;

		case 4:
			{
				ItemIndex = ITEMGET(12, 265);
			} break;
		}

		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, Item(ItemIndex, 0, 0, 0, luck, option, excellent + (16 * Random(2))), 1);

		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
	}

	this->ChaosMixDBSave();
}
	
void MixHandler::ChaosMixConquerorWings(uint8 data)
{
	int32 ConquerorsBadge = 0;
	int32 SoulPack = 0;
	int32 BlessPack = 0;
	int32 LifePack = 0;
	int32 ChaosJewel = 0;

	chaos_box_loop(i)
	{
		Item const* item = this->GetPlayer()->GetMixInventory()->GetItem(i);

		if ( !item->IsItem() )
		{
			continue;
		}

		if( item->GetItem() == ITEMGET(13, 276))
		{
			ConquerorsBadge++;
			this->SetDBFlag(item->GetDBFlag());
		}
		else if( item->GetItem() == JEWEL::SOUL)
		{
			SoulPack += item->GetDurability();
		}
		else if( item->GetItem() == JEWEL::BLESS)
		{
			BlessPack += item->GetDurability();
		}
		else if( item->GetItem() == JEWEL::LIFE)
		{
			LifePack += item->GetDurability();
		}
		else if (item->GetItem() == JEWEL::CHAOS)
		{
			ChaosJewel += item->GetDurability();
		}
	}

	if( ConquerorsBadge == 0 || SoulPack < 10 || BlessPack < 10 || LifePack < 10 || ChaosJewel == 0 )
	{
		this->ChaosMixLogInvalidMixItems();
		return;
	}
	
	this->success_rate.set(100);
	this->mix_price = 10000000;
	
	if ( !this->tributeTaxPrice() )
	{
		return;
	}

	this->ChaosMixDebugMessage();

	if ( this->roll_success() )
	{
		uint8 opt_rate = Random<uint8>(100);
		uint8 opt_type = Random<uint8>(3);
		
		uint8 exe = 1 << Random(3);
		uint8 luck = roll_chance_i(50) ? 1 : 0;
		uint8 option = (opt_type == 0 && opt_rate < 30)?3:
					   (opt_type == 1 && opt_rate < 45)?2:
					   (opt_type == 2 && opt_rate < 60)?1:
					   0;

		if ( roll_chance_i(50) )
		{
			exe |= 1 << Random(3);
		}

		Item item_create(ITEMGET(12, 268), 0, 0, 0, luck, option, exe);
		item_create.SetDBFlag(this->GetDBFlag());

		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, item_create, 1);

		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
	}

	this->ChaosMixDBSave();
}

void MixHandler::ChaosMixDarkTransformationRing(uint8 data)
{
	int32 HardshipBadge = 0;
	int32 SoulPack = 0;
	int32 BlessPack = 0;
	int32 LifePack = 0;
	int32 ChaosJewel = 0;

	chaos_box_loop(i)
	{
		Item const* item = this->GetPlayer()->GetMixInventory()->GetItem(i);

		if ( !item->IsItem() )
		{
			continue;
		}

		if( item->GetItem() == ITEMGET(13, 277))
		{
			HardshipBadge++;
			this->SetDBFlag(item->GetDBFlag());
		}
		else if( item->GetItem() == JEWEL::SOUL)
		{
			SoulPack += item->GetDurability();
		}
		else if( item->GetItem() == JEWEL::BLESS)
		{
			BlessPack += item->GetDurability();
		}
		else if( item->GetItem() == JEWEL::LIFE)
		{
			LifePack += item->GetDurability();
		}
		else if (item->GetItem() == JEWEL::CHAOS)
		{
			ChaosJewel += item->GetDurability();
		}
	}

	if( HardshipBadge == 0 || SoulPack < 10 || BlessPack < 10 || LifePack < 10 || ChaosJewel == 0 )
	{
		this->ChaosMixLogInvalidMixItems();
		return;
	}
	
	this->success_rate.set(100);
	this->mix_price = 10000000;
	
	if ( !this->tributeTaxPrice() )
	{
		return;
	}

	this->ChaosMixDebugMessage();

	if ( this->roll_success() )
	{
		Item item_create(ITEMGET(13, 268));
		item_create.SetDBFlag(this->GetDBFlag());

		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, item_create, 1);

		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
	}

	this->ChaosMixDBSave();
}

void MixHandler::ChaosMixAncientHeroSoul(uint8 data)
{
	int32 AncientItem = 0;

	chaos_box_loop(i)
	{
		Item const* item = this->GetPlayer()->GetMixInventory()->GetItem(i);

		if (!item->IsItem())
		{
			continue;
		}

		item_set_type const* pItemSetType = sItemMgr->GetItemSetType(item->GetItem());

		if (!pItemSetType)
		{
			continue;
		}

		if (!item->GetAncientType())
		{
			continue;
		}

		item_set_option const* pItemSetOption = sItemMgr->GetItemSetOption(pItemSetType->GetAncientID(item->GetAncientType() - 1));

		if (!pItemSetOption)
		{
			continue;
		}

		if (!pItemSetOption->IsAncientHeroSoul())
		{
			continue;
		}

		if (item->IsAncient() && item->GetLevel() >= 11)
		{
			AncientItem++;
		}
	}

	if (AncientItem == 0)
	{
		this->ChaosMixLogInvalidMixItems();
		return;
	}

	this->success_rate.set(80);

	this->mix_price = 0;

	if (!this->tributeTaxPrice())
	{
		return;
	}

	this->ChaosMixDebugMessage();

	if (this->roll_success())
	{
		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, Item(ITEMGET(13, 407)), 1);

		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
	}

	this->ChaosMixDBSave();
}

void MixHandler::ChaosMixSealedMasteryFinal(uint8 data)
{
	int32 AncientHeroSoul = 0;
	int32 SealedMasteryID = 0;

	chaos_box_loop(i)
	{
		Item const* item = this->GetPlayer()->GetMixInventory()->GetItem(i);

		if (!item->IsItem())
		{
			continue;
		}

		this->AddDBFlag(item->GetDBFlag());

		if (item->GetItem() == ITEMGET(13, 407))
		{
			AncientHeroSoul++;
		}
		else if (item->GetItem() == ITEMGET(13, 352))
		{
			SealedMasteryID = ITEMGET(7, 98);
		}
		else if (item->GetItem() == ITEMGET(13, 360))
		{
			SealedMasteryID = ITEMGET(7, 99);
		}
		else if (item->GetItem() == ITEMGET(13, 364))
		{
			SealedMasteryID = ITEMGET(7, 100);
		}
		else if (item->GetItem() == ITEMGET(13, 380))
		{
			SealedMasteryID = ITEMGET(7, 103);
		}
		else if (item->GetItem() == ITEMGET(13, 388))
		{
			SealedMasteryID = ITEMGET(7, 101);
		}
		else if (item->GetItem() == ITEMGET(13, 392))
		{
			SealedMasteryID = ITEMGET(7, 104);
		}
		else if (item->GetItem() == ITEMGET(13, 400))
		{
			SealedMasteryID = ITEMGET(7, 105);
		}
		else if (item->GetItem() == ITEMGET(13, 351)) // Fury
		{
			SealedMasteryID = ITEMGET(8, 98);
		}
		else if (item->GetItem() == ITEMGET(13, 359)) // Transcendence
		{
			SealedMasteryID = ITEMGET(8, 99);
		}
		else if (item->GetItem() == ITEMGET(13, 363)) // Flurry
		{
			SealedMasteryID = ITEMGET(8, 100);
		}
		else if (item->GetItem() == ITEMGET(13, 371)) // Extremity
		{
			SealedMasteryID = ITEMGET(8, 102);
		}
		else if (item->GetItem() == ITEMGET(13, 379)) // Conquest
		{
			SealedMasteryID = ITEMGET(8, 103);
		}
		else if (item->GetItem() == ITEMGET(13, 387)) // Honor
		{
			SealedMasteryID = ITEMGET(8, 101);
		}
		else if (item->GetItem() == ITEMGET(13, 391)) // Destruction
		{
			SealedMasteryID = ITEMGET(8, 104);
		}
		else if (item->GetItem() == ITEMGET(13, 399)) // Tenacity
		{
			SealedMasteryID = ITEMGET(8, 105);
		}
		else if (item->GetItem() == ITEMGET(13, 353))
		{
			SealedMasteryID = ITEMGET(9, 98);
		}
		else if (item->GetItem() == ITEMGET(13, 361))
		{
			SealedMasteryID = ITEMGET(9, 99);
		}
		else if (item->GetItem() == ITEMGET(13, 365))
		{
			SealedMasteryID = ITEMGET(9, 100);
		}
		else if (item->GetItem() == ITEMGET(13, 373))
		{
			SealedMasteryID = ITEMGET(9, 102);
		}
		else if (item->GetItem() == ITEMGET(13, 381))
		{
			SealedMasteryID = ITEMGET(9, 103);
		}
		else if (item->GetItem() == ITEMGET(13, 389))
		{
			SealedMasteryID = ITEMGET(9, 101);
		}
		else if (item->GetItem() == ITEMGET(13, 393))
		{
			SealedMasteryID = ITEMGET(9, 104);
		}
		else if (item->GetItem() == ITEMGET(13, 401))
		{
			SealedMasteryID = ITEMGET(9, 105);
		}
		else if (item->GetItem() == ITEMGET(13, 362))
		{
			SealedMasteryID = ITEMGET(10, 99);
		}
		else if (item->GetItem() == ITEMGET(13, 366))
		{
			SealedMasteryID = ITEMGET(10, 100);
		}
		else if (item->GetItem() == ITEMGET(13, 374))
		{
			SealedMasteryID = ITEMGET(10, 102);
		}
		else if (item->GetItem() == ITEMGET(13, 382))
		{
			SealedMasteryID = ITEMGET(10, 103);
		}
		else if (item->GetItem() == ITEMGET(13, 390))
		{
			SealedMasteryID = ITEMGET(10, 101);
		}
		else if (item->GetItem() == ITEMGET(13, 402))
		{
			SealedMasteryID = ITEMGET(10, 105);
		}
		else if (item->GetItem() == ITEMGET(13, 354))
		{
			SealedMasteryID = ITEMGET(11, 98);
		}
		else if (item->GetItem() == ITEMGET(13, 372))
		{
			SealedMasteryID = ITEMGET(11, 102);
		}
		else if (item->GetItem() == ITEMGET(13, 394))
		{
			SealedMasteryID = ITEMGET(11, 104);
		}
		else if (item->GetItem() == ITEMGET(13, 367))
		{
			SealedMasteryID = ITEMGET(8, 158);
		}
		else if (item->GetItem() == ITEMGET(13, 368))
		{
			SealedMasteryID = ITEMGET(7, 158);
		}
		else if (item->GetItem() == ITEMGET(13, 369))
		{
			SealedMasteryID = ITEMGET(9, 158);
		}
		else if (item->GetItem() == ITEMGET(13, 370))
		{
			SealedMasteryID = ITEMGET(10, 158);
		}
		else if (item->GetItem() == ITEMGET(13, 375))
		{
			SealedMasteryID = ITEMGET(8, 161);
		}
		else if (item->GetItem() == ITEMGET(13, 376))
		{
			SealedMasteryID = ITEMGET(11, 161);
		}
		else if (item->GetItem() == ITEMGET(13, 377))
		{
			SealedMasteryID = ITEMGET(9, 161);
		}
		else if (item->GetItem() == ITEMGET(13, 378))
		{
			SealedMasteryID = ITEMGET(10, 161);
		}
		else if (item->GetItem() == ITEMGET(13, 355))
		{
			SealedMasteryID = ITEMGET(8, 122);
		}
		else if (item->GetItem() == ITEMGET(13, 356))
		{
			SealedMasteryID = ITEMGET(7, 122);
		}
		else if (item->GetItem() == ITEMGET(13, 357))
		{
			SealedMasteryID = ITEMGET(9, 122);
		}
		else if (item->GetItem() == ITEMGET(13, 358))
		{
			SealedMasteryID = ITEMGET(10, 122);
		}
		else if (item->GetItem() == ITEMGET(13, 383))
		{
			SealedMasteryID = ITEMGET(8, 180);
		}
		else if (item->GetItem() == ITEMGET(13, 384))
		{
			SealedMasteryID = ITEMGET(7, 180);
		}
		else if (item->GetItem() == ITEMGET(13, 385))
		{
			SealedMasteryID = ITEMGET(9, 180);
		}
		else if (item->GetItem() == ITEMGET(13, 386))
		{
			SealedMasteryID = ITEMGET(10, 180);
		}
		else if (item->GetItem() == ITEMGET(13, 395))
		{
			SealedMasteryID = ITEMGET(7, 200);
		}
		else if (item->GetItem() == ITEMGET(13, 396))
		{
			SealedMasteryID = ITEMGET(8, 200);
		}
		else if (item->GetItem() == ITEMGET(13, 397))
		{
			SealedMasteryID = ITEMGET(9, 200);
		}
		else if (item->GetItem() == ITEMGET(13, 398))
		{
			SealedMasteryID = ITEMGET(11, 200);
		}
		else if (item->GetItem() == ITEMGET(13, 403))
		{
			SealedMasteryID = ITEMGET(7, 212);
		}
		else if (item->GetItem() == ITEMGET(13, 404))
		{
			SealedMasteryID = ITEMGET(8, 212);
		}
		else if (item->GetItem() == ITEMGET(13, 405))
		{
			SealedMasteryID = ITEMGET(9, 212);
		}
		else if (item->GetItem() == ITEMGET(13, 406))
		{
			SealedMasteryID = ITEMGET(11, 212);
		}
	}

	if (AncientHeroSoul == 0 || SealedMasteryID == 0)
	{
		this->ChaosMixLogInvalidMixItems();
		return;
	}

	uint8 ancient = sItemMgr->GetRandomOptionAncient(SealedMasteryID);
	uint8 excellent = 0;

	if (!ancient)
	{
		this->ChaosMixLogInvalidMixItems();
		return;
	}

	this->success_rate.set(100);
	this->mix_price = 0;

	if (!this->tributeTaxPrice())
	{
		return;
	}

	this->ChaosMixDebugMessage();

	if (this->roll_success())
	{
		Item item_create(SealedMasteryID, 0, 0, 0, 0, 0, excellent, ancient);
		item_create.SetDBFlag(this->GetDBFlag());

		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, item_create, 1);

		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
	}

	this->ChaosMixDBSave();
}

void MixHandler::ChaosMixBloodangelToDarkangelSet(uint8 data)
{
	int32 SealedStoneShard = 0;
	int32 BloodAngelItem = 0;
	int32 ChaosJewel = 0;
	int32 CreationJewel = 0;
	int32 BundleBless = 0;
	int32 BundleSoul = 0;
	uint8 Luck = 0;
	uint8 Ancient = 0;
	uint8 Excellent = 0;
	INIT_SOCKET_DATA(Socket);
	uint8 SocketBonus = 0xFF;

	chaos_box_loop(i)
	{
		Item const* item = this->GetPlayer()->GetMixInventory()->GetItem(i);

		if (!item->IsItem())
		{
			continue;
		}

		if (item->GetItem() == ITEMGET(14, 341))
		{
			SealedStoneShard++;
			this->AddDBFlag(item->GetDBFlag());
		}
		else if (item->GetItem() == JEWEL::CHAOS)
		{
			ChaosJewel += item->GetDurability();
		}
		else if (item->GetItem() == JEWEL::CREATION)
		{
			CreationJewel += item->GetDurability();
		}
		else if (item->GetItem() == JEWEL::BLESS)
		{
			BundleBless += item->GetDurability();
		}
		else if (item->GetItem() == JEWEL::SOUL)
		{
			BundleSoul += item->GetDurability();
		}
		else if (item->IsAncient() && item->GetLevel() >= 9 && item->GetOption() >= 4)
		{
			if (item->GetItemType() == 7 ||
				item->GetItemType() == 8 ||
				item->GetItemType() == 9 ||
				item->GetItemType() == 10 ||
				item->GetItemType() == 11)
			{
				if (item->GetItemIndex() == 98)
				{
					BloodAngelItem = ITEMGET(item->GetItemType(), 138);
				}
				else if (item->GetItemIndex() == 99)
				{
					BloodAngelItem = ITEMGET(item->GetItemType(), 139);
				}
				else if (item->GetItemIndex() == 100)
				{
					BloodAngelItem = ITEMGET(item->GetItemType(), 140);
				}
				else if (item->GetItemIndex() == 101)
				{
					BloodAngelItem = ITEMGET(item->GetItemType(), 141);
				}
				else if (item->GetItemIndex() == 102)
				{
					BloodAngelItem = ITEMGET(item->GetItemType(), 142);
				}
				else if (item->GetItemIndex() == 103)
				{
					BloodAngelItem = ITEMGET(item->GetItemType(), 143);
				}
				else if (item->GetItemIndex() == 104)
				{
					BloodAngelItem = ITEMGET(item->GetItemType(), 144);
				}
				else if (item->GetItemIndex() == 105)
				{
					BloodAngelItem = ITEMGET(item->GetItemType(), 145);
				}
				else if (item->GetItemIndex() == 158)
				{
					BloodAngelItem = ITEMGET(item->GetItemType(), 159);
				}
				else if (item->GetItemIndex() == 161)
				{
					BloodAngelItem = ITEMGET(item->GetItemType(), 162);
				}
				else if (item->GetItemIndex() == 122)
				{
					BloodAngelItem = ITEMGET(item->GetItemType(), 123);
				}
				else if (item->GetItemIndex() == 122)
				{
					BloodAngelItem = ITEMGET(item->GetItemType(), 123);
				}
				else if (item->GetItemIndex() == 180)
				{
					BloodAngelItem = ITEMGET(item->GetItemType(), 181);
				}
				else if (item->GetItemIndex() == 200)
				{
					BloodAngelItem = ITEMGET(item->GetItemType(), 201);
				}
				else if (item->GetItemIndex() == 212)
				{
					BloodAngelItem = ITEMGET(item->GetItemType(), 213);
				}
			}

			Luck = item->GetLuck();
			Ancient = item->GetAncient();
			Excellent = item->GetExe();

			SOCKET_SLOT_LOOP(j)
			{
				Socket[j] = item->GetSocket(j);
			}

			SocketBonus = item->GetSocketBonus();

			this->AddDBFlag(item->GetDBFlag());
		}
	}

	if (SealedStoneShard == 0 || BloodAngelItem == 0 || ChaosJewel == 0 || CreationJewel == 0 || BundleBless < 10 || BundleSoul < 10)
	{
		this->ChaosMixLogInvalidMixItems();
		return;
	}

	this->success_rate.set(100);
	this->mix_price = 0;

	if (!this->tributeTaxPrice())
	{
		return;
	}

	this->ChaosMixDebugMessage();

	if (this->roll_success())
	{
		Item item_create(BloodAngelItem, 0, 0, 0, Luck, 0, Excellent, Ancient, Socket, SocketBonus);
		item_create.SetDBFlag(this->GetDBFlag());

		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, item_create, 1);

		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
	}

	this->ChaosMixDBSave();
}

void MixHandler::ChaosMixUpgradeDarkangelToHolyangelSet(uint8 data)
{
	int32 SpiritOfHolyAngel = 0;
	int32 DarkAngelItem = 0;
	int32 ChaosJewel = 0;
	int32 CreationJewel = 0;
	int32 BundleBless = 0;
	int32 BundleSoul = 0;
	uint8 Luck = 0;
	uint8 Ancient = 0;
	uint8 Excellent = 0;
	INIT_SOCKET_DATA(Socket);
	uint8 SocketBonus = 0xFF;

	chaos_box_loop(i)
	{
		Item const* item = this->GetPlayer()->GetMixInventory()->GetItem(i);

		if (!item->IsItem())
		{
			continue;
		}

		if (item->GetItem() == ITEMGET(14, 389))
		{
			SpiritOfHolyAngel++;
			this->AddDBFlag(item->GetDBFlag());
		}
		else if (item->GetItem() == JEWEL::CHAOS)
		{
			ChaosJewel += item->GetDurability();
		}
		else if (item->GetItem() == JEWEL::CREATION)
		{
			CreationJewel += item->GetDurability();
		}
		else if (item->GetItem() == JEWEL::BLESS)
		{
			BundleBless += item->GetDurability();
		}
		else if (item->GetItem() == JEWEL::SOUL)
		{
			BundleSoul += item->GetDurability();
		}
		else if (item->IsAncient() && item->GetLevel() >= 11 && item->GetOption() >= 4)
		{
			if (item->GetItemType() == 7 ||
				item->GetItemType() == 8 ||
				item->GetItemType() == 9 ||
				item->GetItemType() == 10 ||
				item->GetItemType() == 11)
			{
				if (item->GetItemIndex() == 138)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 150);
				}
				else if (item->GetItemIndex() == 139)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 151);
				}
				else if (item->GetItemIndex() == 140)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 152);
				}
				else if (item->GetItemIndex() == 141)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 153);
				}
				else if (item->GetItemIndex() == 142)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 154);
				}
				else if (item->GetItemIndex() == 143)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 155);
				}
				else if (item->GetItemIndex() == 144)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 156);
				}
				else if (item->GetItemIndex() == 145)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 157);
				}
				else if (item->GetItemIndex() == 159)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 160);
				}
				else if (item->GetItemIndex() == 162)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 163);
				}
				else if (item->GetItemIndex() == 123)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 127);
				}
				else if (item->GetItemIndex() == 181)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 182);
				}
				else if (item->GetItemIndex() == 201)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 202);
				}
				else if (item->GetItemIndex() == 213)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 214);
				}
			}

			Luck = item->GetLuck();
			Ancient = item->GetAncient();
			Excellent = item->GetExe();

			SOCKET_SLOT_LOOP(j)
			{
				Socket[j] = item->GetSocket(j);
			}

			SocketBonus = item->GetSocketBonus();

			this->AddDBFlag(item->GetDBFlag());
		}
	}

	if (SpiritOfHolyAngel != 1 || DarkAngelItem == 0 || ChaosJewel != 1 || CreationJewel != 1 || BundleBless < 30 || BundleSoul < 30)
	{
		this->ChaosMixLogInvalidMixItems();
		return;
	}

	this->success_rate.set(100);
	this->mix_price = 0;

	if (!this->tributeTaxPrice())
	{
		return;
	}

	this->ChaosMixDebugMessage();

	if (this->roll_success())
	{
		Item item_create(DarkAngelItem, 0, 0, 0, Luck, 0, Excellent, Ancient, Socket, SocketBonus);
		item_create.SetDBFlag(this->GetDBFlag());

		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, item_create, 1);

		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
	}

	this->ChaosMixDBSave();
}

void MixHandler::ChaosMixUpgradeHolyangelToAwakeningSet(uint8 data)
{
	int32 AwakeningSpirit = 0;
	int32 DarkAngelItem = 0;
	int32 ChaosJewel = 0;
	int32 CreationJewel = 0;
	int32 BundleBless = 0;
	int32 BundleSoul = 0;
	uint8 Luck = 0;
	uint8 Ancient = 0;
	uint8 Excellent = 0;
	INIT_SOCKET_DATA(Socket);
	uint8 SocketBonus = 0xFF;

	chaos_box_loop(i)
	{
		Item const* item = this->GetPlayer()->GetMixInventory()->GetItem(i);

		if (!item->IsItem())
		{
			continue;
		}

		if (item->GetItem() == ITEMGET(14, 415))
		{
			AwakeningSpirit++;
			this->AddDBFlag(item->GetDBFlag());
		}
		else if (item->GetItem() == JEWEL::CHAOS)
		{
			ChaosJewel += item->GetDurability();
		}
		else if (item->GetItem() == JEWEL::CREATION)
		{
			CreationJewel += item->GetDurability();
		}
		else if (item->GetItem() == JEWEL::BLESS)
		{
			BundleBless += item->GetDurability();
		}
		else if (item->GetItem() == JEWEL::SOUL)
		{
			BundleSoul += item->GetDurability();
		}
		else if (item->IsAncient() && item->GetLevel() >= 11 && item->GetOption() >= 4)
		{
			if (item->GetItemType() == 7 ||
				item->GetItemType() == 8 ||
				item->GetItemType() == 9 ||
				item->GetItemType() == 10 ||
				item->GetItemType() == 11)
			{
				if (item->GetItemIndex() == 150)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 106);
				}
				else if (item->GetItemIndex() == 151)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 107);
				}
				else if (item->GetItemIndex() == 152)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 108);
				}
				else if (item->GetItemIndex() == 153)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 109);
				}
				else if (item->GetItemIndex() == 154)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 110);
				}
				else if (item->GetItemIndex() == 155)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 111);
				}
				else if (item->GetItemIndex() == 156)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 112);
				}
				else if (item->GetItemIndex() == 157)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 113);
				}
				else if (item->GetItemIndex() == 160)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 114);
				}
				else if (item->GetItemIndex() == 163)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 115);
				}
				else if (item->GetItemIndex() == 127)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 128);
				}
				else if (item->GetItemIndex() == 182)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 183);
				}
				else if (item->GetItemIndex() == 202)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 203);
				}
				else if (item->GetItemIndex() == 214)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 215);
				}
			}

			Luck = item->GetLuck();
			Ancient = item->GetAncient();
			Excellent = item->GetExe();

			SOCKET_SLOT_LOOP(j)
			{
				Socket[j] = item->GetSocket(j);
			}

			SocketBonus = item->GetSocketBonus();

			this->AddDBFlag(item->GetDBFlag());
		}
	}

	if (AwakeningSpirit != 1 || DarkAngelItem == 0 || ChaosJewel != 1 || CreationJewel != 1 || BundleBless < 30 || BundleSoul < 30)
	{
		this->ChaosMixLogInvalidMixItems();
		return;
	}

	this->success_rate.set(100);
	this->mix_price = 0;

	if (!this->tributeTaxPrice())
	{
		return;
	}

	this->ChaosMixDebugMessage();

	if (this->roll_success())
	{
		Item item_create(DarkAngelItem, 0, 0, 0, Luck, 0, Excellent, Ancient, Socket, SocketBonus);
		item_create.SetDBFlag(this->GetDBFlag());

		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, item_create, 1);

		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
	}

	this->ChaosMixDBSave();
}

void MixHandler::ChaosMixUpgradeAwakeningToBlueEyeSet(uint8 data)
{
	int32 FrozenSpirit = 0;
	int32 DarkAngelItem = 0;
	int32 ChaosJewel = 0;
	int32 CreationJewel = 0;
	int32 BundleBless = 0;
	int32 BundleSoul = 0;
	uint8 Luck = 0;
	uint8 Ancient = 0;
	uint8 Excellent = 0;
	INIT_SOCKET_DATA(Socket);
	uint8 SocketBonus = 0xFF;

	chaos_box_loop(i)
	{
		Item const* item = this->GetPlayer()->GetMixInventory()->GetItem(i);

		if (!item->IsItem())
		{
			continue;
		}

		if (item->GetItem() == ITEMGET(14, 449))
		{
			FrozenSpirit++;
			this->AddDBFlag(item->GetDBFlag());
		}
		else if (item->GetItem() == JEWEL::CHAOS)
		{
			ChaosJewel += item->GetDurability();
		}
		else if (item->GetItem() == JEWEL::CREATION)
		{
			CreationJewel += item->GetDurability();
		}
		else if (item->GetItem() == JEWEL::BLESS)
		{
			BundleBless += item->GetDurability();
		}
		else if (item->GetItem() == JEWEL::SOUL)
		{
			BundleSoul += item->GetDurability();
		}
		else if (item->IsAncient() && item->GetLevel() >= 11 && item->GetOption() >= 4)
		{
			if (item->GetItemType() == 7 ||
				item->GetItemType() == 8 ||
				item->GetItemType() == 9 ||
				item->GetItemType() == 10 ||
				item->GetItemType() == 11)
			{
				if (item->GetItemIndex() == 106)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 164);
				}
				else if (item->GetItemIndex() == 107)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 165);
				}
				else if (item->GetItemIndex() == 108)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 166);
				}
				else if (item->GetItemIndex() == 109)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 167);
				}
				else if (item->GetItemIndex() == 110)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 168);
				}
				else if (item->GetItemIndex() == 111)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 169);
				}
				else if (item->GetItemIndex() == 112)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 170);
				}
				else if (item->GetItemIndex() == 113)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 171);
				}
				else if (item->GetItemIndex() == 114)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 172);
				}
				else if (item->GetItemIndex() == 115)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 173);
				}
				else if (item->GetItemIndex() == 128)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 129);
				}
				else if (item->GetItemIndex() == 183)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 184);
				}
				else if (item->GetItemIndex() == 203)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 204);
				}
				else if (item->GetItemIndex() == 215)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 216);
				}
			}

			Luck = item->GetLuck();
			Ancient = item->GetAncient();
			Excellent = item->GetExe();

			SOCKET_SLOT_LOOP(j)
			{
				Socket[j] = item->GetSocket(j);
			}

			SocketBonus = item->GetSocketBonus();

			this->AddDBFlag(item->GetDBFlag());
		}
	}

	if (FrozenSpirit != 1 || DarkAngelItem == 0 || ChaosJewel != 1 || CreationJewel != 1 || BundleBless < 30 || BundleSoul < 30)
	{
		this->ChaosMixLogInvalidMixItems();
		return;
	}

	this->success_rate.set(100);
	this->mix_price = 0;

	if (!this->tributeTaxPrice())
	{
		return;
	}

	this->ChaosMixDebugMessage();

	if (this->roll_success())
	{
		Item item_create(DarkAngelItem, 0, 0, 0, Luck, 0, Excellent, Ancient, Socket, SocketBonus);
		item_create.SetDBFlag(this->GetDBFlag());

		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, item_create, 1);

		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
	}

	this->ChaosMixDBSave();
}

void MixHandler::ChaosMixUpgradeBlueEyeToSilverHeartSet(uint8 data)
{
	int32 FrozenSpirit = 0;
	int32 DarkAngelItem = 0;
	int32 ChaosJewel = 0;
	int32 CreationJewel = 0;
	int32 BundleBless = 0;
	int32 BundleSoul = 0;
	uint8 Luck = 0;
	uint8 Ancient = 0;
	uint8 Excellent = 0;
	INIT_SOCKET_DATA(Socket);
	uint8 SocketBonus = 0xFF;

	chaos_box_loop(i)
	{
		Item const* item = this->GetPlayer()->GetMixInventory()->GetItem(i);

		if (!item->IsItem())
		{
			continue;
		}

		if (item->GetItem() == ITEMGET(14, 494))
		{
			FrozenSpirit++;
			this->AddDBFlag(item->GetDBFlag());
		}
		else if (item->GetItem() == JEWEL::CHAOS)
		{
			ChaosJewel += item->GetDurability();
		}
		else if (item->GetItem() == JEWEL::CREATION)
		{
			CreationJewel += item->GetDurability();
		}
		else if (item->GetItem() == JEWEL::BLESS)
		{
			BundleBless += item->GetDurability();
		}
		else if (item->GetItem() == JEWEL::SOUL)
		{
			BundleSoul += item->GetDurability();
		}
		else if (item->IsAncient() && item->GetLevel() >= 11 && item->GetOption() >= 4)
		{
			if (item->GetItemType() == 7 ||
				item->GetItemType() == 8 ||
				item->GetItemType() == 9 ||
				item->GetItemType() == 10 ||
				item->GetItemType() == 11)
			{
				if (item->GetItemIndex() == 164)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 189);
				}
				else if (item->GetItemIndex() == 165)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 190);
				}
				else if (item->GetItemIndex() == 166)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 191);
				}
				else if (item->GetItemIndex() == 167)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 192);
				}
				else if (item->GetItemIndex() == 168)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 193);
				}
				else if (item->GetItemIndex() == 169)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 194);
				}
				else if (item->GetItemIndex() == 170)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 195);
				}
				else if (item->GetItemIndex() == 171)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 196);
				}
				else if (item->GetItemIndex() == 172)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 197);
				}
				else if (item->GetItemIndex() == 173)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 198);
				}
				else if (item->GetItemIndex() == 129)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 199);
				}
				else if (item->GetItemIndex() == 184)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 185);
				}
				else if (item->GetItemIndex() == 204)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 205);
				}
				else if (item->GetItemIndex() == 216)
				{
					DarkAngelItem = ITEMGET(item->GetItemType(), 217);
				}
			}

			Luck = item->GetLuck();
			Ancient = item->GetAncient();
			Excellent = item->GetExe();

			SOCKET_SLOT_LOOP(j)
			{
				Socket[j] = item->GetSocket(j);
			}

			SocketBonus = item->GetSocketBonus();

			this->AddDBFlag(item->GetDBFlag());
		}
	}

	if (FrozenSpirit != 1 || DarkAngelItem == 0 || ChaosJewel != 1 || CreationJewel != 1 || BundleBless < 30 || BundleSoul < 30)
	{
		this->ChaosMixLogInvalidMixItems();
		return;
	}

	this->success_rate.set(100);
	this->mix_price = 0;

	if (!this->tributeTaxPrice())
	{
		return;
	}

	this->ChaosMixDebugMessage();

	if (this->roll_success())
	{
		Item item_create(DarkAngelItem, 0, 0, 0, Luck, 0, Excellent, Ancient, Socket, SocketBonus);
		item_create.SetDBFlag(this->GetDBFlag());

		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, item_create, 1);

		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
	}

	this->ChaosMixDBSave();
}

void MixHandler::ChaosMixArchangelsHammer(uint8 data)
{
	int32 SteelOfHeaven = 0;
	int32 ChaosJewel = 0;
	int32 CreationJewel = 0;
	int32 GuardianJewel = 0;
	int32 BundleBless = 0;
	int32 BundleSoul = 0;

	chaos_box_loop(i)
	{
		Item const* item = this->GetPlayer()->GetMixInventory()->GetItem(i);

		if ( !item->IsItem() )
		{
			continue;
		}

		if( item->GetItem() == ITEMGET(14, 342) )
		{
			SteelOfHeaven += item->GetDurability();
		}
		else if (item->GetItem() == JEWEL::CHAOS)
		{
			ChaosJewel += item->GetDurability();
		}
		else if( item->GetItem() == JEWEL::CREATION)
		{
			CreationJewel += item->GetDurability();
		}
		else if( item->GetItem() == JEWEL::GUARDIAN)
		{
			GuardianJewel += item->GetDurability();
		}
		else if( item->GetItem() == JEWEL::BLESS)
		{
			BundleBless += item->GetDurability();
		}
		else if( item->GetItem() == JEWEL::SOUL)
		{
			BundleSoul += item->GetDurability();
		}
	}

	if( SteelOfHeaven < 10 || ChaosJewel == 0 || CreationJewel == 0 || GuardianJewel == 0 || BundleBless < 10 || BundleSoul < 10 )
	{
		this->ChaosMixLogInvalidMixItems();
		return;
	}

	this->success_rate.set(100);

	this->mix_price = 0;
	
	if ( !this->tributeTaxPrice() )
	{
		return;
	}

	this->ChaosMixDebugMessage();

	if ( this->roll_success() )
	{
		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, Item(ITEMGET(14, 343)), 1);

		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
	}

	this->ChaosMixDBSave();
}

void MixHandler::ChaosMixUpgradeDivineArchangelWeapon(uint8 data)
{
	int32 ArchangelsHammer = 0;
	int32 DivineArchangelWeapon = 0;
	int32 ChaosJewel = 0;
	int32 BundleLife = 0;
	int32 BundleBless = 0;
	int32 BundleSoul = 0;
	uint8 Luck = 0;
	uint8 Excellent = 0;

	chaos_box_loop(i)
	{
		Item const* item = this->GetPlayer()->GetMixInventory()->GetItem(i);

		if ( !item->IsItem() )
		{
			continue;
		}

		if( item->GetItem() == ITEMGET(14, 343) )
		{
			ArchangelsHammer++;
			this->AddDBFlag(item->GetDBFlag());
		}
		else if (item->GetItem() == JEWEL::CHAOS)
		{
			ChaosJewel += item->GetDurability();
		}
		else if( item->GetItem() == JEWEL::LIFE)
		{
			BundleLife += item->GetDurability();
		}
		else if( item->GetItem() == JEWEL::BLESS)
		{
			BundleBless += item->GetDurability();
		}
		else if( item->GetItem() == JEWEL::SOUL)
		{
			BundleSoul += item->GetDurability();
		}
		else if ( item->GetLevel() >= 15 && item->GetOption() >= 4 )
		{
			Luck = item->GetLuck();
			Excellent = item->GetExe();
			this->AddDBFlag(item->GetDBFlag());

			if( item->GetItem() == ITEMGET(0, 19) )
			{
				DivineArchangelWeapon = ITEMGET(0, 51);
			}
			else if (item->GetItem() == ITEMGET(0, 60))
			{
				DivineArchangelWeapon = ITEMGET(0, 61);
			}
			else if( item->GetItem() == ITEMGET(2, 13) )
			{
				DivineArchangelWeapon = ITEMGET(2, 25);
			}
			else if (item->GetItem() == ITEMGET(2, 34))
			{
				DivineArchangelWeapon = ITEMGET(2, 35);
			}
			else if (item->GetItem() == ITEMGET(3, 26))
			{
				DivineArchangelWeapon = ITEMGET(3, 27);
			}
			else if( item->GetItem() == ITEMGET(4, 18) )
			{
				DivineArchangelWeapon = ITEMGET(4, 30);
			}
			else if( item->GetItem() == ITEMGET(5, 10) )
			{
				DivineArchangelWeapon = ITEMGET(5, 49);
			}
			else if( item->GetItem() == ITEMGET(5, 36) )
			{
				DivineArchangelWeapon = ITEMGET(5, 50);
			}
			else if (item->GetItem() == ITEMGET(0, 76))
			{
				DivineArchangelWeapon = ITEMGET(0, 77);
			}
		}
	}

	if( ArchangelsHammer == 0 || DivineArchangelWeapon == 0 || ChaosJewel == 0 || BundleLife < 30 || BundleBless < 30 || BundleSoul < 30 )
	{
		this->ChaosMixLogInvalidMixItems();
		return;
	}

	this->success_rate.set(100);
	this->mix_price = 0;
	
	if ( !this->tributeTaxPrice() )
	{
		return;
	}

	this->ChaosMixDebugMessage();

	if ( this->roll_success() )
	{
		if ( Excellent == 0 )
		{
			Excellent = ExeOption::BA_INCREASE_EXE_DAMAGE_RATE | ExeOption::BA_INCREASE_DAMAGE;

			uint8 option1 = Random(10);
			uint8 option2 = Random(2);

			switch ( option1 )
			{
			case 0:
				{
					Excellent |= ExeOption::BA_INCREASE_HP_RECOVERY_AFTER_KILL_MONSTER;
				} break;

			case 1:
				{
					Excellent |= ExeOption::BA_INCREASE_MANA_RECOVERY_AFTER_KILL_MONSTER;
				} break;
			}

			switch ( option2 )
			{
			case 0:
				{
					Excellent |= ExeOption::BA_INCREASE_ATTACK_SPEED;
				} break;

			case 1:
				{
					Excellent |= ExeOption::BA_INCREASE_DAMAGE_PER_LEVEL;
				} break;
			}
		}

		if ( Luck == 0 )
		{
			Luck = roll_chance_i(10) ? 1 : 0;
		}

		Item item_create(DivineArchangelWeapon, 0, 0, 1, Luck, 0, Excellent);
		item_create.SetDBFlag(this->GetDBFlag());

		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, item_create, 1);

		this->ChaosMixLogSuccess();
	}
	else
	{
		this->GetPlayer()->GetInterfaceState()->SetState(0);
		this->ChaosMixArchangelItemDown();
		this->GetPlayer()->ChaosMixSend(0);
		this->GetPlayer()->ChaosMixResult(CHAOS_MIX_RESULT_FAIL);
		this->ChaosMixLogFail();
	}

	this->ChaosMixDBSave();
}

void MixHandler::ChaosMixUpgradeBloodangelToDarkangelWeapon(uint8 data)
{
	int32 Anvil = 0;
	int32 Weapon = 0;
	int32 ChaosJewel = 0;
	int32 CreationJewel = 0;
	int32 BundleBless = 0;
	int32 BundleSoul = 0;
	uint8 Luck = 0;
	uint8 Excellent = 0;
	uint8 Skill = 0;
	INIT_SOCKET_DATA(Socket);
	uint8 SocketBonus = 0xFF;

	chaos_box_loop(i)
	{
		Item const* item = this->GetPlayer()->GetMixInventory()->GetItem(i);

		if ( !item->IsItem() )
		{
			continue;
		}

		if( item->GetItem() == ITEMGET(14, 386) )
		{
			Anvil++;
			this->AddDBFlag(item->GetDBFlag());
		}
		else if (item->GetItem() == JEWEL::CHAOS)
		{
			ChaosJewel += item->GetDurability();
		}
		else if( item->GetItem() == JEWEL::CREATION)
		{
			CreationJewel += item->GetDurability();
		}
		else if( item->GetItem() == JEWEL::BLESS)
		{
			BundleBless += item->GetDurability();
		}
		else if( item->GetItem() == JEWEL::SOUL)
		{
			BundleSoul += item->GetDurability();
		}
		else if ( item->GetLevel() >= 11 && item->GetOption() >= 4 )
		{
			Luck = item->GetLuck();
			Excellent = item->GetExe();
			Skill = item->GetSkill();

			SOCKET_SLOT_LOOP(j)
			{
				Socket[j] = item->GetSocket(j);
			}

			SocketBonus = item->GetSocketBonus();

			this->AddDBFlag(item->GetDBFlag());
			
			if( item->GetItem() == ITEMGET(0, 42) )
			{
				Weapon = ITEMGET(0, 54);
			}
			else if( item->GetItem() == ITEMGET(0, 44) )
			{
				Weapon = ITEMGET(0, 55);
			}
			else if( item->GetItem() == ITEMGET(0, 46) )
			{
				Weapon = ITEMGET(0, 56);
			}
			else if( item->GetItem() == ITEMGET(2, 22) )
			{
				Weapon = ITEMGET(2, 26);
			}
			else if (item->GetItem() == ITEMGET(2, 32))
			{
				Weapon = ITEMGET(2, 33);
			}
			else if( item->GetItem() == ITEMGET(3, 19) )
			{
				Weapon = ITEMGET(3, 24);
			}
			else if( item->GetItem() == ITEMGET(4, 28) )
			{
				Weapon = ITEMGET(4, 31);
			}
			else if( item->GetItem() == ITEMGET(5, 41) )
			{
				Weapon = ITEMGET(5, 51);
			}
			else if( item->GetItem() == ITEMGET(5, 43) )
			{
				Weapon = ITEMGET(5, 52);
			}
			else if( item->GetItem() == ITEMGET(4, 34) )
			{
				Weapon = ITEMGET(4, 35);
			}
			else if (item->GetItem() == ITEMGET(5, 57))
			{
				Weapon = ITEMGET(5, 58);
			}
			else if (item->GetItem() == ITEMGET(0, 71))
			{
				Weapon = ITEMGET(0, 72);
			}
			else if (item->GetItem() == ITEMGET(6, 44))
			{
				Weapon = ITEMGET(6, 45);
			}
		}
	}

	if (Anvil != 1 || Weapon == 0 || ChaosJewel != 1 || CreationJewel != 1 || BundleBless < 30 || BundleSoul < 30)
	{
		this->ChaosMixLogInvalidMixItems();
		return;
	}

	this->success_rate.set(100);
	this->mix_price = 0;
	
	if ( !this->tributeTaxPrice() )
	{
		return;
	}

	this->ChaosMixDebugMessage();

	if ( this->roll_success() )
	{
		Item item_create(Weapon, 0, 0, Skill, Luck, 0, Excellent, 0, Socket, SocketBonus);
		item_create.SetDBFlag(this->GetDBFlag());

		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, item_create, 1);

		this->ChaosMixLogSuccess();
	}
	else
	{
		this->GetPlayer()->GetInterfaceState()->SetState(0);
		this->GetPlayer()->ChaosMixSend(0);
		this->GetPlayer()->ChaosMixResult(CHAOS_MIX_RESULT_FAIL);
		this->ChaosMixLogFail();
	}

	this->ChaosMixDBSave();
}

void MixHandler::ChaosMixUpgradeDarkangelToHolyangelWeapon(uint8 data)
{
	int32 Anvil = 0;
	int32 Weapon = 0;
	int32 ChaosJewel = 0;
	int32 CreationJewel = 0;
	int32 BundleBless = 0;
	int32 BundleSoul = 0;
	uint8 Luck = 0;
	uint8 Excellent = 0;
	uint8 Skill = 0;
	INIT_SOCKET_DATA(Socket);
	uint8 SocketBonus = 0xFF;

	chaos_box_loop(i)
	{
		Item const* item = this->GetPlayer()->GetMixInventory()->GetItem(i);

		if (!item->IsItem())
		{
			continue;
		}

		if (item->GetItem() == ITEMGET(14, 416))
		{
			Anvil++;
			this->AddDBFlag(item->GetDBFlag());
		}
		else if (item->GetItem() == JEWEL::CHAOS)
		{
			ChaosJewel += item->GetDurability();
		}
		else if (item->GetItem() == JEWEL::CREATION)
		{
			CreationJewel += item->GetDurability();
		}
		else if (item->GetItem() == JEWEL::BLESS)
		{
			BundleBless += item->GetDurability();
		}
		else if (item->GetItem() == JEWEL::SOUL)
		{
			BundleSoul += item->GetDurability();
		}
		else if (item->GetLevel() >= 11 && item->GetOption() >= 4)
		{
			Luck = item->GetLuck();
			Excellent = item->GetExe();
			Skill = item->GetSkill();

			SOCKET_SLOT_LOOP(j)
			{
				Socket[j] = item->GetSocket(j);
			}

			SocketBonus = item->GetSocketBonus();

			this->AddDBFlag(item->GetDBFlag());

			if (item->GetItem() == ITEMGET(0, 54))
			{
				Weapon = ITEMGET(0, 57);
			}
			else if (item->GetItem() == ITEMGET(0, 55))
			{
				Weapon = ITEMGET(0, 58);
			}
			else if (item->GetItem() == ITEMGET(0, 56))
			{
				Weapon = ITEMGET(0, 59);
			}
			else if (item->GetItem() == ITEMGET(2, 26))
			{
				Weapon = ITEMGET(2, 27);
			}
			else if (item->GetItem() == ITEMGET(2, 33))
			{
				Weapon = ITEMGET(2, 38);
			}
			else if (item->GetItem() == ITEMGET(3, 24))
			{
				Weapon = ITEMGET(3, 25);
			}
			else if (item->GetItem() == ITEMGET(4, 31))
			{
				Weapon = ITEMGET(4, 36);
			}
			else if (item->GetItem() == ITEMGET(5, 51))
			{
				Weapon = ITEMGET(5, 53);
			}
			else if (item->GetItem() == ITEMGET(5, 52))
			{
				Weapon = ITEMGET(5, 54);
			}
			else if (item->GetItem() == ITEMGET(4, 35))
			{
				Weapon = ITEMGET(4, 37);
			}
			else if (item->GetItem() == ITEMGET(5, 58))
			{
				Weapon = ITEMGET(5, 59);
			}
			else if (item->GetItem() == ITEMGET(0, 72))
			{
				Weapon = ITEMGET(0, 73);
			}
			else if (item->GetItem() == ITEMGET(6, 45))
			{
				Weapon = ITEMGET(6, 46);
			}
		}
	}

	if (Anvil != 1 || Weapon == 0 || ChaosJewel != 1 || CreationJewel != 1 || BundleBless < 30 || BundleSoul < 30)
	{
		this->ChaosMixLogInvalidMixItems();
		return;
	}

	this->success_rate.set(100);
	this->mix_price = 0;

	if (!this->tributeTaxPrice())
	{
		return;
	}

	this->ChaosMixDebugMessage();

	if (this->roll_success())
	{
		Item item_create(Weapon, 0, 0, Skill, Luck, 0, Excellent, 0, Socket, SocketBonus);
		item_create.SetDBFlag(this->GetDBFlag());

		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, item_create, 1);

		this->ChaosMixLogSuccess();
	}
	else
	{
		this->GetPlayer()->GetInterfaceState()->SetState(0);
		this->GetPlayer()->ChaosMixSend(0);
		this->GetPlayer()->ChaosMixResult(CHAOS_MIX_RESULT_FAIL);
		this->ChaosMixLogFail();
	}

	this->ChaosMixDBSave();
}

void MixHandler::ChaosMixUpgradeHolyangelToSoulWeapon(uint8 data)
{
	int32 Anvil = 0;
	int32 Weapon = 0;
	int32 ChaosJewel = 0;
	int32 CreationJewel = 0;
	int32 BundleBless = 0;
	int32 BundleSoul = 0;
	uint8 Luck = 0;
	uint8 Excellent = 0;
	uint8 Skill = 0;
	INIT_SOCKET_DATA(Socket);
	uint8 SocketBonus = 0xFF;

	chaos_box_loop(i)
	{
		Item const* item = this->GetPlayer()->GetMixInventory()->GetItem(i);

		if (!item->IsItem())
		{
			continue;
		}

		if (item->GetItem() == ITEMGET(14, 450))
		{
			Anvil++;
			this->AddDBFlag(item->GetDBFlag());
		}
		else if (item->GetItem() == JEWEL::CHAOS)
		{
			ChaosJewel += item->GetDurability();
		}
		else if (item->GetItem() == JEWEL::CREATION)
		{
			CreationJewel += item->GetDurability();
		}
		else if (item->GetItem() == JEWEL::BLESS)
		{
			BundleBless += item->GetDurability();
		}
		else if (item->GetItem() == JEWEL::SOUL)
		{
			BundleSoul += item->GetDurability();
		}
		else if (item->GetLevel() >= 11 && item->GetOption() >= 4)
		{
			Luck = item->GetLuck();
			Excellent = item->GetExe();
			Skill = item->GetSkill();

			SOCKET_SLOT_LOOP(j)
			{
				Socket[j] = item->GetSocket(j);
			}

			SocketBonus = item->GetSocketBonus();

			this->AddDBFlag(item->GetDBFlag());

			if (item->GetItem() == ITEMGET(0, 57))
			{
				Weapon = ITEMGET(0, 62);
			}
			else if (item->GetItem() == ITEMGET(0, 58))
			{
				Weapon = ITEMGET(0, 63);
			}
			else if (item->GetItem() == ITEMGET(0, 59))
			{
				Weapon = ITEMGET(0, 64);
			}
			else if (item->GetItem() == ITEMGET(2, 27))
			{
				Weapon = ITEMGET(2, 36);
			}
			else if (item->GetItem() == ITEMGET(2, 38))
			{
				Weapon = ITEMGET(2, 39);
			}
			else if (item->GetItem() == ITEMGET(3, 25))
			{
				Weapon = ITEMGET(3, 28);
			}
			else if (item->GetItem() == ITEMGET(4, 36))
			{
				Weapon = ITEMGET(4, 38);
			}
			else if (item->GetItem() == ITEMGET(5, 53))
			{
				Weapon = ITEMGET(5, 55);
			}
			else if (item->GetItem() == ITEMGET(5, 54))
			{
				Weapon = ITEMGET(5, 56);
			}
			else if (item->GetItem() == ITEMGET(4, 37))
			{
				Weapon = ITEMGET(4, 39);
			}
			else if (item->GetItem() == ITEMGET(5, 59))
			{
				Weapon = ITEMGET(5, 60);
			}
			else if (item->GetItem() == ITEMGET(0, 73))
			{
				Weapon = ITEMGET(0, 74);
			}
			else if (item->GetItem() == ITEMGET(6, 46))
			{
				Weapon = ITEMGET(6, 47);
			}
		}
	}

	if (Anvil != 1 || Weapon == 0 || ChaosJewel != 1 || CreationJewel != 1 || BundleBless < 30 || BundleSoul < 30)
	{
		this->ChaosMixLogInvalidMixItems();
		return;
	}

	this->success_rate.set(100);
	this->mix_price = 0;

	if (!this->tributeTaxPrice())
	{
		return;
	}

	this->ChaosMixDebugMessage();

	if (this->roll_success())
	{
		Item item_create(Weapon, 0, 0, Skill, Luck, 0, Excellent, 0, Socket, SocketBonus);
		item_create.SetDBFlag(this->GetDBFlag());

		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, item_create, 1);

		this->ChaosMixLogSuccess();
	}
	else
	{
		this->GetPlayer()->GetInterfaceState()->SetState(0);
		this->GetPlayer()->ChaosMixSend(0);
		this->GetPlayer()->ChaosMixResult(CHAOS_MIX_RESULT_FAIL);
		this->ChaosMixLogFail();
	}

	this->ChaosMixDBSave();
}

void MixHandler::ChaosMixUpgradeSoulToBlueEyeWeapon(uint8 data)
{
	int32 Anvil = 0;
	int32 Weapon = 0;
	int32 ChaosJewel = 0;
	int32 CreationJewel = 0;
	int32 BundleBless = 0;
	int32 BundleSoul = 0;
	uint8 Luck = 0;
	uint8 Excellent = 0;
	uint8 Skill = 0;
	INIT_SOCKET_DATA(Socket);
	uint8 SocketBonus = 0xFF;

	chaos_box_loop(i)
	{
		Item const* item = this->GetPlayer()->GetMixInventory()->GetItem(i);

		if (!item->IsItem())
		{
			continue;
		}

		if (item->GetItem() == ITEMGET(14, 495))
		{
			Anvil++;
			this->AddDBFlag(item->GetDBFlag());
		}
		else if (item->GetItem() == JEWEL::CHAOS)
		{
			ChaosJewel += item->GetDurability();
		}
		else if (item->GetItem() == JEWEL::CREATION)
		{
			CreationJewel += item->GetDurability();
		}
		else if (item->GetItem() == JEWEL::BLESS)
		{
			BundleBless += item->GetDurability();
		}
		else if (item->GetItem() == JEWEL::SOUL)
		{
			BundleSoul += item->GetDurability();
		}
		else if (item->GetLevel() >= 11 && item->GetOption() >= 4)
		{
			Luck = item->GetLuck();
			Excellent = item->GetExe();
			Skill = item->GetSkill();

			SOCKET_SLOT_LOOP(j)
			{
				Socket[j] = item->GetSocket(j);
			}

			SocketBonus = item->GetSocketBonus();

			this->AddDBFlag(item->GetDBFlag());

			if (item->GetItem() == ITEMGET(0, 62))
			{
				Weapon = ITEMGET(0, 78);
			}
			else if (item->GetItem() == ITEMGET(0, 63))
			{
				Weapon = ITEMGET(0, 79);
			}
			else if (item->GetItem() == ITEMGET(0, 64))
			{
				Weapon = ITEMGET(0, 80);
			}
			else if (item->GetItem() == ITEMGET(2, 36))
			{
				Weapon = ITEMGET(2, 40);
			}
			else if (item->GetItem() == ITEMGET(2, 39))
			{
				Weapon = ITEMGET(2, 41);
			}
			else if (item->GetItem() == ITEMGET(3, 28))
			{
				Weapon = ITEMGET(3, 29);
			}
			else if (item->GetItem() == ITEMGET(4, 38))
			{
				Weapon = ITEMGET(4, 40);
			}
			else if (item->GetItem() == ITEMGET(4, 39))
			{
				Weapon = ITEMGET(4, 41);
			}
			else if (item->GetItem() == ITEMGET(5, 55))
			{
				Weapon = ITEMGET(5, 61);
			}
			else if (item->GetItem() == ITEMGET(5, 56))
			{
				Weapon = ITEMGET(5, 62);
			}
			else if (item->GetItem() == ITEMGET(5, 60))
			{
				Weapon = ITEMGET(5, 63);
			}
			else if (item->GetItem() == ITEMGET(0, 74))
			{
				Weapon = ITEMGET(0, 75);
			}
			else if (item->GetItem() == ITEMGET(6, 47))
			{
				Weapon = ITEMGET(6, 48);
			}
		}
	}

	if (Anvil != 1 || Weapon == 0 || ChaosJewel != 1 || CreationJewel != 1 || BundleBless < 30 || BundleSoul < 30)
	{
		this->ChaosMixLogInvalidMixItems();
		return;
	}

	this->success_rate.set(100);
	this->mix_price = 0;

	if (!this->tributeTaxPrice())
	{
		return;
	}

	this->ChaosMixDebugMessage();

	if (this->roll_success())
	{
		Item item_create(Weapon, 0, 0, Skill, Luck, 0, Excellent, 0, Socket, SocketBonus);
		item_create.SetDBFlag(this->GetDBFlag());

		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, item_create, 1);

		this->ChaosMixLogSuccess();
	}
	else
	{
		this->GetPlayer()->GetInterfaceState()->SetState(0);
		this->GetPlayer()->ChaosMixSend(0);
		this->GetPlayer()->ChaosMixResult(CHAOS_MIX_RESULT_FAIL);
		this->ChaosMixLogFail();
	}

	this->ChaosMixDBSave();
}

void MixHandler::ChaosMixSealedBloodangelHero(uint8 data)
{
	int32 BloodAngelsSpirit = 0;
	int32 SealedMasteryID = 0;

	chaos_box_loop(i)
	{
		Item const* item = this->GetPlayer()->GetMixInventory()->GetItem(i);

		if ( !item->IsItem() )
		{
			continue;
		}

		this->AddDBFlag(item->GetDBFlag());

		if( item->GetItem() == ITEMGET(14, 399) )
		{
			BloodAngelsSpirit++;
		}
		else if( item->GetItem() == ITEMGET(13, 351) ) // Fury
		{
			SealedMasteryID = ITEMGET(8, 98);
		}
		else if( item->GetItem() == ITEMGET(13, 359) ) // Transcendence
		{
			SealedMasteryID = ITEMGET(8, 99);
		}
		else if( item->GetItem() == ITEMGET(13, 363) ) // Flurry
		{
			SealedMasteryID = ITEMGET(8, 100);
		}
		else if( item->GetItem() == ITEMGET(13, 371) ) // Extremity
		{
			SealedMasteryID = ITEMGET(8, 102);
		}
		else if( item->GetItem() == ITEMGET(13, 379) ) // Conquest
		{
			SealedMasteryID = ITEMGET(8, 103);
		}
		else if( item->GetItem() == ITEMGET(13, 387) ) // Honor
		{
			SealedMasteryID = ITEMGET(8, 101);
		}
		else if( item->GetItem() == ITEMGET(13, 391) ) // Destruction
		{
			SealedMasteryID = ITEMGET(8, 104);
		}
		else if( item->GetItem() == ITEMGET(13, 399) ) // Tenacity
		{
			SealedMasteryID = ITEMGET(8, 105);
		}
		else if( item->GetItem() == ITEMGET(13, 352) )
		{
			SealedMasteryID = ITEMGET(7, 98);
		}
		else if( item->GetItem() == ITEMGET(13, 360) )
		{
			SealedMasteryID = ITEMGET(7, 99);
		}
		else if( item->GetItem() == ITEMGET(13, 364) )
		{
			SealedMasteryID = ITEMGET(7, 100);
		}
		else if( item->GetItem() == ITEMGET(13, 380) )
		{
			SealedMasteryID = ITEMGET(7, 103);
		}
		else if( item->GetItem() == ITEMGET(13, 388) )
		{
			SealedMasteryID = ITEMGET(7, 101);
		}
		else if( item->GetItem() == ITEMGET(13, 392) )
		{
			SealedMasteryID = ITEMGET(7, 104);
		}
		else if( item->GetItem() == ITEMGET(13, 400) )
		{
			SealedMasteryID = ITEMGET(7, 105);
		}
		else if( item->GetItem() == ITEMGET(13, 353) )
		{
			SealedMasteryID = ITEMGET(9, 98);
		}
		else if( item->GetItem() == ITEMGET(13, 361) )
		{
			SealedMasteryID = ITEMGET(9, 99);
		}
		else if( item->GetItem() == ITEMGET(13, 365) )
		{
			SealedMasteryID = ITEMGET(9, 100);
		}
		else if( item->GetItem() == ITEMGET(13, 373) )
		{
			SealedMasteryID = ITEMGET(9, 102);
		}
		else if( item->GetItem() == ITEMGET(13, 381) )
		{
			SealedMasteryID = ITEMGET(9, 103);
		}
		else if( item->GetItem() == ITEMGET(13, 389) )
		{
			SealedMasteryID = ITEMGET(9, 101);
		}
		else if( item->GetItem() == ITEMGET(13, 393) )
		{
			SealedMasteryID = ITEMGET(9, 104);
		}
		else if( item->GetItem() == ITEMGET(13, 401) )
		{
			SealedMasteryID = ITEMGET(9, 105);
		}
		else if( item->GetItem() == ITEMGET(13, 362) )
		{
			SealedMasteryID = ITEMGET(10, 99);
		}
		else if( item->GetItem() == ITEMGET(13, 366) )
		{
			SealedMasteryID = ITEMGET(10, 100);
		}
		else if( item->GetItem() == ITEMGET(13, 374) )
		{
			SealedMasteryID = ITEMGET(10, 102);
		}
		else if( item->GetItem() == ITEMGET(13, 382) )
		{
			SealedMasteryID = ITEMGET(10, 103);
		}
		else if( item->GetItem() == ITEMGET(13, 390) )
		{
			SealedMasteryID = ITEMGET(10, 101);
		}
		else if( item->GetItem() == ITEMGET(13, 402) )
		{
			SealedMasteryID = ITEMGET(10, 105);
		}
		else if( item->GetItem() == ITEMGET(13, 354) )
		{
			SealedMasteryID = ITEMGET(11, 98);
		}
		else if( item->GetItem() == ITEMGET(13, 372) )
		{
			SealedMasteryID = ITEMGET(11, 102);
		}
		else if( item->GetItem() == ITEMGET(13, 394) )
		{
			SealedMasteryID = ITEMGET(11, 104);
		}
		else if( item->GetItem() == ITEMGET(13, 367) )
		{
			SealedMasteryID = ITEMGET(8, 158);
		}
		else if( item->GetItem() == ITEMGET(13, 368) )
		{
			SealedMasteryID = ITEMGET(7, 158);
		}
		else if( item->GetItem() == ITEMGET(13, 369) )
		{
			SealedMasteryID = ITEMGET(9, 158);
		}
		else if( item->GetItem() == ITEMGET(13, 370) )
		{
			SealedMasteryID = ITEMGET(10, 158);
		}
		else if( item->GetItem() == ITEMGET(13, 375) )
		{
			SealedMasteryID = ITEMGET(8, 161);
		}
		else if( item->GetItem() == ITEMGET(13, 376) )
		{
			SealedMasteryID = ITEMGET(11, 161);
		}
		else if( item->GetItem() == ITEMGET(13, 377) )
		{
			SealedMasteryID = ITEMGET(9, 161);
		}
		else if( item->GetItem() == ITEMGET(13, 378) )
		{
			SealedMasteryID = ITEMGET(10, 161);
		}
		else if (item->GetItem() == ITEMGET(13, 355))
		{
			SealedMasteryID = ITEMGET(8, 122);
		}
		else if (item->GetItem() == ITEMGET(13, 356))
		{
			SealedMasteryID = ITEMGET(7, 122);
		}
		else if (item->GetItem() == ITEMGET(13, 357))
		{
			SealedMasteryID = ITEMGET(9, 122);
		}
		else if (item->GetItem() == ITEMGET(13, 358))
		{
			SealedMasteryID = ITEMGET(10, 122);
		}
	}

	if( BloodAngelsSpirit != 1 || SealedMasteryID == 0 )
	{
		this->ChaosMixLogInvalidMixItems();
		return;
	}

	uint8 ancient = sItemMgr->GetRandomOptionAncient(SealedMasteryID);

	if ( !ancient )
	{
		this->ChaosMixLogInvalidMixItems();
		return;
	}
	
	this->success_rate.set(100);
	this->mix_price = 0;
	
	if ( !this->tributeTaxPrice() )
	{
		return;
	}

	this->ChaosMixDebugMessage();

	if ( this->roll_success() )
	{
		Item item_create(SealedMasteryID, 11, 0, 0, 0, 0, 0, ancient);
		item_create.SetDBFlag(this->GetDBFlag());

		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, item_create, 1);

		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
	}

	this->ChaosMixDBSave();
}

void MixHandler::ChaosMixElfConversionAttackToSupport(uint8 data)
{
	int32 ConversionOrder = 0;
	int32 ConversionItem = 0;
	int32 ChaosJewel = 0;
	int32 BlessJewel = 0;
	int32 SoulJewel = 0;
	uint8 Slot = -1;
	
	chaos_box_loop(i)
	{
		Item const* item = this->GetPlayer()->GetMixInventory()->GetItem(i);

		if ( !item->IsItem() )
		{
			continue;
		}

		if( item->GetItem() == ITEMGET(14, 395) )
		{
			ConversionOrder++;
			this->AddDBFlag(item->GetDBFlag());
		}
		else if (item->GetItem() == JEWEL::CHAOS)
		{
			ChaosJewel += item->GetDurability();
		}
		else if( item->GetItem() == JEWEL::BLESS)
		{
			BlessJewel += item->GetDurability();
		}
		else if( item->GetItem() == JEWEL::SOUL)
		{
			SoulJewel += item->GetDurability();
		}
		else if ( item->IsAncient() )
		{
			if (item->GetItemType() == 7 ||
				item->GetItemType() == 8 ||
				item->GetItemType() == 9 ||
				item->GetItemType() == 10 ||
				item->GetItemType() == 11)
			{
				Slot = i;
				this->AddDBFlag(item->GetDBFlag());

				if (item->GetItemIndex() == 100)
				{
					ConversionItem = ITEMGET(item->GetItemType(), 158);
				}
				else if (item->GetItemIndex() == 140)
				{
					ConversionItem = ITEMGET(item->GetItemType(), 159);
				}
				else if (item->GetItemIndex() == 152)
				{
					ConversionItem = ITEMGET(item->GetItemType(), 160);
				}
				else if (item->GetItemIndex() == 108)
				{
					ConversionItem = ITEMGET(item->GetItemType(), 114);
				}
				else if (item->GetItemIndex() == 166)
				{
					ConversionItem = ITEMGET(item->GetItemType(), 172);
				}
				else if (item->GetItemIndex() == 191)
				{
					ConversionItem = ITEMGET(item->GetItemType(), 197);
				}
			}
		}
	}

	if( ConversionOrder != 1 || ChaosJewel != 1 || BlessJewel < 10 || SoulJewel < 10 || ConversionItem == 0 || Slot == uint8(-1) )
	{
		this->ChaosMixLogInvalidMixItems();
		return;
	}

	this->success_rate.set(100);
	this->mix_price = 0;
	
	if ( !this->tributeTaxPrice() )
	{
		return;
	}

	this->ChaosMixDebugMessage();

	if ( this->roll_success() )
	{
		Item const* pItem = this->GetPlayer()->GetMixInventory()->GetItem(Slot);

		INIT_SOCKET_DATA(socket_data);
		SOCKET_SLOT_LOOP(i)
		{
			socket_data[i] = pItem->GetSocket(i);
		}

		Item item_create(ConversionItem, pItem->GetLevel(), 0, pItem->GetSkill(), pItem->GetLuck(), pItem->GetOption(), pItem->GetExe(), pItem->GetAncient(), socket_data, pItem->GetSocketBonus());
		item_create.SetDBFlag(this->GetDBFlag());

		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, item_create, 1);

		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
	}

	this->ChaosMixDBSave();
}

void MixHandler::ChaosMixElfConversionSupportToAttack(uint8 data)
{
	int32 ConversionOrder = 0;
	int32 ConversionItem = 0;
	int32 ChaosJewel = 0;
	int32 BlessJewel = 0;
	int32 SoulJewel = 0;
	uint8 Slot = -1;
	
	chaos_box_loop(i)
	{
		Item const* item = this->GetPlayer()->GetMixInventory()->GetItem(i);

		if ( !item->IsItem() )
		{
			continue;
		}

		if( item->GetItem() == ITEMGET(14, 394) )
		{
			ConversionOrder++;
			this->AddDBFlag(item->GetDBFlag());
		}
		else if (item->GetItem() == JEWEL::CHAOS)
		{
			ChaosJewel += item->GetDurability();
		}
		else if( item->GetItem() == JEWEL::BLESS)
		{
			BlessJewel += item->GetDurability();
		}
		else if( item->GetItem() == JEWEL::SOUL)
		{
			SoulJewel += item->GetDurability();
		}
		else if ( item->IsAncient() )
		{
			if (item->GetItemType() == 7 ||
				item->GetItemType() == 8 ||
				item->GetItemType() == 9 ||
				item->GetItemType() == 10 ||
				item->GetItemType() == 11)
			{
				Slot = i;
				this->AddDBFlag(item->GetDBFlag());

				if (item->GetItemIndex() == 158)
				{
					ConversionItem = ITEMGET(item->GetItemType(), 100);
				}
				else if (item->GetItemIndex() == 159)
				{
					ConversionItem = ITEMGET(item->GetItemType(), 140);
				}
				else if (item->GetItemIndex() == 160)
				{
					ConversionItem = ITEMGET(item->GetItemType(), 152);
				}
				else if (item->GetItemIndex() == 114)
				{
					ConversionItem = ITEMGET(item->GetItemType(), 108);
				}
				else if (item->GetItemIndex() == 172)
				{
					ConversionItem = ITEMGET(item->GetItemType(), 166);
				}
				else if (item->GetItemIndex() == 197)
				{
					ConversionItem = ITEMGET(item->GetItemType(), 191);
				}
			}
		}
	}

	if( ConversionOrder != 1 || ChaosJewel != 1 || BlessJewel < 10 || SoulJewel < 10 || ConversionItem == 0 || Slot == uint8(-1) )
	{
		this->ChaosMixLogInvalidMixItems();
		return;
	}

	this->success_rate.set(100);
	this->mix_price = 0;
	
	if ( !this->tributeTaxPrice() )
	{
		return;
	}

	this->ChaosMixDebugMessage();

	if ( this->roll_success() )
	{
		Item const* pItem = this->GetPlayer()->GetMixInventory()->GetItem(Slot);

		INIT_SOCKET_DATA(socket_data);
		SOCKET_SLOT_LOOP(i)
		{
			socket_data[i] = pItem->GetSocket(i);
		}

		Item item_create(ConversionItem, pItem->GetLevel(), 0, pItem->GetSkill(), pItem->GetLuck(), pItem->GetOption(), pItem->GetExe(), pItem->GetAncient(), socket_data, pItem->GetSocketBonus());
		item_create.SetDBFlag(this->GetDBFlag());

		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, item_create, 1);

		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
	}

	this->ChaosMixDBSave();
}

void MixHandler::ChaosMixMGConversionAttackToMagic(uint8 data)
{
	int32 ConversionOrder = 0;
	int32 ConversionItem = 0;
	int32 ChaosJewel = 0;
	int32 BlessJewel = 0;
	int32 SoulJewel = 0;
	uint8 Slot = -1;
	
	chaos_box_loop(i)
	{
		Item const* item = this->GetPlayer()->GetMixInventory()->GetItem(i);

		if ( !item->IsItem() )
		{
			continue;
		}

		if( item->GetItem() == ITEMGET(14, 397) )
		{
			ConversionOrder++;
			this->AddDBFlag(item->GetDBFlag());
		}
		else if (item->GetItem() == JEWEL::CHAOS)
		{
			ChaosJewel += item->GetDurability();
		}
		else if( item->GetItem() == JEWEL::BLESS)
		{
			BlessJewel += item->GetDurability();
		}
		else if( item->GetItem() == JEWEL::SOUL)
		{
			SoulJewel += item->GetDurability();
		}
		else if ( item->IsAncient() )
		{
			if (item->GetItemType() == 7 ||
				item->GetItemType() == 8 ||
				item->GetItemType() == 9 ||
				item->GetItemType() == 10 ||
				item->GetItemType() == 11)
			{
				Slot = i;
				this->AddDBFlag(item->GetDBFlag());

				if (item->GetItemIndex() == 102)
				{
					ConversionItem = ITEMGET(item->GetItemType(), 161);
				}
				else if (item->GetItemIndex() == 142)
				{
					ConversionItem = ITEMGET(item->GetItemType(), 162);
				}
				else if (item->GetItemIndex() == 154)
				{
					ConversionItem = ITEMGET(item->GetItemType(), 163);
				}
				else if (item->GetItemIndex() == 110)
				{
					ConversionItem = ITEMGET(item->GetItemType(), 115);
				}
				else if (item->GetItemIndex() == 168)
				{
					ConversionItem = ITEMGET(item->GetItemType(), 173);
				}
				else if (item->GetItemIndex() == 193)
				{
					ConversionItem = ITEMGET(item->GetItemType(), 198);
				}
			}
		}
	}

	if( ConversionOrder != 1 || ChaosJewel != 1 || BlessJewel < 10 || SoulJewel < 10 || ConversionItem == 0 || Slot == uint8(-1) )
	{
		this->ChaosMixLogInvalidMixItems();
		return;
	}

	this->success_rate.set(100);
	this->mix_price = 0;
	
	if ( !this->tributeTaxPrice() )
	{
		return;
	}

	this->ChaosMixDebugMessage();

	if ( this->roll_success() )
	{
		Item const* pItem = this->GetPlayer()->GetMixInventory()->GetItem(Slot);

		INIT_SOCKET_DATA(socket_data);
		SOCKET_SLOT_LOOP(i)
		{
			socket_data[i] = pItem->GetSocket(i);
		}

		Item item_create(ConversionItem, pItem->GetLevel(), 0, pItem->GetSkill(), pItem->GetLuck(), pItem->GetOption(), pItem->GetExe(), pItem->GetAncient(), socket_data, pItem->GetSocketBonus());
		item_create.SetDBFlag(this->GetDBFlag());
		
		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, item_create, 1);

		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
	}

	this->ChaosMixDBSave();
}

void MixHandler::ChaosMixMGConversionMagicToAttack(uint8 data)
{
	int32 ConversionOrder = 0;
	int32 ConversionItem = 0;
	int32 ChaosJewel = 0;
	int32 BlessJewel = 0;
	int32 SoulJewel = 0;
	uint8 Slot = -1;
	
	chaos_box_loop(i)
	{
		Item const* item = this->GetPlayer()->GetMixInventory()->GetItem(i);

		if ( !item->IsItem() )
		{
			continue;
		}

		if( item->GetItem() == ITEMGET(14, 396) )
		{
			ConversionOrder++;
			this->AddDBFlag(item->GetDBFlag());
		}
		else if (item->GetItem() == JEWEL::CHAOS)
		{
			ChaosJewel += item->GetDurability();
		}
		else if( item->GetItem() == JEWEL::BLESS)
		{
			BlessJewel += item->GetDurability();
		}
		else if( item->GetItem() == JEWEL::SOUL)
		{
			SoulJewel += item->GetDurability();
		}
		else if ( item->IsAncient() )
		{
			if (item->GetItemType() == 7 ||
				item->GetItemType() == 8 ||
				item->GetItemType() == 9 ||
				item->GetItemType() == 10 ||
				item->GetItemType() == 11)
			{
				Slot = i;
				this->AddDBFlag(item->GetDBFlag());

				if (item->GetItemIndex() == 161)
				{
					ConversionItem = ITEMGET(item->GetItemType(), 102);
				}
				else if (item->GetItemIndex() == 162)
				{
					ConversionItem = ITEMGET(item->GetItemType(), 142);
				}
				else if (item->GetItemIndex() == 163)
				{
					ConversionItem = ITEMGET(item->GetItemType(), 154);
				}
				else if (item->GetItemIndex() == 115)
				{
					ConversionItem = ITEMGET(item->GetItemType(), 110);
				}
				else if (item->GetItemIndex() == 173)
				{
					ConversionItem = ITEMGET(item->GetItemType(), 168);
				}
				else if (item->GetItemIndex() == 198)
				{
					ConversionItem = ITEMGET(item->GetItemType(), 193);
				}
			}
		}
	}

	if( ConversionOrder != 1 || ChaosJewel != 1 || BlessJewel < 10 || SoulJewel < 10 || ConversionItem == 0 || Slot == uint8(-1) )
	{
		this->ChaosMixLogInvalidMixItems();
		return;
	}

	this->success_rate.set(100);
	this->mix_price = 0;
	
	if ( !this->tributeTaxPrice() )
	{
		return;
	}

	this->ChaosMixDebugMessage();

	if ( this->roll_success() )
	{
		Item const* pItem = this->GetPlayer()->GetMixInventory()->GetItem(Slot);

		INIT_SOCKET_DATA(socket_data);
		SOCKET_SLOT_LOOP(i)
		{
			socket_data[i] = pItem->GetSocket(i);
		}

		Item item_create(ConversionItem, pItem->GetLevel(), 0, pItem->GetSkill(), pItem->GetLuck(), pItem->GetOption(), pItem->GetExe(), pItem->GetAncient(), socket_data, pItem->GetSocketBonus());
		item_create.SetDBFlag(this->GetDBFlag());

		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, item_create, 1);

		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
	}

	this->ChaosMixDBSave();
}

void MixHandler::ChaosMixKnightConversionEnergyToStrength(uint8 data)
{
	int32 ConversionOrder = 0;
	int32 ConversionItem = 0;
	int32 ChaosJewel = 0;
	int32 BlessJewel = 0;
	int32 SoulJewel = 0;
	uint8 Slot = -1;

	chaos_box_loop(i)
	{
		Item const* item = this->GetPlayer()->GetMixInventory()->GetItem(i);

		if (!item->IsItem())
		{
			continue;
		}

		if (item->GetItem() == ITEMGET(14, 499))
		{
			ConversionOrder++;
			this->AddDBFlag(item->GetDBFlag());
		}
		else if (item->GetItem() == JEWEL::CHAOS)
		{
			ChaosJewel += item->GetDurability();
		}
		else if (item->GetItem() == JEWEL::BLESS)
		{
			BlessJewel += item->GetDurability();
		}
		else if (item->GetItem() == JEWEL::SOUL)
		{
			SoulJewel += item->GetDurability();
		}
		else if (item->IsAncient())
		{
			if (item->GetItemType() == 7 ||
				item->GetItemType() == 8 ||
				item->GetItemType() == 9 ||
				item->GetItemType() == 10 ||
				item->GetItemType() == 11)
			{
				Slot = i;
				this->AddDBFlag(item->GetDBFlag());

				if (item->GetItemIndex() == 200)
				{
					ConversionItem = ITEMGET(item->GetItemType(), 98);
				}
				else if (item->GetItemIndex() == 201)
				{
					ConversionItem = ITEMGET(item->GetItemType(), 138);
				}
				else if (item->GetItemIndex() == 202)
				{
					ConversionItem = ITEMGET(item->GetItemType(), 150);
				}
				else if (item->GetItemIndex() == 203)
				{
					ConversionItem = ITEMGET(item->GetItemType(), 106);
				}
				else if (item->GetItemIndex() == 204)
				{
					ConversionItem = ITEMGET(item->GetItemType(), 164);
				}
				else if (item->GetItemIndex() == 205)
				{
					ConversionItem = ITEMGET(item->GetItemType(), 189);
				}
			}
		}
	}

	if (ConversionOrder != 1 || ChaosJewel != 1 || BlessJewel < 10 || SoulJewel < 10 || ConversionItem == 0 || Slot == uint8(-1))
	{
		this->ChaosMixLogInvalidMixItems();
		return;
	}

	this->success_rate.set(100);
	this->mix_price = 0;

	if (!this->tributeTaxPrice())
	{
		return;
	}

	this->ChaosMixDebugMessage();

	if (this->roll_success())
	{
		Item const* pItem = this->GetPlayer()->GetMixInventory()->GetItem(Slot);

		INIT_SOCKET_DATA(socket_data);
		SOCKET_SLOT_LOOP(i)
		{
			socket_data[i] = pItem->GetSocket(i);
		}

		Item item_create(ConversionItem, pItem->GetLevel(), 0, pItem->GetSkill(), pItem->GetLuck(), pItem->GetOption(), pItem->GetExe(), pItem->GetAncient(), socket_data, pItem->GetSocketBonus());
		item_create.SetDBFlag(this->GetDBFlag());

		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, item_create, 1);

		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
	}

	this->ChaosMixDBSave();
}

void MixHandler::ChaosMixKnightConversionStrengthToEnergy(uint8 data)
{
	int32 ConversionOrder = 0;
	int32 ConversionItem = 0;
	int32 ChaosJewel = 0;
	int32 BlessJewel = 0;
	int32 SoulJewel = 0;
	uint8 Slot = -1;

	chaos_box_loop(i)
	{
		Item const* item = this->GetPlayer()->GetMixInventory()->GetItem(i);

		if (!item->IsItem())
		{
			continue;
		}

		if (item->GetItem() == ITEMGET(14, 500))
		{
			ConversionOrder++;
			this->AddDBFlag(item->GetDBFlag());
		}
		else if (item->GetItem() == JEWEL::CHAOS)
		{
			ChaosJewel += item->GetDurability();
		}
		else if (item->GetItem() == JEWEL::BLESS)
		{
			BlessJewel += item->GetDurability();
		}
		else if (item->GetItem() == JEWEL::SOUL)
		{
			SoulJewel += item->GetDurability();
		}
		else if (item->IsAncient())
		{
			if (item->GetItemType() == 7 ||
				item->GetItemType() == 8 ||
				item->GetItemType() == 9 ||
				item->GetItemType() == 10 ||
				item->GetItemType() == 11)
			{
				Slot = i;
				this->AddDBFlag(item->GetDBFlag());

				if (item->GetItemIndex() == 98)
				{
					ConversionItem = ITEMGET(item->GetItemType(), 200);
				}
				else if (item->GetItemIndex() == 138)
				{
					ConversionItem = ITEMGET(item->GetItemType(), 201);
				}
				else if (item->GetItemIndex() == 150)
				{
					ConversionItem = ITEMGET(item->GetItemType(), 202);
				}
				else if (item->GetItemIndex() == 106)
				{
					ConversionItem = ITEMGET(item->GetItemType(), 203);
				}
				else if (item->GetItemIndex() == 164)
				{
					ConversionItem = ITEMGET(item->GetItemType(), 204);
				}
				else if (item->GetItemIndex() == 189)
				{
					ConversionItem = ITEMGET(item->GetItemType(), 205);
				}
			}
		}
	}

	if (ConversionOrder != 1 || ChaosJewel != 1 || BlessJewel < 10 || SoulJewel < 10 || ConversionItem == 0 || Slot == uint8(-1))
	{
		this->ChaosMixLogInvalidMixItems();
		return;
	}

	this->success_rate.set(100);
	this->mix_price = 0;

	if (!this->tributeTaxPrice())
	{
		return;
	}

	this->ChaosMixDebugMessage();

	if (this->roll_success())
	{
		Item const* pItem = this->GetPlayer()->GetMixInventory()->GetItem(Slot);

		INIT_SOCKET_DATA(socket_data);
		SOCKET_SLOT_LOOP(i)
		{
			socket_data[i] = pItem->GetSocket(i);
		}

		Item item_create(ConversionItem, pItem->GetLevel(), 0, pItem->GetSkill(), pItem->GetLuck(), pItem->GetOption(), pItem->GetExe(), pItem->GetAncient(), socket_data, pItem->GetSocketBonus());
		item_create.SetDBFlag(this->GetDBFlag());

		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, item_create, 1);

		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
	}

	this->ChaosMixDBSave();
}

void MixHandler::ChaosMixCreateSpellStone(uint8 data)
{
	int32 count = 0;

	int32 SpiritCount[chaos_box_size];
	uint16 SpiritType[chaos_box_size];

	chaos_box_loop(i)
	{
		Item const* item = this->GetPlayer()->GetMixInventory()->GetItem(i);

		if (!item->IsItem())
		{
			continue;
		}

		if (item->GetItem() >= ITEMGET(0, 0) && item->GetItem() < ITEMGET(12, 0))
		{
			if (item->IsAncient())
			{
				SpiritCount[count] = sMixMgr->GetSpiritStoneCount(SPIRIT_STONE_GREATER, item->GetBuyPrice());
				SpiritType[count] = ITEMGET(12, 406);
				count++;
			}
			else if (item->GetExe())
			{
				SpiritCount[count] = sMixMgr->GetSpiritStoneCount(SPIRIT_STONE_MEDIUM, item->GetBuyPrice());
				SpiritType[count] = ITEMGET(12, 405);
				count++;
			}
			else
			{
				SpiritCount[count] = sMixMgr->GetSpiritStoneCount(SPIRIT_STONE_LESSER, item->GetBuyPrice());
				SpiritType[count] = ITEMGET(12, 404);
				count++;
			}
		}
	}

	if (count <= 0)
	{
		this->ChaosMixLogLackingMixItems();
		return;
	}

	if (/*count != this->GetCount() || */count > this->GetMaxCount())
	{
		this->ChaosMixLogInvalidMixItems();
		return;
	}

	for (int32 i = 0; i < count; ++i)
	{
		if (SpiritCount[i] <= 0)
		{
			this->ChaosMixLogInvalidMixItems();
			return;
		}
	}

	this->mix_price = 50000 * count;

	if (!this->tributeTaxPrice())
	{
		return;
	}

	this->GetPlayer()->GetMixInventory()->Clear();

	this->GenerateMassCombinationResult(count, 100, 100);

	for (int32 i = 0; i < count; ++i)
	{
		this->SetSuccess(this->GetMassCombinationList(i) == 1 ? true : false);

		this->ChaosMixDebugMessage();

		if (this->IsSuccess())
		{
			if (SpiritCount[i] > 0)
			{
				sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, Item(SpiritType[i], 0, SpiritCount[i]));
			}

			this->ChaosMixLogSuccess();
		}
		else
		{
			if (count == 1)
			{
				this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
			}

			this->ChaosMixLogFail();
		}

		this->ChaosMixDBSave();
	}

	this->GetPlayer()->GetInterfaceState()->SetState(0);
}

void MixHandler::ChaosMixBindWingsCreateLvl2(uint8 data)
{
	int32 MagicSolution = 0;
	int32 Wings = 0;

	chaos_box_loop(i)
	{
		Item const* item = this->GetPlayer()->GetMixInventory()->GetItem(i);

		if (!item->IsItem())
		{
			continue;
		}

		if (item->GetItem() == ITEMGET(12, 408))
		{
			MagicSolution++;
		}
		else if (item->IsWingLvl1())
		{
			Wings++;
		}
	}

	if (MagicSolution != 1 || Wings != 1)
	{
		this->ChaosMixLogInvalidMixItems();
		return;
	}

	this->success_rate.set(100);
	this->mix_price = 100000;

	if (!this->tributeTaxPrice())
	{
		return;
	}

	this->ChaosMixDebugMessage();

	if (this->roll_success())
	{
		uint16 ConvertedWings = ITEMGET(12, 422);

		switch (this->GetPlayer()->GetClass())
		{
		case Character::DARK_WIZARD:
		{
									   ConvertedWings = ITEMGET(12, 422);
		} break;

		case Character::DARK_KNIGHT:
		{
									   ConvertedWings = ITEMGET(12, 423);
		} break;

		case Character::FAIRY_ELF:
		{
									 ConvertedWings = ITEMGET(12, 424);
		} break;

		case Character::MAGIC_GLADIATOR:
		{
										   ConvertedWings = ITEMGET(12, 425);
		} break;

		case Character::DARK_LORD:
		{
									 ConvertedWings = ITEMGET(12, 426);
		} break;

		case Character::SUMMONER:
		{
									ConvertedWings = ITEMGET(12, 427);
		} break;

		case Character::RAGE_FIGHTER:
		{
										ConvertedWings = ITEMGET(12, 428);
		} break;

		case Character::GROW_LANCER:
		{
									   ConvertedWings = ITEMGET(12, 429);
		} break;

		case Character::RUNE_WIZARD:
		{
									   ConvertedWings = ITEMGET(12, 422);
		} break;

		case Character::SLAYER:
		{
								  ConvertedWings = ITEMGET(12, 423);
		} break;
		}

		uint8 opt_rate = Random<uint8>(100);
		uint8 opt_type = Random<uint8>(10);

		uint8 exe = 0;
		uint8 luck = Random<uint8>(20) == 0;
		uint8 option = (opt_type == 0 && opt_rate < 30) ? 3 :
			(opt_type == 1 && opt_rate < 45) ? 2 :
			(opt_type == 2 && opt_rate < 60) ? 1 :
			0;

		if (Random<uint8>(10) == 0)
			exe = 1 << Random<uint8>(3);

		if (Random<uint8>(2))
			exe |= 0x20;

		Item item_create(ConvertedWings, 0, 0, 0, luck, option, exe);

		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, item_create, 1);

		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
	}

	this->ChaosMixDBSave();
}

void MixHandler::ChaosMixBindWingsCreateLvl3(uint8 data)
{
	int32 MagicSolution = 0;
	int32 Wings = 0;

	chaos_box_loop(i)
	{
		Item const* item = this->GetPlayer()->GetMixInventory()->GetItem(i);

		if (!item->IsItem())
		{
			continue;
		}

		if (item->GetItem() == ITEMGET(12, 410))
		{
			MagicSolution++;
		}
		else if (item->GetLevel() >= 9 && item->GetOption() >= 1 && item->IsWingLvl2())
		{
			Wings++;
		}
	}

	if (MagicSolution != 1 || Wings != 1)
	{
		this->ChaosMixLogInvalidMixItems();
		return;
	}

	this->success_rate.set(100);
	this->mix_price = 25000000;

	if (!this->tributeTaxPrice())
	{
		return;
	}

	this->ChaosMixDebugMessage();

	if (this->roll_success())
	{
		uint16 ConvertedWings = ITEMGET(12, 430);

		switch (this->GetPlayer()->GetClass())
		{
		case Character::DARK_WIZARD:
		{
									   ConvertedWings = ITEMGET(12, 430);
		} break;

		case Character::DARK_KNIGHT:
		{
									   ConvertedWings = ITEMGET(12, 431);
		} break;

		case Character::FAIRY_ELF:
		{
									 ConvertedWings = ITEMGET(12, 432);
		} break;

		case Character::MAGIC_GLADIATOR:
		{
										   ConvertedWings = ITEMGET(12, 433);
		} break;

		case Character::DARK_LORD:
		{
									 ConvertedWings = ITEMGET(12, 434);
		} break;

		case Character::SUMMONER:
		{
									ConvertedWings = ITEMGET(12, 435);
		} break;

		case Character::RAGE_FIGHTER:
		{
										ConvertedWings = ITEMGET(12, 436);
		} break;

		case Character::GROW_LANCER:
		{
									   ConvertedWings = ITEMGET(12, 437);
		} break;

		case Character::RUNE_WIZARD:
		{
									   ConvertedWings = ITEMGET(12, 468);
		} break;

		case Character::SLAYER:
		{
									   ConvertedWings = ITEMGET(12, 473);
		} break;
		}

		uint8 opt_rate = Random<uint8>(100);
		uint8 opt_type = Random<uint8>(10);

		uint8 exe = 0;
		uint8 luck = roll_chance_i(5) ? 1 : 0;
		uint8 option = (opt_type == 0 && opt_rate < 30) ? 3 :
			(opt_type == 1 && opt_rate < 45) ? 2 :
			(opt_type == 2 && opt_rate < 60) ? 1 :
			0;

		if ((RAND % 20) == 0)
			exe = 1 << (RAND % 4);

		exe |= 0x20;

		Item item_create(ConvertedWings, 0, 0, 0, luck, option, exe);

		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, item_create, 1);

		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
	}

	this->ChaosMixDBSave();
}

void MixHandler::ChaosMixBindWingsCreateLvl4(uint8 data)
{
	int32 MagicSolution = 0;
	int32 Wings = 0;
	uint16 ConvertedWings = -1;

	chaos_box_loop(i)
	{
		Item const* item = this->GetPlayer()->GetMixInventory()->GetItem(i);

		if (!item->IsItem())
		{
			continue;
		}

		if (item->GetItem() == ITEMGET(12, 412))
		{
			MagicSolution++;
		}
		else if (item->GetLevel() >= 9 && item->GetOption() >= 4)
		{
			///- DW
			if (item->GetItem() == ITEMGET(12, 37) || item->GetItem() == ITEMGET(12, 430))
			{
				Wings++;
				ConvertedWings = ITEMGET(12, 438);
			}
			///- DK
			else if (item->GetItem() == ITEMGET(12, 36) || item->GetItem() == ITEMGET(12, 431))
			{
				Wings++;
				ConvertedWings = ITEMGET(12, 439);
			}
			///- ELF
			else if (item->GetItem() == ITEMGET(12, 38) || item->GetItem() == ITEMGET(12, 432))
			{
				Wings++;
				ConvertedWings = ITEMGET(12, 440);
			}
			///- MG
			else if (item->GetItem() == ITEMGET(12, 39) || item->GetItem() == ITEMGET(12, 433))
			{
				Wings++;
				ConvertedWings = ITEMGET(12, 441);
			}
			///- DL
			else if (item->GetItem() == ITEMGET(13, 40) || item->GetItem() == ITEMGET(12, 434))
			{
				Wings++;
				ConvertedWings = ITEMGET(12, 442);
			}
			///- SUM
			else if (item->GetItem() == ITEMGET(12, 43) || item->GetItem() == ITEMGET(12, 435))
			{
				Wings++;
				ConvertedWings = ITEMGET(12, 443);
			}
			///- RF
			else if (item->GetItem() == ITEMGET(12, 50) || item->GetItem() == ITEMGET(12, 436))
			{
				Wings++;
				ConvertedWings = ITEMGET(12, 444);
			}
			///- GL
			else if (item->GetItem() == ITEMGET(12, 270) || item->GetItem() == ITEMGET(12, 437))
			{
				Wings++;
				ConvertedWings = ITEMGET(12, 445);
			}
		}
	}

	if (MagicSolution != 1 || Wings != 1 || ConvertedWings == uint16(-1))
	{
		this->ChaosMixLogInvalidMixItems();
		return;
	}

	this->success_rate.set(100);
	this->mix_price = 25000000;

	if (!this->tributeTaxPrice())
	{
		return;
	}

	this->ChaosMixDebugMessage();

	if (this->roll_success())
	{
		uint8 opt_rate = Random<uint8>(100);
		uint8 opt_type = Random<uint8>(3);

		uint8 exe = 0;
		uint8 luck = roll_chance_i(20) ? 1 : 0;
		uint8 option = (opt_type == 0 && opt_rate < 30) ? 3 :
			(opt_type == 1 && opt_rate < 45) ? 2 :
			(opt_type == 2 && opt_rate < 60) ? 1 :
			0;

		if ((RAND % 2) == 0)
			exe = 1 << (RAND % 4);

		exe |= 0x20;

		Item item_create(ConvertedWings, 0, 0, 0, luck, option, exe);

		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, item_create, 1);

		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
	}

	this->ChaosMixDBSave();
}

void MixHandler::ChaosMixGarudaFeather(uint8 data)
{
	int32 jewel_chaos = 0;
	int32 jewel_creation = 0;
	int32 jewel_bless = 0;
	int32 jewel_soul = 0;
	int32 wing_count = 0;

	chaos_box_loop(i)
	{
		Item const& item = this->GetPlayer()->GetMixInventory()->item[i];

		if (!item.IsItem())
		{
			continue;
		}

		if (item.GetItem() == JEWEL::CHAOS)
		{
			jewel_chaos += item.GetDurability();
		}
		else if (item.GetItem() == JEWEL::CREATION)
		{
			jewel_creation += item.GetDurability();
		}
		else if (item.GetItem() == JEWEL::BLESS)
		{
			jewel_bless += item.GetDurability();
		}
		else if (item.GetItem() == JEWEL::SOUL)
		{
			jewel_soul += item.GetDurability();
		}
		else if (item.IsWingLvl3() && item.GetLevel() >= 13 && item.GetOption() >= 1)
		{
			wing_count++;
		}
	}

	if (jewel_chaos < 30 || jewel_creation < 30 || jewel_bless < 30 || jewel_soul < 30 || wing_count < 1)
	{
		this->ChaosMixLogLackingMixItems();
		return;
	}

	if (!this->ChaosMixTalismanOfLuck())
	{
		this->ChaosMixLogTooManyTalismanOfLuck();
		return;
	}

	//this->success_rate.set((price_tmp / 100000) + 1);
	this->success_rate.op_add(this->GetSpiritStoneRate(SPIRIT_STONE_FLAG_GREATER));

	this->success_rate.limit_max(60);

	this->success_rate.op_add(this->GetLuckyCharm());
	this->success_rate.limit_min(0);
	this->success_rate.limit_max(70);

	this->mix_price = 200000 * this->success_rate.get();

	if (!this->tributeTaxPrice())
	{
		return;
	}

	this->ChaosMixDebugMessage();

	if (this->roll_success())
	{
		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, Item(ITEMGET(13, 467)), 1);
		this->ChaosMixLogSuccess();
	}
	else
	{
		this->GetPlayer()->GetInterfaceState()->SetState(0);
		this->ChaosMixGarudaFeatherItemDown();
		this->GetPlayer()->ChaosMixSend(0);
		this->GetPlayer()->ChaosMixResult(CHAOS_MIX_RESULT_FAIL);
		this->ChaosMixLogFail();
	}

	this->ChaosMixDBSave();
}

void MixHandler::ChaosMixFourthWings(uint8 data)
{
	int32 jewel_chaos = 0;
	int32 flame_count = 0;
	int32 feather_count = 0;
	int32 jewel_creation = 0;
	int32 jewel_bless_pack = 0;
	int32 jewel_soul_pack = 0;
	int32 golden_sentence = 0;

	chaos_box_loop(i)
	{
		Item const& item = this->GetPlayer()->GetMixInventory()->item[i];

		if (!item.IsItem())
			continue;

		if (item.GetItem() == JEWEL::CHAOS)
		{
			jewel_chaos += item.GetDurability();
		}
		else if (item.GetItem() == ITEMGET(13, 466))
		{
			flame_count++;
		}
		else if (item.GetItem() == ITEMGET(13, 467))
		{
			feather_count++;
		}
		else if (item.GetItem() == JEWEL::CREATION)
		{
			jewel_creation += item.GetDurability();
		}
		else if (item.GetItem() == JEWEL::BLESS)
		{
			jewel_bless_pack += item.GetDurability();
		}
		else if (item.GetItem() == JEWEL::SOUL)
		{
			jewel_soul_pack += item.GetDurability();
		}
		else if (item.GetItem() == ITEMGET(13, 468))
		{
			golden_sentence += item.GetDurability();
		}
	}

	if (jewel_chaos < 30 || flame_count < 1 || feather_count < 1 || jewel_creation < 30 || jewel_bless_pack < 30 || jewel_soul_pack < 30 || golden_sentence < 50)
	{
		this->ChaosMixLogLackingMixItems();
		return;
	}

	if (!this->ChaosMixTalismanOfLuck())
	{
		this->ChaosMixLogTooManyTalismanOfLuck();
		return;
	}

	this->success_rate.op_add(this->GetSpiritStoneRate(SPIRIT_STONE_FLAG_GREATER));
	this->success_rate.limit_min(1);
	this->success_rate.limit_max(40);

	this->success_rate.op_add(this->GetLuckyCharm());
	this->success_rate.limit_max(50);

	this->mix_price = this->success_rate.get() * 200000;

	if (!this->tributeTaxPrice())
		return;

	this->ChaosMixDebugMessage();

	if (this->roll_success())
	{
		uint8 opt_rate = Random<uint8>(100);
		uint8 opt_type = Random<uint8>(3);

		uint8 exe = 0;
		uint8 luck = roll_chance_i(30) ? 1 : 0;
		uint8 option = (opt_type == 0 && opt_rate < 30) ? 3 :
			(opt_type == 1 && opt_rate < 45) ? 2 :
			(opt_type == 2 && opt_rate < 60) ? 1 :
			0;
		
		if (Random(3) == 0)
		{
			exe = 0x10;
		}
		else if (Random(3) == 0)
		{
			exe = 0x20;
		}

		int32 socket_rate = Random(100);
		int32 random_socket = 1;
		
		if (socket_rate < 60)
		{
			random_socket = 2;
		}

		if (socket_rate < 20)
		{
			random_socket = 3;
		}

		if (socket_rate < 5)
		{
			random_socket = 4;
		}

		INIT_SOCKET_DATA(socket);
		for (int32 i = 0; i < random_socket; ++i)
		{
			socket[i] = sItemMgr->GenerateWingGradedOption(socket);
		}

		RandomValue<uint16> m_RandomValue(ITEMGET(12, 414));
		m_RandomValue.AddValue(ITEMGET(12, 414), 0);
		m_RandomValue.AddValue(ITEMGET(12, 415), 0);
		m_RandomValue.AddValue(ITEMGET(12, 416), 0);
		m_RandomValue.AddValue(ITEMGET(12, 417), 0);
		m_RandomValue.AddValue(ITEMGET(12, 418), 0);
		m_RandomValue.AddValue(ITEMGET(12, 419), 0);
		m_RandomValue.AddValue(ITEMGET(12, 420), 0);
		m_RandomValue.AddValue(ITEMGET(12, 421), 0);
		m_RandomValue.AddValue(ITEMGET(12, 469), 0);
		m_RandomValue.AddValue(ITEMGET(12, 474), 0);

		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, Item(m_RandomValue.GetRandomValue(RANDOM_POOL_RANDOM), 0, 0, 0, luck, option, exe, 0, socket), 1);
		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
	}

	this->ChaosMixDBSave();
}

void MixHandler::ChaosMixHandcraftStones(uint8 data)
{
	int32 earring_amount = 0;
	int32 ring_amount = 0;
	int32 pendant_amount = 0;
	int32 jewel_amount = 0;
	bool jewel_type[6];
	for (int32 i = 0; i < 6; ++i)
	{
		jewel_type[i] = false;
	}
	
	chaos_box_loop(i)
	{
		Item const& item = this->GetPlayer()->GetMixInventory()->item[i];

		if (!item.IsItem())
			continue;

		if (item.IsEarring())
		{
			earring_amount++;
		}
		else if (item.GetExe())
		{
			if (item.GetKind2() == ItemKind::RING)
			{
				ring_amount++;
			}
			else if (item.GetKind2() == ItemKind::PHYSICAL_PENDANT || item.GetKind2() == ItemKind::WIZARD_PENDANT)
			{
				pendant_amount++;
			}
		}
		else if (item.GetItem() == JEWEL::SOUL && !jewel_type[0])
		{
			jewel_type[0] = true;
			jewel_amount += item.GetDurability();
		}
		else if (item.GetItem() == JEWEL::LIFE && !jewel_type[1])
		{
			jewel_type[1] = true;
			jewel_amount += item.GetDurability();
		}
		else if (item.GetItem() == JEWEL::CREATION && !jewel_type[2])
		{
			jewel_type[2] = true;
			jewel_amount += item.GetDurability();
		}
		else if (item.GetItem() == JEWEL::GUARDIAN && !jewel_type[3])
		{
			jewel_type[3] = true;
			jewel_amount += item.GetDurability();
		}
		else if (item.GetItem() == JEWEL::HARMONY && !jewel_type[4])
		{
			jewel_type[4] = true;
			jewel_amount += item.GetDurability();
		}
		else if (item.GetItem() == JEWEL::CHAOS && !jewel_type[5])
		{
			jewel_type[5] = true;
			jewel_amount += item.GetDurability();
		}

	}

	if (earring_amount <= 0 && ring_amount <= 0 && pendant_amount <= 0 && jewel_amount != 60)
	{
		this->ChaosMixLogLackingMixItems();
		return;
	}

	if ((earring_amount % 2) != 0 || (ring_amount % 2) != 0 || (pendant_amount % 2) != 0)
	{
		this->ChaosMixLogInvalidMixItems();
		return;
	}

	int32 count = (earring_amount / 2) + (ring_amount / 2) + (pendant_amount / 2) + (jewel_amount / 60);

	if (count != this->GetCount() || count > this->GetMaxCount())
	{
		this->ChaosMixLogInvalidMixItems();
		return;
	}

	this->mix_price = 0;

	if (!this->tributeTaxPrice())
	{
		return;
	}

	int32 amount[32];
	int32 amount_loop = 0;
	for (int32 i = 0; i < (earring_amount / 2); ++i)
	{
		amount[amount_loop++] = 4;
	}

	for (int32 i = 0; i < (pendant_amount / 2); ++i)
	{
		amount[amount_loop++] = 3;
	}

	for (int32 i = 0; i < (ring_amount / 2); ++i)
	{
		amount[amount_loop++] = 2;
	}

	for (int32 i = 0; i < (jewel_amount / 6); ++i)
	{
		amount[amount_loop++] = 1;
	}

	this->GetPlayer()->GetMixInventory()->Clear();

	this->GenerateMassCombinationResult(count, 100, 100);
	
	for (int32 i = 0; i < count; ++i)
	{
		this->SetSuccess(this->GetMassCombinationList(i) == 1 ? true : false);

		this->ChaosMixDebugMessage();

		if (this->IsSuccess())
		{
			sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, Item(ITEMGET(13, 510), 0, amount[i]));

			this->ChaosMixLogSuccess();
		}
		else
		{
			if (count == 1)
			{
				this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
			}

			this->ChaosMixLogFail();
		}

		this->ChaosMixDBSave();
	}

	this->GetPlayer()->GetInterfaceState()->SetState(0);
}

void MixHandler::ChaosMixUpgradeEarringRageToAncestors(uint8 data)
{
	int32 jewel_chaos = 0;
	int32 framework = 0;
	int32 jewel_creation = 0;
	int32 jewel_bless_pack = 0;
	int32 jewel_soul_pack = 0;
	int32 earring_level = 0;
	uint16 earring_item = -1;
	uint8 earring_option_count = 0;

	chaos_box_loop(i)
	{
		Item const& item = this->GetPlayer()->GetMixInventory()->item[i];

		if (!item.IsItem())
			continue;

		if (item.GetItem() == JEWEL::CHAOS)
		{
			jewel_chaos += item.GetDurability();
		}
		else if (item.GetItem() == ITEMGET(14, 458))
		{
			framework++;
		}
		else if (item.GetItem() == JEWEL::CREATION)
		{
			jewel_creation += item.GetDurability();
		}
		else if (item.GetItem() == JEWEL::BLESS)
		{
			jewel_bless_pack += item.GetDurability();
		}
		else if (item.GetItem() == JEWEL::SOUL)
		{
			jewel_soul_pack += item.GetDurability();
		}
		else if (item.IsEarring())
		{
			earring_level = item.GetLevel();
			earring_option_count = item.GetActiveSocketCount();

			switch (item.GetItem())
			{
			case ITEMGET(12, 449):
			case ITEMGET(12, 457):
			{
									 earring_item = item.GetItem() + 1;
			} break;
			}
		}
	}

	if (jewel_chaos < 1 || framework < 1 || jewel_creation < 1 || jewel_bless_pack < 20 || jewel_soul_pack < 20 || earring_level < 11 || earring_item == uint16(-1))
	{
		this->ChaosMixLogLackingMixItems();
		return;
	}

	this->success_rate.set(60);

	switch (earring_level)
	{
	case 11:
	{
			   this->success_rate.set(60);
	} break;

	case 12:
	{
			   this->success_rate.set(70);
	} break;

	case 13:
	{
			   this->success_rate.set(80);
	} break;

	case 14:
	{
			   this->success_rate.set(90);
	} break;

	case 15:
	{
			   this->success_rate.set(100);
	} break;
	}

	//if (!this->tributeTaxPrice())
	//	return;

	this->ChaosMixDebugMessage();

	if (this->roll_success())
	{
		Item item(earring_item);

		sItemMgr->GenerateEarringOptions(&item, earring_option_count >= 3 ? 2 : 1);

		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, item, 1);
		this->ChaosMixLogSuccess();
	}
	else
	{
		this->GetPlayer()->GetInterfaceState()->SetState(0);
		this->ChaosMixEarringItemDown();
		this->GetPlayer()->ChaosMixSend(0);
		this->GetPlayer()->ChaosMixResult(CHAOS_MIX_RESULT_FAIL);
		this->ChaosMixLogFail();
	}

	this->ChaosMixDBSave();
}

void MixHandler::ChaosMixUpgradeEarringAncestorsToHonor(uint8 data)
{
	int32 jewel_chaos = 0;
	int32 framework = 0;
	int32 jewel_creation = 0;
	int32 jewel_bless_pack = 0;
	int32 jewel_soul_pack = 0;
	int32 earring_level = 0;
	uint16 earring_item = -1;
	uint8 earring_option_count = 0;

	chaos_box_loop(i)
	{
		Item const& item = this->GetPlayer()->GetMixInventory()->item[i];

		if (!item.IsItem())
			continue;

		if (item.GetItem() == JEWEL::CHAOS)
		{
			jewel_chaos += item.GetDurability();
		}
		else if (item.GetItem() == ITEMGET(14, 497))
		{
			framework++;
		}
		else if (item.GetItem() == JEWEL::CREATION)
		{
			jewel_creation += item.GetDurability();
		}
		else if (item.GetItem() == JEWEL::BLESS)
		{
			jewel_bless_pack += item.GetDurability();
		}
		else if (item.GetItem() == JEWEL::SOUL)
		{
			jewel_soul_pack += item.GetDurability();
		}
		else if (item.IsEarring())
		{
			earring_level = item.GetLevel();
			earring_option_count = item.GetActiveSocketCount();

			switch (item.GetItem())
			{
			case ITEMGET(12, 450):
			case ITEMGET(12, 458):
			{
									 earring_item = item.GetItem() + 1;
			} break;
			}
		}
	}

	if (jewel_chaos < 1 || framework < 1 || jewel_creation < 1 || jewel_bless_pack < 20 || jewel_soul_pack < 20 || earring_level < 11 || earring_item == uint16(-1))
	{
		this->ChaosMixLogLackingMixItems();
		return;
	}

	this->success_rate.set(60);

	switch (earring_level)
	{
	case 11:
	{
			   this->success_rate.set(60);
	} break;

	case 12:
	{
			   this->success_rate.set(70);
	} break;

	case 13:
	{
			   this->success_rate.set(80);
	} break;

	case 14:
	{
			   this->success_rate.set(90);
	} break;

	case 15:
	{
			   this->success_rate.set(100);
	} break;
	}

	//if (!this->tributeTaxPrice())
	//	return;

	this->ChaosMixDebugMessage();

	if (this->roll_success())
	{
		Item item(earring_item);

		sItemMgr->GenerateEarringOptions(&item, earring_option_count >= 3 ? 2 : 1);

		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, item, 1);
		this->ChaosMixLogSuccess();
	}
	else
	{
		this->GetPlayer()->GetInterfaceState()->SetState(0);
		this->ChaosMixEarringItemDown();
		this->GetPlayer()->ChaosMixSend(0);
		this->GetPlayer()->ChaosMixResult(CHAOS_MIX_RESULT_FAIL);
		this->ChaosMixLogFail();
	}

	this->ChaosMixDBSave();
}

void MixHandler::ChaosMixSealArmorPiece(uint8 data)
{
	int32 jewel_chaos = 0;
	int32 jewel_creation = 0;
	int32 jewel_bless = 0;
	int32 jewel_soul = 0;
	int32 seal_piece = 0;

	chaos_box_loop(i)
	{
		Item const& item = this->GetPlayer()->GetMixInventory()->item[i];

		if (!item.IsItem())
			continue;

		if (item.GetItem() == JEWEL::CHAOS)
		{
			jewel_chaos += item.GetDurability();
		}
		else if (item.GetItem() == JEWEL::CREATION)
		{
			jewel_creation += item.GetDurability();
		}
		else if (item.GetItem() == JEWEL::BLESS)
		{
			jewel_bless += item.GetDurability();
		}
		else if (item.GetItem() == JEWEL::SOUL)
		{
			jewel_soul += item.GetDurability();
		}
		else if (item.GetItem() == ITEMGET(14, 471))
		{
			seal_piece += item.GetDurability();
		}
	}

	if (jewel_chaos < 1 || jewel_creation < 1 || jewel_bless < 5 || jewel_soul < 5 || seal_piece < 50)
	{
		this->ChaosMixLogLackingMixItems();
		return;
	}

	this->success_rate.set(100);
	this->mix_price = 100000;

	if (!this->tributeTaxPrice())
		return;

	this->ChaosMixDebugMessage();

	if (this->roll_success())
	{
		RandomValue<uint16> m_RandomValue(ITEMGET(14, 472));
		m_RandomValue.AddValue(ITEMGET(14, 472), 0);
		m_RandomValue.AddValue(ITEMGET(14, 473), 0);
		m_RandomValue.AddValue(ITEMGET(14, 474), 0);
		m_RandomValue.AddValue(ITEMGET(14, 475), 0);
		m_RandomValue.AddValue(ITEMGET(14, 476), 0);

		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, Item(m_RandomValue.GetRandomValue(RANDOM_POOL_RANDOM)), 1);
		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
	}

	this->ChaosMixDBSave();
}

void MixHandler::ChaosMixScratchedSealedBloodangel(uint8 data)
{
	int32 jewel_chaos = 0;
	int32 jewel_creation = 0;
	int32 jewel_bless = 0;
	int32 jewel_soul = 0;
	uint8 type = -1;
	
	chaos_box_loop(i)
	{
		Item const& item = this->GetPlayer()->GetMixInventory()->item[i];

		if (!item.IsItem())
			continue;

		if (item.GetItem() == JEWEL::CHAOS)
		{
			jewel_chaos += item.GetDurability();
		}
		else if (item.GetItem() == JEWEL::CREATION)
		{
			jewel_creation += item.GetDurability();
		}
		else if (item.GetItem() == JEWEL::BLESS)
		{
			jewel_bless += item.GetDurability();
		}
		else if (item.GetItem() == JEWEL::SOUL)
		{
			jewel_soul += item.GetDurability();
		}
		else if (item.GetItem() >= ITEMGET(14, 472) && item.GetItem() <= ITEMGET(14, 476))
		{
			type = (item.GetItem() - ITEMGET(14, 472)) + 7;
		}
	}

	if (jewel_chaos < 1 || jewel_creation < 1 || jewel_bless < 10 || jewel_soul < 10 || type == (uint8)-1)
	{
		this->ChaosMixLogLackingMixItems();
		return;
	}

	this->success_rate.set(40);
	this->mix_price = 100000;

	if (!this->tributeTaxPrice())
		return;

	this->ChaosMixDebugMessage();

	if (this->roll_success())
	{
		uint16 item = -1;
		uint8 ancient = 0;

		while (ancient == 0)
		{
			RandomValue<uint16> m_RandomValue(ITEMGET(type, 98));
			m_RandomValue.AddValue(ITEMGET(type, 98), 0);
			m_RandomValue.AddValue(ITEMGET(type, 99), 0);
			m_RandomValue.AddValue(ITEMGET(type, 100), 0);
			m_RandomValue.AddValue(ITEMGET(type, 101), 0);

			if (type != 7)
			{
				m_RandomValue.AddValue(ITEMGET(type, 102), 0);
			}

			m_RandomValue.AddValue(ITEMGET(type, 103), 0);

			if (type != 10)
			{
				m_RandomValue.AddValue(ITEMGET(type, 104), 0);
			}

			m_RandomValue.AddValue(ITEMGET(type, 105), 0);
			m_RandomValue.AddValue(ITEMGET(type, 122), 0);
			m_RandomValue.AddValue(ITEMGET(type, 158), 0);

			if (type != 7)
			{
				m_RandomValue.AddValue(ITEMGET(type, 161), 0);
			}

			m_RandomValue.AddValue(ITEMGET(type, 180), 0);

			item = m_RandomValue.GetRandomValue(RANDOM_POOL_RANDOM);
			ancient = sItemMgr->GetRandomOptionAncient(item);
		}

		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_chaos_box, Item(item, 0, 0, 0, 0, 0, 0, ancient), 1);
		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL);
	}

	this->ChaosMixDBSave();
}

void MixHandler::PentagramMix(uint8 id, uint8 * slot, uint8 data)
{
	if ( sMixMgr->IsMixDisabled(1, id) )
	{
		this->ChaosMixLogInvalidMix(252);
		return;
	}

	if ( id >= CHAOS_MIX_PENTAGRAM_MAX )
	{
		this->ChaosMixLogInvalidMix(252);
		return;
	}

	PentagramMixHandler const& mixhandle = PentagramMixTable[id];

	if ( mixhandle.type != this->GetPlayer()->GetInterfaceState()->GetID() )
	{
		this->GetPlayer()->SendNotice(CUSTOM_MESSAGE_ID_RED, "Wrong Mix Interface. Contact Administrators");
		sLog->outError(LOG_CHAOS_MIX, "Wrong Mix Interface. %s - Mix: %d [%s], Mix Interface: %s, Current Interface: %s",
			this->GetPlayer()->BuildLog().c_str(), id, mixhandle.name, mixhandle.type.c_str(), this->GetPlayer()->GetInterfaceState()->GetID().c_str());

		this->ChaosMixLogInvalidMix(252);
		return;
	}

	if ( this->GetPlayer()->GetInterfaceState()->GetState() == 1 )
	{
		return;
	}

	this->GetPlayer()->GetInterfaceState()->SetState(1);
	this->SetMixName(mixhandle.name);
	this->SetMixID(id);
	this->SetSuccess(false);
	this->GetPlayer()->SetMixCompleted(false);
	this->SetDBFlag(0);
	this->ResetItemDataBefore();
	this->ResetItemDataAfter();

	for ( int32 i = 0; i < PENTAGRAM_MIX_MAX_SLOT; ++i )
	{
		if ( !use_inventory_range(slot[i]) )
		{
			continue;
		}

		Item * item = this->GetPlayer()->GetInventory()->GetItem(slot[i]);

		if ( !item->IsItem() )
		{
			continue;
		}

		this->ChaosMixLogItem(item, slot[i]);
	
		if (this->GetItemDataBefore().empty())
		{
			this->SetItemDataBefore("[");
		}
		else
		{
			this->SetItemDataBefore(this->GetItemDataBefore() + ",");
		}
		
		this->SetItemDataBefore(this->GetItemDataBefore() + item->BuildLogDB(slot[i], false));
	}

	if (!this->GetItemDataBefore().empty())
	{
		this->SetItemDataBefore(this->GetItemDataBefore() + "]");
	}

	(this->*mixhandle.handler)(slot, data);
}

void MixHandler::PentagramMixErrtel(uint8 id, uint8 * data, uint8 * slot)
{
	if ( sMixMgr->IsMixDisabled(2, id) )
	{
		this->ChaosMixLogInvalidMix(252);
		return;
	}

	if ( id >= CHAOS_MIX_PENTAGRAM_JEWEL_UPGRADE_MAX )
	{
		this->ChaosMixLogInvalidMix(252);
		return;
	}

	PentagramJewelMixHandler const& mixhandle = PentagramUpgradeMixTable[id];

	if ( mixhandle.type != this->GetPlayer()->GetInterfaceState()->GetID() )
	{
		this->GetPlayer()->SendNotice(CUSTOM_MESSAGE_ID_RED, "Wrong Mix Interface. Contact Administrators");
		sLog->outError(LOG_CHAOS_MIX, "Wrong Mix Interface. %s - Mix: %d [%s], Mix Interface: %s, Current Interface: %s",
			this->GetPlayer()->BuildLog().c_str(), id, mixhandle.name, mixhandle.type.c_str(), this->GetPlayer()->GetInterfaceState()->GetID().c_str());

		this->ChaosMixLogInvalidMix(252);
		return;
	}

	if ( this->GetPlayer()->GetInterfaceState()->GetState() == 1 )
	{
		return;
	}

	this->GetPlayer()->GetInterfaceState()->SetState(1);
	this->SetMixName(mixhandle.name);
	this->SetMixID(id);
	this->SetSuccess(false);
	this->GetPlayer()->SetMixCompleted(false);
	this->SetDBFlag(0);
	this->ResetItemDataBefore();
	this->ResetItemDataAfter();

	for ( int32 i = 0; i < PENTAGRAM_MIX_UPGRADE_MAX_SLOT; ++i )
	{
		if ( !use_inventory_range(slot[i]) )
		{
			continue;
		}

		Item * item = this->GetPlayer()->GetInventory()->GetItem(slot[i]);

		if ( !item->IsItem() )
		{
			continue;
		}

		this->ChaosMixLogItem(item, slot[i]);

		if (this->GetItemDataBefore().empty())
		{
			this->SetItemDataBefore("[");
		}
		else
		{
			this->SetItemDataBefore(this->GetItemDataBefore() + ",");
		}
		
		this->SetItemDataBefore(this->GetItemDataBefore() + item->BuildLogDB(slot[i], false));
	}

	if (!this->GetItemDataBefore().empty())
	{
		this->SetItemDataBefore(this->GetItemDataBefore() + "]");
	}

	(this->*mixhandle.handler)(slot, data);
}

void MixHandler::PentagramMixNone(uint8 * slot, uint8 data)
{
	this->ChaosMixLogInvalidMix(252);
}
	
void MixHandler::PentagramJewelMix(uint8 * slot, uint8 data)
{
	this->SetJewelItem(0, JEWEL::BLESS);
	this->SetMaxJewelCount(0, 1);

	int32 MithrilCount = 0;
	int32 MithrilType = 0;

	for ( int32 i = 0; i < PENTAGRAM_MIX_MAX_SLOT; ++i )
	{
		if ( !use_inventory_range(slot[i]) )
		{
			continue;
		}

		Item * item = this->GetPlayer()->GetInventory()->GetItem(slot[i]);

		if ( !item )
		{
			continue;
		}

		if ( !item->IsItem() )
		{
			continue;
		}

		if ( item->GetItem() == ITEMGET(12, 145) )
		{
			MithrilCount++;
			MithrilType = item->GetSocketBonus();
		}
	}

	if (!this->HaveJewels())
	{
		this->ChaosMixLogInvalidMix(251);
		return;
	}

	if (MithrilCount == 0)
	{
		this->ChaosMixLogInvalidMix(251);
		return;
	}

	MithrilCount = 1;

	if( !CHECK_RANGE((MithrilType-1), MAX_PENTAGRAM_ELEMENTAL_ATTRIBUTE) )
	{
		this->ChaosMixLogInvalidMix(251);
		return;
	}

	this->success_rate.set(50);
	this->mix_price = 100000;

	if ( !this->tributeTaxPrice(true, true) )
	{
		return;
	}

	this->ChaosMixDebugMessage();

	for ( int32 i = 0; i < PENTAGRAM_MIX_MAX_SLOT; ++i )
	{
		if ( !use_inventory_range(slot[i]) )
		{
			continue;
		}

		Item * item = this->GetPlayer()->GetInventory()->GetItem(slot[i]);

		if ( !item )
		{
			continue;
		}

		if ( !item->IsItem() )
		{
			continue;
		}

		if (this->IsJewel(slot[i]))
		{
			continue;
		}

		if ( item->GetItem() == ITEMGET(12, 145) && MithrilCount > 0 )
		{
			MithrilCount--;
		}

		this->GetPlayer()->ClearItem(slot[i]);
	}

	this->JewelConsume(slot, PENTAGRAM_MIX_MAX_SLOT);

	if ( this->roll_success() )
	{
		uint16 ItemIndex = 0;
		INIT_SOCKET_DATA(SocketOption);
		uint8 SocketOptionBonus = MithrilType | 16;
		RandomValue<uint16> m_RandomValue(ITEMGET(12, 221));
		m_RandomValue.AddValue(ITEMGET(12, 221), 4000);
		m_RandomValue.AddValue(ITEMGET(12, 231), 3000);
		m_RandomValue.AddValue(ITEMGET(12, 241), 1500);
		m_RandomValue.AddValue(ITEMGET(12, 251), 1000);
		m_RandomValue.AddValue(ITEMGET(12, 261), 500);
		ItemIndex = m_RandomValue.GetRandomValue(RANDOM_POOL_WEIGHT);

		SocketOption[0] = sPentagramSystem->GetRandomErrtelOption(ItemIndex, 1);
		
		if( SocketOption[0] == SOCKET_SLOT_NONE )
		{
			this->GetPlayer()->SetMixCompleted(false);
			this->ChaosMixLogInvalidMix(251);
			return;
		}

		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_pentagram_mix, Item(ItemIndex, 0, 0, 0, 0, 0, 0, 0, SocketOption, SocketOptionBonus));

		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixFailInit(0, false, 0, 1);
	}

	this->ChaosMixDBSave();
}

void MixHandler::PentagramSlotOfRadiance(uint8 * slot, uint8 data)
{
	int32 FragmentOfRadianceSlot1 = 0;
	int32 FragmentOfRadianceSlot2 = 0;
	int32 FragmentOfRadianceSlot3 = 0;
	int32 FragmentOfRadianceSlot4 = 0;

	for ( int32 i = 0; i < PENTAGRAM_MIX_MAX_SLOT; ++i )
	{
		if ( !use_inventory_range(slot[i]) )
		{
			continue;
		}

		Item * item = this->GetPlayer()->GetInventory()->GetItem(slot[i]);

		if ( !item )
		{
			continue;
		}

		if ( !item->IsItem() )
		{
			continue;
		}

		if ( item->GetItem() == ITEMGET(14, 309) )
		{
			FragmentOfRadianceSlot1++;
		}
		else if ( item->GetItem() == ITEMGET(14, 310) )
		{
			FragmentOfRadianceSlot2++;
		}
		else if ( item->GetItem() == ITEMGET(14, 311) )
		{
			FragmentOfRadianceSlot3++;
		}
		else if ( item->GetItem() == ITEMGET(14, 312) )
		{
			FragmentOfRadianceSlot4++;
		}
	}
	
	if( FragmentOfRadianceSlot1 != 1 || FragmentOfRadianceSlot2 != 1 || FragmentOfRadianceSlot3 != 1 || FragmentOfRadianceSlot4 != 1 )
	{
		this->ChaosMixLogInvalidMix(251);
		return;
	}

	this->success_rate.set(100);
	this->mix_price = 100000000;

	if ( !this->tributeTaxPrice(true) )
	{
		return;
	}

	this->ChaosMixDebugMessage();

	for ( int32 i = 0; i < PENTAGRAM_MIX_MAX_SLOT; ++i )
	{
		if ( !use_inventory_range(slot[i]) )
		{
			continue;
		}

		Item * item = this->GetPlayer()->GetInventory()->GetItem(slot[i]);

		if ( !item )
		{
			continue;
		}

		if ( !item->IsItem() )
		{
			continue;
		}

		this->GetPlayer()->ClearItem(slot[i]);
	}

	if ( this->roll_success() )
	{
		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_pentagram_mix, Item(ITEMGET(14, 316)));

		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixFailInit(0, false, 0, 1);
	}

	this->ChaosMixDBSave();
}

void MixHandler::ErrtelElementChange(uint8 * slot, uint8 data)
{
	uint8 element = data;

	if ( element >= ELEMENTAL_ATTRIBUTE_MAX )
	{
		this->ChaosMixLogInvalidMix(251);
		return;
	}

	if ( element <= 0 )
	{
		this->ChaosMixLogInvalidMix(251);
		return;
	}

	int32 TalismanElementChange = 0;
	int32 Errtel = 0;
	uint8 errtel_slot = -1;
	uint8 errtel_rank = 0;

	for ( int32 i = 0; i < PENTAGRAM_MIX_MAX_SLOT; ++i )
	{
		if ( !use_inventory_range(slot[i]) )
		{
			continue;
		}

		Item * item = this->GetPlayer()->GetInventory()->GetItem(slot[i]);

		if ( !item )
		{
			continue;
		}

		if ( !item->IsItem() )
		{
			continue;
		}

		if ( item->GetItem() == ITEMGET(14, 317) )
		{
			TalismanElementChange++;
		}
		else if (item->GetKind2() == ItemKind::ERRTEL && (item->GetSocketBonus() % 16) != element)
		{
			///- No tiene que ser Radiance
			if (item->GetItem() != ITEMGET(12, 261))
			{
				Errtel++;
				errtel_slot = slot[i];
				errtel_rank = item->GetSocketBonus() / 16;
			}
		}
		else if ( item->IsPentagramItem() && (item->GetSocketBonus() % 16) != element )
		{
			///- No tiene que tener Errtels
			if ( item->GetActiveSocketCount() <= 0 )
			{
				Errtel++;
				errtel_slot = slot[i];
				errtel_rank = item->GetSocketBonus() / 16;
			}
		}
	}
	
	if( TalismanElementChange != 1 || Errtel != 1 )
	{
		this->ChaosMixLogInvalidMix(251);
		return;
	}

	this->success_rate.set(100);
	this->mix_price = 0;

	if ( !this->tributeTaxPrice(true) )
	{
		return;
	}

	this->ChaosMixDebugMessage();

	for ( int32 i = 0; i < PENTAGRAM_MIX_MAX_SLOT; ++i )
	{
		if ( !use_inventory_range(slot[i]) )
		{
			continue;
		}

		Item * item = this->GetPlayer()->GetInventory()->GetItem(slot[i]);

		if ( !item )
		{
			continue;
		}

		if ( !item->IsItem() )
		{
			continue;
		}

		if ( slot[i] == errtel_slot )
		{
			continue;
		}

		this->GetPlayer()->ClearItem(slot[i]);
	}

	if ( this->roll_success() )
	{
		this->GetPlayer()->GetInventory()->GetItem(errtel_slot)->SetSocketBonus((errtel_rank * 16) | element);
		this->GetPlayer()->GetInventory()->GetItem(errtel_slot)->Convert(true);

		this->GetPlayer()->SendInventory(errtel_slot);
		
		this->GetPlayer()->GetInterfaceState()->SetState(0);
		this->GetPlayer()->ChaosMixPentagramResult(1, 0);
		
		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixFailInit(0, false, 0, 1);
	}

	this->ChaosMixDBSave();
}

void MixHandler::PentagramJewelMixNone(uint8 * slot, uint8 * data)
{
	this->ChaosMixLogInvalidMix(252);
}

void MixHandler::PentagramJewelUpgradeLevelMix(uint8 * slot, uint8 * data)
{
	int32 ElementalRune = 0;
	int32 PentagramJewelCount = 0;
	int32 PentagramJewelSlot = -1;
	int32 ElementalChaosAmulet = 0;
	int32 ElementalLuckAmulet = 0;

	uint8 level = data[0] - 1;
	uint8 option_slot = data[1] - 1;
		
	for ( int32 i = 0; i < PENTAGRAM_MIX_UPGRADE_MAX_SLOT; ++i )
	{
		if ( !use_inventory_range(slot[i]) )
		{
			continue;
		}

		Item * item = this->GetPlayer()->GetInventory()->GetItem(slot[i]);

		if ( !item )
		{
			continue;
		}

		if ( !item->IsItem() )
		{
			continue;
		}

		if( item->GetItem() == ITEMGET(12, 288) )
		{
			ElementalRune += item->GetDurability();
		}
		else if( item->GetItem() == ITEMGET(14, 190) )
		{
			ElementalChaosAmulet++;
		}
		else if ( item->GetItem() == ITEMGET(14, 189) )
		{
			ElementalLuckAmulet++;
		}
		else if (item->GetKind2() == ItemKind::ERRTEL)
		{
			PentagramJewelCount++;
			PentagramJewelSlot = slot[i];
		}
	}

	if( PentagramJewelCount != 1 || PentagramJewelSlot == -1 )
	{
		this->ChaosMixLogInvalidMix(251);
		return;
	}

	if ( ElementalChaosAmulet != 0 && ElementalLuckAmulet != 0 )
	{
		this->ChaosMixLogInvalidMix(251);
		return;
	}

	if ( !CHECK_RANGE(option_slot, MAX_SOCKET_SLOT) )
	{
		this->ChaosMixLogInvalidMix(251);
		return;
	}

	if ( !CHECK_RANGE(level, MAX_PENTAGRAM_JEWEL_LEVEL) )
	{
		this->ChaosMixLogInvalidMix(251);
		return;
	}

	Item * pPentagramJewel = this->GetPlayer()->GetInventory()->GetItem(PentagramJewelSlot);

	if ( (pPentagramJewel->GetSocket(option_slot) / 16) != level )
	{
		this->ChaosMixLogInvalidMix(251);
		return;
	}
	
	PentagramJewelUpgradeLevelInfo const* pPentagramJewelUpgradeLevelInfo = sPentagramSystem->GetPentagramJewelUpgradeLevelInfo(level);

	if( !pPentagramJewelUpgradeLevelInfo )
	{
		this->ChaosMixLogInvalidMix(251);
		return;
	}

	if ( ElementalRune < pPentagramJewelUpgradeLevelInfo->GetRequiredElementalRune(option_slot) )
	{
		this->ChaosMixLogInvalidMix(251);
		return;
	}

	ElementalRune = pPentagramJewelUpgradeLevelInfo->GetRequiredElementalRune(option_slot);

	this->success_rate.set(pPentagramJewelUpgradeLevelInfo->GetMixRate(option_slot));

	if ( ElementalLuckAmulet )
	{
		this->success_rate.op_add(10);
	}

	this->success_rate.limit_max(100);

	this->mix_price = pPentagramJewelUpgradeLevelInfo->GetMixMoney();

	if ( !this->tributeTaxPrice(true) )
	{
		return;
	}

	this->ChaosMixDebugMessage();

	for ( int32 i = 0; i < PENTAGRAM_MIX_UPGRADE_MAX_SLOT; ++i )
	{
		if ( !use_inventory_range(slot[i]) )
		{
			continue;
		}

		if ( slot[i] == PentagramJewelSlot )
		{
			continue;
		}

		Item * item = this->GetPlayer()->GetInventory()->GetItem(slot[i]);

		if ( !item )
		{
			continue;
		}

		if ( !item->IsItem() )
		{
			continue;
		}

		if( item->GetItem() == ITEMGET(12, 288) )
		{
			if ( ElementalRune > 0 )
			{
				if ( item->GetDurability() > ElementalRune )
				{
					item->AlterDurability(0, ElementalRune);
					this->GetPlayer()->SendItemDurability(slot[i], 1);
					ElementalRune = 0;
				}
				else
				{
					ElementalRune -= item->GetDurability();
					this->GetPlayer()->ClearItem(slot[i]);
				}
			}
		}
		else
		{
			this->GetPlayer()->ClearItem(slot[i]);
		}
	}

	if (!ElementalChaosAmulet)
	{
		pPentagramJewel->common.talisman += 1;
	}

	if ( this->roll_success() )
	{
		//pPentagramJewel->IncreaseLevel(1);
		pPentagramJewel->IncreaseSocket(option_slot, 16);
		pPentagramJewel->Convert(true);

		this->GetPlayer()->SendInventory(PentagramJewelSlot);
		
		this->GetPlayer()->GetInterfaceState()->SetState(0);
		this->GetPlayer()->ChaosMixPentagramResult(1, 0);

		this->ChaosMixLogSuccess();
	}
	else
	{
		if( ElementalChaosAmulet == 0 )
		{
			pPentagramJewel->SetLevel(0);
			pPentagramJewel->AlterDurability(3, 0.0f);
			pPentagramJewel->Convert(true);

			this->GetPlayer()->AddRecoveryMixItem(pPentagramJewel);

			this->GetPlayer()->ClearItem(PentagramJewelSlot);
			this->ChaosMixFailInit(0, false, 0, 1);
		}
		else
		{
			this->GetPlayer()->GetInterfaceState()->SetState(0);

			pPentagramJewel->SetLevel(0);
			pPentagramJewel->AlterDurability(3, 0.0f);
			pPentagramJewel->Convert(true);
			this->GetPlayer()->SendInventory(PentagramJewelSlot);
			this->ChaosMixFailInit(0, false, 0, 1);
		}
	}

	this->ChaosMixDBSave();
}
	
void MixHandler::PentagramJewelUpgradeRankMix(uint8 * slot, uint8 * data)
{
	int32 PentagramJewelCount = 0;
	int32 PentagramJewelSlot_1 = -1;
	int32 PentagramJewelSlot_2 = -1;
	uint8 update_rank = data[0] - 1;

	for ( int32 i = 0; i < PENTAGRAM_MIX_UPGRADE_MAX_SLOT; ++i )
	{
		if ( !use_inventory_range(slot[i]) )
		{
			continue;
		}

		Item * item = this->GetPlayer()->GetInventory()->GetItem(slot[i]);

		if ( !item )
		{
			continue;
		}

		if ( !item->IsItem() )
		{
			continue;
		}

		if (item->GetKind2() == ItemKind::ERRTEL)
		{
			if ( PentagramJewelSlot_1 == -1 )
			{
				PentagramJewelCount++;
				PentagramJewelSlot_1 = slot[i];
			}
			else
			{
				PentagramJewelCount++;
				PentagramJewelSlot_2 = slot[i];
			}
		}
	}

	if ( PentagramJewelCount != 2 || PentagramJewelSlot_1 == -1 || PentagramJewelSlot_2 == -1 )
	{
		this->ChaosMixLogInvalidMix(251);
		return;
	}

	Item * pPentagramJewel01 = this->GetPlayer()->GetInventory()->GetItem(PentagramJewelSlot_1);
	Item * pPentagramJewel02 = this->GetPlayer()->GetInventory()->GetItem(PentagramJewelSlot_2);

	if ( pPentagramJewel01->GetItem() != pPentagramJewel02->GetItem() )
	{
		this->ChaosMixLogInvalidMix(251);
		return;
	}

	if ( (pPentagramJewel01->GetSocketBonus() / 16) != (pPentagramJewel02->GetSocketBonus() / 16) )
	{
		this->ChaosMixLogInvalidMix(251);
		return;
	}

	if ( !CHECK_RANGE(update_rank, MAX_SOCKET_SLOT) )
	{
		this->ChaosMixLogInvalidMix(251);
		return;
	}

	if ( (pPentagramJewel01->GetSocketBonus() / 16) != update_rank )
	{
		this->ChaosMixLogInvalidMix(251);
		return;
	}

	if( !CHECK_RANGE(((pPentagramJewel01->GetSocketBonus() / 16) - 1), MAX_PENTAGRAM_JEWEL_RANK) )
	{
		this->ChaosMixLogInvalidMix(251);
		return;
	}

	PentagramJewelUpgradeRankInfo const* PentagramJewelUpgradeRankInfo = sPentagramSystem->GetPentagramJewelUpgradeRankInfo(update_rank);

	if( !PentagramJewelUpgradeRankInfo )
	{
		this->ChaosMixLogInvalidMix(251);
		return;
	}

	this->success_rate.set(PentagramJewelUpgradeRankInfo->GetMixRate());
	this->SetMaxSuccessRate(10000);
	this->success_rate.limit_max(10000);
	this->mix_price = PentagramJewelUpgradeRankInfo->GetMixMoney();

	if ( !this->tributeTaxPrice(true) )
	{
		return;
	}

	this->ChaosMixDebugMessage();

	for ( int32 i = 0; i < PENTAGRAM_MIX_UPGRADE_MAX_SLOT; ++i )
	{
		if ( !use_inventory_range(slot[i]) )
		{
			continue;
		}

		if ( PentagramJewelSlot_1 == slot[i] )
		{
			continue;
		}

		Item * item = this->GetPlayer()->GetInventory()->GetItem(slot[i]);

		if ( !item )
		{
			continue;
		}

		if ( !item->IsItem() )
		{
			continue;
		}

		this->GetPlayer()->ClearItem(slot[i]);
	}

	if ( this->roll_success() )
	{
		this->GetPlayer()->GetInventory()->GetItem(PentagramJewelSlot_1)->SetLevel(0);
		this->GetPlayer()->GetInventory()->GetItem(PentagramJewelSlot_1)->IncreaseSocketBonus(16);
		this->GetPlayer()->GetInventory()->GetItem(PentagramJewelSlot_1)->SetSocket(update_rank, sPentagramSystem->GetRandomErrtelOption(this->GetPlayer()->GetInventory()->GetItem(PentagramJewelSlot_1)->GetItem(), update_rank + 1));
		this->GetPlayer()->GetInventory()->GetItem(PentagramJewelSlot_1)->Convert(true);

		this->GetPlayer()->SendInventory(PentagramJewelSlot_1);
		this->GetPlayer()->GetInterfaceState()->SetState(0);
		this->GetPlayer()->ChaosMixPentagramResult(1, 0);

		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixFailInit(0, false, 0, 1);
	}

	this->ChaosMixDBSave();
}

void MixHandler::SocketMix(uint8 id, uint8 * slot, uint8 data, uint8 count)
{
	if ( sMixMgr->IsMixDisabled(3, id) )
	{
		this->ChaosMixLogInvalidMix(252);
		return;
	}

	if ( id >= MAX_SOCKET_MIX )
	{
		this->ChaosMixLogInvalidMix(252);
		return;
	}

	SocketMixHandler const& mixhandle = SocketMixTable[id];

	if ( mixhandle.type != this->GetPlayer()->GetInterfaceState()->GetID() )
	{
		this->GetPlayer()->SendNotice(CUSTOM_MESSAGE_ID_RED, "Wrong Mix Interface. Contact Administrators");
		sLog->outError(LOG_CHAOS_MIX, "Wrong Mix Interface. %s - Mix: %d [%s], Mix Interface: %s, Current Interface: %s",
			this->GetPlayer()->BuildLog().c_str(), id, mixhandle.name, mixhandle.type.c_str(), this->GetPlayer()->GetInterfaceState()->GetID().c_str());

		this->ChaosMixLogInvalidMix(252);
		return;
	}

	if ( this->GetPlayer()->GetInterfaceState()->GetState() == 1 )
	{
		return;
	}

	this->GetPlayer()->GetInterfaceState()->SetState(1);
	this->SetMixName(mixhandle.name);
	this->SetMixID(id);
	this->SetSuccess(false);
	this->GetPlayer()->SetMixCompleted(false);
	this->SetDBFlag(0);
	this->ResetItemDataBefore();
	this->ResetItemDataAfter();

	for ( int32 i = 0; i < count; ++i )
	{
		if ( !use_inventory_range(slot[i]) )
		{
			continue;
		}

		Item * item = this->GetPlayer()->GetInventory()->GetItem(slot[i]);

		if ( !item->IsItem() )
		{
			continue;
		}

		this->ChaosMixLogItem(item, slot[i]);

		if (this->GetItemDataBefore().empty())
		{
			this->SetItemDataBefore("[");
		}
		else
		{
			this->SetItemDataBefore(this->GetItemDataBefore() + ",");
		}
		
		this->SetItemDataBefore(this->GetItemDataBefore() + item->BuildLogDB(slot[i], false));
	}

	if (!this->GetItemDataBefore().empty())
	{
		this->SetItemDataBefore(this->GetItemDataBefore() + "]");
	}

	(this->*mixhandle.handler)(slot, data, count);
}

void MixHandler::SocketMixResult(uint8 result)
{
	this->GetPlayer()->ChaosMixSocketResult(result, 0);
	this->GetPlayer()->GetInterfaceState()->SetState(0);

	sLog->outError(LOG_CHAOS_MIX, "%s - %s Invalid Mix Result %u",
		this->GetMixName().c_str(), this->GetPlayer()->BuildLog().c_str(), result);
}

void MixHandler::SocketMixNone(uint8 * slot, uint8 data, int32 slot_count)
{
	this->SocketMixResult(252);
}

void MixHandler::SocketMixSetUpgrade(uint8 * slot, uint8 data, int32 slot_count)
{
	this->SetJewelItem(0, JEWEL::CHAOS);
	this->SetJewelItem(1, JEWEL::BLESS);
	this->SetJewelItem(2, JEWEL::SOUL);
	this->SetJewelItem(3, ITEMGET(14, 364));

	this->SetMaxJewelCount(0, 1);
	this->SetMaxJewelCount(1, 5);
	this->SetMaxJewelCount(2, 5);
	this->SetMaxJewelCount(3, 1);

	int32 ItemCount = 0;
	item_socket_upgrade const* pSocketUpgrade = nullptr;
	int32 SocketCount = 0;
	uint8 SocketLuck = 0;
	uint8 SocketSkill = 0;
	uint8 SocketItemUpgradeSlot = -1;

	for ( int32 i = 0; i < slot_count; ++i )
	{
		if ( !use_inventory_range(slot[i]) )
		{
			continue;
		}

		Item * item = this->GetPlayer()->GetInventory()->GetItem(slot[i]);

		if ( !item )
		{
			continue;
		}

		if ( !item->IsItem() )
		{
			continue;
		}
		
		if ( item->GetMaxSocket() > 0 )
		{
			pSocketUpgrade = sItemMgr->GetSocketUpgrade(item);
			++ItemCount;

			SocketCount = item->GetSocketCount();
			SocketLuck = item->GetLuck();
			SocketSkill = item->GetSkill();
			SocketItemUpgradeSlot = slot[i];
			this->SetDBFlag(item->GetDBFlag());
		}
	}

	if (!this->HaveJewels())
	{
		this->SocketMixResult(251);
		return;
	}

	if( ItemCount != 1 || !pSocketUpgrade )
	{
		this->SocketMixResult(251);
		return;
	}

	this->success_rate.set(pSocketUpgrade->GetRate());
	this->SetMaxSuccessRate(10000);
	this->mix_price = pSocketUpgrade->GetPrice();

	if ( !this->tributeTaxPrice(true, 2) )
	{
		return;
	}

	this->ChaosMixDebugMessage();

	for ( int32 i = 0; i < slot_count; ++i )
	{
		if ( !use_inventory_range(slot[i]) )
		{
			continue;
		}

		if ( SocketItemUpgradeSlot == slot[i] )
		{
			continue;
		}

		Item * item = this->GetPlayer()->GetInventory()->GetItem(slot[i]);

		if ( !item )
		{
			continue;
		}

		if ( !item->IsItem() )
		{
			continue;
		}

		if (this->IsJewel(slot[i]))
		{
			continue;
		}

		this->GetPlayer()->ClearItem(slot[i]);
	}

	this->JewelConsume(slot, slot_count);

	if ( this->roll_success() )
	{
		this->GetPlayer()->ClearItem(SocketItemUpgradeSlot);

		uint8 skill = (SocketSkill != 0 || roll_chance_i(pSocketUpgrade->GetUpgradeSkillRate(), 10000)) ? 1 : 0;
		uint8 luck = (SocketLuck != 0 || roll_chance_i(pSocketUpgrade->GetUpgradeLuckRate(), 10000)) ? 1 : 0;
		INIT_SOCKET_DATA(socket);

		for ( int32 i = 0; i < SocketCount; ++i )
		{
			socket[i] = SOCKET_SLOT_EMPTY;
		}

		for ( int32 i = 0; i < MAX_SOCKET_SLOT; ++i )
		{
			if ( socket[i] == SOCKET_SLOT_EMPTY )
			{
				continue;
			}

			if ( !pSocketUpgrade->GetUpgradeSocketRate(i) )
			{
				break;
			}

			if ( roll_chance_i(pSocketUpgrade->GetUpgradeSocketRate(i), 10000) )
			{
				socket[i] = SOCKET_SLOT_EMPTY;
			}
			else
			{
				 break;
			}
		}

		Item item_create(pSocketUpgrade->GetUpgradeItem(), pSocketUpgrade->GetUpgradeLevel(), 0, skill, luck, 0, 0, 0, socket);
		item_create.SetDBFlag(this->GetDBFlag());
	
		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_socket_mix, item_create);

		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixFailInit(0, false, 0, 2);
	}

	this->ChaosMixDBSave();
}
	
void MixHandler::SocketMixSeedCreate(uint8 * slot, uint8 data, int32 slot_count)
{
	this->SetJewelItem(0, JEWEL::HARMONY);
	this->SetJewelItem(1, JEWEL::CREATION);
	this->SetJewelItem(2, JEWEL::CHAOS);

	this->SetMaxJewelCount(0, 1);
	this->SetMaxJewelCount(1, 1);
	this->SetMaxJewelCount(2, 1);

	int32 ExcellentItem = 0;
	int32 AncientItem = 0;

	for ( int32 i = 0; i < slot_count; ++i )
	{
		if ( !use_inventory_range(slot[i]) )
		{
			continue;
		}

		Item * item = this->GetPlayer()->GetInventory()->GetItem(slot[i]);

		if ( !item )
		{
			continue;
		}

		if ( !item->IsItem() )
		{
			continue;
		}

		if ( item->GetLevel() >= 4 )
		{
			if ( item->IsExcellent() )
			{
				ExcellentItem++;
			}
			else if ( item->IsAncient() )
			{
				AncientItem++;
			}
		}
	}

	if (!this->HaveJewels())
	{
		this->SocketMixResult(251);
		return;
	}
	
	if( ExcellentItem != 1 || AncientItem != 1 )
	{
		this->SocketMixResult(251);
		return;
	}

	this->success_rate.set(100);
	this->mix_price = 1000000;

	if ( !this->tributeTaxPrice(true, 2) )
	{
		return;
	}

	this->ChaosMixDebugMessage();

	for ( int32 i = 0; i < slot_count; ++i )
	{
		if ( !use_inventory_range(slot[i]) )
		{
			continue;
		}

		Item * item = this->GetPlayer()->GetInventory()->GetItem(slot[i]);

		if ( !item )
		{
			continue;
		}

		if ( !item->IsItem() )
		{
			continue;
		}

		if (this->IsJewel(slot[i]))
		{
			continue;
		}

		this->GetPlayer()->ClearItem(slot[i]);
	}

	this->JewelConsume(slot, slot_count);

	if ( this->roll_success() )
	{
		uint16 item = 0;
		uint8 level = 0;

		sItemMgr->GetRandomSocketSeed(item, level);

		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_socket_mix, Item(item, level));

		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixFailInit(0, false, 0, 2);
	}

	this->ChaosMixDBSave();
}

void MixHandler::SocketMixSeedAssemble(uint8 * slot, uint8 data, int32 slot_count)
{
	this->SetJewelItem(0, JEWEL::CREATION);
	this->SetJewelItem(1, JEWEL::CHAOS);

	this->SetMaxJewelCount(0, 1);
	this->SetMaxJewelCount(1, 1);

	int32 Seed = 0;
	int32 Sphere = 0;
	uint8 SeedType = 0;
	uint8 SeedLevel = 0;

	for ( int32 i = 0; i < slot_count; ++i )
	{
		if ( !use_inventory_range(slot[i]) )
		{
			continue;
		}

		Item * item = this->GetPlayer()->GetInventory()->GetItem(slot[i]);

		if ( !item )
		{
			continue;
		}

		if ( !item->IsItem() )
		{
			continue;
		}

		if ( item->GetItem() == ITEMGET(12, 70) )
		{
			Sphere++;
		}
		else if ( item->GetItem() >= ITEMGET(12, 60) && item->GetItem() <= ITEMGET(12, 65) )
		{
			Seed++;
			SeedType = item->GetItem() - ITEMGET(12, 60);
			SeedLevel = item->GetLevel();
		}
	}

	if (!this->HaveJewels())
	{
		this->SocketMixResult(251);
		return;
	}

	if (Seed != 1 || Sphere != 1)
	{
		this->SocketMixResult(251);
		return;
	}

	this->success_rate.set(100);
	this->mix_price = 1000000;

	if ( !this->tributeTaxPrice(true, 2) )
	{
		return;
	}

	this->ChaosMixDebugMessage();

	for ( int32 i = 0; i < slot_count; ++i )
	{
		if ( !use_inventory_range(slot[i]) )
		{
			continue;
		}

		Item * item = this->GetPlayer()->GetInventory()->GetItem(slot[i]);

		if ( !item )
		{
			continue;
		}

		if ( !item->IsItem() )
		{
			continue;
		}

		if (this->IsJewel(slot[i]))
		{
			continue;
		}

		this->GetPlayer()->ClearItem(slot[i]);
	}

	this->JewelConsume(slot, slot_count);

	if ( this->roll_success() )
	{
		uint16 item = ITEMGET(12, 100) + SeedType;
		uint8 level = SeedLevel;

		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_socket_mix, Item(item, level));

		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixFailInit(0, false, 0, 2);
	}

	this->ChaosMixDBSave();
}

void MixHandler::SocketMixSeedUpgrade(uint8 * slot, uint8 data, int32 slot_count)
{
	int32 Rune = 0;
	int32 RequiredRune = 0;
	int32 SeedSphere = 0;
	uint8 SeedSphereType = -1;
	uint8 SeedSphereLevel = -1;
	uint8 SeedSphereRank = -1;
	uint8 SeedSphereCurRank = -1;
	uint16 SeedSphereNext = -1;
	uint8 SeedSphereSlot = -1;

	for ( int32 i = 0; i < slot_count; ++i )
	{
		if ( !use_inventory_range(slot[i]) )
		{
			continue;
		}

		Item * item = this->GetPlayer()->GetInventory()->GetItem(slot[i]);

		if ( !item )
		{
			continue;
		}

		if ( !item->IsItem() )
		{
			continue;
		}

		if ( item->GetItem() == ITEMGET(12, 401) )
		{
			Rune += item->GetDurability();
		}
		else if ( item_socket_seed_sphere const* pSeedSphere = sItemMgr->GetSocketSeedSphere(item->GetItem()) )
		{
			if ( SeedSphereSlot == uint8(-1) )
			{
				SeedSphereType = pSeedSphere->GetAttribute();
				SeedSphereLevel = item->GetLevel();
				SeedSphereSlot = slot[i];
				SeedSphereCurRank = pSeedSphere->GetRank();
				SeedSphereNext = pSeedSphere->GetNextID();
				SeedSphere++;
			}
			else
			{
				if ( SeedSphereCurRank != pSeedSphere->GetRank() )
				{
					SeedSphere = 0;
				}
				else
				{
					SeedSphere++;
				}
			}
		}
	}

	this->SocketMixSphereStrengthenerRuneFind(Rune, slot, slot_count);

	item_socket_mix const* pSocketMix = sItemMgr->GetSocketMix(SeedSphereCurRank);

	if ( !pSocketMix )
	{
		this->SocketMixResult(251);
		return;
	}

	if( Rune < pSocketMix->GetRequiredRune() || SeedSphere != 2 || SeedSphereSlot == uint8(-1) )
	{
		this->SocketMixResult(251);
		return;
	}

	Rune = pSocketMix->GetRequiredRune();

	this->success_rate.set(pSocketMix->GetSuccessRate());
	this->mix_price = pSocketMix->GetPrice();

	if ( !this->tributeTaxPrice(true, 2) )
	{
		return;
	}

	this->ChaosMixDebugMessage();

	for ( int32 i = 0; i < slot_count; ++i )
	{
		if ( !use_inventory_range(slot[i]) )
		{
			continue;
		}

		Item * item = this->GetPlayer()->GetInventory()->GetItem(slot[i]);

		if ( !item )
		{
			continue;
		}

		if ( !item->IsItem() )
		{
			continue;
		}

		if ( SeedSphereSlot == slot[i] )
		{
			continue;
		}

		if( item->GetItem() == ITEMGET(12, 401) )
		{
			if ( Rune > 0 )
			{
				if ( item->GetDurability() > Rune )
				{
					item->AlterDurability(0, Rune);
					this->GetPlayer()->SendItemDurability(slot[i], 1);
					Rune = 0;
				}
				else
				{
					Rune -= item->GetDurability();
					this->GetPlayer()->ClearItem(slot[i]);
				}
			}
		}
		else
		{
			this->GetPlayer()->ClearItem(slot[i]);
		}
	}

	this->SocketMixSphereStrengthenerRuneConsume(Rune, slot, slot_count);

	if ( this->roll_success() )
	{
		this->GetPlayer()->ClearItem(SeedSphereSlot);

		uint16 item = SeedSphereNext;
		uint8 level = SeedSphereLevel;

		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_socket_mix, Item(item, level));

		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixFailInit(0, false, 0, 2);
	}

	this->ChaosMixDBSave();
}

void MixHandler::SocketMixSeedInsert(uint8 * slot, uint8 data, int32 slot_count)
{
	if ( data >= MAX_SOCKET_SLOT )
	{
		this->SocketMixResult(251);
		return;
	}

	this->SetJewelItem(0, JEWEL::CHAOS);
	this->SetJewelItem(1, JEWEL::CREATION);

	this->SetMaxJewelCount(0, 1);
	this->SetMaxJewelCount(1, 1);
	
	uint8 ItemSlot = -1;
	SocketDataType SeedSphereOption = -1;
	item_socket_seed_sphere const* pSeedSphere = nullptr;

	for ( int32 i = 0; i < slot_count; ++i )
	{
		if ( !use_inventory_range(slot[i]) )
		{
			continue;
		}

		Item * item = this->GetPlayer()->GetInventory()->GetItem(slot[i]);

		if ( !item )
		{
			continue;
		}

		if ( !item->IsItem() )
		{
			continue;
		}

		if ( item->IsSocket() && item->GetMaxSocket() > 0 )
		{
			ItemSlot = slot[i];
		}
		else if (!pSeedSphere)
		{
			pSeedSphere = sItemMgr->GetSocketSeedSphere(item->GetItem());

			if (pSeedSphere)
			{
				SeedSphereOption = sItemMgr->GetSocketByteOption(item->GetItem(), item->GetLevel());
			}
		}
	}

	if (!this->HaveJewels())
	{
		this->SocketMixResult(251);
		return;
	}

	if( ItemSlot == uint8(-1) || SeedSphereOption == SocketDataType(-1) || !pSeedSphere )
	{
		this->SocketMixResult(251);
		return;
	}

	if ( this->GetPlayer()->GetInventory()->GetItem(ItemSlot)->GetSocket(data) != SOCKET_SLOT_EMPTY )
	{
		this->SocketMixResult(251);
		return;
	}

	if ( !this->GetPlayer()->GetInventory()->GetItem(ItemSlot)->SocketAllowed(SeedSphereOption % 50) )
	{
		this->SocketMixResult(251);
		return;
	}

	if ( this->GetPlayer()->GetInventory()->GetItem(ItemSlot)->GetItem() >= ITEMGET(0, 0) &&
		 this->GetPlayer()->GetInventory()->GetItem(ItemSlot)->GetItem() < ITEMGET(6, 0) )
	{
		if ( pSeedSphere->GetAttribute() != 1 &&
			 pSeedSphere->GetAttribute() != 3 &&
			 pSeedSphere->GetAttribute() != 5 )
		{
			this->SocketMixResult(251);
			return;
		}
	}
	else if ( this->GetPlayer()->GetInventory()->GetItem(ItemSlot)->GetItem() >= ITEMGET(6, 0) &&
			  this->GetPlayer()->GetInventory()->GetItem(ItemSlot)->GetItem() < ITEMGET(12, 0) )
	{
		if ( pSeedSphere->GetAttribute() != 2 &&
			 pSeedSphere->GetAttribute() != 4 &&
			 pSeedSphere->GetAttribute() != 6 )
		{
			this->SocketMixResult(251);
			return;
		}
	}

	this->success_rate.set(100);
	this->mix_price = 1000000;

	if ( !this->tributeTaxPrice(true, 2) )
	{
		return;
	}

	this->ChaosMixDebugMessage();

	for ( int32 i = 0; i < slot_count; ++i )
	{
		if ( !use_inventory_range(slot[i]) )
		{
			continue;
		}

		Item * item = this->GetPlayer()->GetInventory()->GetItem(slot[i]);

		if ( !item )
		{
			continue;
		}

		if ( !item->IsItem() )
		{
			continue;
		}

		if ( ItemSlot == slot[i] )
		{
			continue;
		}

		if (this->IsJewel(slot[i]))
		{
			continue;
		}

		this->GetPlayer()->ClearItem(slot[i]);
	}

	this->JewelConsume(slot, slot_count);

	if ( this->roll_success() )
	{
		this->GetPlayer()->GetInventory()->GetItem(ItemSlot)->SetSocket(data, SeedSphereOption);
		this->GetPlayer()->GetInventory()->GetItem(ItemSlot)->SetSocketBonus(sItemMgr->GetSocketBonus(this->GetPlayer()->GetInventory()->GetItem(ItemSlot), (SeedSphereOption / MAX_SOCKET_OPTION) + 1));
		this->GetPlayer()->GetInventory()->GetItem(ItemSlot)->AlterDurability(3, 0.0f);
		this->GetPlayer()->GetInventory()->GetItem(ItemSlot)->Convert();

		this->GetPlayer()->SendInventory(ItemSlot);

		this->GetPlayer()->GetInterfaceState()->SetState(0);
		this->GetPlayer()->ChaosMixSocketResult(1, 0, ItemSlot);

		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixFailInit(0, false, 0, 2);
	}

	this->ChaosMixDBSave();
}

void MixHandler::SocketMixSeedRemove(uint8 * slot, uint8 data, int32 slot_count)
{
	if ( data >= MAX_SOCKET_SLOT )
	{
		this->SocketMixResult(251);
		return;
	}

	this->SetJewelItem(0, JEWEL::CHAOS);
	this->SetJewelItem(1, JEWEL::GUARDIAN);
	this->SetJewelItem(2, JEWEL::HARMONY);

	this->SetMaxJewelCount(0, 5);
	this->SetMaxJewelCount(1, 1);
	this->SetMaxJewelCount(2, 5);

	uint8 ItemSlot = -1;
	
	for ( int32 i = 0; i < slot_count; ++i )
	{
		if ( !use_inventory_range(slot[i]) )
		{
			continue;
		}

		Item * item = this->GetPlayer()->GetInventory()->GetItem(slot[i]);

		if ( !item )
		{
			continue;
		}

		if ( !item->IsItem() )
		{
			continue;
		}

		if ( item->IsSocket() )
		{
			ItemSlot = slot[i];
		}
	}

	if (!this->HaveJewels())
	{
		this->SocketMixResult(251);
		return;
	}

	if( ItemSlot == uint8(-1) )
	{
		this->SocketMixResult(251);
		return;
	}

	if ( this->GetPlayer()->GetInventory()->GetItem(ItemSlot)->GetSocket(data) == SOCKET_SLOT_NONE ||
		 this->GetPlayer()->GetInventory()->GetItem(ItemSlot)->GetSocket(data) == SOCKET_SLOT_EMPTY )
	{
		this->SocketMixResult(251);
		return;
	}

	this->success_rate.set(90);
	this->mix_price = 1000000;

	if ( !this->tributeTaxPrice(true, 2) )
	{
		return;
	}

	this->ChaosMixDebugMessage();

	for ( int32 i = 0; i < slot_count; ++i )
	{
		if ( !use_inventory_range(slot[i]) )
		{
			continue;
		}

		Item * item = this->GetPlayer()->GetInventory()->GetItem(slot[i]);

		if ( !item )
		{
			continue;
		}

		if ( !item->IsItem() )
		{
			continue;
		}

		if ( ItemSlot == slot[i] )
		{
			continue;
		}

		if (this->IsJewel(slot[i]))
		{
			continue;
		}

		this->GetPlayer()->ClearItem(slot[i]);
	}

	this->JewelConsume(slot, slot_count);

	SocketDataType SocketData = this->GetPlayer()->GetInventory()->GetItem(ItemSlot)->GetSocket(data);
	uint8 SeedSphereType = SocketData % MAX_SOCKET_OPTION;
	uint8 SeedSphereLevel = SocketData / MAX_SOCKET_OPTION;
	uint16 SeedSphereItem = -1;

	if ( SeedSphereLevel >= MAX_SOCKET_LEVEL )
	{
		SeedSphereLevel = 0;
	}

	item_socket_seed const* pSeed = sItemMgr->GetSocketSeed(SeedSphereType);

	if ( pSeed )
	{
		SeedSphereItem = sItemMgr->GetSocketSeedSphere(pSeed->GetType(), SeedSphereLevel);
		SeedSphereLevel = pSeed->GetSubType();
	}

	//this->GetPlayer()->ChaosMixSocketResult(1, 0, ItemSlot);

	if ( this->roll_success() && SeedSphereItem != uint16(-1) )
	{
		this->GetPlayer()->GetInventory()->GetItem(ItemSlot)->SetSocket(data, SOCKET_SLOT_EMPTY);
		this->GetPlayer()->GetInventory()->GetItem(ItemSlot)->SetSocketBonus(sItemMgr->GetSocketBonus(this->GetPlayer()->GetInventory()->GetItem(ItemSlot), 0));
		this->GetPlayer()->GetInventory()->GetItem(ItemSlot)->AlterDurability(3, 0.0f);
		this->GetPlayer()->GetInventory()->GetItem(ItemSlot)->Convert();

		this->GetPlayer()->SendInventory(ItemSlot);

		sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_socket_mix, Item(SeedSphereItem, SeedSphereLevel));

		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixFailInit(0, false, 0, 2);
	}

	this->ChaosMixDBSave();
}

void MixHandler::SocketMixSlotReduction(uint8 * slot, uint8 data, int32 slot_count)
{
	this->SetJewelItem(0, JEWEL::CREATION);
	this->SetJewelItem(1, JEWEL::CHAOS);

	this->SetMaxJewelCount(0, 1);
	this->SetMaxJewelCount(1, 1);
	
	uint8 ItemSlot = -1;
	
	for ( int32 i = 0; i < slot_count; ++i )
	{
		if ( !use_inventory_range(slot[i]) )
		{
			continue;
		}

		Item * item = this->GetPlayer()->GetInventory()->GetItem(slot[i]);

		if ( !item )
		{
			continue;
		}

		if ( !item->IsItem() )
		{
			continue;
		}

		if ( item->GetMaxSocket() > 0 && item->GetSocketCount() >= 4 )
		{
			ItemSlot = slot[i];
		}
	}

	if (!this->HaveJewels())
	{
		this->SocketMixResult(251);
		return;
	}
	
	if( ItemSlot == uint8(-1) )
	{
		this->SocketMixResult(251);
		return;
	}

	this->success_rate.set(100);
	this->mix_price = 1000000;

	if ( !this->tributeTaxPrice(true, 2) )
	{
		return;
	}

	this->ChaosMixDebugMessage();

	for ( int32 i = 0; i < slot_count; ++i )
	{
		if ( !use_inventory_range(slot[i]) )
		{
			continue;
		}

		Item * item = this->GetPlayer()->GetInventory()->GetItem(slot[i]);

		if ( !item )
		{
			continue;
		}

		if ( !item->IsItem() )
		{
			continue;
		}

		if ( ItemSlot == slot[i] )
		{
			continue;
		}

		if (this->IsJewel(slot[i]))
		{
			continue;
		}

		this->GetPlayer()->ItemDeleteByUse(slot[i]);
	}

	this->JewelConsume(slot, slot_count);

	if ( this->roll_success() )
	{
		this->GetPlayer()->GetInventory()->GetItem(ItemSlot)->SetSocket(3, SOCKET_SLOT_NONE);
		this->GetPlayer()->GetInventory()->GetItem(ItemSlot)->SetSocket(4, SOCKET_SLOT_NONE);
		if ( this->GetPlayer()->GetInventory()->GetItem(ItemSlot)->GetSocketBonus() != uint8(-1) )
		{
			this->GetPlayer()->GetInventory()->GetItem(ItemSlot)->SetSocketBonus(sItemMgr->GetSocketBonus(this->GetPlayer()->GetInventory()->GetItem(ItemSlot), 0));
		}
		this->GetPlayer()->GetInventory()->GetItem(ItemSlot)->AlterDurability(3, 0.0f);
		this->GetPlayer()->GetInventory()->GetItem(ItemSlot)->Convert();

		this->GetPlayer()->SendInventory(ItemSlot);

		this->GetPlayer()->ChaosMixSocketResult(1, 0, ItemSlot);

		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixFailInit(0, false, 0, 2);
	}

	this->ChaosMixDBSave();
}

bool MixHandler::HaveJewels()
{
	this->JewelFind();

	for (int32 i = 0; i < MAX_JEWEL_ITEM_COUNT; ++i)
	{
		if (this->GetJewelItem(i) == uint16(-1))
		{
			continue;
		}

		if (this->GetJewelCount(i) < this->GetMaxJewelCount(i))
		{
			return false;
		}
	}

	this->NormalizeJewels();
	return true;
}

void MixHandler::NormalizeJewels()
{
	for (int32 i = 0; i < MAX_JEWEL_ITEM_COUNT; ++i)
	{
		this->SetJewelCount(i, this->GetMaxJewelCount(i));
	}
}

bool MixHandler::IsJewel(uint8 slot)
{
	Item* pItem = this->GetPlayer()->GetInventory()->GetItem(slot);

	if (!pItem || !pItem->IsItem())
	{
		return false;
	}

	for (int32 n = 0; n < MAX_JEWEL_ITEM_COUNT; ++n)
	{
		if (this->GetJewelItem(n) == uint16(-1))
		{
			continue;
		}

		if (pItem->GetItem() == this->GetJewelItem(n))
		{
			return true;
		}
	}

	return false;
}

void MixHandler::JewelFind()
{
	use_inventory_loop(i)
	{
		Item* pItem = this->GetPlayer()->GetInventory()->GetItem(i);

		if (!pItem || !pItem->IsItem())
		{
			continue;
		}

		for (int32 n = 0; n < MAX_JEWEL_ITEM_COUNT; ++n)
		{
			if (this->GetJewelItem(n) == uint16(-1))
			{
				continue;
			}

			if (pItem->GetItem() == this->GetJewelItem(n))
			{
				item_template const* pItemInfo = sItemMgr->GetItem(pItem->GetItem());

				if (pItemInfo)
				{
					if (pItemInfo->GetStackData()->GetMaxStack() > 0)
					{
						this->IncreaseJewelCount(n, pItem->GetDurability());
					}
					else
					{
						this->IncreaseJewelCount(n, 1);
					}
				}
			}
		}
	}
}

void MixHandler::JewelConsume(uint8 * slot, uint8 slot_count)
{
	for (int32 i = 0; i < slot_count; ++i)
	{
		if (!use_inventory_range(slot[i]))
		{
			continue;
		}

		Item* pItem = this->GetPlayer()->GetInventory()->GetItem(slot[i]);

		if (!pItem || !pItem->IsItem())
		{
			continue;
		}

		for (int32 n = 0; n < MAX_JEWEL_ITEM_COUNT; ++n)
		{
			if (this->GetJewelCount(n) <= 0)
			{
				continue;
			}

			if (this->GetJewelItem(n) == uint16(-1))
			{
				continue;
			}

			if (pItem->GetItem() == this->GetJewelItem(n))
			{
				item_template const* pItemInfo = sItemMgr->GetItem(pItem->GetItem());

				if (pItemInfo)
				{
					if (pItemInfo->GetStackData()->GetMaxStack() > 0)
					{
						if (pItem->GetDurability() > this->GetJewelCount(n))
						{
							pItem->AlterDurability(0, this->GetJewelCount(n));
							this->GetPlayer()->SendItemDurability(slot[i], 1);
							this->SetJewelCount(n, 0);
						}
						else
						{
							this->ReduceJewelCount(n, pItem->GetDurability());
							this->GetPlayer()->ClearItem(slot[i]);
						}
					}
					else
					{
						this->ReduceJewelCount(n, 1);
						this->GetPlayer()->ClearItem(slot[i]);
					}
				}
			}
		}
	}

	use_inventory_loop(i)
	{
		Item* pItem = this->GetPlayer()->GetInventory()->GetItem(i);

		if (!pItem || !pItem->IsItem())
		{
			continue;
		}

		for (int32 n = 0; n < MAX_JEWEL_ITEM_COUNT; ++n)
		{
			if (this->GetJewelCount(n) <= 0)
			{
				continue;
			}

			if (this->GetJewelItem(n) == uint16(-1))
			{
				continue;
			}

			if (pItem->GetItem() == this->GetJewelItem(n))
			{
				item_template const* pItemInfo = sItemMgr->GetItem(pItem->GetItem());

				if (pItemInfo)
				{
					if (pItemInfo->GetStackData()->GetMaxStack() > 0)
					{
						if (pItem->GetDurability() > this->GetJewelCount(n))
						{
							pItem->AlterDurability(0, this->GetJewelCount(n));
							this->GetPlayer()->SendItemDurability(i, 1);
							this->SetJewelCount(n, 0);
						}
						else
						{
							this->ReduceJewelCount(n, pItem->GetDurability());
							this->GetPlayer()->ClearItem(i);
						}
					}
					else
					{
						this->ReduceJewelCount(n, 1);
						this->GetPlayer()->ClearItem(i);
					}
				}
			}
		}
	}
}

void MixHandler::SocketMixSphereStrengthenerRuneFind(int32 & count, uint8 * slot, uint8 slot_count)
{
	use_inventory_loop(i)
	{
		bool success = true;

		for ( int32 n = 0; n < slot_count; ++n )
		{
			if ( slot[n] == i )
			{
				success = false;
				break;
			}
		}

		if ( !success )
		{
			continue;
		}

		Item const* pItem = this->GetPlayer()->GetInventory()->GetItem(i);

		if ( !pItem )
		{
			continue;
		}

		if ( pItem->GetItem() == ITEMGET(12, 401) )
		{
			count += pItem->GetDurability();
		}
	}
}
	
void MixHandler::SocketMixSphereStrengthenerRuneConsume(int32 & count, uint8 * slot, uint8 slot_count)
{
	if ( count <= 0 )
	{
		return;
	}

	use_inventory_loop(i)
	{
		bool success = true;

		for ( int32 n = 0; n < slot_count; ++n )
		{
			if ( slot[n] == i )
			{
				success = false;
				break;
			}
		}

		if ( !success )
		{
			continue;
		}

		Item * item = this->GetPlayer()->GetInventory()->GetItem(i);

		if ( !item )
		{
			continue;
		}

		if ( item->GetItem() == ITEMGET(12, 401) )
		{
			if ( item->GetDurability() > count )
			{
				item->AlterDurability(0, count);
				this->GetPlayer()->SendItemDurability(i, 1);
				count = 0;
			}
			else
			{
				count -= item->GetDurability();
				this->GetPlayer()->ClearItem(i);
			}

			if ( count <= 0 )
			{
				return;
			}
		}
	}
}

void MixHandler::WingSocketMix(uint8 id, uint8 * slot, uint8 count)
{
	if (sMixMgr->IsMixDisabled(4, id))
	{
		this->ChaosMixLogInvalidMix(252);
		return;
	}

	if (id >= MAX_WING_SOCKET_MIX)
	{
		this->ChaosMixLogInvalidMix(252);
		return;
	}

	WingSocketMixHandler const& mixhandle = WingSocketMixTable[id];

	if (mixhandle.type != this->GetPlayer()->GetInterfaceState()->GetID())
	{
		this->GetPlayer()->SendNotice(CUSTOM_MESSAGE_ID_RED, "Wrong Mix Interface. Contact Administrators");
		sLog->outError(LOG_CHAOS_MIX, "Wrong Mix Interface. %s - Mix: %d [%s], Mix Interface: %s, Current Interface: %s",
			this->GetPlayer()->BuildLog().c_str(), id, mixhandle.name, mixhandle.type.c_str(), this->GetPlayer()->GetInterfaceState()->GetID().c_str());

		this->ChaosMixLogInvalidMix(252);
		return;
	}

	if (this->GetPlayer()->GetInterfaceState()->GetState() == 1)
	{
		return;
	}

	this->GetPlayer()->GetInterfaceState()->SetState(1);
	this->SetMixName(mixhandle.name);
	this->SetMixID(id);
	this->SetSuccess(false);
	this->GetPlayer()->SetMixCompleted(false);
	this->SetDBFlag(0);
	this->ResetItemDataBefore();
	this->ResetItemDataAfter();

	for (int32 i = 0; i < count; ++i)
	{
		if (!use_inventory_range(slot[i]))
		{
			continue;
		}

		Item * item = this->GetPlayer()->GetInventory()->GetItem(slot[i]);

		if (!item->IsItem())
		{
			continue;
		}

		this->ChaosMixLogItem(item, slot[i]);

		if (this->GetItemDataBefore().empty())
		{
			this->SetItemDataBefore("[");
		}
		else
		{
			this->SetItemDataBefore(this->GetItemDataBefore() + ",");
		}

		this->SetItemDataBefore(this->GetItemDataBefore() + item->BuildLogDB(slot[i], false));
	}

	if (!this->GetItemDataBefore().empty())
	{
		this->SetItemDataBefore(this->GetItemDataBefore() + "]");
	}

	(this->*mixhandle.handler)(slot, count);
}

void MixHandler::WingSocketMixAdd(uint8 * slot, int32 slot_count)
{
	this->SetJewelItem(0, ITEMGET(12, 406));
	this->SetJewelItem(1, ITEMGET(13, 468));

	this->SetMaxJewelCount(0, 3);
	this->SetMaxJewelCount(1, 9);

	uint8 ErrtelLevel = -1;
	uint8 ItemSlot = -1;
	int32 ElementalLuckAmulet = 0;
	
	for (int32 i = 0; i < slot_count; ++i)
	{
		if (!use_inventory_range(slot[i]) && !INVENTORY_WEAR_RANGE(slot[i]))
		{
			continue;
		}

		Item * item = this->GetPlayer()->GetInventory()->GetItem(slot[i]);

		if (!item)
		{
			continue;
		}

		if (!item->IsItem())
		{
			continue;
		}

		if (item->IsWingLvl4() && item->GetSocketBonus() == 0xFF)
		{
			ItemSlot = slot[i];
		}
		else if (item->GetKind2() == ItemKind::ERRTEL)
		{
			ErrtelLevel = item->GetSocket(0) / 16;
		}
		else if (item->GetItem() == ITEMGET(14, 189))
		{
			ElementalLuckAmulet = 1;
		}
	}

	if (!this->HaveJewels())
	{
		this->ChaosMixLogInvalidMix(251);
		return;
	}

	if ((!use_inventory_range(ItemSlot) && !INVENTORY_WEAR_RANGE(ItemSlot)) || ErrtelLevel >= MAX_WING_SOCKET_OPTION_UPGRADE_LEVEL)
	{
		this->ChaosMixLogInvalidMix(251);
		return;
	}

	this->ChaosMixTalismanOfLuck(true);

	Item* pWings = this->GetPlayer()->GetInventory()->GetItem(ItemSlot);

	this->success_rate.set(WingSocketOptionCreateRate[ErrtelLevel]);
	this->success_rate.op_add(ElementalLuckAmulet * 10);
	this->mix_price = 100000;

	if (!this->tributeTaxPrice(true, 3))
	{
		return;
	}

	this->ChaosMixDebugMessage();

	for (int32 i = 0; i < slot_count; ++i)
	{
		if (!use_inventory_range(slot[i]) && !INVENTORY_WEAR_RANGE(slot[i]))
		{
			continue;
		}

		Item * item = this->GetPlayer()->GetInventory()->GetItem(slot[i]);

		if (!item)
		{
			continue;
		}

		if (!item->IsItem())
		{
			continue;
		}

		if (ItemSlot == slot[i])
		{
			continue;
		}

		if (this->IsJewel(slot[i]))
		{
			continue;
		}

		this->GetPlayer()->ClearItem(slot[i]);
	}

	this->JewelConsume(slot, slot_count);

	if (this->roll_success())
	{
		pWings->SetSocket(4, sItemMgr->GenerateWingSocketOption(1));
		pWings->SetSocketBonus(sItemMgr->GenerateWingSocketOption(0));
		pWings->AlterDurability(3, 0.0f);
		pWings->Convert();

		this->GetPlayer()->SendInventory(ItemSlot);

		if (INVENTORY_WEAR_RANGE(ItemSlot))
		{
			this->GetPlayer()->CalculateCharacter();
		}

		this->GetPlayer()->GetInterfaceState()->SetState(0);
		this->GetPlayer()->WingSocketOptionAddResult(1);

		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixFailInit(0, false, 0, 3);
	}

	this->ChaosMixDBSave();
}

void MixHandler::WingSocketMixUpgrade(uint8 * slot, int32 slot_count)
{
	int32 ItemCount[MAX_WING_SOCKET_OPTION_UPGRADE_ITEM];
	for (int32 i = 0; i < MAX_WING_SOCKET_OPTION_UPGRADE_ITEM; ++i)
	{
		ItemCount[i] = 0;
	}
	
	uint8 ItemSlot = -1;
	uint8 ItemLevel = -1;
	int32 ElementalLuckAmulet = 0;

	for (int32 i = 0; i < slot_count; ++i)
	{
		if (!use_inventory_range(slot[i]) && !INVENTORY_WEAR_RANGE(slot[i]))
		{
			continue;
		}

		Item * item = this->GetPlayer()->GetInventory()->GetItem(slot[i]);

		if (!item)
		{
			continue;
		}

		if (!item->IsItem())
		{
			continue;
		}

		if (item->IsWingLvl4() && item->GetSocketBonus() != 0xFF)
		{
			ItemSlot = slot[i];
			ItemLevel = item->GetSocketBonus() % 16;
		}
		else if (item->GetItem() == ITEMGET(14, 189))
		{
			ElementalLuckAmulet = 1;
		}
	}

	if ((!use_inventory_range(ItemSlot) && !INVENTORY_WEAR_RANGE(ItemSlot)) || ItemLevel == uint8(-1))
	{
		sLog->outError("root", "%s(1)", __FUNCTION__);
		this->ChaosMixLogInvalidMix(251);
		return;
	}

	item_wing_socket_upgrade const* pData = sItemMgr->GetWingSocketUpgrade(ItemLevel);

	if (!pData)
	{
		sLog->outError("root", "%s(2)", __FUNCTION__);
		this->ChaosMixLogInvalidMix(251);
		return;
	}

	for (int32 i = 0; i < slot_count; ++i)
	{
		if (!use_inventory_range(slot[i]) && !INVENTORY_WEAR_RANGE(slot[i]))
		{
			continue;
		}

		Item * item = this->GetPlayer()->GetInventory()->GetItem(slot[i]);

		if (!item)
		{
			continue;
		}

		if (!item->IsItem())
		{
			continue;
		}

		for (int32 n = 0; n < MAX_WING_SOCKET_OPTION_UPGRADE_ITEM; ++n)
		{
			if (pData->GetItemCount(n) == -1)
			{
				continue;
			}

			if (pData->GetItem(n) == item->GetItem())
			{
				item_template const* pItemInfo = sItemMgr->GetItem(item->GetItem());

				if (pItemInfo)
				{
					if (pItemInfo->GetStackData()->GetMaxStack() > 0)
					{
						ItemCount[n] += item->GetDurability();
					}
					else
					{
						ItemCount[n]++;
					}
				}
			}
		}
	}

	for (int32 n = 0; n < MAX_WING_SOCKET_OPTION_UPGRADE_ITEM; ++n)
	{
		if (ItemCount[n] < pData->GetItemCount(n))
		{
			sLog->outError("root", "%s(3)", __FUNCTION__);
			this->ChaosMixLogInvalidMix(251);
			return;
		}
	}

	Item* pWings = this->GetPlayer()->GetInventory()->GetItem(ItemSlot);

	for (int32 n = 0; n < MAX_WING_SOCKET_OPTION_UPGRADE_ITEM; ++n)
	{
		ItemCount[n] = pData->GetItemCount(n);
	}

	this->success_rate.set(pData->GetRate());
	this->success_rate.op_add(ElementalLuckAmulet * 10);
	this->SetMaxSuccessRate(1000000);
	this->mix_price = pData->GetZen();

	if (!this->tributeTaxPrice(true, 3))
	{
		return;
	}

	this->ChaosMixDebugMessage();

	for (int32 i = 0; i < slot_count; ++i)
	{
		if (!use_inventory_range(slot[i]) && !INVENTORY_WEAR_RANGE(slot[i]))
		{
			continue;
		}

		Item * item = this->GetPlayer()->GetInventory()->GetItem(slot[i]);

		if (!item)
		{
			continue;
		}

		if (!item->IsItem())
		{
			continue;
		}

		if (ItemSlot == slot[i])
		{
			continue;
		}

		bool founded = false;

		for (int32 n = 0; n < MAX_WING_SOCKET_OPTION_UPGRADE_ITEM; ++n)
		{
			if (pData->GetItemCount(n) == -1)
			{
				continue;
			}

			if (pData->GetItem(n) == item->GetItem())
			{
				founded = true;

				if (ItemCount[n] > 0)
				{
					item_template const* pItemInfo = sItemMgr->GetItem(item->GetItem());

					if (pItemInfo)
					{
						if (pItemInfo->GetStackData()->GetMaxStack() > 0)
						{
							if (item->GetDurability() > ItemCount[n])
							{
								item->AlterDurability(0, ItemCount[n]);
								this->GetPlayer()->SendItemDurability(slot[i], 1);
								ItemCount[n] = 0;
							}
							else
							{
								ItemCount[n] -= item->GetDurability();
								this->GetPlayer()->ClearItem(slot[i]);
							}
						}
						else
						{
							ItemCount[n]--;
							this->GetPlayer()->ClearItem(slot[i]);
						}
					}
				}
			}
		}

		if (!founded)
		{
			this->GetPlayer()->ClearItem(slot[i]);
		}
	}

	if (this->roll_success())
	{
		pWings->SetSocket(4, pWings->GetSocket(4) + 1);
		pWings->SetSocketBonus(pWings->GetSocketBonus() + 1);
		pWings->AlterDurability(3, 0.0f);
		pWings->Convert();

		this->GetPlayer()->SendInventory(ItemSlot);

		if (INVENTORY_WEAR_RANGE(ItemSlot))
		{
			this->GetPlayer()->CalculateCharacter();
		}

		this->GetPlayer()->GetInterfaceState()->SetState(0);
		this->GetPlayer()->WingSocketOptionUpgradeResult(1, ItemSlot);

		this->ChaosMixLogSuccess();
	}
	else
	{
		if (ItemLevel > 0)
		{
			pWings->SetSocket(4, pWings->GetSocket(4) - 1);
			pWings->SetSocketBonus(pWings->GetSocketBonus() - 1);

			pWings->AlterDurability(3, 0.0f);
			pWings->Convert();

			this->GetPlayer()->SendInventory(ItemSlot);

			if (INVENTORY_WEAR_RANGE(ItemSlot))
			{
				this->GetPlayer()->CalculateCharacter();
			}
		}

		this->ChaosMixFailInit(0, false, 0, 4);
		this->GetPlayer()->WingSocketOptionUpgradeResult(2, ItemSlot);
	}

	this->ChaosMixDBSave();
}

void MixHandler::EarringMix(uint8 id, uint8 * slot, uint8 count)
{
	if (sMixMgr->IsMixDisabled(5, id))
	{
		this->ChaosMixLogInvalidMix(252);
		return;
	}

	if (id >= MAX_EARRING_MIX)
	{
		this->ChaosMixLogInvalidMix(252);
		return;
	}

	EarringMixHandler const& mixhandle = EarringMixTable[id];

	if (mixhandle.type != this->GetPlayer()->GetInterfaceState()->GetID())
	{
		this->GetPlayer()->SendNotice(CUSTOM_MESSAGE_ID_RED, "Wrong Mix Interface. Contact Administrators");
		sLog->outError(LOG_CHAOS_MIX, "Wrong Mix Interface. %s - Mix: %d [%s], Mix Interface: %s, Current Interface: %s",
			this->GetPlayer()->BuildLog().c_str(), id, mixhandle.name, mixhandle.type.c_str(), this->GetPlayer()->GetInterfaceState()->GetID().c_str());

		this->ChaosMixLogInvalidMix(252);
		return;
	}

	if (this->GetPlayer()->GetInterfaceState()->GetState() == 1)
	{
		return;
	}

	this->GetPlayer()->GetInterfaceState()->SetState(1);
	this->SetMixName(mixhandle.name);
	this->SetMixID(id);
	this->SetSuccess(false);
	this->GetPlayer()->SetMixCompleted(false);
	this->SetDBFlag(0);
	this->ResetItemDataBefore();
	this->ResetItemDataAfter();

	for (int32 i = 0; i < count; ++i)
	{
		if (!use_inventory_range(slot[i]))
		{
			continue;
		}

		Item * item = this->GetPlayer()->GetInventory()->GetItem(slot[i]);

		if (!item->IsItem())
		{
			continue;
		}

		this->ChaosMixLogItem(item, slot[i]);

		if (this->GetItemDataBefore().empty())
		{
			this->SetItemDataBefore("[");
		}
		else
		{
			this->SetItemDataBefore(this->GetItemDataBefore() + ",");
		}

		this->SetItemDataBefore(this->GetItemDataBefore() + item->BuildLogDB(slot[i], false));
	}

	if (!this->GetItemDataBefore().empty())
	{
		this->SetItemDataBefore(this->GetItemDataBefore() + "]");
	}

	(this->*mixhandle.handler)(slot, count);
}

void MixHandler::EarringUpgrade(uint8 * slot, int32 slot_count)
{
	int32 ItemCount[MAX_EARRING_UPGRADE_MATERIAL];
	for (int32 i = 0; i < MAX_EARRING_UPGRADE_MATERIAL; ++i)
	{
		ItemCount[i] = 0;
	}

	uint8 ItemSlot = -1;
	uint8 ItemLevel = -1;

	for (int32 i = 0; i < slot_count; ++i)
	{
		if (!use_inventory_range(slot[i]))
		{
			continue;
		}

		Item * item = this->GetPlayer()->GetInventory()->GetItem(slot[i]);

		if (!item)
		{
			continue;
		}

		if (!item->IsItem())
		{
			continue;
		}

		if (item->IsEarring())
		{
			ItemSlot = slot[i];
			ItemLevel = item->GetLevel();
		}
	}

	if (!use_inventory_range(ItemSlot) || ItemLevel == uint8(-1))
	{
		this->ChaosMixLogInvalidMix(251);
		return;
	}

	if (ItemLevel >= MAX_ITEM_LEVEL)
	{
		this->ChaosMixLogInvalidMix(251);
		return;
	}

	item_earring_upgrade const* pData = sItemMgr->GetEarringUpgrade(ItemLevel);

	if (!pData)
	{
		this->ChaosMixLogInvalidMix(251);
		return;
	}

	for (int32 i = 0; i < slot_count; ++i)
	{
		if (!use_inventory_range(slot[i]))
		{
			continue;
		}

		Item * item = this->GetPlayer()->GetInventory()->GetItem(slot[i]);

		if (!item)
		{
			continue;
		}

		if (!item->IsItem())
		{
			continue;
		}

		for (int32 n = 0; n < MAX_EARRING_UPGRADE_MATERIAL; ++n)
		{
			if (pData->GetAmount(n) <= 0)
			{
				continue;
			}

			if (pData->GetItem(n) == item->GetItem())
			{
				item_template const* pItemInfo = sItemMgr->GetItem(item->GetItem());

				if (pItemInfo)
				{
					if (pItemInfo->GetStackData()->GetMaxStack() > 0)
					{
						ItemCount[n] += item->GetDurability();
					}
					else
					{
						ItemCount[n]++;
					}
				}
			}
		}
	}

	for (int32 n = 0; n < MAX_EARRING_UPGRADE_MATERIAL; ++n)
	{
		if (ItemCount[n] < pData->GetAmount(n))
		{
			this->ChaosMixLogInvalidMix(251);
			return;
		}
	}

	Item* pEarring = this->GetPlayer()->GetInventory()->GetItem(ItemSlot);

	for (int32 n = 0; n < MAX_EARRING_UPGRADE_MATERIAL; ++n)
	{
		ItemCount[n] = pData->GetAmount(n);
	}

	this->success_rate.set(pData->GetRate());
	this->SetMaxSuccessRate(1000000);
	this->mix_price = pData->GetPrice();

	if (!this->tributeTaxPrice(true, 3))
	{
		return;
	}

	this->ChaosMixDebugMessage();

	for (int32 i = 0; i < slot_count; ++i)
	{
		if (!use_inventory_range(slot[i]))
		{
			continue;
		}

		Item * item = this->GetPlayer()->GetInventory()->GetItem(slot[i]);

		if (!item)
		{
			continue;
		}

		if (!item->IsItem())
		{
			continue;
		}

		if (ItemSlot == slot[i])
		{
			continue;
		}

		bool founded = false;

		for (int32 n = 0; n < MAX_EARRING_UPGRADE_MATERIAL; ++n)
		{
			if (pData->GetAmount(n) <= 0)
			{
				continue;
			}

			if (pData->GetItem(n) == item->GetItem())
			{
				founded = true;

				if (ItemCount[n] > 0)
				{
					item_template const* pItemInfo = sItemMgr->GetItem(item->GetItem());

					if (pItemInfo)
					{
						if (pItemInfo->GetStackData()->GetMaxStack() > 0)
						{
							if (item->GetDurability() > ItemCount[n])
							{
								item->AlterDurability(0, ItemCount[n]);
								this->GetPlayer()->SendItemDurability(slot[i], 1);
								ItemCount[n] = 0;
							}
							else
							{
								ItemCount[n] -= item->GetDurability();
								this->GetPlayer()->ClearItem(slot[i]);
							}
						}
						else
						{
							ItemCount[n]--;
							this->GetPlayer()->ClearItem(slot[i]);
						}
					}
				}
			}
		}

		if (!founded)
		{
			this->GetPlayer()->ClearItem(slot[i]);
		}
	}

	if (this->roll_success())
	{
		pEarring->IncreaseLevel(1);
		pEarring->AlterDurability(3, 0.0f);
		pEarring->Convert();

		this->GetPlayer()->SendInventory(ItemSlot);

		this->GetPlayer()->GetInterfaceState()->SetState(0);
		this->GetPlayer()->EarringUpgradeResult(0);

		this->ChaosMixLogSuccess();
	}
	else
	{
		if (ItemLevel > 0)
		{
			pEarring->ReduceLevel(1);
			pEarring->AlterDurability(3, 0.0f);
			pEarring->Convert();

			this->GetPlayer()->SendInventory(ItemSlot);
		}

		this->ChaosMixFailInit(0, false, 0, 5);
		this->GetPlayer()->EarringUpgradeResult(1);
	}

	this->ChaosMixDBSave();
}

void MixHandler::PetTrainerMix(uint8 type, uint8 id, uint8 main_slot, uint8 * slot, uint8 count)
{
	/*if (sMixMgr->IsMixDisabled(5, id))
	{
		this->ChaosMixLogInvalidMix(252);
		return;
	}*/

	/*PetTrainerMixHandler const* mixhandle = GetPetTrainerMix(type, id);

	if (!mixhandle)
	{
		this->ChaosMixLogInvalidMix(252);
		return;
	}*/

	if (!use_inventory_range(main_slot))
	{
		this->GetPlayer()->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "%s => 1", __FUNCTION__);

		this->ChaosMixLogInvalidMix(252);
		return;
	}

	if (!this->GetPlayer()->GetInventory()->GetItem(main_slot)->IsItem())
	{
		this->GetPlayer()->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "%s => 2", __FUNCTION__);

		this->ChaosMixLogInvalidMix(252);
		return;
	}

	if (this->GetPlayer()->GetInterfaceState()->GetID() != InterfaceData::PetTrainer)
	{
		this->GetPlayer()->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "%s => 3", __FUNCTION__);

		this->GetPlayer()->SendNotice(CUSTOM_MESSAGE_ID_RED, "Wrong Mix Interface. Contact Administrators");
		sLog->outError(LOG_CHAOS_MIX, "Wrong Mix Interface. %s - Mix: %d [%s], Mix Interface: %s, Current Interface: %s",
			this->GetPlayer()->BuildLog().c_str(), id, __FUNCTION__, InterfaceData::PetTrainer.c_str(), this->GetPlayer()->GetInterfaceState()->GetID().c_str());

		this->ChaosMixLogInvalidMix(252);
		return;
	}

	if (this->GetPlayer()->GetInterfaceState()->GetState() == 1)
	{
		return;
	}

	MixGuardian const* pMixGuardian = sMixMgr->GetGuardianMix(type, id);

	if (!pMixGuardian)
	{
		this->GetPlayer()->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "%s => 4", __FUNCTION__);

		this->ChaosMixLogInvalidMix(252);
		return;
	}

	this->GetPlayer()->GetInterfaceState()->SetState(1);
	this->SetMixName(pMixGuardian->GetName());
	this->SetMixID(id);
	this->SetSuccess(false);
	this->GetPlayer()->SetMixCompleted(false);
	this->SetDBFlag(0);
	this->ResetItemDataBefore();
	this->ResetItemDataAfter();

	for (int32 i = 0; i < count; ++i)
	{
		if (!use_inventory_range(slot[i]))
		{
			continue;
		}

		Item * item = this->GetPlayer()->GetInventory()->GetItem(slot[i]);

		if (!item->IsItem())
		{
			continue;
		}

		this->ChaosMixLogItem(item, slot[i]);

		if (this->GetItemDataBefore().empty())
		{
			this->SetItemDataBefore("[");
		}
		else
		{
			this->SetItemDataBefore(this->GetItemDataBefore() + ",");
		}

		this->SetItemDataBefore(this->GetItemDataBefore() + item->BuildLogDB(slot[i], false));
	}

	if (!this->GetItemDataBefore().empty())
	{
		this->SetItemDataBefore(this->GetItemDataBefore() + "]");
	}

	this->PetTrainerGuardianResurrection(type, id, main_slot, slot, count);
}

void MixHandler::PetTrainerGuardianResurrection(uint8 type, uint8 id, uint8 main_slot, uint8 * slot, int32 slot_count)
{
	MixGuardian const* pMixGuardian = sMixMgr->GetGuardianMix(type, id);

	if (!pMixGuardian)
	{
		this->GetPlayer()->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "%s => 1", __FUNCTION__);

		this->ChaosMixLogInvalidMix(252);
		return;
	}

	Item* pMainItem = this->GetPlayer()->GetInventory()->GetItem(main_slot);

	item_template const* pItemInfo = sItemMgr->GetItem(pMixGuardian->GetMainItem());
	int32 option_count = 0;

	if (pItemInfo)
	{
		if (pMixGuardian->GetMainItem() != pMainItem->GetItem() || pMixGuardian->GetMainItemLevel() != pMainItem->GetLevel())
		{
			this->GetPlayer()->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "%s => 2", __FUNCTION__);

			this->ChaosMixLogInvalidMix(252);
			return;
		}

		if (pItemInfo->GetStackData()->GetMaxStack() > 0 && pMainItem->GetDurability() <= 0)
		{
			this->ChaosMixLogInvalidMix(252);
			return;
		}

		if (pItemInfo->GetKind2() == ItemKind::GUARDIAN_MOUNT)
		{
			option_count = pMainItem->GetSocketCount();
		}
	}
	else
	{
		if (pMainItem->GetLevel() != id)
		{
			this->ChaosMixLogInvalidMix(252);
			return;
		}
	}

	for (int32 i = 0; i < MAX_GUARDIAN_MIX_MATERIAL; ++i)
	{
		this->SetJewelItem(i, pMixGuardian->GetMaterialItem(i));
		this->SetMaxJewelCount(i, pMixGuardian->GetMaterialAmount(i));
	}

	if (!this->HaveJewels())
	{
		this->GetPlayer()->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "%s => 3", __FUNCTION__);

		this->ChaosMixLogInvalidMix(251);
		return;
	}

	this->success_rate.set(pMixGuardian->GetSuccessRate());
	this->mix_price = pMixGuardian->GetPrice();
	this->SetMaxSuccessRate(1000000);

	if (!this->tributeTaxPrice(true))
	{
		return;
	}

	this->ChaosMixDebugMessage();

	if (pItemInfo)
	{
		if (pItemInfo->GetStackData()->GetMaxStack() > 0)
		{
			pMainItem->AlterDurability(0, 1);

			if (pMainItem->GetDurability() <= 0)
			{
				this->GetPlayer()->ClearItem(main_slot);
			}
			else
			{
				this->GetPlayer()->SendItemDurability(main_slot, 1);
			}
		}
		else
		{
			if (pItemInfo->GetKind2() != ItemKind::GUARDIAN_MOUNT)
			{
				this->GetPlayer()->ClearItem(main_slot);
			}
		}
	}

	this->JewelConsume(slot, slot_count);

	if (this->roll_success())
	{
		if (pItemInfo && pItemInfo->GetKind2() == ItemKind::GUARDIAN_MOUNT)
		{
			this->GetPlayer()->ClearItem(main_slot);
		}

		pItemInfo = sItemMgr->GetItem(pMixGuardian->GetResultItem());

		if (pItemInfo)
		{
			Item item(pMixGuardian->GetResultItem());
			if (pItemInfo->GetKind2() == ItemKind::GUARDIAN_MOUNT && option_count >= 3)
			{
				if (pMixGuardian->GetResultItem() == ITEMGET(12, 483) && roll_chance_i(sGameServer->GetRareIceDragonRate()))
				{
					item.SetItem(ITEMGET(12, 484));
				}

				sItemMgr->GenerateGuardianOptions(&item, 2);
			}

			sItemMgr->ItemSerialCreateItem(this->GetPlayer(), serial_create_socket_mix, item);
		}
		else
		{
			pMainItem->IncreaseLevel(1);
			pMainItem->AlterDurability(3, 0);
			pMainItem->Convert(true);

			this->GetPlayer()->SendInventory(main_slot);

			this->GetPlayer()->GetInterfaceState()->SetState(0);
		}

		this->ChaosMixLogSuccess();

		this->GetPlayer()->SendPetTrainerMix1Result(type, 0);
	}
	else
	{
		if (pItemInfo && pItemInfo->GetKind2() == ItemKind::GUARDIAN_MOUNT)
		{
			pMainItem->SetLevel(0);
			pMainItem->SetState(ITEM_CHANGED);
			pMainItem->AlterDurability(3, 0);
			pMainItem->Convert(true);

			this->GetPlayer()->SendInventory(main_slot);
		}

		pItemInfo = sItemMgr->GetItem(pMixGuardian->GetResultItem());

		if (!pItemInfo)
		{
			switch (pMixGuardian->GetResultLevel())
			{
			case 0:
			{
			} break;

			case 1:
			{
					  pMainItem->ReduceLevel(1);
			} break;

			case 2:
			{
					  pMainItem->SetLevel(0);
			} break;
			}

			pMainItem->AlterDurability(3, 0);
			pMainItem->Convert(true);

			this->GetPlayer()->SendInventory(main_slot);
		}

		this->ChaosMixFailInit(0, false, type, 7);
	}

	this->ChaosMixDBSave();
}

void MixHandler::ChaosMixUpgradeLevel(uint8 data, uint8 level)
{
	uint32 jewel_chaos = 0;
	uint32 jewel_bless = 0;
	uint32 jewel_soul = 0;
	uint32 upgrade_item_count = 0;
	uint8 upgrade_item_slot = 0;
	uint32 talisman_assembly = 0;
	int32 ElementalChaosAmulet = 0;
	
	chaos_box_loop(i)
	{
		Item const& item = this->GetPlayer()->GetMixInventory()->item[i];

		if ( !item.IsItem() )
			continue;

		if ( item.GetItem() == JEWEL::CHAOS )
		{
			jewel_chaos += item.GetDurability();
		}
		else if ( item.GetItem() == JEWEL::BLESS )
		{
			jewel_bless += item.GetDurability();
		}
		else if ( item.GetItem() == JEWEL::SOUL )
		{
			jewel_soul += item.GetDurability();
		}
		else if ( item.GetItem() == ITEMGET(14, 96) )
		{
			talisman_assembly++;
		}
		else if( item.GetItem() == ITEMGET(14, 190) )
		{
			ElementalChaosAmulet++;
		}
		else if ( item.GetLevel() == (level + 9) )
		{
			upgrade_item_count++;
			upgrade_item_slot = i;
		}
	}

	if ( jewel_chaos < 1 || jewel_bless < (level + 1) || jewel_soul < (level + 1) || upgrade_item_count < 1 )
	{
		this->ChaosMixLogLackingMixItems();
		return;
	}

	if ( jewel_chaos > 1 || jewel_bless > (level + 1) || jewel_soul > (level + 1) || upgrade_item_count > 1 )
	{
		this->ChaosMixLogTooManyMixItems();
		return;
	}

	Item* pItem = this->GetPlayer()->GetMixInventory()->GetItem(upgrade_item_slot);

	if (pItem->IsPentagramItem())
	{
		if ( !this->ChaosMixTalismanOfLuck(true) )
		{
			this->ChaosMixLogTooManyTalismanOfLuck();
			return;
		}
	}
	else
	{
		if ( !this->ChaosMixTalismanOfLuck() )
		{
			this->ChaosMixLogTooManyTalismanOfLuck();
			return;
		}
	}

	this->mix_price = 2000000 * (level + 1);
	//this->mix_price = 4000000;
	//this->mix_price = 6000000;
	//this->mix_price = 8000000;
	//this->mix_price = 10000000;
	//this->mix_price = 12000000;

	if ( !this->tributeTaxPrice() )
	{
		return;
	}

	this->ChaosMixGetUpgradeItemSuccessRate(upgrade_item_slot);

	this->success_rate.op_add(this->GetLuckyCharm());
	this->success_rate.limit_min(0);
	this->success_rate.limit_max(100);

	this->ChaosMixDebugMessage();

	if (!talisman_assembly && !ElementalChaosAmulet)
	{
		pItem->common.talisman += 1;
	}

	if ( this->roll_success() )
	{
		pItem->IncreaseLevel(1);
		pItem->AlterDurability(3, 0.0f);
		pItem->Convert();

		uint8 item_buffer[MAX_ITEM_INFO];
		pItem->ConvertToBuffer(item_buffer);

		Item item = this->GetPlayer()->GetMixInventory()->item[upgrade_item_slot];
		
		this->GetPlayer()->GetInterfaceState()->SetState(0);
		this->GetPlayer()->GetMixInventory()->Clear();
		this->GetPlayer()->GetMixInventory()->AddItem(item, 0);
		this->GetPlayer()->ChaosMixResult(CHAOS_MIX_RESULT_SUCCESS, item_buffer, 0);
		this->ChaosMixLogSuccess();
	}
	else if ( talisman_assembly || ElementalChaosAmulet )
	{
		pItem->common.talisman = 0;
		
		this->GetPlayer()->GetInterfaceState()->SetState(0);
		this->ChaosMixItemDown(upgrade_item_slot);
		this->GetPlayer()->ChaosMixSend(0);
		this->GetPlayer()->ChaosMixResult(CHAOS_MIX_RESULT_FAIL);
		this->ChaosMixLogSuccess();
	}
	else
	{
		this->ChaosMixItemDown(upgrade_item_slot);

		sPentagramSystem->DelAllPentagramJewelInfo(this->GetPlayer(), pItem, 0);

		this->GetPlayer()->AddRecoveryMixItem(pItem);

		this->ChaosMixFailInit(CHAOS_MIX_RESULT_FAIL, false);
	}

	this->ChaosMixDBSave();
}

void MixHandler::ChaosMixLogItem(Item * item, uint8 slot)
{
	sLog->outInfo(LOG_CHAOS_MIX,"%s -- %s - %s ITEM -> %s",
		__FUNCTION__, this->GetMixName().c_str(), this->GetPlayer()->BuildLog().c_str(), item->BuildLog(slot).c_str());
}

void MixHandler::ChaosMixLogFinalItem(Item * item, uint8 slot)
{
	sLog->outInfo(LOG_CHAOS_MIX,"%s -- %s - %s ITEM -> %s",
		__FUNCTION__, this->GetMixName().c_str(), this->GetPlayer()->BuildLog().c_str(), item->BuildLog(slot).c_str());
}

void MixHandler::ChaosMixLogError(const char * log, ...)
{
	this->GetPlayer()->ChaosMixResult(CHAOS_MIX_RESULT_INCORRECT_MIX_ITEMS);
	this->GetPlayer()->GetInterfaceState()->SetState(0);

	ARG(buffer, log);

	sLog->outError(LOG_CHAOS_MIX,"%s - %s %s",
		this->GetMixName().c_str(), this->GetPlayer()->BuildLog().c_str(), buffer);
}

void MixHandler::ChaosMixLogInvalidMixItems(bool pentagram)
{
	if ( pentagram )
	{
		this->GetPlayer()->ChaosMixPentagramResult(251, 0);
	}
	else
	{
		this->GetPlayer()->ChaosMixResult(CHAOS_MIX_RESULT_INCORRECT_MIX_ITEMS);
	}

	this->GetPlayer()->GetInterfaceState()->SetState(0);

	sLog->outError(LOG_CHAOS_MIX,"%s - %s Invalid Mix Items",
		this->GetMixName().c_str(), this->GetPlayer()->BuildLog().c_str());
}

void MixHandler::ChaosMixLogLackingMixItems(bool pentagram)
{
	if ( pentagram )
	{
		this->GetPlayer()->ChaosMixPentagramResult(251, 0);
	}
	else
	{
		this->GetPlayer()->ChaosMixResult(CHAOS_MIX_RESULT_LACKING_MIX_ITEMS);
	}

	this->GetPlayer()->GetInterfaceState()->SetState(0);

	sLog->outError(LOG_CHAOS_MIX,"%s - %s Lacking Mix Items",
		this->GetMixName().c_str(), this->GetPlayer()->BuildLog().c_str());
}

void MixHandler::ChaosMixLogTooManyMixItems(bool pentagram)
{
	if ( pentagram )
	{
		this->GetPlayer()->ChaosMixPentagramResult(251, 0);
	}
	else
	{
		this->GetPlayer()->ChaosMixResult(CHAOS_MIX_RESULT_TOO_MANY_ITEMS);
	}

	this->GetPlayer()->GetInterfaceState()->SetState(0);

	sLog->outError(LOG_CHAOS_MIX,"%s - %s Too Many Mix Items",
		this->GetMixName().c_str(), this->GetPlayer()->BuildLog().c_str());
}

void MixHandler::ChaosMixLogInvalidMixItemLevel(bool pentagram)
{
	if ( pentagram )
	{
		this->GetPlayer()->ChaosMixPentagramResult(251, 0);
	}
	else
	{
		this->GetPlayer()->ChaosMixResult(CHAOS_MIX_RESULT_INVALID_ITEM_LEVEL);
	}

	this->GetPlayer()->GetInterfaceState()->SetState(0);

	sLog->outError(LOG_CHAOS_MIX, "%s - %s Invalid Item Level",
		this->GetMixName().c_str(), this->GetPlayer()->BuildLog().c_str());
}

void MixHandler::ChaosMixLogNotEnoughMixZen()
{
	sLog->outError(LOG_CHAOS_MIX, "%s - %s Not Enough Mix Zen",
		this->GetMixName().c_str(), this->GetPlayer()->BuildLog().c_str());
}

void MixHandler::ChaosMixLogTooManyTalismanOfLuck(bool pentagram)
{
	this->GetPlayer()->ChaosMixResult(CHAOS_MIX_RESULT_TALISMAN_OF_LUCK);
	this->GetPlayer()->GetInterfaceState()->SetState(0);

	sLog->outError(LOG_CHAOS_MIX, "%s - %s Too many talisman of luck",
		this->GetMixName().c_str(), this->GetPlayer()->BuildLog().c_str());
}

void MixHandler::ChaosMixLogInvalidMix(uint8 result)
{
	this->GetPlayer()->ChaosMixPentagramResult(result, 0);
	this->GetPlayer()->GetInterfaceState()->SetState(0);

	sLog->outError(LOG_CHAOS_MIX, "%s - %s Invalid Mix Result %u",
		this->GetMixName().c_str(), this->GetPlayer()->BuildLog().c_str(), result);
}

void MixHandler::ChaosMixLogSuccess()
{
	sLog->outInfo(LOG_CHAOS_MIX, "%s - %s Mix Success. Rate:%d, Cost:%d",
		this->GetMixName().c_str(), this->GetPlayer()->BuildLog().c_str(), success_rate.get(), mix_price);
}
	
void MixHandler::ChaosMixLogFail()
{
	sLog->outInfo(LOG_CHAOS_MIX,"%s - %s Mix Fail. Rate:%d, Cost:%d",
		this->GetMixName().c_str(), this->GetPlayer()->BuildLog().c_str(), success_rate.get(), mix_price);
}

void MixHandler::ChaosMixDebugMessage()
{
	if ( this->GetPlayer()->IsAdministrator() )
	{
		this->GetPlayer()->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "%s - Rate:%d, Cost:%d", this->GetMixName().c_str(), this->success_rate.get(), this->mix_price);
	}
}

void MixHandler::ChaosMixDBSave()
{
	SQLTransaction trans = MuLogDatabase.BeginTransaction();

	PreparedStatement* stmt = MuLogDatabase.GetPreparedStatement(MULOG_STATEMENT_MIX);
	stmt->setUInt16(sGameServer->GetServerCode());
	stmt->setUInt32(this->GetPlayer()->GetAccountData()->GetGUID());
	stmt->setString(this->GetPlayer()->GetAccountData()->GetAccount());
	stmt->setUInt32(this->GetPlayer()->GetGUID());
	stmt->setString(this->GetPlayer()->GetName());
	stmt->setString(this->GetPlayer()->BuildLogDB());
	stmt->setUInt16(this->GetMixID());
	stmt->setString(this->GetMixName());
	stmt->setInt32(this->success_rate.get());
	stmt->setUInt32(this->mix_price);
	stmt->setUInt8(uint8(this->IsSuccess()));
	stmt->setString(this->GetItemDataBefore());
	stmt->setString(this->GetItemDataAfter());

	trans->Append(stmt);

	MuLogDatabase.CommitTransaction(trans);

	this->ResetItemDataBefore();
	this->ResetItemDataAfter();
}

void MixHandler::ChaosMixItemDown()
{
	chaos_box_loop(i)
	{
		Item & item = this->GetPlayer()->GetMixInventory()->item[i];

		if ( !item.IsItem() )
			continue;

		if ( item.GetItem() == JEWEL::CHAOS ||
			 item.GetItem() == JEWEL::BLESS ||
			 item.GetItem() == JEWEL::SOUL ||
			 item.GetItem() == LUCKY_CHARM ||
			 item.GetItem() == ITEMGET(14, 189) )
		{
			this->GetPlayer()->GetMixInventory()->DeleteItem(i);
			continue;
		}

		if ( item.GetSkill() )
		{
			if ( RAND%2 == 0 )
			{
				item.SetSkill(0);
			}
		}

		if ( item.GetOption() )
		{
			if ( RAND%2 == 0 )
			{
				item.SetOption(item.GetOption() - 1);
			}
		}

		if ( item.GetLevel() )
		{
			item.SetLevel(Random<uint8>(item.GetLevel()));
		}

		item.AlterDurability(3, 0.0f);
		item.Convert();
	}
}

void MixHandler::ChaosMixArchangelItemDown()
{
	chaos_box_loop(i)
	{
		Item * item = this->GetPlayer()->GetMixInventory()->GetItem(i);

		if ( !item->IsItem() )
		{
			continue;
		}

		if ( item->GetLevel() >= 15 && item->GetOption() >= 4 )
		{
			item->SetLevel(0);
			item->AlterDurability(3, 0.0f);
			item->Convert();
		}
		else
		{
			this->GetPlayer()->GetMixInventory()->DeleteItem(i);
		}
	}
}

void MixHandler::ChaosMixEarringItemDown()
{
	chaos_box_loop(i)
	{
		Item * item = this->GetPlayer()->GetMixInventory()->GetItem(i);

		if (!item->IsItem())
		{
			continue;
		}

		if (item->IsEarring())
		{
			item->SetLevel(0);
			item->AlterDurability(3, 0.0f);
			item->Convert();
		}
		else
		{
			this->GetPlayer()->GetMixInventory()->DeleteItem(i);
		}
	}
}

void MixHandler::ChaosMixItemDown(uint8 slot)
{
	chaos_box_loop(i)
	{
		Item & item = this->GetPlayer()->GetMixInventory()->item[i];

		if ( !item.IsItem() )
		{
			continue;
		}

		if ( slot != i )
		{
			this->GetPlayer()->GetMixInventory()->DeleteItem(i);
			continue;
		}

		item.SetLevel(0);
		item.AlterDurability(3, 0.0f);
		item.Convert();
	}
}

void MixHandler::ChaosMixCondorFeatherItemDown()
{
	chaos_box_loop(i)
	{
		Item & item = this->GetPlayer()->GetMixInventory()->item[i];

		if ( !item.IsItem() )
		{
			continue;
		}

		if ( item.GetItem() == JEWEL::CHAOS || 
			 item.GetItem() == LUCKY_CHARM || 
			 item.GetItem() == JEWEL::CREATION || 
			 item.GetItem() == JEWEL::SOUL)
		{
			this->GetPlayer()->GetMixInventory()->DeleteItem(i);
			continue;
		}

		if ( item.GetSkill() )
		{
			if ( RAND%2 == 0 )
			{
				item.SetSkill(0);
			}
		}

		if ( item.GetOption() )
		{
			if ( RAND%2 == 0 )
			{
				item.SetOption(item.GetOption() - 1);
			}
		}

		if ( item.GetLevel() )
		{
			item.SetLevel(Random<uint8>(item.GetLevel()));
		}

		item.AlterDurability(3, 0.0f);
		item.Convert();
	}
}

void MixHandler::ChaosMixGarudaFeatherItemDown()
{
	chaos_box_loop(i)
	{
		Item & item = this->GetPlayer()->GetMixInventory()->item[i];

		if (!item.IsItem())
		{
			continue;
		}

		if (!item.IsWingLvl3())
		{
			this->GetPlayer()->GetMixInventory()->DeleteItem(i);
			continue;
		}

		if (item.GetSkill())
		{
			if (RAND % 2 == 0)
			{
				item.SetSkill(0);
			}
		}

		if (item.GetOption())
		{
			if (RAND % 2 == 0)
			{
				item.SetOption(item.GetOption() - 1);
			}
		}

		if (item.GetLevel())
		{
			item.SetLevel(Random<uint8>(item.GetLevel()));
		}

		item.AlterDurability(3, 0.0f);
		item.Convert();
	}
}

void MixHandler::ChaosMix380ItemDelete()
{
	chaos_box_loop(i)
	{
		Item * item = this->GetPlayer()->GetMixInventory()->GetItem(i);

		if ( !item->IsItem() )
		{
			continue;
		}

		if ( item->GetItem() == LUCKY_CHARM || 
			 item->GetItem() == JEWEL::HARMONY || 
			 item->GetItem() == JEWEL::GUARDIAN )
		{
			this->GetPlayer()->GetMixInventory()->DeleteItem(i);
		}
	}
}

void MixHandler::ChaosMixGetUpgradeItemSuccessRate(uint8 slot)
{
	Item const* item = this->GetPlayer()->GetMixInventory()->GetItem(slot);

	uint8 type = 0;

	if ( item->GetMaxSocket() > 0 )
	{
		type = 1;
	}
	else if ( item->GetKind3() == ItemKind3::PVP ||
			  item->IsExcellent() ||
			  item->IsAncient() )
	{
		type = 2;
	}

	if ( item->IsWingLvl1() ||
		 item->IsWingLvl2() ||
		 item->IsWingLvl3() ||
		 item->IsWingLvl4() ||
		 item->IsWingMonster() ||
		 item->IsBlessedArchangelWeapon() || 
		 item->IsArchangelWeapon() ||
		 item->IsSpecialWing() )
	{
		type = 0;
	}
	
	switch ( type )
	{
	case 0:
		{
			this->success_rate.set(this->GetMixID() == 3 ? 60:
								   this->GetMixID() == 4 ? 60:
								   this->GetMixID() == 22 ? 55:
								   this->GetMixID() == 23 ? 55:
								   this->GetMixID() == 49 ? 50:
								   50);

		} break;

	case 1:
		{
			this->success_rate.set(this->GetMixID() == 3 ? 40:
								   this->GetMixID() == 4 ? 40:
								   this->GetMixID() == 22 ? 35:
								   this->GetMixID() == 23 ? 35:
								   this->GetMixID() == 49 ? 30:
								   30);

		} break;

	case 2:
		{
			this->success_rate.set(this->GetMixID() == 3 ? 50:
								   this->GetMixID() == 4 ? 50:
								   this->GetMixID() == 22 ? 45:
								   this->GetMixID() == 23 ? 45:
								   this->GetMixID() == 49 ? 40:
								   40);

		} break;
	}

	if ( item->GetLuck() )
	{
		this->success_rate.op_add(25);
	}
}

void MixHandler::ChaosMixFailInit(uint8 result, bool commit, uint8 type, uint8 mix_kind)
{
	this->GetPlayer()->GetInterfaceState()->SetState(0);
	this->GetPlayer()->GetMixInventory()->Clear();

	switch ( mix_kind )
	{
	case 0:
		{
			this->GetPlayer()->ChaosMixSend(type);
			this->GetPlayer()->ChaosMixResult(result);
		} break;

	case 1:
		{
			this->GetPlayer()->ChaosMixPentagramResult(0, 0);
		} break;

	case 2:
		{
			this->GetPlayer()->ChaosMixSocketResult(0, 0);
		} break;

	case 3:
		{
			this->GetPlayer()->WingSocketOptionAddResult(2);
		} break;

	case 7:
		{
			  this->GetPlayer()->SendPetTrainerMix1Result(type, 1);
		} break;
	}

	this->ChaosMixLogFail();
}

bool MixHandler::ChaosMixTalismanOfLuck(bool elemental)
{
	this->SetLuckyCharm(0);

	chaos_box_loop(i)
	{
		Item const* item = this->GetPlayer()->GetMixInventory()->GetItem(i);

		if ( !item->IsItem() )
		{
			continue;
		}

		if (elemental)
		{
			if (item->GetItem() == ITEMGET(14, 189))
			{
				this->IncreaseLuckyCharm(10);
			}
		}
		else
		{
			if (item->GetItem() == ITEMGET(14, 53))
			{
				this->IncreaseLuckyCharm(item->GetDurability());
			}
		}
	}

	return this->GetLuckyCharm() <= 10;
}

bool MixHandler::roll_success()
{
	if ( this->GetPlayer() )
		this->GetPlayer()->SetMixCompleted(true);

	if ( this->GetMaxSuccessRate() <= 0 )
	{
		this->SetMaxSuccessRate(100);
	}

	this->SetSuccess(roll_chance_i(this->success_rate.get(), this->GetMaxSuccessRate()));

	return this->IsSuccess();
}

void MixHandler::GenerateMassCombinationResult(uint8 count, int32 success_rate, int32 max_success_rate)
{
	this->ResetMassCombinationList(0);
	uint8 item_info[MAX_ITEM_INFO];
	memset(item_info, 0, MAX_ITEM_INFO);

	this->success_rate.set(success_rate);
	this->success_rate.limit_min(0);
	this->success_rate.limit_max(max_success_rate);

	for ( int32 i = 0; i < count; ++i )
	{
		if ( this->roll_success() )
		{
			item_info[i / 8] |= (1 << (i % 8));

			this->SetMassCombinationList(i, 1);
			
			if ( this->GetPlayer()->IsAdministrator() )
			{
				this->GetPlayer()->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Mix OK");
			}
		}
		else
		{
			this->SetMassCombinationList(i, 0);

			if ( this->GetPlayer()->IsAdministrator() )
			{
				this->GetPlayer()->SendNotice(CUSTOM_MESSAGE_ID_RED, "Mix FAIL");
			}
		}
	}

	if ( count > 1 )
	{
		this->GetPlayer()->ChaosMixResult(CHAOS_MIX_RESULT_MASS_COMBINATION, item_info, count);
	}
}

void MixHandler::CalculateSpiritStone()
{
	this->ResetSpiritStone(0);

	chaos_box_loop(i)
	{
		Item const* pItem = this->GetPlayer()->GetMixInventory()->GetItem(i);

		if (!pItem || !pItem->IsItem())
		{
			continue;
		}

		switch (pItem->GetItem())
		{
		case ITEMGET(12, 404):
			{
				this->IncreaseSpiritStone(SPIRIT_STONE_LESSER, pItem->GetDurability() * sGameServer->GetMixSpellStoneRate(SPIRIT_STONE_LESSER));
			} break;

		case ITEMGET(12, 405):
			{
				this->IncreaseSpiritStone(SPIRIT_STONE_MEDIUM, pItem->GetDurability() * sGameServer->GetMixSpellStoneRate(SPIRIT_STONE_MEDIUM));
			} break;

		case ITEMGET(12, 406):
			{
				this->IncreaseSpiritStone(SPIRIT_STONE_GREATER, pItem->GetDurability() * sGameServer->GetMixSpellStoneRate(SPIRIT_STONE_GREATER));
			} break;
		}
	}
}

int32 MixHandler::GetSpiritStoneRate(uint32 flags) const
{
	int32 count = 0;

	if (flags & SPIRIT_STONE_FLAG_LESSER)
	{
		count += this->GetSpiritStone(SPIRIT_STONE_LESSER);
	}

	if (flags & SPIRIT_STONE_FLAG_MEDIUM)
	{
		count += this->GetSpiritStone(SPIRIT_STONE_MEDIUM);
	}

	if (flags & SPIRIT_STONE_FLAG_GREATER)
	{
		count += this->GetSpiritStone(SPIRIT_STONE_GREATER);
	}

	return count;
}