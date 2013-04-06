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
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"

#if !defined(macintosh)
extern	int	_filbuf		args( (FILE *) );
#endif


extern char str_empty[1];

/*
 * Globals.
 */
HELP_DATA *		help_first;
HELP_DATA *		help_last;

SHOP_DATA *		shop_first;
SHOP_DATA *		shop_last;

CHAR_DATA *		char_free;
EXTRA_DESCR_DATA *	extra_descr_free;
NOTE_DATA *		note_free;
OBJ_DATA *		obj_free;
PC_DATA *		pcdata_free;
PREDELAY_DATA *		predelay_free;
ROOM_INDEX_DATA *	room_free;
EXIT_DATA *		exit_free;
struct sector_type *	sector_type_free;

char			bug_buf		[2*MAX_INPUT_LENGTH];
CHAR_DATA *		char_list;
CHAR_DATA *		saving_npc_list;
CHAR_DATA *		combat_list = NULL;
char *			help_greeting;
char *                  help_greeting2;
char *                  help_greeting3;
char			log_buf		[2*MAX_INPUT_LENGTH];
NOTE_DATA *		note_list;
OBJ_DATA *		object_list;
TIME_INFO_DATA		time_info;
WEATHER_DATA		weather_info;

int			environment_interval = 0;

struct sector_type	*sector_type_list = NULL;

struct damage_mess	*death_table[ATTACK_MAX];
struct damage_mess	*damage_table[ATTACK_MAX];

sh_int			wpr_club;
sh_int			wpr_club_heavy;
sh_int			wpr_dagger;
sh_int			wpr_flail;
sh_int			wpr_half_staff;
sh_int			wpr_hammer;
sh_int			wpr_knife;
sh_int			wpr_pole_arms;
sh_int			wpr_scimitar;
sh_int			wpr_spear_long;
sh_int			wpr_spear_short;
sh_int			wpr_staff;
sh_int			wpr_sword_bastard;
sh_int			wpr_sword_broad;
sh_int			wpr_sword_long;
sh_int			wpr_sword_short;
sh_int			wpr_sword_two_handed;

sh_int			wpr_blowgun;
sh_int			wpr_bow_cross;
sh_int			wpr_bow_cross_heavy;
sh_int			wpr_bow_long;
sh_int			wpr_bow_short;

sh_int			wpr_thrown_boomerang;
sh_int			wpr_thrown_dart;
sh_int			wpr_thrown_knife;
sh_int			wpr_thrown_spear;

sh_int			gsn_acid_breath;
sh_int			gsn_archery;
sh_int			gsn_armor;
sh_int			gsn_backstab;
sh_int			gsn_bash;
sh_int			gsn_bleeding;		/* stores damage message */
sh_int			gsn_bless;
sh_int			gsn_blindness;
sh_int			gsn_blud_proficiency;
sh_int			gsn_call_lightning;
sh_int			gsn_charm_person;
sh_int			gsn_climbing;
sh_int                  gsn_contact;
sh_int			gsn_control_weather;
sh_int			gsn_create_food;
sh_int			gsn_create_water;
sh_int			gsn_cure_blindness;
sh_int			gsn_cure_poison;
sh_int			gsn_curse;
sh_int			gsn_detect_evil;
sh_int			gsn_detect_invis;
sh_int			gsn_detect_magic;
sh_int			gsn_detect_poison;
sh_int			gsn_disarm;
sh_int			gsn_dispel_evil;
sh_int			gsn_dispel_magic;
sh_int			gsn_dodge;
sh_int			gsn_dual_wield;
sh_int			gsn_earthquake;
sh_int			gsn_enchant_weapon;
sh_int			gsn_enhanced_damage;
sh_int			gsn_falling;
sh_int			gsn_fire_breath;
sh_int			gsn_fireball;
sh_int			gsn_flash;
sh_int			gsn_fly;
sh_int			gsn_frost_breath;
sh_int			gsn_gas_breath;
sh_int			gsn_gate;
sh_int			gsn_harm;
sh_int			gsn_heal;
sh_int			gsn_hide;
sh_int			gsn_identify;
sh_int			gsn_infravision;
sh_int			gsn_invis;
sh_int			gsn_kick;
sh_int			gsn_lightning_bolt;
sh_int			gsn_lightning_breath;
sh_int			gsn_locate_object;
sh_int			gsn_martial_arts;
sh_int			gsn_noquit;
sh_int			gsn_pass_door;
sh_int			gsn_pebble;
sh_int			gsn_peek;
sh_int			gsn_pick_lock;
sh_int			gsn_pierce_proficiency;
sh_int			gsn_poison;
sh_int			gsn_protection_from_evil;
sh_int			gsn_read_magic;
sh_int			gsn_remove_curse;
sh_int			gsn_rescue;
sh_int			gsn_sanctuary;
sh_int			gsn_sense_life;
sh_int			gsn_slash_proficiency;
sh_int			gsn_sleep;
sh_int			gsn_sneak;
sh_int			gsn_steal;
sh_int			gsn_stone_skin;
sh_int			gsn_strength;
sh_int			gsn_summon;
sh_int			gsn_teleport;
sh_int			gsn_tracking;
sh_int			gsn_tumbling;
sh_int			gsn_use_magical_devices;
sh_int			gsn_vampiric_touch;
sh_int			gsn_ventriloquate;
sh_int			gsn_vitality;
sh_int			gsn_weaken;
sh_int			gsn_word_of_recall;


sh_int			gsn_andean;
sh_int			gsn_elekun;
sh_int			gsn_vedican;
sh_int			gsn_luderwen;
sh_int                  gsn_tongues;
sh_int                  gsn_jikkean;


/*
 * Locals.
 */
MOB_INDEX_DATA *	mob_index_hash		[MAX_KEY_HASH];
OBJ_INDEX_DATA *	obj_index_hash		[MAX_KEY_HASH];
ROOM_INDEX_DATA *	room_index_hash		[MAX_KEY_HASH];

AREA_DATA *		area_first;
AREA_DATA *		area_last;

int			top_affect;
int			top_area;
int			top_ed;
int			top_exit;
int			top_help;
int			top_mob_index;
int			top_obj_index;
int			top_reset;
int			top_room;
int			top_shop;



/*
 * Semi-locals.
 */
bool			fBootDb;
FILE *			fpArea;
char			strArea[MAX_INPUT_LENGTH];



/*
 * Local booting procedures.
 */
void	load_area	args( ( FILE *fp ) );
void	load_helps	args( ( FILE *fp ) );
void	load_mobiles	args( ( FILE *fp ) );
void	load_objects	args( ( FILE *fp ) );
void	load_resets	args( ( FILE *fp ) );
void	load_rooms	args( ( FILE *fp ) );
void	load_notes	args( ( void ) );
void	load_dam_msg	args( ( void ) );
void	load_weat_msg	args( ( void ) );
void	load_saved_rooms args( ( void ) );

void	fix_exits	args( ( void ) );

void	reset_area	args( ( AREA_DATA * pArea ) );



/*
 * Big mama top level function.
 */
void boot_db( void )
{
    init_string_space( );

    fBootDb		= TRUE;

    /*
     * Init random number generator.
     */
    {
	init_mm( );
    }

    /*
     * Set time and weather.
     */
    {
	long lhour, lday, lmonth;

	lhour		= (current_time - 650336715)
			/ (PULSE_TICK / PULSE_PER_SECOND);
	time_info.hour	= lhour  % 24;
	lday		= lhour  / 24;
	time_info.day	= lday   % 35;
	lmonth		= lday   / 35;
	time_info.month	= lmonth % 17;
	time_info.year	= lmonth / 17;

	     if ( time_info.hour <  5 ) weather_info.sunlight = SUN_DARK;
	else if ( time_info.hour <  6 ) weather_info.sunlight = SUN_RISE;
	else if ( time_info.hour < 19 ) weather_info.sunlight = SUN_LIGHT;
	else if ( time_info.hour < 20 ) weather_info.sunlight = SUN_SET;
	else                            weather_info.sunlight = SUN_DARK;

	weather_info.change	= 0;
	weather_info.sky = SKY_CLOUDLESS;

    }

    /*
     * Assign gsn's for skills which have them.
     */
    {
	int sn;

	for ( sn = 0; sn < MAX_SKILL; sn++ )
	{
	    if ( skill_table[sn].pgsn != NULL )
		*skill_table[sn].pgsn = sn;
	}

    }

    /*
     * Assign gn's for wear locations
     */
    {
	int n;
	extern struct wear_data const wear_info[];

	for ( n = 0; n < MAX_WEAR; n++ )
	{
	    if ( wear_info[n].pgn != NULL )
		*wear_info[n].pgn = n;
	}
    }

    /*
     * Assign wprs for profis.
     */
    {
	int sn;

	for ( sn = 0; sn < MAX_PROFI; sn++ )
	{
	    if ( profi_table[sn].pwpr != NULL )
		*profi_table[sn].pwpr = sn;
	}
    }

    /*
     * Load races, sectors.  Needed for areas.
     */
    load_races( );
    load_main_sectors( );
    load_flora( );

    /*
     * Load names.
     */
    load_names( );

    /*
     * Read in all the area files.
     */
    {
	FILE *fpList;

	if ( ( fpList = fopen( AREA_LIST, "r" ) ) == NULL )
	{
	    perror( AREA_LIST );
	    exit( 1 );
	}

	for ( ; ; )
	{
	    strcpy( strArea, fread_word( fpList ) );
	    if ( strArea[0] == '$' )
		break;

	    if ( strArea[0] == '-' )
	    {
		fpArea = stdin;
	    }
	    else
	    {
		if ( ( fpArea = fopen( strArea, "r" ) ) == NULL )
		{
		    perror( strArea );
		    exit( 1 );
		}
	    }
/* debug line */
/*
	sprintf( buf, "[*****] FILE: %s", strArea );
	log_string( buf );
*/

	    for ( ; ; )
	    {
		char *word;

		if ( fread_letter( fpArea ) != '#' )
		{
		    bug( "Boot_db: # not found.", 0 );
		    exit( 1 );
		}

		word = fread_word( fpArea );

		     if ( word[0] == '$'               )                 break;
		else if ( !str_cmp( word, "AREA"     ) ) load_area    (fpArea);
		else if ( !str_cmp( word, "HELPS"    ) ) load_helps   (fpArea);
		else if ( !str_cmp( word, "MOBILES"  ) ) load_mobiles (fpArea);
		else if ( !str_cmp( word, "OBJECTS"  ) ) load_objects (fpArea);
		else if ( !str_cmp( word, "RESETS"   ) ) load_resets  (fpArea);
		else if ( !str_cmp( word, "ROOMS"    ) ) load_rooms   (fpArea);
		else
		{
		    bug( "Boot_db: bad section name.", 0 );
		    exit( 1 );
		}
	    }

	    if ( fpArea != stdin )
		fclose( fpArea );
	    fpArea = NULL;
	}
	fclose( fpList );
    }

    /*
     * Read external messages
     */
    load_dam_msg( );
/*
    load_weat_msg( );
*/

    /*
     *  Load saved contents to rooms that save.
     */
    load_saved_rooms( );
    load_environment( );

    /*
     * Fix up exits.
     * Declare db booting over.
     * Reset all areas once.
     * Load up the notes file.
     */
    {
	fix_exits( );
	fBootDb	= FALSE;
/*	load_equipment_totals( ); */
	start_update( );
        
	load_notes( );
    }

    return;
}


