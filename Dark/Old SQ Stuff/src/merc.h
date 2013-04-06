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

#include "macros.h"


/*
 * Accommodate old non-Ansi compilers.
 */
#if defined(TRADITIONAL)
#define const
#define args( list )			( )
#define DECLARE_DO_FUN( fun )		void fun( )
#define DECLARE_DELAY_FUN( fun )	void fun( )
#define DECLARE_OBJ_FUN( fun )		bool fun( )
#define DECLARE_SPEC_FUN( fun )		bool fun( )
#define DECLARE_ROOM_SPECIAL( fun )	bool fun( )
#define DECLARE_SPELL_FUN( fun )	void fun( )
#else
#define args( list )			list
#define DECLARE_DO_FUN( fun )		DO_FUN    fun
#define DECLARE_DELAY_FUN( fun )	DELAY_FUN    fun
#define DECLARE_OBJ_FUN( fun )		OBJ_FUN  fun
#define DECLARE_SPEC_FUN( fun )		SPEC_FUN  fun
#define DECLARE_ROOM_SPECIAL( fun )	ROOM_SPECIAL fun
#define DECLARE_SPELL_FUN( fun )	SPELL_FUN fun
#endif



/*
 * Short scalar types.
 * Diavolo reports AIX compiler has bugs with short types.
 */
#if	!defined(FALSE)
#define FALSE	 0
#endif

#if	!defined(TRUE)
#define TRUE	 1
#endif

#if	defined(_AIX)
#if	!defined(const)
#define const
#endif
typedef int				sh_int;
typedef int				bool;
#define unix
#else
typedef short    int			sh_int;
typedef unsigned char			bool;
#endif

/*
 * Structure types.
 */
typedef struct	adj_data		ADJ_DATA;
typedef struct	affect_data		AFFECT_DATA;
typedef struct	area_data		AREA_DATA;
typedef struct	ban_data		BAN_DATA;
typedef struct	char_data		CHAR_DATA;
typedef struct	descriptor_data		DESCRIPTOR_DATA;
typedef struct	equip_data		EQUIP_DATA;
typedef struct	exit_data		EXIT_DATA;
typedef struct	extra_descr_data	EXTRA_DESCR_DATA;
typedef struct	flora_instance		FLORA_DATA;
typedef struct	help_data		HELP_DATA;
typedef struct  hunting_data		HUNTING_DATA;
typedef struct	kill_data		KILL_DATA;
typedef struct	matl_data		MATL_DATA;
typedef struct	mob_index_data		MOB_INDEX_DATA;
typedef struct	mob_data		MOB_DATA;
typedef	struct	name_data		NAME_DATA;
typedef struct	note_data		NOTE_DATA;
typedef struct	obj_data		OBJ_DATA;
typedef struct	obj_index_data		OBJ_INDEX_DATA;
typedef struct	pc_data			PC_DATA;
typedef struct	predelay_data		PREDELAY_DATA;
typedef	struct	race_data		RACE_DATA;
typedef struct	reset_data		RESET_DATA;
typedef struct	room_index_data		ROOM_INDEX_DATA;
typedef struct	shop_data		SHOP_DATA;
typedef	struct	skill_delay_data	SKILL_DELAY_DATA;
typedef struct	time_info_data		TIME_INFO_DATA;
typedef struct	weather_data		WEATHER_DATA;



/*
 * Function types.
 */
typedef	void DO_FUN	args( ( CHAR_DATA *ch, char *argument ) );
typedef	void DELAY_FUN	args( ( CHAR_DATA *ch, char *argument ) );
typedef bool OBJ_FUN	args( ( CHAR_DATA *ch, char *cmd, DO_FUN * fnptr,
				char *argument, OBJ_DATA *obj ) );
typedef bool SPEC_FUN	args( ( CHAR_DATA *ch, char *cmd, DO_FUN * fnptr,
				char *argument, CHAR_DATA *mob ) );
typedef bool ROOM_SPECIAL args( ( CHAR_DATA *ch, char *cmd, DO_FUN * fnptr,
				char *argument, ROOM_INDEX_DATA *room ) );
typedef void SPELL_FUN	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );




/*
 * String and memory management parameters.
 */
#define	MAX_KEY_HASH		 1024
#define MAX_STRING_LENGTH	 4096
#define MAX_INPUT_LENGTH	  300
#define MAX_STRING            1048576

/* Arbitrary value for some control commands. */
#define MAX_VNUM                50000


/*
 * Memory type integers.
 */
#define MEM_GENERAL			0
#define MEM_PCDATA			1

#define MEM_MOB_NAME			11
#define MEM_MOB_NAMELIST		12
#define MEM_MOB_SHORT_DESCR		13
#define MEM_MOB_LONG_DESCR		14
#define MEM_MOB_DESCRIPTION		15
#define MEM_MOB_SPEC_FUN_NAME		16

#define MEM_MOB_NAME_FREE_CHAR		21
#define MEM_MOB_NAMELIST_FREE_CHAR	22
#define MEM_MOB_SHORT_DESCR_FREE_CHAR	23
#define MEM_MOB_LONG_DESCR_FREE_CHAR	24
#define MEM_MOB_DESCRIPTION_FREE_CHAR	25
#define MEM_MOB_SPEC_FUN_NAME_FREE_CHAR	26

#define MEM_MOB_SHOP_MESSAGE		27


#define MEM_OBJ_NAME			30
#define MEM_OBJ_SHORT_DESCR		31
#define MEM_OBJ_LOOK_DESCR		32
#define MEM_OBJ_DESCRIPTION		33
#define MEM_OBJ_SPEC_FUN_NAME		34

#define MEM_DIRECTION_KEYWORD		50
#define MEM_DIRECTION_DESCRIPTION	51

#define MEM_AREA_NAME			100
#define MEM_AREA_FILENAME		101
#define MEM_AREA_BUILDERS		102

#define MEM_ROOM_NAME			110
#define MEM_ROOM_DESCRIPTION		111
#define MEM_ROOM_SPECIAL_NAME		112




/*
 * Game parameters.
 * Increase the max'es if you add more of something.
 * Adjust the pulse numbers to suit yourself.
 */
#define MAX_SKILL		   90
#define MAX_LANGUAGE		   6
#define MAX_PROFI		   30
#define MAX_CLASS		   15 + 1
#define MAX_RACE		   7
#define MAX_CLAN                   10 + 1
#define MAX_NAME_LENGTH            12
#define MAX_ADJ                    180
#define MAX_MATL                   123
#define MAX_LEVEL		   40
#define MAX_WEAR		     18
#define LEVEL_IMMORTAL		   (MAX_LEVEL - 5)

/*
 * PULSE_ macros are in game pulses
 */
#define PULSE_PER_SECOND	    4
#define PULSE_PER_MINUTE	  (60 * PULSE_PER_SECOND)
#define PULSE_VIOLENCE              5
#define PULSE_MOBILE		    1
#define PULSE_FALL                  ( 3 * PULSE_PER_SECOND)
#define PULSE_KNOCKOUT              5
#define PULSE_TICK		  (100 * PULSE_PER_SECOND)
#define PULSE_AREA		  (60 * PULSE_PER_SECOND)
#define PULSE_WEATHER		  (160 * PULSE_PER_SECOND)
#define PULSE_PC_UPDATE		  ( 1 * PULSE_PER_SECOND)
#define PULSE_ENVIRONMENT	  (60 * PULSE_PER_MINUTE)



/*
 * Site ban structure.
 */
struct	ban_data
{
    BAN_DATA *	next;
    char *	name;
};

/* Weather Booleans and Voids */
bool    number_chance 		args((int num));
void    show_weather            args(( CHAR_DATA *ch ));
bool    IS_OUTDOORS      args(( CHAR_DATA *ch ));

/*
 * Time and weather stuff.
 */
#define SUN_DARK		    0
#define SUN_RISE		    1
#define SUN_LIGHT		    2
#define SUN_SET			    3

#define SKY_CLOUDLESS               0
#define SKY_CLOUDY                  1
#define SKY_SMALL_STORM             2
#define SKY_MED_SAND_STORM          3
#define SKY_BIG_STORM               4
#define SKY_ENERGY_STORM            5
#define SKY_FOGGY                   6
#define SKY_LIGHT_SAND              7



#define VISION_NORMAL               1
#define VISION_HAZY                 2
#define VISION_TOO_BRIGHT           3
#define VISION_TOO_DARK             4

struct	time_info_data
{
    int		hour;
    int		day;
    int		month;
    int		year;
};

struct	weather_data
{
    
    int		change;
    int		sky;
    int		sunlight;
};



/*
 * Connected state for a channel.
 */
#define CON_PLAYING			 0
#define CON_READ_MOTD			 1

#define CON_ANSI_ENABLE	                 99
#define CON_MAIN_MENU                    100

#define CON_LOGIN_GET_NAME	         150
#define CON_LOGIN_PASSWORD               151

#define CON_CREATE_NEW_CHAR              200
#define CON_CREATE_NEW_NAME              201
#define CON_CREATE_NEW_PASSWORD          202
#define CON_CREATE_CONFIRM_PASSWORD      203
#define CON_CREATE_NEW_SEX               204
#define CON_CREATE_NEW_RACE              205
#define CON_CREATE_NEW_CLASS             206
#define CON_CREATE_LONG_DESC             207
#define CON_CREATE_SHORT_DESC            208
#define CON_CREATE_NAMELIST              209
#define CON_CREATE_DESCRIPTION           210
#define CON_CREATE_EMAIL                 211

#define CON_CHECK_NEW_CHAR               220

#define CON_CHANGE_PASSWORD              300
#define CON_GET_CHANGED_PASSWORD         301
#define CON_CONFIRM_CHANGED_PASSWORD     302



/*
 * Descriptor (channel) structure.
 */
struct	descriptor_data
{
    DESCRIPTOR_DATA *	next;
    DESCRIPTOR_DATA *	snoop_by;
    CHAR_DATA *		character;
    CHAR_DATA *		original;
   
    char *		host;
    sh_int		descriptor;
    sh_int		connected;
    bool		fcommand;
    char		inbuf		[4 * MAX_INPUT_LENGTH];
    char		incomm		[MAX_INPUT_LENGTH];
    char		inlast		[MAX_INPUT_LENGTH];
    int			repeat;
    char *		showstr_head;
    char *		showstr_point;
    char *		outbuf;
    int			outsize;
    int			outtop;
    bool		ansi_enable;
};


/*
 * Predelay data.  Used to pass arguments to chained functions.
 */
struct predelay_data
{
    DO_FUN *		fnptr;
    char		argument[MAX_INPUT_LENGTH];
    int			number;
    CHAR_DATA *		victim1;
    CHAR_DATA *		victim2;
    OBJ_DATA *		obj1;
    OBJ_DATA *		obj2;
};



/*
 * TO types for act.
 */
#define TO_ROOM		    0
#define TO_NOTVICT	    1
#define TO_VICT		    2
#define TO_CHAR		    3



/*
 * Help table types.
 */
struct	help_data
{
    HELP_DATA *	next;
    sh_int	level;
    char *	keyword;
    char *	text;
};



/*
 * Shop types.
 */
#define MAX_TRADE	 5
#define MAX_COIN	10


struct coin_data
{
    int		value;			/* this many = 100 silver */
    char	*name;
    char	*name_plural;
    int		density;		/* number per unit weight */
};

#define SHOP_DOESNT_HAVE     0
#define SHOP_YOU_DONT_HAVE   1
#define SHOP_WONT_BUY        2
#define SHOP_IS_BROKE        3
#define SHOP_YOU_ARE_BROKE   4
#define SHOP_THAT_WILL_BE    5
#define SHOP_TAKE_THIS_MONEY 6
#define SHOP_THAT_IS_WORTH   7
#define SHOP_THAT_ISNT_MONEY 8
#define SHOP_MAX_MESSAGE     9

