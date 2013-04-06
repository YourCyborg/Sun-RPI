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
#include <string.h>
#include <time.h>
#include "merc.h"



AFFECT_DATA *		affect_free;
SKILL_DELAY_DATA *	skill_delay_free;



/*
 * Local functions.
 */
void	affect_modify	args( ( CHAR_DATA *ch, AFFECT_DATA *paf, bool fAdd ) );



/*
 * Retrieve a character's trusted level for permission checking.
 */
int get_trust( CHAR_DATA *ch )
{
    if ( ch->desc != NULL && ch->desc->original != NULL )
	ch = ch->desc->original;

    if ( ch->trust != 0 )
	return ch->trust;

    if ( IS_NPC(ch) && ch->level >= LEVEL_IMMORTAL )
	return LEVEL_IMMORTAL - 1;
    else
	return ch->level;
}



/*
 * Retrieve a character's age.
 */
int get_age( CHAR_DATA *ch )
{
    long year;

    if ( IS_NPC( ch ) )
	return 18;

    year = (long) current_time - (long) ch->pcdata->birth;
    year /= (PULSE_TICK / PULSE_PER_SECOND);
    year /= 24;
    year /= 35;
    year /= 17;
    return 17 + year;
}


int get_hours_played( CHAR_DATA *ch )
{
    if ( IS_NPC( ch ) )
	return 0;

    return ( ch->pcdata->played + (int) (current_time - ch->pcdata->logon) ) / 3600;
}



/*
 * Retrieve character's current strength.
 */
int get_curr_str( CHAR_DATA *ch )
{
    return URANGE( 3, ch->perm_stat_phys_str + ch->mod_stat_phys_str, 200 );
}



/*
 * Retrieve character's current intelligence.
 */
int get_curr_int( CHAR_DATA *ch )
{
    return URANGE( 3, ch->perm_stat_ment_int + ch->mod_stat_ment_int, 200 );
}



/*
 * Retrieve character's current dexterity.
 */
int get_curr_dex( CHAR_DATA *ch )
{
    return URANGE( 3, ch->perm_stat_phys_dex + ch->mod_stat_phys_dex, 200 );
}



/*
 * Retrieve character's current quickness.
 */
int get_curr_qui( CHAR_DATA *ch )
{
    return URANGE( 3, ch->perm_stat_phys_qui + ch->mod_stat_phys_qui, 200 );
}



/*
 * Retrieve character's current toughness.
 */
int get_curr_tou( CHAR_DATA *ch )
{
    return URANGE( 3, ch->perm_stat_phys_tou + ch->mod_stat_phys_tou, 200 );
}


/*
 * Get a characters skill
 */
int get_skill( CHAR_DATA *ch, int sn )
{
    if ( !IS_NPC( ch ) )
	return ch->pcdata->learned[sn];

    if ( ch->pIndexData == NULL )
	return 0;

    return ch->pIndexData->learned[sn];
}

/*
 * Get a character's power level
 */
int get_power_level( CHAR_DATA *ch, int sn )
{
    if ( !IS_NPC( ch ) )
	return ch->pcdata->power_level[sn];

    if ( ch->pIndexData == NULL )
	return 0;

    return ch->pIndexData->power_level[sn];
}


/*
 * Retrieve a character's carry capacity.
 */
int can_carry_n( CHAR_DATA *ch )
{
    if ( !IS_NPC(ch) && ch->level >= LEVEL_IMMORTAL )
	return 1000;

    if ( get_curr_dex(ch) < 10 )
	return get_curr_dex(ch) / 2 + MAX_WEAR + 2;
    else if (get_curr_dex(ch) < 90 )
	return get_curr_dex(ch) / 10 + 5 + MAX_WEAR + 2;
    else
	return get_curr_dex(ch) / 2 - 31 + MAX_WEAR + 2;
}



/*
 * Retrieve a character's carry capacity.
 */
int can_carry_w( CHAR_DATA *ch )
{
    if ( !IS_NPC(ch) && ch->level >= LEVEL_IMMORTAL )
	return 1000000;

    if ( get_curr_str(ch) < 10 )
	return get_curr_str(ch)*13*10;
    else if ( get_curr_str(ch) < 90 )
	return get_curr_str(ch)*3*10 + 1000;
    else
	return get_curr_str(ch)*13*10 - 8000;
}



/*
 * See if a string is one of the names of an object.
 */
bool is_name( const char *str, char *namelist )
{
    char name[MAX_INPUT_LENGTH];

    for ( ; ; )
    {
	namelist = one_argument( namelist, name );
	if ( name[0] == '\0' )
	    return FALSE;
	if ( !str_cmp( str, name ) )
	    return TRUE;
    }
}


/*
 * Give a delay to a char.
 */
void delay_to_char( CHAR_DATA *ch, SKILL_DELAY_DATA *delay )
{
    SKILL_DELAY_DATA *delay_new;

    if ( skill_delay_free == NULL )
    {
	delay_new	= alloc_perm( sizeof(*delay_new) );
    }
    else
    {
	delay_new		= skill_delay_free;
	skill_delay_free	= skill_delay_free->next;
    }

    *delay_new		= *delay;
    delay_new->next	= ch->pcdata->skill_delays;
    ch->pcdata->skill_delays	= delay_new;

    return;
}

/*
 * Remove a delay from a char.
 */
void delay_remove( CHAR_DATA *ch, SKILL_DELAY_DATA *delay )
{
    if ( ch->pcdata->skill_delays == NULL )
    {
	bug( "Delay_remove: no delay.", 0 );
	return;
    }

    if ( delay == ch->pcdata->skill_delays )
    {
	ch->pcdata->skill_delays	= delay->next;
    }
    else
    {
	SKILL_DELAY_DATA *prev;

	for ( prev = ch->pcdata->skill_delays; prev != NULL; prev = prev->next )
	{
	    if ( prev->next == delay )
	    {
		prev->next = delay->next;
		break;
	    }
	}

	if ( prev == NULL )
	{
	    bug( "Delay_remove: cannot find delay.", 0 );
	    return;
	}
    }

    delay->next	= skill_delay_free;
    skill_delay_free	= delay;
    return;
}

/*
 * Return true if a char is delayed by a skill.
 */
bool is_delayed( CHAR_DATA *ch, int sn )
{
  SKILL_DELAY_DATA *delay;

  for ( delay = ch->pcdata->skill_delays; delay != NULL; delay = delay->next )
  {
    if ( delay->skill == sn )
    {
      if ( current_time >= delay->expire )
      {
        delay_remove( ch, delay );
	return FALSE;
      }
      else
        return TRUE;
    }
  }

  return FALSE;
}


