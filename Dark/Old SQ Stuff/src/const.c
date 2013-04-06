/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <time.h>
#include "merc.h"



/*
 * Class table.
 */
const	struct	class_type	class_table	[MAX_CLASS]	=
{
    {
	"none","           "
    },

    {
	"sorcerer","       "
    },

    {
	"fighter","        "
    },

    {
	"ranger","         "
    },

    {
	"fire-elementalist","           "
    },

    {
	"water-elementalist","         "
    },

    {
	"templar","        ",
        
    },

    {
	"assassin","       "
    },

    {
	"burglar","        "
    },

    {
	"immortal","            "
    },

    {
	"stone-elementalist","          "
    },

    {
	"energy-elementalist","         "
    },

    {
	"dark-elementalist","      "
    },

    {
	"void-elementalist","        "
    },

    {
	"psionist","        "
    },

    {
	"merchant","         "
    }
};


/*
 * Clan names
 */
const	char	*clan_table	[MAX_CLAN]	=
{
    "no clan",
    "Veiled Aliance",
    "Urik Templarate",
    "Tyr Templarate",
    "clan04",
    "clan05",
    "clan06",
    "clan07",
    "clan08",
    "clan09",
    "clan10"
};



/*
 * Liquid properties.
 * Used in world.obj.
 */
const	struct	liq_type	liq_table	[LIQ_MAX]	=
{
    { "water",			"clear",	{  0, 1, 10 }	},  /*  0 */
    { "beer",			"amber",	{  3, 2,  5 }	},
    { "wine",			"purple",		{  5, 2,  5 }	},
    { "ale",			"brown",	{  2, 2,  5 }	},
    { "dark ale",		"dark",		{  1, 2,  5 }	},

    { "whisky",			"golden",	{  6, 1,  4 }	},  /*  5 */
    { "lemonade",		"pink",		{  0, 1,  8 }	},
    { "firebreather",		"boiling",	{ 10, 0,  0 }	},
    { "local specialty",	"everclear",	{  3, 3,  3 }	},
    { "slime mold juice",	"green",	{  0, 4, -8 }	},

    { "milk",			"white",	{  0, 3,  6 }	},  /* 10 */
    { "tea",			"tan",		{  0, 1,  6 }	},
    { "coffee",			"black",	{  0, 1,  6 }	},
    { "blood",			"red",		{  0, 2, -1 }	},
    { "salt water",		"clear",	{  0, 1, -2 }	},

    { "cola",			"cherry",	{  0, 1,  5 }	}   /* 15 */
};


const sh_int sunlight_table[25] =
{
    -100,
    -100,
    -100,
     -80,
     -40,
       0,		/* 5 am */
      20,
      60,
     100,
     140,
     180,
     220,
     250,		/* 12 noon */
     220,
     180,
     140,
     100,
      60,
      20,
       0,		/* 7 am */
     -40,
     -80,
    -100,
    -100,
    -100
};