AREA_DATA *new_area( void )
{
  AREA_DATA *pArea;

  pArea			= alloc_perm( sizeof(*pArea) );
  pArea->reset_first	= NULL;
  pArea->reset_last	= NULL;
  pArea->mob_first	= NULL;
  pArea->mob_last	= NULL;
  pArea->room_first	= NULL;
  pArea->room_last	= NULL;
  pArea->obj_first	= NULL;
  pArea->obj_last	= NULL;

  pArea->name		= NULL;
  pArea->filename	= NULL;
  pArea->builders	= NULL;
  pArea->vnum_start	= 0;
  pArea->vnum_final	= 0;
  pArea->reset_length	= 500;
  pArea->area_bits	= 0;

  pArea->sector_list	= NULL;
  pArea->age		= 15;
  pArea->nplayer	= 0;
  /* randomize save time. */
  pArea->env_save_interval	= number_range( 0, 24 );

  if ( area_first == NULL )
    area_first = pArea;
  if ( area_last  != NULL )
    area_last->next = pArea;
  area_last	= pArea;
  pArea->next	= NULL;

  top_area++;
  return pArea;
}


/*
 * Snarf an 'area' header line.
 */
void load_area( FILE *fp )
{
    AREA_DATA *pArea;

    pArea = new_area( );

    pArea->name		= fread_string( fp );
    pArea->filename	= fread_string( fp );
    pArea->builders	= fread_string( fp );
    pArea->vnum_start	= fread_number( fp );
    pArea->vnum_final	= fread_number( fp );
    pArea->reset_length	= fread_number( fp );
    pArea->area_bits	= fread_number( fp );

    return;
}


void write_area_list( void )
{
  char buf[MAX_STRING_LENGTH];
  FILE *fp;
  AREA_DATA *pArea;

  if ( ( fp = fopen( "area.list.tmp", "w" ) ) == NULL )
  {
    log_string( "Error opening area list." );
    return;
  }

  fprintf( fp, "help.are\n" );

  for ( pArea = area_first; pArea != NULL; pArea = pArea->next )
  {
    fprintf( fp, "%s.are\n", pArea->filename );
  }

  fprintf( fp, "$\n" );
  fclose( fp );

  sprintf( buf, "cp %s.are %s", pArea->filename, AREA_LIST );
  system( buf );

  return;
}


void do_aset( CHAR_DATA *ch, char *argument )
{
  char buf[MAX_STRING_LENGTH];
  char arg1[MAX_INPUT_LENGTH];
  char arg2[MAX_INPUT_LENGTH];
  AREA_DATA *pArea;
  int i;

  if ( IS_NPC( ch ) )
    return;

  argument = one_argument( argument, arg1 );

  if ( arg1[0] == '\0' )
  {
    send_to_char( "Syntax: aset builders <builder list>\n\r", ch );
    send_to_char( "Syntax: aset filename <filename>\n\r", ch );
    send_to_char( "Syntax: aset name <Area Name>\n\r", ch );
    send_to_char( "Syntax: aset expand <new final vnum>\n\r", ch );
    send_to_char( "Syntax: aset reset <reset time>\n\r", ch );
    send_to_char( "Syntax: aset bits <area bits>\n\r", ch );
    send_to_char(
      "Syntax: aset new <filename> <vnum start> <vnum final>\n\r", ch );
    return;
  }

  pArea = ch->in_room->area;

  if ( !str_cmp( "builders", arg1 ) )
  {
    if ( pArea == NULL )
    {
      send_to_char( "No area found.\n\r", ch );
      return;
    }

    if ( get_trust( ch ) < MAX_LEVEL )
    {
      send_to_char(
        "You must be level MAX_LEVEL to use this option.\n\r", ch );
      return;
    }

    if ( argument[0] == '\0' )
    {
      send_to_char( "Syntax: aset builders <builder list>\n\r", ch );
      return;
    }

    free_string( pArea->builders, MEM_AREA_BUILDERS );
    pArea->builders = str_dup( argument );

    sprintf( buf, "Builders for %s set to: %s\n\r",
      pArea->name, pArea->builders );
    send_to_char( buf, ch );

    return;
  }

  if ( !str_cmp( "name", arg1 ) )
  {
    if ( pArea == NULL )
    {
      send_to_char( "No area found.\n\r", ch );
      return;
    }

    if ( !can_build( ch, pArea ) )
    {
      send_to_char(
        "You do not have authorization to build in this zone.\n\r", ch );
      return;
    }

    if ( argument[0] == '\0' )
    {
      send_to_char( "Syntax: aset name <Area Name>\n\r", ch );
      return;
    }

    free_string( pArea->name, MEM_AREA_NAME );
    pArea->name = str_dup( argument );

    sprintf( buf, "Name of area set to:  %s\n\r", pArea->name );
    send_to_char( buf, ch );

    return;
  }

  argument = one_argument( argument, arg2 );

  if ( !str_cmp( "new", arg1 ) )
  {
    char arg3[MAX_INPUT_LENGTH];
    char arg4[MAX_INPUT_LENGTH];
    int vnum_start, vnum_end;

    if ( get_trust( ch ) < MAX_LEVEL )
    {
      send_to_char(
        "You must be level MAX_LEVEL to use this option.\n\r", ch );
      return;
    }

    argument = one_argument( argument, arg3 );
    argument = one_argument( argument, arg4 );

    if ( arg2[0] == '\0' || arg3[0] == '\0' || arg4[0] == '\0' )
    {
      send_to_char(
        "Syntax: aset new <filename> <vnum_start> <vnum_end>\n\r", ch );
      return;
    }

    for ( i = strlen( arg2 ) - 1; i >= 0; i-- )
    {
      if ( !isalnum( arg2[i] ) )
      {
        send_to_char( "Only letters or numbers allowed for filename.\n\r", ch);
	send_to_char( ".are is automatically appended by the game.\n\r", ch );
	return;
      }
    }

    vnum_start = atoi( arg3 );
    vnum_end = atoi( arg4 );

    if ( ( vnum_start % 100 ) || ( ( vnum_end + 1 ) % 100 )
    || vnum_end < vnum_start )
    {
      send_to_char(
        "vnum_start must end with 00.\n\rvnum_end must end with 99.\n\r"
        "vnum end must be greater than vnum_start.\n\r", ch );
      return;
    }

    for ( pArea = area_first; pArea != NULL; pArea = pArea->next )
    {
      if ( ( vnum_start >= pArea->vnum_start
          && vnum_start <= pArea->vnum_final )
      || ( vnum_end >= pArea->vnum_start && vnum_end <= pArea->vnum_final ) )
      {
        sprintf( buf, "Vnums specified overlap with %s.\n\r",
	  pArea->name );
        send_to_char( buf, ch );
	return;
      }

      if ( !str_cmp( arg2, pArea->filename ) )
      {
        sprintf( buf, "Filename the same as %s.\n\r",
	  pArea->name );
        send_to_char( buf, ch );
	return;
      }
    }

    pArea = new_area( );
    pArea->name = str_dup( "New Area" );
    pArea->filename = str_dup( arg2 );
    pArea->vnum_start = vnum_start;
    pArea->vnum_final = vnum_end;

    new_room( pArea, vnum_start );

    save_zone_file( pArea );

    write_area_list( );

    send_to_char( "Area created.\n\r", ch );
    return;
  }

  if ( pArea == NULL )
  {
    send_to_char( "No area found.\n\r", ch );
    return;
  }

  if ( !str_cmp( "filename", arg1 ) )
  {
    if ( get_trust( ch ) < MAX_LEVEL )
    {
      send_to_char(
        "You must be level MAX_LEVEL to use this option.\n\r", ch );
      return;
    }

    if ( arg2[0] == '\0' )
    {
      send_to_char( "Syntax: aset filename <filename>\n\r", ch );
      return;
    }

    for ( i = strlen( arg2 ) - 1; i >= 0; i-- )
    {
      if ( !isalnum( arg2[i] ) )
      {
        send_to_char( "Only letters or numbers allowed for filename.\n\r", ch);
	send_to_char( ".are is automatically appended by the game.\n\r", ch );
	return;
      }
    }

    for ( pArea = area_first; pArea != NULL; pArea = pArea->next )
    {
      if ( !str_cmp( arg2, pArea->filename ) )
      {
        sprintf( buf, "Filename the same as %s.\n\r",
	  pArea->name );
        send_to_char( buf, ch );
	return;
      }
    }

    pArea = ch->in_room->area;

    free_string( pArea->filename, MEM_AREA_FILENAME );
    pArea->filename = str_dup( arg2 );

    save_zone_file( pArea );
    write_area_list( );

    sprintf( buf, "Filename for %s set to: %s\n\r",
      pArea->name, pArea->filename );
    send_to_char( buf, ch );
    send_to_char(
      "Clean up extra files in area directory as required.\n\r", ch );

    return;
  }

  if ( !str_cmp( arg1, "expand" ) )
  {
    int vnum, vnum_base;

    if ( get_trust( ch ) < MAX_LEVEL )
    {
      send_to_char(
        "You must be level MAX_LEVEL to use this option.\n\r", ch );
      return;
    }

    vnum = atoi( arg2 );

    if ( vnum < 0 || vnum > MAX_VNUM )
    {
      send_to_char(
        "Vnum out of range.  Must be between 0 and MAX_VNUM.\n\r", ch );
      return;
    }

    if ( vnum <= pArea->vnum_final )
    {
      send_to_char(
  "Expand argument must be a vnum greater than the current max vnum.\n\r", ch );
      return;
    }

    if ( ( vnum + 1 ) % 100 )
    {
      send_to_char( "High vnum must end in 99.\n\r", ch );
      return;
    }

    vnum_base = pArea->vnum_start;

    for ( pArea = area_first; pArea != NULL; pArea = pArea->next )
    {
      if ( vnum > pArea->vnum_start && pArea->vnum_start > vnum_base )
      {
        send_to_char( "New vnum may not overlap existing area.\n\r", ch );
	return;
      }
    }

    ch->in_room->area->vnum_final = vnum;

    send_to_char( "vnum range expanded.\n\r", ch );
    return;
  }

  if ( !str_cmp( arg1, "reset" ) )
  {
    i = atoi( arg2 );

    pArea->reset_length = i;

    sprintf( buf, "%s reset timer set to %d ticks.\n\r",
      pArea->name, pArea->reset_length );
    send_to_char( buf, ch );

    return;
  }

  if ( !str_cmp( arg1, "bits" ) )
  {
    i = atoi( arg2 );

    pArea->area_bits = i;

    sprintf( buf, "%s bits set to %d.\n\r",
      pArea->name, pArea->reset_length );
    send_to_char( buf, ch );

    return;
  }

  do_aset( ch, "" );

  return;
}


