// FILE NOT USED

/*
 *  random.c, a module for merc2.1, written specifically for Sword Quest
 *  (c) 1994 Robert L. Peckham
 *
 *  requires randstd.are, randpot.are, randmag.are, randvar.are
 */



#include <stdio.h>
#include <string.h>
#include <time.h>
#include "merc.h"

#define BASE_VNUM 9500
#define POTION_VNUM 9600
#define MAGIC_VNUM 9700
#define VARIOUS_VNUM 9800
#define LOC_BODY 0
#define LOC_ARM 1
#define LOC_LEG 2
#define LOC_FOOT 3
#define LOC_SHIELD 4
#define LOC_HEAD 5
#define LOC_HANDS 6
#define TYPE_LEATHER 0
#define TYPE_STUDDED 10
#define TYPE_SCALE 20
#define TYPE_CHAIN 30
#define TYPE_ELVEN 40
#define TYPE_BRONZE 50
#define TYPE_PLATE 60
#define TYPE_FIELD 70
#define MAX_SCROLL 81

/*
 * local prototypes
 */
char *material_flags_name( int matl_flags );
char *material_color_name( int color_flags );

const struct adj_data adjective_table [MAX_ADJ] =
{
    { "age",		100,	0 },
    { "ageless",	100,	0 },
    { "ancient",	300,	ADJ_JEWELRY|ADJ_STAFF },
    { "antique",	200,	ADJ_JEWELRY|ADJ_STAFF },
    { "arched",		100,	ADJ_STAFF|ADJ_SHAPE },
    { "barbed",		100,	ADJ_STAFF|ADJ_SHAPE },
    { "battered",	 50,
	ADJ_NEGATIVE|ADJ_SHAPE|ADJ_QUALITY|ADJ_STAFF|ADJ_JEWELRY },
    { "beautiful",	150,
	ADJ_POSITIVE|ADJ_QUALITY|ADJ_JEWELRY|ADJ_STAFF },
    { "bent",		 80,	ADJ_SHAPE|ADJ_STAFF },
    { "black",		100,	ADJ_COLOR|ADJ_STAFF|ADJ_JEWELRY },
    { "blazing",	200,	ADJ_COLOR|ADJ_JEWELRY|ADJ_POSITIVE },
    { "bleak",		100,	ADJ_STAFF|ADJ_JEWELRY },
    { "blistered",	100,	ADJ_STAFF|ADJ_SHAPE },
    { "bloody",		100,	0 },
    { "blunt",		100,	ADJ_STAFF|ADJ_SHAPE },
    { "blurry",		 80,	ADJ_JEWELRY|ADJ_NEGATIVE },
    { "bowed",		100,	ADJ_STAFF|ADJ_SHAPE },
    { "bright",		120,
	ADJ_JEWELRY|ADJ_POSITIVE|ADJ_QUALITY },
    { "brilliant",	150,
	ADJ_JEWELRY|ADJ_POSITIVE|ADJ_QUALITY },
    { "brittle",	100,	0 },
    { "bumpy",		100,	ADJ_STAFF|ADJ_SHAPE },
    { "burnished",	130,
	ADJ_STAFF|ADJ_JEWELRY|ADJ_POSITIVE|ADJ_QUALITY },
    { "burnt",		 50,	ADJ_STAFF },
    { "carved",		130,
	ADJ_STAFF|ADJ_JEWELRY|ADJ_SHAPE|ADJ_POSITIVE },
    { "chromatic",	100,	ADJ_JEWELRY|ADJ_COLOR },
    { "cloudy",		 70,	ADJ_JEWELRY|ADJ_NEGATIVE|ADJ_QUALITY },
    { "coiled",		100,	ADJ_STAFF|ADJ_JEWELRY|ADJ_SHAPE },
    { "colorful",	110,	ADJ_JEWELRY|ADJ_STAFF|ADJ_COLOR|ADJ_POSITIVE },
    { "colorless",	 90,	ADJ_JEWELRY|ADJ_STAFF|ADJ_COLOR|ADJ_NEGATIVE },
    { "common",		 70,	ADJ_JEWELRY|ADJ_QUALITY|ADJ_NEGATIVE },
    { "course",		100,	ADJ_STAFF|ADJ_SHAPE },
    { "cracked",	 50,
	ADJ_SHAPE|ADJ_STAFF|ADJ_JEWELRY|ADJ_QUALITY|ADJ_NEGATIVE },
    { "crooked",	100,	ADJ_STAFF|ADJ_SHAPE },
    { "crude",		 50,
	ADJ_STAFF|ADJ_JEWELRY|ADJ_SHAPE|ADJ_QUALITY|ADJ_NEGATIVE },
    { "crushed",	 30,	0 },
    { "crystalline",	100,	ADJ_STAFF },
    { "curious",	100,	0 },
    { "curved",		100,	ADJ_STAFF|ADJ_SHAPE },
    { "delicate",	200,	ADJ_JEWELRY|ADJ_QUALITY|ADJ_POSITIVE },
    { "dark",		100,	ADJ_STAFF|ADJ_JEWELRY|ADJ_COLOR },
    { "dim",		 80,	ADJ_JEWELRY|ADJ_COLOR|ADJ_QUALITY|ADJ_NEGATIVE },
    { "dirty",		100,	0 },
    { "distinctive",	130,	ADJ_JEWELRY|ADJ_QUALITY|ADJ_POSITIVE },
    { "drab",		 70,	ADJ_JEWELRY|ADJ_COLOR|ADJ_QUALITY|ADJ_NEGATIVE },
    { "dull",		 80,	ADJ_JEWELRY|ADJ_STAFF|ADJ_COLOR|ADJ_NEGATIVE },
    { "eccentric",	100,	0 },
    { "elaborate",	150,	ADJ_JEWELRY|ADJ_STAFF|ADJ_QUALITY|ADJ_POSITIVE },
    { "engraved",	150,
	ADJ_JEWELRY|ADJ_STAFF|ADJ_QUALITY|ADJ_SHAPE|ADJ_POSITIVE },
    { "faint",		 80,
	ADJ_JEWELRY|ADJ_COLOR|ADJ_QUALITY|ADJ_NEGATIVE },
    { "fiery",		500,
	ADJ_JEWELRY|ADJ_COLOR|ADJ_QUALITY|ADJ_POSITIVE },
    { "filthy",		100,	0 },
    { "fine",		150,
	ADJ_JEWELRY|ADJ_QUALITY|ADJ_POSITIVE },
    { "flaming",	150,	ADJ_JEWELRY|ADJ_COLOR|ADJ_QUALITY|ADJ_POSITIVE },
    { "forked",		100,	ADJ_STAFF|ADJ_SHAPE },
    { "fractal",	350,	ADJ_JEWELRY|ADJ_COLOR|ADJ_QUALITY|ADJ_POSITIVE },
    { "fragmented",	100,	0 },
    { "frosty",		200,	ADJ_JEWELRY|ADJ_COLOR|ADJ_QUALITY|ADJ_POSITIVE },
    { "glazed",		180,	ADJ_JEWELRY|ADJ_COLOR|ADJ_QUALITY|ADJ_POSITIVE },
    { "glinting",	200,	ADJ_JEWELRY|ADJ_COLOR|ADJ_QUALITY|ADJ_POSITIVE },
    { "glittering",	300,	ADJ_JEWELRY|ADJ_COLOR|ADJ_QUALITY|ADJ_POSITIVE },
    { "glorious",	140,	ADJ_JEWELRY|ADJ_QUALITY|ADJ_POSITIVE },
    { "glossy",		140,	ADJ_JEWELRY|ADJ_COLOR|ADJ_QUALITY|ADJ_POSITIVE },
    { "glowing",	500,	ADJ_JEWELRY|ADJ_COLOR|ADJ_QUALITY|ADJ_POSITIVE },
    { "gnarled",	100,	ADJ_STAFF|ADJ_SHAPE },
    { "grimy",		100,	0 },
    { "hazy",		 70,	ADJ_JEWELRY|ADJ_COLOR|ADJ_QUALITY|ADJ_NEGATIVE },
    { "holy",		100,	0 },
    { "horned",		100,	ADJ_STAFF|ADJ_SHAPE },
    { "illuminated",	300,	ADJ_JEWELRY|ADJ_COLOR|ADJ_QUALITY|ADJ_POSITIVE },
    { "impressive",	300,	ADJ_JEWELRY|ADJ_COLOR|ADJ_QUALITY|ADJ_POSITIVE },
    { "inscribed",	100,	ADJ_STAFF|ADJ_JEWELRY|ADJ_SHAPE },
    { "intricate",	300,
	ADJ_JEWELRY|ADJ_STAFF|ADJ_QUALITY|ADJ_SHAPE|ADJ_POSITIVE },
    { "iridescent",	200,	ADJ_JEWELRY|ADJ_COLOR|ADJ_QUALITY|ADJ_POSITIVE },
    { "jaded",		100,	0 },
    { "jagged",		100,	ADJ_STAFF|ADJ_SHAPE },
    { "jewelled",	500,	ADJ_STAFF },
    { "knotted",	100,	ADJ_STAFF|ADJ_SHAPE },
    { "knurled",	100,	ADJ_STAFF|ADJ_SHAPE },
    { "long",		100,	ADJ_STAFF|ADJ_SHAPE },
    { "luminous",	200,	ADJ_JEWELRY|ADJ_COLOR|ADJ_QUALITY|ADJ_POSITIVE },
    { "magnificent",	150,	ADJ_JEWELRY|ADJ_QUALITY|ADJ_POSITIVE },
    { "majestic",	150,	ADJ_JEWELRY|ADJ_QUALITY|ADJ_POSITIVE },
    { "massive",	100,	ADJ_STAFF|ADJ_SHAPE },
    { "mighty",		100,	0 },
    { "muddy",		 50,	0 },
    { "murky",		 50,	0 },
    { "mysterious",	100,	0 },
    { "mystical",	100,	0 },
    { "neon",		300,	ADJ_JEWELRY|ADJ_COLOR|ADJ_QUALITY|ADJ_POSITIVE },
    { "noble",		200,	0 },
    { "notched",	100,	ADJ_STAFF|ADJ_SHAPE },
    { "obscure",	100,	0 },
    { "odd",		100,	0 },
    { "old",		120,	0 },
    { "ornate",		200,	ADJ_STAFF|ADJ_JEWELRY|ADJ_QUALITY|ADJ_POSITIVE },
    { "oval",		100,	ADJ_STAFF|ADJ_JEWELRY|ADJ_SHAPE },
    { "pale", 		 70,	ADJ_JEWELRY|ADJ_STAFF|ADJ_COLOR|ADJ_NEGATIVE },
    { "peculiar",	100,	0 },
    { "plain",		100,	ADJ_JEWELRY },
    { "pointy",		100,	ADJ_STAFF|ADJ_SHAPE },
    { "polished",	150,	ADJ_JEWELRY|ADJ_COLOR|ADJ_QUALITY|ADJ_POSITIVE },
    { "prickly",	100,	ADJ_STAFF|ADJ_SHAPE },
    { "prismatic",	150,	ADJ_JEWELRY|ADJ_COLOR|ADJ_QUALITY|ADJ_POSITIVE },
    { "pronged",	100,	ADJ_STAFF|ADJ_SHAPE },
    { "radiant",	300,	ADJ_JEWELRY|ADJ_COLOR|ADJ_QUALITY|ADJ_POSITIVE },
    { "ragged",		100,	ADJ_STAFF|ADJ_SHAPE },
    { "rainbow",	250,	ADJ_JEWELRY|ADJ_COLOR|ADJ_QUALITY|ADJ_POSITIVE },
    { "ravishing",	200,	ADJ_JEWELRY|ADJ_QUALITY|ADJ_POSITIVE },
    { "rough",		 50,
	ADJ_STAFF|ADJ_JEWELRY|ADJ_SHAPE|ADJ_QUALITY|ADJ_NEGATIVE },
    { "round",		100,	ADJ_STAFF|ADJ_JEWELRY|ADJ_SHAPE },
    { "rugged",		100,	ADJ_STAFF|ADJ_SHAPE },
    { "runed",		500,	ADJ_STAFF|ADJ_JEWELRY|ADJ_POSITIVE },
    { "rusty",		 50,	ADJ_STAFF|ADJ_JEWELRY|ADJ_QUALITY|ADJ_NEGATIVE },
    { "sacred",		100,	0 },
    { "scaled",		100,	ADJ_STAFF|ADJ_SHAPE },
    { "scarred",	 60,
	ADJ_STAFF|ADJ_JEWELRY|ADJ_SHAPE|ADJ_QUALITY|ADJ_NEGATIVE },
    { "scored",		 50,
	ADJ_STAFF|ADJ_JEWELRY|ADJ_SHAPE|ADJ_QUALITY|ADJ_NEGATIVE },
    { "serpentine",	100,	ADJ_STAFF|ADJ_JEWELRY|ADJ_SHAPE },
    { "serrated",	100,	ADJ_STAFF|ADJ_SHAPE },
    { "shadowy",	 80,	ADJ_JEWELRY|ADJ_QUALITY|ADJ_COLOR|ADJ_NEGATIVE },
    { "shattered",	 50,	0 },
    { "shimmering",	300,	ADJ_JEWELRY|ADJ_COLOR|ADJ_QUALITY|ADJ_POSITIVE },
    { "shiny",		150,	ADJ_JEWELRY|ADJ_COLOR|ADJ_QUALITY|ADJ_POSITIVE },
    { "shoddy",		 50,	0 },
    { "short",		100,	ADJ_STAFF|ADJ_SHAPE },
    { "shriveled",	100,	ADJ_STAFF|ADJ_SHAPE },
    { "shrouded",	100,	0 },
    { "simple",		100,	0 },
    { "sleek",		100,	ADJ_STAFF|ADJ_SHAPE },
    { "slender",	100,	ADJ_STAFF|ADJ_SHAPE },
    { "smoky",		130,	ADJ_JEWELRY|ADJ_COLOR|ADJ_QUALITY|ADJ_POSITIVE },
    { "smooth",		100,	ADJ_STAFF|ADJ_SHAPE },
    { "sparkling",	250,	ADJ_JEWELRY|ADJ_COLOR|ADJ_QUALITY|ADJ_POSITIVE },
    { "spectral",	150,	ADJ_JEWELRY|ADJ_COLOR|ADJ_QUALITY|ADJ_POSITIVE },
    { "spiked",		100,	ADJ_STAFF|ADJ_SHAPE },
    { "spiny",		100,	ADJ_STAFF|ADJ_SHAPE },
    { "spiraled",	100,	ADJ_STAFF|ADJ_JEWELRY|ADJ_SHAPE },
    { "splendid",	100,	0 },
    { "splintered",	100,	ADJ_STAFF|ADJ_SHAPE },
    { "square",		100,	ADJ_STAFF|ADJ_JEWELRY|ADJ_SHAPE },
    { "stalwart",	100,	ADJ_STAFF|ADJ_SHAPE },
    { "stout",		100,	ADJ_STAFF|ADJ_SHAPE },
    { "striking",	120,	ADJ_JEWELRY|ADJ_QUALITY|ADJ_POSITIVE },
    { "stunning",	150,	ADJ_JEWELRY|ADJ_QUALITY|ADJ_POSITIVE },
    { "sturdy",		100,	ADJ_STAFF|ADJ_SHAPE },
    { "superb",		200,	ADJ_JEWELRY|ADJ_QUALITY|ADJ_POSITIVE },
    { "tangled",	100,	ADJ_STAFF|ADJ_SHAPE },
    { "tarnished",	 80,	ADJ_JEWELRY|ADJ_QUALITY|ADJ_NEGATIVE },
    { "tattered",	100,	0 },
    { "thick",		100,	ADJ_STAFF|ADJ_SHAPE },
    { "thin",		100,	ADJ_STAFF|ADJ_SHAPE },
    { "tined",		100,	ADJ_STAFF|ADJ_SHAPE },
    { "twisted",	100,	ADJ_STAFF|ADJ_JEWELRY|ADJ_SHAPE },
    { "unadorned",	100,	ADJ_STAFF|ADJ_JEWELRY|ADJ_SHAPE },
    { "unembellished",	100,	ADJ_STAFF|ADJ_JEWELRY|ADJ_SHAPE },
    { "unformed",	100,	ADJ_STAFF|ADJ_JEWELRY|ADJ_SHAPE },
    { "unscathed",	100,	ADJ_STAFF|ADJ_JEWELRY|ADJ_SHAPE },
    { "varnished",	100,	ADJ_STAFF|ADJ_JEWELRY },
    { "veneered",	100,	ADJ_STAFF|ADJ_JEWELRY },
    { "vibrant",	300,	ADJ_JEWELRY|ADJ_COLOR|ADJ_QUALITY|ADJ_POSITIVE },
    { "vivid",		200,	ADJ_JEWELRY|ADJ_COLOR|ADJ_QUALITY|ADJ_POSITIVE },
    { "white",		100,	ADJ_STAFF|ADJ_JEWELRY|ADJ_COLOR },
    { "winding",	100,	ADJ_STAFF|ADJ_SHAPE },
    { "winged",		100,	ADJ_STAFF|ADJ_SHAPE },
    { "wiry",		100,	ADJ_STAFF|ADJ_SHAPE },
    { "withered",	100,	ADJ_STAFF|ADJ_SHAPE },
    { "lastadj",	  0,	0 }
/* EDIT MAX_ADJECTIVE ABOVE IF ADD MORE */
};

