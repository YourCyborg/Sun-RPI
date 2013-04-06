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
#include "flora.h"



/*
 * Local functions.
 */
void	get_obj		args( ( CHAR_DATA *ch, OBJ_DATA *obj,
			    OBJ_DATA *container ) );
int	get_wear_loc	args( ( OBJ_DATA *obj, char *argument ) );
bool	remove_obj	args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );
int	sum_worn	args( ( CHAR_DATA *ch, int location ) );
void	wear_obj	args( ( CHAR_DATA *ch, OBJ_DATA *obj, int location ) );




int wear_loc_lookup( char *argument )
{
    extern struct wear_data const wear_info[];
    int i;

    for ( i = 0; i < MAX_WEAR; i++ )
	if ( !str_cmp( argument, wear_info[i].name ) )
	    return i;

    return -1;
}

void get_obj( CHAR_DATA *ch, OBJ_DATA *obj, OBJ_DATA *container )
{
    if (IS_SET(obj->extra_flags, ITEM_NOTAKE))
    {
	send_to_char( "You can't take that.\n\r", ch );
	return;
    }

    if ( ch->carry_number + 1 > can_carry_n( ch ) )
    {
	act( "$d: you can't carry that many items.",
	    ch, NULL, NULL, obj->name, NULL, TO_CHAR, SENSE_SIXTH );
	return;
    }

    if ( ch->carry_weight + get_obj_weight( obj ) > can_carry_w( ch ) )
    {
	act( "$d: you can't carry that much weight.",
	    ch, NULL, NULL, obj->name, NULL, TO_CHAR, SENSE_SIXTH );
	return;
    }

    if ( container != NULL )
    {
	act( "You get $p from $P.", ch, NULL, obj, container, NULL, TO_CHAR, SENSE_SIXTH );
	act( "$n gets $p from $P.", ch, NULL, obj, container, NULL, TO_ROOM, SENSE_SIGHT );
	obj_from_obj( obj );
    }
    else
    {
	act( "You get $p.", ch, NULL, obj, container, NULL, TO_CHAR, SENSE_SIXTH );
	act( "$n gets $p.", ch, NULL, obj, container, NULL, TO_ROOM, SENSE_SIGHT );
	obj_from_room( obj );
    }

    if ( obj->item_type == ITEM_MONEY )
    {
	gain_coin( ch, obj->value[0], obj->value[1], FALSE, NULL );

	extract_obj( obj, TRUE );

	return;
    }

    obj_to_char( obj, ch );

    return;
}



void do_get( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    OBJ_DATA *container;
    bool found;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    /* Get type. */
    if ( arg1[0] == '\0' )
    {
	send_to_char( "Get what?\n\r", ch );
	return;
    }

    if ( arg2[0] == '\0' )
    {
	if ( str_cmp( arg1, "all" ) && str_prefix( "all.", arg1 ) )
	{
	    /* 'get obj' */
	    obj = get_obj_list( ch, arg1, ch->in_room->contents );
	    if ( obj == NULL )
	    {
		act( "I see no $t here.", ch, NULL, NULL, arg1, NULL, TO_CHAR, SENSE_SIXTH );
		return;
	    }

	    get_obj( ch, obj, NULL );
	}
	else
	{
	    /* 'get all' or 'get all.obj' */
	    found = FALSE;
	    for ( obj = ch->in_room->contents; obj != NULL; obj = obj_next )
	    {
		obj_next = obj->next_content;
		if ( ( arg1[3] == '\0' || is_name( &arg1[4], obj->name ) )
		&&   can_see_obj( ch, obj ) )
		{
		    found = TRUE;
		    get_obj( ch, obj, NULL );
		}
	    }

	    if ( !found ) 
	    {
		if ( arg1[3] == '\0' )
		    send_to_char( "I see nothing here.\n\r", ch );
		else
		    act( "I see no $t here.", ch, NULL, NULL, &arg1[4], NULL, TO_CHAR, SENSE_SIXTH );
	    }
	}
    }
    else
    {
	/* 'get ... container' */
	if ( !str_cmp( arg2, "all" ) || !str_prefix( "all.", arg2 ) )
	{
	    send_to_char( "You can't do that.\n\r", ch );
	    return;
	}

	if ( ( container = get_obj_here( ch, arg2 ) ) == NULL )
	{
	    act( "I see no $t here.", ch, NULL, NULL, arg2, NULL, TO_CHAR, SENSE_SIXTH );
	    return;
	}

	if (  container->capacity <= 0
	&& container->contains == NULL )
	{
	    send_to_char( "That's not a container.\n\r", ch );
	    return;
	}

	if ( container->item_type == ITEM_CONTAINER
	&& IS_SET(container->value[1], CONT_CLOSED) )
	{
	    act( "The $d is closed.", ch, NULL, NULL, container->name, NULL, TO_CHAR, SENSE_SIXTH );
	    return;
	}

	if ( str_cmp( arg1, "all" ) && str_prefix( "all.", arg1 ) )
	{
	    /* 'get obj container' */
	    obj = get_obj_list( ch, arg1, container->contains );
	    if ( obj == NULL )
	    {
		act( "I see nothing like that in the $t.",
		    ch, NULL, NULL, arg2, NULL, TO_CHAR, SENSE_SIXTH );
		return;
	    }
	    get_obj( ch, obj, container );
	}
	else
	{
	    /* 'get all container' or 'get all.obj container' */
	    found = FALSE;
	    for ( obj = container->contains; obj != NULL; obj = obj_next )
	    {
		obj_next = obj->next_content;
		if ( ( arg1[3] == '\0' || is_name( &arg1[4], obj->name ) )
		&&   can_see_obj( ch, obj ) )
		{
		    found = TRUE;
		    get_obj( ch, obj, container );
		}
	    }

	    if ( !found )
	    {
		if ( arg1[3] == '\0' )
		    act( "I see nothing in the $t.",
			ch, NULL, NULL, arg2, NULL, TO_CHAR, SENSE_SIXTH );
		else
		    act( "I see nothing like that in the $T.",
			ch, NULL, NULL, arg2, NULL, TO_CHAR, SENSE_SIXTH );
	    }
	}
    }

    return;
}