/*
 * Apply or remove an affect to a character.
 */
void affect_modify( CHAR_DATA *ch, AFFECT_DATA *paf, bool fAdd )
{
    OBJ_DATA *wield;
    int mod;
    extern int making_corpse;

    mod = paf->modifier;

    if ( fAdd )
    {
	SET_BIT( ch->affected_by, paf->bitvector );
    }
    else
    {
	REMOVE_BIT( ch->affected_by, paf->bitvector );
	mod = 0 - mod;
    }

/* note, NPC check removed, don't add any affects to pcdata */

    switch ( paf->location )
    {
    default:
	bug( "Affect_modify: unknown location %d.", paf->location );
	return;

    case APPLY_NONE:						break;
    case APPLY_STAT_PHYS_STR: ch->mod_stat_phys_str	+= mod;	break;
    case APPLY_STAT_PHYS_DEX: ch->mod_stat_phys_dex	+= mod;	break;
    case APPLY_STAT_PHYS_TOU: ch->mod_stat_phys_tou	+= mod;	break;
    case APPLY_STAT_PHYS_QUI: ch->mod_stat_phys_qui	+= mod;	break;
    case APPLY_STAT_MENT_INT: ch->mod_stat_ment_int	+= mod;	break;
    case APPLY_SEX:           ch->sex			+= mod;	break;
    case APPLY_CLASS:						break;
    case APPLY_LEVEL:						break;
    case APPLY_AGE:						break;
    case APPLY_HEIGHT:						break;
    case APPLY_WEIGHT:						break;
    case APPLY_MANA:          ch->max_mana		+= mod;	break;
    case APPLY_HIT:           ch->max_hit		+= mod;	break;
    case APPLY_MOVE:          ch->max_move		+= mod;	break;
    case APPLY_STUN:          ch->max_stun              += mod; break;
    case APPLY_GOLD:						break;
    case APPLY_EXP:						break;
    case APPLY_HITROLL:       ch->hitroll		+= mod;	break;
    case APPLY_DAMROLL:       ch->damroll		+= mod;	break;
    case APPLY_SAVING_PARA:   ch->saving_throw		+= mod;	break;
    case APPLY_SAVING_ROD:    ch->saving_throw		+= mod;	break;
    case APPLY_SAVING_PETRI:  ch->saving_throw		+= mod;	break;
    case APPLY_SAVING_BREATH: ch->saving_throw		+= mod;	break;
    case APPLY_SAVING_SPELL:  ch->saving_throw		+= mod;	break;
    case APPLY_ABSORBTION:    ch->general_absorb	+= mod; break;
    case APPLY_CHANNELING:    ch->channeling		+= mod; break;
    case APPLY_VISION_PROFILE:  ch->vision_min += mod;
				ch->vision_max += mod;		break;
    case APPLY_VISION_RANGE:	ch->vision_range += mod;	break;
    }

    /*
     * Check for weapon wielding.
     * Guard against recursion (for weapons with affects).
     *
     * Don't do this is making a corpse.
     */
  if ( !making_corpse )
  {
    if ( ( wield = get_eq_char( ch, gn_wear_primary ) ) != NULL
    &&   get_obj_weight(wield) > can_carry_w(ch)/15 )
    {
	static int depth;

	if ( depth == 0 )
	{
	    depth++;
	    act( "You drop $p.", ch, NULL, wield, NULL, NULL, TO_CHAR, SENSE_SIXTH );
	    act( "$n drops $p.", ch, NULL, wield, NULL, NULL, TO_ROOM, SENSE_SIGHT );
	    obj_from_char( wield );
	    obj_to_room( wield, ch->in_room, TRUE );
	    depth--;
	}
    }
  }

    return;
}



/*
 * Give an affect to a char.
 */
void affect_to_char( CHAR_DATA *ch, AFFECT_DATA *paf )
{
    AFFECT_DATA *paf_new;

    if ( affect_free == NULL )
    {
	paf_new		= alloc_perm( sizeof(*paf_new) );
    }
    else
    {
	paf_new		= affect_free;
	affect_free	= affect_free->next;
    }

    *paf_new		= *paf;
    paf_new->next	= ch->affected;
    ch->affected	= paf_new;

    affect_modify( ch, paf_new, TRUE );
    return;
}



/*
 * Remove an affect from a char.
 */
void affect_remove( CHAR_DATA *ch, AFFECT_DATA *paf )
{
    if ( ch->affected == NULL )
    {
	bug( "Affect_remove: no affect.", 0 );
	return;
    }

    affect_modify( ch, paf, FALSE );

    if ( paf == ch->affected )
    {
	ch->affected	= paf->next;
    }
    else
    {
	AFFECT_DATA *prev;

	for ( prev = ch->affected; prev != NULL; prev = prev->next )
	{
	    if ( prev->next == paf )
	    {
		prev->next = paf->next;
		break;
	    }
	}

	if ( prev == NULL )
	{
	    bug( "Affect_remove: cannot find paf.", 0 );
	    return;
	}
    }

    paf->next	= affect_free;
    affect_free	= paf;
    return;
}



/*
 * Strip all affects of a given sn.
 */
void affect_strip( CHAR_DATA *ch, int sn )
{
    AFFECT_DATA *paf;
    AFFECT_DATA *paf_next;

    for ( paf = ch->affected; paf != NULL; paf = paf_next )
    {
	paf_next = paf->next;
	if ( paf->type == sn )
	    affect_remove( ch, paf );
    }

    return;
}



/*
 * Return true if a char is affected by a spell.
 */
bool is_affected( CHAR_DATA *ch, int sn )
{
    AFFECT_DATA *paf;

    for ( paf = ch->affected; paf != NULL; paf = paf->next )
    {
	if ( paf->type == sn )
	    return TRUE;
    }

    return FALSE;
}



/*
 * Add or enhance an affect.
 */
void affect_join( CHAR_DATA *ch, AFFECT_DATA *paf )
{
    AFFECT_DATA *paf_old;
    bool found;

    found = FALSE;
    for ( paf_old = ch->affected; paf_old != NULL; paf_old = paf_old->next )
    {
	if ( paf_old->type == paf->type )
	{
	    paf->duration += paf_old->duration;
	    paf->modifier += paf_old->modifier;
	    affect_remove( ch, paf_old );
	    break;
	}
    }

    affect_to_char( ch, paf );
    return;
}



/*
 * Move a char out of a room.
 */
