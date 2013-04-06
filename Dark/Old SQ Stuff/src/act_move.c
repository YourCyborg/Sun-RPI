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
#include "flora.h"



char *	const	dir_name	[]		=
{
    "north", "east", "south", "west", "up", "down"
};

char *	const	dir_letter	[]		=
{
    "N", "E", "S", "W", "U", "D"
};

char *  const   dir_name_2      []              =
{
    "the north", "the east", "the south", "the west", "above", "below"
};

const	sh_int	rev_dir		[]		=
{
    2, 3, 0, 1, 5, 4
};

const char *move_names [MOVE_MAX] =
{
  "walks", "runs", "sneaks"
};


/*
 * Local functions.
 */
int	find_door	args( ( CHAR_DATA *ch, char *arg ) );
bool	has_key		args( ( CHAR_DATA *ch, int key ) );
int	get_move_cost	args( ( CHAR_DATA *ch, ROOM_INDEX_DATA *room_from,
                                ROOM_INDEX_DATA *room_to ) );
int	get_move_delay	args( ( CHAR_DATA *ch, ROOM_INDEX_DATA *room_from,
                                ROOM_INDEX_DATA *room_to ) );





void move_char( CHAR_DATA *ch, int door )
{
  CHAR_DATA *fch;
  CHAR_DATA *fch_next;
  ROOM_INDEX_DATA *in_room;
  ROOM_INDEX_DATA *to_room;
  EXIT_DATA *pexit;
  int move;
  bool fSneak;

  if ( door < 0 || door > 5 )
  {
    bug( "Do_move: bad door %d.", door );
    return;
  }

  in_room = ch->in_room;
  if ( ( pexit   = in_room->exit[door] ) == NULL
  ||   ( to_room = pexit->to_room      ) == NULL )
  {
    send_to_char( "Alas, you cannot go that way.\n\r", ch );
    return;
  }


  if ( ch->riding != NULL && ch->riding->in_room != in_room )
    ch->riding = NULL;

  if (IS_SET(pexit->exit_flags, EX_CLOSED)
      && !IS_AFFECTED(ch, AFF_PASS_DOOR)
      && !IS_SET(ch->act, PLR_HOLYWALK)) {
      act( "The $d is closed.", ch, NULL, NULL, pexit->keyword,
	   NULL, TO_CHAR, SENSE_SIXTH );
      return;
  }

  if ( IS_AFFECTED(ch, AFF_CHARM)
       &&   ch->master != NULL
       &&   in_room == ch->master->in_room 
       &&   !IS_SET(ch->act, PLR_HOLYWALK)) {
      send_to_char( "You dont want to leave your beloved master.\n\r", ch );
      return;
  }

  if ( room_is_private( to_room ) && get_trust(ch) < 40 ) {
      send_to_char( "That room is private right now.\n\r", ch );
      return;
  }

  if (IS_SET( pexit->exit_flags, EX_CLIMB) )
  {
     if( pexit->difficulty == 100 )
     {
        send_to_char("You can't climb thin air.\n\r",ch );
        return;
     }
     
     if ( pexit->difficulty > get_skill( ch, gsn_climbing ) )
     {
         if ( ( pexit   = in_room->exit[DIR_UP] ) == NULL
         ||   ( to_room = pexit->to_room      ) == NULL )
         {

         send_to_char("You try to climb, but can't find a handhold.\n\r",ch);
         return;
         }
     
        send_to_char("You try to climb but loose your grip.\n\r",ch);
        
        if ( IS_SET(ch->act, PLR_CLIMBING) )
        {
        REMOVE_BIT(ch->act, PLR_CLIMBING);
        return;
        }

        if ( ch->riding == NULL || (!IS_SET(ch->act, PLR_WIZINVIS) ) )
        act( "$n tries to climb but slips.", ch, NULL, NULL,
        dir_name_2[rev_dir[door]], move_names[ch->move_speed],
	 TO_ROOM, SENSE_SIGHT|SENSE_HEARING );
      
     }
     
     }
 


    if( (IS_SET( to_room->sector->flags, SECTOR_AIR ) && !IS_SET(pexit->exit_flags, EX_CLIMB) )
    && ( !IS_AFFECTED(ch, AFF_FLYING) )  )
    {
        send_to_char("You can't fly.\n\r",ch);
        return;
    }
    


  if ((IS_SET (in_room->sector->flags, SECTOR_NOSWIM )
      ||  IS_SET (to_room->sector->flags, SECTOR_NOSWIM))
      && !IS_SET(ch->act, PLR_HOLYWALK)) {
      OBJ_DATA *obj;
      bool found;

    /*
     * Look for a boat.
     */
      found = FALSE;
      for ( obj = ch->carrying; obj != NULL; obj = obj->next_content) {
	  if ( obj->item_type == ITEM_BOAT ) {
	      found = TRUE;
	      break;
	  }
      }
      if ( !found ) {
	  send_to_char( "You need a boat to go there.\n\r", ch );
	  return;
      }
  }

  if ((IS_SET( pexit->exit_flags, EX_FENCE))
      && !IS_SET(ch->act, PLR_HOLYWALK)) {
      send_to_char("You'd be better off jumping or climbing that.\n\r", ch );
      return;
  }

  if (IS_SET(pexit->exit_flags, EX_WALL)
      && !IS_AFFECTED(ch, AFF_PASS_DOOR)
      && !IS_SET(ch->act, PLR_HOLYWALK)) {
      if (IS_SET( pexit->exit_flags, EX_CLIMB))
	  send_to_char("You'd be better off jumping or climbing that.\n\r", ch );
      else
	  send_to_char( "Alas, you cannot go that way.\n\r", ch );
      return;
  }

  if ( ch->move_speed < 0 || ch->move_speed >= MOVE_MAX )
  {
    send_to_char( "Error with your move speed, setting to walk.\n\r", ch );
    do_bug( ch, "Bad move speed." );
    ch->move_speed = MOVE_WALK;
  }

  move = get_move_cost( ch, in_room, to_room );

  if ( ch->move < move && ch->riding == NULL )
  {
    send_to_char( "You are too exhausted.\n\r", ch );
    return;
  }

  if ( ch->riding != NULL && ch->riding->move < 2 * move )
  {
    send_to_char( "Your mount is exhausted.\n\r", ch );
    return;
  }

  if ( ch->riding == NULL )
  {
    ch->move -= move;
  }
  else
  {
    ch->riding->move -= 2 * move;
  }

  fSneak = FALSE;

  if ( ch->move_speed == MOVE_SNEAK && ch->riding == NULL )
  {
    if ( number_percent() < get_skill( ch, gsn_sneak ) )
      fSneak = TRUE;
    else
    {
      if ( ch->next_in_room != NULL )
      skill_improve(ch,gsn_sneak);
    }
  }

  if ( !fSneak && !IS_SET(ch->act, PLR_WIZINVIS))
  {
    if ( IS_AFFECTED(ch, AFF_FLYING ) )
    {
    act( "$n flies $t.", ch, NULL, NULL, dir_name[door], NULL, TO_ROOM, SENSE_SIGHT|SENSE_HEARING );
    }
    else if ( ch->riding == NULL )
    {
      act( "$n $T $t.", ch, NULL, NULL, dir_name[door],
        move_names[ch->move_speed], TO_ROOM, SENSE_SIGHT|SENSE_HEARING );
    }
    else
    {
      act( "$n leaves $t, riding $N.", ch, ch->riding, NULL, dir_name[door],
        NULL, TO_ROOM, SENSE_SIGHT|SENSE_HEARING );
    }
  }

  char_from_room( ch );
  char_to_room( ch, to_room, TRUE );
  if ( ch->riding != NULL )
  {
    char_from_room( ch->riding );
    char_to_room( ch->riding, to_room, TRUE );
  }
  if ( !fSneak && !IS_SET(ch->act, PLR_WIZINVIS))
  {
    if ( ch->riding == NULL  )
       act( "$n $T in from $t.", ch, NULL, NULL,
        dir_name_2[rev_dir[door]], move_names[ch->move_speed],
	TO_ROOM, SENSE_SIGHT|SENSE_HEARING );
    else
    {
      act( "$n has arrived from $t, riding $N.", ch, ch->riding, NULL,
        dir_name_2[rev_dir[door]], NULL, TO_ROOM, SENSE_SIGHT|SENSE_HEARING );
    }
  }

  do_look( ch, "auto" );

  WAIT_STATE( ch, get_move_delay( ch, in_room, to_room ) );
  clear_dynamic( ch );

  for ( fch = in_room->people; fch != NULL; fch = fch_next )
  {
    fch_next = fch->next_in_room;
    if ( fch->master == ch && fch->position == POS_STANDING
    && !check_fighting( fch ) )
    {
      act( "You follow $N.", fch, ch, NULL, NULL, NULL, TO_CHAR, SENSE_SIXTH );
      move_char( fch, door );
    }
  }

  return;
}