struct	shop_data
{
    SHOP_DATA *	next;			/* Next shop in list		*/
    sh_int	keeper;			/* Vnum of shop keeper mob	*/
    sh_int	buy_type [MAX_TRADE];	/* Item types shop will buy	*/
    sh_int	profit_buy;		/* Cost multiplier for buying	*/
    sh_int	profit_sell;		/* Cost multiplier for selling	*/
    sh_int	open_hour;		/* First opening hour		*/
    sh_int	close_hour;		/* First closing hour		*/
    sh_int	gain;			/* cash gained per tick		*/
    sh_int	coinage_buy[MAX_COIN];	/* value when buying		*/
    sh_int	coinage_sell[MAX_COIN];	/* value when selling		*/
    char *	shop_messages[SHOP_MAX_MESSAGE];
};



/*
 * Per-class stuff.
 */
struct	class_type
{
    char 	class_name	[20];	/* Class name for 'who'	*/
    char	spaces		[20];	/* extra spaces for formatting */
};


/*
 * Per-race stuff.
 */
struct	race_data
{
    char 	*race_name;		/* Race name for 'who'	*/
    char	*hazy_desc;		/* description for hazy chars */
    char	*skin;			/* what they absorb with */
    struct racial_relationship	*relationships;		/* race relations */
    int		prace;			/* generic race number */
    int		race_bits;
    sh_int	stat_phys_str_mod;
    sh_int	stat_phys_dex_mod;
    sh_int	stat_phys_tou_mod;
    sh_int	stat_phys_qui_mod;
    sh_int	stat_ment_int_mod;
    sh_int	hits_base;
    sh_int	hitnodice;
    sh_int	hitsizedice;
    sh_int      mana_base;
    sh_int      mananodice;
    sh_int      manasizedice;
    sh_int	move_base;
    sh_int      movesizedice;
    sh_int      move_nodice;
    sh_int      stun_base;
    sh_int      stunsizedice;
    sh_int      stun_nodice;
    sh_int	vision_min;
    sh_int	vision_max;
    sh_int	vision_range;
    sh_int	height_base;
    sh_int	heightnodice;
    sh_int	heightsizedice;
    int		weight_base;
    int		weightnodice;
    int		weightsizedice;
    int		hit_loc_damage[10];
    char	*hit_loc_string[10];
    int		hit_loc_location[10];
    int		hit_loc_bits[10];
    int		hit_loc_slash[10][10];
    int		hit_loc_bludgeon[10][10];
    int		hit_loc_pierce[10][10];
};


/*
 * racial relationship list element.
 */
struct racial_relationship
{
    struct racial_relationship	*next;
    int		prace;
};


/*
 * Naming information.
 */
struct name_data
{
    struct name_data *next;
    char	*filename;
    int		n_female_names;
    char	**female_names;
    int		n_male_names;
    char	**male_names;
};



/*
 * structure for material properties
 */
struct matl_data
{
    char	*name;
    int		value;
    int		hardness;
    int		flags;
    int		color;
};



/*
 * structure for adjective data
 */
struct adj_data
{
    char	*adjective;
    int		modifier;
    int		flags;
};



/*
 * Data structure for notes.
 */
struct	note_data
{
    NOTE_DATA *	next;
    char *	sender;
    char *	date;
    char *	to_list;
    char *	subject;
    char *	text;
};



/*
 * An affect.
 */
struct	affect_data
{
    AFFECT_DATA *	next;
    sh_int		type;
    sh_int		duration;
    sh_int		location;
    sh_int		modifier;
    int			bitvector;
};


/*
 * A skill delay.
 */
struct	skill_delay_data
{
    SKILL_DELAY_DATA *	next;
    sh_int		skill;
    time_t		expire;
};




/*
 * an annoyed mob's hunting list.
 */
struct	hunting_data
{
    HUNTING_DATA *	next;
    int			reason;
    int			time;
    CHAR_DATA *		victim;
    char		name[MAX_NAME_LENGTH+1];
};


struct flag_info
{
  char *name;
  int  flag;
};


/***************************************************************************
 *                                                                         *
 *                   VALUES OF INTEREST TO AREA BUILDERS                   *
 *                   (Start of section ... start here)                     *
 *                                                                         *
 ***************************************************************************/

/*
 * Well known mob virtual numbers.
 * Defined in #MOBILES.
 */



/*
 * ACT bits for mobs.
 * Used in #MOBILES.
 */
#define ACT_IS_NPC		      1		/* Auto set for mobs	*/
#define ACT_SENTINEL		      2		/* Stays in one room	*/
#define ACT_SCAVENGER		      4		/* Picks up objects	*/
#define ACT_MOUNT                     8		/* Is a mount		*/
#define ACT_AGGRESSIVE		     32		/* Attacks PC's		*/
#define ACT_STAY_AREA		     64		/* Won't leave area	*/
#define ACT_WIMPY		    128		/* Flees when hurt	*/
#define ACT_HUNTING_AGGRO           256		/* aggro from next room */
#define ACT_SHOPKEEPER		   1024		/* Is a shopkeeper	*/
#define ACT_SAVING_MOB             2048		/* Mobile saves like a PC */



/*
 * Bits for 'affected_by'.
 * Used in #MOBILES.
 */
#define AFF_BLIND		      1
#define AFF_INVISIBLE		      2
#define AFF_DETECT_EVIL		      4
#define AFF_DETECT_INVIS	      8
#define AFF_DETECT_MAGIC	     16
#define AFF_DETECT_HIDDEN	     32
#define AFF_HOLD		     64		/* Unused	*/
#define AFF_SANCTUARY		    128
#define AFF_FAERIE_FIRE		    256		/* Unused	*/
#define AFF_INFRARED		    512		/* Unused	*/
#define AFF_CURSE		   1024
#define AFF_FLAMING		   2048		/* Unused	*/
#define AFF_POISON		   4096
#define AFF_PROTECT		   8192
#define AFF_PARALYSIS		  16384		/* Unused	*/
#define AFF_SNEAK		  32768
#define AFF_HIDE		  65536
#define AFF_SLEEP		 131072
#define AFF_CHARM		 262144
#define AFF_FLYING		 524288
#define AFF_PASS_DOOR		1048576


/*
 * Aggression bits for mobiles
 */
#define AGGRESSIVE_ALL                1
#define AGGRESSIVE_SMALLER            2
#define AGGRESSIVE_WOUNDED            4
#define AGGRESSIVE_RACIAL             8
#define AGGRESSIVE_CLAN              16
#define AGGRESSIVE_BANDIT            32
#define AGGRESSIVE_PREDATOR          64
#define AGGRESSIVE_LONG_DISTANCE    128


/*
 * Sex.
 * Used in #MOBILES.
 */
#define SEX_NEUTRAL		      0
#define SEX_MALE		      1
#define SEX_FEMALE		      2


/*
 * Move speed.
 */
#define MOVE_WALK                     0
#define MOVE_RUN                      1
#define MOVE_SNEAK                    2
#define MOVE_MAX                      3


/*
 * List of classes
 */
#define CLASS_NONE		0
#define CLASS_SORCERER		1
#define CLASS_WARLORD		2
#define CLASS_SHAMAN		3
#define CLASS_BARD		4
#define CLASS_HEALER		5
#define CLASS_TEMPLAR		6
#define CLASS_ASSASSIN		7
#define CLASS_BURGLAR		8
#define CLASS_SPY		9
#define CLASS_SCOUT		10
#define CLASS_RANGER		11
#define CLASS_GLADIATOR		12
#define CLASS_FIGHTER		13
#define CLASS_STALKER		14
#define CLASS_ARCHER		15


/*
 * Well known object virtual numbers.
 * Defined in #OBJECTS.
 */
#define OBJ_VNUM_MONEY_ONE	      2
#define OBJ_VNUM_MONEY_SOME	      3

#define OBJ_VNUM_CORPSE		     10

#define OBJ_VNUM_MUSHROOM	     20

#define OBJ_VNUM_RED_APPLE	     50
#define OBJ_VNUM_GREEN_APPLE	     51
#define OBJ_VNUM_RED_GRAPES	     52



/*
 * Item types.
 * Used in #OBJECTS.
 */
#define ITEM_LIGHT		      1
#define ITEM_SCROLL		      2
#define ITEM_WAND		      3
#define ITEM_STAFF		      4
#define ITEM_WEAPON		      5

#define ITEM_TREASURE		      8
#define ITEM_ARMOR		      9
#define ITEM_POTION		     10

#define ITEM_FURNITURE		     12
#define ITEM_TRASH		     13

#define ITEM_CONTAINER		     15

#define ITEM_DRINK_CON		     17
#define ITEM_KEY		     18
#define ITEM_FOOD		     19
#define ITEM_MONEY		     20
#define ITEM_BOAT		     22
#define ITEM_CORPSE		     23

#define ITEM_FOUNTAIN		     25
#define ITEM_PILL		     26

#define ITEM_JEWELRY		     30
#define ITEM_LOCKPICKS		     31

#define ITEM_BUILDING_OBJECT         50
#define ITEM_BUILDING_WINDOW         51
#define ITEM_WAGON_CONTROLS          52

#define ITEM_RANGED_WEAPON           60
#define ITEM_AMMUNITION              61
#define ITEM_HOODED                  62

/*
 * weapon property bits
 */
#define WEAPON_BIT_BACKSTAB           1


/*
 *    item properties
 */
#define MATL_LEGEND                   1
#define MATL_METAL                    2
#define MATL_MINERAL                  4
#define MATL_ORGANIC                  8
#define MATL_GEMSTONE                16
#define MATL_WOOD                    32
#define MATL_PRECIOUS                64
#define MATL_SEMI_PREC              128
#define MATL_BRITTLE                256
#define MATL_FLAMMABLE              512
#define MATL_ELEMENT               1024
#define MATL_SPARK                 2048
#define MATL_POWDER                4096
#define MATL_LIQUID                8192

/*
 *   colors
 */
#define COLOR_BLACK                   1
#define COLOR_WHITE                   2
#define COLOR_RED                     4
#define COLOR_ORANGE                  8
#define COLOR_YELLOW                 16
#define COLOR_GREEN                  32
#define COLOR_BLUE                   64
#define COLOR_PURPLE                128
#define COLOR_SILVER                256
#define COLOR_GOLD                  512
#define COLOR_BROWN                1024
#define COLOR_LIGHT                2048
#define COLOR_DARK                 4096
#define COLOR_TRANSPARENT          8192
#define COLOR_TRANSLUCENT         16384


/*
 *  adjective bits
 */
#define ADJ_JEWELRY                   1
#define ADJ_POSITIVE                  2
#define ADJ_NEGATIVE                  4
#define ADJ_COLOR                     8
#define ADJ_QUALITY                  16
#define ADJ_SHAPE                    32
#define ADJ_SCROLL                   64
#define ADJ_STAFF                   128


/*
 * Window flags.
 */
#define WINDOW_ROOM                   0
#define WINDOW_OBJECT                 1


/*
 * Extra flags.
 * Used in #OBJECTS.
 */
#define ITEM_GLOW		      1
#define GLOW_LIGHT                    +10
#define ITEM_HUM		      2
#define ITEM_DARK		      4
#define DARK_LIGHT                    -10
#define ITEM_LOCK		      8
#define ITEM_NOTAKE		     16
#define ITEM_INVIS		     32
#define ITEM_MAGIC		     64
#define ITEM_NODROP		    128
#define ITEM_BLESS		    256
#define ITEM_ANTI_GOOD		    512
#define ITEM_ANTI_EVIL		   1024
#define ITEM_ANTI_NEUTRAL	   2048
#define ITEM_NOREMOVE		   4096
#define ITEM_INVENTORY		   8192
#define ITEM_VISIBLE_DISTANCE	  16384
#define ITEM_HOOD                 32768               



/*
 * Wear flags.
 * Used in #OBJECTS.
 */
#define ITEM_TAKE		      1
#define ITEM_WEAR_FINGER	      2
#define ITEM_WEAR_NECK		      4
#define ITEM_WEAR_BODY		      8
#define ITEM_WEAR_HEAD		     16
#define ITEM_WEAR_LEGS		     32
#define ITEM_WEAR_FEET		     64
#define ITEM_WEAR_HANDS		    128 
#define ITEM_WEAR_ARMS		    256
#define ITEM_WEAR_ON_BELT	    512
#define ITEM_WEAR_ABOUT		   1024 
#define ITEM_WEAR_WAIST		   2048
#define ITEM_WEAR_LEFT_WRIST	   4096
#define ITEM_WIELD		   8192
#define ITEM_HOLD		  16384
#define ITEM_WEAR_RIGHT_WRIST     32768
#define ITEM_WEAR_UNWEARABLE      65536



