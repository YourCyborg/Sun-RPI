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
 * Local functions.
 */
void	say_spell	args( ( CHAR_DATA *ch, int sn ) );



/*
 * Lookup a skill by name.
 */
int skill_lookup( const char *name )
{
    int sn;

    for ( sn = 0; sn < MAX_SKILL; sn++ )
    {
	if ( skill_table[sn].name == NULL )
	    break;
	if ( LOWER(name[0]) == LOWER(skill_table[sn].name[0])
	&&   !str_prefix( name, skill_table[sn].name ) )
	    return sn;
    }

    return -1;
}



/*
 * Lookup a skill by slot number.
 * Used for object loading.
 */
int slot_lookup( int slot )
{
    extern bool fBootDb;
    int sn;

    if ( slot <= 0 )
	return -1;

    for ( sn = 0; sn < MAX_SKILL; sn++ )
    {
	if ( slot == skill_table[sn].slot )
	    return sn;
    }

    if ( fBootDb )
    {
	bug( "Slot_lookup: bad slot %d.", slot );
	abort( );
    }

    return -1;
}



/*
 * Utter mystical words for an sn.
 */
void say_spell( CHAR_DATA *ch, int sn )
{
    char buf  [MAX_STRING_LENGTH];
    char buf2 [MAX_STRING_LENGTH];
    CHAR_DATA *rch;
    char *pName;
    int iSyl;
    int length;

    struct syl_type
    {
	char *	old;
	char *	new;
    };

    static const struct syl_type syl_table[] =
    {
	{ " ",		" "		},
	{ "ar",		"dneh"		},
	{ "au",		"ra"		},
	{ "bless",	"ing"		},
	{ "blind",	"ness"		},
	{ "bur",	"ner"		},
	{ "cu",		"rve"		},
	{ "de",		"stroy"		},
	{ "en",		"ter"		},
	{ "light",	"ton"		},
	{ "lo",		"wer"		},
	{ "mor",	"dor"		},
	{ "move",	"tv"		},
	{ "ness",	"loch"		},
	{ "ning",	"gaub"		},
	{ "per",	"jur"		},
	{ "ra",		"dio"		},
	{ "re",		"cor"		},
	{ "son",	"hav"		},
	{ "tect",	"til"		},
	{ "tri",	"phas"		},
	{ "ven",	"trik"		},
	{ "a", "i" }, { "b", "d" }, { "c", "s" }, { "d", "l" },
	{ "e", "a" }, { "f", "p" }, { "g", "q" }, { "h", "k" },
	{ "i", "e" }, { "j", "h" }, { "k", "j" }, { "l", "m" },
	{ "m", "n" }, { "n", "w" }, { "o", "u" }, { "p", "b" },
	{ "q", "x" }, { "r", "u" }, { "s", "z" }, { "t", "v" },
	{ "u", "o" }, { "v", "f" }, { "w", "g" }, { "x", "c" },
	{ "y", "i" }, { "z", "b" },
	{ "", "" }
    };

    buf[0]	= '\0';
    for ( pName = skill_table[sn].name; *pName != '\0'; pName += length )
    {
	for ( iSyl = 0; (length = strlen(syl_table[iSyl].old)) != 0; iSyl++ )
	{
	    if ( !str_prefix( syl_table[iSyl].old, pName ) )
	    {
		strcat( buf, syl_table[iSyl].new );
		break;
	    }
	}

	if ( length == 0 )
	    length = 1;
    }
    if ( IS_SET(ch->act, PLR_WIZINVIS) )
       return;
   // if ( !IS_SET(ch->act, PLR_WIZINVIS) ) {
    sprintf( buf2, "$n utters the words, '%s'", buf );
    sprintf( buf,  "$n utters the words, '%s'", skill_table[sn].name );
  //  return;
  //  }

    for ( rch = ch->in_room->people; rch; rch = rch->next_in_room )
    {
	if ( rch != ch )
	{
	    if ( get_skill( rch, sn ) > 5 )
	    {
		act( buf, ch, rch, NULL, NULL, NULL, TO_VICT, SENSE_SIGHT|SENSE_HEARING );
	    }
	    else
	    {
		act( buf2, ch, rch, NULL, NULL, NULL, TO_VICT, SENSE_SIGHT|SENSE_HEARING );
	    }
	}
    }

    return;
}



/*
 * Compute a saving throw.
 * Negative apply's make saving throw better.
 */
bool saves_spell( int level, CHAR_DATA *victim )
{
    int save;

    save = 50 + ( dice( 1, get_curr_int(victim) ) - level - victim->saving_throw ) * 5;
    save = URANGE( 5, save, 95 );
    return number_percent( ) < save;
}



/*
 * The kludgy global is for spells who want more stuff from command line.
 */
char *target_name;