/*
 * Snarf a help section.
 */
void load_helps( FILE *fp )
{
    HELP_DATA *pHelp;

    for ( ; ; )
    {
	pHelp		= alloc_perm( sizeof(*pHelp) );
	pHelp->level	= fread_number( fp );
	pHelp->keyword	= fread_string( fp );
	if ( pHelp->keyword[0] == '$' )
	    break;
	pHelp->text	= fread_string( fp );

	if ( !str_cmp( pHelp->keyword, "greeting" ) )
	    help_greeting = pHelp->text;

        if ( !str_cmp( pHelp->keyword, "greeting2" ) )
            help_greeting2 = pHelp->text;
 
        if ( !str_cmp( pHelp->keyword, "greeting3" ) )
            help_greeting3 = pHelp->text;


	if ( help_first == NULL )
	    help_first = pHelp;
	if ( help_last  != NULL )
	    help_last->next = pHelp;

	help_last	= pHelp;
	pHelp->next	= NULL;
	top_help++;
    }

    return;
}



/*
 * Snarf a mob section.
 */
void load_mobiles( FILE *fp )
{
    int ii;
    MOB_INDEX_DATA *pMobIndex;
    EQUIP_DATA *pEquip;

    if ( area_last == NULL )
    {
	bug( "Load_resets: no #AREA seen yet.", 0 );
	exit( 1 );
    }

    for ( ; ; )
    {
	sh_int vnum;
	char letter;
	int iHash;

	letter				= fread_letter( fp );
	if ( letter != '#' )
	{
	    bug( "Load_mobiles: # not found.", 0 );
	    exit( 1 );
	}

	vnum				= fread_number( fp );
	if ( vnum == 0 )
	    break;

	fBootDb = FALSE;
	if ( get_mob_index( vnum ) != NULL )
	{
	    bug( "Load_mobiles: vnum %d duplicated.", vnum );
	    exit( 1 );
	}
	fBootDb = TRUE;

	pMobIndex			= alloc_perm( sizeof(*pMobIndex) );
	pMobIndex->vnum			= vnum;
	pMobIndex->area			= area_last;
	pMobIndex->name			= fread_string( fp );
	pMobIndex->namelist		= fread_string( fp );
	pMobIndex->short_descr		= fread_string( fp );
	pMobIndex->long_descr		= fread_string( fp );
	pMobIndex->description		= fread_description( fp );

	pMobIndex->long_descr[0]	= UPPER(pMobIndex->long_descr[0]);
	pMobIndex->description[0]	= UPPER(pMobIndex->description[0]);

	pMobIndex->spec_fun_name	= fread_string( fp );
	pMobIndex->spec_fun	= spec_lookup	( pMobIndex->spec_fun_name );

	pMobIndex->act			= fread_number( fp ) | ACT_IS_NPC;
	pMobIndex->affected_by		= fread_number( fp );
	pMobIndex->pShop		= NULL;
	pMobIndex->alignment		= fread_number( fp );
	pMobIndex->aggression		= fread_number( fp );
	pMobIndex->speed		= fread_number( fp );
	pMobIndex->clan			= fread_number( fp );
	letter				= fread_letter( fp );
	pMobIndex->level		= fread_number( fp );

	pMobIndex->hitnodice		= fread_number( fp );
	/* 'd'		*/		  fread_letter( fp );
	pMobIndex->hitsizedice		= fread_number( fp );
	/* '+'		*/		  fread_letter( fp );
	pMobIndex->hitplus		= fread_number( fp );
	pMobIndex->damnodice		= fread_number( fp );	
	/* 'd'		*/		  fread_letter( fp );
	pMobIndex->damsizedice		= fread_number( fp );
	/* '+'		*/		  fread_letter( fp );
	pMobIndex->damplus		= fread_number( fp );
	pMobIndex->mob_stat_phys_str	= fread_number( fp );
	pMobIndex->mob_stat_phys_dex	= fread_number( fp );
	pMobIndex->mob_stat_phys_tou	= fread_number( fp );
	pMobIndex->mob_stat_phys_qui	= fread_number( fp );
	pMobIndex->mob_stat_ment_int	= fread_number( fp );
	pMobIndex->mob_offense		= fread_number( fp );
	pMobIndex->mob_defense		= fread_number( fp );
	pMobIndex->absorb[0]		= fread_number( fp );
	pMobIndex->absorb[1]		= fread_number( fp );
	pMobIndex->absorb[2]		= fread_number( fp );
	pMobIndex->absorb[3]		= fread_number( fp );
	pMobIndex->start_pos		= fread_number( fp );

	/*
	 * Back to meaningful values.
	 */
	pMobIndex->sex			= fread_number( fp );

	pMobIndex->race			= race_lookup( fread_word( fp ) );
	pMobIndex->class		= fread_number( fp );
	pMobIndex->equipped		= NULL;

	for ( ii = 0; ii < MAX_COIN; ii++ )
	    pMobIndex->coins[ii] = 0;

	if ( IS_SET( pMobIndex->act, ACT_SHOPKEEPER ) )
	{
	    SHOP_DATA *pshop;

	    pshop = new_shop( );

	    for ( ii = 0; ii < MAX_TRADE; ii++ )
		pshop->buy_type[ii] = fread_number( fp );
	    pshop->profit_buy = fread_number( fp );
	    pshop->profit_sell = fread_number( fp );
	    pshop->open_hour = fread_number( fp );
	    pshop->close_hour = fread_number( fp );
	    pshop->gain = fread_number( fp );

	    for ( ii = 0; ii < SHOP_MAX_MESSAGE; ii++ )
		pshop->shop_messages[ii] = fread_string( fp );

	    pMobIndex->pShop = pshop;
	}

	if ( letter != 'S' )
	{
	    bug( "Load_mobiles: vnum %d non-S.", vnum );
	    exit( 1 );
	}

	for ( ; ; )
	{
	    letter = fread_letter( fp );

	    if ( letter == 'C' )
	    {
		/* C is used to assign currency exchange rates to shopkeeps */
		int coin;
		coin = get_coin_index( fread_word( fp ) );
		if ( coin == -1 || coin >= MAX_COIN
		|| pMobIndex->pShop == NULL )
		{
		    fread_number( fp );
		    fread_number( fp );
		    continue;
		}
		pMobIndex->pShop->coinage_buy[coin] = fread_number( fp );
		pMobIndex->pShop->coinage_sell[coin] = fread_number( fp );
	    }
	    else if ( letter == 'E' )
	    {
		pEquip = alloc_perm( sizeof( *pEquip ) );
		pEquip->item = fread_number( fp );
		pEquip->location = wear_loc_lookup( fread_word( fp ) );
		pEquip->next = pMobIndex->equipped;
		pMobIndex->equipped = pEquip;
	    }
	    else if ( letter == 'G' )
	    {
		int coin;
		coin = get_coin_index( fread_word( fp ) );
		if ( coin == -1 )
		    fread_number( fp );
		else
		    pMobIndex->coins[coin] = fread_number( fp );
	    }
	    else if ( letter == 'K' )
	    {
		if ( pMobIndex->skin_objects[0] == 0 )
		    pMobIndex->skin_objects[0] = fread_number( fp );
		else if ( pMobIndex->skin_objects[1] == 0 )
		    pMobIndex->skin_objects[1] = fread_number( fp );
		else if ( pMobIndex->skin_objects[2] == 0 )
		    pMobIndex->skin_objects[2] = fread_number( fp );
		else
		    pMobIndex->skin_objects[3] = fread_number( fp );
	    }
	    else if ( letter == 'S' )
	    {
		int sn;

		sn = skill_lookup( fread_word( fp ) );

		if ( sn == -1 )
		{
		    fread_number( fp );
		    fread_number( fp );
		}
		else
		{
		    pMobIndex->learned[sn] = fread_number( fp );
		    pMobIndex->power_level[sn] = fread_number( fp );
		}
	    }
	    else
	    {
		ungetc( letter, fp );
		break;
	    }
	}

	iHash			= vnum % MAX_KEY_HASH;
	pMobIndex->next		= mob_index_hash[iHash];
	mob_index_hash[iHash]	= pMobIndex;
	top_mob_index++;
	pMobIndex->zone_next = NULL;
	if (area_last->mob_first == NULL)
	{
	    area_last->mob_first = pMobIndex;
	    area_last->mob_last = pMobIndex;
	}
	else
	{
	    area_last->mob_last->zone_next = pMobIndex;
	    area_last->mob_last = pMobIndex;
	}
    }

    return;
}



/*
 * Snarf an obj section.
 */
