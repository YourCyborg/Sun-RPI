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
#include <string.h>
#include <time.h>
#include "merc.h"



/*
 * Local functions.
 */
void	dam_message	args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam,
			    int dt, OBJ_DATA *obj ) );
void	dam_message_physical	args( ( CHAR_DATA *ch, CHAR_DATA *victim,
			    int dam, int dt, OBJ_DATA *obj, int location ) );
void	death_cry	args( ( CHAR_DATA *ch ) );
void	group_gain	args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool	is_safe		args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void	make_corpse	args( ( CHAR_DATA *ch ) );
void	raw_kill	args( ( CHAR_DATA *victim ) );
void	maybe_set_fighting	args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void	set_fighting	args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void	disarm		args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void	bash		args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
int	calc_strike_rank args( ( CHAR_DATA *ch ) );
void	second_hit	args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ) );
int	check_armor	args( ( int dam, CHAR_DATA *ch, CHAR_DATA *victim,
			    int ptype, int location ) );
int	get_hit_location  args( ( CHAR_DATA *ch, CHAR_DATA *victim, int pt ) );

/*
 * global to prevent crash where weapons are dropped during corpse creation.
 */
int	making_corpse = FALSE;

/*
 * semi-global for adjusting next character in combat.
 *
 * if "next" dies, it's a bad thing.
 */
CHAR_DATA	*combat_next = NULL;



/*
 * Control the fights going on.
 * Called periodically by update_handler.
 */
void violence_update( void )
{
    CHAR_DATA *ch;
    CHAR_DATA *victim;
    CHAR_DATA *rch;
    CHAR_DATA *rch_next;
    OBJ_DATA *obj;

    for ( ch = combat_list; ch != NULL; ch = combat_next )
    {
	combat_next = ch->next_fighting;

	if ( ( victim = ch->fighting ) == NULL || ch->in_room == NULL )
	{
	    stop_fighting( ch, FALSE );
	    continue;
	}

	if ( check_specials( ch, NULL, (DO_FUN *) violence_update, NULL ) )
	    continue;

      if ( --ch->strike_rank < 0 )
      {
	if ( IS_AWAKE(ch) && ch->in_room == victim->in_room )
	    one_hit( ch, victim, TYPE_UNDEFINED );
	else
	    stop_fighting( ch, FALSE );

	ch->strike_rank = calc_strike_rank( ch );

	if ( ( victim = ch->fighting ) == NULL )
	    continue;
      }

	if ( (obj = get_eq_char( ch, gn_wear_secondary )) != NULL
	&& obj->item_type == ITEM_WEAPON
	&& --ch->strike_rank_2 < 0 )
	{
	    if ( IS_AWAKE(ch) && ch->in_room == victim->in_room )
		second_hit( ch, victim, TYPE_UNDEFINED );
	    else
		stop_fighting( ch, FALSE );

	    ch->strike_rank_2 = calc_strike_rank( ch ) + 100
		- UMIN( 100, get_skill( ch, gsn_dual_wield ) );

	    if ( ( victim = ch->fighting ) == NULL )
		continue;
	}

	/*
	 * Fun for the whole family!
	 */
	for ( rch = ch->in_room->people; rch != NULL; rch = rch_next )
	{
	    rch_next = rch->next_in_room;

	    if ( IS_AWAKE(rch) && rch->fighting == NULL )
	    {
		/*
		 * NPC's assist NPC's based on criteria to be determined.
		 */
		if ( IS_NPC(rch) && !IS_AFFECTED(rch, AFF_CHARM) )
		{
/* npcs might assist later based on other criteria */
		    if (0)
		    {
			if ( victim != NULL )
			    one_hit( rch, victim, TYPE_UNDEFINED );
		    }
		}
	    }
	}
    }

    return;
}


int calc_strike_rank( CHAR_DATA *ch )
{
    int sr;
    OBJ_DATA *wield;

/* formula is:
 *
 *  12 (3 seconds) + (100 - str)/10 + (100 - dex)/10
 *
 * +- weapon size bonuses etc.
 */

    sr = 33;

    sr -= (get_curr_dex( ch ) + get_curr_str( ch ))/10;

    if ( ( wield = get_eq_char( ch, gn_wear_primary ) ) != NULL )
    {
/* weapons less than weight 4 pounds give bonus, not penalty */
	sr -= 4;

	sr += wield->weight/10;

/* length */

    }

/* can't be less than 2 seconds */
    sr = UMAX( 8, sr );

    sr = dice( 2, sr );

    return sr;
}


/*
 * Hit one guy once.
 */