const struct matl_data material_table [MAX_MATL] =
{
    { "wood",		1,	1,
	MATL_WOOD,
	COLOR_BROWN },
    { "adamantite",	1000,	1000,
	MATL_LEGEND|MATL_METAL|MATL_PRECIOUS,
	COLOR_SILVER },
    { "agate",		  50,	200,
	MATL_MINERAL|MATL_GEMSTONE|MATL_PRECIOUS|MATL_BRITTLE,
	0 },
    { "alexandrite",	  10,	200,
	MATL_MINERAL|MATL_GEMSTONE|MATL_SEMI_PREC|MATL_BRITTLE,
	0 },
    { "aluminum",	  30,	150,
	MATL_METAL|MATL_ELEMENT,
	COLOR_LIGHT|COLOR_SILVER },
    { "amber",		  90,	200,
	MATL_MINERAL|MATL_GEMSTONE|MATL_PRECIOUS|MATL_BRITTLE,
	0 },
    { "amethyst",	150,	200,
	MATL_MINERAL|MATL_GEMSTONE|MATL_PRECIOUS|MATL_BRITTLE,
	0 },
    { "antler",		 20,	50,
	MATL_MINERAL|MATL_ORGANIC,
	COLOR_LIGHT|COLOR_BROWN },
    { "aquamarine",	120,	200,
	MATL_MINERAL|MATL_GEMSTONE|MATL_PRECIOUS|MATL_BRITTLE,
	0 },
    { "arsenic",	 50,	 1,
	MATL_ELEMENT|MATL_POWDER,
	0 },
    { "ash",		 15,	30,
	MATL_ORGANIC|MATL_WOOD|MATL_FLAMMABLE,
	COLOR_BROWN },
    { "azurite",	 10,	200,
	MATL_MINERAL|MATL_GEMSTONE|MATL_SEMI_PREC|MATL_BRITTLE,
	0 },
    { "balsa",		 30,	10,
	MATL_ORGANIC|MATL_WOOD|MATL_FLAMMABLE,
	COLOR_LIGHT|COLOR_BROWN },
    { "bamboo",		 30,	40,
	MATL_ORGANIC|MATL_WOOD|MATL_FLAMMABLE,
	COLOR_LIGHT|COLOR_BROWN|COLOR_GREEN },
    { "bark",		  5,	5,
	MATL_ORGANIC|MATL_WOOD|MATL_FLAMMABLE|MATL_BRITTLE,
	COLOR_BROWN },
    { "birch",		 20,	30,
	MATL_ORGANIC|MATL_WOOD|MATL_BRITTLE,
	COLOR_LIGHT|COLOR_BROWN|COLOR_WHITE },
    { "bloodstone",	 40,	200,
	MATL_MINERAL|MATL_GEMSTONE|MATL_SEMI_PREC,
	0 },
    { "blue quartz",	 20,	200,
	MATL_MINERAL|MATL_GEMSTONE|MATL_SEMI_PREC|MATL_BRITTLE,
	COLOR_BLUE|COLOR_TRANSLUCENT },
    { "bone",		  5,	60,
	MATL_MINERAL|MATL_ORGANIC|MATL_BRITTLE,
	COLOR_WHITE },
    { "brass",		 40,	130,
	MATL_METAL,
	COLOR_LIGHT|COLOR_GOLD|COLOR_ORANGE|COLOR_YELLOW },
    { "bronze",		 60,	400,
	MATL_METAL,
	COLOR_DARK|COLOR_GOLD },
    { "calcium",	 40,	  1,
	MATL_ELEMENT|MATL_POWDER,
	COLOR_WHITE },
    { "carbon",		 70,	  1,
	MATL_ELEMENT|MATL_POWDER,
	COLOR_BLACK },
    { "carnelian",	 20,	200,
	MATL_MINERAL|MATL_GEMSTONE|MATL_SEMI_PREC|MATL_BRITTLE,
	0 },
    { "chalcedony",	 30,	200,
	MATL_MINERAL|MATL_GEMSTONE|MATL_SEMI_PREC|MATL_BRITTLE,
	0 },
    { "cherrywood",	 80,	20,
	MATL_ORGANIC|MATL_WOOD|MATL_FLAMMABLE,
	COLOR_DARK|COLOR_RED },
    { "chromium",	100,	100,
	MATL_METAL|MATL_ELEMENT,
	0 },
    { "chrysoberyl",	 30,	200,
	MATL_MINERAL|MATL_GEMSTONE|MATL_SEMI_PREC|MATL_BRITTLE,
	0 },
    { "chrysoprase",	 30,	200,
	MATL_MINERAL|MATL_GEMSTONE|MATL_SEMI_PREC|MATL_BRITTLE,
	0 },
    { "citrine",	 30,	200,
	MATL_MINERAL|MATL_GEMSTONE|MATL_SEMI_PREC|MATL_BRITTLE,
	0 },
    { "clay", 		  8,	2,
	MATL_MINERAL|MATL_ORGANIC,
	COLOR_RED|COLOR_BROWN },
    { "cobalt",		 20,	 1,
	MATL_ELEMENT|MATL_POWDER,
	COLOR_DARK|COLOR_BLUE },
    { "copper",		 50,	100,
	MATL_METAL|MATL_SEMI_PREC|MATL_ELEMENT,
	COLOR_DARK|COLOR_GOLD|COLOR_ORANGE|COLOR_GREEN },
    { "coral",		 40,	250,
	MATL_ORGANIC|MATL_SEMI_PREC|MATL_FLAMMABLE|MATL_BRITTLE,
	0 },
    { "diamond",	800,	1000,
	MATL_MINERAL|MATL_GEMSTONE|MATL_PRECIOUS|MATL_BRITTLE,
	COLOR_TRANSLUCENT|COLOR_WHITE },
    { "dragon tooth",	500,	700,
	MATL_LEGEND|MATL_MINERAL|MATL_PRECIOUS,
	0 },
    { "ebony",		 30,	150,
	MATL_MINERAL|MATL_SEMI_PREC,
	COLOR_BLACK },
    { "eggshell",	  1,	2,
	MATL_MINERAL|MATL_ORGANIC|MATL_BRITTLE,
	COLOR_WHITE|COLOR_LIGHT|COLOR_BROWN },
    { "electrum",	100,	200,
	MATL_MINERAL|MATL_GEMSTONE|MATL_PRECIOUS|MATL_BRITTLE,
	0 },
    { "elm",		 10,	30,
	MATL_ORGANIC|MATL_WOOD|MATL_FLAMMABLE,
	COLOR_LIGHT|COLOR_BROWN },
    { "emerald",	300,	200,
	MATL_MINERAL|MATL_GEMSTONE|MATL_PRECIOUS|MATL_BRITTLE,
	COLOR_TRANSLUCENT|COLOR_GREEN },
    { "eucalyptus",	 30,	20,
	MATL_ORGANIC|MATL_WOOD|MATL_FLAMMABLE,
	COLOR_LIGHT|COLOR_BROWN },
    { "fire opal",	500,	200,
	MATL_MINERAL|MATL_GEMSTONE|MATL_PRECIOUS|MATL_BRITTLE,
	0 },
    { "flint",		 50,	250,
	MATL_MINERAL|MATL_BRITTLE|MATL_SPARK,
	0 },
    { "fruit apple",	 3,      7,
	MATL_ORGANIC|MATL_BRITTLE,
	0 },
    { "fruit grapes",	 5,      2,
	MATL_ORGANIC,
	0 },
    { "fur",		 10,	 10,
	MATL_ORGANIC|MATL_FLAMMABLE,
	0 },
    { "garnet",		100,	200,
	MATL_MINERAL|MATL_GEMSTONE|MATL_PRECIOUS|MATL_BRITTLE,
	0 },
    { "gold",		300,	50,
	MATL_METAL|MATL_PRECIOUS|MATL_ELEMENT,
	COLOR_GOLD|COLOR_YELLOW },
    { "glass",		 30,	70,
	MATL_MINERAL|MATL_BRITTLE,
	COLOR_TRANSPARENT },
    { "granite",	 10,	150,
	MATL_MINERAL,
	COLOR_BLACK|COLOR_WHITE },
    { "graphite",	 10,	50,
	MATL_BRITTLE,
	COLOR_BLACK },
    { "hematite",	 20,	200,
	MATL_MINERAL|MATL_GEMSTONE|MATL_SEMI_PREC|MATL_BRITTLE,
	0 },
    { "hemp",             5,     1,
        MATL_ELEMENT|MATL_FLAMMABLE, 
  
COLOR_RED|COLOR_YELLOW|COLOR_BLACK|COLOR_WHITE|COLOR_BLUE|COLOR_GREEN|COLOR_ORANGE|COLOR_BROWN 
},
    { "iodine",		 50,	 1,
	MATL_ELEMENT|MATL_LIQUID,
	COLOR_BLUE },
    { "iron",		 40,	300,
	MATL_METAL|MATL_ELEMENT,
	COLOR_SILVER|COLOR_WHITE },
    { "ivory",		150,	100,
	MATL_MINERAL|MATL_ORGANIC|MATL_PRECIOUS|MATL_BRITTLE,
	COLOR_WHITE },
    { "jacinth",	 30,	200,
	MATL_MINERAL|MATL_GEMSTONE|MATL_SEMI_PREC|MATL_BRITTLE,
	0 },
    { "jade",		200,	200,
	MATL_MINERAL|MATL_PRECIOUS,
	COLOR_GREEN|COLOR_LIGHT },
    { "jasper",		 30,	200,
	MATL_MINERAL|MATL_GEMSTONE|MATL_SEMI_PREC|MATL_BRITTLE,
	0 },
    { "lapis lazuli",	 20,	200,
	MATL_MINERAL|MATL_GEMSTONE|MATL_SEMI_PREC|MATL_BRITTLE,
	0 },
    { "lead",		 20,	20,
	MATL_METAL|MATL_ELEMENT,
	COLOR_DARK|COLOR_SILVER },
    { "leather",	 40,	1,
	MATL_ORGANIC|MATL_FLAMMABLE,
	COLOR_LIGHT|COLOR_DARK|COLOR_BROWN|COLOR_RED },
    { "linen",           10,    1,
        MATL_ORGANIC|MATL_FLAMMABLE,
        COLOR_LIGHT|COLOR_DARK|COLOR_BROWN|COLOR_RED },
    { "silk",            20,     20,
        MATL_FLAMMABLE|MATL_SEMI_PREC,
        COLOR_RED|COLOR_BLACK|COLOR_GREEN|COLOR_ORANGE|COLOR_BLUE|COLOR_YELLOW|COLOR_WHITE|COLOR_BROWN },
    { "burlap",          30,     30,
        MATL_FLAMMABLE,
        COLOR_BROWN },
    { "cotton",          20,     20,
        MATL_FLAMMABLE,
        COLOR_RED|COLOR_BLACK|COLOR_GREEN|COLOR_ORANGE|COLOR_BLUE|COLOR_YELLOW|COLOR_WHITE|COLOR_BROWN },
    { "wool",            20,     20,
        MATL_FLAMMABLE,
        COLOR_RED|COLOR_BLACK|COLOR_GREEN|COLOR_ORANGE|COLOR_BLUE|COLOR_YELLOW|COLOR_WHITE|COLOR_BROWN },
    { "chitin",          160,   70,
        MATL_ORGANIC,
        COLOR_RED|COLOR_LIGHT|COLOR_DARK|COLOR_BROWN|COLOR_YELLOW| COLOR_BLACK|COLOR_GREEN },
    { "magnesium",	250,	120,
	MATL_METAL|MATL_ELEMENT,
	COLOR_SILVER },
    { "malachite",	 20,	200,
	MATL_MINERAL|MATL_GEMSTONE|MATL_SEMI_PREC|MATL_BRITTLE,
	0 },
    { "maple",		 30,	30,
	MATL_ORGANIC|MATL_WOOD|MATL_FLAMMABLE,
	COLOR_DARK|COLOR_BROWN },
    { "marble",		100,	200,
	MATL_MINERAL|MATL_PRECIOUS,
	COLOR_WHITE|COLOR_BLACK },
    { "meat raw",	  1,	 1,
	MATL_ORGANIC,
	COLOR_RED },
    { "mercury",	300,	 1,
	MATL_ELEMENT|MATL_LIQUID,
	COLOR_SILVER },
    { "mithril",	1200,	1100,
	MATL_LEGEND|MATL_METAL|MATL_PRECIOUS,
	COLOR_LIGHT|COLOR_SILVER },
    { "moonstone",	 30,	200,
	MATL_MINERAL|MATL_GEMSTONE|MATL_SEMI_PREC,
	0 },
    { "nickel",		 20,	100,
	MATL_METAL|MATL_ELEMENT,
	COLOR_SILVER },
    { "oak",		 30,	40,
	MATL_ORGANIC|MATL_WOOD|MATL_FLAMMABLE,
	COLOR_BROWN },
    { "obsidian",	 40,	20,
	MATL_MINERAL|MATL_BRITTLE,
	COLOR_BLACK },
    { "onyx",		200,	200,
	MATL_MINERAL|MATL_PRECIOUS,
	COLOR_BLACK },
    { "opal",		400,	200,
	MATL_MINERAL|MATL_GEMSTONE|MATL_PRECIOUS|MATL_BRITTLE,
	0 },
    { "palladium",	100,	200,
	MATL_METAL|MATL_ELEMENT|MATL_SEMI_PREC,
	0 },
    { "pearl",		200,	30,
	MATL_MINERAL|MATL_PRECIOUS|MATL_BRITTLE,
	COLOR_WHITE },
    { "peridot",	 20,	200,
	MATL_MINERAL|MATL_GEMSTONE|MATL_SEMI_PREC|MATL_BRITTLE,
	0 },
    { "pewter",		 30,	15,
	MATL_METAL,
	0 },
    { "phosphorous",	100,	 1,
	MATL_ELEMENT|MATL_POWDER,
	0 },
    { "pine",		 40,	30,
	MATL_ORGANIC|MATL_WOOD|MATL_FLAMMABLE,
	COLOR_BROWN },
    { "platinum",	500,	35,
	MATL_METAL|MATL_PRECIOUS|MATL_ELEMENT,
	COLOR_DARK|COLOR_SILVER },
    { "potassium",	100,	 1,
	MATL_ELEMENT|MATL_POWDER,
	0 },
    { "pyrite",		 50,	30,
	MATL_MINERAL|MATL_SEMI_PREC,
	COLOR_GOLD },
    { "quartz",		 10,	200,
	MATL_MINERAL|MATL_GEMSTONE|MATL_SEMI_PREC|MATL_BRITTLE,
	COLOR_TRANSLUCENT|COLOR_WHITE },
    { "redwood",	 60,	35,
	MATL_ORGANIC|MATL_WOOD|MATL_FLAMMABLE,
	COLOR_DARK|COLOR_BROWN|COLOR_RED },
    { "reed",		  7,	3,
	MATL_ORGANIC|MATL_FLAMMABLE,
	0 },
    { "rhodochrosite",	 20,	200,
	MATL_MINERAL|MATL_GEMSTONE|MATL_SEMI_PREC|MATL_BRITTLE,
	0 },
    { "rock crystal",	 30,	200,
	MATL_MINERAL|MATL_GEMSTONE|MATL_SEMI_PREC|MATL_BRITTLE,
	0 },
    { "ruby",		600,	700,
	MATL_MINERAL|MATL_GEMSTONE|MATL_PRECIOUS|MATL_BRITTLE,
	COLOR_TRANSLUCENT|COLOR_RED },
    { "sapphire",	500,	600,
	MATL_MINERAL|MATL_GEMSTONE|MATL_PRECIOUS|MATL_BRITTLE,
	COLOR_TRANSLUCENT|COLOR_BLUE },
    { "sardonyx",	 20,	200,
	MATL_MINERAL|MATL_GEMSTONE|MATL_SEMI_PREC|MATL_BRITTLE,
	0 },
    { "shell",		 30,	350,
	MATL_MINERAL|MATL_ORGANIC,
	COLOR_WHITE },
    { "silicon",	 30,	40,
	MATL_ELEMENT,
	COLOR_WHITE },
    { "silver",		200,	150,
	MATL_METAL|MATL_ELEMENT|MATL_PRECIOUS,
	COLOR_SILVER },
    { "sodium",		 50,	 1,
	MATL_ELEMENT|MATL_POWDER,
	0 },
    { "spinel",		 30,	200,
	MATL_MINERAL|MATL_GEMSTONE|MATL_SEMI_PREC|MATL_BRITTLE,
	0 },
    { "star sapphire",	1000,	200,
	MATL_MINERAL|MATL_GEMSTONE|MATL_PRECIOUS|MATL_BRITTLE,
	COLOR_TRANSLUCENT|COLOR_LIGHT|COLOR_BLUE },
    { "star rose quartz", 700,	200,
	MATL_MINERAL|MATL_GEMSTONE|MATL_PRECIOUS|MATL_BRITTLE,
	COLOR_TRANSLUCENT|COLOR_LIGHT|COLOR_RED },
    { "star ruby",	1200,	200,
	MATL_MINERAL|MATL_GEMSTONE|MATL_PRECIOUS|MATL_BRITTLE,
	COLOR_TRANSLUCENT|COLOR_LIGHT|COLOR_RED },
    { "steel",		160,	900,
	MATL_METAL|MATL_SEMI_PREC,
	COLOR_SILVER },
    { "stone",		  1,	250,
	MATL_MINERAL,
	0 },
    { "sulfur",		100,	 1,
	MATL_ELEMENT|MATL_POWDER,
	0 },
    { "teak",		 90,	30,
	MATL_ORGANIC|MATL_WOOD|MATL_FLAMMABLE,
	COLOR_BROWN },
    { "tiger eye",	 30,	200,
	MATL_MINERAL|MATL_GEMSTONE|MATL_SEMI_PREC|MATL_BRITTLE,
	0 },
    { "tin",		 20,	80,
	MATL_METAL|MATL_ELEMENT,
	COLOR_SILVER },
    { "titanium",	800,	700,
	MATL_METAL|MATL_PRECIOUS|MATL_ELEMENT,
	COLOR_SILVER|COLOR_LIGHT|COLOR_BLUE },
    { "topaz",		200,	200,
	MATL_MINERAL|MATL_GEMSTONE|MATL_PRECIOUS|MATL_BRITTLE,
	0 },
    { "tourmaline",	 30,	200,
	MATL_MINERAL|MATL_GEMSTONE|MATL_SEMI_PREC|MATL_BRITTLE,
	0 },
    { "turquoise",	250,	200,
	MATL_MINERAL|MATL_GEMSTONE|MATL_PRECIOUS|MATL_BRITTLE,
	0 },
    { "wax",		 20,	1,
	MATL_ORGANIC,
	0 },
    { "wicker",		 10,	5,
	MATL_ORGANIC|MATL_FLAMMABLE,
	0 },
    { "zinc",		100,	100,
	MATL_METAL|MATL_ELEMENT,
	COLOR_SILVER },
    { "zircon",		 20,	200,
	MATL_MINERAL|MATL_GEMSTONE|MATL_SEMI_PREC|MATL_BRITTLE,
	0 },
    { "lastmatl",	1,	1, 	0, 	0 }
/* EDIT MAX_MATERIAL ABOVE IF ADD MORE */
};