void do_north( CHAR_DATA *ch, char *argument )
{
    move_char( ch, DIR_NORTH );
    return;
}



void do_east( CHAR_DATA *ch, char *argument )
{
    move_char( ch, DIR_EAST );
    return;
}



void do_south( CHAR_DATA *ch, char *argument )
{
    move_char( ch, DIR_SOUTH );
    return;
}



void do_west( CHAR_DATA *ch, char *argument )
{
    move_char( ch, DIR_WEST );
    return;
}



void do_up( CHAR_DATA *ch, char *argument )
{
    move_char( ch, DIR_UP );
    return;
}



void do_down( CHAR_DATA *ch, char *argument )
{
    move_char( ch, DIR_DOWN );
    return;
}


int get_move_cost( CHAR_DATA *ch, ROOM_INDEX_DATA *room_from,
  ROOM_INDEX_DATA *room_to )
{
  int cost;

  if (!IS_UNSWITCHED_MOB(ch) && IS_SET(ch->act, PLR_HOLYWALK))
    return 0;

  cost = room_from->move_cost + room_to->move_cost
    + ( ( room_from->sector->difficulty + room_to->sector->difficulty ) / 10 );

  cost /= 2;

  switch( ch->move_speed )
  {
    case MOVE_SNEAK:
    case MOVE_WALK:
    default:
      break;
    case MOVE_RUN:
      cost *= 2;
      break;
  }

  return UMAX( 0, cost );
}


int get_move_delay( CHAR_DATA *ch, ROOM_INDEX_DATA *room_from,
  ROOM_INDEX_DATA *room_to )
{
  int delay;

  if (!IS_UNSWITCHED_MOB(ch) && IS_SET(ch->act, PLR_HOLYWALK))
    return 0;

  delay = room_from->move_delay + room_to->move_delay
    + ( ( room_from->sector->difficulty + room_to->sector->difficulty ) / 3 );

  delay /= 2;

  switch( ch->move_speed )
  {
    case MOVE_SNEAK:
      delay *= 3;
      delay /= 2;
    case MOVE_WALK:
    default:
      break;
    case MOVE_RUN:
      delay /= 2;
      break;
  }

  return UMAX( 0, delay );
}


int get_direction( char *arg )
{
    if ( !str_cmp( arg, "n" ) || !str_cmp( arg, "north" ) ) return 0;
    if ( !str_cmp( arg, "e" ) || !str_cmp( arg, "east"  ) ) return 1;
    if ( !str_cmp( arg, "s" ) || !str_cmp( arg, "south" ) ) return 2;
    if ( !str_cmp( arg, "w" ) || !str_cmp( arg, "west"  ) ) return 3;
    if ( !str_cmp( arg, "u" ) || !str_cmp( arg, "up"    ) ) return 4;
    if ( !str_cmp( arg, "d" ) || !str_cmp( arg, "down"  ) ) return 5;

    return -1;
}


int find_door( CHAR_DATA *ch, char *arg )
{
    EXIT_DATA *pexit;
    int door;


    door = get_direction( arg );

    if ( door == -1 )
    {
	for ( door = 0; door <= 5; door++ )
	{
	    if ( ( pexit = ch->in_room->exit[door] ) != NULL
	    &&   IS_SET(pexit->exit_flags, EX_ISDOOR)
	    &&   pexit->keyword != NULL
	    &&   is_name( arg, pexit->keyword ) )
		return door;
	}
	act( "I see no $t here.", ch, NULL, NULL, arg, NULL, TO_CHAR, SENSE_SIXTH );
	return -1;
    }

    if ( ( pexit = ch->in_room->exit[door] ) == NULL )
    {
	act( "I see no door $T here.", ch, NULL, NULL, arg, NULL, TO_CHAR, SENSE_SIXTH );
	return -1;
    }

    if ( !IS_SET(pexit->exit_flags, EX_ISDOOR) )
    {
	send_to_char( "You can't do that.\n\r", ch );
	return -1;
    }

    return door;
}