void one_hit( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
    OBJ_DATA *wield;
    int attacker_roll;
    int defense_roll;
    int hit_chance;
    int dam;
    int location;

    /*
     * Can't beat a dead char!
     * Guard against weird room-leavings.
     */
    if ( victim->position == POS_DEAD || ch->in_room != victim->in_room )
	return;

    /*
     * Figure out the type of damage message.
     */
    wield = get_eq_char( ch, gn_wear_primary );
    if ( dt == TYPE_UNDEFINED )
    {
	dt = TYPE_HIT;
	if ( wield != NULL )
	    dt += wield->attack_type;
    }

    /*
     * Calculate hit and dodge values
     */
    attacker_roll = ch->perm_offense + ch->mod_offense;

    attacker_roll += GET_HITROLL(ch);

    if ( !can_see( ch, victim ) )
	attacker_roll -= 40;

    defense_roll = victim->perm_defense + victim->mod_defense;

    hit_chance = attacker_roll - defense_roll;

    hit_chance = URANGE( 5, hit_chance, 95 );

    location = get_hit_location( ch, victim, (dt - TYPE_HIT) );

    /*
     * The moment of excitement!
     */
    if ( dice(1,100) > hit_chance && IS_AWAKE(victim))
    {
	/* Miss. */
	damage( ch, victim, 0, dt, wield, location );
	tail_chain( );
	return;
    }

    /* learn */
    if ( !IS_NPC( ch ) )
    {
	if ( dice(1,100) > ch->perm_offense && !number_bits( 5 ) )
	    ch->perm_offense++;
    }

    if ( !IS_NPC( victim ) )
    {
	if ( dice(1,100) > victim->perm_defense && !number_bits( 4 ) )
	    victim->perm_defense++;
    }

    /*
     * Hit.
     * Calc damage.
     */
    if (wield == NULL )
    {
	dam = dice( ch->damnodice, ch->damsizedice );

	if (IS_NPC(ch))
	    dam = dam + ch->damplus;
    }
    else
    {
	dam = dice( wield->dam[0], wield->dam[1] );

	if ( IS_NPC( ch ) )
	    dam += ch->damplus;

/* weapon + bare hands */
/*
	if ( IS_NPC( ch ) )
	    dam += dice( ch->damnodice, ch->damsizedice ) + ch->damplus;
*/
    }

    /*
     * Bonuses.
     */
    dam += GET_DAMROLL(ch);
    if ( get_skill( ch, gsn_enhanced_damage) > 0 )
	dam += dam * get_skill( ch, gsn_enhanced_damage ) / 100;
    if ( !IS_AWAKE(victim) )
	dam *= 2;
    if ( dt == gsn_backstab )
    {
	dam = dam*(get_power_level( ch, dt ) )/10;

	if (dice(1,80) > get_power_level( ch, gsn_backstab) )
	{
	    power_improve( ch, gsn_backstab );
	}
    }
    else
    {
	dam *= race_array[victim->race]->hit_loc_damage[location];
	dam /= 100;
    }

    if ( dam <= 0 )
	dam = 1;

    damage( ch, victim, dam, dt, wield, location );

/* damage attacker's weapon */
    if ( wield != NULL )
	damage_item( wield, dam, TRUE, FALSE );

    tail_chain( );
    return;
}


/*
 * dual wield version of one_hit
 */
void second_hit( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
    OBJ_DATA *wield;
    int attacker_roll;
    int defense_roll;
    int hit_chance;
    int dam;
    int location;

    /*
     * Can't beat a dead char!
     * Guard against weird room-leavings.
     */
    if ( victim->position == POS_DEAD || ch->in_room != victim->in_room )
	return;

    /*
     * Figure out the type of damage message.
     */
    wield = get_eq_char( ch, gn_wear_secondary );
    dt = TYPE_HIT + wield->attack_type;

    /*
     * Calculate hit and dodge values
     */
    attacker_roll = ch->perm_offense + ch->mod_offense;

    attacker_roll += GET_HITROLL(ch);

    if ( !can_see( ch, victim ) )
	attacker_roll -= 40;

    defense_roll = victim->perm_defense + victim->mod_defense;

    hit_chance = attacker_roll - defense_roll;

    hit_chance = URANGE( 5, hit_chance, 95 );

    /*
     *  Less chance to hit with second weapon
     */
    hit_chance -= 50 - UMIN( 50, get_skill( ch, gsn_dual_wield ) );

    location = get_hit_location( ch, victim, (dt - TYPE_HIT) );

    /*
     * The moment of excitement!
     */
    if ( dice(1,100) > hit_chance && IS_AWAKE(victim))
    {
	/* Miss. */
	damage( ch, victim, 0, dt, wield, location );
	tail_chain( );
	return;
    }

/* learn */
    if ( !IS_NPC( ch ) )
    {
	if ( dice(1,100) > ch->pcdata->learned[gsn_dual_wield]
	&& !number_bits( 4 ) )
	    ch->pcdata->learned[gsn_dual_wield]++;
    }

    if ( !IS_NPC( victim ) )
    {
	if ( dice(1,100) > victim->perm_defense && !number_bits( 4 ) )
	    victim->perm_defense++;
    }

    /*
     * Hit.
     * Calc damage.
     */
    dam = dice( wield->dam[0], wield->dam[1] );

    /*
     * Bonuses.
     */
    dam += GET_DAMROLL(ch);
/*
 * no enhanced on second hand
 */
/*
    if ( !IS_NPC(ch) && ch->pcdata->learned[gsn_enhanced_damage] > 0 )
	dam += dam * ch->pcdata->learned[gsn_enhanced_damage] / 100;
*/
    if ( !IS_AWAKE(victim) )
	dam *= 2;

    if ( dam <= 0 )
	dam = 1;

    damage( ch, victim, dam, dt, wield, location );

/* damage attacker's weapon */
    if ( wield != NULL )
	damage_item( wield, dam, TRUE, FALSE );

    tail_chain( );
    return;
}



void damage_from_object( CHAR_DATA *ch, OBJ_DATA *obj, int dam, int sn )
{
    damage( ch, ch, dam, sn, obj, gn_wear_none );
    return;
}


