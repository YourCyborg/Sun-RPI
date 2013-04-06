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
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"

/*
 * The following special functions are available for mobiles.
 */
DECLARE_SPEC_FUN(	spec_breath_any		);
DECLARE_SPEC_FUN(	spec_breath_acid	);
DECLARE_SPEC_FUN(	spec_breath_fire	);
DECLARE_SPEC_FUN(	spec_breath_frost	);
DECLARE_SPEC_FUN(	spec_breath_gas		);
DECLARE_SPEC_FUN(	spec_breath_lightning	);
DECLARE_SPEC_FUN(	spec_cast_cleric	);
DECLARE_SPEC_FUN(	spec_cast_mage		);
DECLARE_SPEC_FUN(	spec_cast_undead	);
DECLARE_SPEC_FUN(	spec_fido		);
DECLARE_SPEC_FUN(	spec_guard		);
DECLARE_SPEC_FUN(	spec_janitor		);
DECLARE_SPEC_FUN(	spec_mayor		);
DECLARE_SPEC_FUN(	spec_poison		);
DECLARE_SPEC_FUN(	spec_thief		);



/*
 * Given a name, return the appropriate spec fun.
 */
SPEC_FUN *spec_lookup( const char *name )
{
    if ( !str_cmp( name, "spec_breath_any"	  ) ) return spec_breath_any;
    if ( !str_cmp( name, "spec_breath_acid"	  ) ) return spec_breath_acid;
    if ( !str_cmp( name, "spec_breath_fire"	  ) ) return spec_breath_fire;
    if ( !str_cmp( name, "spec_breath_frost"	  ) ) return spec_breath_frost;
    if ( !str_cmp( name, "spec_breath_gas"	  ) ) return spec_breath_gas;
    if ( !str_cmp( name, "spec_breath_lightning"  ) ) return
							spec_breath_lightning;
    if ( !str_cmp( name, "spec_cast_cleric"	  ) ) return spec_cast_cleric;
    if ( !str_cmp( name, "spec_cast_mage"	  ) ) return spec_cast_mage;
    if ( !str_cmp( name, "spec_cast_undead"	  ) ) return spec_cast_undead;
    if ( !str_cmp( name, "spec_fido"		  ) ) return spec_fido;
    if ( !str_cmp( name, "spec_guard"		  ) ) return spec_guard;
    if ( !str_cmp( name, "spec_janitor"		  ) ) return spec_janitor;
    if ( !str_cmp( name, "spec_mayor"		  ) ) return spec_mayor;
    if ( !str_cmp( name, "spec_poison"		  ) ) return spec_poison;
    if ( !str_cmp( name, "spec_thief"		  ) ) return spec_thief;
    return 0;
}



/*
 * Core procedure for dragons.
 */
bool dragon( CHAR_DATA *ch, char *spell_name )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    int sn;

    if ( ch->fighting == NULL )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_bits( 2 ) == 0 )
	    break;
    }

    if ( victim == NULL )
	return FALSE;

    if ( ( sn = skill_lookup( spell_name ) ) < 0 )
	return FALSE;
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim );
    return TRUE;
}



/*
 * Special procedures for mobiles.
 */
bool spec_breath_any( CHAR_DATA *ch, char *cmd, DO_FUN *fnptr, char *arg,
   CHAR_DATA *mob )
{
    if ( ch != NULL || fnptr != NULL )
	return FALSE;

    if ( mob->fighting == NULL )
	return FALSE;

    switch ( number_bits( 3 ) )
    {
    case 0: return spec_breath_fire		( NULL, NULL, NULL, NULL, mob );
    case 1:
    case 2: return spec_breath_lightning	( NULL, NULL, NULL, NULL, mob );
    case 3: return spec_breath_gas		( NULL, NULL, NULL, NULL, mob );
    case 4: return spec_breath_acid		( NULL, NULL, NULL, NULL, mob );
    case 5:
    case 6:
    case 7: return spec_breath_frost		( NULL, NULL, NULL, NULL, mob );
    }

    return FALSE;
}