void char_from_room( CHAR_DATA *ch )
{
    ROOM_INDEX_DATA *room;

    if ( (room = ch->in_room) == NULL )
    {
	bug( "Char_from_room: NULL.", 0 );
	return;
    }

    if ( !IS_NPC(ch) )
	--ch->in_room->area->nplayer;

    ch->in_room->light_mod -= ch->light;

    if ( ch == ch->in_room->people )
    {
	ch->in_room->people = ch->next_in_room;
    }
    else
    {
	CHAR_DATA *prev;

	for ( prev = ch->in_room->people; prev; prev = prev->next_in_room )
	{
	    if ( prev->next_in_room == ch )
	    {
		prev->next_in_room = ch->next_in_room;
		break;
	    }
	}

	if ( prev == NULL )
	    bug( "Char_from_room: ch not found.", 0 );
    }

    ch->in_room      = NULL;
    ch->next_in_room = NULL;

    if ( IS_NPC( ch ) && IS_SET( room->room_flags, ROOM_SAVES_CONTENTS ) )
	save_room( room );

    return;
}



/*
 * Move a char into a room.
 */
void char_to_room( CHAR_DATA *ch, ROOM_INDEX_DATA *pRoomIndex, bool fSave )
{
    if ( pRoomIndex == NULL )
    {
	bug( "Char_to_room: NULL.", 0 );
	return;
    }

    ch->in_room		= pRoomIndex;
    ch->next_in_room	= pRoomIndex->people;
    pRoomIndex->people	= ch;

    if ( !IS_NPC(ch) )
	++ch->in_room->area->nplayer;

    ch->in_room->light_mod += ch->light;

    if ( IS_NPC( ch ) && fSave && IS_SET( pRoomIndex->room_flags, ROOM_SAVES_CONTENTS ) )
	save_room( pRoomIndex );

    return;
}



/*
 * Give an obj to a char.
 */
void obj_to_char( OBJ_DATA *obj, CHAR_DATA *ch )
{
    obj->next_content	 = ch->carrying;
    ch->carrying	 = obj;
    obj->carried_by	 = ch;
    obj->in_room	 = NULL;
    obj->in_obj		 = NULL;
    ch->carry_number	+= 1;
    ch->carry_weight	+= get_obj_weight( obj );
}



/*
 * Take an obj from its character.
 */
void obj_from_char( OBJ_DATA *obj )
{
    CHAR_DATA *ch;

    if ( ( ch = obj->carried_by ) == NULL )
    {
	bug( "Obj_from_char: null ch.", 0 );
	return;
    }

    if ( obj->wear_loc != gn_wear_none )
	unequip_char( ch, obj );

    if ( ch->carrying == obj )
    {
	ch->carrying = obj->next_content;
    }
    else
    {
	OBJ_DATA *prev;

	for ( prev = ch->carrying; prev != NULL; prev = prev->next_content )
	{
	    if ( prev->next_content == obj )
	    {
		prev->next_content = obj->next_content;
		break;
	    }
	}

	if ( prev == NULL )
	    bug( "Obj_from_char: obj not in list.", 0 );
    }

    obj->carried_by	 = NULL;
    obj->next_content	 = NULL;
    ch->carry_number	-= 1;
    ch->carry_weight	-= get_obj_weight( obj );
    return;
}




/*
 * Find a piece of eq on a character.
 */
OBJ_DATA *get_eq_char( CHAR_DATA *ch, int iWear )
{
    return ch->wearing[iWear];
}


OBJ_DATA *get_eq_char_hit_loc( CHAR_DATA *ch, int hit_loc )
{
    int iWear;

    iWear = race_array[ch->race]->hit_loc_location[hit_loc];

    if ( iWear == gn_wear_none )
	return NULL;

    return get_eq_char( ch, iWear );
}


//
// Modifies the character's light by a new object.
void modify_light( CHAR_DATA *ch, OBJ_DATA *obj, bool fEquip )
{
  int light_mod = 0;

  if ( ch == NULL || obj == NULL )
    return;

  if ( obj->item_type == ITEM_LIGHT )
    light_mod += obj->value[0];

  if ( IS_SET( obj->extra_flags, ITEM_GLOW ) )
    light_mod += GLOW_LIGHT;

  if ( IS_SET( obj->extra_flags, ITEM_DARK ) )
    light_mod += DARK_LIGHT;

  if ( !fEquip )
    light_mod *= -1;

  ch->light += light_mod;
  if ( ch->in_room )
    ch->in_room->light_mod += light_mod;

  return;
}


/*
 * Equip a char with an obj.
 */
void equip_char( CHAR_DATA *ch, OBJ_DATA *obj, int iWear )
{
    AFFECT_DATA *paf;

/*
    if ( get_eq_char( ch, iWear ) != NULL )
    {
	bug( "Equip_char: already equipped (%d).", iWear );
	return;
    }
*/

    if ( ( IS_OBJ_STAT(obj, ITEM_ANTI_EVIL)    && IS_EVIL(ch)    )
    ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_GOOD)    && IS_GOOD(ch)    )
    ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_NEUTRAL) && IS_NEUTRAL(ch) ) )
    {
	act( "You are zapped by $p and drop it.", ch, NULL, obj, NULL, NULL, TO_CHAR, SENSE_SIXTH );
	act( "$n is zapped by $p and drops it.",  ch, NULL, obj, NULL, NULL, TO_ROOM, SENSE_SIGHT );
	obj_from_char( obj );
	obj_to_room( obj, ch->in_room, TRUE );
	return;
    }

    obj->wear_loc	 = iWear;
    if ( ch->wearing[iWear] == NULL )
    {
	ch->wearing[iWear] = obj;
    }
    else if ( ch->wearing[iWear]->size > 50 )
    {
	obj->next_worn = ch->wearing[iWear]->next_worn;
	ch->wearing[iWear]->next_worn = obj;
    }
    else
    {
	obj->next_worn = ch->wearing[iWear];
	ch->wearing[iWear] = obj;
    }

    for ( paf = obj->affected; paf != NULL; paf = paf->next )
	affect_modify( ch, paf, TRUE );

  modify_light( ch, obj, TRUE );

    return;
}



/*
 * Unequip a char with an obj.
 */