const	struct	profi_type	profi_table	[MAX_PROFI]	=
{
    {
	"undefined",
	{ 0, 0, 0, 0 },
	{ NULL, NULL, NULL, NULL },
	NULL
    },

    {
	"dagger",
	{ 1, 1, 1, 1 },
	{ &wpr_knife, &wpr_sword_short, &wpr_spear_short, NULL },
	&wpr_dagger
    },

    {
	"club",
	{ 2, 2, 2, 2 },
	{ &wpr_half_staff, &wpr_staff, &wpr_club_heavy, &wpr_hammer },
	&wpr_club
    },

    {
	"sword, long",
	{ 3, 3, 3, 3 },
	{ &wpr_sword_broad, &wpr_sword_bastard, &wpr_half_staff, &wpr_sword_short },
	&wpr_sword_long
    },

    {
	"knife",
	{ 3, 1, 1, 1 },
	{ &wpr_dagger, &wpr_sword_short, NULL, NULL },
	&wpr_knife
    },

    {
 	"sword, short",
	{ 3, 3, 1, 1 },
	{ &wpr_knife, &wpr_dagger, &wpr_sword_long, &wpr_spear_short },
	&wpr_sword_short
    },

    {
	"scimitar",
	{ 3, 3, 3, 1 },
	{ &wpr_sword_broad, NULL, NULL, NULL },
	&wpr_scimitar
    },

    {
	"sword, broad",
	{ 3, 3, 3, 3 },
	{ &wpr_scimitar, &wpr_sword_long, &wpr_sword_bastard, NULL },
	&wpr_sword_broad
    },

    {
	"sword, bastard",
	{ 3, 3, 3, 3 },
	{ &wpr_sword_broad, &wpr_sword_long, &wpr_sword_two_handed, NULL },
	&wpr_sword_bastard
    },

    {
	"sword, two handed",
	{ 3, 3, 3, 3 },
	{ &wpr_sword_bastard, &wpr_club_heavy, NULL, NULL },
	&wpr_sword_two_handed
    },

    {
	"club, heavy",
	{ 2, 2, 2, 2 },
	{ &wpr_sword_two_handed, &wpr_club, &wpr_hammer, NULL },
	&wpr_club_heavy
    },

    {
	"hammer",
	{ 2, 2, 2, 2 },
	{ &wpr_club_heavy, &wpr_club, &wpr_pole_arms, &wpr_flail },
	&wpr_hammer
    },

    {
	"staff",
	{ 2, 2, 2, 2 },
	{ &wpr_half_staff, &wpr_club, NULL, NULL },
	&wpr_staff
    },

    {
	"half staff",
	{ 2, 2, 2, 3 },
	{ &wpr_staff, &wpr_sword_long, &wpr_club, NULL },
	&wpr_half_staff
    },

    {
	"spear, short",
	{ 2, 1, 1, 1 },
	{ &wpr_dagger, &wpr_sword_short, &wpr_spear_long, NULL },
	&wpr_spear_short
    },

    {
	"spear, long",
	{ 2, 2, 1, 1 },
	{ &wpr_spear_short, &wpr_pole_arms, NULL, NULL },
	&wpr_spear_long
    },

    {
	"pole arms",
	{ 1, 2, 3, 1 },
	{ &wpr_spear_long, &wpr_hammer, NULL, NULL },
	&wpr_pole_arms
    },

    {
	"flail",
	{ 2, 2, 2, 2 },
	{ &wpr_hammer,  NULL, NULL, NULL },
	&wpr_flail
    },

    {
	"bow, short",
	{ 0, 0, 0, 0 },
	{ &wpr_blowgun, &wpr_bow_long, &wpr_bow_cross, NULL },
	&wpr_bow_short
    },

    {
	"bow, long",
	{ 0, 0, 0, 0 },
	{ &wpr_bow_short, &wpr_bow_cross, NULL, NULL },
	&wpr_bow_long
    },

    {
	"blowgun",
	{ 0, 0, 0, 0 },
	{ &wpr_bow_short, NULL, NULL, NULL },
	&wpr_blowgun
    },

    {
	"bow, cross",
	{ 0, 0, 0, 0 },
	{ &wpr_bow_short, &wpr_bow_long, &wpr_bow_cross_heavy, NULL },
	&wpr_bow_cross
    },

    {
	"bow, cross, heavy",
	{ 0, 0, 0, 0 },
	{ &wpr_bow_cross, NULL, NULL, NULL },
	&wpr_bow_cross_heavy
    },

    {
	"thrown, dart",
	{ 0, 0, 0, 0 },
	{ &wpr_thrown_knife, NULL, NULL, NULL },
	&wpr_thrown_dart
    },

    {
	"thrown, knife",
	{ 0, 0, 0, 0 },
	{ &wpr_thrown_dart, &wpr_thrown_spear, NULL, NULL },
	&wpr_thrown_knife
    },

    {
	"thrown, spear",
	{ 0, 0, 0, 0 },
	{ &wpr_thrown_knife, &wpr_thrown_boomerang, NULL, NULL },
	&wpr_thrown_spear
    },

    {
	"thrown, boomerang",
	{ 0, 0, 0, 0 },
	{ &wpr_thrown_boomerang, NULL, NULL, NULL },
	&wpr_thrown_boomerang
    }
};


/*
 * The skill and spell table.
 * Slot numbers must never be changed as they appear in #OBJECTS sections.
 */
#define SLOT(n)	n