void do_jump( CHAR_DATA *ch, char *argument )
{
    int dir;
    int pain;
    EXIT_DATA *pexit;
    char arg1[MAX_INPUT_LENGTH];
    char fence[MAX_STRING_LENGTH];

    one_argument( argument, arg1 );

    if ( arg1[0] == '\0' )
    {
      send_to_char( "What do you want to jump?\n\r", ch );
      return;
    }

    dir = get_direction( arg1 );

    if ( dir == -1 )
    {
	for ( dir = 0; dir <= 5; dir++ )
	{
	    if ( ( pexit = ch->in_room->exit[dir] ) != NULL
	    && ( IS_SET(pexit->exit_flags, EX_FENCE)
		|| IS_SET(pexit->exit_flags, EX_WALL) )
	    &&   pexit->keyword != NULL
	    &&   is_name( arg1, pexit->keyword ) )
		break;
	}

	if ( dir == 6 )
	{
	    act( "I see no $t here.", ch, NULL, NULL, arg1, NULL, TO_CHAR, SENSE_SIXTH );
	    return;
	}
    }

    if ( ( pexit = ch->in_room->exit[dir] ) == NULL
    || ( pexit->to_room == NULL ) )
    {
	send_to_char("No way to jump there.\n\r", ch );
	return;
    }

    if ( !IS_SET(pexit->exit_flags, EX_FENCE)
    && !IS_SET(pexit->exit_flags, EX_WALL) )
    {
	send_to_char("What are you trying to jump that for?\n\r", ch );
	return;
    }

    if ( pexit->keyword == NULL || pexit->keyword[0] == '\0' )
    {
	strcpy( fence, "fence" );
    }
    else
    {
	one_argument( pexit->keyword, fence );
    }

    act("You take a mighty leap...", ch, NULL, NULL, fence, NULL, TO_CHAR, SENSE_SIXTH );
    act("$n takes a mighty leap at $t...", ch, NULL, NULL, fence, NULL, TO_ROOM, SENSE_SIGHT );

    /* failure */
    pain = pexit->difficulty - dice(1,get_curr_dex(ch));
    if ( pain > 0 )
    {
	if ( dice(1,get_curr_dex(ch)) == 1 )
	{
	    damage( ch, ch, dice(2,pexit->difficulty), gsn_falling, NULL, gn_wear_none );
	    return;
	}
    }

    act("...fly over the $t...", ch, NULL, NULL, fence, NULL, TO_CHAR, SENSE_SIXTH );
    act("...and flies over $tward.", ch, NULL, NULL, dir_name[dir], NULL, TO_ROOM, SENSE_SIGHT );

    char_from_room( ch );

    char_to_room( ch, pexit->to_room, TRUE );

    act("$n flies in from $t...", ch, NULL, NULL, dir_name_2[rev_dir[dir]], NULL, TO_ROOM, SENSE_SIGHT );

    if ( pain > 0 )
    {
	damage( ch, ch, UMIN(dice(1,pain),dice(1,ch->hit[0])), gsn_tumbling, NULL, gn_wear_none );
    }
    else
    {
	send_to_char("...and land perfectly.\n\r", ch );
	act("...and lands perfectly having jumped the $t.", ch, NULL, NULL, fence, NULL, TO_ROOM, SENSE_SIGHT );
    }

    do_look( ch, "auto" );

    WAIT_STATE( ch, pexit->to_room->move_delay );

    return;
}

void do_knock( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    int door;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Knock on what?\n\r", ch );
	return;
    }


    if ( ( door = find_door( ch, arg ) ) >= 0 )
    {
	/* 'knock on it' */
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit = ch->in_room->exit[door];
	if ( !IS_SET(pexit->exit_flags, EX_CLOSED) )
	{
	    send_to_char( "It's open already\n\r",      ch );
	    return; 
	}

	
	act( "$n knocks on the $d.", ch, NULL, NULL, pexit->keyword, NULL, TO_ROOM, SENSE_SIGHT );
	act( "You rap on the $d.",ch,NULL,NULL,pexit->keyword,NULL,TO_CHAR,SENSE_SIXTH);

	/* open the other side */
	if ( ( to_room   = pexit->to_room               ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != NULL
	&&   pexit_rev->to_room == ch->in_room )
	{
	    CHAR_DATA *rch;

	    
	    for ( rch = to_room->people; rch != NULL; rch = rch->next_in_room )
		act( "A loud knock is heard on the $d.", rch, NULL, NULL, pexit_rev->keyword, NULL, TO_CHAR, SENSE_SIXTH );
	}
    }

    return;
}