void damage_item( OBJ_DATA *obj, int dam, bool fUsed, bool fPropigate )
{
    CHAR_DATA *ch;
    int wear_loc = obj->wear_loc;

/* indestructible items */
    if ( obj-> toughness == -1 )
	return;

    obj->toughness -= dam;

    if ( obj->toughness > 0 )
	return;

    if ( (ch = obj->carried_by) != NULL )
    {
      if ( fUsed )
      {
	act("Your $p is destroyed!", ch, NULL, obj, NULL, NULL, TO_CHAR, SENSE_SIXTH );
	act("$p used by $n is destroyed!", ch, NULL, obj, NULL, NULL, TO_ROOM, SENSE_SIGHT );
      }
      else
      {
	act("$p is destroyed!", ch, NULL, obj, NULL, NULL, TO_CHAR, SENSE_SIXTH );
	act("$p is destroyed!", ch, NULL, obj, NULL, NULL, TO_ROOM, SENSE_SIGHT );
      }
	unequip_char( ch, obj );
	obj_from_char( obj );
	remove_dependent_items( ch, wear_loc );
    }
    extract_obj( obj, TRUE );

    return;
}


/*
 * Inflict damage from a hit.
 */
void damage( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, OBJ_DATA *obj, int location )
{
    int ptype;
    bool fMessage = TRUE;

    if ( victim->position == POS_SLEEPING && dam > 0 )
    {
	send_to_char("You are awoken by combat!\n\r", victim );
	victim->position = POS_RESTING;
    }

    if ( victim->position == POS_DEAD )
	return;

    /*
     * Stop up any residual loopholes.
     */
    if ( dam > 1000 )
    {
	bug( "Damage: %d: more than 1000 points!", dam );
	dam = 1000;
    }

    if ( victim != ch )
    {
	/*
	 * Certain attacks are forbidden.
	 * Most other attacks are returned.
	 */
	if ( is_safe( ch, victim ) )
	    return;

	if ( victim->position > POS_STUNNED )
	{
          maybe_set_fighting( victim, ch );
          maybe_set_fighting( ch, victim );
        }

	/* hunting */
	if ( !is_hunting( ch, victim ) )
	{
	    set_hunting( victim, ch, 0, 100 );
	}

	/*
	 * More charm stuff.
	 */
	if ( victim->master == ch )
	    stop_follower( victim );

	/*
	 * Inviso attacks ... not.
	 */
	/*if ( IS_AFFECTED(ch, AFF_INVISIBLE) )
	{
	    affect_strip( ch, gsn_invis );
	    REMOVE_BIT( ch->affected_by, AFF_INVISIBLE );
	    act( "$n fades into existence.", ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIGHT );
	}*/

      /* no reason to do most of this if damage is zero */
      if (dam > 0)
      {
	/*
	 * Damage modifiers.
	 */
	if ( IS_AFFECTED(victim, AFF_SANCTUARY) )
	    dam /= 2;

	if ( IS_AFFECTED(victim, AFF_PROTECT) && IS_EVIL(ch) )
	    dam -= dam / 4;

	if ( dam < 0 )
	    dam = 0;

	/*
	 * Check for disarm, bash, parry, and dodge.
	 * Set up which armor to check.
	 */
	if ( dt >= TYPE_HIT && dt <= (TYPE_HIT+3) )
	{
/* disable this stuff right now, kinda cheese */
/*
	    if ( IS_NPC(ch) && number_percent( ) < ch->level / 2 )
		disarm( ch, victim );
	    if ( IS_NPC(ch) && number_percent( ) < ch->level / 2 )
		bash( ch, victim );
*/
	    ptype = dt - TYPE_HIT;
	}
	else
	{
	    ptype = 0;
	}

	dam = check_armor( dam, ch, victim, ptype, location );

	if ( dam <= 0 )
	{
	    fMessage = FALSE;
	    dam = 0;
	}
      }
    }

  if ( fMessage == TRUE )
  {
    if ( location != -1 && ( dt < 0 || dt > MAX_SKILL ) )
	dam_message_physical( ch, victim, dam, dt, obj, location );
    else
	dam_message( ch, victim, dam, dt, obj );
  }

    /*
     * Hurt the victim.
     * Inform the victim of his new state.
     */
    victim->hit[HIT_LOC_BODY] -= dam;
    if ( !IS_NPC(victim)
    &&   victim->level >= LEVEL_IMMORTAL
    &&   victim->hit[HIT_LOC_BODY] < 1 )
	victim->hit[HIT_LOC_BODY] = 1;
    update_pos( victim );

    switch( victim->position )
    {
    case POS_MORTAL:
	act( "$n is mortally wounded, and will die soon, if not aided.",
	    victim, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIGHT );
	send_to_char( 
	    "You are mortally wounded, and will die soon, if not aided.\n\r",
	    victim );
	break;

    case POS_INCAP:
	act( "$n is incapacitated and will slowly die, if not aided.",
	    victim, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIGHT );
	send_to_char(
	    "You are incapacitated and will slowly die, if not aided.\n\r",
	    victim );
	break;

    case POS_STUNNED:
	act( "$n is stunned, but will probably recover.",
	    victim, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIGHT );
	send_to_char("You are stunned, but will probably recover.\n\r",
	    victim );
	break;

    case POS_DEAD:
	act( "$n is dead! R.I.P.\a", victim, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIGHT );
	send_to_char( "You are dead! R.I.P.\a\n\r\n\r", victim );
	break;

    default:
	if ( dam > victim->max_hit / 4 )
	    send_to_char( "That really did HURT!\n\r", victim );
	if ( victim->hit[HIT_LOC_BODY] < victim->max_hit / 4 )
	    send_to_char( "You sure are BLEEDING!\n\r", victim );
	break;
    }

    /*
     * Sleep spells and extremely wounded folks.
     */
    if ( !IS_AWAKE(victim) )
    {
	if ( combat_next != NULL && victim == combat_next )
	    combat_next = victim->next_fighting;
	stop_fighting( victim, TRUE );
    }

    /*
     * Payoff for killing things.
     */
    if ( victim->position == POS_DEAD )
    {
	group_gain( ch, victim );

	if ( !IS_NPC(victim) )
	{
	    sprintf( log_buf, "%s killed by %s at %d",
		victim->name,
		(IS_NPC(ch) ? ch->short_descr : ch->name),
		victim->in_room->vnum );
	    log_string( log_buf );

	    /*
	     * Dying penalty:
	     */
/* need to add death penalty */
	}

	raw_kill( victim );

	return;
    }

    if ( victim == ch )
	return;

    /*
     * Take care of link dead people.
     */
/* let em die */
    if ( !IS_NPC(victim) && victim->desc == NULL )
    {
    }

    /*
     * Wimp out?
     */
    if ( IS_NPC(victim) && dam > 0 )
    {
	if ( ( IS_SET(victim->act, ACT_WIMPY) && number_bits( 1 ) == 0
	&&   victim->hit[HIT_LOC_BODY] < victim->max_hit / 5 )
	||   ( IS_AFFECTED(victim, AFF_CHARM) && victim->master != NULL
	&&     victim->master->in_room != victim->in_room ) )
	    do_flee( victim, "" );
    }

    tail_chain( );
    return;
}