bool spec_breath_acid( CHAR_DATA *ch, char *cmd, DO_FUN *fnptr, char *arg,
    CHAR_DATA *mob )
{
    if ( ch != NULL || fnptr != NULL )
	return FALSE;

    return dragon( mob, "acid breath" );
}



bool spec_breath_fire( CHAR_DATA *ch, char *cmd, DO_FUN *fnptr, char *arg,
    CHAR_DATA *mob )
{
    if ( ch != NULL || fnptr != NULL )
	return FALSE;

    return dragon( mob, "fire breath" );
}



bool spec_breath_frost( CHAR_DATA *ch, char *cmd, DO_FUN *fnptr, char *arg,
    CHAR_DATA *mob )
{
    if ( ch != NULL || fnptr != NULL )
	return FALSE;

    return dragon( mob, "frost breath" );
}



bool spec_breath_gas( CHAR_DATA *ch, char *cmd, DO_FUN *fnptr, char *arg,
    CHAR_DATA *mob )
{
    int sn;

    if ( ch != NULL || fnptr != NULL )
	return FALSE;

    if ( mob->fighting == NULL )
	return FALSE;

    if ( ( sn = skill_lookup( "gas breath" ) ) < 0 )
	return FALSE;
    (*skill_table[sn].spell_fun) ( sn, mob->level, mob, NULL );
    return TRUE;
}



bool spec_breath_lightning( CHAR_DATA *ch, char *cmd, DO_FUN *fnptr, char *arg,
    CHAR_DATA *mob )
{
    if ( ch != NULL || fnptr != NULL )
	return FALSE;

    return dragon( mob, "lightning breath" );
}



bool spec_cast_cleric( CHAR_DATA *ch, char *cmd, DO_FUN *fnptr, char *arg,
    CHAR_DATA *mob )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    char *spell;
    int sn;

    if ( ch != NULL || fnptr != NULL )
	return FALSE;

    if ( mob->fighting == NULL )
	return FALSE;

    for ( victim = mob->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == mob && number_bits( 2 ) == 0 )
	    break;
    }

    if ( victim == NULL )
	return FALSE;

    for ( ;; )
    {
	int min_level;

	switch ( number_bits( 4 ) )
	{
	case  0: min_level =  0; spell = "blindness";      break;
	case  1: min_level =  3; spell = "harm";           break;
	case  2: min_level =  7; spell = "earthquake";     break;
	case  3: min_level =  9; spell = "harm";           break;
	case  4: min_level = 10; spell = "dispel evil";    break;
	case  5: min_level = 12; spell = "curse";          break;
	case  6:
	case  7:
	case  8:
	case  9:
	case 10: min_level = 15; spell = "harm";           break;
	default: min_level = 16; spell = "dispel magic";   break;
	}

	if ( mob->level >= min_level )
	    break;
    }

    if ( ( sn = skill_lookup( spell ) ) < 0 )
	return FALSE;
    (*skill_table[sn].spell_fun) ( sn, mob->level, mob, victim );
    return TRUE;
}



bool spec_cast_mage( CHAR_DATA *ch, char *cmd, DO_FUN *fnptr, char *arg,
    CHAR_DATA *mob )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    char *spell;
    int sn;

    if ( ch != NULL || fnptr != NULL )
	return FALSE;

    if ( mob->fighting == NULL )
	return FALSE;

    for ( victim = mob->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == mob && number_bits( 2 ) == 0 )
	    break;
    }

    if ( victim == NULL )
	return FALSE;

    for ( ;; )
    {
	int min_level;

	switch ( number_bits( 4 ) )
	{
	case  0: min_level =  0; spell = "blindness";      break;
	case  1:
	case  2: min_level =  7; spell = "weaken";         break;
	case  3: min_level =  8; spell = "teleport";       break;
	case  4: min_level = 11; spell = "lightning bolt"; break;
	case  5:
	case  6: min_level = 13; spell = "vampiric touch"; break;
	case  7:
	case  8:
	case  9: min_level = 15; spell = "fireball";       break;
	default: min_level = 15; spell = "fireball";       break;
	}

	if ( mob->level >= min_level )
	    break;
    }

    if ( ( sn = skill_lookup( spell ) ) < 0 )
	return FALSE;
    (*skill_table[sn].spell_fun) ( sn, mob->level, mob, victim );
    return TRUE;
}



