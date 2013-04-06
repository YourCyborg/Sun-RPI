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
#include "languages.h"

#if !defined(macintosh)
extern	int	_filbuf		args( (FILE *) );
#endif



/*
 * Array of containers read for proper re-nesting of objects.
 */
#define MAX_NEST	100
static	OBJ_DATA *	rgObjNest	[MAX_NEST];



/*
 * Local functions.
 */
void	fwrite_char	args( ( CHAR_DATA *ch,  FILE *fp ) );
void    pwrite_char     args( ( CHAR_DATA *ch,  FILE *fp ) );
void	fwrite_obj	args( ( CHAR_DATA *ch,  OBJ_DATA  *obj,
			    FILE *fp, int iNest, bool fList ) );
void	fread_char	args( ( CHAR_DATA *ch,  FILE *fp ) );
void	reverse_items	args( ( CHAR_DATA *ch ) );
void	fread_obj	args( ( CHAR_DATA *ch, ROOM_INDEX_DATA *room,
			    FILE *fp ) );



/*
 * Save a character and inventory.
 * Would be cool to save NPC's too for quest purposes,
 *   some of the infrastructure is provided.
 */
void save_char_load( CHAR_DATA *ch )
{
    bool fNewPlayer = FALSE;
    char strsave[MAX_INPUT_LENGTH];
    FILE *fp;

    if ( IS_NPC(ch) )
	return;

    if ( ch->desc != NULL && ch->desc->original != NULL )
	ch = ch->desc->original;

    if ( IS_SET( ch->act, PLR_UNAPPROVED ) )
	fNewPlayer = TRUE;

    ch->save_time = current_time;
    fclose( fpReserve );
    if ( IS_SET( ch->act, PLR_UNAPPROVED ) )
	sprintf( strsave, "%s%s", UNAPPROVED_DIR, capitalize( ch->name ) );
    else if ( IS_SET( ch->act, PLR_REJECTED ) )
	sprintf( strsave, "%s%s", REJECTED_DIR, capitalize( ch->name ) );
    else
	sprintf( strsave, "%s%s", PLAYER_DIR, capitalize( ch->name ) );

    if ( ( fp = fopen( strsave, "w" ) ) == NULL )
    {
	bug( "Save_char_obj: fopen", 0 );
	perror( strsave );
    }
    else
    {
	pwrite_char( ch, fp );
	if ( ch->carrying != NULL )
	{
	    OBJ_DATA *obj;
	    int i;

	    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
	    {
		if ( obj->wear_loc == gn_wear_none )
		    fwrite_obj( ch, obj, fp, 0, FALSE );
	    }
	    for ( i = 0; i < MAX_WEAR; i++ )
	    {
		for ( obj = ch->wearing[i]; obj != NULL; obj = obj->next_worn )
		    fwrite_obj( ch, obj, fp, 0, FALSE );
	    }
	}
	fprintf( fp, "#END\n" );
    }
    fclose( fp );
    fpReserve = fopen( NULL_FILE, "r" );
    return;
}

void save_char_obj( CHAR_DATA *ch )
{
    bool fNewPlayer = FALSE;
    char strsave[MAX_INPUT_LENGTH];
    FILE *fp;

    if ( IS_NPC(ch) )
	return;

    if ( ch->desc != NULL && ch->desc->original != NULL )
	ch = ch->desc->original;

    if ( IS_SET( ch->act, PLR_UNAPPROVED ) )
	fNewPlayer = TRUE;

    ch->save_time = current_time;
    fclose( fpReserve );
    if ( IS_SET( ch->act, PLR_UNAPPROVED ) )
	sprintf( strsave, "%s%s", UNAPPROVED_DIR, capitalize( ch->name ) );
    else if ( IS_SET( ch->act, PLR_REJECTED ) )
	sprintf( strsave, "%s%s", REJECTED_DIR, capitalize( ch->name ) );
    else
	sprintf( strsave, "%s%s", PLAYER_DIR, capitalize( ch->name ) );

    if ( ( fp = fopen( strsave, "w" ) ) == NULL )
    {
	bug( "Save_char_obj: fopen", 0 );
	perror( strsave );
    }
    else
    {
	fwrite_char( ch, fp );
	if ( ch->carrying != NULL )
	{
	    OBJ_DATA *obj;
	    int i;

	    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
	    {
		if ( obj->wear_loc == gn_wear_none )
		    fwrite_obj( ch, obj, fp, 0, FALSE );
	    }
	    for ( i = 0; i < MAX_WEAR; i++ )
	    {
		for ( obj = ch->wearing[i]; obj != NULL; obj = obj->next_worn )
		    fwrite_obj( ch, obj, fp, 0, FALSE );
	    }
	}
	fprintf( fp, "#END\n" );
    }
    fclose( fp );
    fpReserve = fopen( NULL_FILE, "r" );
    return;
}

/*
 * Write the char without roomdesc (used for punload)
 */