/*
 * Apply types (for affects).
 * Used in #OBJECTS.
 */
#define APPLY_NONE		      0
#define APPLY_STAT_PHYS_STR	      1
#define APPLY_STAT_PHYS_DEX	      2
#define APPLY_STAT_PHYS_QUI	      4
#define APPLY_STAT_PHYS_TOU	      5
#define APPLY_STAT_MENT_INT	      3
#define APPLY_SEX		      6
#define APPLY_CLASS		      7
#define APPLY_LEVEL		      8
#define APPLY_AGE		      9
#define APPLY_HEIGHT		     10
#define APPLY_WEIGHT		     11
#define APPLY_MANA		     12
#define APPLY_HIT		     13
#define APPLY_MOVE		     14
#define APPLY_GOLD		     15
#define APPLY_EXP		     16
#define APPLY_HITROLL		     18
#define APPLY_DAMROLL		     19
#define APPLY_SAVING_PARA	     20
#define APPLY_SAVING_ROD	     21
#define APPLY_SAVING_PETRI	     22
#define APPLY_SAVING_BREATH	     23
#define APPLY_SAVING_SPELL	     24
#define APPLY_ABSORBTION	     50
#define APPLY_CHANNELING	     51
#define APPLY_VISION_PROFILE         52
#define APPLY_VISION_RANGE           53
#define APPLY_STUN                   54



/*
 * Values for containers (value[1]).
 * Used in #OBJECTS.
 */
#define CONT_CLOSEABLE		      1
#define CONT_PICKPROOF		      2
#define CONT_CLOSED		      4
#define CONT_LOCKED		      8


/*
 * Tables for renaming items.
 */
#define TABLE_SCROLL		      0
#define TABLE_JEWELRY		      1
#define TABLE_STAFF		      2


/*
 * Zone flags
 */
#define ZONE_NOROOMS                  1


/*
 * Well known room virtual numbers.
 * Defined in #ROOMS.
 */
#define ROOM_VNUM_LIMBO		      2
#define ROOM_VNUM_CHAT		      2
#define ROOM_VNUM_CHARACTER_GENERATION	5
#define ROOM_VNUM_REVIEW_ROOM		6
#define ROOM_VNUM_TEMPLE	   13001
#define ROOM_VNUM_ALTAR		   13067



/*
 * Room flags.
 * Used in #ROOMS.
 */
#define ROOM_DARK		      1
#define ROOM_SAVES_CONTENTS	      2
#define ROOM_NO_MOB		      4
#define ROOM_UNUSED		      8
#define ROOM_DRINKABLE_WATER         16
#define ROOM_PRIVATE		    512
#define ROOM_SAFE		   1024
#define ROOM_SOLITARY		   2048
#define ROOM_PET_SHOP		   4096
#define ROOM_NO_RECALL		   8192
#define ROOM_RENT		  16384



/*
 * Directions.
 * Used in #ROOMS.
 */
#define DIR_NORTH		      0
#define DIR_EAST		      1
#define DIR_SOUTH		      2
#define DIR_WEST		      3
#define DIR_UP			      4
#define DIR_DOWN		      5



/*
 * Exit flags.
 * Used in #ROOMS.
 */
#define EX_ISDOOR		      1
#define EX_CLOSED		      2
#define EX_LOCKED		      4
#define EX_PICKPROOF		     32
#define EX_SECRET		     64
#define EX_FENCE		    128
#define EX_WALL			    256
#define EX_CLIMB		    512



/*
 * Sector types.
 * Used in #ROOMS.
 */
#define SECT_INSIDE		      0
#define SECT_CITY		      1
#define SECT_FIELD		      2
#define SECT_FOREST		      3
#define SECT_HILLS		      4
#define SECT_MOUNTAIN		      5
#define SECT_WATER_SWIM		      6
#define SECT_WATER_NOSWIM	      7
#define SECT_UNUSED		      8
#define SECT_AIR		      9
#define SECT_DESERT		     10
#define SECT_MAX		     11

#define SECTOR_OUTDOORS               1
#define SECTOR_WATER                  2
#define SECTOR_NOSWIM                 4
#define SECTOR_UNDERGROUND            8
#define SECTOR_BUILDING              16
#define SECTOR_AIR                   32
#define SECTOR_WINDOW                64


/*
 * hit locations
 */
#define HIT_LOC_BODY                 0
#define HIT_LOC_HEAD                 1
#define HIT_LOC_ARM_RIGHT            2
#define HIT_LOC_ARM_LEFT             3
#define HIT_LOC_LEG_RIGHT            4
#define HIT_LOC_LEG_LEFT             5
#define HIT_LOC_HAND_RIGHT           6
#define HIT_LOC_HAND_LEFT            7
#define HIT_LOC_NECK                 8
#define HIT_LOC_CLAW                 9
#define HIT_LOC_SHELL               10
#define HIT_LOC_MAX                 12

#define HIT_LOC_SIDES                1
#define HIT_LOC_PAIR                 2


/***************************************************************************
 *                                                                         *
 *                   VALUES OF INTEREST TO AREA BUILDERS                   *
 *                   (End of this section ... stop here)                   *
 *                                                                         *
 ***************************************************************************/

/*
 * Conditions.
 */
#define COND_DRUNK		      0
#define COND_FULL		      1
#define COND_THIRST		      2



/*
 * Positions.
 */
#define POS_DEAD		      0
#define POS_MORTAL		      1
#define POS_INCAP		      2
#define POS_STUNNED		      3
#define POS_SLEEPING		      4
#define POS_RESTING		      5
#define POS_SITTING		      6
#define POS_FIGHTING		      7
#define POS_STANDING		      8



/*
 * ACT bits for players.
 */
#define PLR_IS_NPC		      1		/* Don't EVER set.	*/
#define PLR_CAN_SAVE		      2
#define PLR_UNAPPROVED                4
#define PLR_REVIEWING                 8
#define PLR_REJECTED                 16
#define PLR_PEACEFUL                 32
#define PLR_BLANK		     64
#define PLR_BRIEF		    128
#define PLR_COMBINE		    512
#define PLR_PROMPT		   1024
#define PLR_TELNET_GA		   2048

#define PLR_HOLYLIGHT		   4096
#define PLR_WIZINVIS		   8192
#define PLR_HOLYWALK              16384
#define PLR_HOLYNAME              32768
#define PLR_HOLYVNUM              65536
#define PLR_HOLYSPEECH           131072

#define PLR_NO_EMOTE		 262144
#define PLR_NO_TELL		 262144
#define PLR_LOG			 524288
#define PLR_AFK                 1048576
#define PLR_DENY		2097152
#define PLR_FREEZE	        4194304
#define PLR_DEAD                8388608
#define PLR_CLIMBING            16777216

#define SENSE_SIGHT                   1
#define SENSE_HEARING                 2
#define SENSE_SMELL                   4
#define SENSE_TOUCH                   8
#define SENSE_TASTE                  16
#define SENSE_SIXTH                  32


/*
 * ANSI/VT100 stuff
 */
#define TEXT_BOLD_ON		"\033[0;1m"
#define TEXT_NORMAL		"\033[0m"
#define VT100_CURS_UP		"\033[%dA"
#define VT100_CURS_DOWN		"\033[%dB"
#define VT100_CURS_RIGHT	"\033[%dC"
#define VT100_CURS_LEFT		"\033[%dD"
#define VT100_CURS_POSITION	"\033[%d;%dH"
#define VT100_CURS_HOME		"\033[H"
#define VT100_DEOL_X		"\033[K"
#define VT100_DEOL_0		"\033[0K"
#define VT100_D_LINETOCURS	"\033[1K"
#define VT100_DLINE		"\033[2K"
#define VT100_E_CURSE_TO_BOT	"\033[J"
#define VT100_E_CURSE_TO_BOT0	"\033[0J"
#define VT100_E_TOP_TO_CURS	"\033[1J"
#define VT100_ERASE_SCREEN	"\033[2J"


/*
 * Channel bits.
 */
#define	CHANNEL_CHAT		      2
#define CHANNEL_TELL                  4
#define	CHANNEL_IMMTALK		      8
#define CHANNEL_LOG                  16



/*
 *  Skill flags.
 */
#define SKILL_SPELL		      1
#define SKILL_COMBAT		      2
#define SKILL_STEALTH		      4
#define SKILL_HANDLING		      8
#define SKILL_FIELD		     16
#define SKILL_MAGIC		     32
#define	SKILL_LANGUAGE               64
#define SKILL_PSIONIST              128



/*
 * Prototype for a mob.
 * This is the in-memory version of #MOBILES.
 */
struct	mob_index_data
{
    MOB_INDEX_DATA *	next;
    MOB_INDEX_DATA *	zone_next;
    SPEC_FUN *		spec_fun;
    SHOP_DATA *		pShop;
    AREA_DATA *		area;
    EQUIP_DATA *	equipped;
    char *		name;
    char *		namelist;
    char *		short_descr;
    char *		long_descr;
    char *		description;
    char *		spec_fun_name;
    int			vnum;
    sh_int		sex;
    sh_int		level;
    int			act;
    int			aggression;
    int			affected_by;
    int			clan;
    sh_int		speed;
    sh_int		learned		[MAX_SKILL];
    sh_int		power_level	[MAX_SKILL];
    sh_int		alignment;
    sh_int		mob_stat_phys_str;
    sh_int		mob_stat_phys_dex;
    sh_int		mob_stat_phys_tou;
    sh_int		mob_stat_phys_qui;
    sh_int		mob_stat_ment_int;
    sh_int		mob_offense;
    sh_int		mob_defense;
    sh_int		height;
    sh_int		weight;
    sh_int		absorb[4];
    sh_int		hitnodice;
    sh_int		hitsizedice;
    sh_int		hitplus;
    sh_int		damnodice;
    sh_int		damsizedice;
    sh_int		damplus;
    int			coins[MAX_COIN];   /* probably cheaper than
					      making linked list */
    sh_int		start_pos;
    sh_int		race;
    sh_int		class;
    sh_int              language;
    int                 skin_objects[4];
};



/*
 * One character (PC or NPC).
 */
struct	char_data
{
    CHAR_DATA *		next;
    CHAR_DATA *		next_save_mob;
    CHAR_DATA *		next_in_room;
    CHAR_DATA *		next_fighting;
    CHAR_DATA *		master;
    CHAR_DATA *		leader;
    CHAR_DATA *		fighting;
    CHAR_DATA *		reply;
    CHAR_DATA *		riding;
    CHAR_DATA *		switched_into;
    SPEC_FUN *		spec_fun;
    MOB_INDEX_DATA *	pIndexData;
    DESCRIPTOR_DATA *	desc;
    AFFECT_DATA *	affected;
    NOTE_DATA *		pnote;
    OBJ_DATA *		carrying;
    OBJ_DATA *		wearing[MAX_WEAR];
    ROOM_INDEX_DATA *	in_room;
    ROOM_INDEX_DATA *	was_in_room;
    PC_DATA *		pcdata;
    RESET_DATA *	pReset;
    HUNTING_DATA *	hunting;
    PREDELAY_DATA *	predelay_info;
    int                 predelay_time;
    int                 fallstun;
    int                 fallhit;
    char *		name;
    char *		namelist;
    char *		short_descr;
    char *		long_descr;
    char *		dynamic_descr;
    char *		description;
    char *		spec_fun_name;
    char *		prompt;
    char *              contact;
    sh_int		sex;
    sh_int		class;
    sh_int		race;
    int			clan;
    sh_int		level;
    sh_int		trust;
    char *              time;
    time_t		save_time;
    sh_int		timer;
    sh_int		wait;
    sh_int		perm_stat_phys_str;
    sh_int		perm_stat_phys_dex;
    sh_int		perm_stat_phys_tou;
    sh_int		perm_stat_phys_qui;
    sh_int		perm_stat_ment_int;
    sh_int		perm_offense;
    sh_int		perm_defense;
    sh_int		mod_stat_phys_str;
    sh_int		mod_stat_phys_dex;
    sh_int		mod_stat_phys_tou;
    sh_int		mod_stat_phys_qui;
    sh_int		mod_stat_ment_int;
    sh_int		mod_offense;
    sh_int		mod_defense;
    sh_int		strike_rank;
    sh_int		strike_rank_2;
    sh_int		height;
    sh_int		weight;
    sh_int		hit[HIT_LOC_MAX];
    sh_int		max_hit;
    sh_int		mana;
    sh_int		max_mana;
    sh_int		move;
    sh_int		max_move;
    sh_int              stun;
    sh_int              max_stun;
    sh_int		channeling;
    int			act;
    int			aggression;
    int			affected_by;
    sh_int		speed;
    sh_int		position;
    sh_int		carry_weight;
    sh_int		carry_number;
    sh_int		saving_throw;
    sh_int		alignment;
    sh_int		absorb[4];
    sh_int		general_absorb;
    sh_int		hitroll;
    sh_int		damroll;
    sh_int		deaf;
    sh_int		damnodice;
    sh_int		damsizedice;
    sh_int		damplus;
    sh_int		vision_max;
    sh_int		vision_min;
    sh_int		vision_current;
    sh_int		vision_range;
    sh_int		light;
    sh_int		language;   /* current language */
    sh_int		move_speed;
};