void do_cast( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    void *vo;
    int mana;
    int sn;

    target_name = one_argument( argument, arg1 );
    one_argument( target_name, arg2 );

    if ( arg1[0] == '\0' )
    {
	send_to_char( "Cast which what where?\n\r", ch );
	return;
    }

    if ( !( sn = skill_lookup( arg1 ) ) || sn == -1 )
    {
	send_to_char( "You can't do that.\n\r", ch );
	return;
    }

    if ( ch->position < skill_table[sn].minimum_position )
    {
	send_to_char( "You can't concentrate enough.\n\r", ch );
	return;
    }

    mana = 0;

    /*
     * Locate targets.
     */
    victim	= NULL;
    obj		= NULL;
    vo		= NULL;
      
    switch ( skill_table[sn].target )
    {
    default:
	bug( "Do_cast: bad target for sn %d.", sn );
	return;

    case TAR_IGNORE:
	break;

    case TAR_CHAR_OFFENSIVE:
	if ( arg2[0] == '\0' )
	{
	    if ( ( victim = ch->fighting ) == NULL )
	    {
		send_to_char( "Cast the spell on whom?\n\r", ch );
		return;
	    }
	}
	else
	{
	    if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
	    {
		send_to_char( "They aren't here.\n\r", ch );
		return;
	    }
	}

	if ( ch == victim )
	{
	    send_to_char( "You can't do that to yourself.\n\r", ch );
	    return;
	}

	if ( !IS_NPC(ch) )
	{
	    if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim )
	    {
		send_to_char( "You can't do that on your own master.\n\r",
		    ch );
		return;
	    }
	}

	vo = (void *) victim;
	break;

    case TAR_CHAR_HINDRANCE:
	{
	    if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
	    {
		send_to_char( "They aren't here.\n\r", ch );
		return;
	    }
	}

	if ( !IS_NPC(ch) )
	{
	    if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim )
	    {
		send_to_char( "You can't do that to your own master.\n\r",
		    ch );
		return;
	    }
	}


	vo = (void *) victim;
	break;

    case TAR_CHAR_DEFENSIVE:
	if ( arg2[0] == '\0' )
	{
	    victim = ch;
	}
	else
	{
	    if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
	    {
		send_to_char( "They aren't here.\n\r", ch );
		return;
	    }
	}

	vo = (void *) victim;
	break;

    case TAR_CHAR_SELF:
	if ( arg2[0] != '\0' && !is_name( arg2, ch->name ) )
	{
	    send_to_char( "You cannot cast this spell on another.\n\r", ch );
	    return;
	}

	vo = (void *) ch;
	break;

    case TAR_OBJ_INV:
	if ( arg2[0] == '\0' )
	{
	    send_to_char( "What should the spell be cast upon?\n\r", ch );
	    return;
	}

	if ( ( obj = get_obj_carry( ch, arg2, ch ) ) == NULL )
	{
	    send_to_char( "You are not carrying that.\n\r", ch );
	    return;
	}

	vo = (void *) obj;
	break;
    }
	    
    if ( ch->mana < mana )
    {
	send_to_char( "You don't have enough mana.\n\r", ch );
	return;
    }
      
    if ( str_cmp( skill_table[sn].name, "ventriloquate" ) )
	say_spell( ch, sn );
      
    WAIT_STATE( ch, skill_table[sn].beats );
      
    if ( number_percent( ) > get_skill( ch, sn ) )
    {
	send_to_char( "You lost your concentration.\n\r", ch );
	ch->mana -= dice( 1, mana );

	skill_improve( ch, sn );
    }
    else
    {
	ch->mana -= mana;
	{
	    int power;

	    power = UMIN( get_power_level( ch, sn ),
			skill_table[sn].max_power + ch->channeling );

	    (*skill_table[sn].spell_fun) ( sn, power, ch, vo );

	    power_improve( ch, sn );
	}
    }

    if ( ( skill_table[sn].target == TAR_CHAR_OFFENSIVE
        || skill_table[sn].target == TAR_CHAR_HINDRANCE )
    &&   victim != ch )
    {
	CHAR_DATA *vch;

	for ( vch = ch->in_room->people; vch; vch = vch->next )
	{
	    if ( victim == vch && victim->fighting == NULL )
	    {
		set_hunting( victim, ch, 0, 100 );
		break;
	    }
	}
    }

    return;
}



/*
 * Cast spells at targets using a magical object.
 */
void obj_cast_spell( int sn, int level, CHAR_DATA *ch, CHAR_DATA *victim, OBJ_DATA *obj )
{
    void *vo;

    if ( sn <= 0 )
	return;

    if ( sn >= MAX_SKILL || skill_table[sn].spell_fun == 0 )
    {
	bug( "Obj_cast_spell: bad sn %d.", sn );
	return;
    }

    switch ( skill_table[sn].target )
    {
    default:
	bug( "Obj_cast_spell: bad target for sn %d.", sn );
	return;

    case TAR_IGNORE:
	vo = NULL;
	break;

    case TAR_CHAR_OFFENSIVE:
/* if no victim specified.. return */
	if ( victim == NULL )
	    return;
	vo = (void *) victim;
	break;

    case TAR_CHAR_DEFENSIVE:
/* if no victim specified.. return */
	if ( victim == NULL )
	    return;
/*
	if ( victim == NULL )
	    victim = ch;
*/
	vo = (void *) victim;
	break;

    case TAR_CHAR_SELF:
/* if no victim specified.. return */
	if ( victim == NULL )
	    return;
	vo = (void *) victim;
	break;

    case TAR_OBJ_INV:
/* if no victim specified.. return */
	if ( obj == NULL )
	    return;

	vo = (void *) obj;
	break;
    }

    target_name = "";
    (*skill_table[sn].spell_fun) ( sn, level, ch, vo );

    if ( ( skill_table[sn].target == TAR_CHAR_OFFENSIVE
        || skill_table[sn].target == TAR_CHAR_HINDRANCE )
    &&   victim != ch )
    {
	CHAR_DATA *vch;

	for ( vch = ch->in_room->people; vch; vch = vch->next )
	{
	    if ( victim == vch && victim->fighting == NULL )
	    {
		set_hunting( victim, ch, 0, 100 );
		break;
	    }
	}
    }

    return;
}