static char * const scroll_table [] =
{
	"blackened", "stretched", "printed", "mass produced", "cursed",
	"tied", "encoded", "radiating", "dark", "darkened", "translucent",
/* 10 */
	"bark", "invisible ink", "hard", "flaked", "gilded", "modly",
	"vital", "frail", "pliable", "parchment", "tattered", "rolled",
/* 22 */
	"bound", "old", "strange", "runed", "encrypted", "bloodridden",
	"gold inlaid", "engraved", "crinkled", "folded", "painted",
/* 33 */
	"stone", "irrational", "speckled", "ivory", "brittle", "decomposing",
	"white", "yellow", "papyrus", "flayed skin", "pressed leather",
/* 44 */
	"dirty", "dusty", "thin", "scratched", "silver inlaid", "wax",
	"chalked", "faded", "crimped", "ordinary", "fried", "dragon",
/* 56 */
	"wet", "bronzed", "long", "torn", "heavy", "cotton", "sealed",
	"burnt", "half eaten", "bonded", "messy", "inked", "clay",
/* 69 */
	"wax stained", "crumpled", "washed out", "lamb skin", "odd",
	"fringed", "moss covered", "dry", "brazen", "fine printed",
/* 79 */
	"stained", "soiled"
/* 81 */
};


char *material_flags_name( int matl_flags )
{
    static char buf[512];

    buf[0] = '\0';
    if ( matl_flags & MATL_LEGEND       ) strcat( buf, " legend"       );
    if ( matl_flags & MATL_METAL        ) strcat( buf, " metal"        );
    if ( matl_flags & MATL_MINERAL      ) strcat( buf, " mineral"      );
    if ( matl_flags & MATL_ORGANIC      ) strcat( buf, " organic"      );
    if ( matl_flags & MATL_GEMSTONE     ) strcat( buf, " gemstone"     );
    if ( matl_flags & MATL_WOOD         ) strcat( buf, " wood"         );
    if ( matl_flags & MATL_PRECIOUS     ) strcat( buf, " precious"     );
    if ( matl_flags & MATL_SEMI_PREC    ) strcat( buf, " semi_prec"    );
    if ( matl_flags & MATL_BRITTLE      ) strcat( buf, " brittle"      );
    if ( matl_flags & MATL_FLAMMABLE    ) strcat( buf, " flammable"    );
    if ( matl_flags & MATL_ELEMENT      ) strcat( buf, " element"      );
    if ( matl_flags & MATL_SPARK        ) strcat( buf, " spark"        );
    if ( matl_flags & MATL_POWDER       ) strcat( buf, " powder"       );
    if ( matl_flags & MATL_LIQUID       ) strcat( buf, " liquid"       );
    return ( buf[0] != '\0' ) ? buf+1 : "none";
}