/*
 * Data which only PC's have.
 */
struct	pc_data
{
    PC_DATA *		next;
    SKILL_DELAY_DATA *	skill_delays;
    char *		pwd;
    char *		bamfin;
    char *		bamfout;
    char *		email_address;
    char *		rejected;
    time_t		birth;
    time_t		logon;
    int			played;
    int			pagelen;
    sh_int		condition	[3];
    sh_int		learned		[MAX_SKILL];
    sh_int		power_level	[MAX_SKILL];
};


/*
 * data which only mobiles have
 */
struct	mob_data
{
    MOB_DATA *		next;
};



/*
 * Liquids.
 */
#define LIQ_WATER        0
#define LIQ_MAX		16

struct	liq_type
{
    char *	liq_name;
    char *	liq_color;
    sh_int	liq_affect[3];
};


struct sector_type
{
    char *	name;
    struct sector_type *next;
    int		flags;
    int		difficulty;
    int		water_min;
    int		water_degrade;
};



/*
 * Extra description data for a room or object.
 */
struct	extra_descr_data
{
    EXTRA_DESCR_DATA *next;	/* Next in list                     */
    char *keyword;              /* Keyword in look/examine          */
    char *description;          /* What to see                      */
};



/*
 * Prototype for an object.
 */
struct	obj_index_data
{
    OBJ_INDEX_DATA *	next;
    OBJ_INDEX_DATA *	zone_next;
    EXTRA_DESCR_DATA *	extra_descr;
    AFFECT_DATA *	affected;
    AREA_DATA *		area;
    char *		name;
    char *		short_descr;
    char *		look_descr;
    char *		description;
    char *		spec_fun_name;
    OBJ_FUN *		spec_fun;
    int			vnum;
    int			number;
    int			limit;
    sh_int		item_type;
    sh_int		extra_flags;
    sh_int		wear_flags;
    int			weight;
    int			capacity;
    int			size;
    int			material;
    int			color;
    int			cost;
    int			load_chance;
    int			value	[4];
    sh_int		dam	[2];
    int			toughness;
    sh_int		attack_type;
};



/*
 * One object.
 */
struct	obj_data
{
    OBJ_DATA *		next;
    OBJ_DATA *		next_content;
    OBJ_DATA *		next_worn;
    OBJ_DATA *		contains;
    OBJ_DATA *		in_obj;
    OBJ_FUN *		spec_fun;
    CHAR_DATA *		carried_by;
    EXTRA_DESCR_DATA *	extra_descr;
    AFFECT_DATA *	affected;
    OBJ_INDEX_DATA *	pIndexData;
    ROOM_INDEX_DATA *	in_room;
    RESET_DATA *	pReset;
    char *		name;
    char *		short_descr;
    char *		look_descr;
    char *		description;
    char *		spec_fun_name;
    sh_int		item_type;
    sh_int		extra_flags;
    sh_int		wear_flags;
    sh_int		wear_loc;
    int			weight;
    int			capacity;
    int			size;
    int			material;
    int			color;
    int			cost;
    sh_int		timer;
    int			value	[4];
    sh_int		dam	[2];
    int			toughness;
    sh_int		attack_type;
};


struct wear_data
{
    int			bit;
    char		*name;
    char		*worn;
    char		*wear_string_ch;
    char		*wear_string_room;
    sh_int *		covered_by;
    sh_int *		dependency;
    bool		covers;
    sh_int *		pgn;		/* Pointer to associated gn	*/
};


/*
 * Exit data.
 */
struct	exit_data
{
    ROOM_INDEX_DATA *	to_room;
    int			vnum;
    int			exit_flags;
    int			key;
    char *		keyword;
    char *		description;
    char *              cdescription;
    int			difficulty;
};



/*
 * Reset commands:
 *   '*': comment
 *   'M': read a mobile 
 *   'O': read an object
 *   'P': put object in object
 *   'G': give object to mobile
 *   'E': equip object to mobile
 *   'D': set state of door
 *   'R': randomize room exits
 *   'S': stop (end of list)
 */

/*
 * Area-reset definition.
 */
struct	reset_data
{
    RESET_DATA *	next;
    char		command;
    int		arg1;
    int		arg2;
    int		arg3;
    void *		target;
};


/*
 * Mobile standard equipment definition
 */
struct	equip_data
{
    EQUIP_DATA *	next;
    int			item;
    int			location;
};

/*
 * Area definition.
 */
struct	area_data
{
    AREA_DATA *		next;
    RESET_DATA *	reset_first;
    RESET_DATA *	reset_last;
    MOB_INDEX_DATA *	mob_first;
    MOB_INDEX_DATA *	mob_last;
    ROOM_INDEX_DATA *	room_first;
    ROOM_INDEX_DATA *	room_last;
    OBJ_INDEX_DATA *	obj_first;
    OBJ_INDEX_DATA *	obj_last;
    char *		name;
    char *		filename;
    char *		builders;
    struct sector_type * sector_list;
    int			vnum_start;
    int			vnum_final;
    int			reset_length;
    int			area_bits;
    sh_int		age;
    sh_int		nplayer;
    int			env_save_interval;
};



/*
 * Room type.
 */
struct	room_index_data
{
    ROOM_INDEX_DATA *	next;
    ROOM_INDEX_DATA *	zone_next;
    CHAR_DATA *		people;
    OBJ_DATA *		contents;
    EXTRA_DESCR_DATA *	extra_descr;
    AREA_DATA *		area;
    EXIT_DATA *		exit	[6];
    ROOM_SPECIAL *	spec_fun;
    FLORA_DATA *	flora;
    char *		name;
    char *		description;
    char *		spec_fun_name;
    int			vnum;
    int			room_flags;
    sh_int		light;
    sh_int		light_mod;
    sh_int		light_filter;
    struct sector_type	*sector;
    sh_int		move_cost;
    sh_int		move_delay;
    sh_int		size;
    int			water;
    int			water_min;
    int			water_degrade;
    int			water_update;
};



/*
 * Types of attacks.
 * Must be non-overlapping with spell/skill types,
 * but may be arbitrary beyond that.
 */
#define TYPE_UNDEFINED               -1
#define TYPE_HIT                     1000


/*
 * Armor types.
 * Which value is for what type of attack
 */
#define ABSORB_OTHER			0
#define ABSORB_PIERCE			1
#define ABSORB_BLUDGEON			2
#define ABSORB_SLASH			3



/*
 * Attack types
 */
#define ATTACK_NONE		0
#define ATTACK_PIERCE		1
#define ATTACK_BLUDGEON		2
#define ATTACK_SLASH		3
#define ATTACK_GLASS		4
#define ATTACK_HIT		5
#define ATTACK_MAX		6



/*
 *  Target types.
 */
#define TAR_IGNORE		    0
#define TAR_CHAR_OFFENSIVE	    1
#define TAR_CHAR_DEFENSIVE	    2
#define TAR_CHAR_SELF		    3
#define TAR_OBJ_INV		    4
#define TAR_CHAR_HINDRANCE	    5


/*
 * weapon proficiencies.
 */
struct	profi_type
{
    char *	name;			/* weapon type */
    sh_int	damage_msgs[4];		/* message pointers */
    sh_int *	related_types[4];	/* pointers to similar profis */
    sh_int *	pwpr;			/* generic pointer */
};


/*
 * Skills include spells as a particular case.
 */
struct	skill_type
{
    char *	name;			/* Name of skill		*/
    SPELL_FUN *	spell_fun;		/* Spell pointer (for spells)	*/
    sh_int	target;			/* Legal targets		*/
    sh_int	minimum_position;	/* Position for caster / user	*/
    sh_int *	pgsn;			/* Pointer to associated gsn	*/
    sh_int	slot;			/* Slot for #OBJECT loading	*/
    sh_int	min_mana;		/* Minimum mana used		*/
    sh_int	max_mana;		/* Starting mana cost           */
    sh_int	beats;			/* Waiting time after use	*/
    char *	spell_color;		/* color of spell in potions	*/
    char *	msg_off;		/* Wear off message		*/
    char *	msg_death_char;		/* fatal damage message	to damager*/
    char *	msg_death_targ;		/* fatal msg to target		*/
    char *	msg_death_room;		/* fatal msg to room		*/
    char *	msg_damage_char;	/* damage to damager		*/
    char *	msg_damage_targ;	/* damage to target		*/
    char *	msg_damage_room;	/* damage to room		*/
    char *	msg_miss_char;		/* miss to damager		*/
    char *	msg_miss_targ;		/* miss to target		*/
    char *	msg_miss_room;		/* miss to room			*/
    sh_int	percent_prac;		/* percent you can practice skil*/
    sh_int	percent_max;		/* max skill percent		*/
    sh_int	learn_delay;		/* learn delay, in minutes@100 int */
    sh_int	max_power;		/* max power without channeling dev */
    sh_int	junk3;			/* unused sh_int */
    sh_int *	skill_group;		/* group the skill belongs to   */
    sh_int *	prerequisite;		/* prerequisite to skill	*/
    int		skill_flags;		/* various info on the skill	*/
};


/*
 * weapon proficiency return values.
 */
extern	sh_int	wpr_club;
extern	sh_int	wpr_club_heavy;
extern	sh_int	wpr_dagger;
extern	sh_int	wpr_flail;
extern	sh_int	wpr_half_staff;
extern	sh_int	wpr_hammer;
extern	sh_int	wpr_knife;
extern	sh_int	wpr_pole_arms;
extern	sh_int	wpr_scimitar;
extern	sh_int	wpr_spear_long;
extern	sh_int	wpr_spear_short;
extern	sh_int	wpr_staff;
extern	sh_int	wpr_sword_bastard;
extern	sh_int	wpr_sword_broad;
extern	sh_int	wpr_sword_long;
extern	sh_int	wpr_sword_short;
extern	sh_int	wpr_sword_two_handed;

extern	sh_int	wpr_blowgun;
extern	sh_int	wpr_bow_cross;
extern	sh_int	wpr_bow_cross_heavy;
extern	sh_int	wpr_bow_long;
extern	sh_int	wpr_bow_short;

extern	sh_int	wpr_thrown_boomerang;
extern	sh_int	wpr_thrown_dart;
extern	sh_int	wpr_thrown_knife;
extern	sh_int	wpr_thrown_spear;


/*
 * General lookup values for wear locations.
 */
