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
 *                                                                         *
 *  This module (C) 1994, 1995, 1996 Robert L. Peckham (Rip)               *
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

/* Local functions */
bool	save_zone_file( AREA_DATA *pArea );


bool can_build( CHAR_DATA *ch, AREA_DATA *pArea )
{
  if ( IS_NPC( ch ) )
    return FALSE;

  if ( pArea->builders == NULL )
    return FALSE;

  if ( ch->level >=38 )
  return TRUE;

  if ( is_name( ch->name, pArea->builders ) )
    return TRUE;

  return FALSE;
}


void do_export( CHAR_DATA *ch, char*argument )
{
    char arg1 [MAX_INPUT_LENGTH];

    if ( IS_NPC( ch ) )
	return;

    if ( ch->in_room == NULL )
    {
	send_to_char("You aren't in a room.\n\r", ch );
	return;
    }

    if ( !can_build( ch, ch->in_room->area ) )
    {
	send_to_char(
	  "You don't have authorization to build in this zone.\n\r", ch );
	return;
    }

    if ( ch->pnote == NULL || ch->pnote->text[0] == '\0' )
    {
	send_to_char("No note to export.\n\r", ch );
	return;
    }

/* these only need one argument */
    if ( !str_cmp( argument, "room" ) )
    {
	free_string( ch->in_room->description, MEM_ROOM_DESCRIPTION );
	ch->in_room->description = str_dup_tab( ch->pnote->text, TRUE );
	send_to_char("Room description copied from note buffer.\n\r", ch );
	return;
    }

/* these need more than one argument */
    argument = one_argument( argument, arg1 );

    if ( !str_cmp( arg1, "mob" ) )
    {
	CHAR_DATA *mob;
	MOB_INDEX_DATA *pMobIndex = NULL;

	if ( is_number( argument ) )
	{
	    pMobIndex = get_mob_index( atoi( argument ) );
	}
	else
	{
	    if ( (mob = get_char_room( ch, argument )) != NULL )
	    {
		if ( !IS_NPC( mob ) )
		{
		    send_to_char("Target must be a non-player.\n\r", ch );
		    return;
		}

		pMobIndex = mob->pIndexData;
	    }
	}

	if ( pMobIndex == NULL )
	{
	    send_to_char( "No such mobile.\n\r", ch );
	    return;
	}

	if ( !can_build( ch, pMobIndex->area ) )
	{
	    send_to_char(
      "You don't have authorization to build in that mobile's zone.\n\r", ch );
	    return;
	}

	free_string( pMobIndex->description, MEM_MOB_DESCRIPTION );
	pMobIndex->description = str_dup_tab( ch->pnote->text, TRUE );

	send_to_char("Text stored as mobile description.\n\r", ch );
	return;
    }

    if ( !str_cmp( arg1, "obj" ) )
    {
	OBJ_DATA *obj;
	OBJ_INDEX_DATA *pObjIndex = NULL;

	if ( is_number( argument ) )
	{
	    if ( (pObjIndex = get_obj_index( atoi( argument ) )) == NULL )
	    {
		send_to_char("No object with that vnum.\n\r", ch );
		return;
	    }
	}
	else
	{
	    if ( ( obj = get_obj_here( ch, argument ) ) == NULL )
	    {
		send_to_char("No such object here.\n\r", ch );
		return;
	    }

	    pObjIndex = obj->pIndexData;
	}

	if ( !can_build( ch, pObjIndex->area ) )
	{
	    send_to_char(
     "You do not have authorization to build in that object's zone.\n\r", ch );
	    return;
	}

	free_string( pObjIndex->description, MEM_OBJ_DESCRIPTION );
	pObjIndex->description = str_dup_tab( ch->pnote->text, TRUE );

	send_to_char("Text stored as object description.\n\r", ch );
	return;
    }

    if ( !str_cmp( arg1, "exit" ) )
    {
	EXIT_DATA *pexit;

	if ( get_direction( argument ) == -1 )
	{
	    send_to_char("Exit direction must be n/s/e/w/u/d.\n\r", ch );
	    return;
	}

	if ( ( pexit = ch->in_room->exit[get_direction( argument )]) == NULL )
	{
	    send_to_char("No exit data in that direction.\n\r", ch );
	    return;
	}

	free_string( pexit->description, MEM_DIRECTION_DESCRIPTION );
	pexit->description = str_dup_tab( ch->pnote->text, TRUE );
	send_to_char("Note buffer copied to exit description.\n\r", ch );
	return;
    }

/*    if ( !str_cmp( arg1, "cdesc" ) )
    {
        EXIT_DATA *pexit;

        if ( ( pexit = ch->in_room->exit[get_direction( argument )]) == NULL )
        {
            send_to_char("No exit data in that direction.\n\r", ch );
            return;
        }

        if ( get_direction( argument ) == -1 )
        {
            send_to_char("Cdesc Exit direction must be n/s/e/w/u/d.\n\r", ch );
            return;
        }

        if ( ( pexit = ch->in_room->exit[get_direction( argument )]) == NULL )
        {
            send_to_char("No exit data in that direction.\n\r", ch );
            return;
        }

        free_string( pexit->cdescription, MEM_DIRECTION_DESCRIPTION );
        pexit->cdescription = str_dup_tab( ch->pnote->text, TRUE );
        send_to_char("Note buffer copied to exit cdescription.\n\r", ch );
        return;
    }*/

    if ( !str_cmp( arg1, "extra" ) )
    {
	EXTRA_DESCR_DATA *ed;

	for ( ed = ch->in_room->extra_descr; ed != NULL; ed = ed->next )
	{
	    if ( is_name( argument, ed->keyword ) )
		break;
	}

	if ( ed == NULL )
	{
	    send_to_char("No such extra description in this room.\n\r", ch );
	    return;
	}

	free_string( ed->description, MEM_DIRECTION_DESCRIPTION );
	ed->description = str_dup_tab( ch->pnote->text, TRUE );
	send_to_char("Description copied to extra desc.\n\r", ch );
	return;
    }

    send_to_char("Usage: export (room/mob/object/exit/extra) <argument>.\n\r", ch );
    send_to_char("  mob and object store on the prototype.\n\r", ch );

    return;
}


void do_import( CHAR_DATA *ch, char*argument )
{
    char arg1 [MAX_INPUT_LENGTH];

    if ( IS_NPC( ch ) )
	return;

    if ( ch->in_room == NULL )
    {
	send_to_char("You aren't in a room.\n\r", ch );
	return;
    }

    if ( ch->pnote == NULL )
	do_note( ch, "subject editing note" );

/* these only need one argument */
    if ( !str_cmp( argument, "room" ) )
    {
	if ( ch->in_room->description == NULL )
	{
	    send_to_char("No description in this room to copy.\n\r", ch );
	    return;
	}

	free_string( ch->pnote->text, MEM_GENERAL );
	ch->pnote->text = str_dup_tab( ch->in_room->description, FALSE );
	send_to_char("Room description copied into note buffer.\n\r", ch );
	return;
    }

/* these need more than one argument */
    argument = one_argument( argument, arg1 );

    if ( !str_cmp( arg1, "mob" ) )
    {
	CHAR_DATA *mob;
	MOB_INDEX_DATA *pMobIndex = NULL;

	if ( is_number( argument ) )
	{
	    pMobIndex = get_mob_index( atoi( argument ) );
	}
	else
	{
	    if ( (mob = get_char_room( ch, argument )) != NULL )
	    {
		if ( !IS_NPC( mob ) )
		{
		    send_to_char("Target must be a non-player.\n\r", ch );
		    return;
		}

		pMobIndex = mob->pIndexData;
	    }
	}

	if ( pMobIndex == NULL )
	{
	    send_to_char( "No such mobile.\n\r", ch );
	    return;
	}

	if ( pMobIndex->description == NULL )
	{
	    send_to_char("Mobile has no description to copy.\n\r", ch );
	    return;
	}

	free_string( ch->pnote->text, MEM_GENERAL );
	ch->pnote->text = str_dup_tab( pMobIndex->description, FALSE );

	send_to_char("Text imported from mobile description.\n\r", ch );
	return;
    }

    if ( !str_cmp( arg1, "obj" ) )
    {
	OBJ_DATA *obj;
	OBJ_INDEX_DATA *pObjIndex = NULL;

	if ( is_number( argument ) )
	{
	    if ( (pObjIndex = get_obj_index( atoi( argument ) )) == NULL )
	    {
		send_to_char("No object with that vnum.\n\r", ch );
		return;
	    }
	}
	else
	{
	    if ( ( obj = get_obj_here( ch, argument ) ) == NULL )
	    {
		send_to_char("No such object here.\n\r", ch );
		return;
	    }

	    pObjIndex = obj->pIndexData;
	}

	if ( pObjIndex->description == NULL )
	{
	    send_to_char("Object has no description to copy.\n\r", ch );
	    return;
	}

	free_string( ch->pnote->text, MEM_GENERAL );
	ch->pnote->text = str_dup_tab( pObjIndex->description, FALSE );

	send_to_char("Text imported from object description.\n\r", ch );
	return;
    }

    if ( !str_cmp( arg1, "exit" ) )
    {
	EXIT_DATA *pexit;

	if ( get_direction( argument ) == -1 )
	{
	    send_to_char("Exit direction must be n/s/e/w/u/d.\n\r", ch );
	    return;
	}

	if ( ( pexit = ch->in_room->exit[get_direction( argument )]) == NULL )
	{
	    send_to_char("No exit data in that direction.\n\r", ch );
	    return;
	}

	if ( pexit->description == NULL )
	{
	    send_to_char("Exit has no description to copy.\n\r", ch );
	    return;
	}

	free_string( ch->pnote->text, MEM_GENERAL );
	ch->pnote->text = str_dup_tab( pexit->description, FALSE );
	send_to_char("Note buffer copied from exit description.\n\r", ch );
	return;
    }

    if ( !str_cmp( arg1, "extra" ) )
    {
	EXTRA_DESCR_DATA *ed;

	for ( ed = ch->in_room->extra_descr; ed != NULL; ed = ed->next )
	{
	    if ( is_name( argument, ed->keyword ) )
		break;
	}

	if ( ed == NULL )
	{
	    send_to_char("No such extra description in this room.\n\r", ch );
	    return;
	}

	if ( ed->description == NULL )
	{
	    send_to_char("Extra has no description to copy.\n\r", ch );
	    return;
	}

	free_string( ch->pnote->text, MEM_GENERAL );
	ch->pnote->text = str_dup_tab( ed->description, FALSE );
	send_to_char("Description copied from extra desc.\n\r", ch );
	return;
    }

    send_to_char("Usage: import (room/mob/object/exit/cdesc/extra) <argument>.\n\r", ch );

    return;
}


void do_break( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *room = ch->in_room;
    int direction = -1;

    if ( IS_NPC( ch ) )
	return;

    if ( !can_build( ch, ch->in_room->area ) )
    {
	send_to_char(
	  "You don't have authorization to build in this zone.\n\r", ch );
	return;
    }

    if ( !str_cmp( argument, "all" ) )
    {
	for ( direction = 0; direction <= 5; direction++ )
	{
	    if ( room->exit[direction] == NULL )
		continue;

	    if ( room->exit[direction]->keyword[0] == '\0'
		&& room->exit[direction]->description[0] == '\0' )
	    {
		free_exit( room->exit[direction] );
		room->exit[direction] = NULL;
	    }
	    else
	    {
		room->exit[direction]->to_room = NULL;
		room->exit[direction]->vnum = 0;
		room->exit[direction]->exit_flags = 0;
		room->exit[direction]->key = 0;
	    }
	}

	send_to_char("All exits broken.\n\r", ch );
	return;
    }

    direction = get_direction( argument );

    if ( direction == -1 )
    {
	send_to_char("Syntax:   break <direction>  (north/east, etc.  or all).\n\r", ch );
	return;
    }

    if ( room->exit[direction] == NULL )
    {
	send_to_char("No exit in that direction.\n\r", ch );
	return;
    }

    if ( room->exit[direction]->keyword[0] == '\0'
    && room->exit[direction]->description[0] == '\0' )
    {
	free_exit( room->exit[direction] );
	room->exit[direction] = NULL;
    }
    else
    {
	room->exit[direction]->to_room = NULL;
	room->exit[direction]->vnum = 0;
	room->exit[direction]->exit_flags = 0;
	room->exit[direction]->key = 0;
    }

    send_to_char("Exit removed.\n\r", ch );
    return;
}


void do_collaps( CHAR_DATA *ch, char *argument )
{
    send_to_char("You need to type collapse all the way out.\n\r", ch );
    return;
}

void do_collapse( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC( ch ) )
	return;

    if ( !can_build( ch, ch->in_room->area ) )
    {
	send_to_char("You don't have authorization to build in this zone.\n\r", ch );
	return;
    }

    if ( ch->in_room->vnum == ROOM_VNUM_CHAT )
    {
	send_to_char("You can't collapse the chat room.\n\r", ch );
	return;
    }

    act("$n collapses the room.", ch, NULL, NULL,
	NULL, NULL, TO_ROOM, SENSE_SIXTH );

    free_room( ch->in_room );

    send_to_char("Room deleted.\n\r", ch );
    return;
}