void do_put( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *container;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    int amount = -1;

    argument = one_argument( argument, arg1 );
    if ( is_number( arg1 ) )
    {
	amount = atoi( arg1 );
	if ( amount < 1 )
	{
	    send_to_char( "How can you put that many?n\r", ch );
	    return;
	}

	argument = one_argument( argument, arg1 );
    }

    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Put what in what?\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "all" ) || !str_prefix( "all.", arg2 ) )
    {
	send_to_char( "You can't do that.\n\r", ch );
	return;
    }

    if ( ( container = get_obj_here( ch, arg2 ) ) == NULL )
    {
	act( "I see no $t here.", ch, NULL, NULL, arg2, NULL,
	    TO_CHAR, SENSE_SIXTH );
	return;
    }

    if ( container->capacity <= 0 )
    {
	send_to_char( "That's not a container.\n\r", ch );
	return;
    }

    if ( container->item_type == ITEM_CONTAINER
    && IS_SET(container->value[1], CONT_CLOSED) )
    {
	act( "The $d is closed.", ch, NULL, NULL, container->name,
	    NULL, TO_CHAR, SENSE_SIXTH );
	return;
    }

    if ( str_cmp( arg1, "all" ) && str_prefix( "all.", arg1 ) )
    {
	/* 'put obj container' */
	if ( ( obj = get_obj_carry( ch, arg1, ch ) ) == NULL )
	{
	    send_to_char( "You do not have that item.\n\r", ch );
	    return;
	}

	if ( obj == container )
	{
	    send_to_char( "You can't fold it into itself.\n\r", ch );
	    return;
	}

	if ( !can_drop_obj( ch, obj ) )
	{
	    send_to_char( "You can't let go of it.\n\r", ch );
	    return;
	}

	if ( obj->item_type == ITEM_MONEY && amount > -1 )
	{
	  if ( amount > obj->value[0] )
	  {
	      send_to_char( "You don't have that many.\n\r", ch );
	      return;
	  }

	  if ( get_coin_weight( amount, obj->value[1] )
		+ get_obj_weight( container ) > container->capacity )
	  {
	    send_to_char( "It won't fit.\n\r", ch );
	    return;
	  }

	  lose_coin( ch, amount, obj->value[1], FALSE, NULL );
	  obj = create_money( amount, obj->value[1] );
	}
	else
	{
	  if ( get_obj_weight( obj ) + get_obj_weight( container )
	       > container->capacity )
	  {
	    send_to_char( "It won't fit.\n\r", ch );
	    return;
	  }

	  obj_from_char( obj );
	}

	obj_to_obj( obj, container );
	act( "$n puts $p in $P.", ch, NULL, obj, container,
	    NULL, TO_ROOM, SENSE_SIXTH );
	act( "You put $p in $P.", ch, NULL, obj, container,
	    NULL, TO_CHAR, SENSE_SIGHT );
    }
    else
    {
	/* 'put all container' or 'put all.obj container' */
	for ( obj = ch->carrying; obj != NULL; obj = obj_next )
	{
	    obj_next = obj->next_content;

	    if ( ( arg1[3] == '\0' || is_name( &arg1[4], obj->name ) )
	    &&   can_see_obj( ch, obj )
	    &&   obj->wear_loc == gn_wear_none
	    &&   obj != container
	    &&   can_drop_obj( ch, obj )
	    &&   get_obj_weight( obj ) + get_obj_weight( container )
		 <= container->capacity )
	    {
		obj_from_char( obj );
		obj_to_obj( obj, container );
		act( "$n puts $p in $P.", ch, NULL, obj, container,
		    NULL, TO_ROOM, SENSE_SIGHT );
		act( "You put $p in $P.", ch, NULL, obj, container,
		    NULL, TO_CHAR, SENSE_SIXTH );
	    }
	}
    }

    return;
}



void do_drop( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    bool found;

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
      send_to_char( "Drop what?\n\r", ch );
      // send_to_char ("Disabled.\n\r", ch);
	return;
    }

    if ( is_number( arg ) )
    {
	/* 'drop NNNN coins' */
	int amount;
	int coin;
	OBJ_DATA *c_obj;

	amount   = atoi(arg);
	argument = one_argument( argument, arg );
	if ( amount <= 0
	|| ( c_obj = get_obj_carry( ch, arg, ch ) ) == NULL
	|| c_obj->item_type != ITEM_MONEY )
	{
	    send_to_char( "Sorry, you can't do that.\n\r", ch );
	    return;
	}

	if ( c_obj->value[0] < amount )
	{
	    send_to_char( "You haven't got that many coins.\n\r", ch );
	    return;
	}

	coin = c_obj->value[1];
	lose_coin( ch, amount, coin, FALSE, NULL );

	for ( obj = ch->in_room->contents; obj != NULL; obj = obj_next )
	{
	    obj_next = obj->next_content;

	    if ( obj->item_type == ITEM_MONEY
	    && obj->value[1] == coin )
	    {
		amount += obj->value[0];
		extract_obj( obj, TRUE );
		break;
	    }
	}

	obj_to_room( create_money( amount, coin ), ch->in_room, TRUE );
	act( "$n drops some coins.", ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIGHT );
	send_to_char( "OK.\n\r", ch );
	return;
    }

    if ( str_cmp( arg, "all" ) && str_prefix( "all.", arg ) )
    {
	/* 'drop obj' */
	if ( ( obj = get_obj_carry( ch, arg, ch ) ) == NULL )
	{
	    send_to_char( "You do not have that item.\n\r", ch );
	    return;
	}

	if ( !can_drop_obj( ch, obj ) )
	{
	    send_to_char( "You can't let go of it.\n\r", ch );
	    return;
	}

	obj_from_char( obj );
	obj_to_room( obj, ch->in_room, TRUE );
	act( "$n drops $p.", ch, NULL, obj, NULL, NULL, TO_ROOM, SENSE_SIGHT );
	act( "You drop $p.", ch, NULL, obj, NULL, NULL, TO_CHAR, SENSE_SIXTH );
    }
    else
    {
	/* 'drop all' or 'drop all.obj' */
	found = FALSE;
	for ( obj = ch->carrying; obj != NULL; obj = obj_next )
	{
	    obj_next = obj->next_content;

	    if ( ( arg[3] == '\0' || is_name( &arg[4], obj->name ) )
	    &&   can_see_obj( ch, obj )
	    &&   obj->wear_loc == gn_wear_none
	    &&   can_drop_obj( ch, obj ) )
	    {
		found = TRUE;
		obj_from_char( obj );
		obj_to_room( obj, ch->in_room, TRUE );
		act( "$n drops $p.", ch, NULL, obj, NULL, NULL, TO_ROOM, SENSE_SIGHT );
		act( "You drop $p.", ch, NULL, obj, NULL, NULL, TO_CHAR, SENSE_SIXTH );
	    }
	}

	if ( !found )
	{
	    if ( arg[3] == '\0' )
		act( "You are not carrying anything.",
		    ch, NULL, NULL, arg, NULL, TO_CHAR, SENSE_SIXTH );
	    else
		act( "You are not carrying any $t.",
		    ch, NULL, NULL, &arg[4], NULL, TO_CHAR, SENSE_SIXTH );
	}
    }

    return;
}