bool is_safe( CHAR_DATA *ch, CHAR_DATA *victim )
{
/* no one is safe!  ever! */
/*
    if ( IS_NPC(ch) || IS_NPC(victim) )
	return FALSE;
*/

    /* Thx Josh! */
    if ( victim->fighting == ch )
	return FALSE;

    return FALSE;
}



/*
 * Set position of a victim.
 */
void update_pos( CHAR_DATA *victim )
{
    if ( victim->hit[HIT_LOC_BODY] > 0 )
    {
    	if ( victim->position <= POS_STUNNED )
	    victim->position = POS_STANDING;
	return;
    }

    if ( victim->hit[HIT_LOC_BODY] <= -11 )
    {
	victim->position = POS_DEAD;
	return;
    }

         if ( victim->hit[HIT_LOC_BODY] <= -6 ) victim->position = POS_MORTAL;
    else if ( victim->hit[HIT_LOC_BODY] <= -3 ) victim->position = POS_INCAP;
    else                          victim->position = POS_STUNNED;

    return;
}


void maybe_set_fighting( CHAR_DATA *ch, CHAR_DATA *victim )
{
  if ( ch->fighting != NULL )
    return;

  if ( IS_NPC( ch ) )
  {
    if ( ch->desc != NULL
    && ch->desc->original != NULL 
    && IS_SET( ch->desc->original->act, PLR_PEACEFUL ) )
      return;
  }
  else if ( IS_SET( ch->act, PLR_PEACEFUL ) )
    return;

  set_fighting( ch, victim );
}


/*
 * Start fights.
 */
void set_fighting( CHAR_DATA *ch, CHAR_DATA *victim )
{
    if ( ch->fighting != NULL )
    {
	bug( "Set_fighting: already fighting", 0 );
	return;
    }

    clear_dynamic( ch );
    update_noquit( ch );

    if ( IS_AFFECTED(ch, AFF_SLEEP) )
	affect_strip( ch, gsn_sleep );

    ch->fighting = victim;

    ch->next_fighting = combat_list;
    combat_list = ch;

    return;
}



/*
 * Stop fights.
 */
void stop_fighting( CHAR_DATA *ch, bool fBoth )
{
    CHAR_DATA *fch;

    for ( fch = char_list; fch != NULL; fch = fch->next )
    {
	if ( fch == ch )
	{
	    fch->fighting	= NULL;
	    update_pos( fch );
	    continue;
	}

	if ( fBoth && fch->fighting == ch )
	{
	    stop_fighting( fch, FALSE );
	    continue;
	}
    }

  if ( ch == combat_list )
  {
    combat_list = ch->next_fighting;
  }
  else
  {
    for ( fch = combat_list; fch != NULL; fch = fch->next_fighting )
    {
	if ( fch->next_fighting == ch )
	{
	    fch->next_fighting = ch->next_fighting;
	    break;
	}
    }
  }

    ch->next_fighting = NULL;

    return;
}



/*
 * Make a corpse out of a character.
 */
void make_corpse( CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *corpse;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    MOB_INDEX_DATA *pMobIndex;
    char *name;

    making_corpse = TRUE;

    {
	name		= ch->short_descr;
	corpse		= create_object(get_obj_index(OBJ_VNUM_CORPSE));
	corpse->timer	= number_range( 25, 40 );
    }

    if ( IS_NPC( ch ) && ( pMobIndex = ch->pIndexData ) != NULL )
    {
	corpse->value[0] = pMobIndex->skin_objects[0];
	corpse->value[1] = pMobIndex->skin_objects[1];
	corpse->value[2] = pMobIndex->skin_objects[2];
	corpse->value[3] = pMobIndex->skin_objects[3];
    }

    sprintf( buf, corpse->short_descr, name );
    free_string( corpse->short_descr, MEM_OBJ_SHORT_DESCR );
    corpse->short_descr = str_dup( buf );

    sprintf( buf, corpse->look_descr, name );
    free_string( corpse->look_descr, MEM_OBJ_LOOK_DESCR );
    corpse->look_descr = str_dup( buf );

    for ( obj = ch->carrying; obj != NULL; obj = obj_next )
    {
	obj_next = obj->next_content;
	obj_from_char( obj );
	obj_to_obj( obj, corpse );
    }

    obj_to_room( corpse, ch->in_room, TRUE );

    making_corpse = FALSE;

    return;
}