void pwrite_char( CHAR_DATA *ch, FILE *fp )
{
    bool fNewPlayer = FALSE;
    AFFECT_DATA *paf;
    SKILL_DELAY_DATA *delay;
    int sn;

    if ( !IS_NPC( ch ) && IS_SET( ch->act, PLR_UNAPPROVED ) )
	fNewPlayer = TRUE;

    fprintf( fp, "#%s\n", IS_NPC(ch) ? "MOB" : "PLAYER"		);

    fprintf( fp, "Name         " );
    write_string( fp, ch->name );
    fprintf( fp, "~\n" );
    fprintf( fp, "Namelist     " );
    write_string( fp, ch->namelist );
    fprintf( fp, "~\n" );
    fprintf( fp, "ShortDescr   " );
    write_string( fp, ch->short_descr );
    fprintf( fp, "~\n" );
    fprintf( fp, "LongDescr    " );
    write_string( fp, ch->long_descr );
    fprintf( fp, "~\n" );
    fprintf( fp, "Description  " );
    write_string( fp, ch->description );
    fprintf( fp, "~\n" );
  if ( IS_NPC( ch ) && ch->spec_fun != NULL )
  {
    fprintf( fp, "Spec         " );
    write_string( fp, ch->spec_fun_name );
    fprintf( fp, "~\n" );
  }
    fprintf( fp, "Language     '%s'\n",
      skill_table[*language_table[ch->language].pgsn].name );
  if ( !IS_NPC( ch ) )
  {
    fprintf( fp, "Bamfin       " );
    write_string( fp, ch->pcdata->bamfin );
    fprintf( fp, "~\n" );
    fprintf( fp, "Bamfout      " );
    write_string( fp, ch->pcdata->bamfout );
    fprintf( fp, "~\n" );
   if ( !fNewPlayer && ch->prompt != NULL )
   {
    fprintf( fp, "Prompt       " );
    write_string( fp, ch->prompt );
    fprintf( fp, "~\n" );
   }
    fprintf( fp, "Email        " );
    write_string( fp, ch->pcdata->email_address );
    fprintf( fp, "~\n" );
    fprintf( fp, "Pagelen      %d\n",	ch->pcdata->pagelen	);
   if ( IS_SET( ch->act, PLR_REJECTED ) )
   {
    fprintf( fp, "Reject       " );
    write_string( fp, ch->pcdata->rejected );
    fprintf( fp, "~\n" );
   }
  }
    fprintf( fp, "Sex          %d\n",	ch->sex			);
    fprintf( fp, "Class        %d\n",	ch->class		);
    fprintf( fp, "Race         '%s'\n", race_array[ch->race]->race_name	);
  if ( ch->clan != 0 )
    fprintf( fp, "Clan         %d\n",	ch->clan		);
    fprintf( fp, "Level        %d\n",	ch->level		);
  if ( ch->trust != 0 )
    fprintf( fp, "Trust        %d\n",	ch->trust		);
  if ( ch->time != 0 )
    fprintf( fp, "Time         %s\n",    ch->time             );
  if ( !IS_NPC( ch ) )
    fprintf( fp, "Played       %d\n",
	ch->pcdata->played + (int) (current_time - ch->pcdata->logon) );
  if ( !IS_NPC( ch ) )
    fprintf( fp, "Birth        %d\n",	(int) ch->pcdata->birth	);
  

  {
    int a;
    a = ch->act;
    if ( !IS_NPC( ch ) )
	REMOVE_BIT( a, PLR_REVIEWING );
    fprintf( fp, "Act          %d\n",   a			);
  }

  if ( !fNewPlayer )
  {
    fprintf( fp, "HpManaMoveStun   %d %d %d %d %d %d %d %d\n",
	ch->hit[HIT_LOC_BODY], ch->max_hit, ch->mana, ch->max_mana, ch->move, ch->max_move, ch->stun, ch->max_stun );
    fprintf( fp, "HeightWeight %d %d\n",
	ch->height, ch->weight );
    fprintf( fp, "AffectedBy   %d\n",	ch->affected_by		);
    fprintf( fp, "Position     %d\n",	ch->position		);

  if ( ch->saving_throw != 0 )
    fprintf( fp, "SavingThrow  %d\n",	ch->saving_throw	);
    fprintf( fp, "Alignment    %d\n",	ch->alignment		);
  if ( ch->damroll != 0 )
    fprintf( fp, "Damroll      %d\n",	ch->damroll		);
  if ( ch->hitroll != 0 )
    fprintf( fp, "Hitroll      %d\n",	ch->hitroll		);
  if ( ch->absorb[0] != 0 || ch->absorb[1] != 0 || ch->absorb[2] != 0
  || ch->absorb[3] != 0 )
    fprintf( fp, "Absorb       %d %d %d %d\n",
	ch->absorb[0],
	ch->absorb[1],
	ch->absorb[2],
	ch->absorb[3] );
  if ( ch->general_absorb != 0 )
    fprintf( fp, "AbsGeneral   %d\n",	ch->general_absorb	);
  if ( ch->channeling != 0 )
    fprintf( fp, "Channeling   %d\n",	ch->channeling		);
  if ( ch->deaf != 0 )
    fprintf( fp, "Deaf         %d\n",	ch->deaf		);
  if ( ch->light != 0 )
    fprintf( fp, "Light        %d\n",	ch->light		);
    fprintf( fp, "Vision       %d %d %d %d\n",
	ch->vision_min,
	ch->vision_max,
	ch->vision_current,
	ch->vision_range );
   }

  if ( !fNewPlayer )
  {
    fprintf( fp, "StatPhysStrength %d %d\n",
      ch->perm_stat_phys_str, ch->mod_stat_phys_str );
    fprintf( fp, "StatPhysDexterity %d %d\n",
      ch->perm_stat_phys_dex, ch->mod_stat_phys_dex );
    fprintf( fp, "StatPhysToughness %d %d\n",
      ch->perm_stat_phys_tou, ch->mod_stat_phys_tou );
    fprintf( fp, "StatPhysQuickness %d %d\n",
      ch->perm_stat_phys_qui, ch->mod_stat_phys_qui );
    fprintf( fp, "StatMentIntelligence %d %d\n",
      ch->perm_stat_ment_int, ch->mod_stat_ment_int );

    fprintf( fp, "StatOffense %d %d\n",
      ch->perm_offense, ch->mod_offense );
    fprintf( fp, "StatDefense %d %d\n",
      ch->perm_defense, ch->mod_defense );
  }

    if ( IS_NPC(ch) )
    {
	fprintf( fp, "MVnum        %d\n",	ch->pIndexData->vnum	);
    }
    else
    {
	fprintf( fp, "Password     " );
	write_string( fp, ch->pcdata->pwd );
	fprintf( fp, "~\n" );

      if ( !fNewPlayer )
      {
	fprintf( fp, "Condition    %d %d %d\n",
	    ch->pcdata->condition[0],
	    ch->pcdata->condition[1],
	    ch->pcdata->condition[2] );

	for ( sn = 0; sn < MAX_SKILL; sn++ )
	{
	    if ( skill_table[sn].name != NULL
	    && (ch->pcdata->learned[sn] > 0 || ch->pcdata->power_level[sn] > 0 ) )
	    {
		fprintf( fp, "Skill        %d %d '%s'\n",
		    ch->pcdata->learned[sn],
		    ch->pcdata->power_level[sn],
		    skill_table[sn].name );
	    }
	}

	for ( delay = ch->pcdata->skill_delays;
	      delay != NULL; delay = delay->next )
	{
	  if ( current_time >= delay->expire )
	    continue;
          fprintf( fp, "SkillDelay   %d '%s'\n",
	    (int) delay->expire, skill_table[delay->skill].name );
	}
      }
    }

  if ( !fNewPlayer )
  {
    for ( paf = ch->affected; paf != NULL; paf = paf->next )
    {
	/* Thx Alander */
	if ( paf->type < 0 || paf->type >= MAX_SKILL )
	    continue;

	fprintf( fp, "AffectData   '%s' %3d %3d %3d %10d\n",
	    skill_table[paf->type].name,
	    paf->duration,
	    paf->modifier,
	    paf->location,
	    paf->bitvector
	    );
    }
  }

    fprintf( fp, "End\n\n" );
    return;
}

/*
 * Write the char.
 */