char *material_color_name( int color_flags )
{
    static char buf[512];

    buf[0] = '\0';
    if ( color_flags & COLOR_BLACK       ) strcat( buf, " black"        );
    if ( color_flags & COLOR_WHITE       ) strcat( buf, " white"        );
    if ( color_flags & COLOR_RED         ) strcat( buf, " red"          );
    if ( color_flags & COLOR_ORANGE      ) strcat( buf, " orange"       );
    if ( color_flags & COLOR_YELLOW      ) strcat( buf, " yellow"       );
    if ( color_flags & COLOR_GREEN       ) strcat( buf, " green"        );
    if ( color_flags & COLOR_BLUE        ) strcat( buf, " blue"         );
    if ( color_flags & COLOR_PURPLE      ) strcat( buf, " purple"       );
    if ( color_flags & COLOR_SILVER      ) strcat( buf, " silver"       );
    if ( color_flags & COLOR_GOLD        ) strcat( buf, " gold"         );
    if ( color_flags & COLOR_BROWN       ) strcat( buf, " brown"        );
    if ( color_flags & COLOR_DARK        ) strcat( buf, " dark"         );
    if ( color_flags & COLOR_LIGHT       ) strcat( buf, " light"        );
    if ( color_flags & COLOR_TRANSPARENT ) strcat( buf, " transparent"  );
    if ( color_flags & COLOR_TRANSLUCENT ) strcat( buf, " translucent"  );
    return ( buf[0] != '\0' ) ? buf+1 : "none";
}