void unequip_char( CHAR_DATA *ch, OBJ_DATA *obj )
{
    AFFECT_DATA *paf;
    int wear_loc = obj->wear_loc;

    if ( obj->wear_loc == gn_wear_none )
    {
	bug( "Unequip_char: already unequipped.", 0 );
	return;
    }

    if ( ch->wearing[wear_loc] == NULL )
    {
	/* bug */
    }
    else if ( ch->wearing[wear_loc] == obj )
    {
	ch->wearing[wear_loc] = obj->next_worn;
    }
    else
    {
	OBJ_DATA *o;
	for ( o = ch->wearing[wear_loc]; o != NULL; o = o->next_worn )
	    if ( o->next_worn == obj )
		break;

	if ( o != NULL )  /* bug if null */
	{
	    o->next_worn = obj->next_worn;
	}
    }
    obj->next_worn = NULL;
    obj->wear_loc	 = gn_wear_none;

    if ( obj != ch->carrying )
    {
	OBJ_DATA *prev;

	for ( prev = ch->carrying; prev != NULL; prev = prev->next_content )
	{
	    if ( prev->next_content == obj )
	    {
		prev->next_content = obj->next_content;
		break;
	    }
	}

	if ( prev == NULL )
	    bug( "unequip_char: obj not in list.", 0 );

	prev->next_content = obj->next_content;
	obj->next_content = ch->carrying;
	ch->carrying = obj;
    }

    for ( paf = obj->affected; paf != NULL; paf = paf->next )
	affect_modify( ch, paf, FALSE );

  modify_light( ch, obj, FALSE );

    return;
}


void remove_dependent_items( CHAR_DATA *ch, int wear_loc )
{
    int i;
    OBJ_DATA *od;
    extern struct wear_data const wear_info[];

    if ( ch->wearing[wear_loc] != NULL
    || ch->in_room == NULL )
	return;

    for ( i = 0; i < MAX_WEAR; i++ )
    {
	if ( wear_info[i].dependency != NULL
	&& *wear_info[i].dependency == wear_loc )
	{
	    while( (od = ch->wearing[i]) != NULL )
	    {
		act("$p falls to the ground.", ch, NULL, od, NULL, NULL,
		    TO_CHAR, SENSE_SIGHT|SENSE_TOUCH );
		act("$n's $p falls to the ground.", ch, NULL, od, NULL,
		    NULL, TO_ROOM, SENSE_SIGHT );
		obj_from_char( od );
		obj_to_room( od, ch->in_room, TRUE );
		remove_dependent_items( ch, i );
	    }
	}
    }
}

/*
 * Count occurrences of an obj in a list.
 */
int count_obj_list( OBJ_INDEX_DATA *pObjIndex, OBJ_DATA *list )
{
    OBJ_DATA *obj;
    int nMatch;

    nMatch = 0;
    for ( obj = list; obj != NULL; obj = obj->next_content )
    {
	if ( obj->pIndexData == pObjIndex )
	    nMatch++;
    }

    return nMatch;
}



/*
 * Move an obj out of a room.
 */
void obj_from_room( OBJ_DATA *obj )
{
    ROOM_INDEX_DATA *in_room;

    if ( ( in_room = obj->in_room ) == NULL )
    {
	bug( "obj_from_room: NULL.", 0 );
	return;
    }

    if ( obj->pReset != NULL )
	obj->pReset->target = NULL;

    if ( obj == in_room->contents )
    {
	in_room->contents = obj->next_content;
    }
    else
    {
	OBJ_DATA *prev;

	for ( prev = in_room->contents; prev; prev = prev->next_content )
	{
	    if ( prev->next_content == obj )
	    {
		prev->next_content = obj->next_content;
		break;
	    }
	}

	if ( prev == NULL )
	{
	    bug( "Obj_from_room: obj not found.", 0 );
	    return;
	}
    }

    obj->in_room      = NULL;
    obj->next_content = NULL;

    if ( IS_SET( in_room->room_flags, ROOM_SAVES_CONTENTS ) )
	save_room( in_room );

    return;
}



/*
 * Move an obj into a room.
 */
void obj_to_room( OBJ_DATA *obj, ROOM_INDEX_DATA *pRoomIndex, bool fSave )
{
  if(!pRoomIndex) {
    bug("obj_to_room(): NULL input pRoomIndex", 0);
  } else {
    obj->next_content		= pRoomIndex->contents;
    pRoomIndex->contents	= obj;
  }
    obj->in_room		= pRoomIndex;
    obj->carried_by		= NULL;
    obj->in_obj			= NULL;

    if ( fSave && IS_SET( pRoomIndex->room_flags, ROOM_SAVES_CONTENTS ) )
      save_room( pRoomIndex );

    return;
}



/*
 * Move an object into an object.
 */
void obj_to_obj( OBJ_DATA *obj, OBJ_DATA *obj_to )
{
    obj->next_content		= obj_to->contains;
    obj_to->contains		= obj;
    obj->in_obj			= obj_to;
    obj->in_room		= NULL;
    obj->carried_by		= NULL;

    for ( ; obj_to != NULL; obj_to = obj_to->in_obj )
    {
	if ( obj_to->carried_by != NULL )
	{
	    obj_to->carried_by->carry_weight += get_obj_weight( obj );
	}
    }

    return;
}



/*
 * Move an object out of an object.
 */
void obj_from_obj( OBJ_DATA *obj )
{
    OBJ_DATA *obj_from;

    if ( ( obj_from = obj->in_obj ) == NULL )
    {
	bug( "Obj_from_obj: null obj_from.", 0 );
	return;
    }

    if ( obj->pReset != NULL )
	obj->pReset->target = NULL;

    if ( obj == obj_from->contains )
    {
	obj_from->contains = obj->next_content;
    }
    else
    {
	OBJ_DATA *prev;

	for ( prev = obj_from->contains; prev; prev = prev->next_content )
	{
	    if ( prev->next_content == obj )
	    {
		prev->next_content = obj->next_content;
		break;
	    }
	}

	if ( prev == NULL )
	{
	    bug( "Obj_from_obj: obj not found.", 0 );
	    return;
	}
    }

    obj->next_content = NULL;
    obj->in_obj       = NULL;

    for ( ; obj_from != NULL; obj_from = obj_from->in_obj )
    {
	if ( obj_from->carried_by != NULL )
	{
	    obj_from->carried_by->carry_weight -= get_obj_weight( obj );
	}
    }

    return;
}



/*
 * Extract an obj from the world.
 */