bool spec_cast_undead( CHAR_DATA *ch, char *cmd, DO_FUN *fnptr, char *arg,
    CHAR_DATA *mob )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    char *spell;
    int sn;

    if ( ch != NULL || fnptr != NULL )
	return FALSE;

    if ( mob->fighting == NULL )
	return FALSE;

    for ( victim = mob->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == mob && number_bits( 2 ) == 0 )
	    break;
    }

    if ( victim == NULL )
	return FALSE;

    for ( ;; )
    {
	int min_level;

	switch ( number_bits( 4 ) )
	{
	case  0: min_level =  0; spell = "curse";          break;
	case  1: min_level =  3; spell = "weaken";         break;
	case  2:
	case  3: min_level =  9; spell = "blindness";      break;
	case  4: min_level = 12; spell = "poison";         break;
	case  5: min_level = 15; spell = "vampiric touch"; break;
	case  6: min_level = 18; spell = "harm";           break;
	case  7: min_level = 21; spell = "teleport";       break;
	default: min_level = 24; spell = "weaken";         break;
	}

	if ( mob->level >= min_level )
	    break;
    }

    if ( ( sn = skill_lookup( spell ) ) < 0 )
	return FALSE;
    (*skill_table[sn].spell_fun) ( sn, mob->level, mob, victim );
    return TRUE;
}


bool spec_fido( CHAR_DATA *ch, char *cmd, DO_FUN *fnptr, char *arg,
    CHAR_DATA *mob )
{
    OBJ_DATA *corpse;
    OBJ_DATA *c_next;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;

    if ( ch != NULL || fnptr != NULL )
	return FALSE;

    if ( !IS_AWAKE(mob) )
	return FALSE;

    for ( corpse = mob->in_room->contents; corpse != NULL; corpse = c_next )
    {
	c_next = corpse->next_content;
	if ( corpse->item_type != ITEM_CORPSE )
	    continue;

	act( "$n savagely devours a corpse.", mob, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIGHT );
	for ( obj = corpse->contains; obj; obj = obj_next )
	{
	    obj_next = obj->next_content;
	    obj_from_obj( obj );
	    obj_to_room( obj, mob->in_room, TRUE );
	}
	extract_obj( corpse, TRUE );
	return TRUE;
    }

    return FALSE;
}



bool spec_guard( CHAR_DATA *ch, char *cmd, DO_FUN *fnptr, char *arg,
    CHAR_DATA *mob )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    CHAR_DATA *ech;
	
    

    if ( ch != NULL || fnptr != NULL )
	return FALSE;

    if ( !IS_AWAKE(mob) || mob->fighting != NULL )
	return FALSE;

    
    ech      = NULL;

    for ( victim = mob->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;

	if ( victim->fighting != NULL	 )
	{
	    
	    ech      = ch;
	}
    }

    if ( ech != NULL )
    {
	do_shout( mob, "Yield Criminal!" );
	set_hunting( ech, mob, 0, 40 );
	one_hit( mob, ech, TYPE_UNDEFINED );
	return TRUE;
    }

    return FALSE;
}



bool spec_janitor( CHAR_DATA *ch, char *cmd, DO_FUN *fnptr, char *arg,
    CHAR_DATA *mob )
{
    OBJ_DATA *trash;
    OBJ_DATA *trash_next;

    if ( ch != NULL || fnptr != NULL )
	return FALSE;

    if ( !IS_AWAKE(mob) )
	return FALSE;

    for ( trash = mob->in_room->contents; trash != NULL; trash = trash_next )
    {
	trash_next = trash->next_content;
	if ( !IS_SET( trash->wear_flags, ITEM_TAKE ) )
	    continue;
	if ( trash->item_type == ITEM_DRINK_CON
	||   trash->item_type == ITEM_TRASH
	||   trash->cost < 10 )
	{
	    act( "$n picks up some trash.", mob, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIGHT );
	    obj_from_room( trash );
	    obj_to_char( trash, ch );
	    return TRUE;
	}
    }

    return FALSE;
}