void load_objects( FILE *fp )
{
    OBJ_INDEX_DATA *pObjIndex;

    if ( area_last == NULL )
    {
	bug( "Load_resets: no #AREA seen yet.", 0 );
	exit( 1 );
    }


    for ( ; ; )
    {
	int vnum;
	char letter;
	int iHash;

	letter				= fread_letter( fp );
	if ( letter != '#' )
	{
	    bug( "Load_objects: # not found.", 0 );
	    exit( 1 );
	}

	vnum				= fread_number( fp );
	if ( vnum == 0 )
	    break;

	fBootDb = FALSE;
	if ( get_obj_index( vnum ) != NULL )
	{
	    bug( "Load_objects: vnum %d duplicated.", vnum );
	    exit( 1 );
	}
	fBootDb = TRUE;

	pObjIndex			= alloc_perm( sizeof(*pObjIndex) );
	pObjIndex->vnum			= vnum;
	pObjIndex->area			= area_last;
	pObjIndex->name			= fread_string( fp );
	pObjIndex->short_descr		= fread_string( fp );
	pObjIndex->look_descr		= fread_string( fp );
	pObjIndex->description		= fread_description( fp );
	pObjIndex->spec_fun_name	= fread_string( fp );
	pObjIndex->spec_fun	= obj_fun_lookup ( pObjIndex->spec_fun_name );

	pObjIndex->material		= material_lookup( fread_word( fp ) );

	pObjIndex->short_descr[0]	= LOWER(pObjIndex->short_descr[0]);
	pObjIndex->look_descr[0]	= UPPER(pObjIndex->look_descr[0]);

	pObjIndex->item_type		= fread_number( fp );
	pObjIndex->extra_flags		= fread_number( fp );
	pObjIndex->wear_flags		= fread_number( fp );
	pObjIndex->color		= fread_number( fp );
	pObjIndex->value[0]		= fread_number( fp );
	pObjIndex->value[1]		= fread_number( fp );
	pObjIndex->value[2]		= fread_number( fp );
	pObjIndex->value[3]		= fread_number( fp );
	pObjIndex->weight		= fread_number( fp );
	pObjIndex->capacity		= fread_number( fp );
	pObjIndex->size			= fread_number( fp );
	pObjIndex->cost			= fread_number( fp );
	pObjIndex->load_chance		= fread_number( fp );
	pObjIndex->dam[0]		= fread_number( fp );
	pObjIndex->dam[1]		= fread_number( fp );
	pObjIndex->toughness		= fread_number( fp );
	pObjIndex->attack_type		= fread_number( fp );

	pObjIndex->limit	= -1;
	if ( pObjIndex->item_type == ITEM_KEY )
	    pObjIndex->limit = 1;

	for ( ; ; )
	{
	    char letter;

	    letter = fread_letter( fp );

	    if ( letter == 'A' )
	    {
		AFFECT_DATA *paf;

		paf			= alloc_perm( sizeof(*paf) );
		paf->type		= -1;
		paf->duration		= -1;
		paf->location		= fread_number( fp );
		paf->modifier		= fread_number( fp );
		paf->bitvector		= 0;
		paf->next		= pObjIndex->affected;
		pObjIndex->affected	= paf;
		top_affect++;
	    }

	    else if ( letter == 'E' )
	    {
		EXTRA_DESCR_DATA *ed;

		ed			= alloc_perm( sizeof(*ed) );
		ed->keyword		= fread_string( fp );
		ed->description		= fread_description( fp );
		ed->next		= pObjIndex->extra_descr;
		pObjIndex->extra_descr	= ed;
		top_ed++;
	    }

	    else if ( letter == 'L' )
	    {
		pObjIndex->limit = fread_number( fp );
	    }

	    else
	    {
		ungetc( letter, fp );
		break;
	    }
	}

	/*
	 * Translate spell "slot numbers" to internal "skill numbers."
	 */
	switch ( pObjIndex->item_type )
	{
	case ITEM_PILL:
	case ITEM_POTION:
	case ITEM_SCROLL:
	    pObjIndex->value[1] = slot_lookup( pObjIndex->value[1] );
	    pObjIndex->value[2] = slot_lookup( pObjIndex->value[2] );
	    pObjIndex->value[3] = slot_lookup( pObjIndex->value[3] );
	    break;

	case ITEM_STAFF:
	case ITEM_WAND:
	    pObjIndex->value[3] = slot_lookup( pObjIndex->value[3] );
	    break;
	}

	iHash			= vnum % MAX_KEY_HASH;
	pObjIndex->next		= obj_index_hash[iHash];
	obj_index_hash[iHash]	= pObjIndex;
	top_obj_index++;
	pObjIndex->zone_next = NULL;
	if (area_last->obj_first == NULL)
	{
	    area_last->obj_first = pObjIndex;
	    area_last->obj_last = pObjIndex;
	}
	else
	{
	    area_last->obj_last->zone_next = pObjIndex;
	    area_last->obj_last = pObjIndex;
	}
    }

    return;
}



/*
 * Snarf a reset section.
 */
void load_resets( FILE *fp )
{
    bool fValid;
    RESET_DATA *pReset;
#if 0
    char buf[MAX_STRING_LENGTH];
#endif

    if ( area_last == NULL )
    {
	bug( "Load_resets: no #AREA seen yet.", 0 );
	exit( 1 );
    }

    for ( ; ; )
    {
#if 0
	ROOM_INDEX_DATA *pRoomIndex;
	EXIT_DATA *pexit;
#endif
	char letter;

	fValid = TRUE;

	if ( ( letter = fread_letter( fp ) ) == 'S' )
	    break;

	if ( letter == '*' )
	{
	    fread_to_eol( fp );
	    continue;
	}

	pReset		= alloc_perm( sizeof(*pReset) );
	pReset->command	= letter;
	/* if_flag */	  fread_number( fp );
	pReset->arg1	= fread_number( fp );
	pReset->arg2	= fread_number( fp );
	if ( letter == 'G'
	|| letter == 'I'
	|| letter == 'R' )
	{
	}
	else if ( letter == 'E' )
	{
	    pReset->arg3	= wear_loc_lookup( fread_word( fp ) );
	}
	else
	    pReset->arg3	= fread_number( fp );

	fread_to_eol( fp );

	/*
	 * Validate parameters.
	 * We're calling the index functions for the side effect.
	 */
/* this is retarded.  it should check once, after the entire database
   has been read. */
#if 0
	switch ( letter )
	{
	default:
	    sprintf( buf, "Load_resets: bad command '%c'.", letter );
	    log_string( buf );
/*
	    exit( 1 );
*/
	    fValid = FALSE;
	    break;

	case 'M':
	    if ( get_mob_index  ( pReset->arg1 ) == NULL )
		fValid = FALSE;
	    if ( get_room_index ( pReset->arg3 ) == NULL )
		fValid = FALSE;
	    break;

	case 'O':
	    if ( get_obj_index  ( pReset->arg1 ) == NULL )
		fValid = FALSE;
	    if ( get_room_index ( pReset->arg3 ) == NULL )
		fValid = FALSE;
	    break;

	case 'P':
	    if ( get_obj_index  ( pReset->arg1 ) == NULL )
		fValid = FALSE;
	    if ( get_obj_index  ( pReset->arg3 ) == NULL )
		fValid = FALSE;
	    break;

	case 'G':
	case 'E':
	case 'I':
	    if ( get_obj_index  ( pReset->arg1 ) == NULL )
		fValid = FALSE;
	    break;

	case 'D':
	    if ( (pRoomIndex = get_room_index( pReset->arg1 )) == NULL )
	    {
		fValid = FALSE;
		break;
	    }

	    if ( pReset->arg2 < 0
	    ||   pReset->arg2 > 5
	    || ( pexit = pRoomIndex->exit[pReset->arg2] ) == NULL
	    || !IS_SET( pexit->exit_flags, EX_ISDOOR ) )
	    {
		sprintf( buf, "Load_resets: 'D': exit %d not door.", pReset->arg2 );
		log_string( buf );
		fValid = FALSE;
/*
		exit( 1 );
*/
	    }

	    if ( pReset->arg3 < 0 || pReset->arg3 > 2 )
	    {
		sprintf( buf, "Load_resets: 'D': bad 'locks': %d.", pReset->arg3 );
		log_string( buf );
		fValid = FALSE;
/*
		exit( 1 );
*/
	    }

	    break;

	case 'R':
	    if ( (pRoomIndex = get_room_index( pReset->arg1 )) == NULL)
	    {
		fValid = FALSE;
		break;
	    }

	    if ( pReset->arg2 < 0 || pReset->arg2 > 6 )
	    {
		sprintf( buf, "Load_resets: 'R': bad exit %d.", pReset->arg2 );
		log_string( buf );
		fValid = FALSE;
/*
		exit( 1 );
*/
	    }

	    break;
	}
#endif
	if ( fValid == FALSE )
	    continue;

	if ( area_last->reset_first == NULL )
	    area_last->reset_first	= pReset;
	if ( area_last->reset_last  != NULL )
	    area_last->reset_last->next	= pReset;
	    
	area_last->reset_last	= pReset;
	pReset->next		= NULL;
	top_reset++;
    }

    return;
}



/*
 * Snarf a room section.
 */