void extract_obj( OBJ_DATA *obj, bool fDest )
{
    OBJ_DATA *obj_content;
    OBJ_DATA *obj_next;
    CHAR_DATA *ch;
    PREDELAY_DATA *p;

    if ( obj->in_room != NULL )
	obj_from_room( obj );
    else if ( obj->carried_by != NULL )
	obj_from_char( obj );
    else if ( obj->in_obj != NULL )
	obj_from_obj( obj );

    if ( obj->pReset != NULL )
	obj->pReset->target = NULL;

    for ( ch = char_list; ch != NULL; ch = ch->next )
    {
	if ( (p = ch->predelay_info) != NULL )
	{
	    if ( p->obj1 == obj )
		p->obj1 = NULL;
	    if ( p->obj2 == obj )
		p->obj2 = NULL;
	}
    }

    for ( obj_content = obj->contains; obj_content; obj_content = obj_next )
    {
	obj_next = obj_content->next_content;
	extract_obj( obj->contains, fDest );
    }

    free_string( obj->name, MEM_OBJ_NAME );
    free_string( obj->short_descr, MEM_OBJ_SHORT_DESCR );
    free_string( obj->look_descr, MEM_OBJ_LOOK_DESCR );
    free_string( obj->description, MEM_OBJ_DESCRIPTION );
    free_string( obj->spec_fun_name, MEM_OBJ_SPEC_FUN_NAME );

    if ( object_list == obj )
    {
	object_list = obj->next;
    }
    else
    {
	OBJ_DATA *prev;

	for ( prev = object_list; prev != NULL; prev = prev->next )
	{
	    if ( prev->next == obj )
	    {
		prev->next = obj->next;
		break;
	    }
	}

	if ( prev == NULL )
	{
	    bug( "Extract_obj: obj %d not found.", obj->pIndexData->vnum );
	    return;
	}
    }

    {
	AFFECT_DATA *paf;
	AFFECT_DATA *paf_next;

	for ( paf = obj->affected; paf != NULL; paf = paf_next )
	{
	    paf_next    = paf->next;
	    paf->next   = affect_free;
	    affect_free = paf;
	}
    }

    {
	EXTRA_DESCR_DATA *ed;
	EXTRA_DESCR_DATA *ed_next;

	for ( ed = obj->extra_descr; ed != NULL; ed = ed_next )
	{
	    free_string( ed->description, MEM_DIRECTION_DESCRIPTION );
	    free_string( ed->keyword, MEM_DIRECTION_KEYWORD     );
	    ed_next		= ed->next;
	    extra_descr_free	= ed;
	}
    }

    if ( obj->pIndexData != NULL )
    {
	if ( fDest )
	    obj->pIndexData->number--;
    }
    obj->next	= obj_free;
    obj_free	= obj;
    return;
}



/*
 * Extract a char from the world.
 */
void extract_char( CHAR_DATA *ch, bool fPull )
{
    CHAR_DATA *wch;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    PREDELAY_DATA *p;
    bool fDead = FALSE;

    if ( ch->in_room == NULL )
    {
	bug( "Extract_char: NULL.", 0 );
	return;
    }

    if ( fPull )
	die_follower( ch );

    if ( IS_NPC( ch ) || fPull == FALSE )
      fDead = TRUE;

    stop_fighting( ch, TRUE );

    for ( obj = ch->carrying; obj != NULL; obj = obj_next )
    {
	obj_next = obj->next_content;
	extract_obj( obj, IS_NPC( ch ) );
    }
    
    char_from_room( ch );

    if ( !fPull )
    {
	char_to_room( ch, get_room_index( ROOM_VNUM_ALTAR ), FALSE );
	return;
    }

    if ( IS_NPC(ch) )
    {
	if ( ch->pReset != NULL )
	    ch->pReset->target = NULL;
    }

    if ( ch->desc != NULL && ch->desc->original != NULL )
	do_return( ch, "" );

    for ( wch = char_list; wch != NULL; wch = wch->next )
    {
	if ( wch->reply == ch )
	    wch->reply = NULL;

	remove_hunting( wch, ch, fDead );

	if ( (p = wch->predelay_info) != NULL )
	{
	    if ( p->victim1 == ch )
		p->victim1 = NULL;
	    if ( p->victim2 == ch )
		p->victim2 = NULL;
	}
    }

    /*
     * ch being pulled.  always "dead"
     */
    while ( ch->hunting != NULL )
	remove_hunting( ch, ch->hunting->victim, TRUE );

    if ( ch == char_list )
    {
       char_list = ch->next;
    }
    else
    {
	CHAR_DATA *prev;

	for ( prev = char_list; prev != NULL; prev = prev->next )
	{
	    if ( prev->next == ch )
	    {
		prev->next = ch->next;
		break;
	    }
	}

	if ( prev == NULL )
	{
	    bug( "Extract_char: char not found.", 0 );
	    return;
	}
    }

    if ( ch->desc )
        ch->desc->character = NULL;

    free_char( ch );
    return;
}



/*
 * Find a char in the room.
 */
CHAR_DATA *get_char_room( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *rch;
    int number;
    int count;

    number = number_argument( argument, arg );
    count  = 0;
    if ( !str_cmp( arg, "self" ) || !str_cmp( arg, "me" ) )
	return ch;
    for ( rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room )
    {
	if ( !can_see( ch, rch ) ||
	( !is_name( arg, rch->name ) && !is_name( arg, rch->namelist ) ) )
	    continue;
	if ( ++count == number )
	    return rch;
    }

    return NULL;
}




/*
 * Find a char in the world.
 */
CHAR_DATA *get_char_world( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *wch;
    int number;
    int count;

    if ( ( wch = get_char_room( ch, argument ) ) != NULL )
	return wch;

    number = number_argument( argument, arg );
    count  = 0;
    for ( wch = char_list; wch != NULL ; wch = wch->next )
    {
	if ( !can_see( ch, wch ) ||
	( !is_name( arg, wch->name ) && !is_name( arg, wch->namelist ) ) )
	    continue;
	if ( ++count == number )
	    return wch;
    }

    return NULL;
}



/*
 * Find some object with a given index data.
 * Used by area-reset 'P' command.
 */
OBJ_DATA *get_obj_type( OBJ_INDEX_DATA *pObjIndex )
{
    OBJ_DATA *obj;

    for ( obj = object_list; obj != NULL; obj = obj->next )
    {
	if ( obj->pIndexData == pObjIndex )
	    return obj;
    }

    return NULL;
}


/*
 * Find an obj in a list.
 */
OBJ_DATA *get_obj_list( CHAR_DATA *ch, char *argument, OBJ_DATA *list )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int number;
    int count;

    number = number_argument( argument, arg );
    count  = 0;
    for ( obj = list; obj != NULL; obj = obj->next_content )
    {
	if ( can_see_obj( ch, obj ) && is_name( arg, obj->name ) )
	{
	    if ( ++count == number )
		return obj;
	}
    }

    return NULL;
}



/*
 * Find an obj in player's inventory.
 */
OBJ_DATA *get_obj_carry( CHAR_DATA *ch, char *argument, CHAR_DATA *viewer )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int number;
    int count;

    number = number_argument( argument, arg );
    count  = 0;
    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
    {
	if ( obj->wear_loc == gn_wear_none
	&&   can_see_obj( viewer, obj )
	&&   is_name( arg, obj->name ) )
	{
	    if ( ++count == number )
		return obj;
	}
    }

    return NULL;
}