void do_connect( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    extern sh_int rev_dir[];
    int vnum;
    int direction = -1;
    bool fOneway = FALSE;
    ROOM_INDEX_DATA *pRoomIndex;
    EXIT_DATA *pexit = NULL;

    if ( IS_NPC( ch ) )
	return;

    if ( !can_build( ch, ch->in_room->area ) )
    {
	send_to_char(
	  "You do not have authorization to build in this zone.\n\r", ch );
	return;
    }

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char(" Syntax:  connect <direction> <vnum> [oneway]\n\r", ch );
	send_to_char(" [oneway] is optional parameter.  Exits are two way\n\r", ch );
	send_to_char(" by default.  Anything other than 'oneway' in that\n\r", ch );
	send_to_char(" field will be ignored.\n\r", ch );
	return;
    }

    if ( !is_number( arg2 ) )
    {
	send_to_char("Target room vnum must be numeric.\n\r", ch );
	return;
    }
    vnum = atoi( arg2 );

    direction = get_direction( arg1 );

    if ( direction == -1 )
    {
	do_connect( ch, "" );
	return;
    }

    if ( !str_cmp( argument, "oneway" ) || !str_cmp( argument, "one" ) )
	fOneway = TRUE;

    if ( (pRoomIndex = get_room_index( vnum )) == NULL )
    {
	send_to_char("Target room number does not exist.\n\r", ch );
	return;
    }

    if ( (pexit = ch->in_room->exit[direction]) == NULL )
    {
	pexit = alloc_perm( sizeof(*pexit) );
	pexit->to_room = pRoomIndex;
	pexit->description = str_dup( "\0" );
//        pexit->cdescription = str_dup( "\0" );
	pexit->keyword = str_dup( "\0" );
	pexit->exit_flags = 0;
	pexit->key = 0;
	pexit->vnum = vnum;
	ch->in_room->exit[direction] = pexit;
    }
    else
    {
	pexit->to_room = pRoomIndex;
        pexit->vnum = vnum;
        ch->in_room->exit[direction] = pexit;

    }

    if ( !fOneway )
    {
	if ( (pexit = pRoomIndex->exit[rev_dir[direction]]) == NULL )
	{
	    pexit = alloc_perm( sizeof(*pexit) );
	    pexit->to_room = ch->in_room;
	    pexit->description = str_dup( "\0" );
//            pexit->cdescription = str_dup( "\0" );
	    pexit->keyword = str_dup( "\0" );
	    pexit->exit_flags = 0;
	    pexit->key = 0;
	    pexit->vnum = ch->in_room->vnum;
	    pRoomIndex->exit[rev_dir[direction]] = pexit;
	}
	else
	{
	    if ( pexit->to_room != NULL )
	    {
		send_to_char("Reverse exit already exists.  Forward entrance only created.\n\r", ch );
		return;
	    }

	    pexit->to_room = ch->in_room;
	    pexit->vnum = ch->in_room->vnum;
	}
    }

    send_to_char("Exit connected.\n\r", ch );
    char_from_room( ch );
    char_to_room( ch, pRoomIndex, FALSE );
    do_look( ch, "auto" );
    return;
}


ROOM_INDEX_DATA *new_room( AREA_DATA *pArea, int vnum )
{
  ROOM_INDEX_DATA *pRoomIndex;
  extern ROOM_INDEX_DATA *room_free;
  extern ROOM_INDEX_DATA *room_index_hash[MAX_KEY_HASH];
  extern int top_room;
  int iHash;

  if ( room_free == NULL )
  {
    pRoomIndex = alloc_perm( sizeof(*pRoomIndex));
  }
  else
  {
    pRoomIndex = room_free;
    room_free = pRoomIndex->next;
  }

  if ( pRoomIndex == NULL )
  {
    log_string( "Error opening new room." );
    return NULL;
  }

  pRoomIndex->people = NULL;
  pRoomIndex->contents = NULL;
  pRoomIndex->extra_descr = NULL;
  pRoomIndex->flora = NULL;
  pRoomIndex->area = pArea;
  pRoomIndex->vnum = vnum;
  pRoomIndex->name = str_dup( "New Room." );
  pRoomIndex->description = str_dup( "Empty description.\n" );
  pRoomIndex->room_flags = 0;
  set_sector( pRoomIndex, sector_lookup( "city", NULL ) );
  pRoomIndex->move_cost = 0;
  pRoomIndex->move_delay = 0;
  pRoomIndex->light = 0;
  pRoomIndex->light_mod = 0;
  pRoomIndex->light_filter = 0;
  pRoomIndex->size = 0;
  pRoomIndex->spec_fun_name = str_dup( "\0" );
  pRoomIndex->spec_fun = NULL;

  iHash = vnum % MAX_KEY_HASH;
  pRoomIndex->next = room_index_hash[iHash];
  room_index_hash[iHash] = pRoomIndex;
  pRoomIndex->zone_next = NULL;
  top_room++;

  if ( pArea->room_first == NULL )
  {
    pRoomIndex->area->room_first = pRoomIndex;
    pRoomIndex->area->room_last = pRoomIndex;
  }
  else
  {
    pRoomIndex->area->room_last->zone_next = pRoomIndex;
    pRoomIndex->area->room_last = pRoomIndex;
  }

  return pRoomIndex;
}


void do_dig( CHAR_DATA *ch, char *argument )
{
    extern sh_int rev_dir[];
    int door;
    int direction;
    int vnum;
    bool fEonly = FALSE;
    char arg1 [MAX_INPUT_LENGTH];
    char buf [MAX_STRING_LENGTH];
    ROOM_INDEX_DATA *pRoomIndex;
    EXIT_DATA *pexit = NULL;

    if ( IS_NPC( ch ) )
	return;

    if ( !can_build( ch, ch->in_room->area ) )
    {
	send_to_char(
	  "You do not have authorization to build in this zone.\n\r", ch );
	return;
    }

    argument = one_argument( argument, arg1 );

    if ( arg1[0] != '\0' && !str_cmp( arg1, "list" ) )
    {
      int col = 0;

      sprintf( buf, "Room vnums in use for %s.\n\r", ch->in_room->area->name );
      send_to_char( buf, ch );

      for ( pRoomIndex = ch->in_room->area->room_first;
        pRoomIndex != NULL; pRoomIndex = pRoomIndex->zone_next )
      {
        sprintf( buf, " %6d", pRoomIndex->vnum );
        send_to_char( buf, ch );
	if ( ( ++col % 10 ) == 0 )
	  send_to_char( "\n\r", ch );
      }

      if ( col % 10 != 0 )
        send_to_char( "\n\r", ch );

      return;
    }

    if ( arg1[0] == '\0' || argument[0] == '\0' )
    {
	send_to_char(" Syntax:  dig new <vnum>\n\r",		ch );
	send_to_char("          dig (north/south/east etc) <vnum>\n\r", ch );
	send_to_char("          dig list\n\r", ch );
	return;
    }

    if ( !is_number( argument ) )
    {
	send_to_char("Vnum must be numeric.\n\r", ch );
	return;
    }
    vnum = atoi( argument );

/*  add vnum checking    */
    if ( get_room_index( vnum ) != NULL )
    {
	send_to_char("Vnums can not be duplicated.\n\r", ch );
	return;
    }

    if ( vnum < ch->in_room->area->vnum_start || vnum > ch->in_room->area->vnum_final )
    {
	send_to_char("Vnum out of range.\n\r", ch );
	sprintf( buf, "Vnum must be in range %d to %d.\n\r",
	    ch->in_room->area->vnum_start,
	    ch->in_room->area->vnum_final );
	send_to_char( buf, ch );
	return;
    }

    direction = 10;

    if ( !str_cmp( arg1, "new" ) )
	direction = -1;

    direction = get_direction( arg1 );

    if ( direction > 5 )
    {
	do_dig( ch, "" );
	return;
    }

    if ( direction != -1 && ((pexit = ch->in_room->exit[direction]) != NULL))
    {
	if ( pexit->to_room != NULL )
	{
	    send_to_char("There is already an exit in that direction.\n\r", ch );
	    return;
	}
	fEonly = TRUE;
    }

    if ( ( pRoomIndex = new_room( ch->in_room->area, vnum ) ) == NULL )
      return;

    for ( door = 0; door <= 5; door++ )
    {
	pRoomIndex->exit[door] = NULL;
    }

    if ( direction != -1 )
    {
	if (fEonly)
	{
	    pexit->to_room = pRoomIndex;
	}
	else
	{
/****fixmefixme****/
	    pexit = alloc_mem( sizeof(*pexit) );
	    pexit->to_room = pRoomIndex;
	    pexit->description = str_dup( "\0" );
//            pexit->cdescription = str_dup( "\0" );
	    pexit->keyword = str_dup( "\0" );
	    pexit->exit_flags = 0;
	    pexit->key = 0;
	    pexit->vnum = vnum;
	    ch->in_room->exit[direction] = pexit;
	}

	pexit = alloc_perm( sizeof(*pexit) );
	pexit->to_room = ch->in_room;
	pexit->description = str_dup( "\0" );
//        pexit->cdescription = str_dup( "\0" );
	pexit->keyword = str_dup( "\0" );
	pexit->exit_flags = 0;
	pexit->key = 0;
	pexit->vnum = ch->in_room->vnum;
	pRoomIndex->exit[rev_dir[direction]] = pexit;
    }

    send_to_char("Room created.\n\r", ch );
    char_from_room( ch );
    char_to_room( ch, pRoomIndex, FALSE );
    do_look( ch, "auto" );
    return;
}


void do_exset( CHAR_DATA *ch, char *argument )
{
    char direction [MAX_INPUT_LENGTH];
    char command [MAX_INPUT_LENGTH];
    char buf [MAX_STRING_LENGTH];
    EXIT_DATA *pexit;
    extern sh_int rev_dir[];
    int door = -1;

    if ( IS_NPC( ch ) )
	return;

    if ( !can_build( ch, ch->in_room->area ) )
    {
	send_to_char("You don't have authorization to build in this zone.\n\r", ch );
	return;
    }

    argument = one_argument( argument, direction );
    argument = one_argument( argument, command );

    if ( direction[0] == '\0' || command[0] == '\0' )
    {
	send_to_char("Syntax:\n\r", ch );
	send_to_char(" exset <direction> <command> <argument>\n\r", ch );
	send_to_char("\n\r", ch );
	send_to_char("Where argument is one of the following:\n\r", ch );
	send_to_char("desc cldesc key door/flags keyword lock/diff\n\r", ch );
	return;
    }

    door = get_direction( direction );

    if ( door == -1 )
    {
	send_to_char("Invalid direction.\n\r", ch );
	return;
    }

    if ( (pexit = ch->in_room->exit[door]) == NULL )
    {
	send_to_char("No exit data in that direction.  Creating entry.\n\r", ch );

	pexit = alloc_perm( sizeof( *pexit ) );
	pexit->to_room = NULL;
	pexit->description = str_dup( "\0" );
//        pexit->cdescription = str_dup( "\0" );
	pexit->keyword = str_dup( "\0" );
	pexit->exit_flags = 0;
	pexit->key = 0;
	pexit->vnum = -1;
	pexit->difficulty = 5;
	ch->in_room->exit[door] = pexit;
    }

    if ( !str_cmp( command, "cldesc" ) )
    {
	buf[0] = '\0';

	free_string( pexit->description, MEM_DIRECTION_DESCRIPTION );
	pexit->description = str_dup( buf );

	send_to_char("Exit description cleared.\n\r", ch );
	return;
    }

/*    if ( !str_cmp( command, "clcdesc" ) )
    {
        buf[0] = '\0';

        free_string( pexit->cdescription, MEM_DIRECTION_DESCRIPTION );
        pexit->cdescription = str_dup( buf );

        send_to_char("Exit Cdescription cleared.\n\r",ch );
        return;
    } */

    if ( !str_cmp( command, "desc" ) )
    {
	strcpy( buf, pexit->description );

	if ( strlen( buf ) + strlen( argument ) >= MAX_STRING_LENGTH -4 )
	{
	    send_to_char("Exit description too long, text not added.\n\r", ch );
	    return;
	}

	if ( buf[0] == '\0' )
	    strcpy( buf, argument );
	else
	    strcat( buf, argument );
	strcat( buf, "\n\r" );
	free_string( pexit->description, MEM_DIRECTION_DESCRIPTION );
	pexit->description = str_dup( buf );

	send_to_char("Text added to exit description.\n\r", ch );
	return;
    }

    if ( !str_cmp( command, "cdesc" ) )
    {
        send_to_char("Please import the cdesc, write it, and export it.\n\r",ch );
        send_to_char("Feature Disabled.\n\r",ch);
        return;
    }

    if ( !str_cmp( command, "door" ) || !str_cmp( command, "flags" ) )
    {
	if (pexit->to_room == NULL)
	{
	    send_to_char("No exit, no door..\n\r", ch );
	    return;
	}

	if ( argument[0] == '\0' || !is_number( argument ) )
	{
	    send_to_char("door status must be numeric\n\r", ch );
	    return;
	}

	pexit->exit_flags = atoi( argument );

	if ( pexit->to_room->exit[rev_dir[door]] != NULL
	&& pexit->to_room->exit[rev_dir[door]]->to_room == ch->in_room )
	{
	    pexit->to_room->exit[rev_dir[door]]->exit_flags = atoi( argument );
	}

	return;
    }

    if ( !str_cmp( command, "key" ) )
    {
	if (pexit->to_room == NULL)
	{
	    send_to_char("No exit, no door..\n\r", ch );
	    return;
	}

	if (argument[0] == '\0' || !is_number( argument ) )
	{
	    send_to_char("key vnum must be numeric\n\r", ch );
	    return;
	}

	pexit->key = atoi( argument );

	return;
    }

    if ( !str_cmp( command, "keyword" ) )
    {
	if ( argument == NULL )
	{
	    send_to_char("Set what as keyword?\n\r", ch );
	    return;
	}
	free_string( pexit->keyword, MEM_DIRECTION_KEYWORD );
	pexit->keyword = str_dup( argument );
	return;
    }

    if ( !str_cmp( command, "lock" ) || !str_cmp( command, "diff" ) )
    {
	int level;

	if ( argument[0] == '\0' || !is_number( argument ) )
	{
	    send_to_char("Argument must be numeric.\n\r", ch );
	    return;
	}

	level = atoi( argument );

	if ( level < 0 || level > 100 )
	{
	    send_to_char("Lock level range is 0 to 95.\n\r", ch );
	    return;
	}

	pexit->difficulty = level;
	return;
    }

    do_exset( ch, "" );

    return;
}