void do_material( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    int matl;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Material what?\n\r", ch );
	return;
    }

	if ( ( matl = material_lookup( arg ) ) < 0 )
	{
	    send_to_char( "No such material.\n\r", ch );
	    return;
	}

	sprintf( buf, "Matl #: %4d Material: '%s'\n\r",
	    matl, material_table[matl].name );
	send_to_char( buf, ch );

    sprintf( buf, "Value: %d   Hardness: %d\n\r",
	material_table[matl].value, material_table[matl].hardness );
    send_to_char( buf, ch );

    sprintf( buf, "Material flags: %s\n\r",
	material_flags_name( material_table[matl].flags ) );
    send_to_char( buf, ch );

    sprintf( buf, "Material colors: %s\n\r",
	material_color_name( material_table[matl].color ) );
    send_to_char( buf, ch );

    return;
}


/*
 * Lookup a material by name.
 */
int material_lookup( const char *name )
{
    int matl;

    for ( matl = 0; matl < MAX_MATL; matl++ )
    {
	if ( material_table[matl].name == NULL )
	    break;
	if ( LOWER(name[0]) == LOWER(material_table[matl].name[0])
	&&   !str_prefix( name, material_table[matl].name ) )
	    return matl;
    }

    return 0;
}


OBJ_DATA *create_random_armor( void )
{
	OBJ_INDEX_DATA *pObjIndex;
	int vnum = BASE_VNUM;
	int percent = number_percent();

	vnum += number_bits(3);

	if (percent <= 15)
		vnum += TYPE_LEATHER;
	else if (percent <= 25)
		vnum += TYPE_STUDDED;
	else if (percent <= 35)
		vnum += TYPE_SCALE;
	else if (percent <= 53)
		vnum += TYPE_CHAIN;
	else if (percent <= 55)
		vnum += TYPE_ELVEN;
	else if (percent <= 75)
		vnum += TYPE_BRONZE;
	else if (percent <= 90)
		vnum += TYPE_PLATE;
	else
		vnum += TYPE_FIELD;

	if ((pObjIndex = get_obj_index(vnum)) == NULL)
		return NULL;

	return create_object( pObjIndex );
}