/*
 * Find an obj in player's equipment.
 */
OBJ_DATA *get_obj_wear( CHAR_DATA *ch, char *argument, CHAR_DATA *viewer )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int number;
    int count;

    number = number_argument( argument, arg );
    count  = 0;
    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
    {
	if ( obj->wear_loc != gn_wear_none
	&&   can_see_obj( viewer, obj )
	&&   is_name( arg, obj->name ) )
	{
	    if ( ++count == number )
		return obj;
	}
    }

    return NULL;
}



/*
 * Find an obj in the room or in inventory.
 */
OBJ_DATA *get_obj_here( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;

    obj = get_obj_list( ch, argument, ch->in_room->contents );
    if ( obj != NULL )
	return obj;

    if ( ( obj = get_obj_carry( ch, argument, ch ) ) != NULL )
	return obj;

    if ( ( obj = get_obj_wear( ch, argument, ch ) ) != NULL )
	return obj;

    return NULL;
}

CHAR_DATA *get_mob_world( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *wch;
    int number;
    int count;

    if ( ch && ( wch =  get_char_room( ch, arg ) ) != NULL   )
		return wch;

    number = number_argument( argument, arg );
    count  = 0;

    for ( wch = char_list; wch != NULL ; wch = wch->next )
    {
		if ( wch->in_room == NULL || ( ch && !can_see( ch, wch ) )
		||   !is_name( arg, wch->name ) )
		    continue;
		if ( !IS_NPC( wch ) )
			continue;
		if ( ++count == number )
		    return wch;
    }
    return NULL;
}

CHAR_DATA *get_mob_by_vnum( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *wch;
    int     atoi            args( ( const char *string ) );
    int     vnum;
    vnum = 4;

 if ( !is_number( argument ) )
	{
		bug( "get_mob_by_vnum:: non-number lookup", 0 );
		return NULL;
	}
//	vnum = get_mob_index( atoi( argument ) );
	
    for ( wch = char_list; wch != NULL ; wch = wch->next )
    {
		if ( wch->in_room == NULL || ( ch && !can_see( ch, wch ) ) )
		    continue;
		if ( !IS_NPC( wch ) )
			continue;
		if ( !wch->pIndexData )
			continue;
		if ( wch->pIndexData->vnum != vnum )
			continue;
		if ( wch->pIndexData->vnum == vnum )
		    return wch;
    }
    return NULL;
}

CHAR_DATA *get_pc_world( CHAR_DATA *ch, char*argument )
{
	DESCRIPTOR_DATA *d;
	if(!str_cmp(argument,"self"))
		return ch;
	for ( d = descriptor_list; d != NULL; d = d->next )
	{
		if ( !d || !d->character || d->connected != CON_PLAYING )
		{
			continue;
		}
		if ( is_name(argument,  d->character->name ) )
			return (d->character);
	}
	return NULL;
}

OBJ_DATA *get_obj_world( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int number;
    int count;

    if ( ( obj = get_obj_here( ch, argument ) ) != NULL )
	return obj;

    number = number_argument( argument, arg );
    count  = 0;
    for ( obj = object_list; obj != NULL; obj = obj->next )
    {
	if ( can_see_obj( ch, obj ) && is_name( arg, obj->name ) )
	{
	    if ( ++count == number )
		return obj;
	}
    }

    return NULL;
}



/*
 * Return weight of an object, including weight of contents.
 */
int get_obj_weight( OBJ_DATA *obj )
{
    int weight;

    weight = obj->weight;
    for ( obj = obj->contains; obj != NULL; obj = obj->next_content )
	weight += get_obj_weight( obj );

    return weight;
}



/*
 * True if room is dark.
 */
int room_is_dark( ROOM_INDEX_DATA *pRoomIndex, CHAR_DATA *ch )
{
    int diff;

    diff = ch->vision_current - room_light( pRoomIndex );

    if ( diff < -100 )
	return VISION_TOO_BRIGHT;

    if ( diff < -50 )
	return VISION_HAZY;

    if ( diff > 100 )
	return VISION_TOO_DARK;

    if ( diff > 50 )
	return VISION_HAZY;

    return VISION_NORMAL;
}


int room_light( ROOM_INDEX_DATA *pRoomIndex )
{
    int light;
    char *flags;
    int sign = 1;

    flags = pRoomIndex->sector->name;
    if ( pRoomIndex->light_mod < 0 ) sign = -1;

    light = pRoomIndex->light;

    if ( !str_cmp(flags, "inside" ) )
	light += (sunlight_table[time_info.hour]*pRoomIndex->light_filter/20);

    if ( str_cmp(flags, "inside" ) )
    {
	if ( ( sign > 0 && pRoomIndex->light_mod > pRoomIndex->light )
	|| ( pRoomIndex->light_mod < pRoomIndex->light ) )
	{
	    if ( 20-pRoomIndex->light_filter > 10 )
	        light += ( pRoomIndex->light_mod*(10-pRoomIndex->light_filter)/10);
	}
    }

    if ( !str_cmp(flags, "inside" ) )
    {
	light += ( pRoomIndex->light_mod*(10-pRoomIndex->light_filter)/10);
    }

    return light;
}


/*
 * True if room is private.
 */
bool room_is_private( ROOM_INDEX_DATA *pRoomIndex )
{
    CHAR_DATA *rch;
    int count;

    count = 0;
    for ( rch = pRoomIndex->people; rch != NULL; rch = rch->next_in_room )
	count++;

    if ( IS_SET(pRoomIndex->room_flags, ROOM_PRIVATE)  && count >= 2 )
	return TRUE;

    if ( IS_SET(pRoomIndex->room_flags, ROOM_SOLITARY) && count >= 1 )
	return TRUE;

    return FALSE;
}



/*
 * True if char can see victim.
 */