void do_open( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int door;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Open what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    {
	/* 'open object' */
	if ( obj->item_type != ITEM_CONTAINER )
	    { send_to_char( "That's not a closable.\n\r", ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSED) )
	    { send_to_char( "It's already open.\n\r",      ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSEABLE) )
	    { send_to_char( "You can't do that.\n\r",      ch ); return; }
	if ( IS_SET(obj->value[1], CONT_LOCKED) )
	    { send_to_char( "It's locked.\n\r",            ch ); return; }

	REMOVE_BIT(obj->value[1], CONT_CLOSED);
	send_to_char( "Ok.\n\r", ch );
	act( "$n opens $p.", ch, NULL, obj, NULL, NULL, TO_ROOM, SENSE_SIGHT );
	return;
    }

    if ( ( door = find_door( ch, arg ) ) >= 0 )
    {
	/* 'open door' */
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit = ch->in_room->exit[door];
	if ( !IS_SET(pexit->exit_flags, EX_CLOSED) )
	    { send_to_char( "It's already open.\n\r",      ch ); return; }
	if (  IS_SET(pexit->exit_flags, EX_LOCKED) )
	    { send_to_char( "It's locked.\n\r",            ch ); return; }

	REMOVE_BIT(pexit->exit_flags, EX_CLOSED);
	act( "$n opens the $d.", ch, NULL, NULL, pexit->keyword, NULL, TO_ROOM, SENSE_SIGHT );
	send_to_char( "Ok.\n\r", ch );

	/* open the other side */
	if ( ( to_room   = pexit->to_room               ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != NULL
	&&   pexit_rev->to_room == ch->in_room )
	{
	    CHAR_DATA *rch;

	    REMOVE_BIT( pexit_rev->exit_flags, EX_CLOSED );
	    for ( rch = to_room->people; rch != NULL; rch = rch->next_in_room )
		act( "$N opens $d from the other side.", rch, ch, NULL, pexit_rev->keyword, NULL, TO_CHAR, SENSE_SIGHT );
	}
    }

    return;
}



void do_close( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int door;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Close what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    {
	/* 'close object' */
	if ( obj->item_type != ITEM_CONTAINER )
	    { send_to_char( "That's not a closable.\n\r", ch ); return; }
	if ( IS_SET(obj->value[1], CONT_CLOSED) )
	    { send_to_char( "It's already closed.\n\r",    ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSEABLE) )
	    { send_to_char( "You can't do that.\n\r",      ch ); return; }

	SET_BIT(obj->value[1], CONT_CLOSED);
	send_to_char( "Ok.\n\r", ch );
	act( "$n closes $p.", ch, NULL, obj, NULL, NULL, TO_ROOM, SENSE_SIGHT );
	return;
    }

    if ( ( door = find_door( ch, arg ) ) >= 0 )
    {
	/* 'close door' */
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit	= ch->in_room->exit[door];
	if ( IS_SET(pexit->exit_flags, EX_CLOSED) )
	    { send_to_char( "It's already closed.\n\r",    ch ); return; }

	SET_BIT(pexit->exit_flags, EX_CLOSED);
	act( "$n closes the $d.", ch, NULL, NULL, pexit->keyword, NULL, TO_ROOM, SENSE_SIGHT );
	send_to_char( "Ok.\n\r", ch );

	/* close the other side */
	if ( ( to_room   = pexit->to_room               ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != 0
	&&   pexit_rev->to_room == ch->in_room )
	{
	    CHAR_DATA *rch;

	    SET_BIT( pexit_rev->exit_flags, EX_CLOSED );
	    for ( rch = to_room->people; rch != NULL; rch = rch->next_in_room )
		act( "$N closes $d from the other side.", rch, ch, NULL, pexit_rev->keyword, NULL, TO_CHAR, SENSE_SIGHT );
	}
    }

    return;
}



bool has_key( CHAR_DATA *ch, int key )
{
    OBJ_DATA *obj;

    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
    {
	if ( obj->pIndexData->vnum == key )
	    return TRUE;
    }

    return FALSE;
}


void do_leave( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *building;

    for ( building = object_list; building != NULL; building = building->next )
    {
	if ( building->item_type != ITEM_BUILDING_OBJECT )
	    continue;

	if ( building->value[0] != ch->in_room->vnum )
	    continue;

	break;
    }

    if ( building == NULL )
    {
	send_to_char("Leave what?\n\r", ch );
	return;
    }

    if ( building->in_room == NULL )
    {
	send_to_char("Building room is non existant, please report.\n\r", ch );
	return;
    }

    act("You leave $t.", ch, NULL, NULL, building->short_descr, NULL, TO_CHAR, SENSE_SIXTH );
    act("$n leaves $t.", ch, NULL, NULL, building->short_descr, NULL, TO_ROOM, SENSE_SIGHT );
    char_from_room( ch );

    char_to_room( ch, building->in_room, TRUE );
    act("$n has arrived from $t.", ch, NULL, NULL, building->short_descr, NULL, TO_ROOM, SENSE_SIGHT );
    do_look( ch, "" );

    return;
}


void do_lock( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int door;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Lock what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    {
	/* 'lock object' */
	if ( obj->item_type != ITEM_CONTAINER )
	    { send_to_char( "That's not a lockable.\n\r", ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSED) )
	    { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( obj->value[2] < 0 )
	    { send_to_char( "It can't be locked.\n\r",     ch ); return; }
	if ( !has_key( ch, obj->value[2] ) )
	    { send_to_char( "You lack the key.\n\r",       ch ); return; }
	if ( IS_SET(obj->value[1], CONT_LOCKED) )
	    { send_to_char( "It's already locked.\n\r",    ch ); return; }

	SET_BIT(obj->value[1], CONT_LOCKED);
	send_to_char( "*Click*\n\r", ch );
	act( "$n locks $p.", ch, NULL, obj, NULL, NULL, TO_ROOM, SENSE_SIGHT );
	return;
    }

    if ( ( door = find_door( ch, arg ) ) >= 0 )
    {
	/* 'lock door' */
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit	= ch->in_room->exit[door];
	if ( !IS_SET(pexit->exit_flags, EX_CLOSED) )
	    { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( pexit->key < 0 )
	    { send_to_char( "It can't be locked.\n\r",     ch ); return; }
	if ( !IS_SET(ch->act, PLR_HOLYWALK) && !has_key( ch, pexit->key) )
	    { send_to_char( "You lack the key.\n\r",       ch ); return; }
	if ( IS_SET(pexit->exit_flags, EX_LOCKED) )
	    { send_to_char( "It's already locked.\n\r",    ch ); return; }

	SET_BIT(pexit->exit_flags, EX_LOCKED);
	send_to_char( "*Click*\n\r", ch );
	act( "$n locks the $d.", ch, NULL, NULL, pexit->keyword, NULL, TO_ROOM, SENSE_SIGHT );

	/* lock the other side */
	if ( ( to_room   = pexit->to_room               ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != 0
	&&   pexit_rev->to_room == ch->in_room )
	{
	    SET_BIT( pexit_rev->exit_flags, EX_LOCKED );
	}
    }

    return;
}



void do_unlock( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int door;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Unlock what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    {
	/* 'unlock object' */
	if ( obj->item_type != ITEM_CONTAINER )
	    { send_to_char( "That's not a lockable.\n\r", ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSED) )
	    { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( obj->value[2] < 0 )
	    { send_to_char( "It can't be unlocked.\n\r",   ch ); return; }
	if ( !has_key( ch, obj->value[2] ) )
	    { send_to_char( "You lack the key.\n\r",       ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_LOCKED) )
	    { send_to_char( "It's already unlocked.\n\r",  ch ); return; }

	REMOVE_BIT(obj->value[1], CONT_LOCKED);
	send_to_char( "*Click*\n\r", ch );
	act( "$n unlocks $p.", ch, NULL, obj, NULL, NULL, TO_ROOM, SENSE_SIGHT );
	return;
    }

    if ( ( door = find_door( ch, arg ) ) >= 0 )
    {
	/* 'unlock door' */
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit = ch->in_room->exit[door];
	if ( !IS_SET(pexit->exit_flags, EX_CLOSED) )
	    { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( pexit->key < 0 )
	    { send_to_char( "It can't be unlocked.\n\r",   ch ); return; }
	if ( !IS_SET(ch->act, PLR_HOLYWALK) && !has_key( ch, pexit->key) )
	    { send_to_char( "You lack the key.\n\r",       ch ); return; }
	if ( !IS_SET(pexit->exit_flags, EX_LOCKED) )
	    { send_to_char( "It's already unlocked.\n\r",  ch ); return; }

	REMOVE_BIT(pexit->exit_flags, EX_LOCKED);
	send_to_char( "*Click*\n\r", ch );
	act( "$n unlocks the $d.", ch, NULL, NULL, pexit->keyword, NULL, TO_ROOM, SENSE_SIGHT );

	/* unlock the other side */
	if ( ( to_room   = pexit->to_room               ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != NULL
	&&   pexit_rev->to_room == ch->in_room )
	{
	    REMOVE_BIT( pexit_rev->exit_flags, EX_LOCKED );
	}
    }

    return;
}



void do_pick( CHAR_DATA *ch, char *argument )
{
  FLORA_DATA *fi;
  char arg[MAX_INPUT_LENGTH];
  char arg1[MAX_INPUT_LENGTH];
  OBJ_DATA *obj;
  OBJ_DATA *pick;
  OBJ_INDEX_DATA *pObjIndex = NULL;
  int door;

  argument = one_argument( argument, arg );
  argument = one_argument( argument, arg1 );

  if ( arg[0] == '\0' )
  {
    send_to_char( "Pick what?\n\r", ch );
    return;
  }

  /* pick <obj> <plant> */
  if ( arg1[0] != '\0' )
  {
    fi = get_flora_here( ch, arg1 );
    if ( fi != NULL )
    {
      pObjIndex = get_obj_index( fi->species->fruit_vnum );
      if ( pObjIndex && is_name( arg, pObjIndex->name ) )
      {
        if ( fi->fruit <= 0 )
	{
	  send_to_char( "The plant is presently barren.\n\r", ch );
	  return;
	}
        obj = create_object( pObjIndex );
        obj_to_char( obj, ch );
        act( "$n picks $p.", ch, NULL, obj, NULL, NULL, TO_ROOM, SENSE_SIXTH );
        act( "You pick $p.", ch, NULL, obj, NULL, NULL, TO_CHAR, SENSE_SIXTH );
        update_fruit( fi );
        return;
      }

      pObjIndex = get_obj_index( fi->species->flower_vnum );
      if ( pObjIndex && is_name( arg, pObjIndex->name ) )
      {
        if ( fi->flower <= 0 )
        {
	  send_to_char( "The plant is presently barren.\n\r", ch );
	  return;
        }
        obj = create_object( pObjIndex );
        obj_to_char( obj, ch );
        act( "$n picks $p.", ch, NULL, obj, NULL, NULL, TO_ROOM, SENSE_SIGHT );
        act( "You pick $p.", ch, NULL, obj, NULL, NULL, TO_CHAR, SENSE_SIXTH );
        update_flower( fi );
        return;
      }
    }
  }

  for ( fi = ch->in_room->flora; fi != NULL; fi = fi->next )
  {
    pObjIndex = get_obj_index( fi->species->fruit_vnum );
    if ( pObjIndex && is_name( arg, pObjIndex->name ) )
    {
      if ( fi->fruit <= 0 )
      {
	send_to_char( "The plant is presently barren.\n\r", ch );
	return;
      }
      obj = create_object( pObjIndex );
      obj_to_char( obj, ch );
      act( "$n picks $p.", ch, NULL, obj, NULL, NULL, TO_ROOM, SENSE_SIGHT );
      act( "You pick $p.", ch, NULL, obj, NULL, NULL, TO_CHAR, SENSE_SIXTH );
      update_fruit( fi );
      return;
    }

    pObjIndex = get_obj_index( fi->species->flower_vnum );
    if ( pObjIndex && is_name( arg, pObjIndex->name ) )
    {
      if ( fi->flower <= 0 )
      {
	send_to_char( "The plant is presently barren.\n\r", ch );
	return;
      }
      obj = create_object( pObjIndex );
      obj_to_char( obj, ch );
      act( "$n picks $p.", ch, NULL, obj, NULL, NULL, TO_ROOM, SENSE_SIGHT );
      act( "You pick $p.", ch, NULL, obj, NULL, NULL, TO_CHAR, SENSE_SIXTH );
      update_flower( fi );
      return;
    }
  }

  if ( ( pick = get_eq_char( ch, gn_wear_primary ) ) == NULL )
  {
    send_to_char("What do you intend to pick it with, your fingers?\n\r", ch );
    return;
  }

  if ( pick->item_type != ITEM_LOCKPICKS )
  {
    send_to_char("You aren't wielding a suitable pick.\n\r", ch );
    return;
  }

  WAIT_STATE( ch, skill_table[gsn_pick_lock].beats );

  if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
  {
    /* 'pick object' */
    if ( obj->item_type != ITEM_CONTAINER )
      { send_to_char( "That's not a lockable.\n\r", ch ); return; }
    if ( !IS_SET(obj->value[1], CONT_CLOSED) )
      { send_to_char( "It's not closed.\n\r",        ch ); return; }
    if ( obj->value[2] < 0 )
      { send_to_char( "It can't be unlocked.\n\r",   ch ); return; }
    if ( !IS_SET(obj->value[1], CONT_LOCKED) )
      { send_to_char( "It's already unlocked.\n\r",  ch ); return; }
/*
    if ( IS_SET(obj->value[1], CONT_PICKPROOF) )
      { send_to_char( "You failed.\n\r",             ch ); return; }
*/
    if ( number_percent( ) > get_skill( ch, gsn_pick_lock ) )
    {
      send_to_char( "You failed.\n\r", ch);

      skill_improve(ch,gsn_pick_lock);
      return;
    }

    REMOVE_BIT(obj->value[1], CONT_LOCKED);
    send_to_char( "*Click*\n\r", ch );
    act( "$n picks $p.", ch, NULL, obj, NULL, NULL, TO_ROOM, SENSE_SIGHT );
    return;
  }

  if ( ( door = find_door( ch, arg ) ) >= 0 )
  {
    /* 'pick door' */
    ROOM_INDEX_DATA *to_room;
    EXIT_DATA *pexit;
    EXIT_DATA *pexit_rev;

    pexit = ch->in_room->exit[door];
    if ( !IS_SET(pexit->exit_flags, EX_CLOSED) )
      { send_to_char( "It's not closed.\n\r",        ch ); return; }
    if ( pexit->key < 0 )
      { send_to_char( "It can't be picked.\n\r",     ch ); return; }
    if ( !IS_SET(pexit->exit_flags, EX_LOCKED) )
      { send_to_char( "It's already unlocked.\n\r",  ch ); return; }
/*
    if ( IS_SET(pexit->exit_flags, EX_PICKPROOF) )
      { send_to_char( "You failed.\n\r",             ch ); return; }
*/
    if ( number_percent( ) > get_skill( ch, gsn_pick_lock ) )
    {
      send_to_char( "Your pick is destroyed trying to open the lock.\n\r", ch);

      damage_item( pick, pick->toughness + 1, TRUE, FALSE );

      if ( pexit->difficulty <= get_skill( ch, gsn_pick_lock ) )
        skill_improve(ch,gsn_pick_lock);
      return;
    }

    if ( pexit->difficulty > get_skill( ch, gsn_pick_lock ) )
    {
      send_to_char("This lock is beyond your capabilities.\n\r", ch );
      return;
    }

    if ( pick->value[0] < pexit->difficulty )
    {
      damage_item( pick, pexit->difficulty, TRUE, FALSE );
      send_to_char("Your pick is damaged attempting to pick the lock.\n\r",
        ch );
      return;
    }

    REMOVE_BIT(pexit->exit_flags, EX_LOCKED);
    send_to_char( "*Click*\n\r", ch );
    act( "$n skillfully picks the $d.",
      ch, NULL, NULL, pexit->keyword, NULL, TO_ROOM, SENSE_SIGHT );

    /* pick the other side */
    if ( ( to_room   = pexit->to_room               ) != NULL
    &&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != NULL
    &&   pexit_rev->to_room == ch->in_room )
    {
      REMOVE_BIT( pexit_rev->exit_flags, EX_LOCKED );
    }
  }

  return;
}




void do_stand( CHAR_DATA *ch, char *argument )
{
  extern bool execute_emote( CHAR_DATA *ch, char *arg, bool fPemote );

    switch ( ch->position )
    {
    case POS_SLEEPING:
	if ( IS_AFFECTED(ch, AFF_SLEEP) )
	    { send_to_char( "You can't wake up!\n\r", ch ); return; }

	send_to_char( "In your dreams or what?\n\r", ch );
	break;

    case POS_RESTING:
    case POS_SITTING:
        if ( argument[0] == '\0'
	|| execute_emote( ch, argument, FALSE ) )
	{
	  send_to_char( "You stand up.\n\r", ch );
	  act( "$n stands up.",
	    ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIGHT );
	}
	clear_dynamic( ch );
	ch->position = POS_STANDING;
	break;

    case POS_STANDING:
	send_to_char( "You are already standing.\n\r", ch );
	break;

    case POS_FIGHTING:
	send_to_char( "You are already fighting!\n\r", ch );
	break;
    }

    return;
}



void do_sit( CHAR_DATA *ch, char *argument )
{
  extern bool execute_emote( CHAR_DATA *ch, char *arg, bool fPemote );

    if ( check_fighting( ch ) )
    {
	send_to_char("While fighting?\n\r", ch );
	return;
    }

    switch ( ch->position )
    {
    case POS_SLEEPING:
	send_to_char( "In your dreams or what?\n\r", ch );
	break;

    case POS_RESTING:
        if ( argument[0] == '\0'
	|| execute_emote( ch, argument, FALSE ) )
	{
          send_to_char( "You sit up.\n\r", ch );
	  act( "$n sits up.",
	    ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIGHT );
        }
        clear_dynamic( ch );
	ch->position = POS_RESTING;

    case POS_SITTING:
	send_to_char( "You are already sitting.\n\r", ch );
	break;

    case POS_STANDING:
        if ( argument[0] == '\0'
	|| execute_emote( ch, argument, FALSE ) )
	{
	  send_to_char( "You sit down.\n\r", ch );
	  act( "$n sits down.",
	    ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIGHT );
        }
        clear_dynamic( ch );
	ch->position = POS_SITTING;
	break;

    case POS_FIGHTING:
	send_to_char( "You are already fighting!\n\r", ch );
	break;
    }

    return;
}


void do_rest( CHAR_DATA *ch, char *argument )
{
  extern bool execute_emote( CHAR_DATA *ch, char *arg, bool fPemote );

    if ( check_fighting( ch ) )
    {
	send_to_char("While fighting?\n\r", ch );
	return;
    }

    switch ( ch->position )
    {
    case POS_SLEEPING:
	send_to_char( "In your dreams or what?\n\r", ch );
	break;

    case POS_RESTING:
	send_to_char( "You are already resting.\n\r", ch );
	break;

    case POS_SITTING:
    case POS_STANDING:
        if ( argument[0] == '\0'
	|| execute_emote( ch, argument, FALSE ) )
	{
	  send_to_char( "You lie down to rest.\n\r", ch );
	  act( "$n lies down to rest.",
	    ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIGHT );
        }
        clear_dynamic( ch );
	ch->position = POS_RESTING;
	break;

    case POS_FIGHTING:
	send_to_char( "You are already fighting!\n\r", ch );
	break;
    }

    return;
}


void do_ride( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *mount;
    CHAR_DATA *rch;

    mount = get_char_room( ch, argument );

    if ( ch->riding != NULL )
    {
	send_to_char( "You are already mounted.\n\r", ch );
	return;
    }

    if ( mount == NULL )
    {
	send_to_char("No such mount here.\n\r", ch );
	return;
    }

    if ( !IS_NPC( mount ) || !IS_SET( ACT_MOUNT, mount->act ) )
    {
	send_to_char("You can't ride that.\n\r", ch );
	return;
    }

    for ( rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room )
    {
	if ( rch->riding == mount )
	    break;
    }

    if ( rch != NULL && rch->riding == mount )
    {
	send_to_char("Someone is already riding that mount.\n\r", ch );
	return;
    }

    act("You jump up on the back of $N.", ch, mount, NULL, NULL, NULL, TO_CHAR, SENSE_SIGHT );
    act("$n jumps up on the back of $N.", ch, mount, NULL, NULL, NULL,TO_ROOM, SENSE_SIGHT );
    ch->riding = mount;
    clear_dynamic( ch );
    clear_dynamic( mount );

    return;
}


void do_sleep( CHAR_DATA *ch, char *argument )
{
  extern bool execute_emote( CHAR_DATA *ch, char *arg, bool fPemote );

    if ( check_fighting( ch ) )
    {
	send_to_char("While fighting?\n\r", ch );
	return;
    }

    switch ( ch->position )
    {
    case POS_SLEEPING:
	send_to_char( "You are already sleeping.\n\r", ch );
	break;

    case POS_RESTING:
    case POS_SITTING:
    case POS_STANDING: 
        if ( argument[0] == '\0'
	|| execute_emote( ch, argument, FALSE ) )
	{
	  send_to_char( "You sleep.\n\r", ch );
	  act( "$n sleeps.", ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIGHT );
	}
	clear_dynamic( ch );
	ch->position = POS_SLEEPING;
	break;

    case POS_FIGHTING:
	send_to_char( "You are already fighting!\n\r", ch );
	break;
    }

    return;
}



void do_wake( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	if (ch->position != POS_SLEEPING)
	{
	    send_to_char("You aren't asleep.\n\r", ch );
	    return;
	}

    if ( ch->stun <= 0 )
    {
         send_to_char("You can't wake up.\n\r",ch);
         return;
    }
     
    
	send_to_char("You wake and sit up.\n\r", ch );
	act("$n wakes up.", ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIGHT );

        free_string( ch->dynamic_descr, MEM_GENERAL );
	ch->position = POS_RESTING;
        
        
	return;
    }

    if ( !IS_AWAKE(ch) )
	{ send_to_char( "You are asleep yourself!\n\r",       ch ); return; }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
	{ send_to_char( "They aren't here.\n\r",              ch ); return; }

    if ( IS_AWAKE(victim) )
	{ act( "$N is already awake.", ch, victim, NULL, NULL, NULL, TO_CHAR, SENSE_SIXTH ); return; }

    if ( IS_AFFECTED(victim, AFF_SLEEP) || victim->stun <= 0)
	{ act( "You can't wake $M!",   ch, victim, NULL, NULL, NULL, TO_CHAR, SENSE_SIXTH );  return; }

    act( "You wake $M.", ch, victim, NULL, NULL, NULL, TO_CHAR, SENSE_SIXTH );
    act( "$n wakes you.", ch, victim, NULL, NULL, NULL, TO_VICT, SENSE_SIGHT );
    clear_dynamic( victim );
    victim->position = POS_RESTING;
    return;
}



void do_walk( CHAR_DATA *ch, char *argument )
{
  ch->move_speed = MOVE_WALK;

  send_to_char( "You begin to walk.\n\r", ch );

  return;
}

void do_run( CHAR_DATA *ch, char *argument )
{
  ch->move_speed = MOVE_RUN;

  send_to_char( "You begin to run.\n\r", ch );

  return;
}

void do_sneak( CHAR_DATA *ch, char *argument )
{
  ch->move_speed = MOVE_SNEAK;

  send_to_char( "You begin to move silently.\n\r", ch );

  return;
}


void do_dismount( CHAR_DATA *ch, char *argument )
{
    if ( ch->riding == NULL )
    {
	send_to_char("You aren't riding.\n\r", ch );
	return;
    }

    act("You stop riding $N.", ch, ch->riding, NULL, NULL, NULL, TO_CHAR, SENSE_SIGHT );
    act("$n stops riding $N.", ch, ch->riding, NULL, NULL, NULL, TO_ROOM, SENSE_SIGHT );
    ch->riding = NULL;

    return;
}


void do_drive( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch;
    ROOM_INDEX_DATA *in_room;
    ROOM_INDEX_DATA *to_room;
 //   EXIT_DATA *pexit;
    char buf[MAX_STRING_LENGTH];
    char arg1[MAX_INPUT_LENGTH];
    OBJ_DATA *controls;
    OBJ_DATA *wagon = NULL;
    int direction;

    if (!str_cmp(argument, "u" ) || !str_cmp(argument, "up") || !str_cmp(argument, "d") || !str_cmp(argument, "down") )
    {
        send_to_char("Wagons can't go up and down.\n\r",ch);
        return;
    }

    for ( controls = ch->in_room->contents; controls != NULL; controls = controls->next_content )
    {
	if ( controls->item_type == ITEM_WAGON_CONTROLS )
	    break;
    }

    if ( controls == NULL )
    {
	send_to_char("Nothing to drive here.\n\r", ch );
	return;
    }

/* drive profi check */

    if ( ( wagon = get_obj_type( get_obj_index( controls->value[0] ) ) ) == NULL )
    {
	send_to_char("Error, can't find wagon for control panel.\n\r", ch );
	sprintf( buf, "** Error, can't find wagon # %d for control panel %d.",
	    controls->value[0], controls->pIndexData->vnum );
	bug( buf, 0 );
	return;
    }

    if ( wagon->item_type != ITEM_BUILDING_OBJECT )
    {
	send_to_char("Error, controls point to a non-building object.\n\r", ch );
	sprintf( buf, "** Error, controls %d point to non-building object %d.",
	    controls->pIndexData->vnum, controls->value[0] );
	bug( buf, 0 );
	return;
    }

    if ( wagon->in_room == NULL )
    {
	send_to_char("Error, wagon room is null.\n\r", ch );
	sprintf( buf, "** Error, wagon %d is in null room.",
	    wagon->pIndexData->vnum );
	bug( buf, 0 );
	return;
    }

    in_room = wagon->in_room;

    argument = one_argument( argument, arg1 );

    direction = get_direction( arg1 );

    if ( arg1[0] == '\0' )
	direction = -1;

    if ( direction == -1 )
    {
	send_to_char("Drive where? (n,e,s,w)\n\r", ch );
	return;
    }

	
	

    

	

/*	if ((direction == u) || (direction == d))
	{
		send_to_char("Wagons can't go up and down.\n\r",ch);
		return;
	}*/

/*    if (!str_cmp(arg1, "out" ) )
    {
        in_room = ch->in_room;
    char_from_room( ch );
    char_to_room( ch, to_room, TRUE );
    do_look( ch, "" );
    char_from_room( ch );
    char_to_room( ch, in_room, TRUE );
    return;
    }*/

    if ( in_room->exit[direction] == NULL
    || (to_room = in_room->exit[direction]->to_room ) == NULL )
    {
	send_to_char("There is no way to travel in that direction.\n\r", ch );
	return;
    }
    
	if ( IS_SET(in_room->exit[direction]->exit_flags, EX_CLOSED ) )
	{
	act("The $d is closed.",ch,NULL,NULL,in_room->exit[direction]->keyword,NULL,TO_CHAR,SENSE_SIXTH);
        return;
    }
  

    sprintf( buf, "%s rumbles off to the %s.\n\r",
	wagon->short_descr, dir_name[direction] );

    for ( vch = in_room->people; vch != NULL; vch = vch->next_in_room )
    {

	if ( ch == vch )
		continue;

		send_to_char( buf , vch );
		

    }


    in_room = ch->in_room;
    char_from_room( ch );
    char_to_room( ch, to_room, TRUE );
    do_look( ch, "" );
    char_from_room( ch );
    char_to_room( ch, in_room, TRUE );
	in_room = wagon->in_room;
    obj_from_room( wagon );
    obj_to_room( wagon, to_room, TRUE );
  
     

    sprintf( buf, "%s rumbles in from the %s.\n\r",
	wagon->short_descr, dir_name_2[rev_dir[direction]] );

    for ( vch = to_room->people; vch != NULL; vch = vch->next_in_room )
    {

	if ( ch == vch)
		continue;


	    send_to_char( buf, vch  );

	}
    sprintf( buf, "%s rumbles under you as you drive %s.\n\r",wagon->short_descr,
	dir_name[direction] );
    send_to_char( buf, ch );
	return;
	

    

    
}


void do_enter( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA *to_room;
    OBJ_DATA *building;
    bool fArg = TRUE;

    argument = one_argument( argument, arg1 );

    if ( arg1[0] == '\0' )
	fArg = FALSE;

    for ( building = ch->in_room->contents; building != NULL; building = building->next_content )
    {
	if ( building->item_type != ITEM_BUILDING_OBJECT )
	    continue;

	if ( fArg )
	{
	    if ( !is_name( arg1, building->name ) )
		continue;
	}

	break;
    }

    if ( building == NULL )
    {
	if ( fArg )
	    send_to_char("Nothing like that to enter here.\n\r", ch );
	else
	    send_to_char("Nothing to enter here.\n\r", ch );
	return;
    }

    if ( ( to_room = get_room_index( building->value[0] ) ) == NULL )
    {
	send_to_char("Error, bad room pointed to by building object.\n\r", ch );
	bug( "Enter: Bad room pointed to by building %d.", building->pIndexData->vnum );
	return;
    }

    act("You enter $t.", ch, NULL, NULL, building->short_descr, NULL, TO_CHAR, SENSE_SIXTH );
    act("$n enters $t.", ch, NULL, NULL, building->short_descr, NULL, TO_ROOM, SENSE_SIGHT );

    char_from_room( ch );
    char_to_room( ch, to_room, TRUE );
    do_look( ch, "" );

    act("$n has arrived from outside.", ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIGHT );

    return;
}


void do_hide( CHAR_DATA *ch, char *argument )
{
    send_to_char( "You attempt to hide.\n\r", ch );

    if ( IS_AFFECTED(ch, AFF_HIDE) )
	REMOVE_BIT(ch->affected_by, AFF_HIDE);

/* should automatically set the bit since handler takes care of checks */
/*    if ( number_percent( ) < get_skill( ch, gsn_hide ) ) */
	SET_BIT(ch->affected_by, AFF_HIDE);

    return;
}

/* Psionist Skills */

void do_contact( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char buf[MAX_INPUT_LENGTH];
 

    send_to_char( "You concentrate with your mind.\n\r", ch );
    WAIT_STATE( ch, skill_table[gsn_contact].beats );
    ch->stun -= 15;
   
    if ( ch->stun <= 0 )
    {
        ch->position = POS_SLEEPING;
        send_to_char("You start feeling woozie and pass out.\n\r",ch);
        act("$n's eyes roll back into $s head as $e crumples to the ground.", ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIXTH );
        clear_dynamic( ch );
        update_pos(ch);
        ch->dynamic_descr = str_dup("lays on the ground here, passed out cold.");
        return;
    }
   

    if ( argument[0] == '\0' )
    {
    send_to_char("Contact Whom?\n\r",ch);
    return;
    }
   
   

    if ( ( victim = get_char_world( ch, argument ) ) == NULL )
    {
        send_to_char( "Images race through your mind, but you can't find them.\n\r", ch );
        return;
    }

    if ( victim == ch )
    {
    send_to_char("You lose your concentration.\n\r",ch);
    return;
    }

    if ( ch->contact != NULL )
    {
       send_to_char("Your mind is wracked with pain as you try to contact another.\n\r",ch);
       ch->stun -= 25;
       update_pos(ch);
       return;
    }

    if ( number_percent( ) > get_skill( ch, gsn_contact ) )
    {
    send_to_char( "Your concentration faulters.\n\r", ch );
    skill_improve( ch, gsn_contact );
    return;
    }

    if ( number_percent( ) < get_skill( ch, gsn_contact ) )
    {
    ch->contact = victim->name;
        
    sprintf( buf, "You touch %s's mind.", victim->short_descr);
    send_to_char( buf, ch );
    send_to_char( "\n\r",ch );
    if ( !IS_NPC(victim) )
    {
    send_to_char( "You feel something touch the back of your mind.\n\r", victim );
    power_improve( ch, gsn_contact);
    return;
    }
    return;
    }
    



    return;
}

void do_tele( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    
    if ( get_skill(ch, gsn_contact) <= 0 )
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if ( argument[0] == '\0' )
    {
        send_to_char("What are you going to send?\n\r",ch );
        return;
    }

    if (ch->contact == NULL)
    {
        send_to_char("Your mind is not in contact with anyone at this time.\n\r",ch);
        return;
    }

    if (ch->contact != NULL )
    {
        if ( ch->stun <= 0 )
    {
        ch->position = POS_SLEEPING;
        send_to_char("You start to feel woozie and pass out.\n\r",ch);
        act("$n's eyes roll back into $s head as $e crumples to the ground.", ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIXTH );
        clear_dynamic(ch);
        update_pos(ch);
        ch->dynamic_descr = "lays here, passed out cold.";
        return;
    }

        victim = get_char_world( ch, ch->contact );
          
        ch->stun -= 10;
        sprintf(buf, "You send the following telepathic message to %s:\n   \"%s\"\n\r", ch->contact, argument );
        send_to_char( buf, ch);
        if ( !IS_NPC(victim) )
        {
        sprintf(buf, "%s projects into your mind:\n   \"%s\"\n\r", ch->short_descr, argument );
        send_to_char( buf, victim);
        return;
        }
        else
        {
        sprintf(log_buf, "%s projects to mobile %s\n   \"%s\"\n\r", ch->name, victim->short_descr, argument );
        log_string( log_buf);
        return;
        }
    }

}

void do_unlink( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    if ( ch->contact != NULL )
    {
        victim = get_char_world( ch, ch->contact );
        send_to_char("You feel something withdraw from your mind.\n\r", victim );
        ch->contact = NULL;
        send_to_char("You relax your mind.\n\r",ch);
        return;
    }
return;
}
   

/*
 * Contributed by Alander.
 */
void do_visible( CHAR_DATA *ch, char *argument )
{
    if (IS_SET(ch->affected_by, AFF_INVISIBLE) )
    {
        affect_strip ( ch, gsn_invis			);
        REMOVE_BIT   ( ch->affected_by, AFF_INVISIBLE	);
        send_to_char( "Your translucent form shimmers into view.\n\r", ch );
        act("$n shimmers into view.",ch,NULL,NULL,NULL,NULL,TO_ROOM,SENSE_SIXTH);
        return;
    }

    send_to_char("You aren't invisible.\n\r",ch);
    return;
}