void do_give( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA  *obj;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Give what to whom?\n\r", ch );
	return;
    }

    if ( is_number( arg1 ) )
    {
	/* 'drop NNNN coins' */
	int amount;
	int coin;
	OBJ_DATA *c_obj;

	amount   = atoi(arg1);
	if ( amount <= 0
	|| ( c_obj = get_obj_carry( ch, arg2, ch ) ) == NULL
	|| c_obj->item_type != ITEM_MONEY )
	{
	    send_to_char( "Sorry, you can't do that.\n\r", ch );
	    return;
	}

	if ( c_obj->value[0] < amount )
	{
	    send_to_char( "You haven't got that many coins.\n\r", ch );
	    return;
	}

	argument = one_argument( argument, arg1 );
	if ( arg1[0] == '\0' )
	{
	    send_to_char( "Give what to whom?\n\r", ch );
	    return;
	}

	if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
	{
	    send_to_char( "They aren't here.\n\r", ch );
	    return;
	}

	coin = c_obj->value[1];
	lose_coin( ch, amount, coin, FALSE, NULL );
	gain_coin( victim, amount, coin, FALSE, NULL );

	if ( amount == 1 )
	    sprintf( buf, "%d %s", amount, coin_table[coin].name );
	else
	    sprintf( buf, "%d %s", amount, coin_table[coin].name_plural );

	act( "$n gives $N some coins.",  ch, victim, NULL, NULL,
	    NULL, TO_NOTVICT, SENSE_SIGHT );
	act( "$n gives you $t.", ch, victim, NULL, buf,
	    NULL, TO_VICT, SENSE_SIGHT|SENSE_TOUCH );
	act( "You give $N $t.",  ch, victim, NULL, buf,
	    NULL, TO_CHAR, SENSE_SIXTH);

	return;
    }

    if ( ( obj = get_obj_carry( ch, arg1, ch ) ) == NULL )
    {
	send_to_char( "You do not have that item.\n\r", ch );
	return;
    }

    if ( obj->wear_loc != gn_wear_none )
    {
	send_to_char( "You must remove it first.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( !can_drop_obj( ch, obj ) )
    {
	send_to_char( "You can't let go of it.\n\r", ch );
	return;
    }

    if ( victim->carry_number + 1 > can_carry_n( victim ) )
    {
	act( "$N has $S hands full.", ch, victim, NULL, NULL,
	    NULL, TO_CHAR, SENSE_SIXTH );
	return;
    }

    if ( victim->carry_weight + get_obj_weight( obj ) > can_carry_w( victim ) )
    {
	act( "$N can't carry that much weight.", ch, victim, NULL, NULL,
	    NULL, TO_CHAR, SENSE_SIXTH );
	return;
    }

    if ( !can_see_obj( victim, obj ) )
    {
	act( "$N can't see it.", ch, victim, NULL, NULL,
	    NULL, TO_CHAR, SENSE_SIXTH );
	return;
    }

    act( "$n gives $p to $N.", ch, victim, obj, NULL,
	NULL, TO_NOTVICT, SENSE_SIGHT );
    act( "$n gives you $p.", ch, victim, obj, NULL,
	NULL, TO_VICT, SENSE_SIGHT|SENSE_TOUCH );
    act( "You give $p to $N.", ch, victim, obj, NULL,
	NULL, TO_CHAR, SENSE_SIXTH );

    obj_from_char( obj );
    if ( obj->item_type == ITEM_MONEY )
    {
	gain_coin( victim, obj->value[0], obj->value[1], FALSE, NULL );
	extract_obj( obj, TRUE );
    }
    else
	obj_to_char( obj, victim );

    return;
}


char *get_virtual_water_source( CHAR_DATA *ch, char *arg)
{
    if ( !str_cmp( arg, "channel" ) && is_name( arg, ch->in_room->name ) )
	return "the channel";

    if ( !str_cmp( arg, "lake" ) && is_name( arg, ch->in_room->name ) )
	return "the lake";

    if ( !str_cmp( arg, "river" ) && is_name( arg, ch->in_room->name ) )
	return "the river";

    if ( !str_cmp( arg, "stream" ) && is_name( arg, ch->in_room->name ) )
	return "the stream";

    if ( !str_cmp( arg, "waterfall" ) && is_name( arg, ch->in_room->name ) )
	return "the waterfall";

    return NULL;
}



void do_fill( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char *word = NULL;
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *fountain;

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Fill what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_carry( ch, arg, ch ) ) == NULL )
    {
	send_to_char( "You do not have that item.\n\r", ch );
	return;
    }

    if ( ( fountain = get_obj_here( ch, argument ) ) == NULL )
    {
	if ( argument[0] == '\0' )
	{
	    send_to_char("Fill from what?\n\r", ch );
	    return;
	}

	if ( !IS_SET( ch->in_room->room_flags, ROOM_DRINKABLE_WATER )
	|| ( word = get_virtual_water_source( ch, argument ) ) == NULL )
	{
	    send_to_char("You don't see that here.\n\r", ch );
	    return;
	}
    }
    else
    {
	if ( fountain->item_type != ITEM_FOUNTAIN
	&& fountain->item_type != ITEM_DRINK_CON )
	{
	    send_to_char("How do you expect to fill from that?\n\r", ch );
	    return;
	}
    }

    if ( obj->item_type != ITEM_DRINK_CON )
    {
	send_to_char( "Your container ought to hold liquids.\n\r", ch );
	return;
    }

    if ( ( fountain != NULL
	? obj->value[2] != fountain->value[2]
	: obj->value[2] != 0 )
    && obj->value[1] != 0 )
    {
	send_to_char( "You should pour the other liquid out first.\n\r", ch );
	return;
    }

    if ( obj->value[1] >= obj->value[0] )
    {
	send_to_char( "Your container is full.\n\r", ch );
	return;
    }

    if ( fountain != NULL && fountain->value[1] == 0 )
    {
	send_to_char("It is empty.\n\r", ch );
	return;
    }

    sprintf( buf, "You fill %s from %s.\n\r", obj->short_descr,
	( fountain != NULL ? fountain->short_descr : word ) );
    send_to_char( buf, ch );
    sprintf( buf, "$n fills $p from %s.",
	( fountain != NULL ? fountain->short_descr : word ) );
    act( buf, ch, NULL, obj, NULL, NULL, TO_ROOM, SENSE_SIGHT );
/* liquid */
    if ( fountain != NULL )
	obj->value[2] = fountain->value[2];
    else
	obj->value[2] = 0;
/* poison */
    if ( fountain != NULL )
	obj->value[3] = fountain->value[3];

    if ( fountain == NULL || fountain->value[1] < 0 )
    {
	obj->value[1] = obj->value[0];
    }
    else
    {
	if ( (obj->value[1] - obj->value[0]) > fountain->value[1] )
	{
	    obj->value[1] += fountain->value[1];
	    fountain->value[1] = 0;
	}
	else
	{
	    fountain->value[1] -= obj->value[0] + obj->value[1];
	    obj->value[1] = obj->value[2];
	}
    }
    return;
}


void do_pour( CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];
  char arg2[MAX_INPUT_LENGTH];
  char buf[MAX_STRING_LENGTH];
  OBJ_DATA *obj;
  FLORA_DATA *fi;

  argument = one_argument( argument, arg );
  argument = one_argument( argument, arg2 );

  if ( arg[0] == '\0' || arg[1] == '\1' )
  {
    send_to_char(
      "Pour what where?  (pour <item> out, pour <item> <item>)\n\r", ch );
    return;
  }

  if ( ( obj = get_obj_carry( ch, arg, ch ) ) == NULL )
  {
    send_to_char( "You do not have that item.\n\r", ch );
    return;
  }

  if ( obj->item_type != ITEM_DRINK_CON )
  {
    send_to_char( "That doesn't contain a liquid.\n\r", ch );
    return;
  }

  if ( obj->value[1] <= 0 )
  {
    send_to_char( "It is already empty.\n\r", ch );
    return;
  }

  if ( !str_cmp( arg2, "out" ) )
  {
    if ( obj->value[2] == LIQ_WATER
    && IS_SET( ch->in_room->sector->flags, SECTOR_OUTDOORS ) )
      ch->in_room->water += obj->value[1];

    act( "You pour $p onto the ground.",
      ch, NULL, obj, NULL, NULL, TO_CHAR, SENSE_SIXTH );
    act( "$n pours $p onto the ground.",
      ch, NULL, obj, NULL, NULL, TO_ROOM, SENSE_SIGHT );

    obj->value[1] = 0;

    return;
  }

  fi = get_flora_here( ch, arg2 );
  if ( fi )
  {
    if ( fi->quantity > 1 )
    {
      sprintf( buf, "%s out", arg );
      do_pour( ch, buf );
      return;
    }

    /* double water if watering specific plant */
    if ( obj->value[2] == LIQ_WATER
    && IS_SET( ch->in_room->sector->flags, SECTOR_OUTDOORS ) )
      fi->water += 2*obj->value[1];

    obj->value[1] = 0;

    sprintf( buf, "%s at the base of %s.",
      obj->short_descr, get_plant_sdesc( fi ) );
    act( "You pour $t", ch, NULL, NULL, buf, NULL, TO_CHAR, SENSE_SIXTH );
    act( "$n pours $t", ch, NULL, NULL, buf, NULL, TO_ROOM, SENSE_SIGHT );

    return;
  }

/* item item */
/* item fountain */
  send_to_char( "Pour it into what?\n\r", ch );

  return;
}