int calculate_saving_modifier( int bonus )
{
	int mod;

	mod = number_bits(2) - 2 + bonus;

	if (mod == 0)
		mod = -1;

	return mod;
}


int calculate_attrib_modifier( int bonus )
{
	int mod;

	mod = number_bits(4) - 6 + bonus;

	if (mod == 0)
		mod = 1;

	return mod;
}


int calculate_stat_modifier( int bonus )
{
	int mod;

	mod = 1;

	while (!number_bits(3))
		mod++;

	if (!number_bits(3))
		mod *= -1;

	mod += bonus;

	if (mod == 0)
		mod = 1;

	return mod;
}



void add_random_magic( OBJ_DATA *obj )
{
	int modifier;
	AFFECT_DATA *paf;

	if (affect_free == NULL)
	{
		paf = alloc_perm( sizeof(*paf));
	}
	else
	{
		paf = affect_free;
		affect_free = affect_free->next;
	}

	paf->type = -1;
	paf->duration = -1;
	paf->bitvector = 0;

	modifier = number_percent();

	if (modifier <= 7)
		paf->location = APPLY_SAVING_PARA;
	else if (modifier <= 14)
		paf->location = APPLY_SAVING_ROD;
	else if (modifier <= 21)
		paf->location = APPLY_SAVING_PETRI;
	else if (modifier <= 28)
		paf->location = APPLY_SAVING_BREATH;
	else if (modifier <= 35)
		paf->location = APPLY_SAVING_SPELL;
	else if (modifier <= 40)
		paf->location = APPLY_HITROLL;
	else if (modifier <= 45)
		paf->location = APPLY_DAMROLL;
/* no such thing as AC anymore, lots of +str for a while */
/*
	else if (modifier <= 70)
		paf->location = APPLY_AC;
*/
	else if (modifier <= 72)
		paf->location = APPLY_STAT_PHYS_STR;
	else if (modifier <= 76)
		paf->location = APPLY_STAT_PHYS_DEX;
	else if (modifier <= 78)
		paf->location = APPLY_STAT_MENT_INT;
	else if (modifier <= 80)
		paf->location = APPLY_STAT_PHYS_TOU;
	else if (modifier <= 90)
		paf->location = APPLY_MANA;
	else
		paf->location = APPLY_HIT;

	switch(paf->location)
	{
		default:
			modifier = 0;
			break;
		case APPLY_SAVING_PARA:
		case APPLY_SAVING_ROD:
		case APPLY_SAVING_PETRI:
		case APPLY_SAVING_BREATH:
		case APPLY_SAVING_SPELL:
			modifier = calculate_saving_modifier( 0 );
			break;
		case APPLY_STAT_PHYS_STR:
		case APPLY_STAT_PHYS_DEX:
		case APPLY_STAT_MENT_INT:
		case APPLY_STAT_PHYS_TOU:
		case APPLY_DAMROLL:
			modifier = calculate_stat_modifier( 0 );
			break;
		case APPLY_MANA:
		case APPLY_HIT:
			modifier = calculate_attrib_modifier( 0 );
			break;
	}

	paf->modifier = modifier;
	paf->next = obj->affected;
	obj->affected = paf;

	SET_BIT( obj->extra_flags, ITEM_MAGIC );

	return;
}