/* sh_int*/
bool can_see( CHAR_DATA *ch, CHAR_DATA *victim )
{
 
/*for ( victim = ch->in_room->people; victim != NULL; victim = 
victim->next_in_room )
{*/
    

    if ( ch == victim )
	return TRUE;

    if ( victim == NULL )
       return TRUE;

   if ( !IS_NPC(victim)
    &&   IS_SET(victim->act, PLR_WIZINVIS)
    &&   !IS_IMMORTAL(ch) )
	return FALSE;

    if ( (!IS_NPC(ch) && IS_SET(ch->act, PLR_HOLYLIGHT)) 
    ||   (IS_NPC(ch) && IS_IMMORTAL(ch)))
	return TRUE;

    if ( IS_AFFECTED(ch, AFF_BLIND) )
	return FALSE;

    if ( room_is_dark( ch->in_room, ch ) == VISION_TOO_DARK
    || room_is_dark( ch->in_room, ch ) == VISION_TOO_BRIGHT )
	return FALSE;

   if ( IS_AFFECTED(victim, AFF_INVISIBLE)
    &&   !IS_AFFECTED(ch, AFF_DETECT_INVIS) )
	return FALSE;

   if ( IS_AFFECTED(victim, AFF_HIDE)
    &&   !IS_AFFECTED(ch, AFF_DETECT_HIDDEN)
    &&   !check_fighting( victim )
    &&   ( IS_NPC(ch) ? !IS_NPC(victim) : IS_NPC(victim) ) )
    {
	if (number_percent() < get_skill( victim, gsn_hide ) )
	{
	    return VISION_HAZY;
	}
	else
	{
	    if (!IS_NPC( victim ) )
//    if (!IS_NPC( victim ) && !number_bits(2))
		skill_improve(victim,gsn_hide);
	}
    }

    if ( room_is_dark( ch->in_room, ch ) == VISION_HAZY )
    {
	return VISION_HAZY;
    }

    return TRUE;
  }




/*
 * True if char can see obj.
 */
bool can_see_obj( CHAR_DATA *ch, OBJ_DATA *obj )
{
    if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_HOLYLIGHT) )
	return TRUE;

    if ( IS_AFFECTED( ch, AFF_BLIND ) && obj->carried_by != ch )
	return FALSE;

    if ( obj->item_type == ITEM_LIGHT && obj->value[2] != 0 )
	return TRUE;

    if ( room_is_dark( ch->in_room, ch ) == VISION_TOO_DARK
    || room_is_dark( ch->in_room, ch ) == VISION_TOO_BRIGHT )
	return FALSE;

    if ( IS_SET(obj->extra_flags, ITEM_INVIS)
    &&   !IS_AFFECTED(ch, AFF_DETECT_INVIS) )
	return FALSE;

    if (!can_see(ch, obj->carried_by))
	return FALSE;

    return TRUE;
}



/*
 * True if char can drop obj.
 */
bool can_drop_obj( CHAR_DATA *ch, OBJ_DATA *obj )
{
    if ( !IS_SET(obj->extra_flags, ITEM_NODROP) )
	return TRUE;

    if ( !IS_NPC(ch) && ch->level >= LEVEL_IMMORTAL )
	return TRUE;

    return FALSE;
}



/*
 * Return ascii name of an item type.
 */
char *item_type_name( OBJ_DATA *obj )
{
    switch ( obj->item_type )
    {
    case ITEM_LIGHT:		return "light";
    case ITEM_SCROLL:		return "scroll";
    case ITEM_WAND:		return "wand";
    case ITEM_STAFF:		return "staff";
    case ITEM_WEAPON:		return "weapon";
    case ITEM_TREASURE:		return "treasure";
    case ITEM_ARMOR:		return "armor";
    case ITEM_POTION:		return "potion";
    case ITEM_FURNITURE:	return "furniture";
    case ITEM_TRASH:		return "trash";
    case ITEM_CONTAINER:	return "container";
    case ITEM_DRINK_CON:	return "drink container";
    case ITEM_KEY:		return "key";
    case ITEM_FOOD:		return "food";
    case ITEM_MONEY:		return "money";
    case ITEM_BOAT:		return "boat";
    case ITEM_CORPSE:		return "corpse";
    case ITEM_FOUNTAIN:		return "fountain";
    case ITEM_PILL:		return "pill";
    case ITEM_JEWELRY:		return "jewelry";
    case ITEM_LOCKPICKS:	return "lock picks";
    case ITEM_BUILDING_OBJECT:  return "building object";
    case ITEM_BUILDING_WINDOW:  return "building window";
    case ITEM_WAGON_CONTROLS:   return "wagon controls";
    case ITEM_RANGED_WEAPON:    return "ranged weapon";
    case ITEM_AMMUNITION:       return "ammunition";
    }

    bug( "Item_type_name: unknown type %d.", obj->item_type );
    return "(unknown)";
}



/*
 * Return ascii name of an affect location.
 */
char *affect_loc_name( int location )
{
    switch ( location )
    {
    case APPLY_NONE:		return "none";
    case APPLY_STAT_PHYS_STR:	return "phys-strength";
    case APPLY_STAT_PHYS_DEX:	return "phys-dexterity";
    case APPLY_STAT_PHYS_QUI:	return "phys-quickness";
    case APPLY_STAT_PHYS_TOU:	return "phys-toughness";
    case APPLY_STAT_MENT_INT:	return "ment-intelligence";
    case APPLY_SEX:		return "sex";
    case APPLY_CLASS:		return "class";
    case APPLY_LEVEL:		return "level";
    case APPLY_AGE:		return "age";
    case APPLY_MANA:		return "mana";
    case APPLY_HIT:		return "hp";
    case APPLY_MOVE:		return "moves";
    case APPLY_STUN:            return "stun";
    case APPLY_GOLD:		return "gold";
    case APPLY_EXP:		return "experience";
    case APPLY_HITROLL:		return "hit roll";
    case APPLY_DAMROLL:		return "damage roll";
    case APPLY_SAVING_PARA:	return "save vs paralysis";
    case APPLY_SAVING_ROD:	return "save vs rod";
    case APPLY_SAVING_PETRI:	return "save vs petrification";
    case APPLY_SAVING_BREATH:	return "save vs breath";
    case APPLY_SAVING_SPELL:	return "save vs spell";
    case APPLY_ABSORBTION:	return "absorbtion";
    case APPLY_CHANNELING:	return "channeling";
    case APPLY_VISION_PROFILE:	return "vision profile";
    case APPLY_VISION_RANGE:	return "vision range";
    }

    bug( "Affect_location_name: unknown location %d.", location );
    return "(unknown)";
}



/*
 * Return ascii name of an affect bit vector.
 */