void do_drink( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    char *word = NULL;
    int amount;
    int liquid;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	for ( obj = ch->in_room->contents; obj; obj = obj->next_content )
	{
	    if ( obj->item_type == ITEM_FOUNTAIN )
		break;
	}

	if ( obj == NULL )
	{
	    send_to_char("Drink from what?\n\r", ch );
	    return;
	}
    }
    else
    {
	if ( ( obj = get_obj_here( ch, arg ) ) == NULL
	&& ( !IS_SET( ch->in_room->room_flags, ROOM_DRINKABLE_WATER )
	    || ( word = get_virtual_water_source( ch, arg ) ) == NULL ) )
	{
	    send_to_char( "You can't find it.\n\r", ch );
	    return;
	}
    }

    if ( !IS_NPC(ch)
    && ch->pcdata->condition[COND_DRUNK] > 10
    && obj != NULL )    /* no problem if river etc. */
    {
	send_to_char( "You fail to reach your mouth.  *Hic*\n\r", ch );
	return;
    }

  if ( obj == NULL )
  {
    if ( !IS_NPC(ch) )
	ch->pcdata->condition[COND_THIRST] = 48;
    act("$n takes a drink from $t.", ch, NULL, NULL, word, NULL, TO_ROOM, SENSE_SIGHT );
    act("You take a drink from $t.", ch, NULL, NULL, word, NULL, TO_CHAR, SENSE_SIXTH );
    send_to_char("You are no longer thirsty.\n\r", ch );
  }
  else
  {
    switch ( obj->item_type )
    {
    default:
	send_to_char( "You can't drink from that.\n\r", ch );
	break;

    case ITEM_FOUNTAIN:
	if ( !IS_NPC(ch) )
	    ch->pcdata->condition[COND_THIRST] = 48;
	act( "$n drinks from the fountain.", ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIGHT );
	send_to_char( "You are no longer thirsty.\n\r", ch );
	break;

    case ITEM_DRINK_CON:
	if ( obj->value[1] <= 0 )
	{
	    send_to_char( "It is already empty.\n\r", ch );
	    return;
	}

	if ( ( liquid = obj->value[2] ) >= LIQ_MAX )
	{
	    bug( "Do_drink: bad liquid number %d.", liquid );
	    liquid = obj->value[2] = 0;
	}

	act( "$n drinks $t from $p.",
	    ch, NULL, obj, liq_table[liquid].liq_name, NULL, TO_ROOM, SENSE_SIGHT );
	act( "You drink $t from $p.",
	    ch, NULL, obj, liq_table[liquid].liq_name, NULL, TO_CHAR, SENSE_SIXTH );

	amount = number_range(3, 10);
	amount = UMIN(amount, obj->value[1]);
	
	gain_condition( ch, COND_DRUNK,
	    amount * liq_table[liquid].liq_affect[COND_DRUNK  ] );
	gain_condition( ch, COND_FULL,
	    amount * liq_table[liquid].liq_affect[COND_FULL   ] );
	gain_condition( ch, COND_THIRST,
	    amount * liq_table[liquid].liq_affect[COND_THIRST ] );

	if ( !IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK]  > 10 )
	    send_to_char( "You feel drunk.\n\r", ch );
	if ( !IS_NPC(ch) && ch->pcdata->condition[COND_FULL]   > 40 )
	    send_to_char( "You are full.\n\r", ch );
	if ( !IS_NPC(ch) && ch->pcdata->condition[COND_THIRST] > 40 )
	    send_to_char( "You do not feel thirsty.\n\r", ch );

	if ( !IS_NPC( ch ) )
	{
	    ch->pcdata->condition[COND_FULL] = UMIN( ch->pcdata->condition[COND_FULL], 48 );
	    ch->pcdata->condition[COND_THIRST] = UMIN( ch->pcdata->condition[COND_THIRST], 48 );
	}
	
	if ( obj->value[3] != 0 )
	{
	    /* The shit was poisoned ! */
	    AFFECT_DATA af;

	    act( "$n chokes and gags.", ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIGHT|SENSE_HEARING );
	    send_to_char( "You choke and gag.\n\r", ch );
	    af.type      = gsn_poison;
	    af.duration  = 15 * amount;
	    af.location  = APPLY_NONE;
	    af.modifier  = 0;
	    af.bitvector = AFF_POISON;
	    affect_join( ch, &af );
	}
	
	obj->value[1] -= amount;
	break;
    }
  }

    return;
}



void do_eat( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Eat what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_carry( ch, arg, ch ) ) == NULL )
    {
	send_to_char( "You do not have that item.\n\r", ch );
	return;
    }

    if ( !IS_IMMORTAL(ch) )
    {
	if ( obj->item_type != ITEM_FOOD && obj->item_type != ITEM_PILL )
	{
	    send_to_char( "That's not edible.\n\r", ch );
	    return;
	}

	if ( !IS_NPC(ch) && ch->pcdata->condition[COND_FULL] > 40 )
	{   
	    send_to_char( "You are too full to eat more.\n\r", ch );
	    return;
	}
    }

    act( "$n eats $p.", ch, NULL, obj, NULL, NULL, TO_ROOM, SENSE_SIGHT );
    act( "You eat $p.", ch, NULL, obj, NULL, NULL, TO_CHAR, SENSE_SIXTH );

    switch ( obj->item_type )
    {

    case ITEM_FOOD:
	if ( !IS_NPC(ch) )
	{
	    int condition;

	    condition = ch->pcdata->condition[COND_FULL];
	    gain_condition( ch, COND_FULL, obj->value[0] );
	    if ( condition == 0 && ch->pcdata->condition[COND_FULL] > 0 )
		send_to_char( "You are no longer hungry.\n\r", ch );
	    else if ( ch->pcdata->condition[COND_FULL] > 40 )
		send_to_char( "You are full.\n\r", ch );
	}

	if ( obj->value[3] != 0 )
	{
	    /* The shit was poisoned! */
	    AFFECT_DATA af;

	    act( "$n chokes and gags.", ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIGHT|SENSE_HEARING );
	    send_to_char( "You choke and gag.\n\r", ch );

	    af.type      = gsn_poison;
	    af.duration  = 15 * obj->value[0];
	    af.location  = APPLY_NONE;
	    af.modifier  = 0;
	    af.bitvector = AFF_POISON;
	    affect_join( ch, &af );
	}
	break;

    case ITEM_PILL:
	obj_cast_spell( obj->value[1], obj->value[0], ch, ch, NULL );
	obj_cast_spell( obj->value[2], obj->value[0], ch, ch, NULL );
	obj_cast_spell( obj->value[3], obj->value[0], ch, ch, NULL );
	break;
    }

    extract_obj( obj, TRUE );
    return;
}



/*
 * Remove an object.
 */
bool remove_obj( CHAR_DATA *ch, OBJ_DATA *obj )
{
    int wear_loc = obj->wear_loc;

    if ( IS_SET(obj->extra_flags, ITEM_NOREMOVE) )
    {
	act( "You can't remove $p.", ch, NULL, obj, NULL, NULL, TO_CHAR, SENSE_SIXTH );
	return FALSE;
    }

    unequip_char( ch, obj );
    act( "$n stops using $p.", ch, NULL, obj, NULL, NULL, TO_ROOM, SENSE_SIGHT );
    act( "You stop using $p.", ch, NULL, obj, NULL, NULL, TO_CHAR, SENSE_SIXTH );

    remove_dependent_items( ch, wear_loc );

    return TRUE;
}



/*
 * Wear one object.
 * Big repetitive code, ick.
 *
 * Not anymore!  Bwahahahaha.  Sep 5, 1997, Rip.
 */