void load_rooms( FILE *fp )
{
    ROOM_INDEX_DATA *pRoomIndex;

    if ( area_last == NULL )
    {
	bug( "Load_resets: no #AREA seen yet.", 0 );
	exit( 1 );
    }

    for ( ; ; )
    {
	int vnum;
	char letter;
	int door;
	int iHash;

	letter				= fread_letter( fp );
	if ( letter != '#' )
	{
	    bug( "Load_rooms: # not found.", 0 );
	    exit( 1 );
	}

	vnum				= fread_number( fp );
	if ( vnum == 0 )
	    break;

	fBootDb = FALSE;
	if ( get_room_index( vnum ) != NULL )
	{
	    bug( "Load_rooms: vnum %d duplicated.", vnum );
	    exit( 1 );
	}
	fBootDb = TRUE;

	pRoomIndex			= alloc_perm( sizeof(*pRoomIndex) );
	pRoomIndex->people		= NULL;
	pRoomIndex->contents		= NULL;
	pRoomIndex->extra_descr		= NULL;
	pRoomIndex->flora		= NULL;
	pRoomIndex->area		= area_last;
	pRoomIndex->vnum		= vnum;
	pRoomIndex->name		= fread_string( fp );
	pRoomIndex->description		= fread_description( fp );
	pRoomIndex->spec_fun_name	= fread_string( fp );
	pRoomIndex->spec_fun = room_fun_lookup( pRoomIndex->spec_fun_name );
/*
	    if ( pRoomIndex->spec_fun == 0 )
	    {
		bug( "Load_specials: 'R': vnum %d.", pRoomIndex->vnum );
		exit( 1 );
	    }
*/
	pRoomIndex->sector	= sector_lookup( fread_word( fp ), NULL );

	pRoomIndex->room_flags		= fread_number( fp );
	pRoomIndex->move_cost		= fread_number( fp );
	pRoomIndex->move_delay		= fread_number( fp );
	pRoomIndex->light		= fread_number( fp );
	pRoomIndex->size		= fread_number( fp );
	pRoomIndex->light_filter	= fread_number( fp );

	pRoomIndex->water_min		= pRoomIndex->sector->water_min;
	pRoomIndex->water		= pRoomIndex->water_min;
	pRoomIndex->water_degrade	= pRoomIndex->sector->water_degrade;
/*
 * Setting water update to 0 resets the room immediately on reboot.
 * We probably don't want this, so set it to 1.
 */
	pRoomIndex->water_update	= 1;

	for ( door = 0; door <= 5; door++ )
	    pRoomIndex->exit[door] = NULL;

	for ( ; ; )
	{
	    letter = fread_letter( fp );

	    if ( letter == 'S' )
		break;

	    if ( letter == 'D' )
	    {
		EXIT_DATA *pexit;

		door = fread_number( fp );
		if ( door < 0 || door > 5 )
		{
		    bug( "Fread_rooms: vnum %d has bad door number.", vnum );
		    exit( 1 );
		}

		pexit			= alloc_perm( sizeof(*pexit) );
		pexit->description	= fread_string( fp );
//                pexit->cdescription     = fread_string( fp );
		pexit->keyword		= fread_string( fp );
		pexit->exit_flags	= fread_number( fp );
		pexit->key		= fread_number( fp );
		pexit->vnum		= fread_number( fp );
		pexit->difficulty	= fread_number( fp );

		pRoomIndex->exit[door]	= pexit;
		top_exit++;
	    }
	    else if ( letter == 'E' )
	    {
		EXTRA_DESCR_DATA *ed;

		ed			= alloc_perm( sizeof(*ed) );
		ed->keyword		= fread_string( fp );
		ed->description		= fread_string( fp );
		ed->next		= pRoomIndex->extra_descr;
		pRoomIndex->extra_descr	= ed;
		top_ed++;
	    }
	    else
	    {
		bug( "Load_rooms: vnum %d has flag not 'DES'.", vnum );
		exit( 1 );
	    }
	}

	iHash			= vnum % MAX_KEY_HASH;
	pRoomIndex->next	= room_index_hash[iHash];
	room_index_hash[iHash]	= pRoomIndex;
	top_room++;
	pRoomIndex->zone_next = NULL;
	if (area_last->room_first == NULL)
	{
	    area_last->room_first = pRoomIndex;
	    area_last->room_last = pRoomIndex;
	}
	else
	{
	    area_last->room_last->zone_next = pRoomIndex;
	    area_last->room_last = pRoomIndex;
	}
    }

    return;
}




/*
 * Snarf notes file.
 */
void load_notes( void )
{
    FILE *fp;
    NOTE_DATA *pnotelast;

    if ( ( fp = fopen( NOTE_FILE, "r" ) ) == NULL )
	return;

    pnotelast = NULL;
    for ( ; ; )
    {
	NOTE_DATA *pnote;
	char letter;

	do
	{
	    letter = getc( fp );
	    if ( feof(fp) )
	    {
		fclose( fp );
		return;
	    }
	}
	while ( isspace(letter) );
	ungetc( letter, fp );

	pnote		= alloc_perm( sizeof(*pnote) );

	if ( str_cmp( fread_word( fp ), "sender" ) )
	    break;
	pnote->sender	= fread_string( fp );

	if ( str_cmp( fread_word( fp ), "date" ) )
	    break;
	pnote->date	= fread_string( fp );

	if ( str_cmp( fread_word( fp ), "to" ) )
	    break;
	pnote->to_list	= fread_string( fp );

	if ( str_cmp( fread_word( fp ), "subject" ) )
	    break;
	pnote->subject	= fread_string( fp );

	if ( str_cmp( fread_word( fp ), "text" ) )
	    break;
	pnote->text	= fread_string( fp );

	if ( note_list == NULL )
	    note_list		= pnote;
	else
	    pnotelast->next	= pnote;

	pnotelast	= pnote;
    }

    strcpy( strArea, NOTE_FILE );
    fpArea = fp;
    bug( "Load_notes: bad key word.", 0 );
    exit( 1 );
    return;
}


/*
 * Read all damage messages from file into array.
 * Allows different numbers of messages for each attack type
 */
void load_dam_msg( void )
{
    FILE *fp;
    char c = ' ', clast = 'a', letter;
    char buf[MAX_STRING_LENGTH];
    struct damage_mess *pMess;
    int value, valuelast = 0;

    /*
     * Init all pointers.
     */
    for ( value = 0; value < ATTACK_MAX; value++ )
    {
	death_table[value] = NULL;
	damage_table[value] = NULL;
    }

    value = -1;

    fclose( fpReserve );
    if ( ( fp = fopen( "damage.msg", "r" ) ) == NULL )
    {
	log_string( "**  Damage message file missing!  **" );
	exit( 1 );
    }

    letter = '\0';

    for ( ; ; )
    {
	clast = c;
	c = fread_letter( fp );

	if ( c == '*' ) 
	{
	    fread_to_eol( fp );
	    continue;
	}

	if ( c == '#' )
	{
	    valuelast = value;
	    value = fread_number( fp );

	    if ( value < 0 )
	    {
		fclose ( fp );
		fpReserve = fopen( NULL_FILE, "r" );
		break;
	    }

	    if ( value > ATTACK_MAX )
	    {
		sprintf( buf, "load_dam_msg: Bad damage type: %d", value );
		log_string( buf );
		exit( 1 );
	    }

	    fread_to_eol( fp );
	    continue;
	}

	if ( c == 'D' || c == 'N' )
	{
	    if ( value < 0 || value > ATTACK_MAX )
	    {
		log_string( "load_dam_msg: No attack type before messages" );
		exit( 1 );
	    }

	    pMess = alloc_perm( sizeof( *pMess ) );

	    pMess->min_damage = fread_number( fp );
	    pMess->message_char = fread_string( fp );
	    pMess->message_room = fread_string( fp );
	    pMess->message_victim = fread_string( fp );

	    if ( c == 'D' )
	    {
		pMess->next = death_table[value];
		death_table[value] = pMess;
	    }
	    else if ( c == 'N' )
	    {
		pMess->next = damage_table[value];
		damage_table[value] = pMess;
	    }

	    continue;
	}

	sprintf( buf, "load_dam_msg: c: %c, clast: %c, value: %d, valuelast: %d",
	    c, clast, value, valuelast );
	log_string( buf );

	log_string( "load_dam_msg: error in data file" );
	exit( 1 );
    }

    return;
}

/*
 * Go through all existing rooms, detecting rooms which potentially
 * could have a save file.  Load objects and mobiles for that room.
 */
void load_saved_rooms( )
{
    ROOM_INDEX_DATA *room;
    int i;
    extern void read_room( ROOM_INDEX_DATA *room );

    for ( i = 0; i < MAX_KEY_HASH; i++ )
    {
	for ( room = room_index_hash[i]; room != NULL; room = room->next )
	{
	    if ( IS_SET( room->room_flags, ROOM_SAVES_CONTENTS ) )
		read_room( room );
	}
    }

    return;
}



/*
 * Translate all room exits from virtual to real.
 * Has to be done after all rooms are read in.
 * Check for bad reverse exits.
 */