extern	sh_int          gn_wear_about;
extern	sh_int          gn_wear_arms;
extern	sh_int          gn_wear_body;
extern	sh_int          gn_wear_feet;
extern	sh_int          gn_wear_finger;
extern	sh_int          gn_wear_hands;
extern	sh_int          gn_wear_head;
extern	sh_int          gn_wear_legs;
extern	sh_int          gn_wear_neck;
extern	sh_int          gn_wear_none;
extern	sh_int		gn_wear_on_belt;
extern	sh_int          gn_wear_primary;
extern	sh_int          gn_wear_secondary;
extern	sh_int          gn_wear_waist;
extern	sh_int          gn_wear_leftwrist;
extern  sh_int          gn_wear_rightwrist;

/*
 * These are skill_lookup return values for common skills and spells.
 */
extern	sh_int	gsn_acid_breath;
extern	sh_int	gsn_archery;
extern	sh_int	gsn_armor;
extern	sh_int	gsn_backstab;
extern	sh_int	gsn_bash;
extern	sh_int	gsn_bleeding;	/* stores dam message for bleeding char */
extern	sh_int	gsn_bless;
extern	sh_int	gsn_blindness;
extern	sh_int	gsn_blud_proficiency;
extern	sh_int	gsn_call_lightning;
extern	sh_int	gsn_charm_person;
extern	sh_int	gsn_climbing;
extern  sh_int  gsn_contact;
extern	sh_int	gsn_control_weather;
extern	sh_int	gsn_create_food;
extern	sh_int	gsn_create_water;
extern	sh_int	gsn_cure_blindness;
extern	sh_int	gsn_cure_poison;
extern	sh_int	gsn_curse;
extern	sh_int	gsn_detect_evil;
extern	sh_int	gsn_detect_invis;
extern	sh_int	gsn_detect_magic;
extern	sh_int	gsn_detect_poison;
extern	sh_int	gsn_disarm;
extern	sh_int	gsn_dispel_evil;
extern	sh_int	gsn_dispel_magic;
extern	sh_int	gsn_dodge;
extern	sh_int	gsn_dual_wield;
extern	sh_int	gsn_earthquake;
extern	sh_int	gsn_enchant_weapon;
extern	sh_int	gsn_enhanced_damage;
extern	sh_int	gsn_falling;
extern	sh_int	gsn_fire_breath;
extern	sh_int	gsn_fireball;
extern	sh_int	gsn_flash;
extern	sh_int	gsn_fly;
extern	sh_int	gsn_frost_breath;
extern	sh_int	gsn_gas_breath;
extern	sh_int	gsn_gate;
extern	sh_int	gsn_harm;
extern	sh_int	gsn_heal;
extern	sh_int	gsn_hide;
extern	sh_int	gsn_identify;
extern	sh_int	gsn_infravision;
extern	sh_int	gsn_invis;
extern	sh_int	gsn_kick;
extern	sh_int	gsn_lightning_bolt;
extern	sh_int	gsn_lightning_breath;
extern	sh_int	gsn_locate_object;
extern	sh_int	gsn_martial_arts;
extern	sh_int	gsn_noquit;	/* stores noquit status */
extern	sh_int	gsn_pass_door;
extern	sh_int	gsn_pebble;
extern	sh_int	gsn_peek;
extern	sh_int	gsn_pick_lock;
extern	sh_int	gsn_pierce_proficiency;
extern	sh_int	gsn_poison;
extern	sh_int	gsn_protection_from_evil;
extern	sh_int	gsn_read_magic;
extern	sh_int	gsn_remove_curse;
extern	sh_int	gsn_rescue;
extern	sh_int	gsn_sanctuary;
extern	sh_int	gsn_sense_life;
extern	sh_int	gsn_slash_proficiency;
extern	sh_int	gsn_sleep;
extern	sh_int	gsn_sneak;
extern	sh_int	gsn_steal;
extern	sh_int	gsn_stone_skin;
extern	sh_int	gsn_strength;
extern	sh_int	gsn_summon;
extern	sh_int	gsn_teleport;
extern	sh_int	gsn_tracking;
extern	sh_int	gsn_tumbling;
extern	sh_int	gsn_use_magical_devices;
extern	sh_int	gsn_vampiric_touch;
extern	sh_int	gsn_ventriloquate;
extern	sh_int	gsn_vitality;
extern	sh_int	gsn_weaken;
extern	sh_int	gsn_word_of_recall;

extern	sh_int	gsn_andean; /* Common */
extern	sh_int	gsn_luderwen; /* Elvish */
extern	sh_int	gsn_vedican; /* Dwarvish */
extern	sh_int	gsn_jikkean; /* Gith */
extern  sh_int  gsn_tongues;
extern  sh_int  gsn_elekun; /* Thri-kreen */

/*
 * Utility macros.
 */
#define IS_NULLSTR(str)   ((str)==NULL || (str)[0]=='\0')
#define UMIN(a, b)		((a) < (b) ? (a) : (b))
#define UMAX(a, b)		((a) > (b) ? (a) : (b))
#define URANGE(a, b, c)		((b) < (a) ? (a) : ((b) > (c) ? (c) : (b)))
#define LOWER(c)		((c) >= 'A' && (c) <= 'Z' ? (c)+'a'-'A' : (c))
#define UPPER(c)		((c) >= 'a' && (c) <= 'z' ? (c)+'A'-'a' : (c))
#define IS_SET(flag, bit)	((flag) & (bit))
#define SET_BIT(var, bit)	((var) |= (bit))
#define REMOVE_BIT(var, bit)	((var) &= ~(bit))



/*
 * Character macros.
 */
#define IS_NPC(ch)		(IS_SET((ch)->act, ACT_IS_NPC))
#define IS_IMMORTAL(ch)		(get_trust(ch) >= LEVEL_IMMORTAL)
#define IS_AFFECTED(ch, sn)	(IS_SET((ch)->affected_by, (sn)))

#define IS_GOOD(ch)		(ch->alignment >= 350)
#define IS_EVIL(ch)		(ch->alignment <= -350)
#define IS_NEUTRAL(ch)		(!IS_GOOD(ch) && !IS_EVIL(ch))

#define IS_AWAKE(ch)		(ch->position > POS_SLEEPING)
#define GET_HITROLL(ch)		((ch)->hitroll)
#define GET_DAMROLL(ch)		((ch)->damroll)

#define IS_OUTSIDE(ch)		(!IS_SET(				    \
				    (ch)->in_room->sector->flags,	    \
				    SECTOR_OUTDOORS))
#define VIEW_OUTSIDE(ch)	(IS_SET(				    \
				      (ch)->in_room->sector->flags,	    \
				      SECTOR_OUTDOORS)                      \
				  || IS_SET( (ch)->in_room->sector->flags,  \
				       SECTOR_WINDOW ) )

#define CH_STAND_FIRST(action)  if ( ch->position != POS_STANDING )         \
  { do_stand( ch, "" );  WAIT_STATE( ch, 4 );  continue; }                  \
  else                                                                      \
    (action)



#define WAIT_STATE(ch, npulse)	((ch)->wait = UMAX((ch)->wait, (npulse)))



/*
 * Object macros.
 */
#define CAN_WEAR(obj, part)	(IS_SET((obj)->wear_flags,  (part)))
#define IS_OBJ_STAT(obj, stat)	(IS_SET((obj)->extra_flags, (stat)))



/*
 * Structure for a command in the command lookup table.
 */
struct	cmd_type
{
    char * const	name;
    DO_FUN *		do_fun;
    sh_int		position;
    sh_int		level;
    sh_int		log;
    sh_int		motion;		/* command moves char? for hide */
    DELAY_FUN *         delay_fun;
    sh_int              concentration;  /* breaks concentration? for delays */
};



/*
 * Structure for a social in the socials table.
 */
struct	social_type
{
    char * const	name;
    char * const	char_no_arg;
    char * const	others_no_arg;
    char * const	char_found;
    char * const	others_found;
    char * const	vict_found;
    char * const	char_auto;
    char * const	others_auto;
    int			room_sense;
    int			vict_sense;
};


struct	damage_mess
{
    struct damage_mess		*next;
    sh_int		min_damage;
    char *		message_char;
    char *		message_room;
    char *		message_victim;
};

struct	weather_mess
{
    struct weather_mess	*next;
    sh_int		time;
    char *		message;
};


/*
 * Global lists of messages read in
 */
extern	struct	damage_mess	*damage_table	[ATTACK_MAX];
extern	struct	damage_mess	*death_table	[ATTACK_MAX];
extern	struct	weather_mess	*weather_table	[SECT_MAX];


extern	struct	race_data	*race_array	[MAX_RACE];

/*
 * Global constants.
 */
extern	const	struct	class_type	class_table	[MAX_CLASS];
extern	const	char			*clan_table	[MAX_CLAN];
extern	const	struct	cmd_type	cmd_table	[];
extern	const	struct	liq_type	liq_table	[LIQ_MAX];
extern	const	sh_int			sunlight_table	[25];
extern	const	struct	profi_type	profi_table	[MAX_PROFI];
extern	const	struct	skill_type	skill_table	[MAX_SKILL];
extern	const	struct	social_type	social_table	[];
extern	const	struct	adj_data	adjective_table	[MAX_ADJ];
extern	const	struct	coin_data	coin_table	[MAX_COIN];
extern	const	struct	matl_data	material_table	[MAX_MATL];



/*
 * Global variables.
 */
extern		HELP_DATA	  *	help_first;
extern		SHOP_DATA	  *	shop_first;

extern		BAN_DATA	  *	ban_list;
extern		CHAR_DATA	  *	char_list;
extern		CHAR_DATA	  *	saving_npc_list;
extern		CHAR_DATA	  *	combat_list;
extern		DESCRIPTOR_DATA   *	descriptor_list;
extern		NOTE_DATA	  *	note_list;
extern		OBJ_DATA	  *	object_list;

extern		AFFECT_DATA	  *	affect_free;
extern		BAN_DATA	  *	ban_free;
extern		CHAR_DATA	  *	char_free;
extern		DESCRIPTOR_DATA	  *	descriptor_free;
extern		EXTRA_DESCR_DATA  *	extra_descr_free;
extern		NOTE_DATA	  *	note_free;
extern		OBJ_DATA	  *	obj_free;
extern		PC_DATA		  *	pcdata_free;
extern		SKILL_DELAY_DATA  *	skill_delay_free;

extern		char			bug_buf		[];
extern		time_t			current_time;
extern		bool			fLogAll;
extern		FILE *			fpReserve;
extern		char			log_buf		[];
extern		TIME_INFO_DATA		time_info;
extern		WEATHER_DATA		weather_info;




/*
 * Command functions.
 * Defined in act_*.c (mostly).
 */