void death_cry( CHAR_DATA *ch )
{
    ROOM_INDEX_DATA *was_in_room;
    int door;

    act( "Your blood freezes as you hear $n's death cry.", ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_HEARING );

    was_in_room = ch->in_room;
    for ( door = 0; door <= 5; door++ )
    {
	EXIT_DATA *pexit;

	if ( ( pexit = was_in_room->exit[door] ) != NULL
	&&   pexit->to_room != NULL
	&&   pexit->to_room != was_in_room )
	{
	    ch->in_room = pexit->to_room;
	    act( "Your blood freezes as you hear someone's death cry.", ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_HEARING );
	}
    }
    ch->in_room = was_in_room;

    return;
}



void raw_kill( CHAR_DATA *victim )
{
    stop_fighting( victim, TRUE );
    death_cry( victim );
    make_corpse( victim );

    if ( IS_NPC(victim) )
    {
	extract_char( victim, TRUE );
	return;
    }

    extract_char( victim, FALSE );
    while ( victim->affected )
	affect_remove( victim, victim->affected );
    victim->affected_by	= 0;
    victim->position	= POS_RESTING;
    victim->hit[HIT_LOC_BODY]		= UMAX( 1, victim->hit[HIT_LOC_BODY]  );
    victim->mana	= UMAX( 1, victim->mana );
    victim->move	= UMAX( 1, victim->move );
    save_char_obj( victim );

    if (victim->desc == NULL)
	do_quit(victim,"");

    return;
}



void group_gain( CHAR_DATA *ch, CHAR_DATA *victim )
{
    CHAR_DATA *gch;
    CHAR_DATA *lch;

    /*
     * Monsters don't get kill xp's or alignment changes.
     * Dying of mortal wounds or poison doesn't give xp to anyone!
     */
    if ( IS_NPC(ch) || victim == ch )
	return;
    
    lch = (ch->leader != NULL) ? ch->leader : ch;

    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
	OBJ_DATA *obj;
	OBJ_DATA *obj_next;

	if ( !is_same_group( gch, ch ) )
	    continue;

	for ( obj = ch->carrying; obj != NULL; obj = obj_next )
	{
	    obj_next = obj->next_content;
	    if ( obj->wear_loc == gn_wear_none )
		continue;

/*  there is no good or evil!  */
/*
	    if ( ( IS_OBJ_STAT(obj, ITEM_ANTI_EVIL)    && IS_EVIL(ch)    )
	    ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_GOOD)    && IS_GOOD(ch)    )
	    ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_NEUTRAL) && IS_NEUTRAL(ch) ) )
	    {
		act( "You are zapped by $p.", ch, NULL, obj, NULL, NULL, TO_CHAR, SENSE_SIXTH );
		act( "$n is zapped by $p.",   ch, NULL, obj, NULL, NULL, TO_ROOM, SENSE_SIGHT );
		obj_from_char( obj );
		obj_to_room( obj, ch->in_room, TRUE );
	    }
*/
	}
    }

    return;
}


void dam_message( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, OBJ_DATA *obj )
{
    char buf1[256], buf2[256], buf3[256];

    if ( dam < 0 )
	dam = 0;

    if ( dt == TYPE_HIT )
	dt = TYPE_HIT + ATTACK_HIT;

  if ( dt >= 0 && dt < MAX_SKILL )
  {
    if (dam > (victim->hit[HIT_LOC_BODY] + 10))
    {
	strcpy( buf2, skill_table[dt].msg_death_char );
      if ( ch != victim )
	strcpy( buf3, skill_table[dt].msg_death_targ );
	strcpy( buf1, skill_table[dt].msg_death_room );
    }
    else if (dam <= 0)
    {
	strcpy( buf2, skill_table[dt].msg_miss_char );
      if ( ch != victim )
	strcpy( buf3, skill_table[dt].msg_miss_targ );
	strcpy( buf1, skill_table[dt].msg_miss_room );
    }
    else
    {
	strcpy( buf2, skill_table[dt].msg_damage_char );
      if ( ch != victim )
	strcpy( buf3, skill_table[dt].msg_damage_targ );
	strcpy( buf1, skill_table[dt].msg_damage_room );
    }
  }
  else
  {
    sprintf( buf1, "$n hits $B$N$b." );
    sprintf( buf2, "You hit $B$N$b." );
    sprintf( buf3, "$n hits $Byou$b." );
  }

/* we are assuming we were called with a valid object */
    act( buf1, ch, victim, obj, NULL, NULL, TO_NOTVICT, SENSE_SIGHT );
    act( buf2, ch, victim, obj, NULL, NULL, TO_CHAR, SENSE_SIGHT|SENSE_TOUCH );
  if ( ch != victim )
    act( buf3, ch, victim, obj, NULL, NULL, TO_VICT, SENSE_SIGHT|SENSE_TOUCH );

    return;
}