char *affect_bit_name( int vector )
{
    static char buf[512];

    buf[0] = '\0';
    if ( vector & AFF_BLIND         ) strcat( buf, " blind"         );
    if ( vector & AFF_INVISIBLE     ) strcat( buf, " invisible"     );
    if ( vector & AFF_DETECT_EVIL   ) strcat( buf, " detect_evil"   );
    if ( vector & AFF_DETECT_INVIS  ) strcat( buf, " detect_invis"  );
    if ( vector & AFF_DETECT_MAGIC  ) strcat( buf, " detect_magic"  );
    if ( vector & AFF_DETECT_HIDDEN ) strcat( buf, " detect_hidden" );
    if ( vector & AFF_HOLD          ) strcat( buf, " hold"          );
    if ( vector & AFF_SANCTUARY     ) strcat( buf, " sanctuary"     );
    if ( vector & AFF_FAERIE_FIRE   ) strcat( buf, " faerie_fire"   );
    if ( vector & AFF_INFRARED      ) strcat( buf, " infrared"      );
    if ( vector & AFF_CURSE         ) strcat( buf, " curse"         );
    if ( vector & AFF_FLAMING       ) strcat( buf, " flaming"       );
    if ( vector & AFF_POISON        ) strcat( buf, " poison"        );
    if ( vector & AFF_PROTECT       ) strcat( buf, " protect"       );
    if ( vector & AFF_PARALYSIS     ) strcat( buf, " paralysis"     );
    if ( vector & AFF_SLEEP         ) strcat( buf, " sleep"         );
    if ( vector & AFF_SNEAK         ) strcat( buf, " sneak"         );
    if ( vector & AFF_HIDE          ) strcat( buf, " hide"          );
    if ( vector & AFF_CHARM         ) strcat( buf, " charm"         );
    if ( vector & AFF_FLYING        ) strcat( buf, " flying"        );
    if ( vector & AFF_PASS_DOOR     ) strcat( buf, " pass_door"     );
    return ( buf[0] != '\0' ) ? buf+1 : "none";
}



/*
 * Return ascii name of extra flags vector.
 */
char *extra_bit_name( int extra_flags )
{
    static char buf[512];

    buf[0] = '\0';
    if ( extra_flags & ITEM_GLOW         ) strcat( buf, " glow"         );
    if ( extra_flags & ITEM_HUM          ) strcat( buf, " hum"          );
    if ( extra_flags & ITEM_DARK         ) strcat( buf, " dark"         );
    if ( extra_flags & ITEM_LOCK         ) strcat( buf, " lock"         );
    if ( extra_flags & ITEM_INVIS        ) strcat( buf, " invis"        );
    if ( extra_flags & ITEM_MAGIC        ) strcat( buf, " magic"        );
    if ( extra_flags & ITEM_NODROP       ) strcat( buf, " nodrop"       );
    if ( extra_flags & ITEM_BLESS        ) strcat( buf, " bless"        );
    if ( extra_flags & ITEM_ANTI_GOOD    ) strcat( buf, " anti-good"    );
    if ( extra_flags & ITEM_ANTI_EVIL    ) strcat( buf, " anti-evil"    );
    if ( extra_flags & ITEM_ANTI_NEUTRAL ) strcat( buf, " anti-neutral" );
    if ( extra_flags & ITEM_NOREMOVE     ) strcat( buf, " noremove"     );
    if ( extra_flags & ITEM_INVENTORY    ) strcat( buf, " inventory"    );
    if ( extra_flags & ITEM_VISIBLE_DISTANCE ) strcat( buf, " visible"  );
    if ( extra_flags & ITEM_HOOD         ) strcat( buf, " hood"         );
    if ( extra_flags & ITEM_NOTAKE       ) strcat( buf, " notake"       );
    return ( buf[0] != '\0' ) ? buf+1 : "none";
}


bool is_vowel( char c )
{
    if ( c == 'a' )
	return TRUE;

    if ( c == 'e' )
	return TRUE;

    if ( c == 'i' )
	return TRUE;

    if ( c == 'o' )
	return TRUE;

    if ( c == 'u' )
	return TRUE;

    return FALSE;
}


char *person( CHAR_DATA *ch, CHAR_DATA *victim )
{
    if ( can_see( ch, victim ) == TRUE ) 
	return ((!IS_NPC(victim) && HAS_HOLYNAME(ch)) ? 
		victim->name : victim->short_descr);

    if (!HAS_HOLYLIGHT(ch))
	if ( can_see( ch, victim ) == VISION_HAZY )
	    return (race_array[victim->race]->hazy_desc);
        
    if ( !HAS_HOLYLIGHT(ch) )
        if ( IS_IMMORTAL(victim) && IS_SET(victim->act, PLR_WIZINVIS ))
            return "A staff member";

    if ( can_see( ch, victim) == FALSE && IS_AFFECTED(victim, AFF_INVISIBLE) )
        if (victim->sex == 1 )
        return "Someone";
 
    if ( can_see( ch, victim) == FALSE && IS_AFFECTED(victim, AFF_INVISIBLE) )
        if (victim->sex == 2 )
        return "Someone";

    if ( can_see( ch, victim) == TRUE && IS_AFFECTED(victim, AFF_HIDE) )
       return "Someone";
    
       
    return "Someone";
}



int get_aggr_factor( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int aggr_bits;
    int aggr = 0;

    if ( !can_see( ch, victim ) )
    {
	return 0;
    }

    if ( IS_NPC( victim ) && ch->pIndexData == victim->pIndexData )
	return 0;

    if ( !IS_NPC( victim ) && IS_IMMORTAL( victim ) )
	return 0;

    aggr_bits = ch->aggression;

    if ( IS_SET( aggr_bits, AGGRESSIVE_SMALLER ) )
    {
	if ( victim->weight > ch->weight )
	    return 0;

	aggr += ch->weight - victim->weight;
    }

    if ( IS_SET( aggr_bits, AGGRESSIVE_WOUNDED ) )
    {
	aggr += (victim->max_hit - victim->hit[HIT_LOC_BODY])*100/victim->max_hit;

	aggr -= (ch->max_hit - ch->hit[HIT_LOC_BODY])*100/ch->max_hit;
    }

    if ( IS_SET( aggr_bits, AGGRESSIVE_ALL ) )
	aggr += number_bits(4);

/*
    if ( IS_SET( aggr_bits, AGGRESSIVE_RACIAL ) )
	aggr -= race_array[ch->race]->relations[victim->race];
*/

    if ( IS_SET( aggr_bits, AGGRESSIVE_CLAN ) && 0 )
	aggr += number_bits(4);    /* compare clans */

    return aggr;
}



void set_predelay( CHAR_DATA *ch, int delay, DO_FUN *fnptr, char *argument,
    int number, CHAR_DATA *victim1, CHAR_DATA *victim2, OBJ_DATA *obj1,
    OBJ_DATA *obj2 )
{
    PREDELAY_DATA *p;

    p = new_predelay( );

    p->fnptr = fnptr;
    p->argument[0] = '\0';
    strcpy( p->argument, argument );
    p->number = number;
    p->victim1 = victim1;
    p->victim2 = victim2;
    p->obj1 = obj1;
    p->obj2 = obj2;

    if ( ch->predelay_info != NULL )
	free_predelay( ch->predelay_info );
    ch->predelay_info = p;
    ch->predelay_time = delay;
}