DECLARE_DO_FUN(	do_advance	);
DECLARE_DO_FUN(	do_allow	);
DECLARE_DO_FUN(	do_approve	);
DECLARE_DO_FUN(	do_areas	);
DECLARE_DO_FUN(	do_aset		);
DECLARE_DO_FUN(	do_at		);
DECLARE_DO_FUN(	do_backstab	);
DECLARE_DO_FUN(	do_bash		);
DECLARE_DO_FUN(	do_bamfin	);
DECLARE_DO_FUN(	do_bamfout	);
DECLARE_DO_FUN(	do_ban		);
DECLARE_DO_FUN(	do_break	);
DECLARE_DO_FUN(	do_brief	);
DECLARE_DO_FUN(	do_bug		);
DECLARE_DO_FUN(	do_buy		);
DECLARE_DO_FUN(	do_cast		);
DECLARE_DO_FUN( do_change	);
DECLARE_DO_FUN(	do_channels	);
DECLARE_DO_FUN(	do_chat		);
DECLARE_DO_FUN(	do_cload	);
DECLARE_DO_FUN(	do_close	);
DECLARE_DO_FUN(	do_collaps	);
DECLARE_DO_FUN(	do_collapse	);
DECLARE_DO_FUN(	do_commands	);
DECLARE_DO_FUN(	do_compact	);
DECLARE_DO_FUN(	do_compare	);
DECLARE_DO_FUN(	do_config	);
DECLARE_DO_FUN(	do_connect	);
DECLARE_DO_FUN(	do_consider	);
DECLARE_DO_FUN( do_contact      );
DECLARE_DO_FUN(	do_credits	);
DECLARE_DO_FUN(	do_deny		);
DECLARE_DO_FUN(	do_description	);
DECLARE_DO_FUN(	do_dig		);
DECLARE_DO_FUN(	do_disarm	);
DECLARE_DO_FUN(	do_disconnect	);
DECLARE_DO_FUN(	do_dismount	);
DECLARE_DO_FUN(	do_down		);
DECLARE_DO_FUN( do_downtime     );
DECLARE_DO_FUN(	do_draw		);
DECLARE_DO_FUN(	do_drink	);
DECLARE_DO_FUN(	do_drive	);
DECLARE_DO_FUN(	do_drop		);
DECLARE_DO_FUN(	do_east		);
DECLARE_DO_FUN(	do_eat		);
DECLARE_DO_FUN(	do_echo		);
DECLARE_DO_FUN(	do_emote	);
DECLARE_DO_FUN( do_enter	);
DECLARE_DO_FUN(	do_equipment	);
DECLARE_DO_FUN(	do_examine	);
DECLARE_DO_FUN(	do_exits	);
DECLARE_DO_FUN(	do_export	);
DECLARE_DO_FUN(	do_exset	);
DECLARE_DO_FUN(	do_fill		);
DECLARE_DO_FUN(	do_flee		);
DECLARE_DO_FUN(	do_flora	);
DECLARE_DO_FUN(	do_follow	);
DECLARE_DO_FUN(	do_force	);
DECLARE_DO_FUN(	do_freeze	);
DECLARE_DO_FUN(	do_get		);
DECLARE_DO_FUN(	do_give		);
DECLARE_DO_FUN(	do_newgoto      );
DECLARE_DO_FUN(	do_group	);
DECLARE_DO_FUN(	do_gtell	);
DECLARE_DO_FUN(	do_help		);
DECLARE_DO_FUN(	do_hide		);
DECLARE_DO_FUN(	do_hold		);
DECLARE_DO_FUN( do_holylight	);
DECLARE_DO_FUN( do_holylist     );
DECLARE_DO_FUN( do_holyname     );
DECLARE_DO_FUN( do_holyspeech   );
DECLARE_DO_FUN( do_holywalk     );
DECLARE_DO_FUN( do_holyvnum     );
DECLARE_DO_FUN( do_afk          );
DECLARE_DO_FUN(	do_hset		);
DECLARE_DO_FUN( do_history      );
DECLARE_DO_FUN(	do_idea		);
DECLARE_DO_FUN(	do_immtalk	);
DECLARE_DO_FUN(	do_import	);
DECLARE_DO_FUN(	do_inventory	);
DECLARE_DO_FUN(	do_invis	);
DECLARE_DO_FUN(	do_jump		);
DECLARE_DO_FUN(	do_junk		);
DECLARE_DO_FUN(	do_kick		);
DECLARE_DO_FUN(	do_kill		);
DECLARE_DO_FUN( do_knock        );
DECLARE_DO_FUN( do_leave	);
DECLARE_DO_FUN(	do_list		);
DECLARE_DO_FUN( do_load         );
DECLARE_DO_FUN(	do_lock		);
DECLARE_DO_FUN(	do_log		);
DECLARE_DO_FUN(	do_look		);
DECLARE_DO_FUN(	do_material	);
DECLARE_DO_FUN(	do_memory	);
DECLARE_DO_FUN(	do_mfind	);
DECLARE_DO_FUN(	do_mload	);
DECLARE_DO_FUN(	do_mpset	);
DECLARE_DO_FUN(	do_mset		);
DECLARE_DO_FUN(	do_mstat	);
DECLARE_DO_FUN(	do_mwhere	);
DECLARE_DO_FUN(	do_newhelp	);
DECLARE_DO_FUN(	do_newmob	);
DECLARE_DO_FUN(	do_newobj	);
DECLARE_DO_FUN(	do_nmodify	);
DECLARE_DO_FUN(	do_noemote	);
DECLARE_DO_FUN(	do_north	);
DECLARE_DO_FUN(	do_note		);
DECLARE_DO_FUN(	do_notell	);
DECLARE_DO_FUN(	do_ofind	);
DECLARE_DO_FUN(	do_oload	);
DECLARE_DO_FUN(	do_ooc		);
DECLARE_DO_FUN(	do_open		);
DECLARE_DO_FUN(	do_opset	);
DECLARE_DO_FUN(	do_order	);
DECLARE_DO_FUN(	do_oset		);
DECLARE_DO_FUN(	do_ostat	);
DECLARE_DO_FUN(	do_password	);
DECLARE_DO_FUN(	do_peace	);
DECLARE_DO_FUN(	do_peaceful	);
DECLARE_DO_FUN(	do_pemote	);
DECLARE_DO_FUN(	do_pick		);
DECLARE_DO_FUN(	do_pour		);
DECLARE_DO_FUN(	do_prompt	);
DECLARE_DO_FUN(	do_purge	);
DECLARE_DO_FUN(	do_put		);
DECLARE_DO_FUN(	do_quaff	);
DECLARE_DO_FUN(	do_qui		);
DECLARE_DO_FUN(	do_quit		);
DECLARE_DO_FUN(	do_randarmor	);
DECLARE_DO_FUN(	do_randmagic	);
DECLARE_DO_FUN(	do_randpotion	);
DECLARE_DO_FUN(	do_rdesc	);
DECLARE_DO_FUN(	do_reboo	);
DECLARE_DO_FUN(	do_reboot	);
DECLARE_DO_FUN(	do_recall	);
DECLARE_DO_FUN(	do_recho	);
DECLARE_DO_FUN(	do_recite	);
DECLARE_DO_FUN(	do_reject	);
DECLARE_DO_FUN(	do_remove	);
DECLARE_DO_FUN(	do_reply	);
DECLARE_DO_FUN(	do_rescue	);
DECLARE_DO_FUN(	do_reset	);
DECLARE_DO_FUN(	do_rest		);
DECLARE_DO_FUN(	do_restore	);
DECLARE_DO_FUN(	do_return	);
DECLARE_DO_FUN(	do_rextra	);
DECLARE_DO_FUN(	do_ride		);
DECLARE_DO_FUN(	do_rlist	);
DECLARE_DO_FUN(	do_rload	);
DECLARE_DO_FUN( do_pload        );
DECLARE_DO_FUN( do_punload      );
DECLARE_DO_FUN(	do_rset		);
DECLARE_DO_FUN(	do_rstat	);
DECLARE_DO_FUN(	do_run		);
DECLARE_DO_FUN(	do_sacrifice	);
DECLARE_DO_FUN(	do_save		);
DECLARE_DO_FUN(	do_savehelps	);
DECLARE_DO_FUN(	do_savezone	);
DECLARE_DO_FUN(	do_say		);
DECLARE_DO_FUN(	do_score	);
DECLARE_DO_FUN(	do_sector	);
DECLARE_DO_FUN(	do_sell		);
DECLARE_DO_FUN( do_semote       );
DECLARE_DO_FUN(	do_send		);
DECLARE_DO_FUN(	do_set		);
DECLARE_DO_FUN(	do_sheath	);
DECLARE_DO_FUN(	do_shoot	);
DECLARE_DO_FUN(	do_shopset	);
DECLARE_DO_FUN(	do_shout	);
DECLARE_DO_FUN(	do_shutdow	);
DECLARE_DO_FUN(	do_shutdown	);
DECLARE_DO_FUN(	do_silence	);
DECLARE_DO_FUN(	do_sit		);
DECLARE_DO_FUN(	do_skills	);
DECLARE_DO_FUN(	do_skin		);
DECLARE_DO_FUN(	do_sla		);
DECLARE_DO_FUN(	do_slay		);
DECLARE_DO_FUN(	do_sleep	);
DECLARE_DO_FUN(	do_slookup	);
DECLARE_DO_FUN(	do_sneak	);
DECLARE_DO_FUN(	do_snoop	);
DECLARE_DO_FUN(	do_socials	);
DECLARE_DO_FUN(	do_south	);
DECLARE_DO_FUN(	do_spells	);
DECLARE_DO_FUN( do_spemote      );
DECLARE_DO_FUN(	do_sset		);
DECLARE_DO_FUN(	do_stand	);
DECLARE_DO_FUN(	do_steal	);
DECLARE_DELAY_FUN( predelay_steal );
DECLARE_DO_FUN(	do_switch	);
DECLARE_DO_FUN( do_tele         );
DECLARE_DO_FUN(	do_tell		);
DECLARE_DO_FUN(	do_time		);
DECLARE_DO_FUN(	do_title	);
DECLARE_DO_FUN(	do_transfer	);
DECLARE_DO_FUN(	do_trust	);
DECLARE_DO_FUN(	do_typo		);
DECLARE_DO_FUN( do_unlink       );
DECLARE_DO_FUN(	do_unlock	);
DECLARE_DO_FUN(	do_up		);
DECLARE_DO_FUN( do_use		);
DECLARE_DO_FUN(	do_users	);
DECLARE_DO_FUN( do_ulog         );
DECLARE_DO_FUN(	do_value	);
DECLARE_DO_FUN(	do_viewskills	);
DECLARE_DO_FUN(	do_visible	);
DECLARE_DO_FUN(	do_vmob		);
DECLARE_DO_FUN(	do_vobj		);
DECLARE_DO_FUN(	do_wake		);
DECLARE_DO_FUN(	do_walk		);
DECLARE_DO_FUN(	do_wear		);
DECLARE_DO_FUN( do_weather      );
DECLARE_DO_FUN(	do_wset     );
DECLARE_DO_FUN(	do_west		);
DECLARE_DO_FUN(	do_where	);
DECLARE_DO_FUN(	do_who		);
DECLARE_DO_FUN(	do_wield	);
DECLARE_DO_FUN(	do_wish		);
DECLARE_DO_FUN(	do_wizhelp	);
DECLARE_DO_FUN(	do_wizlock	);
DECLARE_DO_FUN(	do_xload	);



/*
 * Spell functions.
 * Defined in magic.c.
 */
DECLARE_SPELL_FUN(	spell_null		);
DECLARE_SPELL_FUN(	spell_armor		);
DECLARE_SPELL_FUN(	spell_bless		);
DECLARE_SPELL_FUN(	spell_blindness		);
DECLARE_SPELL_FUN(	spell_call_lightning	);
DECLARE_SPELL_FUN(	spell_charm_person	);
DECLARE_SPELL_FUN(	spell_control_weather	);
DECLARE_SPELL_FUN(	spell_create_food	);
DECLARE_SPELL_FUN(	spell_create_water	);
DECLARE_SPELL_FUN(	spell_cure_blindness	);
DECLARE_SPELL_FUN(	spell_cure_poison	);
DECLARE_SPELL_FUN(	spell_curse		);
DECLARE_SPELL_FUN(	spell_detect_evil	);
DECLARE_SPELL_FUN(	spell_detect_invis	);
DECLARE_SPELL_FUN(	spell_detect_magic	);
DECLARE_SPELL_FUN(	spell_detect_poison	);
DECLARE_SPELL_FUN(	spell_dispel_evil	);
DECLARE_SPELL_FUN(	spell_dispel_magic	);
DECLARE_SPELL_FUN(	spell_earthquake	);
DECLARE_SPELL_FUN(	spell_enchant_weapon	);
DECLARE_SPELL_FUN(	spell_fireball		);
DECLARE_SPELL_FUN(	spell_flash		);
DECLARE_SPELL_FUN(	spell_fly		);
DECLARE_SPELL_FUN(	spell_gate		);
DECLARE_SPELL_FUN(	spell_harm		);
DECLARE_SPELL_FUN(	spell_heal		);
DECLARE_SPELL_FUN(	spell_identify		);
DECLARE_SPELL_FUN(	spell_infravision	);
DECLARE_SPELL_FUN(	spell_invis		);
DECLARE_SPELL_FUN(	spell_lightning_bolt	);
DECLARE_SPELL_FUN(	spell_locate_object	);
DECLARE_SPELL_FUN(	spell_pebble		);
DECLARE_SPELL_FUN(	spell_pass_door		);
DECLARE_SPELL_FUN(	spell_poison		);
DECLARE_SPELL_FUN(	spell_protection_from_evil	);
DECLARE_SPELL_FUN(	spell_remove_curse	);
DECLARE_SPELL_FUN(	spell_sanctuary		);
DECLARE_SPELL_FUN(	spell_sense_life	);
DECLARE_SPELL_FUN(	spell_sleep		);
DECLARE_SPELL_FUN(	spell_stone_skin	);
DECLARE_SPELL_FUN(	spell_strength		);
DECLARE_SPELL_FUN(	spell_summon		);
DECLARE_SPELL_FUN(	spell_teleport		);
DECLARE_SPELL_FUN(	spell_vampiric_touch	);
DECLARE_SPELL_FUN(	spell_ventriloquate	);
DECLARE_SPELL_FUN(	spell_vitality		);
DECLARE_SPELL_FUN(	spell_weaken		);
DECLARE_SPELL_FUN(	spell_word_of_recall	);
DECLARE_SPELL_FUN(	spell_acid_breath	);
DECLARE_SPELL_FUN(	spell_fire_breath	);
DECLARE_SPELL_FUN(	spell_frost_breath	);
DECLARE_SPELL_FUN(	spell_gas_breath	);
DECLARE_SPELL_FUN(	spell_lightning_breath	);