void dam_message_physical( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, OBJ_DATA *obj, int location )
{
    char buf1[256], buf2[256], buf3[256];
    struct damage_mess *pMess;
    char *loc_string;

    if ( dam < 0 )
	dam = 0;

    if ( dt == TYPE_HIT )
	dt = TYPE_HIT + ATTACK_HIT;

    if ( dam > (victim->hit[HIT_LOC_BODY] + 10) )
    {
	pMess = death_table[dt-TYPE_HIT];
    }
    else
    {
	pMess = damage_table[dt-TYPE_HIT];
    }

    for ( ; pMess != NULL ; pMess = pMess->next )
    {
	if ( pMess->min_damage <= dam )
	    break;
    }

    loc_string = race_array[victim->race]->hit_loc_string[location];

    if ( pMess->message_room != NULL )
	sprintf( buf1, pMess->message_room, loc_string );
    else
	sprintf( buf1, "$n hits $B$N$b on the %s.", loc_string );

    if ( pMess->message_char != NULL )
	sprintf( buf2, pMess->message_char, loc_string );
    else
	sprintf( buf2, "You hit $B$N$b on the %s.", loc_string );

    if ( pMess->message_victim != NULL )
	sprintf( buf3, pMess->message_victim, loc_string );
    else
	sprintf( buf3, "$n hits $Byou$b on the %s.", loc_string );

/* we are assuming we were called with a valid object */
    act( buf1, ch, victim, obj, NULL, NULL, TO_NOTVICT, SENSE_SIGHT );
    act( buf2, ch, victim, obj, NULL, NULL, TO_CHAR, SENSE_SIGHT|SENSE_TOUCH );
  if ( ch != victim )
    act( buf3, ch, victim, obj, NULL, NULL, TO_VICT, SENSE_SIGHT|SENSE_TOUCH );

    return;
}


/*
 * Disarm a creature.
 * Caller must check for successful attack.
 */
void disarm( CHAR_DATA *ch, CHAR_DATA *victim )
{
    OBJ_DATA *obj;

    if ( ( obj = get_eq_char( victim, gn_wear_primary ) ) == NULL )
	return;

    if ( get_eq_char( ch, gn_wear_primary ) == NULL && number_bits( 1 ) == 0 )
	return;

    act( "$n disarms you!", ch, victim, NULL, NULL, NULL, TO_VICT, SENSE_SIGHT|SENSE_TOUCH );
    act( "You disarm $N!",  ch, victim, NULL, NULL, NULL, TO_CHAR, SENSE_SIGHT|SENSE_TOUCH );
    act( "$n disarms $N!",  ch, victim, NULL, NULL, NULL, TO_NOTVICT, SENSE_SIGHT );

/* need to make mobs smart enough to get their weapon */
    obj_from_char( obj );
    obj_to_room( obj, victim->in_room, TRUE );

    return;
}



/*
 * Bash a creature.
 * Caller must check for successful attack.
 */
void bash( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int delay;

	damage( ch, victim, 1, gsn_bash, NULL, gn_wear_none );

/*
	delay = 2 * PULSE_VIOLENCE * get_power_level( ch, gsn_bash ) / get_curr_str( victim );
*/
/* based on 6 second 100 % bash.   6 * 4 */
	delay = 24 * get_power_level( ch, gsn_bash ) / get_curr_str( victim );

	victim->position = POS_RESTING;

	WAIT_STATE( victim, delay );
	WAIT_STATE( ch, skill_table[gsn_bash].beats );
	victim->position = POS_RESTING;

    return;
}


void do_kill( CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];
  CHAR_DATA *victim;

  one_argument( argument, arg );

  if ( arg[0] == '\0' )
  {
    send_to_char( "Kill whom?\n\r", ch );
    return;
  }

  if ( ( victim = get_char_room( ch, arg ) ) == NULL )
  {
    send_to_char( "They aren't here.\n\r", ch );
    return;
  }

  if ( victim == ch )
  {
    send_to_char( "You hit yourself.  Ouch!\n\r", ch );
    return;
  }

  if ( is_safe( ch, victim ) )
    return;

  if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim )
  {
    act( "$N is your beloved master.",
      ch, victim, NULL, NULL, NULL, TO_CHAR, SENSE_SIXTH );
      return;
  }

  if ( ch->fighting != NULL )
  {
    if ( ch->fighting == victim )
    {
      act( "You're already fighting $M.",
        ch, victim, NULL, NULL, NULL, TO_CHAR, SENSE_SIXTH );
      return;
    }

    ch->fighting = victim;

    act( "You focus your attacks on $N.",
      ch, victim, NULL, NULL, NULL, TO_CHAR, SENSE_SIXTH );
    act( "$n focuses $s attacks on you.",
      ch, victim, NULL, NULL, NULL, TO_VICT, SENSE_SIGHT );
    act( "$n focuses $s attacks on $N.",
      ch, victim, NULL, NULL, NULL, TO_NOTVICT, SENSE_SIGHT );

    return;
  }

  WAIT_STATE( ch, 1 * PULSE_VIOLENCE );
  one_hit( ch, victim, TYPE_UNDEFINED );
  return;
}



void do_backstab( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Backstab whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "How can you sneak up on yourself?\n\r", ch );
	return;
    }

    if ( is_safe( ch, victim ) )
      return;

    if ( ( obj = get_eq_char( ch, gn_wear_primary ) ) == NULL
    ||   obj->item_type != ITEM_WEAPON
    ||   !IS_SET( obj->value[3], WEAPON_BIT_BACKSTAB ) )
    {
	send_to_char( "You need to wield a piercing weapon.\n\r", ch );
	return;
    }

    if ( check_fighting( victim ) )
    {
	send_to_char( "You can't backstab a fighting person.\n\r", ch );
	return;
    }

    WAIT_STATE( ch, skill_table[gsn_backstab].beats );
    if ( !IS_AWAKE(victim)
    ||   number_percent( ) < get_skill( ch, gsn_backstab) )
    {
	one_hit( ch, victim, gsn_backstab );
    }
    else
    {
	damage( ch, victim, 0, gsn_backstab, obj, gn_wear_none );
	skill_improve( ch, gsn_backstab );
    }

    return;
}