void wear_obj( CHAR_DATA *ch, OBJ_DATA *obj, int location )
{
    extern struct wear_data const wear_info[];
    char buf[MAX_STRING_LENGTH];
    int total = sum_worn( ch, location ) + obj->size;

    if ( wear_info[location].dependency != NULL
    && ch->wearing[*wear_info[location].dependency] == NULL )
    {
	send_to_char("What would you attach it to?\n\r", ch );
	return;
    }

    if ( total > 100 )
    {
	sprintf( buf, "Your %s can utilize no additional equipment.\n\r", wear_info[location].name );
	send_to_char( buf, ch );
	return;
    }

    act( wear_info[location].wear_string_ch, ch, NULL, obj, NULL, NULL, TO_CHAR, SENSE_SIXTH );
    act( wear_info[location].wear_string_room, ch, NULL, obj, NULL, NULL, TO_ROOM, SENSE_SIGHT );
    equip_char( ch, obj, location );

    return;
}

int sum_worn( CHAR_DATA *ch, int location )
{
    int i = 0;
    OBJ_DATA *obj;

    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
	if ( obj->wear_loc == location )
	    i += obj->size;

    return i;
}



void do_wear( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Wear, or hold what?\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, "all" ) )
    {
	OBJ_DATA *obj_next;

	if ( check_fighting( ch ) )
	{
	    send_to_char("Equipping yourself in combat? this I must see...\n\r", ch );
	    return;
	}

	for ( obj = ch->carrying; obj != NULL; obj = obj_next )
	{
	    obj_next = obj->next_content;
	    if ( obj->wear_loc == gn_wear_none
	    && can_see_obj( ch, obj )
	    && get_wear_loc( obj, "default" ) != gn_wear_none )
		wear_obj( ch, obj, get_wear_loc( obj, "default" ) );
	}
	return;
    }
    else
    {
	int loc;

	if ( ( obj = get_obj_carry( ch, arg, ch) ) == NULL )
	{
	    send_to_char( "You do not have that item.\n\r", ch );
	    return;
	}

	loc = get_wear_loc( obj, argument );
	if ( loc == gn_wear_none )
	{
	    send_to_char("You can't figure out how to wear that.\n\r", ch );
	    return;
	}

	if ( check_fighting( ch ) )
	{
	    send_to_char("Dressing in combat is possible.. but might take a while..\n\r", ch );
	    WAIT_STATE( ch, 24 );
	}

	wear_obj( ch, obj, loc );
    }

    return;
}


void do_sheath( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
    extern struct wear_data const wear_info[];

    one_argument( argument, arg );

    obj = get_eq_char( ch, gn_wear_primary );

    if ( obj == NULL || !is_name( arg, obj->name ) )
    {
	obj = get_eq_char( ch, gn_wear_secondary );
	if ( obj == NULL || !is_name( arg, obj->name ) )
	{
	    send_to_char("You don't wield that weapon.\n\r", ch );
	    return;
	}
    }

    if ( obj->item_type != ITEM_WEAPON )
    {
	send_to_char("You can't sheath that.\n\r", ch );
	return;
    }

    if ( wear_info[gn_wear_on_belt].dependency != NULL
    && ch->wearing[*wear_info[gn_wear_on_belt].dependency] == NULL )
    {
	send_to_char("You aren't wearing anything to attach it to.\n\r", ch );
	return;
    }

    if ( ( sum_worn( ch, gn_wear_on_belt ) + obj->size ) > 100 )
    {
	send_to_char("There is already too much stuff on your belt.\n\r", ch );
	return;
    }

    unequip_char( ch, obj );

    act( "You sheath $p", ch, NULL, obj, NULL, NULL, TO_CHAR, SENSE_SIXTH );
    act( "$n sheathes $p", ch, NULL, obj, NULL, NULL, TO_ROOM, SENSE_SIGHT );

    equip_char( ch, obj, gn_wear_on_belt );

    return;
}


void do_draw( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
    int loc = gn_wear_primary;

    one_argument( argument, arg );

    if ( get_eq_char( ch, gn_wear_primary ) )
    {
	if ( get_eq_char( ch, gn_wear_secondary ) )
	{
	    send_to_char("Your hands are full.\n\r", ch );
	    return;
	}
	loc = gn_wear_secondary;
    }

    for ( obj = ch->wearing[gn_wear_on_belt]; obj != NULL; obj = obj->next_worn )
    {
	if ( is_name( arg, obj->name ) )
	    break;
    }

    if ( obj == NULL )
    {
	send_to_char("You don't seem to be wearing that weapon.\n\r", ch );
	return;
    }

    if ( get_obj_weight( obj ) > (can_carry_w(ch)/15) )
    {
	send_to_char( "It is too heavy for you to wield.\n\r", ch );
	return;
    }

    act( "$n draws $p.", ch, NULL, obj, NULL, NULL, TO_ROOM, SENSE_SIGHT );
    act( "You draw $p.", ch, NULL, obj, NULL, NULL, TO_CHAR, SENSE_SIXTH );
    unequip_char( ch, obj );
    equip_char( ch, obj, loc );

    return;
}


int get_wear_loc( OBJ_DATA *obj, char *argument )
{
    extern struct wear_data const wear_info[];
    int i;

    for ( i = 0; i < MAX_WEAR; i++ )
	if ( !str_cmp( argument, wear_info[i].name )
	&& IS_SET( obj->wear_flags, wear_info[i].bit ) )
	    return i;

    for ( i = 0; i < MAX_WEAR; i++ )
	if ( IS_SET( obj->wear_flags, wear_info[i].bit ) )
	    return i;

    return gn_wear_none;
}


void do_wield( CHAR_DATA *ch, char* argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Wear, or hold what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_carry( ch, arg, ch ) ) == NULL )
    {
	send_to_char( "You do not have that item.\n\r", ch );
	return;
    }

    if ( get_eq_char( ch, gn_wear_primary ) != NULL
    && get_eq_char( ch, gn_wear_secondary ) != NULL)
    {
	send_to_char("You are already wielding something.\n\r", ch );
	return;
    }

    if ( get_obj_weight( obj ) > (can_carry_w(ch)/15) )
    {
	send_to_char( "It is too heavy for you to wield.\n\r", ch );
	return;
    }

    act( "$n wields $p.", ch, NULL, obj, NULL, NULL, TO_ROOM, SENSE_SIGHT );
    act( "You wield $p.", ch, NULL, obj, NULL, NULL, TO_CHAR, SENSE_SIXTH );
  if ( get_eq_char( ch, gn_wear_primary ) == NULL )
    equip_char( ch, obj, gn_wear_primary );
  else
    equip_char( ch, obj, gn_wear_secondary );
    return;
}

void do_hold( CHAR_DATA *ch, char* argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Hold what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_carry( ch, arg, ch ) ) == NULL )
    {
	send_to_char( "You do not have that item.\n\r", ch );
	return;
    }

    if ( get_eq_char( ch, gn_wear_secondary ) != NULL )
    {
	send_to_char("You are already holding something.\n\r", ch );
	return;
    }

    if ( get_obj_weight( obj ) > (can_carry_w(ch)/15) )
    {
	send_to_char( "It is too heavy for you use effectively.\n\r", ch );
	return;
    }

    act( "$n holds $p.", ch, NULL, obj, NULL, NULL, TO_ROOM, SENSE_SIGHT );
    act( "You hold $p.", ch, NULL, obj, NULL, NULL, TO_CHAR, SENSE_SIXTH );
    equip_char( ch, obj, gn_wear_secondary );
    return;
}