/*
 * OS-dependent declarations.
 * These are all very standard library functions,
 *   but some systems have incomplete or non-ansi header files.
 */
#if	defined(_AIX)
char *	crypt		args( ( const char *key, const char *salt ) );
#endif

#if	defined(apollo)
int	atoi		args( ( const char *string ) );
int     atol            args( ( const char *string ) );
void *	calloc		args( ( unsigned nelem, size_t size ) );
char *	crypt		args( ( const char *key, const char *salt ) );
#endif

#if	defined(hpux)
char *	crypt		args( ( const char *key, const char *salt ) );
#endif

#if	defined(interactive)
#endif

#if	defined(linux)
char *	crypt		args( ( const char *key, const char *salt ) );
#endif

#if	defined(macintosh)
#define NOCRYPT
#if	defined(unix)
#undef	unix
#endif
#endif

#if	defined(MIPS_OS)
char *	crypt		args( ( const char *key, const char *salt ) );
#endif

#if	defined(MSDOS)
#define NOCRYPT
#if	defined(unix)
#undef	unix
#endif
#endif

#if	defined(NeXT)
char *	crypt		args( ( const char *key, const char *salt ) );
#endif

#if	defined(sequent)
char *	crypt		args( ( const char *key, const char *salt ) );
int	fclose		args( ( FILE *stream ) );
int	fprintf		args( ( FILE *stream, const char *format, ... ) );
int	fread		args( ( void *ptr, int size, int n, FILE *stream ) );
int	fseek		args( ( FILE *stream, long offset, int ptrname ) );
void	perror		args( ( const char *s ) );
int	ungetc		args( ( int c, FILE *stream ) );
#endif

#if	defined(sun)
char *	crypt		args( ( const char *key, const char *salt ) );
int	fclose		args( ( FILE *stream ) );
int	fprintf		args( ( FILE *stream, const char *format, ... ) );
#if 	defined(SYSV)
size_t 	fread		args( ( void *ptr, size_t size, size_t n, 
				FILE *stream ) );
#else
int	fread		args( ( void *ptr, int size, int n, FILE *stream ) );
#endif
int	fseek		args( ( FILE *stream, long offset, int ptrname ) );
void	perror		args( ( const char *s ) );
int	ungetc		args( ( int c, FILE *stream ) );
#endif

#if	defined(ultrix)
char *	crypt		args( ( const char *key, const char *salt ) );
#endif



/*
 * The crypt(3) function is not available on some operating systems.
 * In particular, the U.S. Government prohibits its export from the
 *   United States to foreign countries.
 * Turn on NOCRYPT to keep passwords in plain text.
 */
#if	defined(NOCRYPT)
#define crypt(s1, s2)	(s1)
#endif



/*
 * Data files used by the server.
 *
 * AREA_LIST contains a list of areas to boot.
 * All files are read in completely at bootup.
 * Most output files (bug, idea, typo, shutdown) are append-only.
 *
 * The NULL_FILE is held open so that we have a stream handle in reserve,
 *   so players can go ahead and telnet to all the other descriptors.
 * Then we close it whenever we need to open a file (e.g. a save file).
 */

#define ACCOUNT_DIR     "../accounts/"         /* Account Files */
#define AREA_DIR        "../area/"    /* Area Dir */
#define PLAYER_DIR	"../plr/"		/* Player files	*/
#define UNAPPROVED_DIR	"../newplr/" 	/* Player files */
#define REJECTED_DIR	"../rejplr/" 	/* Player files */
#define LOG_DIR         "../log/"       /* Log Files */
#define NULL_FILE	"/dev/null"	/* To reserve one stream	*/

#define AREA_LIST	"area.lst"	/* List of areas		*/
#define SECTOR_FILE	"sectors.txt"

#define FLORA_CLASS_FILE	"flora_classes.txt"
#define FLORA_SPECIES_FILE	"flora_species.txt"

#define NAME_DIR	"../names/"
#define NAME_FILE	"names.lst"

#define RACE_DIR	"../races/"
#define RACE_FILE	"races.lst"

#define ROOM_SAVE_DIR	"../rmsave/"

#define ENVIRONMENT_DIR	"../enviro/"

#define BUG_FILE	"bugs.txt"      /* For 'bug' and bug( )		*/
#define IDEA_FILE	"ideas.txt"	/* For 'idea'			*/
#define TYPO_FILE	"typos.txt"     /* For 'typo'			*/
#define NOTE_FILE	"notes.txt"	/* For 'notes'			*/
#define STAT_FILE       "status.txt"    /* For 'status' on website      */
#define BOOT_FILE       "boot.txt"      /* For 'boot status' on website */
#define MOTD_FILE       "motd.txt"      /* For 'motd' on website        */
#define SHUTDOWN_FILE	"shutdown.txt"	/* For 'shutdown'		*/



/*
 * Our function prototypes.
 * One big lump ... this is every function in Merc.
 */
#define CD	CHAR_DATA
#define MID	MOB_INDEX_DATA
#define OD	OBJ_DATA
#define OID	OBJ_INDEX_DATA
#define RID	ROOM_INDEX_DATA
#define SF	SPEC_FUN

/* act_comm.c */
void	add_follower	args( ( CHAR_DATA *ch, CHAR_DATA *master ) );
void	stop_follower	args( ( CHAR_DATA *ch ) );
void	die_follower	args( ( CHAR_DATA *ch ) );
bool	is_same_group	args( ( CHAR_DATA *ach, CHAR_DATA *bch ) );

/* act_info.c */
void	set_title	args( ( CHAR_DATA *ch, char *title ) );

/* act_move.c */
int	get_direction	args( ( char *arg ) );
void	move_char	args( ( CHAR_DATA *ch, int door ) );

/* act_obj.c */
int	wear_loc_lookup	args( ( char *arg ) );

/* act_wiz.c */
int	p2		args( ( int in ) );
int	get_pow_2	args( ( int in ) );

/* change.c */
inline void	clear_dynamic	args( ( CHAR_DATA *ch ) );

/* comm.c */
void	close_socket	args( ( DESCRIPTOR_DATA *dclose ) );
void	write_to_buffer	args( ( DESCRIPTOR_DATA *d, const char *txt,
			    int length ) );
void	send_to_char	args( ( const char *txt, CHAR_DATA *ch ) );
void	act		args( ( const char *format, CHAR_DATA *ch,
			    CHAR_DATA *vch, OBJ_DATA *obj, const void *arg1,
			    const void *arg2, int type, int sense ) );
void	show_string	args( ( DESCRIPTOR_DATA *d, char *input ) );

/* db.c */
void	append_file	args( ( CHAR_DATA *ch, char *file, char *str ) );
void	append_stat	args( (  char *file, char *str ) );
void	area_update	args( ( void ) );
void	boot_db		args( ( void ) );
void	bug		args( ( const char *str, int param ) );
char *	capitalize	args( ( const char *str ) );
void	clear_char	args( ( CHAR_DATA *ch ) );
CD *	create_mobile	args( ( MOB_INDEX_DATA *pMobIndex ) );
OD *	create_object	args( ( OBJ_INDEX_DATA *pObjIndex ) );
char	fread_letter	args( ( FILE *fp ) );
int	fread_number	args( ( FILE *fp ) );
char *	fread_description	args( ( FILE *fp ) );
char *	fread_string	args( ( FILE *fp ) );
char *	fread_string_full	args( ( FILE *fp, char terminator,
                                        bool fKillSpace ) );
void	fread_to_eol	args( ( FILE *fp ) );
char *	fread_word	args( ( FILE *fp ) );
void	free_char	args( ( CHAR_DATA *ch ) );
void	free_exit	args( ( EXIT_DATA *exit ) );
void	free_extra_descr	args( ( EXTRA_DESCR_DATA *ed ) );
void	free_predelay	args( ( PREDELAY_DATA *p ) );
void	free_room	args( ( ROOM_INDEX_DATA *room ) );
void	free_sector_type args( ( struct sector_type *s ) );
void	free_shop	args( ( SHOP_DATA *pShop ) );
char *	get_extra_descr	args( ( const char *name, EXTRA_DESCR_DATA *ed ) );
MID *	get_mob_index	args( ( int vnum ) );
CD *  get_mob_world  args ( ( CHAR_DATA *ch, char *argument ) );
CD *  get_mob_by_vnum args ( ( CHAR_DATA *ch, char *argument ) );
OID *	get_obj_index	args( ( int vnum ) );
RID *	get_room_index	args( ( int vnum ) );
void	log_string	args( ( const char *str ) );
EXIT_DATA * new_exit	args( ( void ) );
EXTRA_DESCR_DATA * new_extra_descr args( ( void ) );
PREDELAY_DATA * new_predelay args( ( void ) );
struct sector_type *new_sector_type args( ( void ) );
SHOP_DATA * new_shop	args( ( void ) );
void	smash_tilde	args( ( char *str ) );
void    start_update    args( ( void ) );
bool	str_cmp		args( ( const char *astr, const char *bstr ) );
bool	str_prefix	args( ( const char *astr, const char *bstr ) );
bool	str_infix	args( ( const char *astr, const char *bstr ) );
bool	str_suffix	args( ( const char *astr, const char *bstr ) );
void	tail_chain	args( ( void ) );

/* emote.c */
char *	emote_processor	args( ( CHAR_DATA *ch, char *argument, bool fPemote, bool fSemote, 
                            CHAR_DATA *vch ) );

/* environment.c */
int	get_water_value	args( ( ROOM_INDEX_DATA *room ) );
void	load_environment args( ( void ) );
void	soil_condition	args( ( CHAR_DATA *ch, int condition ) );
void	update_soil	args( ( ROOM_INDEX_DATA *room ) );
void	write_environment args( ( bool fAll ) );
void	write_environment_area args( ( AREA_DATA *area ) );

/* fight.c */
int	check_fighting	args( ( CHAR_DATA *ch ) );
void	damage		args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam,
			    int dt, OBJ_DATA *obj, int location ) );
void	damage_from_object args( ( CHAR_DATA *ch, OBJ_DATA *obj, int dam,
			    int sn ) );
void	damage_item	args( ( OBJ_DATA *obj, int dam, bool fUsed,
			    bool fPropigate ) );
void	one_hit		args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ) );
void	stop_fighting	args( ( CHAR_DATA *ch, bool fBoth ) );
void	update_noquit	args( ( CHAR_DATA *ch ) );
void	update_pos	args( ( CHAR_DATA *victim ) );
void	violence_update	args( ( void ) );

/* flora.c */
void	destroy_flora	args( ( FLORA_DATA *fi, ROOM_INDEX_DATA *room ) );
bool	display_flora	args( ( CHAR_DATA *ch, FLORA_DATA *fi, int idist ) );
struct flora_species *flora_species_lookup args( ( char *name ) );
void	free_flora_instance args( ( FLORA_DATA *fi ) );
FLORA_DATA *get_flora_here args( ( CHAR_DATA *ch, char *arg ) );
char *	get_plant_sdesc	args( ( FLORA_DATA *fi ) );
void	load_flora	args( ( void ) );   /* load definitions */
void	load_world_flora args( ( void ) );  /* load current state */
FLORA_DATA *new_flora_instance args( ( void ) );
void	show_plant	args( ( CHAR_DATA *ch, struct flora_instance *fi ) );
void	update_flora	args( ( ROOM_INDEX_DATA *room ) );
void	update_flower	args( ( FLORA_DATA *fi ) );
void	update_fruit	args( ( FLORA_DATA *fi ) );
void	write_flora_instance args( ( FILE *fp, FLORA_DATA *fi ) );