void fwrite_char( CHAR_DATA *ch, FILE *fp )
{
    bool fNewPlayer = FALSE;
    AFFECT_DATA *paf;
    SKILL_DELAY_DATA *delay;
    int sn;

    if ( !IS_NPC( ch ) && IS_SET( ch->act, PLR_UNAPPROVED ) )
	fNewPlayer = TRUE;

    fprintf( fp, "#%s\n", IS_NPC(ch) ? "MOB" : "PLAYER"		);

    fprintf( fp, "Name         " );
    write_string( fp, ch->name );
    fprintf( fp, "~\n" );
    fprintf( fp, "Namelist     " );
    write_string( fp, ch->namelist );
    fprintf( fp, "~\n" );
    fprintf( fp, "ShortDescr   " );
    write_string( fp, ch->short_descr );
    fprintf( fp, "~\n" );
    fprintf( fp, "LongDescr    " );
    write_string( fp, ch->long_descr );
    fprintf( fp, "~\n" );
    fprintf( fp, "Description  " );
    write_string( fp, ch->description );
    fprintf( fp, "~\n" );
  if ( IS_NPC( ch ) && ch->spec_fun != NULL )
  {
    fprintf( fp, "Spec         " );
    write_string( fp, ch->spec_fun_name );
    fprintf( fp, "~\n" );
  }
    fprintf( fp, "Language     '%s'\n",
      skill_table[*language_table[ch->language].pgsn].name );
  if ( !IS_NPC( ch ) )
  {
    fprintf( fp, "Bamfin       " );
    write_string( fp, ch->pcdata->bamfin );
    fprintf( fp, "~\n" );
    fprintf( fp, "Bamfout      " );
    write_string( fp, ch->pcdata->bamfout );
    fprintf( fp, "~\n" );
   if ( !fNewPlayer && ch->prompt != NULL )
   {
    fprintf( fp, "Prompt       " );
    write_string( fp, ch->prompt );
    fprintf( fp, "~\n" );
   }
    fprintf( fp, "Email        " );
    write_string( fp, ch->pcdata->email_address );
    fprintf( fp, "~\n" );
    fprintf( fp, "Pagelen      %d\n",	ch->pcdata->pagelen	);
   if ( IS_SET( ch->act, PLR_REJECTED ) )
   {
    fprintf( fp, "Reject       " );
    write_string( fp, ch->pcdata->rejected );
    fprintf( fp, "~\n" );
   }
  }
    fprintf( fp, "Sex          %d\n",	ch->sex			);
    fprintf( fp, "Class        %d\n",	ch->class		);
    fprintf( fp, "Race         '%s'\n", race_array[ch->race]->race_name	);
  if ( ch->clan != 0 )
    fprintf( fp, "Clan         %d\n",	ch->clan		);
    fprintf( fp, "Level        %d\n",	ch->level		);
  if ( ch->trust != 0 )
    fprintf( fp, "Trust        %d\n",	ch->trust		);
  if ( ch->time != 0 )
    fprintf( fp, "Time         %s\n",    ch->time             );
  if ( !IS_NPC( ch ) )
    fprintf( fp, "Played       %d\n",
	ch->pcdata->played + (int) (current_time - ch->pcdata->logon) );
  if ( !IS_NPC( ch ) )
    fprintf( fp, "Birth        %d\n",	(int) ch->pcdata->birth	);
  if ( !fNewPlayer )
    fprintf( fp, "Room         %d\n",
	(  ch->in_room == get_room_index( ROOM_VNUM_LIMBO )
	&& ch->was_in_room != NULL )
	    ? ch->was_in_room->vnum
	    : ch->in_room->vnum );

  {
    int a;
    a = ch->act;
    if ( !IS_NPC( ch ) )
	REMOVE_BIT( a, PLR_REVIEWING );
    fprintf( fp, "Act          %d\n",   a			);
  }

  if ( !fNewPlayer )
  {
    fprintf( fp, "HpManaMoveStun   %d %d %d %d %d %d %d %d\n",
	ch->hit[HIT_LOC_BODY], ch->max_hit, ch->mana, ch->max_mana, ch->move, ch->max_move, ch->stun, ch->max_stun );
    fprintf( fp, "HeightWeight %d %d\n",
	ch->height, ch->weight );
    fprintf( fp, "AffectedBy   %d\n",	ch->affected_by		);
    fprintf( fp, "Position     %d\n",	ch->position		);

  if ( ch->saving_throw != 0 )
    fprintf( fp, "SavingThrow  %d\n",	ch->saving_throw	);
    fprintf( fp, "Alignment    %d\n",	ch->alignment		);
  if ( ch->damroll != 0 )
    fprintf( fp, "Damroll      %d\n",	ch->damroll		);
  if ( ch->hitroll != 0 )
    fprintf( fp, "Hitroll      %d\n",	ch->hitroll		);
  if ( ch->absorb[0] != 0 || ch->absorb[1] != 0 || ch->absorb[2] != 0
  || ch->absorb[3] != 0 )
    fprintf( fp, "Absorb       %d %d %d %d\n",
	ch->absorb[0],
	ch->absorb[1],
	ch->absorb[2],
	ch->absorb[3] );
  if ( ch->general_absorb != 0 )
    fprintf( fp, "AbsGeneral   %d\n",	ch->general_absorb	);
  if ( ch->channeling != 0 )
    fprintf( fp, "Channeling   %d\n",	ch->channeling		);
  if ( ch->deaf != 0 )
    fprintf( fp, "Deaf         %d\n",	ch->deaf		);
  if ( ch->light != 0 )
    fprintf( fp, "Light        %d\n",	ch->light		);
    fprintf( fp, "Vision       %d %d %d %d\n",
	ch->vision_min,
	ch->vision_max,
	ch->vision_current,
	ch->vision_range );
   }

  if ( !fNewPlayer )
  {
    fprintf( fp, "StatPhysStrength %d %d\n",
      ch->perm_stat_phys_str, ch->mod_stat_phys_str );
    fprintf( fp, "StatPhysDexterity %d %d\n",
      ch->perm_stat_phys_dex, ch->mod_stat_phys_dex );
    fprintf( fp, "StatPhysToughness %d %d\n",
      ch->perm_stat_phys_tou, ch->mod_stat_phys_tou );
    fprintf( fp, "StatPhysQuickness %d %d\n",
      ch->perm_stat_phys_qui, ch->mod_stat_phys_qui );
    fprintf( fp, "StatMentIntelligence %d %d\n",
      ch->perm_stat_ment_int, ch->mod_stat_ment_int );

    fprintf( fp, "StatOffense %d %d\n",
      ch->perm_offense, ch->mod_offense );
    fprintf( fp, "StatDefense %d %d\n",
      ch->perm_defense, ch->mod_defense );
  }

    if ( IS_NPC(ch) )
    {
	fprintf( fp, "MVnum        %d\n",	ch->pIndexData->vnum	);
    }
    else
    {
	fprintf( fp, "Password     " );
	write_string( fp, ch->pcdata->pwd );
	fprintf( fp, "~\n" );

      if ( !fNewPlayer )
      {
	fprintf( fp, "Condition    %d %d %d\n",
	    ch->pcdata->condition[0],
	    ch->pcdata->condition[1],
	    ch->pcdata->condition[2] );

	for ( sn = 0; sn < MAX_SKILL; sn++ )
	{
	    if ( skill_table[sn].name != NULL
	    && (ch->pcdata->learned[sn] > 0 || ch->pcdata->power_level[sn] > 0 ) )
	    {
		fprintf( fp, "Skill        %d %d '%s'\n",
		    ch->pcdata->learned[sn],
		    ch->pcdata->power_level[sn],
		    skill_table[sn].name );
	    }
	}

	for ( delay = ch->pcdata->skill_delays;
	      delay != NULL; delay = delay->next )
	{
	  if ( current_time >= delay->expire )
	    continue;
          fprintf( fp, "SkillDelay   %d '%s'\n",
	    (int) delay->expire, skill_table[delay->skill].name );
	}
      }
    }

  if ( !fNewPlayer )
  {
    for ( paf = ch->affected; paf != NULL; paf = paf->next )
    {
	/* Thx Alander */
	if ( paf->type < 0 || paf->type >= MAX_SKILL )
	    continue;

	fprintf( fp, "AffectData   '%s' %3d %3d %3d %10d\n",
	    skill_table[paf->type].name,
	    paf->duration,
	    paf->modifier,
	    paf->location,
	    paf->bitvector
	    );
    }
  }

    fprintf( fp, "End\n\n" );
    return;
}



