/*
 * shoot.c, May 3,4 1996 (mostly) by Robert L. Peckham (rip)
 * for Sword Quest MURPE.
 */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "merc.h"

extern char * const dir_name[];
extern char * const dir_name_2[];
extern sh_int const rev_dir[];

void do_shoot( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    CHAR_DATA *vch = NULL;
    OBJ_DATA *bow;                /* or crossbow, or sling, etc */
    OBJ_DATA *arrow;
    ROOM_INDEX_DATA *in_room = ch->in_room;
    ROOM_INDEX_DATA *target_room;
    char arg1[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    int direction;
    int ii;

    if ( ( bow = get_eq_char( ch, gn_wear_secondary ) ) == NULL
	|| bow->item_type != ITEM_RANGED_WEAPON )
    {
	send_to_char("You aren't holding a ranged weapon.\n\r", ch );
	return;
    }

    if ( ( arrow = get_eq_char( ch, gn_wear_primary ) ) == NULL
	|| arrow->item_type != ITEM_AMMUNITION )
    {
	send_to_char("What do you intend on firing?\n\r", ch );
	return;
    }

    /* check ammunition type */

    argument = one_argument( argument, arg1 );

    if ( arg1 == NULL || arg1[0] == '\0' )
    {
	send_to_char("Who do you want to shoot at?\n\r", ch );
	return;
    }

    if ( (direction = get_direction( argument ) ) == -1 )
    {
	if ( argument != NULL && argument[0] != '\0' )
	{
	    send_to_char("What direction is that?\n\r", ch );
	    return;
	}

	if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
	{
	    for ( ii = 0; ii < 6; ii++ )
	    {
/* need to change range 5 to based on player's vision */
		if ( (vch = search_dir_name( ch, arg1, ii, 5 ) ) != NULL )
		{
		    if ( direction != -1 )
		    {
			send_to_char("Multiple targets like that, which direction?\n\r", ch );
			return;
		    }
		    victim = vch;
		    direction = ii;
		}
	    }
	}
    }
    else
    {
	victim = search_dir_name( ch, arg1, direction, 5 );
    }

    if ( victim == NULL )
    {
	send_to_char("You don't see anyone like that around.\n\r", ch );
	return;
    }

/* we have a victim and a direction.  start the missile off. */

    if ( number_percent() > (get_skill( ch, gsn_archery ) + (get_curr_dex( ch ) / 2)))
    {
	if ( number_percent() == 1 )
	{
	    send_to_char("You destroy your weapon as you attempt to fire it.\n\r", ch );
	    act("$n's $p is destroyed as $e attempts to fire it.", ch, NULL, bow, NULL, NULL, TO_ROOM, SENSE_SIGHT );
	    unequip_char( ch, bow );
	    obj_from_char( bow );
	    extract_obj( bow, TRUE );
	}
	else
	{
	    send_to_char("You fumble your weapon as you attempt to fire it.\n\r", ch );
	    act("$n attempts to fire $p, but fumbles.", ch, NULL, bow, NULL, NULL, TO_ROOM, SENSE_SIGHT );

	    unequip_char( ch, arrow );
	    obj_from_char( arrow );
	    obj_to_room( arrow, ch->in_room, TRUE );
	}

	skill_improve( ch, gsn_archery );

	return;
    }

    unequip_char( ch, arrow );
    obj_from_char( arrow );

    if ( ch->in_room == victim->in_room )
    {
	act("$n turns and fires $s $p at you!", ch, victim, bow, NULL, NULL, TO_VICT, SENSE_SIGHT );
	act("$n turns and fires $s $p at $N!", ch, victim, bow, NULL, NULL, TO_NOTVICT, SENSE_SIGHT );
	act("You turn and fire your $p at $N.", ch, victim, bow, NULL, NULL, TO_CHAR, SENSE_SIXTH );

	damage_from_object( victim, arrow, 1, gsn_archery );
	obj_to_char( arrow, victim );
	damage_item( arrow, 1, FALSE, FALSE );
/* piss off victim */
	return;
    }


    /* ranged firing section */
    {
	act( "You fire $p $twards.", ch, NULL, bow, dir_name[direction], NULL, TO_CHAR, SENSE_SIXTH );
	act( "$n fires $s $p $twards.", ch, NULL, bow, dir_name[direction], NULL, TO_ROOM, SENSE_SIGHT );

	target_room = ch->in_room->exit[direction]->to_room;

	sprintf( buf, "$p flies across the room %swards.", dir_name[direction] );

	/* send the arrow on it's course */
	/* we know the victim is on the path since it was searched above */
	/* test skill in each room */
	while ( 1 )
	{
	    target_room = ch->in_room->exit[direction]->to_room;
	    char_from_room( ch );
	    char_to_room( ch, target_room, FALSE );

	    if ( target_room == victim->in_room )
		break;

	    if ( number_percent() >
		( get_skill( ch, gsn_archery ) + get_curr_dex(ch)/5) )
	    {
		send_to_char( "Your shot fell short of it's target.\n\r", ch );
		act( "$p flies in from $t and hits the ground.",
		    ch, NULL, arrow, dir_name_2[rev_dir[direction]], NULL, TO_ROOM, SENSE_SIGHT );
		obj_to_room( arrow, target_room, TRUE );
		damage_item( arrow, 1, FALSE, FALSE );

		char_from_room( ch );
		char_to_room( ch, in_room, FALSE );
		return;
	    }

	    act( buf, ch, NULL, arrow, NULL, NULL, TO_ROOM, SENSE_SIGHT );
	}

	act( "$p flies in from $t...",
	    ch, NULL, arrow, dir_name_2[rev_dir[direction]], NULL, TO_ROOM, SENSE_SIGHT );

	send_to_char( "Your shot had enough range...\n\r", ch );

	/* enough distance, check aim */
	if ( number_percent() >
	    ( get_skill ( ch, gsn_archery ) + get_curr_dex(ch)/5 ) )
	{

	    obj_to_room( arrow, target_room, TRUE );
	    /* generate miss messages */
	    damage_from_object( victim, arrow, 0, gsn_archery );
	    damage_item( arrow, 1, FALSE, FALSE );

	    skill_improve( ch, gsn_archery );

	    char_from_room( ch );
	    char_to_room( ch, in_room, FALSE );
	    return;
	}

	/* do damage and generate messages */
	damage_from_object( victim, arrow, 1, gsn_archery );
	obj_to_char( arrow, victim );
	damage_item( arrow, 1, FALSE, FALSE );

	power_improve( ch, gsn_archery );

	char_from_room( ch );
	char_to_room( ch, in_room, FALSE );
    }

    return;
}

CHAR_DATA *search_dir_name( CHAR_DATA *ch, char *argument, int direction, int range )
{
    CHAR_DATA *victim = NULL;
    ROOM_INDEX_DATA *in_room = ch->in_room;
    ROOM_INDEX_DATA *search_room = ch->in_room;
    EXIT_DATA *pexit;
    int irange;

    if ( direction < 0 || direction > 5 )
	return NULL;

    if ( argument == NULL || argument[0] == '\0' )
	return NULL;

    for ( irange = 1; irange <= range; irange++ )
    {
	if ( ( pexit = search_room->exit[direction] ) == NULL )
	    break;

	if ( ( search_room = pexit->to_room ) == NULL )
	    break;

	if ( IS_SET( pexit->exit_flags, EX_CLOSED ) )
	    break;

	char_from_room( ch );
	char_to_room( ch, search_room, FALSE );

	if ( ( victim = get_char_room( ch, argument ) ) != NULL )
	    break;
    }

    char_from_room( ch );
    char_to_room( ch, in_room, FALSE );
    return victim;
}