const	struct	skill_type	skill_table	[MAX_SKILL]	=
{

/*
 * Magic spells.
 */

    {
	"reserved",
	0,			TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT( 0),	 0,	0,	 0,
	"reserved",		"",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	0, 0, 0, 0, 0,
	NULL, NULL,
	0
    },

    {
	"armor",
	spell_armor,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_armor,		SLOT( 1),	5,	35,	12,
	"grey",			"You feel less protected.",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	50, 99, 2, 100, 60,
	NULL, NULL,
	SKILL_SPELL
    },

    {
	"bless",
	spell_bless,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_bless,		SLOT( 3),	5,	50,	12,
	"white",		"You feel less righteous.",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	40, 99, 2, 100, 70,
	NULL, NULL,
	SKILL_SPELL
    },

    {
	"blindness",
	spell_blindness,	TAR_CHAR_HINDRANCE,	POS_FIGHTING,
	&gsn_blindness,		SLOT( 4),	 5,	100,	12,
	"black",		"You can see again.",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	15, 95, 4, 100, 80,
	NULL, &gsn_cure_blindness,
	SKILL_SPELL
    },

   /* {
	"call lightning",
	spell_call_lightning,	TAR_IGNORE,		POS_FIGHTING,
	&gsn_call_lightning,	SLOT( 6),	15,	100,	12,
	"neon blue",		"!Call Lightning!",
	"skill_death_damage_to_char",
	"skill_death_damage_to_vict",
	"skill_death_damage_to_room",
	"skill_damage_to_char",
	"skill_damage_to_vict",
	"skill_damage_to_room",
	"skill_miss_to_char",
	"skill_miss_to_vict",
	"skill_miss_to_room",
	20, 95, 5, 50, 85,
	NULL, &gsn_lightning_bolt,
	SKILL_SPELL
    },*/

    {
	"charm person",
	spell_charm_person,	TAR_CHAR_HINDRANCE,	POS_STANDING,
	&gsn_charm_person,	SLOT( 7),	 33,	100,	12,
	"charm person",		"You feel more self-confident.",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	15, 95, 10, 100, 90,
	NULL, NULL,
	SKILL_SPELL
    },

    {
	"control weather",
	spell_control_weather,	TAR_IGNORE,		POS_STANDING,
	&gsn_control_weather,	SLOT(11),	25,	100,	12,
	"control weather",	"!Control Weather!",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	20, 95, 4, 100, 60,
	NULL, &gsn_call_lightning,
	SKILL_SPELL
    },

    {
	"create food",
	spell_create_food,	TAR_IGNORE,		POS_STANDING,
	&gsn_create_food,	SLOT(12),	 5,	100,	12,
	"create food",		"!Create Food!",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	50, 99, 2, 100, 65,
	NULL, NULL,
	SKILL_SPELL
    },

    {
	"create water",
	spell_create_water,	TAR_OBJ_INV,		POS_STANDING,
	&gsn_create_water,	SLOT(13),	 5,	100,	12,
	"create_water",		"!Create Water!",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	50, 99, 2, 100, 75,
	NULL, NULL,
	SKILL_SPELL
    },

    {
	"cure blindness",
	spell_cure_blindness,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	&gsn_cure_blindness,	SLOT(14),	 5,	30,	12,
	"green",		"!Cure Blindness!",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	40, 95, 2, 100, 50,
	NULL, NULL,
	SKILL_SPELL
    },

    {
	"cure poison",
	spell_cure_poison,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_cure_poison,	SLOT(43),	 5,	50,	12,
	"burgundy",		"!Cure Poison!",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	40, 95, 4, 100, 60,
	NULL, &gsn_detect_poison,
	SKILL_SPELL
    },

    {
	"curse",
	spell_curse,		TAR_CHAR_HINDRANCE,	POS_STANDING,
	&gsn_curse,		SLOT(17),	20,	50,	12,
	"curse",		"The curse wears off.",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	20, 95, 5, 100, 80,
	NULL, &gsn_bless,
	SKILL_SPELL
    },

    {
	"detect evil",
	spell_detect_evil,	TAR_CHAR_SELF,		POS_STANDING,
	&gsn_detect_evil,	SLOT(18),	 5,	50,	12,
	"beige",		"The red in your vision disappears.",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	35, 95, 3, 100, 40,
	NULL, NULL,
	SKILL_SPELL
    },

    {
	"sense life",
	spell_sense_life,	TAR_CHAR_SELF,		POS_STANDING,
	&gsn_sense_life,	SLOT(44),	 20,	100,	12,
	"opal",			"You feel less aware of your suroundings.",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	30, 95, 5, 100, 90,
	NULL, &gsn_detect_invis,
	SKILL_SPELL
    },

    {
	"detect invis",
	spell_detect_invis,	TAR_CHAR_SELF,		POS_STANDING,
	&gsn_detect_invis,	SLOT(19),	 10,	100,	12,
	"yellow",		"You no longer see invisible objects.",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	30, 95, 2, 100, 80,
	NULL, NULL,
	SKILL_SPELL
    },

    {
	"detect magic",
	spell_detect_magic,	TAR_CHAR_SELF,		POS_STANDING,
	&gsn_detect_magic,	SLOT(20),	 10,	50,	12,
	"amber",		"The detect magic wears off.",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	50, 95, 3, 100, 50,
	NULL, &gsn_detect_invis,
	SKILL_SPELL
    },

    {
	"detect poison",
	spell_detect_poison,	TAR_OBJ_INV,		POS_STANDING,
	&gsn_detect_poison,	SLOT(21),	 5,	25,	12,
	"detect poison",	"!Detect Poison!",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	30, 95, 2, 100, 60,
	NULL, NULL,
	SKILL_SPELL
    },

    {
	"dispel evil",
	spell_dispel_evil,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_dispel_evil,	SLOT(22),	15,	50,	12,
	"dispel evil",		"!Dispel Evil!",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	20, 95, 5, 100, 70,
	NULL, &gsn_protection_from_evil,
	SKILL_SPELL
    },

    {
	"dispel magic",
	spell_dispel_magic,	TAR_CHAR_HINDRANCE,	POS_STANDING,
	&gsn_dispel_magic,	SLOT(59),	35,	100,	12,
	"dull",			"!Dispel Magic!",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	10, 95, 5, 80, 90,
	NULL, &gsn_detect_magic,
	SKILL_SPELL
    },

    {
	"earthquake",
	spell_earthquake,	TAR_IGNORE,		POS_STANDING,
	&gsn_earthquake,	SLOT(23),	15,	100,	12,
	"earthquake",		"!Earthquake!",
	"$B$N$b is crushed as a fissure from your earthquake opens beneath $M.",
	"$BYou$b are swallowed by a fissure created by $n's earthquake.",
	"$B$N$b is crushed as a fissure from $n's earthquake opens beneath $M.",
	"",
	"",
	"",
	"The ground rumbles underneath you",
	"The ground rumbles underneath you",
	"The ground rumbles underneath you",
	20, 95, 5, 50, 80,
	NULL, &gsn_pebble,
	SKILL_SPELL
    },

    {
	"enchant weapon",
	spell_enchant_weapon,	TAR_OBJ_INV,		POS_STANDING,
	&gsn_enchant_weapon,	SLOT(24),	50,	100,	24,
	"enchant weapon",	"!Enchant Weapon!",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	15, 95, 10, 65, 90,
	NULL, &gsn_dispel_magic,
	SKILL_SPELL
    },

    {
	"vampiric touch",
	spell_vampiric_touch,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_vampiric_touch,	SLOT(25),	35,	100,	12,
	"vampiric touch",	"!Vampiric Touch!",
	"$N is shiveled completely as you suck the remaining life from $S body.",
	"skill_death_damage_to_vict",
	"skill_death_damage_to_room",
	"skill_damage_to_char",
	"skill_damage_to_vict",
	"skill_damage_to_room",
	"skill_miss_to_char",
	"skill_miss_to_vict",
	"skill_miss_to_room",
	20, 95, 10, 50, 95,
	NULL, &gsn_harm,
	SKILL_SPELL
    },

    {
	"fireball",
	spell_fireball,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_fireball,		SLOT(26),	15,	50,	12,
	"golden",		"!Fireball!",
	"Your fireball hits $B$N$b with full force, causing an immediate death!",
	"$BYou$b burst into flames!",
	"The heat from a fireball from $n turns $B$N$b into a charred corpse!",
	"You throw a fireball at $B$N$b",
	"$BYou$b are enveloped in flames from a fireball!",
	"$n hurls a fireball at $B$N$b!",
	"Your fireball singes $B$N$b.",
	"$n throws a fireball at $BYou$b but it only singes you.",
	"$n throws a fireball at $B$N$b but it only singes $M.",
	20, 95, 3, 50, 60,
	NULL, NULL,
	SKILL_SPELL
    },

    {
	"flash",
	spell_flash,		TAR_IGNORE,	POS_STANDING,
	&gsn_flash,		SLOT(300),	10,	75,	18,
	"flash",		"!Flash!",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	20, 95, 5, 100, 70,
	NULL, NULL,
	SKILL_SPELL
    },

    {
	"fly",
	spell_fly,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_fly,		SLOT(56),	10,	75,	18,
	"fly",			"You slowly float to the ground.",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	20, 95, 5, 100, 70,
	NULL, NULL,
	SKILL_SPELL
    },

    {
	"gate",
	spell_gate,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_gate,		SLOT(83),	50,	100,	12,
	"gate",			"!Gate!",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	10, 95, 10, 100, 95,
	NULL, &gsn_teleport,
	SKILL_SPELL
    },

    {
	"strength",
	spell_strength,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_strength,		SLOT(39),	33,	100,	12,
	"silver",		"You feel weaker.",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	25, 95, 4, 100, 75,
	NULL, NULL,
	SKILL_SPELL
    },

    {
	"harm",
	spell_harm,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_harm,		SLOT(27),	40,	100,	12,
	"harm",			"!Harm!",
	"$B$N$b is obliterated by your blast of energy",
	"You black out as $n's energy hits $Byou$b.",
	"$B$N$b is obliterted by $n's energy blast.",
	"You focus your energy on $B$N$b and blast $M away from you.",
	"$n narrows $s eyes on $Byou$b, causing a blast of energy to knock you back.",
	"$n narrows $s eyes on $B$N$b, causing a blast of energy to knock $N back.",
	"Your focus of energy dissipates right before hitting $B$N$b",
	"$n's blast of energy dissipates right before hitting $Byou$b.",
	"$n's blast of energy dissipates right before hitting $B$N$b.",
	25, 95, 4, 50, 85,
	NULL, &gsn_heal,
	SKILL_SPELL
    },

    {
	"heal",
	spell_heal,		TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	&gsn_heal,		SLOT(28),	30,	50,	12,
	"blue",			"!Heal!",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	25, 95, 3, 75, 70,
	NULL, NULL,
	SKILL_SPELL
    },

    {
	"identify",
	spell_identify,		TAR_OBJ_INV,		POS_STANDING,
	&gsn_identify,		SLOT(53),	100,	200,	24,
	"identify",		"!Identify!",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	10, 95, 10, 50, 95,
	NULL, &gsn_detect_magic,
	SKILL_SPELL
    },

    {
	"infravision",
	spell_infravision,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_infravision,	SLOT(77),	 50,	100,	18,
	"infravision",		"The red in your vision disappears.",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	30, 95, 4, 100, 70,
	NULL, &gsn_detect_magic,
	SKILL_SPELL
    },

    {
	"invis",
	spell_invis,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_invis,		SLOT(29),	 25,	100,	12,
	"clear",		"You shimmer back into existance.",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	35, 95, 5, 100, 75,
	NULL, &gsn_detect_invis,
	SKILL_SPELL
    },

    {
	"lightning bolt",
	spell_lightning_bolt,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_lightning_bolt,	SLOT(30),	15,	50,	12,
	"neon blue",		"!Lightning Bolt!",
	"$B$N$b is shattered as a lightning bolt arcs from your hands.",
	"A burst of electricity shatters $Byou$b moments after appearing in $n's hands.",
	"$B$N$b is shattered as $n's lightning bolt arcs from $s hands.",
	"A burst of blue flame envelops $B$N$b as your lightning bolt crackles in the air.",
	"$BYou$b are burned by electricity as $n arcs a lightning bolt at you.",
	"$B$N$b is enveloped in blue flame as $n fries $m with a lightning bolt.",
	"skill_miss_to_char",
	"skill_miss_to_vict",
	"skill_miss_to_room",
	30, 95, 3, 50, 70,
	NULL, NULL,
	SKILL_SPELL
    },

    {
	"locate object",
	spell_locate_object,	TAR_IGNORE,		POS_STANDING,
	&gsn_locate_object,	SLOT(31),	100,	200,	18,
	"locate object",	"!Locate Object!",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	10, 95, 10, 50, 95,
	NULL, &gsn_identify,
	SKILL_SPELL
    },

    {
	"pebble",
	spell_pebble,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_pebble,		SLOT(32),	15,	50,	12,
	"pebble",		"!Pebble!",
	"You create a huge boulder above $B$N$b, utterly crushing $M as it falls.",
	"$n creates a huge boulder above $Byou$b, utterly crushing you as it falls.",
	"$n creates a huge boulder above $B$N$b, crushing $M utterly.",
	"You create a stream of rocks which pummel into $B$N$b.",
	"Rocks drift up from the sand and pummel into $Byou$b!",
	"Rocks drift up from the sand and pummel into $B$N$b!",
	"skill_miss_to_char",
	"skill_miss_to_vict",
	"skill_miss_to_room",
	50, 99, 3, 50, 50,
	NULL, NULL,
	SKILL_SPELL
    },

    {
	"pass door",
	spell_pass_door,	TAR_CHAR_SELF,		POS_STANDING,
	&gsn_pass_door,		SLOT(74),	50,	100,	12,
	"airy",			"You feel solid again.",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	10, 95, 5, 100, 80,
	NULL, &gsn_fly,
	SKILL_SPELL
    },

    {
	"poison",
	spell_poison,		TAR_CHAR_HINDRANCE,	POS_STANDING,
	&gsn_poison,		SLOT(33),	25,	100,	12,
	"poison",		"You feel less sick.",
	"Your body goes limp as the poison takes affect",
	"skill_death_damage_to_vict",
	"$n shivers uncontrollably and spits up blood, eyes rolling back into $s head.",
	"Your blood burns from the affect of the poison.",
	"skill_damage_to_vict",
	"$n shivers as $s face flushes momentarily.",
	"skill_miss_to_char",
	"skill_miss_to_vict",
	"skill_miss_to_room",
	15, 95, 5, 40, 85,
	NULL, &gsn_cure_poison,
	SKILL_SPELL
    },

    {
	"protection from evil",
	spell_protection_from_evil,	TAR_CHAR_SELF,		POS_STANDING,
	&gsn_protection_from_evil,	SLOT(34),	 25,	50,	12,
	"mauve",		"You feel less protected.",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	25, 95, 4, 100, 70,
	NULL, &gsn_detect_evil,
	SKILL_SPELL
    },

    {
	"vitality",
	spell_vitality,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_vitality,		SLOT(81),	25,	100,	18,
	"shiny",		"!Vitality!",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	35, 95, 4, 100, 90,
	&gsn_vitality, &gsn_heal,
	SKILL_SPELL
    },

    {
	"remove curse",
	spell_remove_curse,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_remove_curse,	SLOT(35),	 15,	50,	12,
	"remove curse",		"!Remove Curse!",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	30, 95, 5, 100, 60,
	NULL, &gsn_dispel_magic,
	SKILL_SPELL
    },

    {
	"sanctuary",
	spell_sanctuary,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_sanctuary,		SLOT(36),	40,	100,	12,
	"purple",		"The white aura around your body fades.",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	10, 95, 10, 100, 90,
	NULL, &gsn_stone_skin,
	SKILL_SPELL
    },

    {
	"sleep",
	spell_sleep,		TAR_CHAR_HINDRANCE,	POS_STANDING,
	&gsn_sleep,		SLOT(38),	35,	100,	12,
	"sleep",		"You feel less tired.",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	10, 95, 6, 70, 90,
	NULL, NULL,
	SKILL_SPELL
    },

    {
	"stone skin",
	spell_stone_skin,	TAR_CHAR_SELF,		POS_STANDING,
	&gsn_stone_skin,	SLOT(66),	30,	80,	18,
	"stony",		"Your skin feels soft again.",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	10, 95, 4, 100, 85,
	NULL, &gsn_strength,
	SKILL_SPELL
    },

    {
	"summon",
	spell_summon,		TAR_IGNORE,		POS_STANDING,
	&gsn_summon,		SLOT(40),	100,	200,	12,
	"summon",		"!Summon!",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	10, 95, 10, 50, 95,
	NULL, &gsn_gate,
	SKILL_SPELL
    },

    {
	"teleport",
	spell_teleport,		TAR_CHAR_SELF,		POS_FIGHTING,
	&gsn_teleport, 		SLOT( 2),	65,	100,	12,
	"sickly red",		"!Teleport!",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	15, 95, 3, 100, 75,
	NULL, &gsn_infravision,
	SKILL_SPELL
    },

    {
	"ventriloquate",
	spell_ventriloquate,	TAR_IGNORE,		POS_STANDING,
	&gsn_ventriloquate,	SLOT(41),	 5,	30,	12,
	"ventriloquate",	"!Ventriloquate!",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	50, 95, 2, 100, 50,
	NULL, NULL,
	SKILL_SPELL
    },

    {
	"weaken",
	spell_weaken,		TAR_CHAR_HINDRANCE,	POS_FIGHTING,
	&gsn_weaken,		SLOT(68),	40,	60,	12,
	"weaken",		"You feel stronger.",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	25, 95, 4, 100, 70,
	NULL, &gsn_strength,
	SKILL_SPELL
    },

    {
	"word of recall",
	spell_word_of_recall,	TAR_CHAR_SELF,		POS_RESTING,
	&gsn_word_of_recall,	SLOT(42),	 100,	200,	12,
	"clear red",		"!Word of Recall!",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	10, 95, 5, 100, 85,
	NULL, &gsn_teleport,
	SKILL_SPELL
    },

/*
 * Dragon breath
 */
    {
	"acid breath",
	spell_acid_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_acid_breath,	SLOT(200),	 0,	0,	 4,
	"blast of acid",	"!Acid Breath!",
	"skill_death_damage_to_char",
	"skill_death_damage_to_vict",
	"skill_death_damage_to_room",
	"skill_damage_to_char",
	"skill_damage_to_vict",
	"skill_damage_to_room",
	"skill_miss_to_char",
	"skill_miss_to_vict",
	"skill_miss_to_room",
	0, 0, 0, 0, 0,
	NULL, NULL,
	0
    },

    {
	"fire breath",
	spell_fire_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_fire_breath,	SLOT(201),	 0,	 0,	4,
	"blast of flame",	"!Fire Breath!",
	"skill_death_damage_to_char",
	"skill_death_damage_to_vict",
	"skill_death_damage_to_room",
	"skill_damage_to_char",
	"skill_damage_to_vict",
	"skill_damage_to_room",
	"skill_miss_to_char",
	"skill_miss_to_vict",
	"skill_miss_to_room",
	0, 0, 0, 0, 0,
	NULL, NULL,
	0
    },

    {
	"frost breath",
	spell_frost_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_frost_breath,	SLOT(202),	 0,	0,	 4,
	"blast of frost",	"!Frost Breath!",
	"skill_death_damage_to_char",
	"skill_death_damage_to_vict",
	"skill_death_damage_to_room",
	"skill_damage_to_char",
	"skill_damage_to_vict",
	"skill_damage_to_room",
	"skill_miss_to_char",
	"skill_miss_to_vict",
	"skill_miss_to_room",
	0, 0, 0, 0, 0,
	NULL, NULL,
	0
    },

    {
	"gas breath",
	spell_gas_breath,	TAR_IGNORE,		POS_FIGHTING,
	&gsn_gas_breath,	SLOT(203),	 0,	 0,	4,
	"blast of gas",		"!Gas Breath!",
	"skill_death_damage_to_char",
	"skill_death_damage_to_vict",
	"skill_death_damage_to_room",
	"skill_damage_to_char",
	"skill_damage_to_vict",
	"skill_damage_to_room",
	"skill_miss_to_char",
	"skill_miss_to_vict",
	"skill_miss_to_room",
	0, 0, 0, 0, 0,
	NULL, NULL,
	0
    },

    {
	"lightning breath",
	spell_lightning_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_lightning_breath,	SLOT(204),	 0,	 0,	4,
	"blast of lightning",	"!Lightning Breath!",
	"skill_death_damage_to_char",
	"skill_death_damage_to_vict",
	"skill_death_damage_to_room",
	"skill_damage_to_char",
	"skill_damage_to_vict",
	"skill_damage_to_room",
	"skill_miss_to_char",
	"skill_miss_to_vict",
	"skill_miss_to_room",
	0, 0, 0, 0, 0,
	NULL, NULL,
	0
    },


/*
 * Psionist Skills
 */

    {
      "contact",
       spell_null,         TAR_IGNORE,         POS_RESTING,
       &gsn_contact,       SLOT( 0),      0,    0,     25,
       "",                 "!Psionist Profi!",
       NULL,
       NULL,
       NULL,
       NULL,
       NULL,
       NULL,
       NULL,
       NULL,
       NULL,
       0, 0, 0, 0, 0,
       NULL, NULL,
       SKILL_PSIONIST
     },
 
/*
 * Fighter and thief skills.
 */
    {
	"archery",
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_archery,		SLOT( 0),	 0,	 0,	0,
	"",			"!Archery Profi!",
	"An arrow $Byour$b skull, killing you instantly.",
	NULL,
	"An arrow strikes $B$N's$b skull, killing $M instantly.",
	"An arrow strikes $Byou$b.",
	NULL,
	"An arrow strikes $B$N$b.",
	"An arrow lands at your feet.",
	NULL,
	"An arrow lands at $N's feet.",
	10, 95, 10, 0, 0,
	NULL, NULL,
	SKILL_COMBAT
    },

    {
	"backstab",
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_backstab,		SLOT( 0),	 0,	0,	24,
	"backstab",		"!Backstab!",
	"$N crumples to the ground as you stab $p in $S back.",
	"You black out as a pain errupts in your chest.",
	"$n places $p in $N's back, causing $N to crumple to the ground.",
	"$N cries out as you stab $p in $S back.",
	"Pain errupts in your chest from $N's $p.",
	"$n stabs $p in $N's back, causing $N to cry out.",
	"$N quickly avoids your stab and steps to the side causing you to go flying past.",
	"$n tries to stab you in the back, but you step aside.",
	"$n tries to stab $N in the back, but $N steps aside.",
	10, 95, 15, 0, 0,
	NULL, &gsn_sneak,
	SKILL_COMBAT
    },

    {
	"bash",
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_bash,		SLOT( 0),	 0,	0,	24,
	"bash",			"!Bash!",
	"You bash $N's skull in, splitting it open.",
	"$n bashes your skull, and your skull splits open...",
	"$n bashes $N's skull, splitting it open and splattering blood all over.",
	"You send $N sprawling with a powerful bash.",
	"$n sends you sprawling with a powerful bash.",
	"$n sends $N sprawling with a powerful bash.",
	"As $N avoids your bash you topple to the ground.",
	"As you avoid $n's bash, $e topples to the ground.",
	"As $n topples to the ground as $N avoids his bash.",
	10, 95, 15, 0, 0,
	NULL, NULL,
	SKILL_COMBAT
    },

    {
	"climbing",
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_climbing,		SLOT( 0),	 0,	0,	0,
	"climbing",		"!Climbing!",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	10, 99, 15, 0, 0,
	NULL, NULL,
	SKILL_COMBAT
    },

    {
	"disarm",
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_disarm,		SLOT( 0),	 0,	0,	24,
	"",			"!Disarm!",
	"skill_death_damage_to_char",
	"skill_death_damage_to_vict",
	"skill_death_damage_to_room",
	"skill_damage_to_char",
	"skill_damage_to_vict",
	"skill_damage_to_room",
	"skill_miss_to_char",
	"skill_miss_to_vict",
	"skill_miss_to_room",
	10, 95, 15, 0, 0,
	NULL, NULL,
	SKILL_COMBAT
    },

    {
	"dodge",
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_dodge,		SLOT( 0),	 0,	 0,	0,
	"",			"!Dodge!",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	10, 85, 10, 0, 0,
	NULL, NULL,
	0
    },

    {
	"dual wield",
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_dual_wield,	SLOT( 0),	 0,	 0,	0,
	"",			"!Dual Wield!",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	10, 99, 0, 0, 0,
	NULL, NULL,
	0
    },

    {
	"enhanced damage",
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_enhanced_damage,	SLOT( 0),	 0,	 0,	0,
	"",			"!Enhanced Damage!",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	0, 0, 50, 0, 0,
	NULL, NULL,
	0
    },

    {
	"falling",
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_falling,		SLOT( 0),	 0,	 0,	0,
	"falling",		"!Falling!",
	"...and hit the ground terminally hard.",
	NULL,
	"...and hits the ground terminally hard.",
	"...and crash into the ground.",
	NULL,
	"...and crashes into the ground.",
	"...and land perfectly.",
	NULL,
	"...and lands perfectly.",
	10, 95, 0, 0, 0,
	NULL, NULL,
	0
    },

    {
	"hide",
	spell_null,		TAR_IGNORE,		POS_RESTING,
	&gsn_hide,		SLOT( 0),	 0,	0,	12,
	"",			"!Hide!",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	10, 99, 20, 0, 0,
	NULL, NULL,
	SKILL_STEALTH
    },

    {
	"kick",
	spell_null,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_kick,		SLOT( 0),	 0,	 0,	8,
	"kick",			"!Kick!",
	"You brutally kick $N in the temple, killing $M.",
	"You black out as $n delivers a hard kick to your temple.",
	"$n brutally kicks $N in the temple, killing $M.",
	"You land a solid kick in $N's gut.",
	"$n lands a solid kick in your gut.",
	"$n lands a solid kick in $N's gut.",
	"Your kick goes flying past $N, missing $M.",
	"$n aims a kick at your gut, but you step aside.",
	"$n aims a kick at $N's gut, but $N steps aside.",
	10, 95, 15, 0, 0,
	NULL, NULL,
	SKILL_COMBAT
    },

    {
	"peek",
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_peek,		SLOT( 0),	 0,	 0,	0,
	"",			"!Peek!",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	10, 99, 20, 0, 0,
	NULL, NULL,
	SKILL_HANDLING
    },

    {
	"pick lock",
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_pick_lock,		SLOT( 0),	 0,	0,	12,
	"",			"!Pick!",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	10, 95, 15, 0, 0,
	NULL, NULL,
	SKILL_HANDLING
    },

    {
	"read magic",
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_read_magic,	SLOT( 0),	 0,	0,	0,
	"",			"!Read Magic!",
	"You are turned to ashes as $p ignites in your hands.",
	NULL,
	"$n is turned to ashes as $p ignites in $s hands.",
	"You are singed as $p burns in your hands.",
	NULL,
	"$n is singed as $p burns in $s hands.",
	NULL,
	NULL,
	NULL,
	10, 90, 20, 0, 0,
	NULL, NULL,
	0
    },

    {
	"rescue",
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_rescue,		SLOT( 0),	 0,	0,	12,
	"",			"!Rescue!",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	10, 99, 12, 0, 0,
	NULL, NULL,
	SKILL_COMBAT
    },

    {
	"sneak",
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_sneak,		SLOT( 0),	 0,	0,	12,
	"",			NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	10, 99, 20, 0, 0,
	NULL, &gsn_hide,
	SKILL_STEALTH
    },

    {
	"steal",
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_steal,		SLOT( 0),	 0,	0,	24,
	"",			"!Steal!",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	10, 99, 15, 0, 0,
	NULL, &gsn_hide,
	SKILL_HANDLING
    },

    {
	"tracking",
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_tracking,		SLOT( 0),	 0,	0,	24,
	"",			"!Tracking!",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	10, 99, 30, 0, 0,
	NULL, NULL,
	0
    },

    {
	"tumbling",
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_tumbling,		SLOT( 0),	 0,	 0,	0,
	"tumbling",		"!Tumbling!",
	"...and hit the ground terminally hard.",
	NULL,
	"...and hits the ground terminally hard.",
	"...and clumsily roll on landing.",
	NULL,
	"...and clumsily rolls on landing.",
	"...and roll perfectly.",
	NULL,
	"...and roll perfectly.",
	10, 95, 0, 0, 0,
	NULL, NULL,
	0
    },

    {
	"use magical devices",
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_use_magical_devices, SLOT( 0),	 0,	0,	0,
	"",			"!Use Magical Devices!",
	"$p shatters into a million pieces, many of them impaling you thoroughly.",
	NULL,
	"$p shatters into a million pieces in $n's hands, many of them implaing $m.",
	"$p explodes in your hands.",
	NULL,
	"$p explodes in $n's hands.",
	NULL,
	NULL,
	NULL,
	10, 90, 20, 0, 0,
	NULL, NULL,
	0
    },


    {
	"martial arts",
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_martial_arts,	SLOT( 0),	 0,	 0,	0,
	"",			"!Martial Arts!",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	0, 0, 0, 0, 0,
	NULL, NULL,
	SKILL_COMBAT
    },

    {
	"pierce proficiency",
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_pierce_proficiency, SLOT( 0),	 0,	 0,	0,
	"",			"!Pierce Profi!",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	0, 0, 0, 0, 0,
	NULL, NULL,
	SKILL_COMBAT
    },

    {
	"bludgeon proficiency",
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_blud_proficiency,	SLOT( 0),	 0,	 0,	0,
	"",			"!Blud Profi!",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	0, 0, 0, 0, 0,
	NULL, NULL,
	SKILL_COMBAT
    },

    {
	"slash proficiency",
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_slash_proficiency,	SLOT( 0),	 0,	 0,	0,
	"",			"!Slash Profi!",
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	0, 0, 0, 0, 0,
	NULL, NULL,
	SKILL_COMBAT
    },

    {
	"andean",
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_andean,		SLOT( 0),	 0,	 0,	0,
	"",			"!Andean Language!",
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	10, 100, 30, 0, 0,
	NULL, NULL,
	SKILL_LANGUAGE
    },

    {
	"elekun",
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_elekun,	SLOT( 0),	 0,	 0,	0,
	"",			"!Elekun Language!",
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	10, 100, 30, 0, 0,
	NULL, NULL,
	SKILL_LANGUAGE
    },

    {
        "tongues",
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_tongues,   SLOT( 0),        0,      0,     0,
        "",                     "!Tongues Language!",
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        10, 100, 30, 0, 0,
        NULL, NULL,
        SKILL_LANGUAGE
    },


    {
	"vedican",
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_vedican,	SLOT( 0),	 0,	 0,	0,
	"",			"!Vedican Language!",
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	10, 100, 30, 0, 0,
	NULL, NULL,
	SKILL_LANGUAGE
    },

    {
	"luderwen",
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_luderwen,		SLOT( 0),	 0,	 0,	0,
	"",			"!Luderwen Language!",
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	10, 100, 30, 0, 0,
	NULL, NULL,
	SKILL_LANGUAGE
    },

    {
        "jikkean",
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_jikkean,          SLOT( 0),        0,      0,     0,
        "",                     "!Jikkean Language!",
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        10, 100, 30, 0, 0,
        NULL, NULL,
        SKILL_LANGUAGE
    },


    {
	"bleeding",
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_bleeding,		SLOT( 0),	 0,	 0,	0,
	NULL,			"!Bleeding!",
	"Your last blood flows from your body, and you see a bright light fade...",
	NULL,
	"$n's last blood flows from $s body.",
	"You are lying on the ground bleeding, and will die soon, if not aided.",
	NULL,
	"$n bleeds slowly on the ground.",
	NULL,
	NULL,
	NULL,
	0, 0, 0, 0, 0,
	NULL, NULL,
	0
    },

    {
	"noquit",
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_noquit,		SLOT( 0),	 0,	 0,	0,
	"",			NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	0, 0, 0, 0, 0,
	NULL, NULL,
	0
    }
};