/*
 * Write an object and its contents.
 */
void fwrite_obj( CHAR_DATA *ch, OBJ_DATA *obj, FILE *fp, int iNest, bool fList )
{
    EXTRA_DESCR_DATA *ed;
    AFFECT_DATA *paf;
    extern struct wear_data const wear_info[];

    /*
     * Slick recursion to write lists backwards,
     *   so loading them will load in forwards order.
     *
     * Rip: Added fList so we can use this for containers, but not worn eq.
     */
    if ( obj->next_content != NULL && fList )
	fwrite_obj( ch, obj->next_content, fp, iNest, TRUE );

    /*
     * Castrate storage characters.
     */
/*
    if ( obj->item_type == ITEM_KEY )
	return;
*/

    fprintf( fp, "#OBJECT\n" );
    fprintf( fp, "Nest         %d\n",	iNest			     );
    fprintf( fp, "Name         " );
    write_string( fp, obj->name );
    fprintf( fp, "~\n" );
    fprintf( fp, "ShortDescr   " );
    write_string( fp, obj->short_descr );
    fprintf( fp, "~\n" );
    fprintf( fp, "LookDescr    " );
    write_string( fp, obj->look_descr );
    fprintf( fp, "~\n" );
    fprintf( fp, "Description  " );
    write_string( fp, obj->description );
    fprintf( fp, "~\n" );
   if ( obj->spec_fun != NULL )
   {
    fprintf( fp, "Spec         " );
    write_string( fp, obj->spec_fun_name );
    fprintf( fp, "~\n" );
   }
    fprintf( fp, "OVnum        %d\n",	obj->pIndexData->vnum	     );
    fprintf( fp, "ExtraFlags   %d\n",	obj->extra_flags	     );
    fprintf( fp, "WearFlags    %d\n",	obj->wear_flags		     );
    fprintf( fp, "Worn         '%s'\n",
	obj->wear_loc > -1 ? wear_info[obj->wear_loc].name : "none" );
    fprintf( fp, "ItemType     %d\n",	obj->item_type		     );
    fprintf( fp, "Weight       %d\n",	obj->weight		     );
    fprintf( fp, "Capacity     %d\n",	obj->capacity		     );
    fprintf( fp, "Size         %d\n",	obj->size		     );
    fprintf( fp, "Timer        %d\n",	obj->timer		     );
    fprintf( fp, "Cost         %d\n",	obj->cost		     );
    fprintf( fp, "Color        %d\n",	obj->color		     );
    fprintf( fp, "Material    '%s'\n",	material_table[obj->material].name );
    fprintf( fp, "Values       %d %d %d %d\n",
	obj->value[0], obj->value[1], obj->value[2], obj->value[3]	     );
    fprintf( fp, "Dam          %d %d\n",
	obj->dam[0], obj->dam[1] );
    fprintf( fp, "Toughness    %d\n",	obj->toughness		    );
    fprintf( fp, "Attack       %d\n",	obj->attack_type	     );

    switch ( obj->item_type )
    {
    case ITEM_POTION:
    case ITEM_SCROLL:
	if ( obj->value[1] > 0 )
	{
	    fprintf( fp, "Spell 1      '%s'\n", 
		skill_table[obj->value[1]].name );
	}

	if ( obj->value[2] > 0 )
	{
	    fprintf( fp, "Spell 2      '%s'\n", 
		skill_table[obj->value[2]].name );
	}

	if ( obj->value[3] > 0 )
	{
	    fprintf( fp, "Spell 3      '%s'\n", 
		skill_table[obj->value[3]].name );
	}

	break;

    case ITEM_PILL:
    case ITEM_STAFF:
    case ITEM_WAND:
	if ( obj->value[3] > 0 )
	{
	    fprintf( fp, "Spell 3      '%s'\n", 
		skill_table[obj->value[3]].name );
	}

	break;
    }

    for ( paf = obj->affected; paf != NULL; paf = paf->next )
    {
/* fixed by Rip to allow enchanted weapons to save */
	if ( paf->type < -1 || paf->type >= MAX_SKILL )
	{
	    continue;
	}
	else if (paf->type == -1)
	{
	    fprintf( fp, "AffectData   'unknown' %d %d %d %d\n",
		paf->duration,
		paf->modifier,
		paf->location,
		paf->bitvector
		);
	}
	else
	{
	    fprintf( fp, "AffectData   '%s' %d %d %d %d\n",
		skill_table[paf->type].name,
		paf->duration,
		paf->modifier,
		paf->location,
		paf->bitvector
		);
	}
    }

    for ( ed = obj->extra_descr; ed != NULL; ed = ed->next )
    {
	fprintf( fp, "ExtraDescr   " );
	write_string( fp, ed->keyword );
	fprintf( fp, "~\n" );
	write_string( fp, ed->description );
	fprintf( fp, "~\n" );
    }

    fprintf( fp, "End\n\n" );

    if ( obj->contains != NULL )
	fwrite_obj( ch, obj->contains, fp, iNest + 1, TRUE );

    return;
}




/*
 * Load a char and inventory into a new ch structure.
 */
bool load_char_obj( DESCRIPTOR_DATA *d, char *name )
{
    char strsave[MAX_INPUT_LENGTH];
    CHAR_DATA *ch;
    FILE *fp;
    bool found = FALSE;

    ch = new_char( );
    init_char( ch, name, TRUE );

    if ( d != NULL )
    {
	d->character			= ch;
	ch->desc				= d;
    }

    fclose( fpReserve );
    sprintf( strsave, "%s%s", PLAYER_DIR, capitalize( name ) );
    if ( ( fp = fopen( strsave, "r" ) ) == NULL )
    {
	sprintf( strsave, "%s%s", UNAPPROVED_DIR, capitalize( name ) );
	fp = fopen( strsave, "r" );
    }
    if ( fp == NULL )
    {
	sprintf( strsave, "%s%s", REJECTED_DIR, capitalize( name ) );
	fp = fopen( strsave, "r" );
    }
    if ( fp != NULL )
    {
	load_char_from_file( ch, fp );
	fclose( fp );
	found = TRUE;
    }

    fpReserve = fopen( NULL_FILE, "r" );

    return found;
}