void add_random_extra( OBJ_DATA *obj )
{
	if (!number_bits(7))
		SET_BIT( obj->extra_flags, ITEM_DARK );
	if (!number_bits(7))
		SET_BIT( obj->extra_flags, ITEM_GLOW );
	if (!number_bits(6))
		SET_BIT( obj->extra_flags, ITEM_HUM );
	if (!number_bits(8))
		SET_BIT( obj->extra_flags, ITEM_INVIS );

	return;
}


int select_potion_color( void )
{
	int color;

	color = number_percent();

	if (color <= 10)
		color = gsn_cure_poison;
	else if (color <= 20)
		color = gsn_bless;
	else if (color <= 30)
		color = gsn_armor;
	else if (color <= 40)
		color = gsn_detect_evil;
	else if (color <= 50)
		color = gsn_blindness;
	else if (color <= 60)
		color = gsn_heal;
	else if (color <= 70)
		color = gsn_detect_invis;
	else if (color <= 75)
		color = gsn_vitality;
	else if (color <= 80)
		color = gsn_detect_magic;
	else if (color <= 85)
		color = gsn_sanctuary;
	else if (color <= 95)
		color = gsn_strength;
	else
		color = gsn_heal;

	return color;
}

OBJ_DATA *create_random_potion( void )
{
	OBJ_DATA *obj;
	int percent;
	char buf[MAX_STRING_LENGTH];
	int vnum;

	if (number_bits(2))
	{
		percent = number_percent();

		if (percent <= 8)
			vnum = 9650; /* burgundy*/
		else if (percent <= 15)
			vnum = 9651; /* white */
		else if (percent <= 23)
			vnum = 9652; /* orange */
		else if (percent <= 31)
			vnum = 9653; /* pink */
		else if (percent <= 36)
			vnum = 9654; /* lavender */
		else if (percent <= 41)
			vnum = 9655; /* yellow */
		else if (percent <= 46)
			vnum = 9656; /* amber */
		else if (percent <= 51)
			vnum = 9657; /* blue */
		else if (percent <= 56)
			vnum = 9658; /* clear */
		else if (percent <= 58)
			vnum = 9659; /* purple */
		else if (percent <= 63)
			vnum = 9660; /* clear red */
		else if (percent <= 68)
			vnum = 9661; /* silver */
		else if (percent <= 74)
			vnum = 9662; /* grey */
		else if (percent <= 79)
			vnum = 9663; /* beige */
		else if (percent <= 84)
			vnum = 9664; /* tan */
		else if (percent <= 86)
			vnum = 9665; /* sky-blue */
		else if (percent <= 87)
			vnum = 9666; /* lumpy black */
		else if (percent <= 92)
			vnum = 9667; /* vileness */
		else if (percent <= 94)
			vnum = 9668; /* chalky */
		else if (percent <= 95)
			vnum = 9670; /* aquamrine */
		else
			vnum = 9669; /* azure */

		obj = create_object( get_obj_index( vnum ) );
	}
	else
	{
		obj = create_object( get_obj_index( 9699 ) );

		obj->value[1] = select_potion_color();
		obj->value[2] = select_potion_color();


		free_string( obj->name, MEM_OBJ_NAME );
		free_string( obj->short_descr, MEM_OBJ_SHORT_DESCR );
		free_string( obj->look_descr, MEM_OBJ_LOOK_DESCR );

		if (number_bits(3))
		{
			sprintf( buf, "potion %s %s",
				skill_table[obj->value[1]].spell_color,
				skill_table[obj->value[2]].spell_color );
			obj->name = str_dup( buf );
			sprintf( buf, "a %s potion with %s swirls",
				skill_table[obj->value[1]].spell_color,
				skill_table[obj->value[2]].spell_color );
			obj->short_descr = str_dup( buf );
			sprintf( buf, "a %s potion with %s swirls lies here.",
				skill_table[obj->value[1]].spell_color,
				skill_table[obj->value[2]].spell_color );
			obj->look_descr = str_dup( buf );
		}
		else
		{
		obj->value[3] = select_potion_color();
			sprintf( buf, "potion %s %s %s",
				skill_table[obj->value[1]].spell_color,
				skill_table[obj->value[2]].spell_color,
				skill_table[obj->value[3]].spell_color );
			obj->name = str_dup( buf );
			sprintf( buf, "a %s potion with %s swirls and %s spots",
				skill_table[obj->value[1]].spell_color,
				skill_table[obj->value[2]].spell_color,
				skill_table[obj->value[3]].spell_color );
			obj->short_descr = str_dup( buf );
			sprintf( buf, "a %s potion with %s swirls and %s spots lies here.",
				skill_table[obj->value[1]].spell_color,
				skill_table[obj->value[2]].spell_color,
				skill_table[obj->value[3]].spell_color );
			obj->look_descr = str_dup( buf );
		}
	}

	return obj;
}