void fix_exits( void )
{
    ROOM_INDEX_DATA *pRoomIndex;
    EXIT_DATA *pexit;
    int iHash;
    int door;

    for ( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
    {
	for ( pRoomIndex  = room_index_hash[iHash];
	      pRoomIndex != NULL;
	      pRoomIndex  = pRoomIndex->next )
	{
	    bool fexit;

	    fexit = FALSE;
	    for ( door = 0; door <= 5; door++ )
	    {
		if ( ( pexit = pRoomIndex->exit[door] ) != NULL )
		{
		    fexit = TRUE;
		    if ( pexit->vnum <= 0 )
			pexit->to_room = NULL;
		    else
		    {
			pexit->to_room = get_room_index( pexit->vnum );
			if ( pexit == NULL )
			{
			    pexit->vnum = -1;
			    if ( pexit->description[0] == '\0'
			    && pexit->keyword[0] == '\0' )
//                            && pexit->cdescription[0] == '\0' )
			    {
				pRoomIndex->exit[door] = NULL;
				free( pexit );
			    }
			}
		    }
		}
	    }

	    if ( !fexit )
		SET_BIT( pRoomIndex->room_flags, ROOM_NO_MOB );
	}
    }

    return;
}



/*
 * Repopulate areas periodically.
 */
void area_update( void )
{
    AREA_DATA *pArea;

    for ( pArea = area_first; pArea != NULL; pArea = pArea->next )
    {
	pArea->age++;

	/*
	 * Check age and reset.
	 */
	if ( pArea->age >= pArea->reset_length && pArea->reset_length != 0 )
	{
	    reset_area( pArea );
	    pArea->age = number_range( 0, 3 );
	}
    }

    return;
}

void start_update(void)
{
    AREA_DATA *pArea;

    for ( pArea = area_first; pArea != NULL; pArea = pArea->next )
    {
        pArea->age++;

            reset_area( pArea );
            pArea->age = number_range( 0, 3 );
        
    }

    return;
}


/*
 * Reset one area.
 */
void reset_area( AREA_DATA *pArea )
{
    RESET_DATA *pReset;
    CHAR_DATA *mob;
    OBJ_DATA *obj_to;
    bool last;

    mob 	= NULL;
    obj_to	= NULL;
    last	= TRUE;
    for ( pReset = pArea->reset_first; pReset != NULL; pReset = pReset->next )
    {
	ROOM_INDEX_DATA *pRoomIndex;
	MOB_INDEX_DATA *pMobIndex;
	OBJ_INDEX_DATA *pObjIndex;
#if 0
	OBJ_INDEX_DATA *pObjToIndex;
#endif
	EXIT_DATA *pexit;
	OBJ_DATA *obj;


	switch ( pReset->command )
	{
	default:
	    bug( "Reset_area: bad command %c.", pReset->command );
	    break;

	case 'M':
	    if ( pReset->target != NULL )
	    {
		last = FALSE;
		continue;
	    }

	    if ( ( pMobIndex = get_mob_index( pReset->arg1 ) ) == NULL )
	    {
		bug( "Reset_area: 'M': bad vnum %d.", pReset->arg1 );
		continue;
	    }

	    if ( ( pRoomIndex = get_room_index( pReset->arg3 ) ) == NULL )
	    {
		bug( "Reset_area: 'R': bad vnum %d.", pReset->arg3 );
		continue;
	    }

	    for ( mob = pRoomIndex->people; mob != NULL; mob = mob->next_in_room )
	    {
		if ( mob->pIndexData == NULL || mob->pIndexData != pMobIndex )
		    continue;

		if ( mob->pReset != NULL )
		    continue;

		mob->pReset = pReset;
		pReset->target = (void * ) mob;
		last = FALSE;
		break;
	    }

	    if ( mob != NULL )
		break;

	    mob = create_mobile( pMobIndex );

	    /*
	    check_random_equip( mob );
	    */

	    char_to_room( mob, pRoomIndex, FALSE );

	    pReset->target = (void *) mob;
	    mob->pReset = pReset;
	    last  = TRUE;
	    break;

	case 'O':
	    if ( pReset->target != NULL )
	    {
		last = FALSE;
		obj_to = (OBJ_DATA *) pReset->target;
		continue;
	    }

	    if ( ( pObjIndex = get_obj_index( pReset->arg1 ) ) == NULL )
	    {
		bug( "Reset_area: 'O': bad vnum %d.", pReset->arg1 );
		continue;
	    }

	    if ( ( pRoomIndex = get_room_index( pReset->arg3 ) ) == NULL )
	    {
		bug( "Reset_area: 'O': bad room %d.", pReset->arg3 );
		continue;
	    }

	    for ( obj = pRoomIndex->contents; obj != NULL; obj = obj->next_content )
	    {
		if ( obj->pIndexData == NULL || obj->pIndexData != pObjIndex )
		    continue;

		if ( obj->pReset != NULL )
		    continue;

		obj->pReset = pReset;
		pReset->target = (void * ) obj;
		last = FALSE;
		obj_to = obj;
		break;
	    }

	    if ( obj != NULL )
		break;

	    if ( pObjIndex->number >= 0
	    && pObjIndex->number >= pObjIndex->limit
	    && pObjIndex->limit >= 0 )
	    {
		last = TRUE;
		if ( pObjIndex->capacity <= 0 )
		    last = FALSE;
		break;
	    }

	    if ( number_percent() > pObjIndex->load_chance )
	    {
		last = TRUE;
		if ( pObjIndex->capacity <= 0 )
		    last = FALSE;
		break;
	    }

	    obj       = create_object( pObjIndex );
	    obj_to_room( obj, pRoomIndex, FALSE );
	    pReset->target = (void *) obj;
	    obj->pReset = pReset;
	    last = TRUE;
	    obj_to = obj;
	    break;

	case 'P':
	    if ( pReset->target != NULL )
	    {
		last = FALSE;
		continue;
	    }

	    if ( ( pObjIndex = get_obj_index( pReset->arg1 ) ) == NULL )
	    {
		bug( "Reset_area: 'P': bad vnum %d.", pReset->arg1 );
		continue;
	    }

/*
	    if ( ( pObjToIndex = get_obj_index( pReset->arg3 ) ) == NULL )
	    {
		bug( "Reset_area: 'P': bad vnum %d.", pReset->arg3 );
		continue;
	    }

	    if ( ( obj_to = get_obj_type( pObjToIndex ) ) == NULL
	    ||   (obj_to->in_room == NULL && obj_to->carried_by == NULL )
	    ||   count_obj_list( pObjIndex, obj_to->contains ) > 0 )
	    {
		last = FALSE;
		break;
	    }
*/
	    if ( obj_to == NULL )
	    {
		last = FALSE;
		break;
	    }

	    for ( obj = obj_to->contains; obj != NULL; obj = obj->next_content )
	    {
		if ( obj->pIndexData == NULL || obj->pIndexData != pObjIndex )
		    continue;

		if ( obj->pReset != NULL )
		    continue;

		obj->pReset = pReset;
		pReset->target = (void * ) obj;
		last = FALSE;
		break;
	    }

	    if ( obj != NULL )
		break;

	    if ( pObjIndex->number >= 0
	    && pObjIndex->number >= pObjIndex->limit
	    && pObjIndex->limit >= 0 )
	    {
		last = TRUE;
		if ( pObjIndex->capacity <= 0 )
		    last = FALSE;
		break;
	    }

	    if ( number_percent() > pObjIndex->load_chance )
	    {
		last = TRUE;
		if ( pObjIndex->capacity <= 0 )
		    last = FALSE;
		break;
	    }
	    
	    obj = create_object( pObjIndex );
	    obj_to_obj( obj, obj_to );
	    pReset->target = (void *) obj;
	    obj->pReset = pReset;
	    last = TRUE;
	    break;

	case 'G':
	case 'E':
	case 'I':
	    if ( ( pObjIndex = get_obj_index( pReset->arg1 ) ) == NULL )
	    {
		bug( "Reset_area: 'E', 'G', or 'I': bad vnum %d.", pReset->arg1 );
		continue;
	    }

	    if ( !last )
		break;

	    if ( mob == NULL )
	    {
		bug( "Reset_area: 'E', 'G', or 'I': null mob for vnum %d.",
		    pReset->arg1 );
		last = FALSE;
		break;
	    }

	    if ( pObjIndex->number >= 0
	    && pObjIndex->number >= pObjIndex->limit
	    && pObjIndex->limit >= 0 )
	    {
		last = TRUE;
		if ( pObjIndex->capacity == 0 )
		    last = FALSE;
		break;
	    }

	    if ( number_percent() >  pObjIndex->load_chance )
	    {
		last = TRUE;
		if ( pObjIndex->capacity == 0 )
		    last = FALSE;
		break;
	    }

	    obj = create_object( pObjIndex );
	    obj_to_char( obj, mob );
	    if ( pReset->command == 'E' )
		equip_char( mob, obj, pReset->arg3 );
	    if ( pReset->command == 'I' )
		SET_BIT( obj->extra_flags, ITEM_INVENTORY );
	    last = TRUE;
	    break;

	case 'D':
	    if ( ( pRoomIndex = get_room_index( pReset->arg1 ) ) == NULL )
	    {
		bug( "Reset_area: 'D': bad vnum %d.", pReset->arg1 );
		continue;
	    }

	    if ( ( pexit = pRoomIndex->exit[pReset->arg2] ) == NULL )
		break;

	    switch ( pReset->arg3 )
	    {
	    case 0:
		REMOVE_BIT( pexit->exit_flags, EX_CLOSED );
		REMOVE_BIT( pexit->exit_flags, EX_LOCKED );
		break;

	    case 1:
		SET_BIT(    pexit->exit_flags, EX_CLOSED );
		REMOVE_BIT( pexit->exit_flags, EX_LOCKED );
		break;

	    case 2:
		SET_BIT(    pexit->exit_flags, EX_CLOSED );
		SET_BIT(    pexit->exit_flags, EX_LOCKED );
		break;
	    }

	    last = TRUE;
	    break;

	case 'R':
	    if ( ( pRoomIndex = get_room_index( pReset->arg1 ) ) == NULL )
	    {
		bug( "Reset_area: 'R': bad vnum %d.", pReset->arg1 );
		continue;
	    }

	    {
		int d0;
		int d1;

		for ( d0 = 0; d0 < pReset->arg2 - 1; d0++ )
		{
		    d1                   = number_range( d0, pReset->arg2-1 );
		    pexit                = pRoomIndex->exit[d0];
		    pRoomIndex->exit[d0] = pRoomIndex->exit[d1];
		    pRoomIndex->exit[d1] = pexit;
		}
	    }
	    break;
	}
    }

    return;
}



/*
 * Create an instance of a mobile.
 */
CHAR_DATA *create_mobile( MOB_INDEX_DATA *pMobIndex )
{
    CHAR_DATA *mob;
    int i;

    if ( pMobIndex == NULL )
    {
	bug( "Create_mobile: NULL pMobIndex.", 0 );
	exit( 1 );
    }

    if ( char_free == NULL )
    {
	mob		= alloc_perm( sizeof(*mob) );
    }
    else
    {
	mob		= char_free;
	char_free	= char_free->next;
    }

    clear_char( mob );
    mob->pIndexData	= pMobIndex;

    mob->name		= pMobIndex->name;
    mob->namelist	= pMobIndex->namelist;
    mob->short_descr	= pMobIndex->short_descr;
    mob->long_descr	= pMobIndex->long_descr;
    mob->dynamic_descr	= NULL;
    mob->description	= pMobIndex->description;
    mob->spec_fun_name	= pMobIndex->spec_fun_name;

    mob->spec_fun	= pMobIndex->spec_fun;
    mob->prompt		= NULL;

    mob->level		= pMobIndex->level;
    mob->act		= pMobIndex->act;
    mob->affected_by	= pMobIndex->affected_by;
    mob->alignment	= pMobIndex->alignment;
    mob->aggression	= pMobIndex->aggression;
    mob->speed		= pMobIndex->speed;
    mob->clan		= pMobIndex->clan;
    mob->sex		= pMobIndex->sex;
    mob->race		= pMobIndex->race;

    mob->max_hit	= dice( pMobIndex->hitnodice, pMobIndex->hitsizedice )
				+ pMobIndex->hitplus;
    mob->hit[HIT_LOC_BODY]		= mob->max_hit;

    mob->damnodice	= pMobIndex->damnodice;
    mob->damsizedice	= pMobIndex->damsizedice;
    mob->damplus	= pMobIndex->damplus;

    for ( i = 0; i < MAX_COIN; i++ )
    {
      if ( pMobIndex->coins[i] != 0 )
	gain_coin( mob, pMobIndex->coins[i], i, FALSE, NULL );
    }

    mob->perm_stat_phys_str	= pMobIndex->mob_stat_phys_str;
    mob->perm_stat_phys_dex	= pMobIndex->mob_stat_phys_dex;
    mob->perm_stat_phys_tou	= pMobIndex->mob_stat_phys_tou;
    mob->perm_stat_phys_qui	= pMobIndex->mob_stat_phys_qui;
    mob->perm_stat_ment_int	= pMobIndex->mob_stat_ment_int;
    mob->perm_offense	= pMobIndex->mob_offense;
    mob->perm_defense	= pMobIndex->mob_defense;
    mob->absorb[0]	= pMobIndex->absorb[0];
    mob->absorb[1]	= pMobIndex->absorb[1];
    mob->absorb[2]	= pMobIndex->absorb[2];
    mob->absorb[3]	= pMobIndex->absorb[3];

    mob->height		= race_array[mob->race]->height_base
			+ dice( race_array[mob->race]->heightnodice,
				race_array[mob->race]->heightsizedice );

    mob->weight		= race_array[mob->race]->weight_base
			+ dice( race_array[mob->race]->weightnodice,
				race_array[mob->race]->weightsizedice );

    assign_name( mob );

    if ( pMobIndex->equipped != NULL )
    {
	OBJ_DATA *obj;
	EQUIP_DATA *pEquip;

	for ( pEquip = pMobIndex->equipped; pEquip != NULL; pEquip = pEquip->next )
	{
	    obj = create_object( get_obj_index( pEquip->item ) );

	    if ( obj == NULL )
		continue;

	    obj_to_char( obj, mob );

	    if ( pEquip->location != -1 )
	    {
		equip_char( mob, obj, pEquip->location );
	    }
	}
    }

    /*
     * Insert in list.
     */
    mob->next		= char_list;
    char_list		= mob;
    return mob;
}



/*
 * Create an instance of an object.
 */
OBJ_DATA *create_object( OBJ_INDEX_DATA *pObjIndex )
{
    static OBJ_DATA obj_zero;
    OBJ_DATA *obj;
    AFFECT_DATA *paf_to;
    AFFECT_DATA *paf_from;

    if ( pObjIndex == NULL )
    {
	bug( "Create_object: NULL pObjIndex.", 0 );
	exit( 1 );
    }

    if ( obj_free == NULL )
    {
	obj		= alloc_perm( sizeof(*obj) );
    }
    else
    {
	obj		= obj_free;
	obj_free	= obj_free->next;
    }

    *obj		= obj_zero;
    obj->pIndexData	= pObjIndex;
    obj->in_room	= NULL;
    obj->wear_loc	= gn_wear_none;

    obj->name		= pObjIndex->name;
    obj->short_descr	= pObjIndex->short_descr;
    obj->look_descr	= pObjIndex->look_descr;
    obj->description	= pObjIndex->description;
    obj->spec_fun_name	= pObjIndex->spec_fun_name;
    obj->spec_fun	= pObjIndex->spec_fun;
    obj->material	= pObjIndex->material;
    obj->item_type	= pObjIndex->item_type;
    obj->extra_flags	= pObjIndex->extra_flags;
    obj->wear_flags	= pObjIndex->wear_flags;
    obj->color		= pObjIndex->color;
    obj->value[0]	= pObjIndex->value[0];
    obj->value[1]	= pObjIndex->value[1];
    obj->value[2]	= pObjIndex->value[2];
    obj->value[3]	= pObjIndex->value[3];
    obj->weight		= pObjIndex->weight;
    obj->capacity	= pObjIndex->capacity;
    obj->size		= pObjIndex->size;
    obj->cost		= pObjIndex->cost;
    obj->dam[0]		= pObjIndex->dam[0];
    obj->dam[1]		= pObjIndex->dam[1];
    obj->toughness	= pObjIndex->toughness;
    obj->attack_type	= pObjIndex->attack_type;

    /*
     * Copy affects.
     */
    for (paf_from = pObjIndex->affected ; paf_from != NULL ; paf_from = paf_from->next )
    {
	if (affect_free == NULL)
	{
	    paf_to = alloc_perm(sizeof(*paf_to));
	}
	else
	{
	    paf_to = affect_free;
	    affect_free = affect_free->next;
	}

	paf_to->type		= paf_from->type;
	paf_to->duration	= paf_from->duration;
	paf_to->location	= paf_from->location;
	paf_to->modifier	= paf_from->modifier;
	paf_to->bitvector	= paf_from->bitvector;
	paf_to->next		= obj->affected;
	obj->affected	= paf_to;
    }

    /*
     * Mess with object properties.
     */
    switch ( obj->item_type )
    {
    default:
	bug( "Read_object: vnum %d bad type.", pObjIndex->vnum );
	break;

    case ITEM_LIGHT:
    case ITEM_TREASURE:
    case ITEM_FURNITURE:
    case ITEM_TRASH:
    case ITEM_CONTAINER:
    case ITEM_DRINK_CON:
    case ITEM_KEY:
    case ITEM_FOOD:
    case ITEM_BOAT:
    case ITEM_CORPSE:
    case ITEM_FOUNTAIN:
    case ITEM_WEAPON:
    case ITEM_ARMOR:
    case ITEM_MONEY:
    case ITEM_LOCKPICKS:
    case ITEM_BUILDING_OBJECT:
    case ITEM_BUILDING_WINDOW:
    case ITEM_WAGON_CONTROLS:
    case ITEM_RANGED_WEAPON:
    case ITEM_AMMUNITION:
	break;

    case ITEM_JEWELRY:
	rename_object_random( obj, TABLE_JEWELRY );
	break;

    case ITEM_SCROLL:
	rename_object_random( obj, TABLE_SCROLL );
	obj->value[0]	= number_fuzzy( obj->value[0] );
	break;

    case ITEM_WAND:
    case ITEM_STAFF:
	rename_object_random( obj, TABLE_STAFF );
	obj->value[0]	= number_fuzzy( obj->value[0] );
	obj->value[1]	= number_fuzzy( obj->value[1] );
	break;

    case ITEM_POTION:
    case ITEM_PILL:
	obj->value[0]	= number_fuzzy( number_fuzzy( obj->value[0] ) );
	break;
    }

    obj->next		= object_list;
    object_list		= obj;
    pObjIndex->number++;

    return obj;
}



/*
 * Clear a new character.
 */
void clear_char( CHAR_DATA *ch )
{
    static CHAR_DATA ch_zero;

    *ch				= ch_zero;
    ch->next_fighting		= NULL;
    ch->name			= &str_empty[0];
    ch->short_descr		= &str_empty[0];
    ch->long_descr		= &str_empty[0];
    ch->dynamic_descr		= NULL;
    ch->description		= &str_empty[0];
    ch->prompt			= NULL;
    ch->predelay_time		= 0;
    ch->predelay_info		= NULL;
    ch->position		= POS_STANDING;
    ch->hit[HIT_LOC_BODY]	= 20;
    ch->max_hit			= 20;
    ch->mana			= 100;
    ch->max_mana		= 100;
    ch->move			= 100;
    ch->max_move		= 100;
    ch->stun                    = 100;
    ch->max_stun                = 100;
    ch->perm_stat_phys_str	= 40;
    ch->perm_stat_phys_dex	= 40;
    ch->perm_stat_phys_tou	= 40;
    ch->perm_stat_phys_qui	= 40;
    ch->perm_stat_ment_int	= 40;
    ch->pReset			= NULL;
    ch->riding			= NULL;
    ch->switched_into		= NULL;
    ch->language		= 0;
    ch->move_speed		= MOVE_WALK;
    return;
}



/*
 * Free a character.
 */
void free_char( CHAR_DATA *ch )
{
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    AFFECT_DATA *paf;
    AFFECT_DATA *paf_next;

    for ( obj = ch->carrying; obj != NULL; obj = obj_next )
    {
	obj_next = obj->next_content;
	extract_obj( obj, FALSE );
    }

    for ( paf = ch->affected; paf != NULL; paf = paf_next )
    {
	paf_next = paf->next;
	affect_remove( ch, paf );
    }

    free_string( ch->name, MEM_MOB_NAME_FREE_CHAR );
    free_string( ch->namelist, MEM_MOB_NAMELIST_FREE_CHAR );
    free_string( ch->short_descr, MEM_MOB_SHORT_DESCR_FREE_CHAR );
    free_string( ch->long_descr, MEM_MOB_LONG_DESCR_FREE_CHAR );
    clear_dynamic( ch );
    free_string( ch->description, MEM_MOB_DESCRIPTION_FREE_CHAR );
    free_string( ch->spec_fun_name, MEM_MOB_SPEC_FUN_NAME_FREE_CHAR );
    free_string( ch->prompt, MEM_GENERAL );

    free_predelay( ch->predelay_info );

    if ( ch->pcdata != NULL )
    {
	free_string( ch->pcdata->pwd, MEM_PCDATA );
	free_string( ch->pcdata->bamfin, MEM_PCDATA );
	free_string( ch->pcdata->bamfout, MEM_PCDATA );
	free_string( ch->pcdata->email_address, MEM_PCDATA );
	free_string( ch->pcdata->rejected, MEM_PCDATA );
	ch->pcdata->next = pcdata_free;
	pcdata_free      = ch->pcdata;
    }

    ch->next	     = char_free;
    char_free	     = ch;
    return;
}


void free_extra_descr( EXTRA_DESCR_DATA *ed )
{
    if ( ed == NULL )
	return;

    free_string( ed->keyword, MEM_DIRECTION_KEYWORD );
    free_string( ed->description, MEM_DIRECTION_DESCRIPTION );

    ed->next = extra_descr_free;
    extra_descr_free = ed;

    return;
}

EXTRA_DESCR_DATA *new_extra_descr( void )
{
    EXTRA_DESCR_DATA *ed;

    if ( extra_descr_free == NULL )
    {
	ed                  = alloc_perm( sizeof(*ed) );
    }
    else
    {
	ed                  = extra_descr_free;
	extra_descr_free    = extra_descr_free->next;
    }

    return ed;
}


void free_predelay( PREDELAY_DATA *p )
{
    if ( p == NULL )
	return;

    p->fnptr = (DO_FUN *) predelay_free;
    predelay_free = p;

    return;
}


PREDELAY_DATA *new_predelay( void )
{
    PREDELAY_DATA *p;

    if ( predelay_free == NULL )
    {
	p                  = alloc_perm( sizeof(*p) );
    }
    else
    {
	p                  = predelay_free;
	predelay_free      = (PREDELAY_DATA *) predelay_free->fnptr;
    }

    return p;
}


void free_shop( SHOP_DATA *pShop )
{
    int i;
    SHOP_DATA *p;
    extern char *shopkeeper_messages[SHOP_MAX_MESSAGE];

    for ( i = 0; i < SHOP_MAX_MESSAGE; i++ )
    {
      if ( pShop->shop_messages[i] != shopkeeper_messages[i] )
	free_string( pShop->shop_messages[i], MEM_MOB_SHOP_MESSAGE );
    }

    if ( pShop == shop_first )
    {
	shop_first = pShop->next;

	if ( pShop == shop_last )
	    shop_last = NULL;
    }
    else
    {
	for ( p = shop_first; p != NULL; p = p->next )
	    if ( p->next == pShop )
	    {
		p->next = pShop->next;
		if ( pShop == shop_last )
		    shop_last = p;
		break;
	    }
    }

    free_mem( pShop, sizeof( *pShop ) );

    return;
}


SHOP_DATA *new_shop( void )
{
    SHOP_DATA *pShop;
    int i;
    extern char *shopkeeper_messages[SHOP_MAX_MESSAGE];

    pShop = alloc_mem( sizeof(*pShop) );

    for ( i = 0; i < MAX_TRADE; i++ )
	pShop->buy_type[i] = 0;

    pShop->profit_buy = 80;
    pShop->profit_sell = 120;

    pShop->open_hour = 0;
    pShop->close_hour = 23;

    pShop->gain = 0;

    for ( i = 0; i < SHOP_MAX_MESSAGE; i++ )
	pShop->shop_messages[i] = shopkeeper_messages[i];

    for ( i = 0; i < MAX_COIN; i++ )
    {
	pShop->coinage_buy[i] = 100;
	pShop->coinage_sell[i] = 100;
    }

    if ( shop_first == NULL )
	shop_first = pShop;
    if ( shop_last  != NULL )
	shop_last->next = pShop;

    shop_last	= pShop;
    pShop->next	= NULL;
    top_shop++;

    return pShop;
}


/*
 * free_exit and new_exit use to_room as a temporary next pointer.
 */
void free_exit( EXIT_DATA *exit )
{
    if ( exit == NULL )
	return;

    free_string( exit->description, MEM_DIRECTION_DESCRIPTION );
    free_string( exit->keyword, MEM_DIRECTION_KEYWORD );

    exit->to_room = (ROOM_INDEX_DATA *) exit_free;
    exit_free = exit;

    return;
}

EXIT_DATA *new_exit( )
{
    EXIT_DATA *exit;

    if ( exit_free == NULL )
    {
	exit = alloc_perm( sizeof(*exit) );
    }
    else
    {
	exit = exit_free;
	exit_free = (EXIT_DATA *) exit->to_room;
    }

    return exit;
}


void free_room( ROOM_INDEX_DATA *room_from )
{
    ROOM_INDEX_DATA *room_to;
    ROOM_INDEX_DATA *pRoomIndex;
    ROOM_INDEX_DATA *pRoom_next;
    CHAR_DATA *vch;
    EXTRA_DESCR_DATA *ed;
    int door;
    int iHash;

    room_to = get_room_index( ROOM_VNUM_CHAT );

    if ( room_from == room_to )
    {
	bug("Attempted free of chat room.", 0 );
	return;
    }

    for ( door = 0; door <= 5; door++ )
    {
	if ( room_from->exit[door] != NULL
	&& room_from->exit[door]->to_room != NULL
	&& room_from->exit[door]->to_room->area == room_from->area )
	{
	    room_to = room_from->exit[door]->to_room;
	    break;
	}
    }

    while ( room_from->contents != NULL )
	extract_obj( room_from->contents, TRUE );

    while ( room_from->flora != NULL )
      destroy_flora( room_from->flora, room_from );

    for ( door = 0; door < 6; door++ )
	free_exit( room_from->exit[door] );

    while ( room_from->people != NULL )
    {
	if ( IS_NPC( room_from->people ) )
	{
	    extract_char( room_from->people, TRUE );
	}
	else
	{
	    char_from_room( (vch = room_from->people) );
	    char_to_room( vch, room_to, FALSE );
	    do_look( vch, "auto" );
	}
    }

    for ( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
    {
	if ( room_index_hash[iHash] == NULL )
	    continue;

	for ( pRoomIndex = room_index_hash[iHash]; pRoomIndex != NULL; pRoomIndex = pRoomIndex->next )
	{
	    for ( door = 0; door <= 5; door++ )
	    {
		if ( pRoomIndex->exit[door] != NULL )
		{
		    if ( pRoomIndex->exit[door]->to_room == room_from )
		    {
			pRoomIndex->exit[door]->to_room = NULL;
			pRoomIndex->exit[door]->vnum = 0;
			pRoomIndex->exit[door]->exit_flags = 0;
			pRoomIndex->exit[door]->key = 0;

			if ( pRoomIndex->exit[door]->description[0] == '\0'
			&& pRoomIndex->exit[door]->keyword[0] == '\0' )
			{
			    free_exit( pRoomIndex->exit[door] );
			    pRoomIndex->exit[door] = NULL;
			}
		    }
		}
	    }
	}
    }

    iHash = room_from->vnum % MAX_KEY_HASH;
    for ( pRoomIndex = room_index_hash[iHash]; pRoomIndex != NULL; pRoomIndex = pRoom_next )
    {
	pRoom_next = pRoomIndex->next;

	if ( pRoomIndex == room_from )
	{
	    room_index_hash[iHash] = pRoomIndex->next;
	    break;
	}

	if ( pRoomIndex->next == room_from )
	{
	    pRoomIndex->next = room_from->next;
	    break;
	}
    }

   if ( room_from->area->room_first == room_from )
   {
	room_from->area->room_first = room_from->zone_next;
	if ( room_from->zone_next == NULL )
	    room_from->area->room_last = NULL;
   }
   else
    for ( pRoomIndex = room_from->area->room_first; pRoomIndex != NULL; pRoomIndex = pRoom_next )
    {
	pRoom_next = pRoomIndex->zone_next;

	if ( pRoom_next == room_from )
	{
	    pRoomIndex->zone_next = pRoom_next->zone_next;
	    if ( room_from->zone_next == NULL )
		room_from->area->room_last = pRoomIndex;
	    break;
	}
    }

    free_string( room_from->name, MEM_ROOM_NAME );
    free_string( room_from->description, MEM_ROOM_DESCRIPTION );
    free_string( room_from->spec_fun_name, MEM_ROOM_SPECIAL_NAME );

    while ( room_from->extra_descr != NULL )
    {
	ed = room_from->extra_descr;

	room_from->extra_descr = ed->next;

	free_extra_descr( ed );
    }

    room_from->next = room_free;
    room_free = room_from;

    return;
}


struct sector_type *new_sector_type( void )
{
  struct sector_type *s;

  if ( sector_type_free == NULL )
  {
    s = alloc_perm( sizeof(*s) );
  }
  else
  {
    s = sector_type_free;
    sector_type_free = s->next;
  }

  s->name = NULL;
  s->next = NULL;
  s->flags = 0;
  s->difficulty = 10;
  s->water_min = 300;
  s->water_degrade = 1;

  return s;
}

void free_sector_type( struct sector_type *s )
{
  if ( s == NULL )
    return;

/*
 * This should be freed, but with short, common words like
 * city, field, etc., it might be a bad idea.  The only time
 * this will leak memory is when a sector type is created
 * then freed during the same boot.  Otherwise it would be
 * in reserved string space and the free would just return.
  free_string( s->name, MEM_GENERAL );
 */

  s->next = sector_type_free;
  sector_type_free = s;

  return;
}


/*
 * Get an extra description from a list.
 */
char *get_extra_descr( const char *name, EXTRA_DESCR_DATA *ed )
{
    for ( ; ed != NULL; ed = ed->next )
    {
	if ( is_name( name, ed->keyword ) )
	    return ed->description;
    }
    return NULL;
}



/*
 * Translates mob virtual number to its mob index struct.
 * Hash table lookup.
 */
MOB_INDEX_DATA *get_mob_index( int vnum )
{
    MOB_INDEX_DATA *pMobIndex;

    for ( pMobIndex  = mob_index_hash[vnum % MAX_KEY_HASH];
	  pMobIndex != NULL;
	  pMobIndex  = pMobIndex->next )
    {
	if ( pMobIndex->vnum == vnum )
	    return pMobIndex;
    }

    if ( fBootDb )
    {
	bug( "Get_mob_index: bad vnum %d.", vnum );
	exit( 1 );
    }

    return NULL;
}



/*
 * Translates mob virtual number to its obj index struct.
 * Hash table lookup.
 */
OBJ_INDEX_DATA *get_obj_index( int vnum )
{
    OBJ_INDEX_DATA *pObjIndex;

    for ( pObjIndex  = obj_index_hash[vnum % MAX_KEY_HASH];
	  pObjIndex != NULL;
	  pObjIndex  = pObjIndex->next )
    {
	if ( pObjIndex->vnum == vnum )
	    return pObjIndex;
    }

    if ( fBootDb )
    {
	bug( "Get_obj_index: bad vnum %d.", vnum );
	exit( 1 );
    }

    return NULL;
}



/*
 * Translates mob virtual number to its room index struct.
 * Hash table lookup.
 */
ROOM_INDEX_DATA *get_room_index( int vnum )
{
    ROOM_INDEX_DATA *pRoomIndex;
    char buf [MAX_STRING_LENGTH];

    for ( pRoomIndex  = room_index_hash[vnum % MAX_KEY_HASH];
	  pRoomIndex != NULL;
	  pRoomIndex  = pRoomIndex->next )
    {
	if ( pRoomIndex->vnum == vnum )
	    return pRoomIndex;
    }

    if ( fBootDb )
    {
	sprintf( buf, "Get_room_index: bad vnum %d.", vnum );
	log_string( buf );
/*
	exit( 1 );
*/
    }

    return NULL;
}



void do_areas( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    AREA_DATA *pArea1;

    for ( pArea1 = area_first; pArea1 != NULL; pArea1 = pArea1->next )
    {
        sprintf( buf, "filename   Author    Area name     Vnums");
	sprintf( buf, "%-8s  %-35s %6d %6d %4d %10s\n\r",
	    pArea1->filename,
	    pArea1->name,
	    pArea1->vnum_start,
	    pArea1->vnum_final,
	    pArea1->area_bits,
	    ( IS_SET( pArea1->area_bits, ZONE_NOROOMS ) ? "(no rooms)" : " " ));
	send_to_char( buf, ch );
    }

    return;
}




/*
 * This function is here to aid in debugging.
 * If the last expression in a function is another function call,
 *   gcc likes to generate a JMP instead of a CALL.
 * This is called "tail chaining."
 * It hoses the debugger call stack for that call.
 * So I make this the last call in certain critical functions,
 *   where I really need the call stack to be right for debugging!
 *
 * If you don't understand this, then LEAVE IT ALONE.
 * Don't remove any calls to tail_chain anywhere.
 *
 * -- Furey
 */
void tail_chain( void )
{
    return;
}