void do_hset( CHAR_DATA *ch, char *argument )
{
    char keyword [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    HELP_DATA *pHelp;

    argument = one_argument( argument, keyword );
    argument = one_argument( argument, arg2 );

    if ( keyword[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char("Syntax:\n\r", ch );
	send_to_char("hset <keyword> <operation> (value/string)\n\r", ch );
	send_to_char("\noperation being one of the following:\n\r", ch );
	send_to_char("level, add, clear\n\r", ch );
	return;
    }

    for ( pHelp = help_first ; pHelp != NULL ; pHelp = pHelp->next )
    {
	if ( pHelp->level > get_trust( ch ) )
	    continue;

	if ( is_name( keyword, pHelp->keyword ) )
	    break;
    }

    if ( pHelp == NULL )
    {
	send_to_char("No such help file.\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "level" ) )
    {
	int value;

	value = is_number( argument ) ? atoi( argument ) : -1;

	if (value > get_trust(ch) || value < 0)
	{
	    send_to_char("Level must be between 0 and your level.\n\r", ch );
	    return;
	}

	pHelp->level = value;

	send_to_char("Help file level set.\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "add" ) )
    {
	char buf[MAX_STRING_LENGTH];

	strcpy( buf, pHelp->text );
	if ( strlen( buf ) + strlen( argument ) >= MAX_STRING_LENGTH-4 )
	{
	    send_to_char("Help too long to add to it.\n\r", ch );
	    return;
	}

	if ( buf[0] == '\0' )
	    strcpy( buf, argument );
	else
	    strcat( buf, argument );
	strcat( buf, "\n\r" );
	free_string( pHelp->text, MEM_GENERAL );
	pHelp->text = str_dup( buf );

	send_to_char("Text appended to help file.\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "clear" ) )
    {
	char buf[MAX_STRING_LENGTH];

	buf[0] = '\0';

	free_string( pHelp->text, MEM_GENERAL );
	pHelp->text = str_dup( buf );

	send_to_char("Help file cleared.\n\r", ch );
	return;
    }

    do_hset( ch, "" );

    return;
}

void do_mpset( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    CHAR_DATA *mob;
    MOB_INDEX_DATA *pMobIndex = NULL;
    int value;

    if ( IS_NPC( ch ) )
	return;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' || argument[0] == '\0' )
    {
	send_to_char( "Syntax: mpset <vnum/name> <field>  <value>\n\r",	ch );
	send_to_char( "\n\r",						ch );
	send_to_char( "Field being one of:\n\r",			ch );
	send_to_char( "  str int dex tou off def sex class level\n\r",	ch );
	send_to_char( "  coins hits damage absorb align\n\r",		ch );
	send_to_char( "  act affected start_pos race\n\r",		ch );
	send_to_char( "  shop aggr skill clan speed skin\n\r",		ch );
	send_to_char( "\n\r",						ch );
	send_to_char( "String being one of:\n\r",			ch );
	send_to_char( "  name short long desc spec\n\r",	ch );
	return;
    }

    if ( is_number( arg1 ) )
    {
	pMobIndex = get_mob_index( atoi( arg1 ) );
    }
    else
    {
	if ( (mob = get_char_room( ch, arg1 )) != NULL )
	{
/*	    if ( !IS_NPC( mob ) )
	    {
		send_to_char("Target must be a non-player.\n\r", ch );
		return;
	    }
*/
	    pMobIndex = mob->pIndexData;
	}
    }

    if ( pMobIndex == NULL )
    {
	send_to_char( "No such mobile.\n\r", ch );
	return;
    }

    if ( !can_build( ch, pMobIndex->area ) )
    {
	send_to_char("You don't have authorization to build in that mobile's zone.\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "name" ) )
    {
	free_string( pMobIndex->name, MEM_MOB_NAME );
	pMobIndex->name = str_dup( argument );
	return;
    }

    if ( !str_cmp( arg2, "namelist" ) )
    {
	free_string( pMobIndex->namelist, MEM_MOB_NAMELIST );
	pMobIndex->namelist = str_dup( argument );
	return;
    }

    if ( !str_cmp( arg2, "short" ) )
    {
	free_string( pMobIndex->short_descr, MEM_MOB_SHORT_DESCR );
	pMobIndex->short_descr = str_dup( argument );
	return;
    }

    if ( !str_cmp( arg2, "long" ) )
    {
	char buf[MAX_STRING_LENGTH];

	free_string( pMobIndex->long_descr, MEM_MOB_LONG_DESCR );

	sprintf( buf, "%s\n\r", argument );
	pMobIndex->long_descr = str_dup( buf );
	return;
    }


    /*
     * Everything after here has separate arg3.
     */
    argument = one_argument( argument, arg3 );
    /*
     * Snarf the value (which need not be numeric).
     */
    value = is_number( arg3 ) ? atoi( arg3 ) : -1;


    /*
     * Set something.
     */
    if ( !str_cmp( arg2, "act" ) )
    {
	if ( value < 0 )
	{
	    send_to_char("Act by must be positive or zero.\n\r", ch );
	    return;
	}

	pMobIndex->act = value;

	if ( pMobIndex->pShop == NULL )
	{
	    if ( IS_SET( pMobIndex->act, ACT_SHOPKEEPER ) )
		pMobIndex->act -= ACT_SHOPKEEPER;
	}
	else
	{
	    if ( !IS_SET( pMobIndex->act, ACT_SHOPKEEPER ) )
		pMobIndex->act += ACT_SHOPKEEPER;
	}

	if ( !IS_SET( pMobIndex->act, ACT_IS_NPC ) )
	    pMobIndex->act += ACT_IS_NPC;

	return;
    }

    if ( !str_cmp( arg2, "affected" ) )
    {
	if ( value < 0 )
	{
	    send_to_char("Affected by must be positive or zero.\n\r", ch );
	    return;
	}

	pMobIndex->affected_by = value;
	return;
    }

    if ( !str_cmp( arg2, "aggr" ) )
    {
	if ( value < 0 )
	{
	    send_to_char("Aggression must be positive or zero.\n\r", ch );
	    return;
	}

	pMobIndex->aggression = value;
	return;
    }

    if ( !str_cmp( arg2, "start_pos" ) )
    {
	if ( value < 4 || value > 7 )
	{
	    send_to_char( "Start position range is 4 to 7.\n\r", ch );
	    return;
	}

	pMobIndex->start_pos = value;
	return;
    }

    if ( !str_cmp( arg2, "race" ) )
    {
	if ( value < 0 || value > 20 )
	{
	    send_to_char( "Race must be between 0 and 20.\n\r", ch );
	    return;
	}

	pMobIndex->race = value;
	return;
    }

    if ( !str_cmp( arg2, "speed" ) )
    {
	if ( value < 0 || (value > 128 && get_trust(ch) < 38) )
	{
	    send_to_char( "Speed range is 0 to 128.\n\r", ch );
	    return;
	}

	pMobIndex->speed = value;
	return;
    }

    if ( !str_cmp( arg2, "skin" ) )
    {
	if ( !str_cmp( arg3, "clear") )
	{
	    pMobIndex->skin_objects[0] = 0;
	    pMobIndex->skin_objects[1] = 0;
	    pMobIndex->skin_objects[2] = 0;
	    pMobIndex->skin_objects[3] = 0;

	    send_to_char("Mobile's skin data cleared.\n\r", ch );
	    return;
	}

	if ( ( get_obj_index( value ) ) == NULL )
	{
	    send_to_char( "No object with that vnum.\n\r", ch );
	    return;
	}

	if ( pMobIndex->skin_objects[0] == 0 )
	    pMobIndex->skin_objects[0] = value;
	else if ( pMobIndex->skin_objects[1] == 0 )
	    pMobIndex->skin_objects[1] = value;
	else if ( pMobIndex->skin_objects[2] == 0 )
	    pMobIndex->skin_objects[2] = value;
	else if ( pMobIndex->skin_objects[3] == 0 )
	    pMobIndex->skin_objects[3] = value;
	else
	{
	    send_to_char("All 4 skin slots are filled.\n\r", ch );
	    send_to_char("use: mpset skin clear  to free all slots.\n\r", ch );
	}

	return;
    }

    if ( !str_cmp( arg2, "clan" ) )
    {
	if ( value < 0 || value > MAX_CLAN )
	{
	    send_to_char( "Clan range is 0 to 10.\n\r", ch );
	    return;
	}

	pMobIndex->clan = p2( value-1 );
	return;
    }

    if ( !str_cmp( arg2, "str" ) )
    {
	if ( value < 3 || value > 200 )
	{
	    send_to_char( "Strength range is 3 to 200.\n\r", ch );
	    return;
	}

	pMobIndex->mob_stat_phys_str = value;
	return;
    }

    if ( !str_cmp( arg2, "int" ) )
    {
	if ( value < 3 || value > 200 )
	{
	    send_to_char( "Intelligence range is 3 to 200.\n\r", ch );
	    return;
	}

	pMobIndex->mob_stat_ment_int = value;
	return;
    }

    if ( !str_cmp( arg2, "dex" ) )
    {
	if ( value < 3 || value > 200 )
	{
	    send_to_char( "Dexterity range is 3 to 200.\n\r", ch );
	    return;
	}

	pMobIndex->mob_stat_phys_dex = value;
	return;
    }

    if ( !str_cmp( arg2, "qui" ) )
    {
	if ( value < 3 || value > 200 )
	{
	    send_to_char( "Quickness range is 3 to 200.\n\r", ch );
	    return;
	}

	pMobIndex->mob_stat_phys_qui = value;
	return;
    }

    if ( !str_cmp( arg2, "tou" ) )
    {
	if ( value < 3 || value > 200 )
	{
	    send_to_char( "Toughness range is 3 to 200.\n\r", ch );
	    return;
	}

	pMobIndex->mob_stat_phys_tou = value;
	return;
    }

    if ( !str_cmp( arg2, "off" ) )
    {
	if ( value < 0 || value > 150 )
	{
	    send_to_char( "Offense range is 0 to 150.\n\r", ch );
	    return;
	}

	pMobIndex->mob_offense = value;
	return;
    }


    if ( !str_cmp( arg2, "def" ) )
    {
	if ( value < 0 || value > 150 )
	{
	    send_to_char( "Defense range is 0 to 150.\n\r", ch );
	    return;
	}

	pMobIndex->mob_defense = value;
	return;
    }

    if ( !str_cmp( arg2, "sex" ) )
    {
	if ( value < 0 || value > 2 )
	{
	    send_to_char( "Sex range is 0 to 2.\n\r", ch );
	    return;
	}
	pMobIndex->sex = value;
	return;
    }

    if ( !str_cmp( arg2, "class" ) )
    {
	if ( value < 0 || value >= MAX_CLASS )
	{
	    char buf[MAX_STRING_LENGTH];

	    sprintf( buf, "Class range is 0 to %d.\n", MAX_CLASS-1 );
	    send_to_char( buf, ch );
	    return;
	}
	pMobIndex->class = value;
	return;
    }

    if ( !str_cmp( arg2, "level" ) )
    {
	if ( value < 0 || value > 30 )
	{
	    send_to_char( "Level range is 0 to 30.\n\r", ch );
	    return;
	}
	pMobIndex->level = value;
	return;
    }

    if ( !str_cmp( arg2, "coins" ) )
    {
	char arg4[MAX_INPUT_LENGTH];
	int coin;

	one_argument( argument, arg4 );

	coin = get_coin_index( arg3 );
	if ( coin == -1 )
	{
	    send_to_char( "Set what coin?\n\r", ch );
	    send_to_char( "Syntax: mpset <mob> coins <type> <number>\n\r", ch );
	    return;
	}

	value = is_number( arg4 ) ? atoi( arg4 ) : -1;

	if ( value < 0 )
	{
	    send_to_char( "Number must be non-negative.\n\r", ch );
	    send_to_char( "Syntax: mpset <mob> coins <type> <number>\n\r", ch );
	    return;
	}
	pMobIndex->coins[coin] = value;

	send_to_char( "Ok.\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "skill" ) )
    {
	char arg4[MAX_INPUT_LENGTH];
	int sn;

	argument = one_argument( argument, arg4 );

	sn = skill_lookup( arg3 );

	if ( sn == -1 )
	{
	    send_to_char("Invalid skill.\n\r", ch );
	    return;
	}

	pMobIndex->learned[sn] = atoi( arg4 );
	pMobIndex->power_level[sn] = atoi( argument );

	send_to_char("Skill set.\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "absorb" ) )
    {
	char arg4[MAX_INPUT_LENGTH];
	char arg5[MAX_INPUT_LENGTH];
	char arg6[MAX_INPUT_LENGTH];
	int value2;
	int value3;
	int value4;

	argument = one_argument( argument, arg4 );
	argument = one_argument( argument, arg5 );
	argument = one_argument( argument, arg6 );

	if ( !is_number( arg3 ) || !is_number( arg4 ) || !is_number( arg5 ) || !is_number( arg6 ) )
	{
	    send_to_char("Absorb must have 4 numerical arguments.\n\r", ch );
	    return;
	}

	value = atoi( arg3 );
	value2 = atoi( arg4 );
	value3 = atoi( arg5 );
	value4 = atoi( arg6 );

	if ( value < 0 || value2 < 0 || value3 < 0 || value4 < 0 )
	{
	    send_to_char("The 4 values must all be positive.\n\r", ch );
	    return;
	}

	pMobIndex->absorb[0] = value;
	pMobIndex->absorb[1] = value2;
	pMobIndex->absorb[2] = value3;
	pMobIndex->absorb[3] = value4;
	return;
    }

    if ( !str_cmp( arg2, "hits" ) )
    {
	char arg4[MAX_INPUT_LENGTH];
	char arg5[MAX_INPUT_LENGTH];
	int value2;
	int value3;

	argument = one_argument( argument, arg4 );
	argument = one_argument( argument, arg5 );

	if ( !is_number( arg3 ) || !is_number( arg4 ) || !is_number( arg5 ) )
	{
	    send_to_char("Hits must have 3 numerical arguments.\n\r", ch );
	    return;
	}

	value = atoi( arg3 );
	value2 = atoi( arg4 );
	value3 = atoi( arg5 );

	if ( value <= 0 || value2 <= 0 || value3 < 0
	|| ((value*value2+value3) > 30000) )
	{
	    send_to_char("The 3 values must all be positive, and a*b+c must\n\r", ch );
	    send_to_char("be less than 30000, where abc are defined by aDb+c.\n\r", ch );
	    return;
	}

	pMobIndex->hitnodice = value;
	pMobIndex->hitsizedice = value2;
	pMobIndex->hitplus = value3;
	return;
    }

    if ( !str_cmp( arg2, "damage" ) || !str_cmp( arg2, "dam" ) )
    {
	char arg4[MAX_STRING_LENGTH];
	char arg5[MAX_STRING_LENGTH];
	int value2;
	int value3;

	argument = one_argument( argument, arg4 );
	argument = one_argument( argument, arg5 );

	if ( !is_number( arg3 ) || !is_number( arg4 ) || !is_number( arg5 ) )
	{
	    send_to_char("Damage must have 3 numerical arguments.\n\r", ch );
	    return;
	}

	value = atoi( arg3 );
	value2 = atoi( arg4 );
	value3 = atoi( arg5 );

/*
	if ( value <= 0 || value2 <= 0 || value3 < 0
*/
	if ( value <= 0 || value2 <= 0
	|| ((value*value2+value3) > 1000) )
	{
	    send_to_char("The 3 values must all be positive, and a*b+c must\n\r", ch );
	    send_to_char("be less than 1000, where abc are defined by aDb+c.\n\r", ch );
	    return;
	}

	pMobIndex->damnodice = value;
	pMobIndex->damsizedice = value2;
	pMobIndex->damplus = value3;
	return;
    }

    if ( !str_cmp( arg2, "align" ) )
    {
	if ( value < -1000 || value > 1000 )
	{
	    send_to_char( "Alignment range is -1000 to 1000.\n\r", ch );
	    return;
	}
	pMobIndex->alignment = value;
	return;
    }

    if ( !str_cmp( arg2, "desc" ) )
    {
	char buf[MAX_STRING_LENGTH];

	if ( !str_cmp( arg3, "clear" ) )
	{
	    free_string( pMobIndex->description, MEM_MOB_DESCRIPTION );
	    pMobIndex->description = str_dup( "\0" );
	    send_to_char("Description cleared.\n\r", ch );
	    return;
	}

	if ( !str_cmp( arg3, "add" ) )
	{
	    strcpy( buf, pMobIndex->description );

	    if ( strlen( buf ) + strlen( argument ) >= MAX_STRING_LENGTH -4 )
	    {
		send_to_char("Mobile description too long, text not added.\n\r", ch );
		return;
	    }

	    if ( buf[0] == '\0' )
		strcpy( buf, argument );
	    else
		strcat( buf, argument );
	    strcat( buf, "\n\r" );
	    free_string( pMobIndex->description, MEM_MOB_DESCRIPTION );
	    pMobIndex->description = str_dup( buf );

	    send_to_char("Text added to mobile description.\n\r", ch );
	    return;
	}

	send_to_char("Desc usage: mpset <mob> desc <add/clear> [text]\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "spec" ) )
    {
	if ( ( pMobIndex->spec_fun = spec_lookup( arg3 ) ) == 0 )
	{
	    send_to_char( "No such spec fun.\n\r", ch );
	    return;
	}

	free_string( pMobIndex->spec_fun_name, MEM_MOB_SPEC_FUN_NAME );
	pMobIndex->spec_fun_name = str_dup( arg3 );

	return;
    }

    /*
     * Generate usage message.
     */
    do_mpset( ch, "" );
    return;
}

void do_newhelp( CHAR_DATA *ch, char *argument )
{
    HELP_DATA *pHelp;
    extern int top_help;

    pHelp = alloc_perm( sizeof(*pHelp) );
    pHelp->level = get_trust( ch );
    pHelp->keyword = str_dup( argument );

    pHelp->text = str_dup( "\0" );

    pHelp->next = help_first;
    help_first = pHelp;
    top_help++;

    send_to_char("New help entry created.\n\r", ch );

    return;
}

void do_newmob( CHAR_DATA *ch, char *argument )
{
    extern int top_mob_index;
    int vnum, iHash, i;
    char buf[MAX_STRING_LENGTH];
    MOB_INDEX_DATA *pMobIndex;
    extern MOB_INDEX_DATA *mob_index_hash[MAX_KEY_HASH];

    if ( IS_NPC( ch ) )
	return;

    if ( !can_build( ch, ch->in_room->area ) )
    {
	send_to_char("You do not have authorization to build in this zone.\n\r", ch );
	return;
    }

    if ( argument[0] == '\0' )
    {
	send_to_char("Syntax:   newmob <vnum>    Where vnum is new mob number.\n\r", ch );
	send_to_char("Syntax:   newmob list\n\r", ch );
	return;
    }

    if ( !str_cmp( argument, "list" ) )
    {
      int col = 0;

      sprintf( buf, "Mob vnums in use for %s.\n\r", ch->in_room->area->name );
      send_to_char( buf, ch );

      for ( pMobIndex = ch->in_room->area->mob_first;
        pMobIndex != NULL; pMobIndex = pMobIndex->zone_next )
      {
        sprintf( buf, " %6d", pMobIndex->vnum );
        send_to_char( buf, ch );
	if ( ( ++col % 10 ) == 0 )
	  send_to_char( "\n\r", ch );
      }

      if ( col % 10 != 0 )
        send_to_char( "\n\r", ch );

      return;
    }

    if ( !is_number( argument ) )
    {
	send_to_char("Vnum must be numeric.\n\r", ch );
	return;
    }

    vnum = atoi( argument );

    if ( get_mob_index( vnum ) != NULL )
    {
	send_to_char("Vnums can not be duplicated.\n\r", ch );
	return;
    }

    if ( vnum < ch->in_room->area->vnum_start || vnum > ch->in_room->area->vnum_final )
    {
	send_to_char("Vnum out of range.\n\r", ch );
	sprintf( buf, "Vnum must be in range %d to %d.\n\r",
	    ch->in_room->area->vnum_start,
	    ch->in_room->area->vnum_final );
	send_to_char( buf, ch );
	return;
    }

    pMobIndex = alloc_perm( sizeof( *pMobIndex ) );

    pMobIndex->next = NULL;
    pMobIndex->zone_next = NULL;
    pMobIndex->spec_fun = NULL;
    pMobIndex->pShop = NULL;
    pMobIndex->area = ch->in_room->area;
    pMobIndex->name = str_dup( "norse" );
    pMobIndex->namelist = str_dup( "new newmob" );
    pMobIndex->short_descr = str_dup( "The new mob" );
    pMobIndex->long_descr = str_dup( "The new mob stands here reminding you to change it's description.\n\r" );
    pMobIndex->description = str_dup( "New mob is waiting for a real description.\n\r" );
    pMobIndex->spec_fun_name = str_dup( "\0" );
    pMobIndex->vnum = vnum;
    pMobIndex->sex = 0;
    pMobIndex->level = 1;
    pMobIndex->act = 3;
    pMobIndex->affected_by = 0;
    pMobIndex->alignment = 0;
    pMobIndex->mob_stat_phys_str = 50;
    pMobIndex->mob_stat_phys_dex = 50;
    pMobIndex->mob_stat_phys_qui = 50;
    pMobIndex->mob_stat_phys_tou = 50;
    pMobIndex->mob_stat_ment_int = 50;
    pMobIndex->mob_offense = 0;
    pMobIndex->mob_defense = 0;
    pMobIndex->absorb[0] = 0;
    pMobIndex->absorb[1] = 0;
    pMobIndex->absorb[2] = 0;
    pMobIndex->absorb[3] = 0;
    pMobIndex->hitnodice = 5;
    pMobIndex->hitsizedice = 5;
    pMobIndex->hitplus = 50;
    pMobIndex->damnodice = 2;
    pMobIndex->damsizedice = 4;
    pMobIndex->damplus = 0;
    for ( i = 0; i < MAX_COIN; i++ )
	pMobIndex->coins[i] = 0;
    pMobIndex->start_pos = POS_STANDING;
    pMobIndex->race = 0;
    pMobIndex->class = 0; 
    pMobIndex->equipped = NULL;
    pMobIndex->skin_objects[0] = 0;
    pMobIndex->skin_objects[1] = 0;
    pMobIndex->skin_objects[2] = 0;
    pMobIndex->skin_objects[3] = 0;
    for ( i = 0; i < MAX_SKILL; i++ )
    {
      pMobIndex->learned[i] = 0;
      pMobIndex->power_level[i] = 0;
    }

    iHash = vnum % MAX_KEY_HASH;
    pMobIndex->next = mob_index_hash[iHash];
    mob_index_hash[iHash] = pMobIndex;
    top_mob_index++;

    if ( ch->in_room->area->mob_first == NULL )
	ch->in_room->area->mob_first = pMobIndex;

    if ( ch->in_room->area->mob_last != NULL )
	ch->in_room->area->mob_last->zone_next = pMobIndex;
    ch->in_room->area->mob_last = pMobIndex;

    send_to_char("New mobile created.\n\r", ch );
    return;
}

void do_newobj( CHAR_DATA *ch, char *argument )
{
    extern int top_obj_index;
    int vnum, iHash;
    char buf[MAX_STRING_LENGTH];
    OBJ_INDEX_DATA *pObjIndex;
    extern OBJ_INDEX_DATA *obj_index_hash[MAX_KEY_HASH];

    if ( IS_NPC( ch ) )
	return;

    if ( !can_build( ch, ch->in_room->area ) )
    {
	send_to_char(
	  "You do not have authorization to build in this zone.\n\r", ch );
	return;
    }

    if ( argument[0] == '\0' )
    {
	send_to_char("Syntax:   newobj <vnum>    Where vnum is new obj number.\n\r", ch );
	send_to_char("Syntax:   newobj list\n\r", ch );
	return;
    }

    if ( !str_cmp( argument, "list" ) )
    {
      int col = 0;

      sprintf( buf, "Object vnums in use for %s.\n\r",
        ch->in_room->area->name );
      send_to_char( buf, ch );

      for ( pObjIndex = ch->in_room->area->obj_first;
        pObjIndex != NULL; pObjIndex = pObjIndex->zone_next )
      {
        sprintf( buf, " %6d", pObjIndex->vnum );
        send_to_char( buf, ch );
	if ( ( ++col % 10 ) == 0 )
	  send_to_char( "\n\r", ch );
      }

      if ( col % 10 != 0 )
        send_to_char( "\n\r", ch );

      return;
    }

    if ( !is_number( argument ) )
    {
	send_to_char("Vnum must be numeric.\n\r", ch );
	return;
    }

    vnum = atoi( argument );

    if ( get_obj_index( vnum ) != NULL )
    {
	send_to_char("Vnums can not be duplicated.\n\r", ch );
	return;
    }

    if ( vnum < ch->in_room->area->vnum_start || vnum > ch->in_room->area->vnum_final )
    {
	send_to_char("Vnum out of range.\n\r", ch );
	sprintf( buf, "Vnum must be in range %d to %d.\n\r",
	    ch->in_room->area->vnum_start,
	    ch->in_room->area->vnum_final );
	send_to_char( buf, ch );
	return;
    }

    pObjIndex = alloc_perm( sizeof( *pObjIndex ) );

    pObjIndex->next = NULL;
    pObjIndex->zone_next = NULL;
    pObjIndex->extra_descr = NULL;
    pObjIndex->affected = NULL;
    pObjIndex->area = ch->in_room->area;
    pObjIndex->name = str_dup( "new newobj" );
    pObjIndex->short_descr = str_dup( "The new object" );
    pObjIndex->look_descr = str_dup( "The new object lies here reminding you to change it's description.\n\r" );
    pObjIndex->description = str_dup( "The new has no description.\n\r" );
    pObjIndex->spec_fun = NULL;
    pObjIndex->spec_fun_name = str_dup( "\0" );
    pObjIndex->vnum = vnum;
    pObjIndex->item_type = ITEM_TRASH;
    pObjIndex->extra_flags = 0;
    pObjIndex->wear_flags = 0;
    pObjIndex->weight = 10;
    pObjIndex->capacity = 0;
    pObjIndex->number = 0;
    pObjIndex->limit = 1;
    pObjIndex->size = 100;
    pObjIndex->cost = 1;
    pObjIndex->load_chance = 101;
    pObjIndex->value[0] = 0;
    pObjIndex->value[1] = 0;
    pObjIndex->value[2] = 0;
    pObjIndex->value[3] = 0;
    pObjIndex->dam[0] = 1;
    pObjIndex->dam[1] = 1;
    pObjIndex->toughness = 1000;
    pObjIndex->attack_type = 2;

    iHash = vnum % MAX_KEY_HASH;
    pObjIndex->next = obj_index_hash[iHash];
    obj_index_hash[iHash] = pObjIndex;
    top_obj_index++;

    if ( ch->in_room->area->obj_first == NULL )
	ch->in_room->area->obj_first = pObjIndex;

    if ( ch->in_room->area->obj_last != NULL )
	ch->in_room->area->obj_last->zone_next = pObjIndex;
    ch->in_room->area->obj_last = pObjIndex;

    send_to_char("New object created.\n\r", ch );
    return;
}

void do_opset( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_INDEX_DATA *pObjIndex;
    int value;

    if ( IS_NPC( ch ) )
	return;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    strcpy( arg3, argument );

    if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
    {
	send_to_char( "Syntax: opset <object> <field>  <value>\n\r",	ch );
	send_to_char( "\n\r",						ch );
	send_to_char( "Field being one of:\n\r",			ch );
	send_to_char( "  value0 value1 value2 value3\n\r",		ch );
	send_to_char( "  extra wear weight size cost type load affect\n\r",	ch );
	send_to_char( "  d0 d1 toughness attack color material limit\n\r",ch );
	send_to_char( "  capacity\n\r",					ch );
	send_to_char( "\n\r",						ch );
	send_to_char( "String being one of:\n\r",			ch );
	send_to_char( "  name short long ed\n\r",			ch );
	return;
    }

    if ( is_number( arg1 ) )
    {
	if ( (pObjIndex = get_obj_index( atoi( arg1 ) )) == NULL )
	{
	    send_to_char("No object with that vnum.\n\r", ch );
	    return;
	}
    }
    else
    {
	if ( ( obj = get_obj_here( ch, arg1 ) ) == NULL )
	{
	    send_to_char("No such object.\n\r", ch );
	    return;
	}

	pObjIndex = obj->pIndexData;
    }

    if ( !can_build( ch, pObjIndex->area ) )
    {
	send_to_char(
     "You do not have authorization to build in that object's zone.\n\r", ch );
	return;
    }

    /*
     * Snarf the value (which need not be numeric).
     */
    value = atoi( arg3 );

    /*
     * Set something.
     */
    if ( !str_cmp( arg2, "damage0" ) || !str_cmp( arg2, "d0" ) )
    {
	pObjIndex->dam[0] = value;
	return;
    }

    if ( !str_cmp( arg2, "damage1" ) || !str_cmp( arg2, "d1" ) )
    {
	pObjIndex->dam[1] = value;
	return;
    }

    if ( !str_cmp( arg2, "limit" ) )
    {
	pObjIndex->limit = value;
	return;
    }

    if ( !str_cmp( arg2, "toughness" ) )
    {
	pObjIndex->toughness = value;
	return;
    }

    if ( !str_cmp( arg2, "attack" ) )
    {
	pObjIndex->attack_type = value;
	return;
    }

    if ( !str_cmp( arg2, "value0" ) || !str_cmp( arg2, "v0" ) )
    {
	pObjIndex->value[0] = value;
	return;
    }

    if ( !str_cmp( arg2, "value1" ) || !str_cmp( arg2, "v1" ) )
    {
	pObjIndex->value[1] = value;
	return;
    }

    if ( !str_cmp( arg2, "value2" ) || !str_cmp( arg2, "v2" ) )
    {
	pObjIndex->value[2] = value;
	return;
    }

    if ( !str_cmp( arg2, "value3" ) || !str_cmp( arg2, "v3" ) )
    {
	pObjIndex->value[3] = value;
	return;
    }

    if ( !str_cmp( arg2, "extra" ) )
    {
	pObjIndex->extra_flags = value;
	return;
    }

    if ( !str_cmp( arg2, "wear" ) )
    {
	pObjIndex->wear_flags = value;
	return;
    }

    if ( !str_cmp( arg2, "weight" ) )
    {
	send_to_char("Weight set.  Don't forget weights are in units of .1 pounds.\n\r", ch );
	pObjIndex->weight = value;
	return;
    }

    if ( !str_cmp( arg2, "capacity" ) )
    {
	send_to_char("Capacity set.  Don't forget weights are in units of .1 pounds.\n\r", ch );
	pObjIndex->capacity = value;
	return;
    }

    if ( !str_cmp( arg2, "size" ) )
    {
	send_to_char("Size set.  Don't forget sizes are in units of percent.\n\r", ch );
	pObjIndex->size = value;
	return;
    }

    if ( !str_cmp( arg2, "cost" ) )
    {
	if ( value < 0 )
	    value = 0;
	pObjIndex->cost = value;
	return;
    }

    if ( !str_cmp( arg2, "type" ) )
    {
	pObjIndex->item_type = value;
	return;
    }

    if ( !str_cmp( arg2, "load" ) )
    {
	if ( value < 0 || value > 101 )
	{
	    send_to_char("Loading percentage must be between 0 and 101.\n\r", ch );
	    return;
	}
	pObjIndex->load_chance = value;
	return;
    }

    if ( !str_cmp( arg2, "color" ) )
    {
	pObjIndex->color = value;
	return;
    }

    if ( !str_cmp( arg2, "material" ) || !str_cmp( arg2, "matl" ) )
    {
	pObjIndex->material = material_lookup( argument );
	return;
    }

    if ( !str_cmp( arg2, "name" ) )
    {
	free_string( pObjIndex->name, MEM_OBJ_NAME );
	pObjIndex->name = str_dup( arg3 );
	return;
    }

    if ( !str_cmp( arg2, "short" ) )
    {
	free_string( pObjIndex->short_descr, MEM_OBJ_SHORT_DESCR );
	pObjIndex->short_descr = str_dup( arg3 );
	return;
    }

    if ( !str_cmp( arg2, "long" ) )
    {
	free_string( pObjIndex->look_descr, MEM_OBJ_LOOK_DESCR );
	pObjIndex->look_descr = str_dup( arg3 );
	return;
    }

    if ( !str_cmp( arg2, "ed" ) )
    {
	EXTRA_DESCR_DATA *ed = NULL;
	EXTRA_DESCR_DATA *ed2;
	char arg4[MAX_INPUT_LENGTH];

	argument = one_argument( argument, arg3 );
	one_argument( argument, arg4 );
	if ( argument == NULL )
	{
	    send_to_char( "Syntax: oset <object> ed <command> <string>\n\r", ch );
	    send_to_char( "Command is one of: add, clear, delete, new\n\r", ch );
	    return;
	}

	if ( str_cmp( arg3, "new" ) && pObjIndex->extra_descr != NULL )
	{
	    for ( ed = pObjIndex->extra_descr; ed != NULL; ed = ed->next )
	    {
		if ( is_name( arg4, ed->keyword ) )
		    break;
	    }
	}

	if ( !str_cmp( arg3, "delete" ) )
	{
	    if ( ed == NULL )
	    {
		send_to_char("No such extra description.\n\r", ch );
		return;
	    }

/* don't free strings because of shared strings */

	    if ( pObjIndex->extra_descr == ed )
		pObjIndex->extra_descr = ed->next;
	    else
	    {
		for ( ed2 = pObjIndex->extra_descr; ed2 != NULL; ed2 = ed2->next )
		    if ( ed2->next == ed )
			ed2->next = ed->next;
/* need to leave it in existance because of shared strings.  */
	    }

	    return;
	}

	if ( !str_cmp( arg3, "new" ) )
	{
	    if ( ed != NULL )
	    {
		send_to_char("Extra description already exists.\n\r", ch );
		return;
	    }

	    if ( extra_descr_free == NULL )
	    {
		ed = alloc_perm( sizeof(*ed) );
	    }
	    else
	    {
		ed = extra_descr_free;
		extra_descr_free = extra_descr_free->next;
	    }

	    ed->keyword = str_dup( argument );
	    ed->description = str_dup( "Empty obj extra desc." );
	    ed->next = pObjIndex->extra_descr;
	    pObjIndex->extra_descr = ed;

	    return;
	}

	if ( ed == NULL )
	{
	    send_to_char("No such extra description.\n\r", ch );
	    return;
	}

	argument = one_argument( argument, arg4 );

	if ( !str_cmp( arg3, "add" ) )
	{
	    char buf[MAX_STRING_LENGTH];

	    strcpy( buf, ed->description );
	    if ( strlen( buf ) + strlen( argument ) >= MAX_STRING_LENGTH -4 )
	    {
		send_to_char("Extra description too long, text not added.\n\r", ch );
		return;
	    }

	    if ( buf[0] == '\0' )
		strcpy( buf, argument );
	    else
		strcat( buf, argument );
	    strcat( buf, "\n\r" );
	    /* do not free.  Shared strings */
	    ed->description = str_dup( buf );

	    return;
	}

	if ( !str_cmp( arg3, "clear" ) )
	{
	    /* do not free.  shared strings. */
	    ed->description = str_dup( "\0" );
	    return;
	}

	send_to_char("Ed usage:  opset <obj> ed <command> <keyword> <args>\n\r", ch );
	send_to_char("Where command is:  add, clear, delete or new.\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "affect" ) )
    {
	AFFECT_DATA *paf = NULL;
	AFFECT_DATA *p2;
	char arg4[MAX_INPUT_LENGTH];
	int value2;

	argument = one_argument( argument, arg3 );
	one_argument( argument, arg4 );

	if ( !is_number( arg3 ) || !is_number( argument ) )
	{
	    send_to_char("Affect requires 2 integer arguments.\n\r", ch );
	    return;
	}

	value = atoi( arg3 );
	value2 = atoi( arg4 );

	if ( value <= 0 || value > 500 )
	{
	    send_to_char("Affect location must be positive.\n\r", ch );
	    return;
	}

	if ( value2 < -30 || value2 > 30 )
	{
	    send_to_char("Don't you think that is a little excessive?\n\r", ch );
	    return;
	}

	if ( pObjIndex->affected != NULL )
	{
	    for ( paf = pObjIndex->affected; paf != NULL; paf = paf->next )
	    {
		if ( paf->location == value )
		    break;
	    }
	}

	if ( value2 == 0 )
	{
	    if ( paf == NULL )
	    {
		send_to_char("An affect with that location does not exist.\n\r", ch );
		return;
	    }

	    if ( pObjIndex->affected == paf )
		pObjIndex->affected = paf->next;
	    else
	    {
		for ( p2 = pObjIndex->affected; p2 != NULL; p2 = p2->next )
		    if ( p2->next == paf )
			break;

		if ( p2 != NULL )
		    p2->next = paf->next;
	    }

	    return;
	}

	if ( paf == NULL )
	{
	    paf = alloc_perm( sizeof( *paf ) );

	    paf->next = pObjIndex->affected;
	    paf->type = -1;
	    paf->duration = -1;
	    paf->bitvector = 0;
	    pObjIndex->affected = paf;
	}

	paf->location = value;
	paf->modifier = value2;

	return;
    }

    /*
     * Generate usage message.
     */
    do_opset( ch, "" );
    return;
}

void do_rdesc( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg1 [MAX_INPUT_LENGTH];

    if ( IS_NPC( ch ) )
	return;

    if ( !can_build( ch, ch->in_room->area ) )
    {
	send_to_char(
	  "You do not have authorization to build in this zone.\n\r", ch );
	return;
    }

    argument = one_argument( argument, arg1 );

    if ( arg1[0] == '\0' )
    {
	send_to_char(" Syntax:  rdesc clear\n\r",		ch );
	send_to_char("          rdesc add <text to add>\n\r",	ch );
	return;
    }

    if ( !str_cmp( arg1, "add" ) )
    {
	strcpy( buf, ch->in_room->description );

	if ( strlen( buf ) + strlen( argument ) >= MAX_STRING_LENGTH -4 )
	{
	    send_to_char("Room description too long, text not added.\n\r", ch );
	    return;
	}

	if ( buf[0] == '\0' )
	    strcpy( buf, argument );
	else
	    strcat( buf, argument );
	strcat( buf, "\n\r" );
	free_string( ch->in_room->description, MEM_ROOM_DESCRIPTION );
	ch->in_room->description = str_dup( buf );

	send_to_char("Text added to room description.\n\r", ch );
	return;
    }

    if ( !str_cmp( arg1, "clear" ) )
    {
	buf[0] = '\0';

	free_string( ch->in_room->description, MEM_ROOM_DESCRIPTION );
	ch->in_room->description = str_dup( buf );

	send_to_char("Room description cleared.\n\r", ch );
	return;
    }

    do_rdesc( ch, "" );

    return;
}


void do_cload( CHAR_DATA *ch, char *argument )
{
    RESET_DATA *pReset;
    CHAR_DATA *mob;
    OBJ_DATA *obj; 

    if ( IS_NPC( ch ) )
	return;

    if ( !can_build( ch, ch->in_room->area ) )
    {
	send_to_char(
	  "You do not have authorization to build in this zone.\n\r", ch );
	return;
    }

    if ( (mob = get_char_room( ch, argument )) != NULL )
    {
	if ( mob->pReset != NULL )
	{
	    mob = create_mobile( mob->pIndexData );
	    char_to_room( mob, ch->in_room, FALSE );
	}

	pReset = alloc_perm( sizeof( *pReset ) );
	pReset->next = NULL;
	pReset->command = 'M';
	pReset->arg1 = mob->pIndexData->vnum;
	pReset->arg2 = 0;
	pReset->arg3 = ch->in_room->vnum;
	pReset->target = (void *) mob;
	mob->pReset = pReset;

	if ( ch->in_room->area->reset_first == NULL )
	{
	    ch->in_room->area->reset_first = pReset;
	    ch->in_room->area->reset_last = pReset;
	}
	else
	{
	    ch->in_room->area->reset_last->next = pReset;
	    ch->in_room->area->reset_last = pReset;
	}

	send_to_char("Mobile added to reset list.\n\r", ch );
	return;
    }
    else if ( (obj = get_obj_here( ch, argument )) != NULL )
    {
	if ( obj->pReset != NULL )
	{
	    obj = create_object( obj->pIndexData );
	    obj_to_room( obj, ch->in_room, FALSE );
	}

	pReset = alloc_perm( sizeof( *pReset ) );
	pReset->next = NULL;
	pReset->command = 'O';
	pReset->arg1 = obj->pIndexData->vnum;
	pReset->arg2 = 0;
	pReset->arg3 = ch->in_room->vnum;
	pReset->target = (void *) obj;
	obj->pReset = pReset;

	if ( ch->in_room->area->reset_first == NULL )
	{
	    ch->in_room->area->reset_first = pReset;
	    ch->in_room->area->reset_last = pReset;
	}
	else
	{
	    ch->in_room->area->reset_last->next = pReset;
	    ch->in_room->area->reset_last = pReset;
	}

	send_to_char("Object added to reset list.\n\r", ch );
	return;
    }

    send_to_char("Current syntax: cload <mobile/object>\n\r", ch );
    return;
}


void do_xload( CHAR_DATA *ch, char *argument )
{
    MOB_INDEX_DATA *pMobIndex;
    CHAR_DATA *mob;
    OBJ_DATA *obj;
    EQUIP_DATA *eq;
    int iloc;
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];

    if ( IS_NPC( ch ) )
	return;

    if ( !can_build( ch, ch->in_room->area ) )
    {
	send_to_char(
	  "You do not have authorization to build in this zone.\n\r", ch );
	return;
    }

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( argument[0] == '\0' || arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char("Usage:  xload <mobile> <object> <location>\n\r", ch );
	return;
    }

    if ( (mob = get_char_room( ch, arg1 )) == NULL
    || !IS_NPC( mob ) || (pMobIndex = mob->pIndexData) == NULL )
    {
	send_to_char("No mobile by that name here.\n\r", ch );
	return;
    }

    if ( (obj = get_obj_carry( ch, arg2, ch )) == NULL
    || obj->pIndexData == NULL )
    {
	send_to_char("No object by that name here.\n\r", ch );
	send_to_char("Selecting no object.\n\r", ch );
    }

    if ( (iloc = parse_location( argument )) == -2 )
    {
	send_to_char("Invalid location.\n\r", ch );
	send_to_char("none lf rf neck1 neck2 body head legs feet hands arms\n\r", ch );
	send_to_char("shield about waist lw rw wield hold\n\r", ch );
	return;
    }

    for ( eq = pMobIndex->equipped; eq != NULL; eq = eq->next )
    {
	if ( eq->location == iloc )
	    break;
    }

    if ( eq == NULL )
    {
	if ( obj == NULL || obj->pIndexData == NULL )
	{
	    send_to_char("No point generating new equipment for null object.\n\r", ch );
	    return;
	}
	eq = alloc_perm( sizeof( *eq ) );
	eq->location = iloc;
	eq->next = pMobIndex->equipped;
	pMobIndex->equipped = eq;
    }

    if ( obj == NULL || obj->pIndexData == NULL )
    {
	EQUIP_DATA *eq2;

	if ( pMobIndex->equipped == eq )
	{
	    pMobIndex->equipped = eq->next;
	}
	else
	{
	    for ( eq2 = pMobIndex->equipped; eq2 != NULL; eq2 = eq2->next )
	    {
		if ( eq2->next == eq )
		    break;
	    }

	    eq2->next = eq->next;
	}

	free_mem( eq, sizeof( eq ) );
	send_to_char("Equipment eaten.\n\r", ch );
	return;
    }

    eq->item = obj->pIndexData->vnum;

    send_to_char("Object added/modified to mobile standard equipment.\n\r", ch );
    return;
}


int parse_location( char *argument )
{
    int i;
    extern struct wear_data const wear_info[];

    if ( is_number( argument ) )
	return atoi( argument );

    if ( !str_cmp( argument, "wield" ) || !str_cmp( argument, "prim" )
    || !str_cmp( argument, "primary" ) )
	return gn_wear_primary;

    if ( !str_cmp( argument, "hold" ) || !str_cmp( argument, "sec" )
    || !str_cmp( argument, "second" ) || !str_cmp( argument, "secondary" ) )
	return gn_wear_secondary;

    if ( !str_cmp( argument, "i" ) || !str_prefix( argument, "inventory" )
    || !str_cmp( argument, "none" ) )
	return gn_wear_none;

    for ( i = 0; i < MAX_WEAR; i++ )
    {
	if ( !str_cmp( argument, wear_info[i].name ) )
	    return i;
    }

    return gn_wear_none;
}


void do_reset( CHAR_DATA *ch, char *argument )
{
    char command[MAX_INPUT_LENGTH];
    char anumber[MAX_INPUT_LENGTH];
    char rtype[MAX_INPUT_LENGTH];
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char arg3[MAX_INPUT_LENGTH];
    int number, ii;
    RESET_DATA *pr_last = NULL;
    RESET_DATA *pReset = NULL;

    if ( IS_NPC( ch ) )
	return;

    if ( !can_build( ch, ch->in_room->area ) )
    {
	send_to_char(
	  "You do not have authorization to build in this zone.\n\r", ch );
	return;
    }

    argument = one_argument( argument, command );
    argument = one_argument( argument, anumber );

/* don't necessarily need to check arg3.  Isn't always used. */
    if ( command[0] == '\0' )
    {
	send_to_char("Syntax:   reset <command> <number> [<type> <arg1> <arg2> [arg3]]\n\r", ch );
	send_to_char("Where command is one of: append, delete or list.\n\r", ch );
	send_to_char("Number is which reset to use as base point. (-1 = last, 0 = 1st )\n\r", ch );
	send_to_char("Type is one of:  mobile, give, inventory, object, put or door\n\r", ch );
	return;
    }

    if ( !str_cmp( command, "area" ) )
    {
      extern void reset_area( AREA_DATA *pArea );
      reset_area( ch->in_room->area );
      send_to_char( "Area reset.\n\r", ch );
      return;
    }

    if ( !is_number( anumber ) )
    {
	send_to_char("Number must be a number. (heh)\n\r", ch );
	return;
    }

    number = atoi( anumber );

    pReset = ch->in_room->area->reset_first;

    if ( number < 0 )
    {
	pReset = ch->in_room->area->reset_last;

	if ( ch->in_room->area->reset_first != pReset )
	for ( pr_last = ch->in_room->area->reset_first; pr_last != NULL; pr_last = pr_last->next )
	    if ( pr_last->next == pReset )
		break;
    }
    else if ( number == 0 )
    {
    }
    else if ( number == 1 )
    {
	number = 0;
    }
    else
    {
	pReset = ch->in_room->area->reset_first;

	for ( ii = 2; ii <= number; ii++ )
	{
	     pr_last = pReset;
             if (pReset == NULL )
                break;
	     pReset = pReset->next;

	     if (pReset == NULL)
		break;   
	}
    }

    if ( pReset == NULL && number > 0 )
    {
	send_to_char("Base reset does not exist.\n\r", ch );
	return;
    }

    if ( !str_cmp( command, "delete" ) )
    {
	if ( ch->in_room->area->reset_first == NULL )
	{
	    send_to_char("No resets in this area.\n\r", ch );
	    return;
	}

	if ( number == 0 || number == 1 )
	{
	    ch->in_room->area->reset_first = pReset->next;
	}
	else
	{
	    if ( pr_last != NULL )
		pr_last->next = pReset->next;
	}

	if ( ch->in_room->area->reset_last == pReset
	&& pr_last != NULL )
	    ch->in_room->area->reset_last = pr_last;
	free_mem( pReset, sizeof( *pReset ) );

	send_to_char("Reset deleted.\n\r", ch );
	return;
    }

    if ( !str_cmp( command, "list" ) )
    {
	if ( number < 0 )
	{
	    send_to_char("There are no resets after end of list.\n\r", ch );
	    return;
	}

	if ( number == 0 )
	{
	    number = 1;
	    pReset = ch->in_room->area->reset_first;
	}

	for ( ii = 1; ii <= 20; ii++ )
	{
	    char buf[MAX_STRING_LENGTH];

	    if ( pReset == NULL )
		break;

	    sprintf( buf, "%d  %c %d %d %d\n\r",
		number + ii - 1,
		pReset->command,
		pReset->arg1, pReset->arg2, pReset->arg3 );
	    send_to_char( buf, ch );

	    pReset = pReset->next;
	}

	return;
    }

    argument = one_argument( argument, rtype );
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    argument = one_argument( argument, arg3 );

/* append */
    if ( !str_cmp( command, "append" ) )
    {
	int v1, v2, v3;
	bool fNoMob = FALSE;
	bool fNoObj = FALSE;
	bool fNoRoom = FALSE;
	RESET_DATA *pAppend;
	ROOM_INDEX_DATA *pRoom;
	EXIT_DATA *pExit;

    if ( pReset == ch->in_room->area->reset_last )
	number = -1;

	if ( !is_number( arg1 ) || !is_number( arg2 )
	|| ( ( str_cmp( rtype, "G" ) && str_cmp( rtype, "give" )
	     && str_cmp( rtype, "I" ) && str_cmp( rtype, "invent" ) )
	   && !is_number( arg3 ) ) )
	{
	    send_to_char("Must provide integer arguments.\n\r", ch );
	    return;
	}

	v1 = atoi( arg1 );
	v2 = atoi( arg2 );
	v3 = atoi( arg3 );

	if ( !str_cmp( rtype, "M" ) || !str_cmp( rtype, "mobile" ) )
	{
		rtype[0] = 'M';
		rtype[1] = '\0';
		if ( get_mob_index( v1 ) == NULL )
		    fNoMob = TRUE;
		if ( get_room_index( v3 ) == NULL )
		    fNoRoom = TRUE;
	}
	else if ( !str_cmp( rtype, "O" ) || !str_cmp( rtype, "object" ) )
	{
		rtype[0] = 'O';
		rtype[1] = '\0';
		if ( get_obj_index( v1 ) == NULL )
		    fNoObj = TRUE;
		if ( get_room_index( v3 ) == NULL )
		    fNoRoom = TRUE;
	}
	else if ( !str_cmp( rtype, "P" ) || !str_cmp( rtype, "put" ) )
	{
		rtype[0] = 'P';
		rtype[1] = '\0';
		if ( get_obj_index( v1 ) == NULL )
		    fNoObj = TRUE;
		if ( get_obj_index( v3 ) == NULL )
		    fNoObj = TRUE;
	}
	else if ( !str_cmp( rtype, "G" ) || !str_cmp( rtype, "give" ) )
	{
		rtype[0] = 'G';
		rtype[1] = '\0';
		if ( get_obj_index( v1 ) == NULL )
		    fNoObj = TRUE;
	}
	else if ( !str_cmp( rtype, "I" ) || !str_cmp( rtype, "invent" ) )
	{
		rtype[0] = 'I';
		rtype[1] = '\0';
		if ( get_obj_index( v1 ) == NULL )
		    fNoObj = TRUE;
	}
	else if ( !str_cmp( rtype, "E" ) || !str_cmp( rtype, "equip" ) )
	{
		rtype[0] = 'E';
		rtype[1] = '\0';
		if ( get_obj_index( v1 ) == NULL )
		    fNoObj = TRUE;
		if ( v3 > MAX_WEAR )
		{
		    send_to_char("No such wear location.\n\r", ch );
		    return;
		}
	}
	else if ( !str_cmp( rtype, "D" ) || !str_cmp( rtype, "door" ) )
	{
		rtype[0] = 'D';
		rtype[1] = '\0';
		if ( ( pRoom = get_room_index( v1 ) ) == NULL )
		{
		    fNoRoom = TRUE;
		}
		else if ( v2 < 0 || v2 > 5 || ( pExit = pRoom->exit[v2] ) == NULL )
		{
		    send_to_char("No such exit.\n\r", ch );
		    return;
		}
		else if ( !IS_SET( pExit->exit_flags, EX_ISDOOR ) )
		{
		    send_to_char("Target exit is not a door.\n\r", ch );
		    return;
		}
		else if ( v3 < 0 || v3 > 2 )
		{
		    send_to_char("Locks must be between 0 and 2.\n\r", ch );
		    return;
		}
	}
	else
	{
		send_to_char("Bad reset argument.\n\r", ch );
		return;
	}

	if ( fNoMob )
	{
	    send_to_char("No such mobile.\n\r", ch );
	    return;
	}

	if ( fNoObj )
	{
	    send_to_char("No such object.\n\r", ch );
	    return;
	}

	if ( fNoRoom )
	{
	    send_to_char("No such room.\n\r", ch );
	    return;
	}

	pAppend = alloc_perm( sizeof( *pAppend ) );
	pAppend->next = NULL;
	pAppend->command = rtype[0];
	pAppend->arg1 = v1;
	pAppend->arg2 = v2;
	pAppend->arg3 = v3;
	pAppend->target = NULL;

	if ( number < 0 )
	{
	    if ( ch->in_room->area->reset_last != NULL )
	    {
		ch->in_room->area->reset_last->next = pAppend;
	    }
	    else
	    {
		ch->in_room->area->reset_first = pAppend;
	    }
	    ch->in_room->area->reset_last = pAppend;
	}
	else if ( number == 0 )
	{
	    if ( ch->in_room->area->reset_first != NULL )
	    {
		pAppend->next = ch->in_room->area->reset_first;
	    }
	    else
	    {
		ch->in_room->area->reset_last = pAppend;
	    }
	    ch->in_room->area->reset_first = pAppend;
	}
	else
	{
	    pAppend->next = pReset->next;
	    pReset->next = pAppend;
	}

	send_to_char("Reset added.\n\r", ch );

	return;
    }

    do_reset( ch, "" );

    return;
}

void do_rextra( CHAR_DATA *ch, char *argument  )
{
    char keyword[MAX_INPUT_LENGTH];
    char command[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    EXTRA_DESCR_DATA *ed;

    if ( IS_NPC( ch ) )
	return;

    if ( !can_build( ch, ch->in_room->area ) )
    {
	send_to_char(
	  "You do not have authorization to build in this zone.\n\r", ch );
	return;
    }

    argument = one_argument( argument, keyword );
    argument = one_argument( argument, command );

    if ( keyword[0] == '\0' || command[0] == '\0' )
    {
	send_to_char("Syntax:  rextra <keyword> <command> [text]\n\r", ch );
	send_to_char("\n\r", ch );
	send_to_char("Where command is one of:\n\r", ch );
	send_to_char(" clear create desc remove\n\r", ch );
	return;
    }

    for ( ed = ch->in_room->extra_descr; ed != NULL; ed = ed->next )
    {
	if ( is_name( keyword, ed->keyword ) )
	    break;
    }

    if ( !str_cmp( command, "create" ) )
    {
	if ( ed != NULL )
	{
	    send_to_char("Extra description already exists.\n\r", ch );
	    return;
	}

	ed = alloc_perm( sizeof( *ed ) );

	if ( argument[0] == '\0' )
	{
	    send_to_char("What should the extra description be called?\n\r", ch );
	    return;
	}

	ed->keyword = str_dup( argument );
	ed->description = str_dup( "\0" );

	ed->next = ch->in_room->extra_descr;
	ch->in_room->extra_descr = ed;

	send_to_char("Extra description created.\n\r", ch );
	return;
    }

    if ( ed == NULL )
    {
	send_to_char("No such extra description.\n\r", ch );
	return;
    }

    if ( !str_cmp( command, "clear" ) )
    {
	free_string( ed->description, MEM_DIRECTION_DESCRIPTION );
	ed->description = str_dup( "\0" );

	send_to_char("Extra description cleared.\n\r", ch );
	return;
    }

    if ( !str_cmp( command, "desc" ) )
    {
	strcpy( buf, ed->description );

	if ( strlen( buf ) + strlen( argument ) >= MAX_STRING_LENGTH -4 )
	{
	    send_to_char("Extra description too long, text not added.\n\r", ch );
	    return;
	}

	if ( buf[0] == '\0' )
	    strcpy( buf, argument );
	else
	    strcat( buf, argument );
	strcat( buf, "\n\r" );
	free_string( ed->description, MEM_DIRECTION_DESCRIPTION );
	ed->description = str_dup( buf );

	send_to_char("Text added to extra description.\n\r", ch );
	return;
    }

    if ( !str_cmp( command, "remove" ) )
    {
	EXTRA_DESCR_DATA *pextra;

	if ( ch->in_room->extra_descr == ed )
	{
	    ch->in_room->extra_descr = ed->next;
	}
	else
	{
	    for ( pextra = ch->in_room->extra_descr; pextra != NULL; pextra = pextra->next )
	    {
		if ( pextra->next == ed )
		    break;
	    }

	    pextra->next = ed->next;
	}

	free_extra_descr( ed );

	send_to_char("Extra description removed.\n\r", ch );
	return;
    }

    do_rextra( ch, "" );
    return;
}


void do_savehelps( CHAR_DATA *ch, char *argument )
{
    FILE *fp;
    HELP_DATA *pHelp;

    fclose( fpReserve );
    if ( ( fp = fopen( "test.helps", "w" )) == NULL )
    {
	send_to_char("Error opening help file.\n\r", ch );
	fpReserve = fopen( NULL_FILE, "r" );
	return;
    }

    fprintf( fp, "#HELPS\n\n\n\n" );

    for ( pHelp = help_first ; pHelp != NULL ; pHelp = pHelp->next )
    {
	if ( pHelp->keyword == NULL )
	    continue;

	if ( pHelp->keyword[0] == '\0' )
	    continue;

	if ( pHelp->text == NULL )
	    continue;

	if ( pHelp->text[0] == '\0' )
	    continue;

	fprintf( fp, "%d ",
	    pHelp->level );

	write_string( fp, pHelp->keyword );
	fprintf( fp, "~\n" );
	write_string( fp, pHelp->text );

	fprintf( fp, "~\n\n\n" );
    }

    fprintf( fp, "0 $~\n\n#$\n" );

    fclose( fp );
    fpReserve = fopen( NULL_FILE, "r" );
    system( "cp test.helps help.are" );
    system( "rm test.helps" );

    send_to_char("Help file written.\n\r", ch );

    return;
}


void do_savezone( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    AREA_DATA *pArea;
    extern AREA_DATA *area_first;

    if ( IS_NPC( ch ) )
	return;

    if ( get_trust( ch ) >= MAX_LEVEL && !str_cmp( argument, "world" ) )
    {
	send_to_char("Initiating World Save...\nPlease wait.\n\r", ch );

	for ( pArea = area_first; pArea != NULL; pArea = pArea->next )
	{
	    if ( save_zone_file( pArea ) == TRUE )
	    {
		sprintf( buf, "%s.are saved.\n\r", pArea->filename );
		send_to_char( buf, ch );
	    }
	    else
	    {
		sprintf( buf, "%s.are save ---|||FAILED|||---\n\r", pArea->filename );
		log_string( buf );
		send_to_char( buf, ch );
	    }
	}

	send_to_char("World save completed.\n\r", ch );
	return;
    }

    if ( !can_build( ch, ch->in_room->area ) )
    {
	send_to_char(
	  "Incorrect Authorization to save this zone.\n\rPlease contact an OV or Senior staff for help.\n\r", ch );
	return;
    }

    if (ch->in_room == NULL || (pArea = ch->in_room->area) == NULL)
    {
	send_to_char("Can't save zone, null area.\n\r", ch );
	return;
    }

    if ( save_zone_file( pArea ) == TRUE )
    {
	send_to_char("Your area has successfully been saved.\n\r", ch );
    }
    else
    {
	send_to_char("Error writing zone file.\n\r", ch );
    }

    return;
}


bool save_zone_file( AREA_DATA *pArea )
{
    int ii;
    FILE *fp;
    ROOM_INDEX_DATA *pRoomIndex;
    MOB_INDEX_DATA *pMobIndex;
    OBJ_INDEX_DATA *pObjIndex;
    RESET_DATA *pReset;
    EXIT_DATA *pExit;
    EXTRA_DESCR_DATA *ed;
    AFFECT_DATA *paf;
    EQUIP_DATA *pEquip;
    char fname[MAX_STRING_LENGTH];
    char buf[MAX_STRING_LENGTH];
    int iDoor, sn, i;

    if (pArea->filename == NULL || pArea->filename[0] == '\0')
    {
/*
	send_to_char("Can't save zone, null file name.\n\r", ch );
*/
	return -1;
    }

    sprintf(fname, "%s.tmp", pArea->filename);

    fclose( fpReserve );
    if ( ( fp = fopen( fname, "w" )) == NULL )
    {
/*
	send_to_char("Error opening output file.\n\r", ch );
*/
	fpReserve = fopen( NULL_FILE, "r" );
	return -1;
    }

    fprintf( fp, "#AREA " );
    write_string( fp, pArea->name );
    fprintf( fp, "~\n" );
    write_string( fp, pArea->filename );
    fprintf( fp, "~\n" );
    write_string( fp, pArea->builders );
    fprintf( fp, "~\n" );
    fprintf( fp, "%d %d\n", pArea->vnum_start, pArea->vnum_final );
    fprintf( fp, "%d %d\n\n\n", pArea->reset_length, pArea->area_bits );


    fprintf( fp, "#MOBILES\n");
    for ( pMobIndex = pArea->mob_first; pMobIndex != NULL; pMobIndex = pMobIndex->zone_next )
    {
	fprintf( fp, "#%d\n", pMobIndex->vnum );
	write_string( fp, pMobIndex->name );
	fprintf( fp, "~\n" );
	write_string( fp, pMobIndex->namelist );
	fprintf( fp, "~\n" );
	write_string( fp, pMobIndex->short_descr );
	fprintf( fp, "~\n" );
	write_string( fp, pMobIndex->long_descr );
	fprintf( fp, "~\n" );
	write_string( fp, pMobIndex->description );
	fprintf( fp, "~\n" );
	write_string( fp, pMobIndex->spec_fun_name );
	fprintf( fp, "~\n" );

	if ( IS_SET( pMobIndex->act, ACT_SHOPKEEPER )
	&& pMobIndex->pShop == NULL )
	    pMobIndex->act -= ACT_SHOPKEEPER;

	fprintf( fp, "%d %d %d %d %d %d S\n",
	    pMobIndex->act,
	    pMobIndex->affected_by,
	    pMobIndex->alignment,
	    pMobIndex->aggression,
	    pMobIndex->speed,
	    pMobIndex->clan );

	fprintf( fp, "%d %dd%d+%d %dd%d+%d\n",
	    pMobIndex->level,
	    pMobIndex->hitnodice, pMobIndex->hitsizedice, pMobIndex->hitplus,
	    pMobIndex->damnodice, pMobIndex->damsizedice, pMobIndex->damplus );

	fprintf( fp, "%d %d %d %d %d %d %d\n",
	    pMobIndex->mob_stat_phys_str,
	    pMobIndex->mob_stat_phys_dex,
	    pMobIndex->mob_stat_phys_tou,
	    pMobIndex->mob_stat_phys_qui,
	    pMobIndex->mob_stat_ment_int,
	    pMobIndex->mob_offense,
	    pMobIndex->mob_defense );

	fprintf( fp, "%d %d %d %d\n",
	    pMobIndex->absorb[0],
	    pMobIndex->absorb[1],
	    pMobIndex->absorb[2],
	    pMobIndex->absorb[3] );

	fprintf( fp, "%d %d\n",
	    pMobIndex->start_pos, pMobIndex->sex );

	fprintf( fp, "'%s'\n", race_array[pMobIndex->race]->race_name );

	fprintf( fp, "%d\n",
	    pMobIndex->class );

	if ( IS_SET( pMobIndex->act, ACT_SHOPKEEPER )
	|| pMobIndex->pShop != NULL )
	{
	    SHOP_DATA *pShop = pMobIndex->pShop;

	    for ( ii = 0; ii < MAX_TRADE; ii++ )
	    {
		fprintf( fp, "%d ", pShop->buy_type[ii] );
	    }
	    fprintf( fp, "%d %d %d %d %d\n",
		pShop->profit_buy,
		pShop->profit_sell,
		pShop->open_hour,
		pShop->close_hour,
		pShop->gain );

	    for ( i = 0; i < SHOP_MAX_MESSAGE; i++ )
	    {
		write_string( fp, pShop->shop_messages[i] );
		fprintf( fp, "~\n" );
	    }

	    for ( i = 0; i < MAX_COIN; i++ )
	    {
		if ( pShop->coinage_buy[i] == 100
		&& pShop->coinage_sell[i] == 100 )
		    continue;

		fprintf( fp, "C '%s' %d %d\n",
		    coin_table[i].name,
		    pShop->coinage_buy[i],
		    pShop->coinage_sell[i] );
	    }
	}

	if ( pMobIndex->equipped != NULL )
	{
	    extern struct wear_data const wear_info[];
	    for ( pEquip = pMobIndex->equipped; pEquip != NULL; pEquip = pEquip-> next )
	    {
		fprintf( fp, "E %d '%s'\n",
		    pEquip->item,
		    pEquip->location > -1 ?
			wear_info[pEquip->location].name : "none" );
	    }
	}

	for ( i = 0; i < MAX_COIN; i++ )
	{
	    if ( pMobIndex->coins[i] != 0 )
		fprintf( fp, "G '%s' %d\n",
		    coin_table[i].name, pMobIndex->coins[i] );
	}

	for ( sn = 0; sn < MAX_SKILL; sn++ )
	{
	    if ( pMobIndex->learned[sn] != 0
	    || pMobIndex->power_level[sn] != 0 )
	    {
		fprintf( fp, "S '%s' %d %d\n",
		    skill_table[sn].name,
		    pMobIndex->learned[sn],
		    pMobIndex->power_level[sn] );
	    }
	}

	if ( pMobIndex->skin_objects[0] > 0 )
	    fprintf( fp, "K %d\n", pMobIndex->skin_objects[0] );
	if ( pMobIndex->skin_objects[1] > 0 )
	    fprintf( fp, "K %d\n", pMobIndex->skin_objects[1] );
	if ( pMobIndex->skin_objects[2] > 0 )
	    fprintf( fp, "K %d\n", pMobIndex->skin_objects[2] );
	if ( pMobIndex->skin_objects[3] > 0 )
	    fprintf( fp, "K %d\n", pMobIndex->skin_objects[3] );
    }

    fprintf( fp, "#0\n\n\n" );

    fprintf( fp, "#OBJECTS\n" );

    for ( pObjIndex  = pArea->obj_first; pObjIndex != NULL; pObjIndex = pObjIndex->zone_next )
    {
	fprintf( fp, "#%d\n", pObjIndex->vnum );
	write_string( fp, pObjIndex->name );
	fprintf( fp, "~\n" );
	write_string( fp, pObjIndex->short_descr );
	fprintf( fp, "~\n" );
	write_string( fp, pObjIndex->look_descr );
	fprintf( fp, "~\n" );
	write_string( fp, pObjIndex->description );
	fprintf( fp, "~\n" );
	write_string( fp, pObjIndex->spec_fun_name );
	fprintf( fp, "~\n" );

	fprintf( fp,"'%s'\n",
	    material_table[pObjIndex->material].name );

	fprintf( fp, "%d %d %d %d\n",
	    pObjIndex->item_type,
	    pObjIndex->extra_flags,
	    pObjIndex->wear_flags,
	    pObjIndex->color );

	switch ( pObjIndex->item_type )
	{
	    case ITEM_PILL:
	    case ITEM_POTION:
	    case ITEM_SCROLL:
		fprintf( fp, "%d %d %d %d\n",
		    pObjIndex->value[0],
		    (pObjIndex->value[1] > 0 ? skill_table[pObjIndex->value[1]].slot : -1 ),
		    (pObjIndex->value[2] > 0 ? skill_table[pObjIndex->value[2]].slot : -1 ),
		    (pObjIndex->value[3] > 0 ? skill_table[pObjIndex->value[3]].slot : -1 ) );
		break;
	    case ITEM_WAND:
	    case ITEM_STAFF:
		fprintf( fp, "%d %d %d %d\n",
		    pObjIndex->value[0],
		    pObjIndex->value[1],
		    pObjIndex->value[2],
		    (pObjIndex->value[3] > 0 ? skill_table[pObjIndex->value[3]].slot : -1 ) );
		break;
	    default:
		fprintf( fp, "%d %d %d %d\n",
		    pObjIndex->value[0],
		    pObjIndex->value[1],
		    pObjIndex->value[2],
		    pObjIndex->value[3] );
	}

	fprintf( fp, "%d %d %d %d %d\n",
	    pObjIndex->weight,
	    pObjIndex->capacity,
	    pObjIndex->size,
	    pObjIndex->cost,
	    pObjIndex->load_chance );

	fprintf( fp, "%d %d %d %d\n",
	    pObjIndex->dam[0],
	    pObjIndex->dam[1],
	    pObjIndex->toughness,
	    pObjIndex->attack_type );

	for ( paf = pObjIndex->affected; paf != NULL; paf = paf->next )
	{
	    if ( paf->location < 0 || paf->modifier == 0 )
		continue;

	    fprintf( fp, "A\n" );
	    fprintf( fp, "%d %d\n",
		paf->location,
		paf->modifier );
	}

	for ( ed = pObjIndex->extra_descr; ed != NULL; ed = ed->next )
	{
	    if ( ed->keyword == NULL || ed->description == NULL )
		continue;

	    fprintf( fp, "E\n" );
	    write_string( fp, ed->keyword );
	    fprintf( fp, "~\n" );
	    write_string( fp, ed->description );
	    fprintf( fp, "~\n" );
	}

	if ( pObjIndex->limit != -1 )
	    fprintf( fp, "L %d\n", pObjIndex->limit );

    }

    fprintf( fp, "#0\n\n\n" );

    fprintf( fp, "#ROOMS\n" );
	for ( pRoomIndex = pArea->room_first; pRoomIndex != NULL; pRoomIndex = pRoomIndex->zone_next )
	{
	    if ( pRoomIndex->area != pArea )
		continue;

	    if ( pRoomIndex->vnum == 0)
		continue;

	    fprintf( fp, "#%d\n", pRoomIndex->vnum );
	    write_string( fp, pRoomIndex->name );
	    fprintf( fp, "~\n" );
	    write_string( fp, pRoomIndex->description );
	    fprintf( fp, "~\n" );
	    write_string( fp, pRoomIndex->spec_fun_name );
	    fprintf( fp, "~\n" );
	    fprintf( fp, "'%s'\n", pRoomIndex->sector->name );
           // write_string( fp, pRoomIndex->sector->flags );
	    fprintf( fp, "%d %d %d\n",
		pRoomIndex->room_flags,
		pRoomIndex->move_cost,
		pRoomIndex->move_delay );
	    fprintf( fp, "%d %d %d\n",
		pRoomIndex->light,
		pRoomIndex->size,
		pRoomIndex->light_filter );

	    for ( iDoor = 0; iDoor < 6; iDoor++ )
	    {
		if ( ( pExit = pRoomIndex->exit[iDoor] ) == NULL )
		    continue;

		fprintf( fp, "D%d\n", iDoor );
		write_string( fp, pExit->description );
//                fprintf( fp, "D%d\n", iDoor );
//                write_string( fp, pExit->cdescription );
		fprintf( fp, "~\n" );
		write_string( fp, pExit->keyword );
		fprintf( fp, "~\n" );
		fprintf( fp, "%d %d %d %d\n",
		    pExit->exit_flags,
		    pExit->key,
		    pExit->vnum,
		    pExit->difficulty );
	    }

	    for ( ed = pRoomIndex->extra_descr; ed != NULL; ed = ed->next )
	    {
		if ( ed->keyword == NULL || ed->description == NULL )
		    continue;

		fprintf( fp, "E\n" );
		write_string( fp, ed->keyword );
		fprintf( fp, "~\n" );
		write_string( fp, ed->description );
		fprintf( fp, "~\n" );
	    }

	    fprintf( fp, "S\n" );
	}

    fprintf( fp, "#0\n\n\n" );

    fprintf( fp, "#RESETS\n" );

    for ( pReset = pArea->reset_first; pReset != NULL; pReset = pReset->next )
    {
	fprintf( fp, "%c 0 %d %d", pReset->command, pReset->arg1, pReset->arg2 );

	if ( pReset->command == 'G'
	|| pReset->command == 'I'
	|| pReset->command == 'R' )
	{
	}
	else if ( pReset->command == 'E' )
	{
	    extern struct wear_data const wear_info[];
	    fprintf( fp, " '%s'",
		pReset->arg3 > -1 ? wear_info[pReset->arg3].name : "none" );
	}
	else
	    fprintf( fp, " %d", pReset->arg3 );

	

	fprintf( fp, "\n" );
    }

    fprintf( fp, "S\n\n\n" );

    fprintf( fp, "#$\n" );

    fclose( fp );
    fpReserve = fopen( NULL_FILE, "r" );
    sprintf( buf, "cp %s.tmp %s.are", pArea->filename,
	pArea->filename );
    system( buf );
    sprintf( buf, "rm %s.tmp", pArea->filename );
    system( buf );

    return 1;
}


void do_vmob( CHAR_DATA *ch, char *argument )
{
    MOB_INDEX_DATA *pMobIndex = NULL;
    CHAR_DATA *mob;
    char buf[MAX_STRING_LENGTH];
    bool fFound = FALSE;
    int i;

    if ( argument[0] == '\0' )
    {
	send_to_char("Syntax:  vmob <vnum/name>\n\r", ch );
	return;
    }

    if ( is_number( argument ) )
    {
	pMobIndex = get_mob_index( atoi( argument ) );
    }
    else
    {
	if ( (mob = get_char_world( ch, argument )) != NULL )
	{
	    if ( !IS_NPC( mob ) )
	    {
		send_to_char("Target must be a non-player.\n\r", ch );
		return;
	    }

	    pMobIndex = mob->pIndexData;
	}
    }

    if ( pMobIndex == NULL )
    {
	send_to_char("No such mobile.\n\r", ch );
	return;
    }

    sprintf( buf, "Vnum: %d, Name: %s %s\n\r",
	pMobIndex->vnum,
	( pMobIndex->name[0] != '\0' ? pMobIndex->name : "(none)" ),
	( pMobIndex->namelist[0] != '\0' ? pMobIndex->namelist : "(none)" ) );
	send_to_char( buf, ch );


    if ( pMobIndex->clan != 0 )
    {
	sprintf( buf, "Clan: %s.\n\r",
	    clan_table[get_pow_2(pMobIndex->clan)+1] );
	send_to_char( buf, ch );
    }

    sprintf( buf, "Short: %s\n\r",
	( pMobIndex->short_descr[0] != '\0' ? pMobIndex->short_descr : "(none)" ) );
	send_to_char( buf, ch );

    sprintf( buf, "Long: %s",
	( pMobIndex->long_descr[0] != '\0' ? pMobIndex->long_descr : "(none)\n\r" ) );
	send_to_char( buf, ch );

    sprintf( buf, "Desc:\n\r %s",
	( pMobIndex->description[0] != '\0' ? pMobIndex->description : "(none)\n\r" ) );
	send_to_char( buf, ch );

    if ( pMobIndex->spec_fun_name[0] != '\0' )
    {
	sprintf( buf, "Spec: %s\n\r", pMobIndex->spec_fun_name );
	send_to_char( buf, ch );
    }

    if ( pMobIndex->pShop != NULL )
    {
	sprintf( buf, "Shop:  Buys: %d %d %d %d %d   Profits: %d %d   Hours: %d %d, Gain: %d.\n\r",
	    pMobIndex->pShop->buy_type[0],
	    pMobIndex->pShop->buy_type[1],
	    pMobIndex->pShop->buy_type[2],
	    pMobIndex->pShop->buy_type[3],
	    pMobIndex->pShop->buy_type[4],
	    pMobIndex->pShop->profit_buy,
	    pMobIndex->pShop->profit_sell,
	    pMobIndex->pShop->open_hour,
	    pMobIndex->pShop->close_hour,
	    pMobIndex->pShop->gain );
	send_to_char( buf, ch );
    }

    sprintf( buf, "Sex: %d, Level: %d, Act: %d, Affected by: %d, Align: %d, Speed: %d\n\r",
	pMobIndex->sex,
	pMobIndex->level,
	pMobIndex->act,
	pMobIndex->affected_by,
	pMobIndex->alignment,
	pMobIndex->speed );
    send_to_char( buf, ch );

    sprintf( buf, "Str: %d, Dex: %d, Tou: %d, Qui: %d, Int: %d.\n\r",
	pMobIndex->mob_stat_phys_str,
	pMobIndex->mob_stat_phys_dex,
	pMobIndex->mob_stat_phys_tou,
	pMobIndex->mob_stat_phys_qui,
	pMobIndex->mob_stat_ment_int );
    send_to_char( buf, ch );

    sprintf( buf, "Off: %d, Def: %d\n\r",
	pMobIndex->mob_offense,
	pMobIndex->mob_defense );
    send_to_char( buf, ch );

    sprintf( buf, "Absorb:  Hit: %d, Pierce: %d, Pound: %d, Slash: %d\n\r",
	pMobIndex->absorb[0],
	pMobIndex->absorb[1],
	pMobIndex->absorb[2],
	pMobIndex->absorb[3] );
    send_to_char( buf, ch );

    sprintf( buf, "Hits: %dd%d+%d,  Damage: %dd%d+%d\n\r",
	pMobIndex->hitnodice, pMobIndex->hitsizedice, pMobIndex->hitplus,
	pMobIndex->damnodice, pMobIndex->damsizedice, pMobIndex->damplus );
    send_to_char( buf, ch );

    sprintf( buf, "Position: %d\n\r",
	pMobIndex->start_pos );
    send_to_char( buf, ch );

    fFound = FALSE;
    send_to_char( "Coins:", ch );
    for ( i = 0; i < MAX_COIN; i++ )
    {
      if ( pMobIndex->coins[i] != 0 )
      {
	if ( fFound )
	  send_to_char( ",", ch );
	else
	  fFound = TRUE;

	sprintf( buf, " %d %s",
	  pMobIndex->coins[i], coin_table[i].name_plural );
	send_to_char( buf, ch );
      }
    }
    if ( !fFound )
      send_to_char( " none", ch );
    send_to_char( ".\n\r", ch );

    sprintf( buf, "Race: %d, Class: %d\n\r",
	pMobIndex->race, pMobIndex->class );
    send_to_char( buf, ch );

    sprintf( buf, "Skinning objects: %d %d %d %d\n\r",
	pMobIndex->skin_objects[0],
	pMobIndex->skin_objects[1],
	pMobIndex->skin_objects[2],
	pMobIndex->skin_objects[3] );
    send_to_char( buf, ch );

    return;
}

void do_vobj( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObjIndex = NULL;
    OBJ_DATA *obj;
    EXTRA_DESCR_DATA *ed;
    AFFECT_DATA *paf;
    char buf[MAX_STRING_LENGTH];

    extern char *material_color_name( int col );

    if ( argument[0] == '\0' )
    {
	send_to_char("Syntax:  vobj <vnum/name>\n\r", ch );
	return;
    }

    if ( is_number( argument ) )
    {
	pObjIndex = get_obj_index( atoi( argument ) );
    }
    else
    {
	if ( (obj = get_obj_world( ch, argument )) != NULL )
	{
	    pObjIndex = obj->pIndexData;
	}
    }

    if ( pObjIndex == NULL )
    {
	send_to_char("No such object.\n\r", ch );
	return;
    }

    sprintf( buf, "Vnum: %d,  Name: %s\n\r",
	pObjIndex->vnum, pObjIndex->name );
    send_to_char( buf, ch );

    sprintf( buf, "Material: %s\n\r",
	material_table[pObjIndex->material].name );
    send_to_char( buf, ch );

    sprintf( buf, "Color: %s\n\r",
	material_color_name( pObjIndex->color ) );
    send_to_char( buf, ch );

    sprintf( buf, "Short:  %s\n\r", pObjIndex->short_descr );
    send_to_char( buf, ch );

    sprintf( buf, "Look:  %s\n\r", pObjIndex->look_descr );
    send_to_char( buf, ch );

    sprintf( buf, "Desc:\n\r%s\n\r",
	(pObjIndex->description != NULL ? pObjIndex->description : "(none)" ));
    send_to_char( buf, ch );

    sprintf( buf, "Item Type: %d,  Extra flags: %d,  Wear flags: %d\n\r",
	pObjIndex->item_type, pObjIndex->extra_flags, pObjIndex->wear_flags );
    send_to_char( buf, ch );

    sprintf( buf, "Weight: %d,  Capacity: %d,  Cost: %d,  Load chance: %d\n\r",
	pObjIndex->weight, pObjIndex->capacity,
	pObjIndex->cost, pObjIndex->load_chance );
    send_to_char( buf, ch );

    sprintf( buf, "Values: %d %d %d %d\n\r",
	pObjIndex->value[0], pObjIndex->value[1],
	pObjIndex->value[2], pObjIndex->value[3] );
    send_to_char( buf, ch );

    sprintf( buf, "Damage: %dd%d, Attack type: %d,  Toughness: %d.\n\r",
	pObjIndex->dam[0], pObjIndex->dam[1],
	pObjIndex->attack_type, pObjIndex->toughness );
    send_to_char( buf, ch );

    if ( pObjIndex->extra_descr != NULL )
    {
	send_to_char("Extra descriptions:\n\r", ch );

	for ( ed = pObjIndex->extra_descr; ed != NULL; ed = ed->next )
	{
	    sprintf( buf, "%s:\n\r%s\n\r",
		ed->keyword, ed->description );
	    send_to_char( buf, ch );
	}
    }

    if ( pObjIndex->affected != NULL )
    {
	for ( paf = pObjIndex->affected; paf != NULL; paf = paf->next )
	{
	    sprintf( buf, "Affects %s (%d) by %d.\n\r",
		affect_loc_name( paf->location ), paf->location,
		paf->modifier );
	    send_to_char( buf, ch );
	}
    }

    return;
}

void write_string( FILE *fp, char *str )
{
    int ii;

    for (ii = 0; ; ii++)
    {
	switch ( str[ii] )
	{
	    case '\0':
		return;
		break;

	    case '\r':
		break;

	    case '~':
		fprintf( fp, "\\~" );
		break;

	    default:
		fprintf( fp, "%c", str[ii]);
		break;
	}
    }

    return;
}