void do_bash( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if ( arg[0] == '\0' && ch->fighting == NULL )
    {
	send_to_char( "Bash whom?\n\r", ch );
	return;
    }

    if ( arg[0] == '\0' )
    {
	victim = ch->fighting;
    }
    else if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "That would be kind of stupid.\n\r", ch );
	return;
    }

    if ( is_safe( ch, victim ) )
      return;

    if ( ( obj = get_eq_char( ch, gn_wear_primary ) ) == NULL )
    {
	send_to_char( "You need to wield a weapon.\n\r", ch );
	return;
    }

    if ( number_percent( ) < get_skill( ch, gsn_bash) )
    {
	bash( ch, victim );

	if ( get_skill( ch, gsn_bash ) < ch->perm_stat_phys_str )
	    power_improve( ch, gsn_bash );
    }
    else
    {
	ch->position = POS_RESTING;
	damage( ch, victim, 0, gsn_bash, NULL, gn_wear_none );
	skill_improve( ch, gsn_bash );
    }
    WAIT_STATE( ch, skill_table[gsn_bash].beats );

    return;
}



void do_flee( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *was_in;
    ROOM_INDEX_DATA *now_in;
    int attempt;

    if ( ch->position < POS_STANDING )
    {
	send_to_char("You need to get to your feet first.\n\r", ch );
	return;
    }

    was_in = ch->in_room;
    for ( attempt = 0; attempt < 6; attempt++ )
    {
	EXIT_DATA *pexit;
	int door;

	door = number_door( );
	if ( ( pexit = was_in->exit[door] ) == 0
	||   pexit->to_room == NULL
	||   IS_SET(pexit->exit_flags, EX_CLOSED) )
	    continue;

	move_char( ch, door );
	if ( ( now_in = ch->in_room ) == was_in )
	    continue;

	ch->in_room = was_in;
	act( "$n panics and attempts to flee!", ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIXTH );
	ch->in_room = now_in;

	stop_fighting( ch, TRUE );
	return;
    }

    send_to_char("PANIC! You couldn't escape!\n\r", ch );

    return;
}



void do_rescue( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *fch;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Rescue whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "What about fleeing instead?\n\r", ch );
	return;
    }

    if ( ch->fighting == victim )
    {
	send_to_char( "Too late.\n\r", ch );
	return;
    }

    if ( ( fch = victim->fighting ) == NULL )
    {
	send_to_char( "That person is not fighting right now.\n\r", ch );
	return;
    }

    WAIT_STATE( ch, skill_table[gsn_rescue].beats );
    if ( number_percent( ) > get_skill( ch, gsn_rescue) )
    {
	send_to_char( "You fail the rescue.\n\r", ch );
	skill_improve(ch,gsn_rescue);
	return;
    }

    act( "You rush in and push $N to the side!", ch, victim, NULL, NULL, NULL, TO_CHAR, SENSE_SIXTH );
    act( "$n rushes in and moves you to the side!", ch, victim, NULL, NULL, NULL, TO_VICT, SENSE_SIGHT|SENSE_HEARING );
    act( "$n rushes in and moves $N to the side!",  ch, victim, NULL, NULL, NULL, TO_NOTVICT, SENSE_SIGHT );

    stop_fighting( fch, FALSE );
    stop_fighting( victim, FALSE );

    set_fighting( ch, fch );
    set_fighting( fch, ch );
    return;
}



void do_kick( CHAR_DATA *ch, char *argument )
{
	char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' && ch->fighting == NULL )
    {
	send_to_char( "Kick whom?\n\r", ch );
	return;
    }

    if ( arg[0] == '\0' )
    {
	victim = ch->fighting;
    }
    else if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "You step on your foot, ooof!\n\r", ch );
	return;
    }

    if ( is_safe( ch, victim ) )
      return;

    WAIT_STATE( ch, skill_table[gsn_kick].beats );
    if ( number_percent( ) < get_skill( ch, gsn_kick) )
    {
	damage( ch, victim, number_range( 1, get_power_level( ch, gsn_kick )/3 ), gsn_kick, NULL, gn_wear_none );
	power_improve( ch, gsn_kick );
    }
    else
    {
	damage( ch, victim, 0, gsn_kick, NULL, gn_wear_none );
	skill_improve( ch, gsn_kick );
    }

    return;
}




void do_disarm( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    int percent;

    if ( get_eq_char( ch, gn_wear_primary ) == NULL )
    {
	send_to_char( "You must wield a weapon to disarm.\n\r", ch );
	return;
    }

    if ( ( victim = ch->fighting ) == NULL )
    {
	send_to_char( "You aren't fighting anyone.\n\r", ch );
	return;
    }

    if ( ( obj = get_eq_char( victim, gn_wear_primary ) ) == NULL )
    {
	send_to_char( "Your opponent is not wielding a weapon.\n\r", ch );
	return;
    }

    WAIT_STATE( ch, skill_table[gsn_disarm].beats );
    percent = number_percent( ) + get_curr_str(victim) - get_curr_str(ch);
    if ( percent < get_skill( ch, gsn_disarm ) * 2 / 3 )
	disarm( ch, victim );
    else
    {
	act("$n attempted to disarm you.", ch, ch->fighting, NULL, NULL, NULL, TO_VICT, SENSE_SIGHT );
	act("$N maintained their grip on their weapon.", ch, ch->fighting, NULL, NULL, NULL, TO_CHAR, SENSE_SIGHT|SENSE_TOUCH );
	skill_improve( ch, gsn_disarm );
    }

    return;
}