void do_remove( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Remove what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_wear( ch, arg, ch ) ) == NULL )
    {
	send_to_char( "You do not have that item.\n\r", ch );
	return;
    }

    if ( check_fighting( ch ) )
    {
	if (obj->item_type != ITEM_WEAPON)
	{
	    send_to_char("You can't remove equipment while fighting.\n\r", ch );
	    return;
	}
	else
	{
	    send_to_char("You strain to avoid getting slaughtered while dumping your weapon.\n\r", ch );
	    WAIT_STATE( ch, 24 );
	}
    }

    remove_obj( ch, obj );
    return;
}



void do_sacrifice( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
/*
    OBJ_DATA *obj;
*/

    one_argument( argument, arg );

    send_to_char("Sacrifice is currently undefined, post suggestions.\n\r", ch );
    return;
}



void do_quaff( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Quaff what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_carry( ch, arg, ch ) ) == NULL )
    {
	send_to_char( "You do not have that potion.\n\r", ch );
	return;
    }

    if ( obj->item_type != ITEM_POTION )
    {
	send_to_char( "You can quaff only potions.\n\r", ch );
	return;
    }

    act( "$n quaffs $p.", ch, NULL, obj, NULL, NULL, TO_ROOM, SENSE_SIGHT );
    act( "You quaff $p.", ch, NULL, obj, NULL, NULL, TO_CHAR, SENSE_SIXTH );

    obj_cast_spell( obj->value[1], obj->value[0], ch, ch, NULL );
    obj_cast_spell( obj->value[2], obj->value[0], ch, ch, NULL );
    obj_cast_spell( obj->value[3], obj->value[0], ch, ch, NULL );

    extract_obj( obj, TRUE );
    return;
}



void do_recite( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *scroll;
    OBJ_DATA *obj;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( ( scroll = get_obj_carry( ch, arg1, ch ) ) == NULL )
    {
	send_to_char( "You do not have that scroll.\n\r", ch );
	return;
    }

    if ( scroll->item_type != ITEM_SCROLL )
    {
	send_to_char( "You can recite only scrolls.\n\r", ch );
	return;
    }

    obj = NULL;
    if ( arg2[0] == '\0' )
    {
	victim = ch;
    }
    else
    {
	if ( ( victim = get_char_room ( ch, arg2 ) ) == NULL
	&&   ( obj    = get_obj_here  ( ch, arg2 ) ) == NULL )
	{
	    send_to_char( "You can't find it.\n\r", ch );
	    return;
	}
    }

    act( "$n recites $p.", ch, NULL, scroll, NULL, NULL, TO_ROOM, SENSE_SIGHT|SENSE_HEARING );
    act( "You recite $p.", ch, NULL, scroll, NULL, NULL, TO_CHAR, SENSE_SIXTH );

    if ( number_percent() < get_skill( ch, gsn_read_magic ) )
    {
	obj_cast_spell( scroll->value[1], scroll->value[0], ch, victim, obj );
	obj_cast_spell( scroll->value[2], scroll->value[0], ch, victim, obj );
	obj_cast_spell( scroll->value[3], scroll->value[0], ch, victim, obj );
    }
    else
    {
	skill_improve( ch, gsn_read_magic );

	damage_from_object( ch, scroll, dice( 1, scroll->value[0] ), gsn_read_magic );
    }

    extract_obj( scroll, TRUE );
    return;
}


void do_use( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *device;
    bool found = FALSE;
    bool fFail = FALSE, fExplode = FALSE;
    int chance;
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' )
    {
	send_to_char("Use what device?\n\r", ch );
	return;
    }

    if ( ( device = get_eq_char( ch, gn_wear_primary ) ) != NULL )
    {
	if ( is_name( arg1, device->name ) )
	    found = TRUE;
    }

    if ( !found && ( device = get_eq_char( ch, gn_wear_secondary ) ) != NULL )
    {
	if ( is_name( arg1, device->name ) )
	    found = TRUE;
    }

    if ( !found && ( device = get_obj_carry( ch, arg2, ch ) ) != NULL )
    {
	if ( is_name( arg1, device->name ) )
	    found = TRUE;
    }

    if ( !found )
    {
	send_to_char("You don't have anything like that to use.\n\r", ch );
	return;
    }

    if ( ( device->item_type != ITEM_WAND ) && ( device->item_type != ITEM_STAFF ) )
    {
	act("You can't figure out what to do with $p.", ch, NULL, device, NULL, NULL, TO_CHAR, SENSE_SIXTH );
	return;
    }

    chance = get_skill( ch, gsn_use_magical_devices );
    chance += get_power_level( ch, gsn_use_magical_devices) - device->value[0];
    if ( number_percent() < ( device->value[0] / 2 ) )
	fExplode = TRUE;

    if ( number_percent() > chance )
	fFail = TRUE;

    if ( device->item_type == ITEM_WAND )
    {
	CHAR_DATA *victim;
	OBJ_DATA *obj = NULL;

	if ( arg2[0] == '\0' ||
	( ( victim = get_char_room( ch, arg2 ) ) == NULL
	&& ( obj = get_obj_here( ch, arg2 ) ) == NULL ) )
	{
	    send_to_char("Point the device at whom or what?\n\r", ch );
	    act( "$n fiddles with $p.", ch, NULL, device, NULL, NULL, TO_ROOM, SENSE_SIGHT );
	    return;
	}

/* delay */

	if ( victim != NULL )
	{
	    act( "$n points $p at $N.", ch, victim, device, NULL, NULL, TO_ROOM, SENSE_SIGHT );
	    act( "You point $p at $N.", ch, victim, device, NULL, NULL, TO_CHAR, SENSE_SIXTH );
	}
	else
	{
	    act( "$n point $p at $P.", ch, NULL, device, obj, NULL, TO_ROOM, SENSE_SIGHT );
	    act( "You point $p at $P.", ch, NULL, device, obj, NULL, TO_CHAR, SENSE_SIXTH );
	}

/* if no charges left, nothing happens.  No message as to why. */
	if ( device->value[1] == 0 )
	    return;

	if ( device->value[1] > 0 )
	    device->value[1] -= 1;

/* chance to successfully use wand is based on skill level of user and power
   of wand */
	if ( fFail )
	{
/* failure.  chance of explosion. */
	    if ( fExplode )
	    {
		damage_from_object( ch, device, dice( 1, device->value[0] ), gsn_use_magical_devices );
		power_improve( ch, gsn_use_magical_devices );
		extract_obj( device, TRUE );
		return;
	    }

/* nothing happened.  no reason why. */
	    skill_improve( ch, gsn_use_magical_devices );
	    return;
	}

	obj_cast_spell( device->value[3], device->value[0], ch, victim, obj );
	return;
    }
    else if ( device->item_type == ITEM_STAFF )
    {
	CHAR_DATA *victim = NULL;
	CHAR_DATA *vch = NULL;
	CHAR_DATA *vch_next = NULL;
	OBJ_DATA *obj = NULL;
	bool fRoom = FALSE;
	int sn;

	sn = device->value[3];

	if ( arg2[0] == '\0' ||
	( ( victim = get_char_room( ch, arg2 ) ) == NULL
	&& ( obj = get_obj_here( ch, arg2 ) ) == NULL ) )
	{
	    fRoom = TRUE;
	}

/* delay */

	if ( victim != NULL )
	{
	    act( "$n points $p at $N.", ch, victim, device, NULL, NULL, TO_ROOM, SENSE_SIGHT );
	    act( "You point $p at $N.", ch, victim, device, NULL, NULL, TO_CHAR, SENSE_SIXTH );
	}
	else if ( obj != NULL )
	{
	    act( "$n point $p at $P.", ch, NULL, device, obj, NULL, TO_ROOM, SENSE_SIGHT );
	    act( "You point $p at $P.", ch, NULL, device, obj, NULL, TO_CHAR, SENSE_SIXTH );
	}
	else
	{
	    act( "$n taps $p on the ground sharply.", ch, NULL, device, NULL, NULL, TO_ROOM, SENSE_SIGHT|SENSE_HEARING );
	    act( "You tap $p on the ground sharply.", ch, NULL, device, NULL, NULL, TO_CHAR, SENSE_SIXTH );
	}

/* if no charges left, nothing happens.  No message as to why. */
	if ( device->value[1] == 0 )
	    return;

	if ( device->value[1] > 0 )
	    device->value[1] -= 1;

/* chance to successfully use staff is based on skill level of user and power
   of staff */
	if ( fFail )
	{
/* failure.  chance of explosion. */
/* staff explosion hurts more than wand explosion */
	    if ( fExplode )
	    {
		damage_from_object( ch, device, dice( 1, (device->value[0]+(device->value[0]/2)) ), gsn_use_magical_devices );
		power_improve( ch, gsn_use_magical_devices );
		extract_obj( device, TRUE );
		return;
	    }

/* nothing happened.  no reason why. */
	    skill_improve( ch, gsn_use_magical_devices );
	    return;
	}

	if ( !fRoom )
	    obj_cast_spell( device->value[3], device->value[0], ch, victim, obj );
	else
	{
	  for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
	  {
	    vch_next	= vch->next_in_room;

	    switch ( skill_table[sn].target )
	    {
	    default:
		bug( "Do_use (staff room): bad target for sn %d.", sn );
		return;

	    case TAR_IGNORE:
		if ( vch != ch )
		    continue;
		break;

	    case TAR_CHAR_OFFENSIVE:
		if ( vch == ch )
		    continue;
		break;
		
	    case TAR_CHAR_DEFENSIVE:
		if ( vch == ch )
		    continue;
		break;

	    case TAR_CHAR_SELF:
		if ( vch == ch )
		    continue;
		break;
	    }

	    obj_cast_spell( device->value[3], device->value[0], ch, vch, NULL );
	  }
	}
	return;
    }
    else
    {
	act( "You don't know what to do with $p.", ch, NULL, device, NULL, NULL, TO_CHAR, SENSE_SIXTH );
	return;
    }

    return;
}