bool spec_mayor( CHAR_DATA *ch, char *cmd, DO_FUN *fnptr, char *arg,
    CHAR_DATA *mob )
{
/* write mayor for Ys? */
    static const char open_path[] =
	"W3a3003b33000c111d0d111Oe333333Oe22c222112212111a1S.";

    static const char close_path[] =
	"W3a3003b33000c111d0d111CE333333CE22c222112212111a1S.";

    static const char *path;
    static int pos;
    static bool move;

    if ( ch != NULL || fnptr != NULL )
	return FALSE;

    if ( !move )
    {
	if ( time_info.hour ==  6 )
	{
	    path = open_path;
	    move = TRUE;
	    pos  = 0;
	}

	if ( time_info.hour == 20 )
	{
	    path = close_path;
	    move = TRUE;
	    pos  = 0;
	}
    }

    if ( mob->fighting != NULL )
	return spec_cast_cleric( NULL, NULL, NULL, NULL, mob );
    if ( !move || ch->position < POS_SLEEPING )
	return FALSE;

    switch ( path[pos] )
    {
    case '0':
    case '1':
    case '2':
    case '3':
	move_char( mob, path[pos] - '0' );
	break;

    case 'W':
	mob->position = POS_STANDING;
	act( "$n awakens and groans loudly.", mob, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIGHT|SENSE_HEARING );
	break;

    case 'S':
	mob->position = POS_SLEEPING;
	act( "$n lies down and falls asleep.", mob, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIGHT );
	break;

    case 'a':
	act( "$n says 'Hello Honey!'", mob, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIGHT|SENSE_HEARING );
	break;

    case 'b':
	act( "$n says 'What a view!  I must do something about that dump!'",
	    mob, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIGHT|SENSE_HEARING );
	break;

    case 'c':
	act( "$n says 'Vandals!  Youngsters have no respect for anything!'",
	    mob, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIGHT|SENSE_HEARING );
	break;

    case 'd':
	act( "$n says 'Good day, citizens!'", mob, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIGHT|SENSE_HEARING );
	break;

    case 'e':
	act( "$n says 'I hereby declare the city of Midgaard open!'",
	    mob, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIGHT|SENSE_HEARING );
	break;

    case 'E':
	act( "$n says 'I hereby declare the city of Midgaard closed!'",
	    mob, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIGHT|SENSE_HEARING );
	break;

    case 'O':
	do_unlock( mob, "gate" );
	do_open( mob, "gate" );
	break;

    case 'C':
	do_close( mob, "gate" );
	do_lock( mob, "gate" );
	break;

    case '.' :
	move = FALSE;
	break;
    }

    pos++;
    return FALSE;
}



bool spec_poison( CHAR_DATA *ch, char *cmd, DO_FUN *fnptr, char *arg, CHAR_DATA *mob )
{
    CHAR_DATA *victim;

    if ( ch != NULL || fnptr != NULL )
	return FALSE;

    if ( mob->fighting == NULL
    || ( victim = mob->fighting ) == NULL
    ||   number_percent( ) > 2 * mob->level )
	return FALSE;

    act( "You bite $N!", mob, victim, NULL, NULL, NULL, TO_CHAR, SENSE_SIXTH );
    act( "$n bites $N!", mob, victim, NULL, NULL, NULL, TO_NOTVICT, SENSE_SIGHT );
    act( "$n bites you!",mob, victim, NULL, NULL, NULL, TO_VICT, SENSE_SIGHT|SENSE_TOUCH );
    spell_poison( gsn_poison, mob->level, mob, victim );
    return TRUE;
}



bool spec_thief( CHAR_DATA *ch, char *cmd, DO_FUN *fnptr, char *arg, CHAR_DATA *mob )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    char buf[MAX_INPUT_LENGTH];

    if ( ch != NULL || fnptr != NULL )
	return FALSE;

    if ( mob->position != POS_STANDING )
	return FALSE;

    for ( victim = mob->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;

	if ( victim->level >= LEVEL_IMMORTAL
	||   number_bits( 2 ) != 0
	||   !can_see( mob, victim ) )	/* Thx Glop */
	    continue;

	    sprintf( buf, "coins %s", victim->name );
	    do_steal( mob, buf );
	    return TRUE;
    }

    return FALSE;
}