OBJ_DATA *create_random_jewelry( void )
{
	OBJ_DATA *obj;

/* bracelets are 9597, amulets 9597, rings 9599 */
	obj = create_object( get_obj_index( 9596+dice(1,3) ) );

	if (number_bits(3))
		add_random_magic( obj );
	add_random_extra( obj );

	obj->cost = 75;
	while (!number_bits(1))
		obj->cost += number_bits(6);

	return obj;
}


void rename_object_random( OBJ_DATA *obj , int table )
{
	char buf[MAX_STRING_LENGTH];
	char *nbuf;
	char *abuf;
	int adj = -1;
	int matl = -1;

	switch (table)
	{
		case TABLE_JEWELRY:
		    while ( 1 )
		    {
			matl = number_range(0,MAX_MATL-1);
			if ( IS_SET(material_table[matl].flags, MATL_PRECIOUS )
			  || IS_SET(material_table[matl].flags, MATL_SEMI_PREC ) )
			    break;
		    }
		    nbuf = material_table[matl].name;

		    while ( 1 )
		    {
			adj = number_range(0,MAX_ADJ-1);
			if ( IS_SET(adjective_table[adj].flags, ADJ_JEWELRY ) )
			    break;
		    }
		    abuf = adjective_table[adj].adjective;
		    break;
		case TABLE_STAFF:
		default:
		    while ( 1 )
		    {
			matl = number_range(0,MAX_MATL-1);
			if ( IS_SET(material_table[matl].flags, MATL_METAL )
			  || IS_SET(material_table[matl].flags, MATL_WOOD )
			  || (IS_SET(material_table[matl].flags, MATL_MINERAL )
			      && !IS_SET(material_table[matl].flags, MATL_GEMSTONE ) ) )
			    break;
		    }
		    nbuf = material_table[matl].name;

		    while ( 1 )
		    {
			adj = number_range(0,MAX_ADJ-1);
			if ( IS_SET(adjective_table[adj].flags, ADJ_STAFF ) )
			    break;
		    }
		    abuf = adjective_table[adj].adjective;
		    break;
		case TABLE_SCROLL:
			nbuf = scroll_table[number_range(0,MAX_SCROLL)];
			abuf = "";
			break;
	}

    if ( adj != -1 && matl != -1 )
    {
	obj->cost = (obj->weight > 0 ? obj->weight : 1 );
	obj->cost *= material_table[matl].value;
	obj->cost *= adjective_table[adj].modifier;
	obj->cost /= 100;
    }


	sprintf(buf, "%s%s%s %s", 
		abuf, (table == TABLE_SCROLL ? "" : " "), nbuf,
		obj->name );
	free_string( obj->name, MEM_OBJ_NAME );
	obj->name = str_dup( buf );

	sprintf(buf, "the %s",
		obj->name );
	free_string( obj->short_descr, MEM_OBJ_SHORT_DESCR );
	obj->short_descr = str_dup( buf );

	sprintf( buf, "%s lies here.",
		obj->short_descr );
	free_string( obj->look_descr, MEM_OBJ_LOOK_DESCR );
	obj->look_descr = str_dup( buf );

	return;
}



OBJ_DATA *create_random_magic( void )
{
	OBJ_DATA *obj;
	int vnum;

int staff_spell_table [10] =
{
	gsn_vampiric_touch,
	gsn_blindness,
	gsn_sleep,
	gsn_bless,
	gsn_heal,
	gsn_heal,
	gsn_teleport,
	gsn_word_of_recall,
	gsn_fly,
	gsn_infravision
};

int wand_spell_table [11] =
{
	gsn_lightning_bolt,
	gsn_lightning_bolt,
	gsn_poison,
	gsn_invis,
	gsn_dispel_evil,
	gsn_dispel_magic,
	gsn_detect_poison,
	gsn_fireball,
	gsn_blindness,
	gsn_curse,
	gsn_pebble
};

	switch (number_bits(2))
	{
		case 0:
			obj = create_object( get_obj_index( 9799 ) );
			obj->value[3] = staff_spell_table[number_range(0,9)];
			obj->value[1] = 5 + number_bits(2);
			break;
		case 1:
			obj = create_object( get_obj_index( 9798 ) );
			obj->value[3] = wand_spell_table[number_range(0,10)];
			obj->value[1] = 10 + number_bits(3);
			break;
		default:
			vnum = 9790;
			vnum += number_bits(3);
			obj = create_object( get_obj_index( vnum ) );
			break;
	}

	add_random_magic( obj );
	add_random_extra( obj );

	return obj;
}


void do_randmagic( CHAR_DATA *ch, char *arg )
{
	OBJ_DATA *obj;
     //   OBJ_INDEX_DATA *pObjIndex;

	obj = create_random_magic();

	if (obj != NULL)
		obj_to_char( obj, ch );

	return;
}


void do_randpotion( CHAR_DATA *ch, char *arg )
{
	OBJ_DATA *obj;

	obj = create_random_potion();

	if (obj != NULL)
		obj_to_char( obj, ch );

	return;
}


void do_randarmor( CHAR_DATA *ch, char *arg )
{
	OBJ_DATA *obj;

	obj = create_random_armor();

	if (obj != NULL)
	{
		obj_to_char( obj, ch );
		add_random_magic( obj );
		add_random_extra( obj );
	}

	return;
}



void check_random_equip( CHAR_DATA *ch )
{
	OBJ_DATA *obj;

	int percent;

	while(number_percent() < UMIN( 80, ch->level ) )
	{

		percent = number_percent();

		if (percent <= 2)
/* various */
		{
			obj = create_random_armor();
		}
		else if (percent <= 20)
		{
			obj = create_random_jewelry();
		}
		else if (percent <= 40)
/* weapon */
		{
			obj = create_random_armor();
		}
		else if (percent <= 70)
		{
			obj = create_random_armor();
			if (obj != NULL)
			{
			while(!number_bits(2))
				add_random_magic( obj );
			add_random_extra( obj );
			}
		}
		else if (percent <= 80)
		{
/*
			obj= create_random_armor();
*/
			obj = create_random_magic();
		}
		else
		{
			obj = create_random_potion();
			add_random_extra( obj );
		}

			if (obj != NULL)
		obj_to_char( obj, ch );
	}

	return;

}