#define STEAL_FROM_GROUND	1
#define STEAL_FROM_OBJECT	2
#define STEAL_FROM_PERSON	4
#define STEAL_FROM_WORN_OBJ	8
#define STEAL_COINS		16
#define STEAL_FROM_BELT		32
#define STEAL_WORN_ITEM		64

void predelay_steal( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    char abuf [MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *objfrom = NULL;
    CHAR_DATA *victim;
    int number = 0;

    strcpy( abuf, argument );

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    argument = one_argument( argument, arg3 );

    if ( arg1[0] == '\0' )
    {
	send_to_char( "Steal what?\n\r", ch );
	return;
    }

    if ( arg2[0] == '\0' )
    {
	if ( ( obj = get_obj_list( ch, arg1, ch->in_room->contents ) ) == NULL )
	{
	    send_to_char("You don't see that here.\n\r", ch );
	    return;
	}
        
/*        if ( IS_SET(ch->act, PLR_HOLYWALK) )
        {
            
            do_steal( );
            send_to_char("Working...\n\r",ch);
            return;
        }*/

	set_predelay( ch, ( 120 - get_skill( ch, gsn_steal ) ),
	    do_steal, abuf, STEAL_FROM_GROUND, NULL, NULL, obj, NULL );
	send_to_char("You begin your steal attempt...\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg2 ) ) == NULL
    && ( objfrom = get_obj_list( ch, arg2, ch->in_room->contents ) ) == NULL )
    {
	send_to_char( "No object or player like that to steal from.\n\r", ch );
	return;
    }

    if ( objfrom != NULL )
    {
	obj = get_obj_list( ch, arg1, objfrom->contains );
	if ( obj == NULL )
	{
	    send_to_char("It doesn't seem to contain that.\n\r", ch );
	    return;
	}

/*        if ( IS_SET(ch->act, PLR_HOLYWALK ) )
        {
            set_predelay( ch,  ( 900 - get_skill(ch, gsn_steal ) ),
            do_steal,abuf,STEAL_FROM_OBJECT,NULL,NULL,obj,objfrom );
            send_to_char("Working...\n\r",ch);
            return;
        }*/

	set_predelay( ch,  ( 120 - get_skill( ch, gsn_steal ) ),
	    do_steal, abuf, STEAL_FROM_OBJECT, NULL, NULL, obj, objfrom );
	send_to_char("You begin your steal attempt...\n\r", ch );
	return;
    }

    /*
     * If this far, we're stealing from a person.
     */
    if ( victim == ch )
    {
	send_to_char( "That's pointless.\n\r", ch );
	return;
    }

    if ( !str_cmp( arg1, "coins" ) )
	number |= STEAL_COINS;

    /*
     * We don't care whether anything is returned,
     * as these are all checked after skill checks/failures.
     *
     * Prevents keyword spam steal attempts.
     */
    if ( arg3[0] != '\0' && str_cmp( "worn", arg3 ) )
    {
	number |= STEAL_FROM_WORN_OBJ;
	objfrom = get_obj_wear( victim, arg3, ch );
    }

    if ( objfrom != NULL )
	obj = get_obj_list( ch, arg1, objfrom->contains );
    else
    {
	obj = get_obj_carry( victim, arg1, ch );
	if ( obj == NULL )
	{
	     for ( obj = victim->wearing[gn_wear_on_belt]; obj != NULL;
		obj = obj->next_worn )
	    {
		if ( !can_see_obj( victim, obj ) )
		    continue;

		if ( !is_name( arg1, obj->name ) )
		    continue;

		number |= STEAL_FROM_BELT;
		break;
	    }
	}

	if ( obj == NULL
	&& ( !strcmp( arg3, "worn" ) || !IS_AWAKE( victim ) ) )
	{
	    obj = get_obj_wear( victim, arg1, ch );
	    number |= STEAL_WORN_ITEM;
	}
    }
/*    if ( IS_SET(ch->act, PLR_HOLYWALK ) )
    {
        set_predelay( ch, ( 900 - get_skill( ch, gsn_steal ) ),
        do_steal, abuf, number, victim, NULL, obj, objfrom );
        send_to_char("Working...\n\r",ch);
        return;
    }*/

    set_predelay( ch, ( 120 - get_skill( ch, gsn_steal ) ),
	do_steal, abuf, number, victim, NULL, obj, objfrom );

    send_to_char("You begin your steal attempt...\n\r", ch );

    return;
}


void do_steal( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    OBJ_DATA *objfrom;
    char buf[MAX_STRING_LENGTH];
    int percent;
    int type;
    int coin_amount = 0;
    int coin_type = 0;

    if ( ch->predelay_info == NULL )
    {
	send_to_char("Steal: arguments missing.\n\r", ch );
	return;
    }

    update_noquit( ch );

    type =  ch->predelay_info->number;
    victim = ch->predelay_info->victim1;
    obj = ch->predelay_info->obj1;
    objfrom = ch->predelay_info->obj2;

    percent  = number_percent( );

    if ( type == STEAL_FROM_GROUND )
    {
	if ( obj == NULL
	|| obj->in_room != ch->in_room )
	{
	    send_to_char("It seems to be gone.\n\r", ch );
	    return;
	}
    }
    else if ( type == STEAL_FROM_OBJECT )
    {
	if ( objfrom == NULL
	|| objfrom->in_room != ch->in_room )
	{
	    send_to_char("The container seems to be missing.\n\r", ch );
	    return;
	}

	if ( obj == NULL
	|| obj->in_obj != objfrom )
	{
	    send_to_char("It doesn't seem to be in there anymore.\n\r", ch );
	    return;
	}
    }
    else
    {
      if ( victim == NULL
      || victim->in_room != ch->in_room
      || !can_see( ch, victim ) )
      {
	send_to_char( "They don't seem to be here anymore.\n\r", ch );
	return;
      }
    }

    if ( victim != NULL )
    {
	percent  -= (get_curr_dex(ch) - get_curr_int(victim));

	/*
	 * generally a 500% penalty.  this should be incredibly difficult.
	 */
	if ( IS_SET( type, STEAL_WORN_ITEM ) )
	    percent += get_curr_int( victim ) * 5;

	/*
	 * generally a 50% penalty.  this should be difficult.
	 */
	if ( IS_SET( type, STEAL_FROM_WORN_OBJ ) )
	    percent += get_curr_int( victim ) / 2;

	/*
	 * generally 20% penalty.
	 */
	if ( IS_SET( type, STEAL_FROM_BELT ) )
	    percent += get_curr_int( victim ) / 5;

	if ( !IS_AWAKE( victim ) )
	    percent = 0;
    }
    else
	percent  -= (get_curr_dex(ch) - 100);

    if ( percent > get_skill( ch, gsn_steal ) )
    {
	/*
	 * Failure.
	 */
	if ( victim != NULL )
	{
	    send_to_char( "Oops.\n\r", ch );
	    act( "$n tried to steal from you.", ch, victim, NULL, NULL, NULL,
		TO_VICT, SENSE_SIXTH );
	    act( "$n tried to steal from $N.",  ch, victim, NULL, NULL, NULL,
		TO_NOTVICT, SENSE_SIGHT );

	    skill_improve(ch,gsn_steal);

	    return;
	}
	else
	{
	    act( "$n stealthily palms something.",  ch, NULL, NULL, NULL, NULL,
		TO_ROOM, SENSE_SIGHT );
	}

/* add steal penalties */
/*
	sprintf( buf, "%s is a bloody thief!", ch->name );
	do_shout( victim, buf );
	if ( !IS_NPC(ch) )
	{
	    if ( IS_NPC(victim) )
		set_fighting( ch, victim );
	}
*/
    }

    if ( IS_SET( type, STEAL_FROM_WORN_OBJ ) )
    {
	if ( objfrom == NULL || objfrom->carried_by != victim )
	{
	    send_to_char("They don't seem to have the object you want to steal from.\n\r", ch );
	    return;
	}

	if ( objfrom->capacity <= 0 )
	{
	    send_to_char("How can you steal from that?\n\r", ch );
	    return;
	}

	if ( obj == NULL || obj->in_obj != objfrom )
	{
	    send_to_char("It doesn't have anything like that in it.\n\r", ch );
	    return;
	}
    }
    else
    {
	if ( obj == NULL || obj->carried_by != victim )
	{
	    send_to_char("They don't seem to have that.\n\r", ch );
	    return;
	}
    }

    if ( !can_drop_obj( ch, obj )
    ||   (IS_SET(obj->extra_flags, ITEM_INVENTORY)
	 && (IS_NPC(victim) ? victim->pIndexData->pShop != NULL : 0 )))
    {
	send_to_char( "You can't pry it away.\n\r", ch );
	return;
    }

    if ( ch->carry_number + 1 > can_carry_n( ch ) )
    {
	send_to_char( "You have your hands full.\n\r", ch );
	return;
    }

    if ( ch->carry_weight + get_obj_weight( obj ) > can_carry_w( ch ) )
    {
	send_to_char( "You can't carry that much weight.\n\r", ch );
	return;
    }

    if ( obj->item_type == ITEM_MONEY )
    {
	SET_BIT( type, STEAL_COINS );
	coin_amount = obj->value[0] * number_range(1, 10) / 100;
	coin_type = obj->value[1];

	if ( coin_amount <= 0 )
	{
	    send_to_char( "You couldn't get any coins.\n\r", ch );
	    return;
	}

	sprintf( buf, "Bingo!  You got " );
	gain_coin( ch, coin_amount, obj->value[1], TRUE, buf );
	strcat( buf, ".\n\r" );
	send_to_char( buf, ch );

	coin_amount = obj->value[0] - coin_amount;
    }

    if ( IS_SET( type, STEAL_FROM_WORN_OBJ|STEAL_FROM_OBJECT ) )
    {
	obj_from_obj( obj );

	if ( IS_SET( type, STEAL_COINS ) )
	{
	    extract_obj( obj, TRUE );
	    if ( coin_amount == 0 )
		return;
	    obj = create_money( coin_amount, coin_type );
	    obj_to_obj( obj, objfrom );
	    return;
	}
    }
    else if ( IS_SET( type, STEAL_FROM_GROUND ) )
    {
	obj_from_room( obj );

	if ( IS_SET( type, STEAL_COINS ) )
	{
	    extract_obj( obj, TRUE );
	    if ( coin_amount == 0 )
		return;
	    obj = create_money( coin_amount, coin_type );
	    obj_to_room( obj, ch->in_room, TRUE );
	    return;
	}
    }
    else
    {
	/* obj_from_char automatically unequips. */
	obj_from_char( obj );

	if ( IS_SET( type, STEAL_COINS ) )
	{
	    extract_obj( obj, TRUE );
	    if ( coin_amount == 0 )
		return;
	    obj = create_money( coin_amount, coin_type );
	    obj_to_char( obj, victim );
	    return;
	}
    }

    obj_to_char( obj, ch );

    send_to_char( "Ok.\n\r", ch );
    return;
}



void do_skin( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *item;
    int ii;
    bool skinned;

    if ( (obj = get_obj_here( ch, argument ) ) == NULL )
    {
	send_to_char("Skin what?\n\r", ch );
	return;
    }

    if ( obj->item_type != ITEM_CORPSE )
    {
	send_to_char("How do you propose skinning that?\n\r", ch );
	return;
    }

    send_to_char("Need test for skinning object here.\n\r", ch );

    while ( obj->contains != NULL )
    {
	item = obj->contains;

	obj_from_obj( item );
	obj_to_room( item, ch->in_room, TRUE );
    }

    skinned = FALSE;
    for ( ii = 0; ii < 4; ii++ )
    {
	if ( (pObjIndex = get_obj_index( obj->value[ii])) == NULL )
	    continue;

	item = create_object( pObjIndex );

	obj_to_room( item, ch->in_room, TRUE );
	skinned = TRUE;
    }

    if ( skinned == TRUE )
    {
	act("$n skins $p, removing all useful parts.", ch, NULL, obj, NULL, NULL, TO_ROOM, SENSE_SIGHT );
	act("You skin $p, removing all useful parts.", ch, NULL, obj, NULL, NULL, TO_CHAR, SENSE_SIXTH );
    }
    else
    {
	act("$n attempts to skin $p, but produces nothing of value.", ch, NULL, obj, NULL, NULL, TO_ROOM, SENSE_SIGHT );
	act("You attempt to skin $p, but produce nothing of value.", ch, NULL, obj, NULL, NULL, TO_CHAR, SENSE_SIXTH );
    }

    extract_obj( obj, TRUE );

    return;
}


void do_junk( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;

    if ( ( obj = get_obj_carry( ch, argument, ch ) ) == NULL )
    {
	send_to_char("Junk what?\n\r", ch );
	return;
    }

    act("You break $p.", ch, NULL, obj, NULL, NULL, TO_CHAR, SENSE_SIXTH );
    act("$n breaks $p.", ch, NULL, obj, NULL, NULL, TO_ROOM, SENSE_SIGHT );

    obj_from_char( obj );
    extract_obj( obj, TRUE );

    return;
}