/* handler.c */
char *	affect_loc_name	args( ( int location ) );
char *	affect_bit_name	args( ( int vector ) );
void	affect_join	args( ( CHAR_DATA *ch, AFFECT_DATA *paf ) );
void	affect_to_char	args( ( CHAR_DATA *ch, AFFECT_DATA *paf ) );
void	affect_remove	args( ( CHAR_DATA *ch, AFFECT_DATA *paf ) );
void	affect_strip	args( ( CHAR_DATA *ch, int sn ) );
int	can_carry_n	args( ( CHAR_DATA *ch ) );
int	can_carry_w	args( ( CHAR_DATA *ch ) );
//sh_int	can_see		args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool    can_see         args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool	can_see_obj	args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );
bool	can_drop_obj	args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );
void	char_from_room	args( ( CHAR_DATA *ch ) );
void	char_to_room	args( ( CHAR_DATA *ch, ROOM_INDEX_DATA *pRoomIndex,
			 bool fSave ) );
int	count_obj_list	args( ( OBJ_INDEX_DATA *obj, OBJ_DATA *list ) );
void	delay_to_char	args( ( CHAR_DATA *ch, SKILL_DELAY_DATA *delay ) );
void	delay_remove	args( ( CHAR_DATA *ch, SKILL_DELAY_DATA *delay ) );
void	equip_char	args( ( CHAR_DATA *ch, OBJ_DATA *obj, int iWear ) );
char *	extra_bit_name	args( ( int extra_flags ) );
void	extract_obj	args( ( OBJ_DATA *obj, bool fDest ) );
void	extract_char	args( ( CHAR_DATA *ch, bool fPull ) );
int	get_age		args( ( CHAR_DATA *ch ) );
int	get_aggr_factor	args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
CD *	get_char_room	args( ( CHAR_DATA *ch, char *argument ) );
CD *	get_char_world	args( ( CHAR_DATA *ch, char *argument ) );
CD *   get_pc_world         args( ( CHAR_DATA *ch, char *argument ) );
int	get_curr_str	args( ( CHAR_DATA *ch ) );
int	get_curr_int	args( ( CHAR_DATA *ch ) );
int	get_curr_dex	args( ( CHAR_DATA *ch ) );
int	get_curr_tou	args( ( CHAR_DATA *ch ) );
int	get_curr_qui	args( ( CHAR_DATA *ch ) );
OD *	get_eq_char	args( ( CHAR_DATA *ch, int iWear ) );
OD *	get_eq_char_hit_loc	args( ( CHAR_DATA *ch, int hit_loc ) );
int	get_hours_played args( ( CHAR_DATA *ch ) );
OD *	get_obj_type	args( ( OBJ_INDEX_DATA *pObjIndexData ) );
OD *	get_obj_list	args( ( CHAR_DATA *ch, char *argument,
			    OBJ_DATA *list ) );
OD *	get_obj_carry	args( ( CHAR_DATA *ch, char *argument,
			    CHAR_DATA *viewer ) );
OD *	get_obj_wear	args( ( CHAR_DATA *ch, char *argument,
			    CHAR_DATA *viewer ) );
OD *	get_obj_here	args( ( CHAR_DATA *ch, char *argument ) );
OD *	get_obj_world	args( ( CHAR_DATA *ch, char *argument ) );
int	get_obj_number	args( ( OBJ_DATA *obj ) );
int	get_obj_weight	args( ( OBJ_DATA *obj ) );
int	get_power_level	args( ( CHAR_DATA *ch, int sn ) );
int	get_skill	args( ( CHAR_DATA *ch, int sn ) );
int	get_trust	args( ( CHAR_DATA *ch ) );
bool	is_affected	args( ( CHAR_DATA *ch, int sn ) );
bool	is_delayed	args( ( CHAR_DATA *ch, int sn ) );
bool	is_name		args( ( const char *str, char *namelist ) );
bool	is_vowel	args( ( char c ) );
char *	item_type_name	args( ( OBJ_DATA *obj ) );
void	obj_to_char	args( ( OBJ_DATA *obj, CHAR_DATA *ch ) );
void	obj_from_char	args( ( OBJ_DATA *obj ) );
void	obj_from_room	args( ( OBJ_DATA *obj ) );
void	obj_to_room	args( ( OBJ_DATA *obj, ROOM_INDEX_DATA *pRoomIndex,
			 bool fSave ) );
void	obj_to_obj	args( ( OBJ_DATA *obj, OBJ_DATA *obj_to ) );
void	obj_from_obj	args( ( OBJ_DATA *obj ) );
void    modify_light    args( ( CHAR_DATA *ch, OBJ_DATA *obj, bool fEquip ) );
char *	person		args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void	remove_dependent_items args( ( CHAR_DATA *ch, int wear_loc ) );
int	room_is_dark	args( ( ROOM_INDEX_DATA *pRoomIndex, CHAR_DATA *ch ) );
int	room_light	args( ( ROOM_INDEX_DATA *pRoomIndex ) );
bool	room_is_private	args( ( ROOM_INDEX_DATA *pRoomIndex ) );
void	set_predelay	args( ( CHAR_DATA *ch, int delay, DO_FUN *fnptr,
			 char *argument, int number, CHAR_DATA *victim1,
			 CHAR_DATA *victim2, OBJ_DATA *obj1, OBJ_DATA *obj2 ) );
void	unequip_char	args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );

/* interp.c */
bool	check_specials	args( ( CHAR_DATA *ch, char *cmd, DO_FUN *fnptr,
				char *argument ) );
void	interpret	args( ( CHAR_DATA *ch, char *argument ) );
bool	is_number	args( ( char *arg ) );
int	number_argument	args( ( char *argument, char *arg ) );
char *	one_argument	args( ( char *argument, char *arg_first ) );
char *	plural		args( ( char *arg ) );

/* emote.c */
char * one_emote_argument args( ( char *argument, char *arg_first ) );
char *strip_punct args(( char *arg ));

/* languages.c */
int	language_lookup	args( ( char *arg ) );
void	language_to_char	args( ( CHAR_DATA *ch, CHAR_DATA *vch,
                                        char *arg, int door, char *method,
					bool fLearn ) );
void    language_tell_to_char   args( ( CHAR_DATA *ch, CHAR_DATA *vch,
                                        CHAR_DATA *victim, char *arg,
                                        char *method, bool fLearn ) );
char	*scramble_language	args( ( int skill, int language, char *arg ) );

/* limited.c */
void	load_equipment_totals args( ( void ) );

/* magic.c */
int	skill_lookup	args( ( const char *name ) );
int	slot_lookup	args( ( int slot ) );
bool	saves_spell	args( ( int level, CHAR_DATA *victim ) );
void	obj_cast_spell	args( ( int sn, int level, CHAR_DATA *ch,
				    CHAR_DATA *victim, OBJ_DATA *obj ) );

/* mem_manage.c */
void *	alloc_mem	args( ( int sMem ) );
void *	alloc_perm	args( ( int sMem ) );
bool	find_string_references args( ( char *pstr, int mem_type ) );
void	free_mem	args( ( void *pMem, int sMem ) );
void	free_string	args( ( char *pstr, int mem_type ) );
void	init_string_space args( ( void ) );
char *	str_dup		args( ( const char *str ) );
char *	str_dup_tab	args( ( const char *str, bool fStrip ) );

/* memory.c */
bool	is_hunting	args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void	remove_hunting	args( ( CHAR_DATA *ch, CHAR_DATA *victim,
				bool fDead ) );
void	restore_hunting	args( ( CHAR_DATA *ch ) );
void	set_hunting	args( ( CHAR_DATA *ch, CHAR_DATA *victim,
			 int reason, int time ) );

/* money.c */
int	check_money	args( ( CHAR_DATA *ch, SHOP_DATA *pShop, bool fBuy ) );
OD *	create_money	args( ( int amount, int type ) );
void	gain_coin	args( ( CHAR_DATA *ch, int amount, int coin,
				bool fShow, char *outbuf ) );
int	get_coin_index	args( ( char *string ) );
int	get_coin_weight	args( ( int amount, int type ) );
void	lose_coin	args( ( CHAR_DATA *ch, int amount, int coin,
				bool fShow, char *outbuf ) );
void	transfer_money	args( ( CHAR_DATA *ch, CHAR_DATA *victim, int amount,
				SHOP_DATA *pShop, bool fBuy, char *order,
				bool fToRecipient, bool fToSource,
				char *outbuf ) );

/* names.c */
void	assign_name	args( ( CHAR_DATA *ch ) );
void	load_names	args( ( void ) );

/* obj_fun.c */
OBJ_FUN * obj_fun_lookup args( ( const char *name ) );

/* olc.c */
bool	can_build	args( ( CHAR_DATA *ch, AREA_DATA *pArea ) );
RID *	new_room	args( ( AREA_DATA *pArea, int vnum ) );
int	parse_location	args( ( char *argument ) );
void	write_string	args( ( FILE *fp, char *str ) );
int	sector_type_lookup	args( ( char *arg ) );
bool	save_zone_file	args( ( AREA_DATA *pArea ) );

/* races.c */
void	load_races	args( ( void ) );
int	race_lookup	args( ( char *name ) );
int	relative_height	args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );

void do_classupdate     args( ( CHAR_DATA *ch) );
/* random.c */
void	check_random_equip args( ( CHAR_DATA *ch ) );
void	rename_object_random args( ( OBJ_DATA *obj, int table ) );
int	material_lookup	args( ( const char *arg ) );

/* rng.c */
int	dice		args( ( int number, int size ) );
void	init_mm		args( ( void ) );
int	number_fuzzy	args( ( int number ) );
int	number_range	args( ( int from, int to ) );
int	number_percent	args( ( void ) );
int	number_door	args( ( void ) );
int	number_stat	args( ( void ) );
int	number_bits	args( ( int width ) );
int	number_mm	args( ( void ) );
int	lcg_next	args( ( int seed, int range ) );

/* room_fun.c */
ROOM_SPECIAL * room_fun_lookup args( ( const char *name ) );


/* save.c */
void	init_char	args( ( CHAR_DATA *ch, char *name, bool fPC ) );
void	load_char_from_file args( ( CHAR_DATA *ch, FILE *fp ) );
bool	load_char_obj	args( ( DESCRIPTOR_DATA *d, char *name ) );
CD *	new_char	args( ( void ) );
void	read_room	args( ( ROOM_INDEX_DATA *room ) );
void	save_char_obj	args( ( CHAR_DATA *ch ) );
void    save_char_load  args( ( CHAR_DATA *ch ) );
void	save_room	args( ( ROOM_INDEX_DATA *room ) );

/* sectors.c */
void	load_main_sectors  args( ( void ) );
struct sector_type *sector_lookup  args( ( char *arg, AREA_DATA *pArea ) );
void	set_sector    args( ( ROOM_INDEX_DATA *room, struct sector_type *s ) );

/* shoot.c */
CD *    search_dir_name	args( ( CHAR_DATA *ch, char *argument,
		 int direction, int range ) );
/* skills.c */
void	skill_improve	args( ( CHAR_DATA *ch, int sn ) );
void	power_improve	args( ( CHAR_DATA *ch, int sn ) );
void	view_skill_delays	args( ( CHAR_DATA *ch, CHAR_DATA *vch ) );

/* special.c */
SF *	spec_lookup	args( ( const char *name ) );

/* update.c */
void	gain_condition	args( ( CHAR_DATA *ch, int iCond, int value ) );
void	update_handler	args( ( void ) );

#undef	CD
#undef	MID
#undef	OD
#undef	OID
#undef	RID
#undef	SF