CHAR_DATA *new_char( void )
{
    CHAR_DATA *ch;

    if ( char_free == NULL )
    {
	ch				= alloc_perm( sizeof(*ch) );
    }
    else
    {
	ch				= char_free;
	char_free			= char_free->next;
    }
    clear_char( ch );
    ch->desc = NULL;

    return ch;
}

void init_char( CHAR_DATA *ch, char *name, bool fPC )
{
    static PC_DATA pcdata_zero;
    int i;

  if ( fPC )
  {
    if ( pcdata_free == NULL )
    {
	ch->pcdata			= alloc_perm( sizeof(*ch->pcdata) );
    }
    else
    {
	ch->pcdata			= pcdata_free;
	pcdata_free			= pcdata_free->next;
    }
    *ch->pcdata				= pcdata_zero;

    ch->pcdata->pwd			= str_dup( "" );
    ch->pcdata->bamfin			= str_dup( "" );
    ch->pcdata->bamfout			= str_dup( "" );
    ch->pcdata->rejected		= str_dup( "" );
    ch->pcdata->email_address		= str_dup( "" );
    ch->pcdata->birth			= current_time;
    ch->pcdata->logon			= current_time;
    ch->pcdata->pagelen			= 20;
    ch->pcdata->condition[COND_THIRST]	= 48;
    ch->pcdata->condition[COND_FULL]	= 48;
    ch->pcdata->skill_delays	= NULL;

    for ( i = 0; i < MAX_SKILL; i++ )
    {
      ch->pcdata->learned[i] = 0;
      ch->pcdata->power_level[i] = 0;
    }
  }

    if( name != NULL )
      ch->name				= str_dup( name );
    else
      ch->name = NULL;
    ch->prompt				= NULL;
    ch->act				= PLR_BLANK
					| PLR_COMBINE
					| PLR_PROMPT;
    ch->damnodice			= 1;
    ch->damsizedice			= 4;
    ch->perm_stat_phys_str		= 25;
    ch->perm_stat_phys_dex		= 25;
    ch->perm_stat_phys_tou		= 25;
    ch->perm_stat_phys_qui		= 25;
    ch->perm_stat_ment_int		= 25; 

    return;
}

void load_char_from_file( CHAR_DATA *ch, FILE *fp )
{
    int iNest;

    for ( iNest = 0; iNest < MAX_NEST; iNest++ )
	rgObjNest[iNest] = NULL;

    for ( ; ; )
    {
	char letter;
	char *word;

	letter = fread_letter( fp );
	if ( letter == '*' )
	{
	    fread_to_eol( fp );
	    continue;
	}

	if ( letter != '#' )
	{
	    bug( "Load_char_obj: # not found.", 0 );
	    break;
	}

	word = fread_word( fp );
	if      ( !str_cmp( word, "PLAYER" ) ) fread_char ( ch, fp );
	else if ( !str_cmp( word, "OBJECT" ) ) fread_obj  ( ch, NULL, fp );
	else if ( !str_cmp( word, "END"    ) ) break;
	else
	{
	    bug( "Load_char_obj: bad section.", 0 );
	    break;
	}
    }

    reverse_items( ch );
}


/*
 * Read in a char.
 */

#if defined(KEY)
#undef KEY
#endif

#define KEY( literal, field, value )					\
				if ( !str_cmp( word, literal ) )	\
				{					\
				    field  = value;			\
				    fMatch = TRUE;			\
				    break;				\
				}