void spell_armor( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;


    if ( is_affected( victim, sn ) )
	return;
    af.type      = sn;
    af.duration  = level*36;
    af.modifier  = level/25+1;
    af.location  = APPLY_ABSORBTION;
    af.bitvector = 0;
    affect_to_char( victim, &af );

   /* if ( !IS_OUTSIDE(ch) )
	{
        send_to_char( "Sand drifts up your body, creating a hard armor around you.\n\r", victim );
	    act("Sand drifts up from the ground and creates a hard armor around $n,ch,NULL,NULL,NULL,NULL,TO_ROOM,SENSE_SIXTH );
		return;
	}*/

	send_to_char("Sand swirls in the room and collects on your body, forming a hard armor.\n\r",victim);
	if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_bless( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( check_fighting( victim ) || is_affected( victim, sn ) )
	return;
    af.type      = sn;
    af.duration  = level*36/10;
    af.location  = APPLY_HITROLL;
    af.modifier  = level / 4;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    af.location  = APPLY_SAVING_SPELL;
    af.modifier  = 0 - level / 25;
    affect_to_char( victim, &af );
    send_to_char( "You feel impowered\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_blindness( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_BLIND) || saves_spell( level, victim ) )
	return;

    af.type      = sn;
    af.location  = APPLY_HITROLL;
    af.modifier  = -40;
    af.duration  = level;
    af.bitvector = AFF_BLIND;
    affect_to_char( victim, &af );
    send_to_char( "You have been blinded!\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_charm_person( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( victim == ch )
    {
	send_to_char( "You like yourself even better!\n\r", ch );
	return;
    }

    if ( IS_IMMORTAL( victim ) && get_trust( victim ) >= get_trust( ch ) )
    {
	send_to_char( "Heh.\n\r", ch );
	return;
    }

    if ( IS_AFFECTED(victim, AFF_CHARM)
    ||   IS_AFFECTED(ch, AFF_CHARM)
    ||   level < victim->level
    ||   saves_spell( level, victim ) )
	return;

    if ( victim->master )
	stop_follower( victim );
    add_follower( victim, ch );
    af.type      = sn;
/* 1 hour real time if ch and victim int are equal */
    af.duration  = level * 72 * get_curr_int(ch)/(10 * get_curr_int(victim));
    af.location  = 0;
    af.modifier  = 0;
    af.bitvector = AFF_CHARM;
    affect_to_char( victim, &af );
    act( "You fall into a trance like state", ch, victim, NULL, NULL, NULL, TO_VICT, SENSE_SIXTH );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_control_weather( int sn, int level, CHAR_DATA *ch, void *vo )
{
    if ( !str_cmp( target_name, "better" ) )
	weather_info.change += dice( level / 8, 4 );
    else if ( !str_cmp( target_name, "worse" ) )
	weather_info.change -= dice( level / 30, 4 );
    else
	send_to_char ("Do you want it to get better or worse?\n\r", ch );

    send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_create_food( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA *mushroom;

    mushroom = create_object( get_obj_index( OBJ_VNUM_MUSHROOM ) );
    mushroom->value[0] = 5 + level / 4;
    obj_to_room( mushroom, ch->in_room, TRUE );
    act( "$p suddenly appears.", ch, NULL, mushroom, NULL, NULL, TO_ROOM, SENSE_SIGHT );
    act( "$p suddenly appears.", ch, NULL, mushroom, NULL, NULL, TO_CHAR, SENSE_SIGHT );
    return;
}



void spell_create_water( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    int water;

    if ( obj->item_type != ITEM_DRINK_CON )
    {
	send_to_char( "It is unable to hold water.\n\r", ch );
	return;
    }

    if ( obj->value[2] != LIQ_WATER && obj->value[1] != 0 )
    {
	send_to_char( "It contains some other liquid.\n\r", ch );
	return;
    }

    water = UMIN(
		level / 5 * (weather_info.sky >= SKY_CLOUDLESS ? 4 : 2),
		obj->value[0] - obj->value[1]
		);

  
    if ( water > 0 )
    {
	obj->value[2] = LIQ_WATER;
	obj->value[1] += water;
	if ( !is_name( "water", obj->name ) )
	{
	    char buf[MAX_STRING_LENGTH];

	    sprintf( buf, "%s water", obj->name );
	    free_string( obj->name, MEM_OBJ_NAME );
	    obj->name = str_dup( buf );
	}
	act( "$p is filled.", ch, NULL, obj, NULL, NULL, TO_CHAR, SENSE_SIGHT );
    }

    return;
}



void spell_cure_blindness( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    if ( !is_affected( victim, gsn_blindness ) )
	return;
    affect_strip( victim, gsn_blindness );
    send_to_char( "Your vision returns!\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_cure_poison( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    if ( is_affected( victim, gsn_poison ) )
    {
	affect_strip( victim, gsn_poison );
	act( "$N looks better.", ch, victim, NULL, NULL, NULL, TO_NOTVICT, SENSE_SIGHT );
	send_to_char( "A warm feeling runs through your body.\n\r", victim );
	send_to_char( "Ok.\n\r", ch );
    }
    return;
}



void spell_curse( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_CURSE) || saves_spell( level, victim ) )
	return;
    af.type      = sn;
    af.duration  = level*12;
    af.location  = APPLY_HITROLL;
    af.modifier  = -1 * level / 5;
    af.bitvector = AFF_CURSE;
    affect_to_char( victim, &af );

    af.location  = APPLY_SAVING_SPELL;
    af.modifier  = level / 25;
    affect_to_char( victim, &af );

/*        act("$n curses you under $s breath.",ch,NULL,victim,NULL,NULL,TO_VICT,SENSE_SIXTH );
	act("You curse $N under your breath.",ch,NULL,victim,NULL,NULL,TO_CHAR,SENSE_SIXTH );
	act("$n curses $N under $s breath.",ch,NULL,victim,NULL,NULL,TO_NOTVICT,SENSE_SIXTH);
*/
    send_to_char("You have been cursed.\n\r",victim);
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_detect_evil( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_DETECT_EVIL) )
	return;
    af.type      = sn;
    af.duration  = level*36/10;
    af.modifier  = 0;
    af.location  = APPLY_NONE;
    af.bitvector = AFF_DETECT_EVIL;
    affect_to_char( victim, &af );
    send_to_char( "Your eyes tingle.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_sense_life( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_DETECT_HIDDEN) )
	return;
    af.type      = sn;
    af.duration  = level*18/10;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_DETECT_HIDDEN;
    affect_to_char( victim, &af );
    send_to_char( "Your awareness improves.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_detect_invis( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_DETECT_INVIS) )
	return;
    af.type      = sn;
    af.duration  = level*36/10;
    af.modifier  = 0;
    af.location  = APPLY_NONE;
    af.bitvector = AFF_DETECT_INVIS;
    affect_to_char( victim, &af );
    send_to_char( "Your eyes tingle.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_detect_magic( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_DETECT_MAGIC) )
	return;
    af.type      = sn;
    af.duration  = level*18/10;
    af.modifier  = 0;
    af.location  = APPLY_NONE;
    af.bitvector = AFF_DETECT_MAGIC;
    affect_to_char( victim, &af );
    send_to_char( "Your eyes tingle.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_detect_poison( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;

    if ( obj->item_type == ITEM_DRINK_CON || obj->item_type == ITEM_FOOD )
    {
	if ( obj->value[3] != 0 )
	    send_to_char( "You smell poisonous fumes.\n\r", ch );
	else
	    send_to_char( "It looks very delicious.\n\r", ch );
    }
    else
    {
	send_to_char( "It doesn't look poisoned.\n\r", ch );
    }

    return;
}



void spell_dispel_evil( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;
  
    if ( IS_EVIL(ch) )
	victim = ch;
  
    if ( IS_GOOD(victim) )
    {
	act( "God protects $N.", ch, victim, NULL, NULL, NULL, TO_ROOM, SENSE_SIXTH );
	return;
    }

    if ( IS_NEUTRAL(victim) )
    {
	act( "$N does not seem to be affected.", ch, victim, NULL, NULL, NULL, TO_CHAR, SENSE_SIXTH );
	return;
    }

    if (ch->level > victim->level)
	dam = 100;
    else
	dam = dice( level / 4, 4 );
    if ( saves_spell( level, victim ) )
	dam /= 2;
    damage( ch, victim, dam, sn, NULL, gn_wear_none );
    return;
}



void spell_dispel_magic( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA *paf;
    AFFECT_DATA *paf_next;

    if (saves_spell( level, victim ))
	return;

    for ( paf = victim->affected ; paf != NULL ; paf = paf_next )
    {
	paf_next = paf->next;

	if ( paf->type > 0 && paf->type < MAX_SKILL
	    && ( skill_table[paf->type].target == TAR_CHAR_SELF
	      || skill_table[paf->type].target == TAR_CHAR_DEFENSIVE ) )
		affect_remove( victim, paf );
    }

    act("$n points at you and you feel your enchantments fading.", ch, victim, NULL, NULL, NULL, TO_VICT, SENSE_SIXTH );

    return;
}



void spell_earthquake( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    

    send_to_char( "Sand swirls up from under you as the sand splits apart\n\r", ch );
    act( "The ground beneath you begins to shake violently", ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIXTH );

    for ( vch = char_list; vch != NULL; vch = vch_next )
    {
	vch_next	= vch->next;
	if ( vch->in_room == NULL )
	    continue;
	if ( vch->in_room == ch->in_room )
	{
	    if ( vch != ch)
        {
		damage( ch, vch, level / 4 + dice(2, 8), sn, NULL, gn_wear_none );    
                vch->position = POS_RESTING;
                return;
        }
            
	    continue;
	}

	if ( vch->in_room->area == ch->in_room->area )
	    send_to_char( "The ground beneath you feet begins to shake violently\n\r", vch );

        if ( vch->position == POS_STANDING )
        {
            send_to_char( "You topple over and fall at the sheer velocity of the earthquake.\n\r",vch );
            vch->position = POS_RESTING;
        }
        
    }

    return;
}



void spell_enchant_weapon( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    AFFECT_DATA *paf;

    if ( obj->item_type != ITEM_WEAPON
    ||   IS_OBJ_STAT(obj, ITEM_MAGIC)
    ||   obj->affected != NULL )
	return;

    if ( affect_free == NULL )
    {
	paf		= alloc_perm( sizeof(*paf) );
    }
    else
    {
	paf		= affect_free;
	affect_free	= affect_free->next;
    }

    paf->type		= -1;
    paf->duration	= -1;
    paf->location	= APPLY_HITROLL;
    paf->modifier	= level / 5;
    paf->bitvector	= 0;
    paf->next		= obj->affected;
    obj->affected	= paf;

    if ( affect_free == NULL )
    {
	paf		= alloc_perm( sizeof(*paf) );
    }
    else
    {
	paf		= affect_free;
	affect_free	= affect_free->next;
    }

    paf->type		= -1;
    paf->duration	= -1;
    paf->location	= APPLY_DAMROLL;
    paf->modifier	= level / 25;
    paf->bitvector	= 0;
    paf->next		= obj->affected;
    obj->affected	= paf;

    if ( IS_GOOD(ch) )
    {
	SET_BIT(obj->extra_flags, ITEM_ANTI_EVIL);
	act( "$p glows blue.", ch, NULL, obj, NULL, NULL, TO_CHAR, SENSE_SIXTH );
    }
    else if ( IS_EVIL(ch) )
    {
	SET_BIT(obj->extra_flags, ITEM_ANTI_GOOD);
	act( "$p glows red.", ch, NULL, obj, NULL, NULL, TO_CHAR, SENSE_SIXTH );
    }
    else
    {
	act( "$p glows yellow.", ch, NULL, obj, NULL, NULL, TO_CHAR, SENSE_SIXTH );
    }

    send_to_char( "Ok.\n\r", ch );
    return;
}



/*
 * Caster gains HP.
 */
void spell_vampiric_touch( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    if ( saves_spell( level, victim ) )
	return;

    ch->alignment = UMAX(-1000, ch->alignment - 200);
    {
	dam		 = dice(1, level);
	ch->hit[HIT_LOC_BODY]		+= dam;
    }

    damage( ch, victim, dam, sn, NULL, gn_wear_none );

    return;
}



void spell_fireball( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam		=	dice( 3, level ) + level;
    if ( saves_spell( level, victim ) )
	dam /= 2;
    damage( ch, victim, dam, sn, NULL, gn_wear_none );
    return;
}


void spell_flash( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *vch;
    act( "$n's releases a tremendous flash of light!", ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIGHT );
    act( "You cause a tremendous burst of light!", ch, NULL, NULL, NULL, NULL, TO_CHAR, SENSE_SIGHT );

    for ( vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room )
    {
	if ( !saves_spell( level, vch ) )
	{
	    vch->vision_current -= 2*level;
	    send_to_char("You are blinded by light!\n\r", vch );
	}
    }

    return;
}


void spell_fly( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_FLYING) )
	return;
    af.type      = sn;
    af.duration  = level*18;
    af.location  = 0;
    af.modifier  = 0;
    af.bitvector = AFF_FLYING;
    affect_to_char( victim, &af );
    send_to_char( "Your feet rise off the ground.\n\r", victim );
    act( "$n's feet rise off the ground.", victim, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIGHT );
    return;
}



void spell_gate( int sn, int level, CHAR_DATA *ch, void *vo )
{
/* gate spell */
    return;
}



void spell_strength( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
	return;
    af.type      = sn;
    af.duration  = level;
    af.location  = APPLY_STAT_PHYS_STR;
    af.modifier  = level/15;
    af.bitvector = 0;
    affect_to_char( victim, &af );
    send_to_char( "You feel stronger.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_harm( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = dice( 1, level ) + level;
    if ( saves_spell( level, victim ) )
	dam /= 2;
    damage( ch, victim, dam, sn, NULL, gn_wear_none );
    return;
}



void spell_heal( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int heal;

    heal = dice( 1, level ) + level;

    victim->hit[HIT_LOC_BODY] = UMIN( victim->hit[HIT_LOC_BODY] + heal, victim->max_hit );
    update_pos( victim );
    send_to_char( "A warm feeling fills your body.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_identify( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    char buf[MAX_STRING_LENGTH];
    AFFECT_DATA *paf;

    sprintf( buf,
	"Object '%s' is type %s, extra flags %s.\n\rWeight is %d, value is %d.\n\r",

	obj->name,
	item_type_name( obj ),
	extra_bit_name( obj->extra_flags ),
	obj->weight,
	obj->cost
	);
    send_to_char( buf, ch );

    switch ( obj->item_type )
    {
    case ITEM_SCROLL: 
    case ITEM_POTION:
	sprintf( buf, "Level %d spells of:", obj->value[0] );
	send_to_char( buf, ch );

	if ( obj->value[1] >= 0 && obj->value[1] < MAX_SKILL )
	{
	    send_to_char( " '", ch );
	    send_to_char( skill_table[obj->value[1]].name, ch );
	    send_to_char( "'", ch );
	}

	if ( obj->value[2] >= 0 && obj->value[2] < MAX_SKILL )
	{
	    send_to_char( " '", ch );
	    send_to_char( skill_table[obj->value[2]].name, ch );
	    send_to_char( "'", ch );
	}

	if ( obj->value[3] >= 0 && obj->value[3] < MAX_SKILL )
	{
	    send_to_char( " '", ch );
	    send_to_char( skill_table[obj->value[3]].name, ch );
	    send_to_char( "'", ch );
	}

	send_to_char( ".\n\r", ch );
	break;

    case ITEM_WAND: 
    case ITEM_STAFF: 
	sprintf( buf, "Has %d(%d) charges of level %d",
	    obj->value[1], obj->value[2], obj->value[0] );
	send_to_char( buf, ch );
      
	if ( obj->value[3] >= 0 && obj->value[3] < MAX_SKILL )
	{
	    send_to_char( " '", ch );
	    send_to_char( skill_table[obj->value[3]].name, ch );
	    send_to_char( "'", ch );
	}

	send_to_char( ".\n\r", ch );
	break;
      
    case ITEM_WEAPON:
	sprintf( buf, "Damage is %dd%d.\n\r",
	    obj->value[1], obj->value[2] );
	send_to_char( buf, ch );
	break;

    case ITEM_ARMOR:
	sprintf( buf, "Armor class is %d.\n\r", obj->value[0] );
	send_to_char( buf, ch );
	break;
    }

    for ( paf = obj->affected; paf != NULL; paf = paf->next )
    {
	if ( paf->location != APPLY_NONE && paf->modifier != 0 )
	{
	    sprintf( buf, "Affects %s by %d.\n\r",
		affect_loc_name( paf->location ), paf->modifier );
	    send_to_char( buf, ch );
	}
    }

    return;
}



void spell_infravision( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
	return;
    act( "$n's eyes glow red.\n\r", ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIGHT );
    af.type      = sn;
    af.duration  = level*18/10;
    af.location  = APPLY_VISION_PROFILE;
    af.modifier  = -1 * level;
    af.bitvector = 0;
    affect_to_char( victim, &af );
    send_to_char( "Your eyes glow red.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_invis( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_INVISIBLE) )
	return;

    act( "$n fades out of existence.", victim, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIGHT );
    af.type      = sn;
    af.duration  = level*36;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_INVISIBLE;
    affect_to_char( victim, &af );
    send_to_char( "You fade out of existence.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_lightning_bolt( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam		= dice( 1, level ) + level;
    if ( saves_spell( level, victim ) )
	dam /= 2;
    damage( ch, victim, dam, sn, NULL, gn_wear_none );
    return;
}



void spell_locate_object( int sn, int level, CHAR_DATA *ch, void *vo )
{
    char buf[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *in_obj;
    bool found;

    found = FALSE;
    for ( obj = object_list; obj != NULL; obj = obj->next )
    {
	if ( !can_see_obj( ch, obj ) || !is_name( target_name, obj->name ) )
	    continue;

	if ( number_percent() > level )
	    continue;

	found = TRUE;

	for ( in_obj = obj; in_obj->in_obj != NULL; in_obj = in_obj->in_obj )
	    ;

	if ( in_obj->carried_by != NULL )
	{
	    sprintf( buf, "%s carried by %s.\n\r",
		obj->short_descr, person(ch, in_obj->carried_by) );
	}
	else
	{
	    sprintf( buf, "%s in %s.\n\r",
		obj->short_descr, in_obj->in_room == NULL
		    ? "somewhere" : in_obj->in_room->name );
	}

	buf[0] = UPPER(buf[0]);
	send_to_char( buf, ch );
    }

    if ( !found )
	send_to_char( "Nothing like that in hell, earth, or heaven.\n\r", ch );

    return;
}



void spell_pebble( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam		= dice( 1 , level ) + level;
    if ( saves_spell( level, victim ) )
	dam /= 2;
    damage( ch, victim, dam, sn, NULL, gn_wear_none );
    return;
}



void spell_null( int sn, int level, CHAR_DATA *ch, void *vo )
{
    send_to_char( "That's not a spell!\n\r", ch );
    return;
}



void spell_pass_door( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_PASS_DOOR) )
	return;
    af.type      = sn;
    af.duration  = level/3+1;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_PASS_DOOR;
    affect_to_char( victim, &af );
    act( "$n turns translucent.", victim, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIGHT );
    send_to_char( "You turn translucent.\n\r", victim );
    return;
}



void spell_poison( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( saves_spell( level, victim ) )
	return;
    af.type      = sn;
    af.duration  = level*10;
    af.location  = APPLY_STAT_PHYS_STR;
    af.modifier  = -1 * level/15;
    af.bitvector = AFF_POISON;
    affect_join( victim, &af );
    send_to_char( "You feel very sick.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_protection_from_evil( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_PROTECT) )
	return;
    af.type      = sn;
    af.duration  = level*18/10;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_PROTECT;
    affect_to_char( victim, &af );
    send_to_char( "You feel protected.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_vitality( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    victim->move = UMIN(victim->max_move,victim->move+level);
    send_to_char( "You feel less tired.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_remove_curse( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    if ( is_affected( victim, gsn_curse ) )
    {
	affect_strip( victim, gsn_curse );
	send_to_char( "You feel better.\n\r", victim );
	if ( ch != victim )
	    send_to_char( "Ok.\n\r", ch );
    }

    return;
}



void spell_sanctuary( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_SANCTUARY) )
	return;
    af.type      = sn;
    af.duration  = level/2;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_SANCTUARY;
    affect_to_char( victim, &af );
    act( "$n is surrounded by a white aura.", victim, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIGHT );
    send_to_char( "You are surrounded by a white aura.\n\r", victim );
    return;
}



void spell_sleep( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
  
    if ( IS_AFFECTED(victim, AFF_SLEEP)
    ||   level < victim->level
    ||   saves_spell( level, victim ) )
	return;

    af.type      = sn;
    af.duration  = level/2+12;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_SLEEP;
    affect_join( victim, &af );

    if ( IS_AWAKE(victim) )
    {
	send_to_char( "You feel very sleepy ..... zzzzzz.\n\r", victim );
	act( "$n goes to sleep.", victim, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIGHT );
	victim->position = POS_SLEEPING;
    }

    return;
}



void spell_stone_skin( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( ch, sn ) )
	return;
    af.type      = sn;
    af.duration  = level/2;
    af.location  = APPLY_ABSORBTION;
    af.modifier  = level/3;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    af.type      = sn;
    af.location  = APPLY_STAT_PHYS_DEX;
    af.modifier  = -150 + level;
    affect_to_char( victim, &af );
    act( "$n's skin turns to stone.", victim, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIGHT );
    send_to_char( "Your skin turns to stone.\n\r", victim );
    return;
}



void spell_summon( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim;

    if ( ( victim = get_char_world( ch, target_name ) ) == NULL
    ||   victim == ch
    ||   victim->in_room == NULL
    ||   IS_SET(victim->in_room->room_flags, ROOM_SAFE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_PRIVATE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_SOLITARY)
    ||   IS_SET(victim->in_room->room_flags, ROOM_NO_RECALL)
    ||   check_fighting( victim )
    ||   saves_spell( level, victim ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    act( "A swirling black vortex of light appears and engulfs $n.", victim, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIGHT );
    char_from_room( victim );
    char_to_room( victim, ch->in_room, TRUE );
    act( "A black vortex opens in the sky and $n falls through.", victim, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIGHT );
    act( "You feel a pulling sensation as a black vortex appears and begins to surround you.", ch, victim, NULL, NULL, NULL, TO_VICT, SENSE_SIXTH );
    send_to_char("You fall through the portal, landing roughly.\n\r", victim );
    victim->position = POS_RESTING;
    do_look( victim, "auto" );
    return;
}



void spell_teleport( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    ROOM_INDEX_DATA *pRoomIndex;

    if ( victim->in_room == NULL
    ||   IS_SET(victim->in_room->room_flags, ROOM_NO_RECALL)
    || ( victim != ch
    && ( saves_spell( level, victim ) || saves_spell( level, victim ) ) ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    for ( ; ; )
    {
	pRoomIndex = get_room_index( number_range( 0, 65535 ) );
	if ( pRoomIndex != NULL )
	if ( !IS_SET(pRoomIndex->room_flags, ROOM_PRIVATE)
	&&   !IS_SET(pRoomIndex->room_flags, ROOM_SOLITARY) )
	    break;
    }

    act( "$n slowly fades out of existence.", victim, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIGHT );
    char_from_room( victim );
    char_to_room( victim, pRoomIndex, TRUE );
    act( "$n slowly fades into existence.", victim, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIGHT );
    do_look( victim, "auto" );
    return;
}



void spell_ventriloquate( int sn, int level, CHAR_DATA *ch, void *vo )
{
    char buf1[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char speaker[MAX_INPUT_LENGTH];
    CHAR_DATA *vch;

    target_name = one_argument( target_name, speaker );

    sprintf( buf1, "%s says '%s'.\n\r",              speaker, target_name );
    sprintf( buf2, "Someone makes %s say '%s'.\n\r", speaker, target_name );
    buf1[0] = UPPER(buf1[0]);

    for ( vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room )
    {
	if ( !is_name( speaker, vch->name ) )
	    send_to_char( saves_spell( level, vch ) ? buf2 : buf1, vch );
    }

    return;
}



void spell_weaken( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) || saves_spell( level, victim ) )
	return;
    af.type      = sn;
    af.duration  = level/2;
    af.location  = APPLY_STAT_PHYS_STR;
    af.modifier  = -1 * level / 12;
    af.bitvector = 0;
    affect_to_char( victim, &af );
    send_to_char( "You feel weaker.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_word_of_recall( int sn, int level, CHAR_DATA *ch, void *vo )
{
/* need to write a proc */
    return;
}



/*
 * NPC spells.
 */
void spell_acid_breath( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    OBJ_DATA *obj_lose;
    OBJ_DATA *obj_next;
    int dam;
    int hpch;

    if ( number_percent( ) < 2 * level && !saves_spell( level, victim ) )
    {
	for ( obj_lose = ch->carrying; obj_lose != NULL; obj_lose = obj_next )
	{
	    obj_next = obj_lose->next_content;

	    if ( number_bits( 2 ) != 0 )
		continue;

	    switch ( obj_lose->item_type )
	    {
	    case ITEM_ARMOR:
		if ( obj_lose->value[0] > 0 )
		{
		    act( "$p is pitted and etched!",
			victim, NULL, obj_lose, NULL, NULL, TO_CHAR, SENSE_SIXTH );
/* damage armor */
		}
		break;

	    case ITEM_CONTAINER:
		act( "$p fumes and dissolves!",
		    victim, NULL, obj_lose, NULL, NULL, TO_CHAR, SENSE_SIXTH );
		extract_obj( obj_lose, TRUE );
		break;
	    }
	}
    }

    hpch = UMAX( 10, ch->hit[HIT_LOC_BODY] );
    dam  = number_range( hpch/16+1, hpch/8 );
    if ( saves_spell( level, victim ) )
	dam /= 2;
    damage( ch, victim, dam, sn, NULL, gn_wear_none );
    return;
}



void spell_fire_breath( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    OBJ_DATA *obj_lose;
    OBJ_DATA *obj_next;
    int dam;
    int hpch;

    if ( number_percent( ) < 2 * level && !saves_spell( level, victim ) )
    {
	for ( obj_lose = victim->carrying; obj_lose != NULL;
	obj_lose = obj_next )
	{
	    char *msg;

	    obj_next = obj_lose->next_content;
	    if ( number_bits( 2 ) != 0 )
		continue;

	    switch ( obj_lose->item_type )
	    {
	    default:             continue;
	    case ITEM_CONTAINER: msg = "$p ignites and burns!";   break;
	    case ITEM_POTION:    msg = "$p bubbles and boils!";   break;
	    case ITEM_SCROLL:    msg = "$p crackles and burns!";  break;
	    case ITEM_STAFF:     msg = "$p smokes and chars!";    break;
	    case ITEM_WAND:      msg = "$p sparks and sputters!"; break;
	    case ITEM_FOOD:      msg = "$p blackens and crisps!"; break;
	    case ITEM_PILL:      msg = "$p melts and drips!";     break;
	    }

	    act( msg, victim, NULL, obj_lose, NULL, NULL, TO_CHAR, SENSE_SIXTH );
	    extract_obj( obj_lose, TRUE );
	}
    }

    hpch = UMAX( 10, ch->hit[HIT_LOC_BODY] );
    dam  = number_range( hpch/16+1, hpch/8 );
    if ( saves_spell( level, victim ) )
	dam /= 2;
    damage( ch, victim, dam, sn, NULL, gn_wear_none );
    return;
}



void spell_frost_breath( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    OBJ_DATA *obj_lose;
    OBJ_DATA *obj_next;
    int dam;
    int hpch;

    if ( number_percent( ) < 2 * level && !saves_spell( level, victim ) )
    {
	for ( obj_lose = victim->carrying; obj_lose != NULL;
	obj_lose = obj_next )
	{
	    char *msg;

	    obj_next = obj_lose->next_content;
	    if ( number_bits( 2 ) != 0 )
		continue;

	    switch ( obj_lose->item_type )
	    {
	    default:            continue;
	    case ITEM_CONTAINER:
	    case ITEM_DRINK_CON:
	    case ITEM_POTION:   msg = "$p freezes and shatters!"; break;
	    }

	    act( msg, victim, NULL, obj_lose, NULL, NULL, TO_CHAR, SENSE_SIXTH );
	    extract_obj( obj_lose, TRUE );
	}
    }

    hpch = UMAX( 10, ch->hit[HIT_LOC_BODY] );
    dam  = number_range( hpch/16+1, hpch/8 );
    if ( saves_spell( level, victim ) )
	dam /= 2;
    damage( ch, victim, dam, sn, NULL, gn_wear_none );
    return;
}



void spell_gas_breath( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    int dam;
    int hpch;

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
	vch_next = vch->next_in_room;
	if ( ch != vch )
	{
	    hpch = UMAX( 10, ch->hit[HIT_LOC_BODY] );
	    dam  = number_range( hpch/16+1, hpch/8 );
	    if ( saves_spell( level, vch ) )
		dam /= 2;
	    damage( ch, vch, dam, sn, NULL, gn_wear_none );
	}
    }
    return;
}



void spell_lightning_breath( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;
    int hpch;

    hpch = UMAX( 10, ch->hit[HIT_LOC_BODY] );
    dam = number_range( hpch/16+1, hpch/8 );
    if ( saves_spell( level, victim ) )
	dam /= 2;
    damage( ch, victim, dam, sn, NULL, gn_wear_none );
    return;
}