void do_sla( CHAR_DATA *ch, char *argument )
{
    send_to_char( "If you want to SLAY, spell it out.\n\r", ch );
    return;
}



void do_slay( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Slay whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "Suicide is a mortal sin.\n\r", ch );
	return;
    }

    if ( !IS_NPC(victim) && victim->level >= ch->level )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    act( "You slay $M in cold blood!",  ch, victim, NULL, NULL, NULL, TO_CHAR, SENSE_SIXTH );
    act( "$n slays you in cold blood!", ch, victim, NULL, NULL, NULL, TO_VICT, SENSE_SIXTH );
    act( "$n slays $N in cold blood!",  ch, victim, NULL, NULL, NULL, TO_NOTVICT, SENSE_SIGHT );
    raw_kill( victim );
    return;
}


int check_fighting( CHAR_DATA *ch )
{
  int i = 0;
  CHAR_DATA *vch;

  if ( ch->in_room == NULL )
    return 0;

  for ( vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room )
  {
    if ( vch == ch )
      continue;

    if ( vch->fighting == ch )
      i++;
  }

  if ( i == 0 && ch->fighting != NULL )
    i++;

  return i;
}


int check_armor( int dam, CHAR_DATA *ch, CHAR_DATA *victim, int ptype, int hit_loc )
{
    OBJ_DATA *armor, *a_next;
    OBJ_DATA *obj;
    int total;
    int odam = dam;

    total = victim->absorb[ptype] + victim->general_absorb;

    armor = get_eq_char_hit_loc( victim, hit_loc );

    if ( armor != NULL )
	act( "Found $p worn on you.", ch, NULL, armor, NULL, NULL, TO_CHAR, SENSE_SIXTH );

    for ( obj = armor; obj != NULL; obj = obj->next_worn )
    {
	if ( obj->item_type != ITEM_ARMOR )
	    continue;

	if ( armor->item_type != ITEM_ARMOR
	|| obj->value[ptype] > armor->value[ptype] )
	    armor = obj;
    }

    if ( armor != NULL && armor->item_type == ITEM_ARMOR )
	total += armor->value[ptype];

    if ( total == 0 )
	return dam;

    /*
     * armor shouldn't be 100% effective, of course.
     */
    total = dice( 1, total );
    dam = UMAX( 0, dam - total );

    /* armor shown when message goes off should not be the same as
     * the one that does the work, it should be the top one.
     */
    armor = get_eq_char_hit_loc( victim, hit_loc );
    if ( dam == 0 && armor != NULL )
    {
	act("$n's blow is absorbed by $N's $p.", ch, victim, armor, NULL, NULL, TO_NOTVICT, SENSE_SIGHT );
	act("$n's blow is absorbed by your $p.", ch, victim, armor, NULL, NULL, TO_VICT, SENSE_SIGHT|SENSE_TOUCH );
	act("Your blow is absorbed by $N's $p.", ch, victim, armor, NULL, NULL, TO_CHAR, SENSE_SIGHT|SENSE_TOUCH );
    }
    else if ( dam == 0 )
    {
	act("$n's blow is absorbed by $N's $t.", ch, victim, armor,
	    race_array[victim->race]->skin, NULL, TO_NOTVICT, SENSE_SIGHT );
	act("$n's blow is absorbed by your $t.", ch, victim, armor,
	    race_array[victim->race]->skin, NULL, TO_VICT, SENSE_SIGHT|SENSE_TOUCH );
	act("Your blow is absorbed by $N's $t.", ch, victim, armor,
	    race_array[victim->race]->skin, NULL, TO_CHAR, SENSE_SIGHT|SENSE_TOUCH );
    }

    for ( ; armor != NULL; armor = a_next )
    {
	a_next = armor->next_worn;

	if ( a_next )
	    odam /= 2;

	damage_item( armor, odam, TRUE, TRUE );
    }

    return dam;
}


int get_hit_location( CHAR_DATA *ch, CHAR_DATA *victim, int ptype )
{
    int h;
    int i;
    int n;
    int percent;
    int *row;

    h = relative_height( ch, victim );

    switch( ptype )
    {
	case 1:
	default:
	    row = &(race_array[victim->race]->hit_loc_slash[h][0]);
	    break;
	case 2:
	    row = &(race_array[victim->race]->hit_loc_bludgeon[h][0]);
	    break;
	case 3:
	    row = &(race_array[victim->race]->hit_loc_pierce[h][0]);
	    break;
    }

    percent = number_percent();

    for ( i = 0; i <= 9; i++ )
    {
	n = row[i];

	if ( percent <= n )
	    break;

	percent -= n;
    }

    return i;
}


void update_noquit( CHAR_DATA *ch )
{
    AFFECT_DATA af;

    af.type = gsn_noquit;
    if ( is_affected( ch, gsn_noquit ) )
	af.duration = 480;   /* 2 minutes */
    else
	af.duration = 2400;   /* 10 minutes */
    af.location = APPLY_NONE;
    af.modifier = 0;
    af.bitvector = 0;
    affect_join( ch, &af );
}