void fread_char( CHAR_DATA *ch, FILE *fp )
{
    char buf[MAX_STRING_LENGTH];
    char *word;
    bool fMatch;
    int temp;

    for ( ; ; )
    {
	word   = feof( fp ) ? "End" : fread_word( fp );
	fMatch = FALSE;

	switch ( UPPER(word[0]) )
	{
	case '*':
	    fMatch = TRUE;
	    fread_to_eol( fp );
	    break;

	case 'A':
	    if ( !str_cmp( word, "Act" ) )
	    {
		if ( IS_SET( ch->act, PLR_REJECTED ) )
		{
		    ch->act = fread_number( fp );
		    SET_BIT( ch->act, PLR_REJECTED );
		}
		else
		    ch->act = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    KEY( "Act",		ch->act,		fread_number( fp ) );
	    KEY( "AffectedBy",	ch->affected_by,	fread_number( fp ) );
	    KEY( "Alignment",	ch->alignment,		fread_number( fp ) );
	    KEY( "Armor",	temp,		fread_number( fp ) );
	    KEY( "AbsGeneral",	ch->general_absorb,	fread_number( fp ) );

	    if ( !str_cmp( word, "Absorb" ) )
	    {
		ch->absorb[0] = fread_number( fp );
		ch->absorb[1] = fread_number( fp );
		ch->absorb[2] = fread_number( fp );
		ch->absorb[3] = fread_number( fp );

		fMatch = TRUE;
		break;
	    }

	    if ( !str_cmp( word, "Affect" ) || !str_cmp( word, "AffectData" ) )
	    {
		AFFECT_DATA *paf;

		if ( affect_free == NULL )
		{
		    paf		= alloc_perm( sizeof(*paf) );
		}
		else
		{
		    paf		= affect_free;
		    affect_free	= affect_free->next;
		}

		if ( !str_cmp( word, "Affect" ) )
		{
		    /* Obsolete 2.0 form. */
		    paf->type	= fread_number( fp );
		}
		else
		{
		    int sn;

		    sn = skill_lookup( fread_word( fp ) );
		    if ( sn < 0 )
			;
/*  don't call bug while player file open!!
			bug( "Fread_char: unknown skill.", 0 );
*/
		    else
			paf->type = sn;
		}

		paf->duration	= fread_number( fp );
		paf->modifier	= fread_number( fp );
		paf->location	= fread_number( fp );
		paf->bitvector	= fread_number( fp );
		paf->next	= ch->affected;
		ch->affected	= paf;
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'B':
	    KEY( "Bamfin",	ch->pcdata->bamfin,	fread_string( fp ) );
	    KEY( "Bamfout",	ch->pcdata->bamfout,	fread_string( fp ) );

	    if ( !str_cmp( word, "Birth" ) )
	    {
		ch->pcdata->birth = (time_t) fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'C':
	    KEY( "Channeling",	ch->channeling,		fread_number( fp ) );
	    KEY( "Clan",	ch->clan,		fread_number( fp ) );
	    KEY( "Class",	ch->class,		fread_number( fp ) );

	    if ( !str_cmp( word, "Condition" ) )
	    {
		ch->pcdata->condition[0] = fread_number( fp );
		ch->pcdata->condition[1] = fread_number( fp );
		ch->pcdata->condition[2] = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'D':
	    KEY( "Damroll",	ch->damroll,		fread_number( fp ) );
	    KEY( "Deaf",	ch->deaf,		fread_number( fp ) );
	    KEY( "Description",	ch->description,	fread_string( fp ) );
	    break;

	case 'E':
	    KEY( "Email",	ch->pcdata->email_address,	fread_string( fp ) );
	    if ( !str_cmp( word, "End" ) )
		return;
	    KEY( "Exp",		temp,		fread_number( fp ) );
	    break;

	case 'H':
	    if ( !str_cmp( word, "HeightWeight" ) )
	    {
		ch->height	= fread_number( fp );
		ch->weight	= fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    KEY( "Hitroll",	ch->hitroll,		fread_number( fp ) );

	    if ( !str_cmp( word, "HpManaMoveStun" ) )
	    {
		ch->hit[HIT_LOC_BODY]		= fread_number( fp );
		ch->max_hit	= fread_number( fp );
		ch->mana	= fread_number( fp );
		ch->max_mana	= fread_number( fp );
		ch->move	= fread_number( fp );
		ch->max_move	= fread_number( fp );
                ch->stun        = fread_number( fp );
                ch->max_stun    = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'L':
	    KEY( "Level",	ch->level,		fread_number( fp ) );
	    KEY( "Light",	ch->light,		fread_number( fp ) );
	    KEY( "LongDescr",	ch->long_descr,		fread_string( fp ) );

	    if ( !str_cmp( word, "Language" ) )
	    {
	      int tmp;
	      char *wtmp;

	      wtmp = fread_word( fp );
	      tmp = language_lookup( wtmp );
              ch->language = UMAX( 0, tmp );
	      fMatch = TRUE;
	      break;
            }
	    break;

	case 'M':
	    if ( !str_cmp( word, "MVnum" ) )
	    {
		ch->pIndexData = get_mob_index( fread_number( fp ) );
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'N':
	    if ( !str_cmp( word, "Name" ) )
	    {
		/*
		 * Name already set externally for PCs.
		 */
		if ( IS_NPC( ch ) )
		{
		    ch->name = fread_string( fp );
		}
		else
		   fread_to_eol( fp );
		fMatch = TRUE;
		break;
	    }
	    KEY( "Namelist",	ch->namelist,		fread_string( fp ) );

	    break;

	case 'P':
	    KEY( "Pagelen",	ch->pcdata->pagelen,	fread_number( fp ) );
	    KEY( "Password",	ch->pcdata->pwd,	fread_string( fp ) );
	    KEY( "Played",	ch->pcdata->played,	fread_number( fp ) );
	    KEY( "Position",	ch->position,		fread_number( fp ) );
	    if ( !str_cmp( word, "Prompt" ) )
	    {
		if ( ch->prompt != NULL )
		    free_string( ch->prompt, MEM_PCDATA );
		ch->prompt = fread_string( fp );
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'R':
	    if ( !str_cmp( word, "Race" ) )
	    {
		ch->race = race_lookup( fread_word( fp ) );
		fMatch = TRUE;
		break;
	    }

	    if ( !str_cmp( word, "Reject" ) )
	    {
		ch->pcdata->rejected = fread_string( fp );
		if ( ch->pcdata->rejected[0] != '\0' )
		    SET_BIT( ch->act, PLR_REJECTED );
		fMatch = TRUE;
		break;
	    }

	    if ( !str_cmp( word, "Room" ) )
	    {
		ch->in_room = get_room_index( fread_number( fp ) );
		if ( ch->in_room == NULL )
		    ch->in_room = get_room_index( ROOM_VNUM_LIMBO );
		fMatch = TRUE;
		break;
	    }

	    break;

	case 'S':
	    KEY( "SavingThrow",	ch->saving_throw,	fread_number( fp ) );
	    KEY( "Sex",		ch->sex,		fread_number( fp ) );
	    KEY( "ShortDescr",	ch->short_descr,	fread_string( fp ) );

	    if ( !str_cmp( word, "Skill" ) )
	    {
		int sn;
		int value, value2;

		value = fread_number( fp );
		value2 = fread_number( fp );

		sn    = skill_lookup( fread_word( fp ) );
		if ( sn < 0 )
		;
/*  don't call bug while player file open!!
		    bug( "Fread_char: unknown skill.", 0 );
*/
		else
		{
		    ch->pcdata->learned[sn] = value;
		    ch->pcdata->power_level[sn] = value2;
		}
		fMatch = TRUE;
		break;
	    }

	    if ( !str_cmp( word, "SkillDelay" ) )
	    {
		int sn;
		int value;

		value = fread_number( fp );

		sn    = skill_lookup( fread_word( fp ) );

                if ( current_time < value )
		{
		  SKILL_DELAY_DATA delay;

		  delay.skill = sn;
		  delay.expire = value;
		  delay_to_char( ch, &delay );
		}

		fMatch = TRUE;
		break;
	    }

	    if ( !str_cmp( word, "Spec" ) )
	    {
		ch->spec_fun_name = fread_string( fp );
		ch->spec_fun = spec_lookup( ch->spec_fun_name );
		fMatch = TRUE;
		break;
	    }

	    if ( !str_cmp( word, "StatPhysStrength" ) ) {
	      ch->perm_stat_phys_str = fread_number( fp );
	      ch->mod_stat_phys_str = fread_number( fp );
	      fMatch = TRUE; break; }
	    if ( !str_cmp( word, "StatPhysDexterity" ) ) {
	      ch->perm_stat_phys_dex = fread_number( fp );
	      ch->mod_stat_phys_dex = fread_number( fp );
	      fMatch = TRUE; break; }
	    if ( !str_cmp( word, "StatPhysToughness" ) ) {
	      ch->perm_stat_phys_tou = fread_number( fp );
	      ch->mod_stat_phys_tou = fread_number( fp );
	      fMatch = TRUE; break; }
	    if ( !str_cmp( word, "StatPhysQuickness" ) ) {
	      ch->perm_stat_phys_qui = fread_number( fp );
	      ch->mod_stat_phys_qui = fread_number( fp );
	      fMatch = TRUE; break; }
	    if ( !str_cmp( word, "StatMentIntelligence" ) ) {
	      ch->perm_stat_ment_int = fread_number( fp );
	      ch->mod_stat_ment_int = fread_number( fp );
	      fMatch = TRUE; break; }

	    if ( !str_cmp( word, "StatOffense" ) ) {
	      ch->perm_offense = fread_number( fp );
	      ch->mod_offense = fread_number( fp );
	      fMatch = TRUE; break; }
	    if ( !str_cmp( word, "StatDefense" ) ) {
	      ch->perm_defense = fread_number( fp );
	      ch->mod_defense = fread_number( fp );
	      fMatch = TRUE; break; }

	    break;

	case 'T':
	    KEY( "Trust",	ch->trust,		fread_number( fp ) );
            KEY( "Time",    ch->time,            fread_word( fp ) );

	    if ( !str_cmp( word, "Title" ) )
	    {
		fread_string( fp );
		fMatch = TRUE;
		break;
	    }

	    break;

	case 'V':
	    if ( !str_cmp( word, "Vision" ) )
	    {
		ch->vision_min = fread_number( fp );
		ch->vision_max = fread_number( fp );
		ch->vision_current = fread_number( fp );
		ch->vision_range = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'W':
	    break;
	}

	if ( !fMatch )
	{
	    sprintf( buf, "Fread_char: no match. '%s'", word );
/*  don't call bug while player file open!!!
	    bug( buf, 0 );
*/
	    fread_to_eol( fp );
	}
    }
}



void fread_obj( CHAR_DATA *ch, ROOM_INDEX_DATA *room, FILE *fp )
{
    static OBJ_DATA obj_zero;
    OBJ_DATA *obj;
    char *word;
    int iNest;
    bool fMatch;
    bool fNest;
    bool fVnum;

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
    obj->name		= str_dup( "" );
    obj->short_descr	= str_dup( "" );
    obj->look_descr	= str_dup( "" );
    obj->description	= str_dup( "" );
    obj->spec_fun_name	= str_dup( "" );

    fNest		= FALSE;
    fVnum		= TRUE;
    iNest		= 0;

    for ( ; ; )
    {
	word   = feof( fp ) ? "End" : fread_word( fp );
	fMatch = FALSE;

	switch ( UPPER(word[0]) )
	{
	case '*':
	    fMatch = TRUE;
	    fread_to_eol( fp );
	    break;

	case 'A':
	    KEY( "Attack",	obj->attack_type,	fread_number( fp ) );
	    if ( !str_cmp( word, "Affect" ) || !str_cmp( word, "AffectData" ) )
	    {
		AFFECT_DATA *paf;

		if ( affect_free == NULL )
		{
		    paf		= alloc_perm( sizeof(*paf) );
		}
		else
		{
		    paf		= affect_free;
		    affect_free	= affect_free->next;
		}

		if ( !str_cmp( word, "Affect" ) )
		{
		    /* Obsolete 2.0 form. */
		    paf->type	= fread_number( fp );
		}
		else
		{
		    int sn;
		    char *buf;

		    buf = fread_word( fp );
		    if (!str_cmp( buf, "unknown" ))
		    {
			paf->type = -1;
		    }
		    else
		    {
		    sn = skill_lookup( fread_word( fp ) );
		    if ( sn < 0 )
			;
/*  dont call bug while player file open!!
			bug( "Fread_obj: unknown skill.", 0 );
*/
		    else
			paf->type = sn;
		    }
		}

		paf->duration	= fread_number( fp );
		paf->modifier	= fread_number( fp );
		paf->location	= fread_number( fp );
		paf->bitvector	= fread_number( fp );
		paf->next	= obj->affected;
		obj->affected	= paf;
		fMatch		= TRUE;
		break;
	    }
	    break;

	case 'C':
	    KEY( "Capacity",	obj->capacity,		fread_number( fp ) );
	    KEY( "Cost",	obj->cost,		fread_number( fp ) );
	    KEY( "Color",	obj->color,		fread_number( fp ) );
	    break;

	case 'D':
	    KEY( "Description",	obj->description,	fread_string( fp ) );

	    if ( !str_cmp( word, "Dam" ) )
	    {
		obj->dam[0]	= fread_number( fp );
		obj->dam[1]	= fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'E':
	    KEY( "ExtraFlags",	obj->extra_flags,	fread_number( fp ) );

	    if ( !str_cmp( word, "ExtraDescr" ) )
	    {
		EXTRA_DESCR_DATA *ed;

		if ( extra_descr_free == NULL )
		{
		    ed			= alloc_perm( sizeof(*ed) );
		}
		else
		{
		    ed			= extra_descr_free;
		    extra_descr_free	= extra_descr_free->next;
		}

		ed->keyword		= fread_string( fp );
		ed->description		= fread_string( fp );
		ed->next		= obj->extra_descr;
		obj->extra_descr	= ed;
		fMatch = TRUE;
	    }

	    if ( !str_cmp( word, "End" ) )
	    {
		if ( !fNest || !fVnum )
		{
/*  dont call bug while player file open!!!
		    bug( "Fread_obj: incomplete object.", 0 );
*/
		    free_string( obj->name, MEM_OBJ_NAME );
		    free_string( obj->description, MEM_OBJ_DESCRIPTION );
		    free_string( obj->look_descr, MEM_OBJ_LOOK_DESCR );
		    free_string( obj->short_descr, MEM_OBJ_SHORT_DESCR );
		    obj->next = obj_free;
		    obj_free  = obj;
		    return;
		}
		else
		{
		    obj->next	= object_list;
		    object_list	= obj;
		    if ( iNest != 0 && rgObjNest[iNest] != NULL )
			obj_to_obj( obj, rgObjNest[iNest-1] );
		    else if ( ch == NULL )
			obj_to_room( obj, room, FALSE );
		    else
			obj_to_char( obj, ch );
		    return;
		}
	    }
	    break;

	case 'I':
	    KEY( "ItemType",	obj->item_type,		fread_number( fp ) );
	    break;

	case 'L':
	    KEY( "LookDescr",	obj->look_descr,	fread_string( fp ) );
	    break;

	case 'M':
	    if ( !str_cmp( word, "Material" ) )
	    {
		obj->material = material_lookup( fread_word( fp ) );
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'N':
	    KEY( "Name",	obj->name,		fread_string( fp ) );

	    if ( !str_cmp( word, "Nest" ) )
	    {
		iNest = fread_number( fp );
		if ( iNest < 0 || iNest >= MAX_NEST )
		{
			;
/* dont call bug while player file open!!
		    bug( "Fread_obj: bad nest %d.", iNest );
*/
		}
		else
		{
		    rgObjNest[iNest] = obj;
		    fNest = TRUE;
		}
		fMatch = TRUE;
	    }
	    break;

	case 'O':
	    if ( !str_cmp( word, "OVnum" ) )
	    {
		int vnum;

		vnum = fread_number( fp );
/* re-writing this to allow players to keep equipment that has been deleted */
		if ( ( obj->pIndexData = get_obj_index( vnum ) ) == NULL )
		{
/* unformed object */
		    obj->pIndexData = get_obj_index( 99 );
		}
		fVnum = TRUE;
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'S':
	    KEY( "ShortDescr",	obj->short_descr,	fread_string( fp ) );
	    KEY( "Size",	obj->size,		fread_number( fp ) );

	    if ( !str_cmp( word, "Spec" ) )
	    {
		obj->spec_fun_name = fread_string( fp );
		obj->spec_fun = obj_fun_lookup( obj->spec_fun_name );
		fMatch = TRUE;
		break;
	    }

	    if ( !str_cmp( word, "Spell" ) )
	    {
		int iValue;
		int sn;

		iValue = fread_number( fp );
		sn     = skill_lookup( fread_word( fp ) );
		if ( iValue < 0 || iValue > 3 )
		{
			;
/* dont call bug while player file open!!!
		    bug( "Fread_obj: bad iValue %d.", iValue );
*/
		}
		else if ( sn < 0 )
		{
			;
/* dont call bug while player file open!!!
		    bug( "Fread_obj: unknown skill.", 0 );
*/
		}
		else
		{
		    obj->value[iValue] = sn;
		}
		fMatch = TRUE;
		break;
	    }

	    break;

	case 'T':
	    KEY( "Timer",	obj->timer,		fread_number( fp ) );
	    KEY( "Toughness",	obj->toughness,		fread_number( fp ) );
	    break;

	case 'V':
	    if ( !str_cmp( word, "Values" ) )
	    {
		obj->value[0]	= fread_number( fp );
		obj->value[1]	= fread_number( fp );
		obj->value[2]	= fread_number( fp );
		obj->value[3]	= fread_number( fp );
		fMatch		= TRUE;
		break;
	    }
	    break;

	case 'W':
	    if ( !str_cmp( word, "Worn" ) )
	    {
		obj->wear_loc = wear_loc_lookup( fread_word( fp ) );
		if ( obj->wear_loc != gn_wear_none )
		{
		    obj->next_worn = ch->wearing[obj->wear_loc];
		    ch->wearing[obj->wear_loc] = obj;
		}
		fMatch = TRUE;
		break;
	    }
	    KEY( "WearFlags",	obj->wear_flags,	fread_number( fp ) );
	    KEY( "Weight",	obj->weight,		fread_number( fp ) );
	    break;

	}

	if ( !fMatch )
	{
/* dont call bug while player file open!!!
	    bug( "Fread_obj: no match.", 0 );
*/
	    fread_to_eol( fp );
	}
    }
}

void reverse_items( CHAR_DATA *ch )
{
    OBJ_DATA *obj;
    OBJ_DATA *o_next = NULL;
    OBJ_DATA *new_list;
    int i;

    new_list = NULL;

    /* first we'll swap inventory. */
    if ( ( obj = ch->carrying ) == NULL
    || ( o_next = obj->next_content ) == NULL )
	return;

    for ( obj = ch->carrying; o_next != NULL; obj = o_next )
    {
	o_next = obj->next_content; 
	obj->next_content = new_list;
	new_list = obj;
    }
    ch->carrying = new_list;

    /* swap _all_ locations of equipment.  blah. */
    for ( i = 0; i < MAX_WEAR; i++ )
    {
	new_list = NULL;

	if ( ( obj = ch->wearing[i] ) == NULL
	|| ( o_next = obj->next_worn ) == NULL )
	    continue;

	for ( obj = ch->wearing[i]; o_next != NULL; obj = o_next )
	{
	    o_next = obj->next_worn;
	    obj->next_worn = new_list;
	    new_list = obj;
	}
	ch->wearing[i] = new_list;
    }
}



void save_room( ROOM_INDEX_DATA *room )
{
    FILE *fp;
    CHAR_DATA *mob;
    OBJ_DATA *obj;
    char buf[MAX_STRING_LENGTH];
    char strsave[MAX_STRING_LENGTH];
    char strtmp[MAX_STRING_LENGTH];

    for ( mob = room->people; mob != NULL; mob = mob->next_in_room )
    {
	if ( IS_NPC( mob ) && IS_SET( mob->act, ACT_SENTINEL ) )
	    break;
    }

    sprintf( strtmp, "%s%d.tmp", ROOM_SAVE_DIR, room->vnum );
    sprintf( strsave, "%s%d.rsv", ROOM_SAVE_DIR, room->vnum );

    if ( mob == NULL && room->contents == NULL )
    {
	sprintf( buf, "rm -f %s", strsave );
	system( buf );
	return;
    }

    fp = fopen( strtmp, "w" );

    if ( fp == NULL )
    {
	bug( "Error opening room save file.", 0 );
	return;
    }

    for ( obj = room->contents; obj != NULL; obj = obj->next_content )
    {
	if ( obj->item_type == ITEM_BUILDING_OBJECT )
	    continue;
	fwrite_obj( NULL, obj, fp, 0, FALSE );
    }

    for ( ; mob != NULL; mob = mob->next_in_room )
    {
	if ( !IS_NPC( mob ) || !IS_SET( mob->act, ACT_SENTINEL ) )
	    continue;

	fwrite_char( mob, fp );

	if ( mob->carrying != NULL )
	{
	    int i;

	    for ( obj = mob->carrying; obj != NULL; obj = obj->next_content )
	    {
		if ( obj->wear_loc == gn_wear_none )
		    fwrite_obj( mob, obj, fp, 0, FALSE );
	    }
	    for ( i = 0; i < MAX_WEAR; i++ )
	    {
		for ( obj = mob->wearing[i]; obj != NULL; obj = obj->next_worn )
		    fwrite_obj( mob, obj, fp, 0, FALSE );
	    }
	}
    }

    fprintf( fp, "#END\n" );
    fclose( fp );

    sprintf( buf, "cp %s %s", strtmp, strsave );
    system( buf );

    return;
}


void read_room( ROOM_INDEX_DATA *room )
{
    FILE *fp;
    int iNest;
    CHAR_DATA *ch = NULL;
    char strsave[MAX_STRING_LENGTH];
    OBJ_DATA *obj, *o_next, *new_olist;
    CHAR_DATA *m_next, *new_mlist;

    sprintf( strsave, "%s%d.rsv", ROOM_SAVE_DIR, room->vnum );

    fp = fopen( strsave, "r" );
    if ( fp == NULL )
	return;

    for ( iNest = 0; iNest < MAX_NEST; iNest++ )
        rgObjNest[iNest] = NULL;

    for ( ; ; )
    {
        char letter;
        char *word;

        letter = fread_letter( fp );
        if ( letter == '*' )
        {
            fread_to_eol( fp );
            continue;
        }

        if ( letter != '#' )
        {
            bug( "read_room: # not found.", 0 );
            break;
        }

	word = fread_word( fp );
	if      ( !str_cmp( word, "MOB" ) )
	{
	    ch = new_char( );
	    init_char( ch, NULL, FALSE );
	    SET_BIT( ch->act, ACT_IS_NPC );
	    fread_char ( ch, fp );
	    ch->next        = char_list;
	    char_list       = ch;
	    char_to_room( ch, room, FALSE );
	}
	else if ( !str_cmp( word, "OBJECT" ) )
	{
	    fread_obj  ( ch, room, fp );
	}
	else if ( !str_cmp( word, "END"    ) )
	    break;
	else
	{
	    bug( "read room: bad section.", 0 );
	    break;
	}
    }

    fclose( fp );

    /* reverse list of objects */
    new_olist = NULL;
    for ( obj = room->contents; obj != NULL; obj = o_next )
    {
	o_next = obj->next_content;
	obj->next_content = new_olist;
	new_olist = obj;
    }
    room->contents = new_olist;

    /* reverse list of mobiles */
    new_mlist = NULL;
    for ( ch = room->people; ch != NULL; ch = m_next )
    {
	m_next = ch->next_in_room;
	ch->next_in_room = new_mlist;
	new_mlist = ch;
    }
    room->people = new_mlist;
}
