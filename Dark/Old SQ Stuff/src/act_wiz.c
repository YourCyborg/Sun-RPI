
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
ROOM_INDEX_DATA *	find_location	args( ( CHAR_DATA *ch, char *arg ) );


/*
void do_wizhelp( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int cmd;
    int col;
 
    col = 0;
    for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
    {
        if ( cmd_table[cmd].level >= LEVEL_IMMORTAL
        &&   cmd_table[cmd].level <= get_trust( ch ) )
	{
	    sprintf( buf, "%-2d %-12s",
		cmd_table[cmd].level, cmd_table[cmd].name );
	    send_to_char( buf, ch );
	    if ( ++col % 5 == 0 )
		send_to_char( "\n\r", ch );
	}
    }
 
    if ( col % 5 != 0 )
	send_to_char( "\n\r", ch );
    return;
}



*/

void do_wizhelp( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int cmd;
    int col;
    int clevel;
    col = 0;

    for( clevel = LEVEL_IMMORTAL ; clevel < MAX_LEVEL + 1; clevel++ )
    {
        for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
        {
            if ( cmd_table[cmd].level >= LEVEL_IMMORTAL
            &&   cmd_table[cmd].level <= get_trust( ch )
            &&   cmd_table[cmd].level == clevel )
            {
                sprintf( buf, "[%-3d] %-13s", cmd_table[cmd].level, cmd_table[cmd].name);
                send_to_char( buf, ch );
                if ( ++col % 4 == 0 )
                    send_to_char( "\n\r", ch );
            }
        }
    }

    if ( col % 4 != 0 )
        send_to_char( "\n\r", ch );
    return;
}

void do_history (CHAR_DATA * ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];
    extern char *ooc1, *ooc2, *ooc3, *ooc4, *ooc5, *ooc6, *ooc7, *ooc8;
    extern char *attack1, *attack2, *attack3, *attack4, *attack5, 
    *attack6, *attack7, *attack8, *attack9, *attack10;
    extern char *k1, *k2, *k3, *k4, *k5, *k6, *k7, *k8, *k9, *k10;

    if (argument[0] == '\0' )
    {
        send_to_char("Syntax: history < ooc/attack/kills >\n\r",ch );
        return;
    }

    if ( !str_cmp( argument, "ooc" ) )
    {
    send_to_char (".======================[ Last 8 OOC's ]======================.\n\r", ch);
    sprintf (buf, ". %s\n\r", ooc8);
    send_to_char (buf, ch);
    sprintf (buf, ". %s\n\r", ooc7);
    send_to_char (buf, ch);
    sprintf (buf, ". %s\n\r", ooc6);
    send_to_char (buf, ch);
    sprintf (buf, ". %s\n\r", ooc5);
    send_to_char (buf, ch);
    sprintf (buf, ". %s\n\r", ooc4);
    send_to_char (buf, ch);
    sprintf (buf, ". %s\n\r", ooc3);
    send_to_char (buf, ch);
    sprintf (buf, ". %s\n\r", ooc2);
    send_to_char (buf, ch);
    sprintf (buf, ". %s\n\r", ooc1);
    send_to_char (buf, ch);
    send_to_char (".==================================================================.\n\r", ch);
    return;
    }
    
    if ( !str_cmp( argument, "attack" ) )
    {
    send_to_char (".======================[ Last 5 Attacks ]======================.\n\r", ch);
    sprintf (buf, ". %s  <Defence>\n\r", attack10);
    send_to_char (buf, ch);
    sprintf (buf, ". %s  <Offence>\n\r\n\r", attack9);
    send_to_char (buf, ch);
    sprintf (buf, ". %s  <Defence>\n\r", attack8);
    send_to_char (buf, ch);
    sprintf (buf, ". %s  <Offence>\n\r\n\r", attack7);
    send_to_char (buf, ch);
    sprintf (buf, ". %s  <Defence>\n\r", attack6);
    send_to_char (buf, ch);
    sprintf (buf, ". %s  <Offence>\n\r\n\r", attack5);
    send_to_char (buf, ch);
    sprintf (buf, ". %s  <Defence>\n\r", attack4);
    send_to_char (buf, ch);
    sprintf (buf, ". %s  <Offence>\n\r\n\r", attack3);
    send_to_char (buf, ch);
    sprintf (buf, ". %s  <Defence>\n\r", attack2);
    send_to_char (buf, ch);
    sprintf (buf, ". %s  <Offence>\n\r", attack1);
    send_to_char (buf, ch);
    send_to_char (".===============================================================.\n\r", ch);
    return;
    }

    if ( !str_cmp( argument, "kills" ) )
    {
    send_to_char (".======================[ Last 10 Deaths ]======================.\n\r", ch);
    sprintf (buf, ". %s\n\r", k10);
    send_to_char (buf, ch);
    sprintf (buf, ". %s\n\r", k9);
    send_to_char (buf, ch);
    sprintf (buf, ". %s\n\r", k8);
    send_to_char (buf, ch);
    sprintf (buf, ". %s\n\r", k7);
    send_to_char (buf, ch);
    sprintf (buf, ". %s\n\r", k6);
    send_to_char (buf, ch);
    sprintf (buf, ". %s\n\r", k5);
    send_to_char (buf, ch);
    sprintf (buf, ". %s\n\r", k4);
    send_to_char (buf, ch);
    sprintf (buf, ". %s\n\r", k3);
    send_to_char (buf, ch);
    sprintf (buf, ". %s\n\r", k2);
    send_to_char (buf, ch);
    sprintf (buf, ". %s\n\r", k1);
    send_to_char (buf, ch);
    send_to_char (".==================================================================.\n\r", ch);
    return;
    }

    send_to_char("Syntax: history < ooc/attack/kills >\n\r",ch);
    
}

void do_bamfin( CHAR_DATA *ch, char *argument )
{
    if ( !IS_NPC(ch) )
    {
	free_string( ch->pcdata->bamfin, MEM_PCDATA );
	ch->pcdata->bamfin = str_dup( argument );
	send_to_char( "Ok.\n\r", ch );
    }
    return;
}


void do_pload( CHAR_DATA *ch, char *arg )
{
    char *name;
    CHAR_DATA *rch;
    DESCRIPTOR_DATA *d;
    char strsave[MAX_INPUT_LENGTH];
    FILE *fp;

    if ( arg == NULL || arg[0] == '\0' )
    {
	send_to_char("Syntax: pload <character>\n\r", ch );
	return;
    }

    name = capitalize( arg );

    for ( rch = char_list; rch != NULL; rch = rch->next )
    {
	if ( !IS_NPC(rch) && !str_cmp( rch->name, name ) )
	{
	    send_to_char("That character is already in the game.\n\r", ch );
	    return;
	}
    }

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d->character == NULL )
	    continue;

	if ( !str_cmp( d->character->name, name ) )
	{
	    send_to_char("That character is logging on.\n\r", ch );
	    return;
	}
    }

    sprintf( strsave, "%s%s", PLAYER_DIR, name );
    if ( ( fp = fopen( strsave, "r" ) ) == NULL )
    {
        sprintf( strsave, "%s%s", UNAPPROVED_DIR, name );
        fp = fopen( strsave, "r" );
    }
    if ( fp == NULL )
    {
        sprintf( strsave, "%s%s", REJECTED_DIR, name );
        fp = fopen( strsave, "r" );
    }
    if ( fp == NULL )
    {
	send_to_char("No such character exists.\n\r", ch );
	return;
    }

    if ( char_free == NULL )
    {
        rch                              = alloc_perm( sizeof(*ch) );
    }
    else
    {
        rch                              = char_free;
        char_free                       = char_free->next;
    }
    clear_char( rch );

    init_char( rch, name, TRUE );


    load_char_from_file( rch, fp );
    fclose( fp );

    char_to_room( rch, get_room_index( ROOM_VNUM_REVIEW_ROOM ), FALSE );


    rch->next = char_list;
    char_list = rch;

    send_to_char( "Character loaded to review room.\n\r", ch );
    send_to_char( "Use PUNLOAD to save them and quit them out.\n\r",ch );
    return;
}

void do_bamfout( CHAR_DATA *ch, char *argument )
{
    if ( !IS_NPC(ch) )
    {
	free_string( ch->pcdata->bamfout, MEM_PCDATA );
	ch->pcdata->bamfout = str_dup( argument );
	send_to_char( "Ok.\n\r", ch );
    }
    return;
}



void do_deny( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Deny whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if ( get_trust( victim ) >= get_trust( ch ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    SET_BIT(victim->act, PLR_DENY);
    send_to_char( "You are denied access!\n\r", victim );
    send_to_char( "OK.\n\r", ch );
    do_quit( victim, "" );

    return;
}



void do_disconnect( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    DESCRIPTOR_DATA *d;
    CHAR_DATA *victim;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Disconnect whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim->desc == NULL )
    {
	act( "$N doesn't have a descriptor.", ch, victim, NULL, NULL, NULL, TO_CHAR, SENSE_SIXTH );
	return;
    }

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d == victim->desc )
	{
	    close_socket( d );
	    send_to_char( "Ok.\n\r", ch );
	    return;
	}
    }

    bug( "Do_disconnect: desc not found.", 0 );
    send_to_char( "Descriptor not found!\n\r", ch );
    return;
}

void do_ulog( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH]; // Buffer

    if ( argument[0] == '\0' ) // No argument, do the following.
    {
        send_to_char("You must type ulog <update/clear>\n\r",ch);
        return;
    }

    if ( !str_cmp( argument, "update" ) )
    {
        send_to_char("Updating logs...\n\r",ch);
        sprintf( buf, "./%supdate",LOG_DIR );
        system ( buf );
        send_to_char("Done.\n\r",ch);
        return;
    }

    if ( !str_cmp( argument, "clear" ) )
    {
        send_to_char("Clearing logs...\n\r",ch );
        sprintf( buf, "./%sclear",LOG_DIR );
        system( buf );
        send_to_char("Done.\n\r",ch);
        return;
    }
    send_to_char("Usage: ulog <update/clear>\n\r",ch);
    return;
}
    

void do_echo( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    
    if ( argument[0] == '\0' )
    {
	send_to_char( "Echo what?\n\r", ch );
	return;
    }

    for ( d = descriptor_list; d; d = d->next )
    {
	if ( d->connected == CON_PLAYING )
	{
	    send_to_char( argument, d->character );
	    send_to_char( "\n\r",   d->character );
	}
    }

    return;
}



void do_recho( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    
    if ( argument[0] == '\0' )
    {
	send_to_char( "Recho what?\n\r", ch );
	return;
    }

    for ( d = descriptor_list; d; d = d->next )
    {
	if ( d->connected == CON_PLAYING
	&&   d->character->in_room == ch->in_room )
	{
	    send_to_char( argument, d->character );
	    send_to_char( "\n\r",   d->character );
	}
    }

    return;
}



ROOM_INDEX_DATA *find_location( CHAR_DATA *ch, char *arg )
{
    CHAR_DATA *victim;
    OBJ_DATA *obj;

    if ( is_number(arg) )
	return get_room_index( atoi( arg ) );

    if ( ( victim = get_char_world( ch, arg ) ) != NULL )
	return victim->in_room;

    if ( ( obj = get_obj_world( ch, arg ) ) != NULL )
	return obj->in_room;

    return NULL;
}



void do_transfer( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA *location;
    DESCRIPTOR_DATA *d;
    CHAR_DATA *victim;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' )
    {
	send_to_char( "Transfer whom (and where)?\n\r", ch );
	return;
    }

    if ( !str_cmp( arg1, "all" ) )
    {
	if ( get_trust( ch ) < 39 )
	{
	    send_to_char("You can't do that.\n\r", ch );
	    return;
	}

	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    if ( d->connected == CON_PLAYING
	    &&   d->character != ch
	    &&   d->character->in_room != NULL
	    &&   can_see( ch, d->character ) )
	    {
		char buf[MAX_STRING_LENGTH];
		sprintf( buf, "%s %s", d->character->name, arg2 );
		do_transfer( ch, buf );
	    }
	}
	return;
    }

    /*
     * Thanks to Grodyn for the optional location parameter.
     */
    if ( arg2[0] == '\0' )
    {
	location = ch->in_room;
    }
    else
    {
	if ( ( location = find_location( ch, arg2 ) ) == NULL )
	{
	    send_to_char( "No such location.\n\r", ch );
	    return;
	}

	if ( room_is_private( location ) && get_trust(ch) < 40 )
	{
	    send_to_char( "That room is private right now.\n\r", ch );
	    return;
	}
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim->in_room == NULL )
    {
	send_to_char( "They are in limbo.\n\r", ch );
	return;
    }

    if ( !IS_NPC( victim ) )
    {
	if ( get_trust( ch ) < 38 )
	{
	    send_to_char("You can't do that.\n\r", ch );
	    return;
	}
    }
    else
    {
	if ( get_trust( ch ) < 38
	&& !can_build( ( ch->desc->original != NULL ?
	                 ch->desc->original : ch), victim->pIndexData->area ) )
	{
	    send_to_char("That mobile is not in your zone.\n\r", ch );
	    return;
	}
    }

    if ( check_fighting( victim ) )
	stop_fighting( victim, TRUE );
   // act( "$n disappears in a mushroom cloud.", victim, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIGHT );
    char_from_room( victim );
    char_to_room( victim, location, TRUE );
//    act( "$n arrives from a puff of smoke.", victim, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIGHT );
    if ( ch != victim )
//	act( "$n has transferred you.", ch, victim, NULL, NULL, NULL, TO_VICT, SENSE_SIXTH );
//    do_look( victim, "auto" );
    send_to_char( "Ok.\n\r", ch );
}



void do_at( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA *location;
    ROOM_INDEX_DATA *original;
    CHAR_DATA *wch;
    
    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
	send_to_char( "At where what?\n\r", ch );
	return;
    }

    if ( ( location = find_location( ch, arg ) ) == NULL )
    {
	send_to_char( "No such location.\n\r", ch );
	return;
    }

    if ( room_is_private( location ) && get_trust(ch) < 40 )
    {
	send_to_char( "That room is private right now.\n\r", ch );
	return;
    }

    original = ch->in_room;
    char_from_room( ch );
    char_to_room( ch, location, FALSE );
    interpret( ch, argument );

    /*
     * See if 'ch' still exists before continuing!
     * Handles 'at XXXX quit' case.
     */
    for ( wch = char_list; wch != NULL; wch = wch->next )
    {
	if ( wch == ch )
	{
	    char_from_room( ch );
	    char_to_room( ch, original, FALSE );
	    break;
	}
    }

    return;
}


void perform_goto( CHAR_DATA *ch, ROOM_INDEX_DATA *location )
{
    CHAR_DATA *rch;
    int count = 0;
    count = 0;

	if ( location->vnum <= 0 )
	{
		send_to_char("That location is not valid. Perhaps the reset doesn't exist?\n\r", ch);
		return;
	}

    for ( rch = location->people; rch != NULL; rch = rch->next_in_room )
        count++;

    if ( check_fighting( ch ) )
	stop_fighting( ch, TRUE );

    

  

    if ( !IS_SET(ch->act, PLR_WIZINVIS) )
    {
        act( "$n $T.", ch, NULL, NULL, NULL,
            (ch->pcdata != NULL && ch->pcdata->bamfout[0] != '\0')
            ? ch->pcdata->bamfout : "leaves in a swirling mist",
            TO_ROOM, SENSE_SIGHT );
    }

    if ( IS_SET(location->room_flags, ROOM_PRIVATE ) && get_trust(ch) < 40)
    {
        send_to_char( "That room is private right now.\n\r", ch );
        return;
    }

    char_from_room( ch );
    char_to_room( ch, location, TRUE );

    if ( !IS_SET(ch->act, PLR_WIZINVIS) )
    {
        act( "$n $T.", ch, NULL, NULL, NULL,
            (ch->pcdata != NULL && ch->pcdata->bamfin[0] != '\0')
            ? ch->pcdata->bamfin : "appears in a swirling mist",
            TO_ROOM, SENSE_SIGHT );
    }

    
    do_look(ch, "auto" );
    return;
}
void do_newgoto( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *location = NULL;
	char arg1[MAX_INPUT_LENGTH];
       char arg2[MAX_INPUT_LENGTH];
       char buf[MAX_INPUT_LENGTH];

	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' || !str_cmp( arg1, "?" ) || !str_cmp( arg1, "help" ) )
    {
		send_to_char("Goto where?\n\r", ch );
		send_to_char("Goto Char  <PC Name>\n\r", ch );
		send_to_char("Goto Mob   <Mobile Name>\n\r", ch );
		send_to_char("Goto Mobv  <Mobile Vnum>\n\r", ch );
		send_to_char("Goto Room  <Room Vnum>\n\r", ch );
		send_to_char("Goto Obj   <Obj Name>\n\r", ch );
		send_to_char("Goto Objv  <Obj Vnum>\n\r", ch );
		return;
    }

	/* working 05.01.2004 - Dalsor */
	if ( !str_cmp( arg1, "char" ) )
	{
		CHAR_DATA *victim;
		if ( ( victim = get_pc_world( ch, arg2 ) ) != NULL)
		{
			location = victim->in_room;
			if ( location != NULL )
				perform_goto( ch, location );
			else
			{
				send_to_char("The location appears to be null.\n\r", ch);
				return;
			}
		}
		else
		{
			send_to_char("No such PC in the world.\n\r", ch);
			return;
		}
	}
	/* working 05.01.2004 - Dalsor */
	if ( !str_cmp( arg1, "mob" ) )
	{
		CHAR_DATA *victim;
		if ( ( victim = get_mob_world( ch, arg2 ) ) != NULL )
		{
			if ( !IS_NPC( victim ) )
			{
				send_to_char("For some reason, get_mob_world returned a PC\n\r", ch);
				return;
			}
			if ( ( location = victim->in_room ) != NULL )
				perform_goto( ch, location );
			else
			{
				send_to_char("The location appears to be null.\n\r", ch);
				return;
			}
		}
		else
		{
			send_to_char("Could not find that mobile.\n\r", ch);
			return;
		}
	}
	/* working 05.01.2004 - Dalsor */
	if ( !str_cmp( arg1, "mobv" ) )
	{
		CHAR_DATA *victim;
		MOB_INDEX_DATA *pMob;
		long vnum;
		if ( !is_number( arg2 ) )
		{
			send_to_char("Location must be a valid mobile vnum.\n\r", ch);
			return;
		}
		vnum = atoi( arg2 );
		if ( !( pMob = get_mob_index( vnum ) ) )
		{
			send_to_char("No such mobile index.\n\r", ch);
			return;
		}
		if ( ( victim = get_mob_by_vnum( ch, arg2 ) ) != NULL )
		{
			if ( ( location = victim->in_room ) != NULL )
			{
				if ( location->vnum <= 0 )
				{
					send_to_char("The object does not appear to be reset in the world.\n\r", ch);
					return;
				}
				else
					perform_goto( ch, location );
			}
			else
			{
				send_to_char("The location appears to be null.\n\r", ch);
				return;
			}
		}
		else
		{
			send_to_char("Could not find that mobile.\n\r", ch);
			return;
		}
	}
	/* working 05.01.2004 - Dalsor */
	if ( !str_cmp( arg1, "room" ) )
	{
		long vnum;
		if ( !is_number( arg2 ) )
		{
			send_to_char("Location must be a valid room vnum.\n\r", ch);
			return;
		}
		vnum = atoi( arg2 );
		if ( ( location = get_room_index( vnum ) ) != NULL )
			perform_goto( ch, location );
		else
		{
			send_to_char("The location appears to be null.\n\r", ch);
			return;
		}
	}
	/* working 05.01.2004 - Dalsor */
	if ( !str_cmp( arg1, "obj" ) )
	{
		OBJ_DATA *obj;
		if ( ( obj = get_obj_world( ch, arg2 ) ) != NULL )
		{
			/*
			if ( obj->carried_by != NULL )
			{
				sprintf( buf, "%s is carried by %s in room %ld.",
					obj->short_descr, obj->carried_by->name, obj->carried_by->in_room->vnum );
                           send_to_char(buf, ch);
				location = obj->carried_by->in_room;
			}
			else
				location = obj->in_room;
			*/
			while(obj->carried_by ==NULL && obj->in_room == NULL)
			{
				sprintf( buf, "%s is in %s.\n\r",
					obj->short_descr, obj->in_obj->short_descr);
                            send_to_char( buf, ch );

				if(!obj->in_obj)
				{
					sprintf(buf,"%s is not in_room, carried_by or in_obj.\n\r",obj->short_descr);
                                  send_to_char(buf, ch);
					return;
				}
				obj = obj->in_obj;
			}
			if ( obj->carried_by != NULL )
			{
				sprintf( buf, "%s is carried by %s in room %d.\n\r",
					obj->short_descr, obj->carried_by->name, obj->carried_by->in_room->vnum );
                            send_to_char(buf, ch);
				location = obj->carried_by->in_room;
			}
			else
			{
				sprintf( buf, "%s is in room %d.\n\r",
						obj->short_descr, obj->in_room->vnum);
                            send_to_char(buf, ch);
				location = obj->in_room;
			}
		}
		if ( location != NULL )
			perform_goto( ch, location );
		else
		{
			send_to_char("The location appears to be null.\n\r", ch);
			return;
		}
	}
	/* working 05.01.2004 - Dalsor */
	if ( !str_cmp( arg1, "objv" ) )
	{
		OBJ_DATA *obj;
		OBJ_INDEX_DATA *pObj;
		long vnum;
		if ( !is_number( arg2 ) )
		{
			send_to_char("Location must be a valid object vnum.\n\r", ch);
			return;
		}
		vnum = atoi( arg2 );
		if ( !( pObj = get_obj_index( vnum ) ) )
		{
			send_to_char("No such object index.\n\r", ch);
			return;
		}
		if ( ( obj = get_obj_here( ch, arg2 ) ) != NULL )
		{
			/*
			if ( obj->carried_by != NULL )
			{
				sprintf( buf, "%s is carried by %s in room %s.",
					obj->short_descr, obj->carried_by->name, obj->carried_by->in_room->vnum );
                           send_to_char(buf, ch);
				location = obj->carried_by->in_room;
			}
			else
				location = obj->in_room;
			*/
			while(obj->carried_by ==NULL && obj->in_room == NULL)
			{
				sprintf( buf, "%s is in %s.\n\r",
					obj->short_descr, obj->in_obj->short_descr);
                           send_to_char(buf, ch);
				if(!obj->in_obj)
				{
					sprintf(buf,"%s is not in_room, carried_by or in_obj.\n\r",obj->short_descr);
                                  send_to_char(buf, ch);
					return;
				}
				obj = obj->in_obj;
			}
			if ( obj->carried_by != NULL )
			{
				sprintf( buf, "%s is carried by %s in room %d.\n\r",
					obj->short_descr, obj->carried_by->name, obj->carried_by->in_room->vnum );
                            send_to_char(buf, ch);
				location = obj->carried_by->in_room;
			}
			else
			{
				sprintf( buf, "%s is in room %d.\n\r",
						obj->short_descr, obj->in_room->vnum);
                            send_to_char(buf, ch);
				location = obj->in_room;
			}
		}
		/* redundant and crash prone *
		if ( location->vnum <= 0 )
		{
			send_to_char("The object does not appear to be reset in the world.\n\r", ch);
			return;
		} */
		if ( location != NULL )
		{
			if ( location->vnum <= 0 )
			{
				send_to_char("The object does not appear to be reset in the world.\n\r", ch);
				return;
			}
			else
				perform_goto( ch, location );
		}
		else
		{
			send_to_char("The location appears to be null.\n\r", ch);
			return;
		}
	}
    return;
}
/*
void do_goto( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA *location;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Goto where?\n\r", ch );
	return;
    }

    if ( ( location = find_location( ch, arg ) ) == NULL )
    {
	send_to_char( "No such location.\n\r", ch );
	return;
    }

    if ( room_is_private( location ) && get_trust(ch) < 40)
    {
	send_to_char( "That room is private right now.\n\r", ch );
	return;
    }

    if ( check_fighting( ch ) )
	stop_fighting( ch, TRUE );
    if ( !IS_SET(ch->act, PLR_WIZINVIS) )
    {
	act( "$n $T.", ch, NULL, NULL, NULL,
	    (ch->pcdata != NULL && ch->pcdata->bamfout[0] != '\0')
	    ? ch->pcdata->bamfout : "leaves in a swirling mist",
	    TO_ROOM, SENSE_SIGHT );
    }

    char_from_room( ch );
    char_to_room( ch, location, TRUE );

    if ( !IS_SET(ch->act, PLR_WIZINVIS) )
    {
	act( "$n $T.", ch, NULL, NULL, NULL,
	    (ch->pcdata != NULL && ch->pcdata->bamfin[0] != '\0')
	    ? ch->pcdata->bamfin : "appears in a swirling mist",
	    TO_ROOM, SENSE_SIGHT );
    }

    do_look( ch, "auto" );
    return;
}*/



void do_rstat( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA *location;
    OBJ_DATA *obj;
    CHAR_DATA *rch;
    int door;

    one_argument( argument, arg );
    location = ( arg[0] == '\0' ) ? ch->in_room : find_location( ch, arg );
    if ( location == NULL )
    {
	send_to_char( "No such location.\n\r", ch );
	return;
    }

    if ( ch->in_room != location && room_is_private( location )
	&& get_trust(ch) < 40 )
    {
	send_to_char( "That room is private right now.\n\r", ch );
	return;
    }

    sprintf( buf, "Name: '%s.'\n\rArea: '%s'.  Reset time: %d (%d).\n\r",
	location->name,
	location->area->name,
	location->area->age, location->area->reset_length );
    send_to_char( buf, ch );

    sprintf( buf, "Builders: %s\n\r", location->area->builders );
    send_to_char( buf, ch );

    sprintf( buf,
	"Vnum: %d.  Cost: %d.  Delay: %d.  Light: %d+%d.  Size: %d.\n\r",
	location->vnum,
	location->move_cost,
	location->move_delay,
	location->light,
	location->light_mod,
	location->size );
    send_to_char( buf, ch );

    if ( location->spec_fun != 0 )
    {
	send_to_char( "Room has spec: ", ch );
	send_to_char( location->spec_fun_name, ch );
	send_to_char( ".\n\r", ch );
    }

    sprintf( buf,
	"Sector: %s.  Room flags: %d.\n\r",
	location->sector->name,
	location->room_flags );
    send_to_char( buf, ch );

    sprintf( buf, "Water:  %d (%d,%d)\n\r",
      get_water_value( location ),
      location->water_min, location->water_degrade );
    send_to_char( buf, ch );

    sprintf( buf,
	"Description:\n\r%s",
	location->description );
    send_to_char( buf, ch );

    if ( location->extra_descr != NULL )
    {
	EXTRA_DESCR_DATA *ed;

	send_to_char( "Extra description keywords: '", ch );
	for ( ed = location->extra_descr; ed; ed = ed->next )
	{
	    send_to_char( ed->keyword, ch );
	    if ( ed->next != NULL )
		send_to_char( " ", ch );
	}
	send_to_char( "'.\n\r", ch );
    }

    send_to_char( "Characters:", ch );
    for ( rch = location->people; rch; rch = rch->next_in_room )
    {
	send_to_char( " ", ch );
	one_argument( rch->name, buf );
	send_to_char( buf, ch );
    }

    send_to_char( ".\n\rObjects:   ", ch );
    for ( obj = location->contents; obj; obj = obj->next_content )
    {
	send_to_char( " ", ch );
	one_argument( obj->name, buf );
	send_to_char( buf, ch );
    }
    send_to_char( ".\n\r", ch );

    for ( door = 0; door <= 5; door++ )
    {
	EXIT_DATA *pexit;

	if ( ( pexit = location->exit[door] ) != NULL )
	{
	    sprintf( buf,
		"Door: %d.  To: %d.  Key: %d.  Exit flags: %d.  Difficulty: %d."
		"\n\rKeyword: '%s'.  Description: %s ",

		door,
		pexit->to_room != NULL ? pexit->to_room->vnum : 0,
	    	pexit->key,
	    	pexit->exit_flags,
		pexit->difficulty,
	    	pexit->keyword,
	    	pexit->description[0] != '\0'
		    ? pexit->description : "(none).\n\r" ),
//                pexit->cdescription[0] != '\0' ? pexit->cdescription : "(none).\n\r" );
                send_to_char( buf, ch );
	}
    }

    return;
}



void do_ostat( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    AFFECT_DATA *paf;
    OBJ_DATA *obj;

    extern char *material_color_name( int color );

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Ostat what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_world( ch, arg ) ) == NULL )
    {
	send_to_char( "Nothing like that in hell, earth, or heaven.\n\r", ch );
	return;
    }

    sprintf( buf, "Name: %s.\n\r",
	obj->name );
    send_to_char( buf, ch );

  if ( obj->pIndexData != NULL )
    sprintf( buf, "Vnum: %d.  Type: %s.  Instances: %d/%d.\n\r",
	obj->pIndexData->vnum, item_type_name( obj ),
	obj->pIndexData->number, obj->pIndexData->limit );
  else
    sprintf( buf, "Type: %s.\n\r", item_type_name( obj ) );
    send_to_char( buf, ch );

    sprintf( buf, "Material: %s.\n\r",
	material_table[obj->material].name );
    send_to_char( buf, ch );

    sprintf( buf, "Color: %s\n\r",
	material_color_name( obj->color ) );
    send_to_char( buf, ch );

    sprintf( buf, "Short description: %s.\n\rLong description: %s\n\r",
	obj->short_descr, obj->look_descr );
    send_to_char( buf, ch );

    sprintf( buf, "Description:\n\r%s\n\r",
	obj->description );
    send_to_char( buf, ch );

    if ( obj->spec_fun != 0 )
    {
	sprintf( buf, "Object has spec: %s.\n\r", obj->spec_fun_name );
	send_to_char( buf, ch );
    }

    sprintf( buf, "Wear bits: %d.  Extra bits: %s.\n\r",
	obj->wear_flags, extra_bit_name( obj->extra_flags ) );
    send_to_char( buf, ch );

    sprintf( buf, "Weight: %d/%d (/10 pounds).  Capacity: %d.  Size: %d.\n\r",
	obj->weight, get_obj_weight( obj ),
	obj->capacity,
	obj->size );
    send_to_char( buf, ch );

    sprintf( buf, "Cost: %d.  Timer: %d.\n\r",
	obj->cost, obj->timer );
    send_to_char( buf, ch );

    sprintf( buf,
	"In room: %d.  In object: %s.  Carried by: %s.  Wear_loc: %d.\n\r",
	obj->in_room    == NULL    ?        0 : obj->in_room->vnum,
	obj->in_obj     == NULL    ? "(none)" : obj->in_obj->short_descr,
	obj->carried_by == NULL    ? "(none)" : obj->carried_by->name,
	obj->wear_loc );
    send_to_char( buf, ch );
    
    sprintf( buf, "Values: %d %d %d %d.\n\r",
	obj->value[0], obj->value[1], obj->value[2], obj->value[3] );
    send_to_char( buf, ch );

    sprintf( buf, "Damage: %dd%d.   Toughness: %d.  Attack type: %d.\n\r",
	obj->dam[0], obj->dam[1], obj->toughness, obj->attack_type );
    send_to_char( buf, ch );

    if ( obj->extra_descr != NULL
    || ( obj->pIndexData != NULL && obj->pIndexData->extra_descr != NULL ) )
    {
	EXTRA_DESCR_DATA *ed;

	send_to_char( "Extra description keywords: '", ch );

	for ( ed = obj->extra_descr; ed != NULL; ed = ed->next )
	{
	    send_to_char( ed->keyword, ch );
	    if ( ed->next != NULL )
		send_to_char( " ", ch );
	}

      if ( obj->pIndexData != NULL )
	for ( ed = obj->pIndexData->extra_descr; ed != NULL; ed = ed->next )
	{
	    send_to_char( ed->keyword, ch );
	    if ( ed->next != NULL )
		send_to_char( " ", ch );
	}

	send_to_char( "'.\n\r", ch );
    }

    for ( paf = obj->affected; paf != NULL; paf = paf->next )
    {
	sprintf( buf, "Affects %s by %d.\n\r",
	    affect_loc_name( paf->location ), paf->modifier );
	send_to_char( buf, ch );
    }

    return;
}


int get_pow_2( int in )
{
    int out;
    int o2 = 1;

    for ( out = 1; o2 <= in; out++ )
    {
	o2 *= 2;

	if ( o2 == in )
	    return out;
    }

    return 0;
}


int p2( int in )
{
    int out = 1;
    int ii;

    if ( in == 0 )
	return 1;

    for ( ii = 1; ii <= in; ii++ )
    {
	out *= 2;
    }

    return out;
}



void do_mstat( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    AFFECT_DATA *paf;
    CHAR_DATA *victim;
    HUNTING_DATA *h;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Mstat whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    sprintf( buf, "Name: %s  %s.\n\r",
	victim->name, victim->namelist );
    send_to_char( buf, ch );

    if ( !IS_NPC( victim ) )
    {
	sprintf( buf, "Email: %s.\n\r", victim->pcdata->email_address );
	send_to_char( buf, ch );
    }

    if ( victim->clan != 0 )
    {
	int ic;
	int icb;

	if ( IS_NPC( victim ) )
	{
	    sprintf( buf, "Clan: %s.\n\r",
		clan_table[get_pow_2(victim->clan)+1] );
	    send_to_char( buf, ch );
	}
	else
	{
	    icb = 1;

	    for ( ic = 1; ic < MAX_CLAN; ic++ )
	    {
	      if ( ic > 1 )
		icb *= 2;

		if ( IS_SET( icb, victim->clan ) )
		{
		    sprintf( buf, "Clan: %s.\n\r", clan_table[ic] );
		    send_to_char( buf, ch );
		}
	    }
	}
    }

    sprintf( buf, "Vnum: %d.  Sex: %s.  Room: %d.  Wait: %d.  Speed: %d.\n\r",
	IS_NPC(victim) ? victim->pIndexData->vnum : 0,
	victim->sex == SEX_MALE    ? "male"   :
	victim->sex == SEX_FEMALE  ? "female" : "neutral",
	victim->in_room == NULL    ?        0 : victim->in_room->vnum,
	victim->wait,
	victim->speed
	);
    send_to_char( buf, ch );

    sprintf( buf, "Str: %d.  Int: %d.  Dex: %d.  Tou: %d.  Qui: %d.\n\r",
	get_curr_str(victim),
	get_curr_int(victim),
	get_curr_dex(victim),
	get_curr_tou(victim),
	get_curr_qui(victim) );
    send_to_char( buf, ch );

    sprintf( buf, "Off: %d  Def: %d.\n\r",
	victim->perm_offense,
	victim->perm_defense );
    send_to_char( buf, ch );

    sprintf( buf, "Absorb:  Hit: %d.  Pierce: %d.  Pound: %d.  Slash: %d.  "
	"General: %d.\n\r",
	victim->absorb[0],
	victim->absorb[1],
	victim->absorb[2],
	victim->absorb[3],
	victim->general_absorb );
    send_to_char( buf, ch );

    sprintf( buf, "Hp: %d/%d.  Mana: %d/%d.  Move: %d/%d.  Stun %d/%d.\n\r  Height: %d.  Weight: %d.\n\r",
	victim->hit[HIT_LOC_BODY],         victim->max_hit,
	victim->mana,        victim->max_mana,
	victim->move,        victim->max_move,
        victim->stun,        victim->max_stun,
	victim->height,      victim->weight );
    send_to_char( buf, ch );
	
    sprintf( buf,
	"Lv: %d.  Race: %s (%d).  Class: %s[%d].  Align: %d.\n\r",
	victim->level,	race_array[victim->race]->race_name,
	victim->race,  class_table[victim->class].class_name, 
        victim->class,    victim->alignment );
    send_to_char( buf, ch );

    sprintf( buf, "Hitroll: %d.  Damroll: %d.  Position: %d.\n\r",
	GET_HITROLL(victim),  GET_DAMROLL(victim),
	victim->position );
    send_to_char( buf, ch );

    sprintf( buf, "Channeling: %d.   Light:  %d.\n\r",
	victim->channeling, victim->light );
    send_to_char( buf, ch );

    sprintf( buf, "Vision:  Min: %d, Max: %d, Current: %d, Range: %d.\n\r",
	victim->vision_min,
	victim->vision_max,
	victim->vision_current,
	victim->vision_range );
    send_to_char( buf, ch );
	
    sprintf( buf, "Fighting: %s.\n\r",
	victim->fighting ? victim->fighting->name : "(none)" );
    send_to_char( buf, ch );

    if ( !IS_NPC(victim) )
    {
	sprintf( buf,
	    "Thirst: %d.  Full: %d.  Drunk: %d.\n\r",
	    victim->pcdata->condition[COND_THIRST],
	    victim->pcdata->condition[COND_FULL],
	    victim->pcdata->condition[COND_DRUNK] );
	send_to_char( buf, ch );
    }

    sprintf( buf, "Save: %d.  Carry number: %d.  Carry weight: %d.\n\r",
	victim->saving_throw,
	victim->carry_number, victim->carry_weight );
    send_to_char( buf, ch );

    sprintf( buf, "Timer: %d.  Act: %d.  Aggression: %d.\n\r",
	victim->timer, victim->act, victim->aggression );
    send_to_char( buf, ch );

    sprintf( buf, "Master: %s.  Leader: %s.  Affected by: %s.\n\r",
	victim->master      ? victim->master->name   : "(none)",
	victim->leader      ? victim->leader->name   : "(none)",
	affect_bit_name( victim->affected_by ) );
    send_to_char( buf, ch );

    sprintf( buf, "Short description: %s.\n\rLong  description: %s",
	victim->short_descr,
	victim->long_descr[0] != '\0' ? victim->long_descr : "(none).\n\r" );
    send_to_char( buf, ch );

    sprintf( buf, "Dynamic description: %s",
      victim->dynamic_descr ? victim->dynamic_descr : "(none).\n\r" );
    send_to_char(buf, ch );

    if ( IS_NPC(victim) && victim->spec_fun != 0 )
    {
	send_to_char( "Mobile has spec: ", ch );
	send_to_char( victim->spec_fun_name, ch );
	send_to_char( ".\n\r", ch );
    }

    for ( paf = victim->affected; paf != NULL; paf = paf->next )
    {
	sprintf( buf,
	    "Spell: '%s' modifies %s by %d for %d seconds with bits %s.\n\r",
	    skill_table[(int) paf->type].name,
	    affect_loc_name( paf->location ),
	    paf->modifier,
	    paf->duration / 4,
	    affect_bit_name( paf->bitvector )
	    );
	send_to_char( buf, ch );
    }

    for ( h = victim->hunting; h != NULL; h = h->next )
    {
      if ( h->name[0] != '\0' )
        sprintf( buf, "Hunting: %s\n\r", h->name );
      else
	sprintf( buf, "Hunting: %s\n\r",
	   ( IS_NPC( h->victim ) ? h->victim->short_descr : h->victim->name ) );

      send_to_char( buf, ch );
    }
    
    sprintf( buf, "Played for %d days and %d hours.\n\r",
        get_hours_played( victim ) / 24, (get_hours_played( victim ) % 24) );
    send_to_char( buf, ch );

    sprintf( buf, "Last login time: %s\n\r", ch->time );
    send_to_char( buf, ch);

    return;
}



void do_mfind( CHAR_DATA *ch, char *argument )
{
    extern int top_mob_index;
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    MOB_INDEX_DATA *pMobIndex;
    int vnum;
    int nMatch;
    bool fAll;
    bool found;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Mfind whom?\n\r", ch );
	return;
    }

    fAll	= !str_cmp( arg, "all" );
    found	= FALSE;
    nMatch	= 0;

    for ( vnum = 0; nMatch < top_mob_index; vnum++ )
    {
	if ( ( pMobIndex = get_mob_index( vnum ) ) != NULL )
	{
	    nMatch++;
	    if ( fAll || is_name( arg, pMobIndex->name )
	    || is_name( arg, pMobIndex->namelist ) )
	    {
		found = TRUE;
		sprintf( buf, "[%5d] %s\n\r",
		    pMobIndex->vnum, pMobIndex->short_descr );
		send_to_char( buf, ch );
	    }
        }
    }

    if ( !found )
	send_to_char( "Nothing like that in hell, earth, or heaven.\n\r", ch );

    return;
}



void do_ofind( CHAR_DATA *ch, char *argument )
{
    extern int top_obj_index;
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    OBJ_INDEX_DATA *pObjIndex;
    int vnum;
    int nMatch;
    bool fAll;
    bool found;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Ofind what?\n\r", ch );
	return;
    }

    fAll	= !str_cmp( arg, "all" );
    found	= FALSE;
    nMatch	= 0;

    /*
     * Yeah, so iterating over all vnum's takes 10,000 loops.
     * Get_obj_index is fast, and I don't feel like threading another link.
     * Do you?
     * -- Furey
     */
    for ( vnum = 0; nMatch < top_obj_index; vnum++ )
    {
	if ( ( pObjIndex = get_obj_index( vnum ) ) != NULL )
	{
	    nMatch++;
	    if ( fAll || is_name( arg, pObjIndex->name ) )
	    {
		found = TRUE;
		sprintf( buf, "[%5d] %4d/%3d %s\n\r",
		    pObjIndex->vnum,
		    pObjIndex->number,
		    pObjIndex->limit,
		    pObjIndex->short_descr );
		send_to_char( buf, ch );
	    }
	}
    }

    if ( !found )
	send_to_char( "Nothing like that in hell, earth, or heaven.\n\r", ch );

    return;
}



void do_mwhere( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    bool found;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Mwhere whom?\n\r", ch );
	return;
    }

    found = FALSE;
    for ( victim = char_list; victim != NULL; victim = victim->next )
    {
	if ( IS_NPC(victim)
	&&   victim->in_room != NULL
	&&  ( is_name( arg, victim->name )
	    ||   is_name( arg, victim->namelist ) ) )
	{
	    found = TRUE;
	    sprintf( buf, "[%5d] %-28s [%5d] %s\n\r",
		victim->pIndexData->vnum,
		victim->short_descr,
		victim->in_room->vnum,
		victim->in_room->name );
	    send_to_char( buf, ch );
	}
    }

    if ( !found )
	act( "You didn't find any $t.", ch, NULL, NULL, arg, NULL, TO_CHAR, SENSE_SIXTH );

    return;
}



void do_reboo( CHAR_DATA *ch, char *argument )
{
    send_to_char( "If you want to REBOOT, spell it out.\n\r", ch );
    return;
}



void do_reboot( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    extern bool merc_down;

    write_environment( TRUE );
    do_force( ch, "all save" );

    sprintf( buf, "Reboot %s by %s.", argument, ch->name );
    do_echo( ch, buf );
    merc_down = TRUE;
    return;
}


void do_send( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    bool write_to_descriptor( int desc, char *txt, int length );

    argument = one_argument( argument, arg );

    if ( arg == NULL || argument == NULL
    || argument[0] == '\0' )
    {
	printf( "Usage:  send <player> <message>\n\r\n\r" );
	printf( "This command sends a message directly to the socket,\n\r" );
	printf( "and may be used on descriptors that are not yet connected.\n\r" );
	return;
    }

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d->character && d->character->name
	&& !str_cmp( arg, d->character->name ) )
	{
	    sprintf( buf, "%s\n\r", argument );
	    write_to_descriptor( d->descriptor, buf, 0 );
	    send_to_char( "Message sent.\n\r", ch );
	    return;
	}
    }

    send_to_char( "Could not find that player.\n\r", ch );
    return;
}



void do_shutdow( CHAR_DATA *ch, char *argument )
{
    send_to_char( "If you want to SHUTDOWN, spell it out.\n\r", ch );
    return;
}



void do_shutdown( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    extern bool merc_down;

    one_argument(argument, arg);

    if (!str_cmp(arg, "code") )
    {
        write_environment( TRUE );
        do_force( ch, "all save" );

        sprintf( buf, "Reboot for code by %s.", ch->name );
        append_file( ch, SHUTDOWN_FILE, buf );
        strcat( buf, "\n\r" );
        do_echo( ch, buf );
        merc_down = TRUE;
        return;
    }

    write_environment( TRUE );
    do_force( ch, "all save" );

    sprintf( buf, "Shutdown %s", argument );
    append_file( ch, SHUTDOWN_FILE, buf );
    append_stat( STAT_FILE, buf );
    strcat( buf, "\n\r" );
    do_echo( ch, buf );
    
    merc_down = TRUE;
    return;
}



void do_snoop( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    DESCRIPTOR_DATA *d;
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Snoop whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim->desc == NULL )
    {
	send_to_char( "No descriptor to snoop.\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "Cancelling all snoops.\n\r", ch );
	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    if ( d->snoop_by == ch->desc )
		d->snoop_by = NULL;
	}
	return;
    }

    if ( victim->desc->snoop_by != NULL )
    {
	send_to_char( "Busy already.\n\r", ch );
	return;
    }

    if ( get_trust( victim ) >= get_trust( ch ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if ( ch->desc != NULL )
    {
	for ( d = ch->desc->snoop_by; d != NULL; d = d->snoop_by )
	{
	    if ( d->character == victim || d->original == victim )
	    {
		send_to_char( "No snoop loops.\n\r", ch );
		return;
	    }
	}
    }

    victim->desc->snoop_by = ch->desc;
    send_to_char( "Ok.\n\r", ch );
    return;
}



void do_switch( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );
    
    if ( arg[0] == '\0' )
    {
	send_to_char( "Switch into whom?\n\r", ch );
	return;
    }

    if ( ch->desc == NULL )
	return;
    
    if ( ch->desc->original != NULL )
    {
	send_to_char( "You are already switched.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "Ok.\n\r", ch );
	return;
    }

    if ( victim->desc != NULL )
    {
	send_to_char( "Character in use.\n\r", ch );
	return;
    }

    if ( !IS_NPC( victim ) )
    {
	send_to_char("You can't switch into PCs.\n\r", ch );
	return;
    }

    ch->desc->character = victim;
    ch->desc->original  = ch;
    victim->desc        = ch->desc;
    ch->desc            = NULL;

    ch->switched_into	= victim;
    send_to_char( "Ok.\n\r", victim );
    return;
}



void do_return( CHAR_DATA *ch, char *argument )
{
    if ( ch->desc == NULL )
	return;

    if ( ch->desc->original == NULL )
    {
	send_to_char( "You aren't switched.\n\r", ch );
	return;
    }

    send_to_char( "You return to your original body.\n\r", ch );
    ch->desc->character       = ch->desc->original;
    ch->desc->original        = NULL;
    ch->desc->character->desc = ch->desc; 
    ch->desc                  = NULL;

    ch->switched_into	      = NULL;
    return;
}



void do_mload( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    MOB_INDEX_DATA *pMobIndex;
    CHAR_DATA *victim;
    
    one_argument( argument, arg );

    if ( arg[0] == '\0' || !is_number(arg) )
    {
	send_to_char( "Syntax: mload <vnum>.\n\r", ch );
	return;
    }

    if ( ( pMobIndex = get_mob_index( atoi( arg ) ) ) == NULL )
    {
	send_to_char( "No mob has that vnum.\n\r", ch );
	return;
    }

    victim = create_mobile( pMobIndex );
    char_to_room( victim, ch->in_room, TRUE );
    act( "$N arrives from nearby.", ch, victim, NULL, NULL, NULL, TO_ROOM, SENSE_SIGHT );
    send_to_char( "Ok.\n\r", ch );
    return;
}



void do_oload( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
 
    argument = one_argument( argument, arg1 );
 
    if ( arg1[0] == '\0' || !is_number( arg1 ) )
    {
        send_to_char( "Syntax: oload <vnum>.\n\r", ch );
        return;
    }
 
    if ( ( pObjIndex = get_obj_index( atoi( arg1 ) ) ) == NULL )
    {
	send_to_char( "No object has that vnum.\n\r", ch );
	return;
    }

    if ( pObjIndex->load_chance < ( MAX_LEVEL - get_trust( ch ) ) )
    {
	send_to_char("You can't load that.\n\r", ch );
	return;
    }

    obj = create_object( pObjIndex );
    if ( !IS_SET(obj->extra_flags, ITEM_NOTAKE) )
    obj_to_char( obj, ch );
    else
	obj_to_room( obj, ch->in_room, TRUE );
	act( "$n has created $p!", ch, NULL, obj, NULL, NULL, TO_ROOM, SENSE_SIGHT );
    

    send_to_char( "Ok.\n\r", ch );
    return;
}



void do_purge( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	/* 'purge' */
	CHAR_DATA *vnext;
	OBJ_DATA  *obj_next;

	for ( victim = ch->in_room->people; victim != NULL; victim = vnext )
	{
	    vnext = victim->next_in_room;
	    if ( IS_NPC(victim) && victim->desc == NULL )
		extract_char( victim, TRUE );
	}

	for ( obj = ch->in_room->contents; obj != NULL; obj = obj_next )
	{
	    obj_next = obj->next_content;
	    extract_obj( obj, TRUE );
	}

//	act( "$n purges the room!", ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIXTH );
	send_to_char( "Ok.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( !IS_NPC(victim) || victim->desc )
    {
	send_to_char( "Not on PC's.\n\r", ch );
	return;
    }

    act( "$n purges $N.", ch, victim, NULL, NULL, NULL, TO_NOTVICT, SENSE_SIGHT );
    extract_char( victim, TRUE );
    return;
}



void do_advance( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int level;
    int iLevel;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' || !is_number( arg2 ) )
    {
	send_to_char( "Syntax: advance <char> <level>.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
    {
	send_to_char( "That player is not here.\n\r", ch);
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if ( ( level = atoi( arg2 ) ) < 0 || level > 40 )
    {
	send_to_char( "Level must be 1 to 40.\n\r", ch );
	return;
    }

    if ( level > get_trust( ch ) )
    {
	send_to_char( "Limited to your trust level.\n\r", ch );
	return;
    }

    /*
     * Lower level:
     *   Reset to level 1.
     *   Then raise again.
     *   Currently, an imp can lower another imp.
     *   -- Swiftest
     */
    if ( level <= victim->level )
    {
//	int sn;

	send_to_char( "Lowering a player's level!\n\r", ch );
	send_to_char( "Your level has been dropped.\n\r", victim );
	victim->level    = 1;
/*	for ( sn = 0; sn < MAX_SKILL; sn++ )
	    victim->pcdata->learned[sn] = 0;*/
    }
    else
    {
	send_to_char( "Raising a player's level!\n\r", ch );
	send_to_char( "Your level has been raised.\n\r", victim );
        SET_BIT(victim->act, PLR_HOLYLIGHT);
        SET_BIT(victim->act, PLR_HOLYWALK);
        SET_BIT(victim->act, PLR_HOLYNAME);
        SET_BIT(victim->act, PLR_HOLYVNUM);
        SET_BIT(victim->act, PLR_HOLYSPEECH);
        SET_BIT(victim->act, PLR_WIZINVIS);
    }

    for ( iLevel = victim->level ; iLevel < level; iLevel++ )
    {
	victim->level += 1;
    }
    victim->trust = 0;
    return;
}



void do_trust( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int level;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' || !is_number( arg2 ) )
    {
	send_to_char( "Syntax: trust <char> <level>.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
    {
	send_to_char( "That player is not here.\n\r", ch);
	return;
    }

    if ( ( level = atoi( arg2 ) ) < 0 || level > 40 )
    {
	send_to_char( "Level must be 0 (reset) or 1 to 40.\n\r", ch );
	return;
    }

    if ( level > get_trust( ch ) )
    {
	send_to_char( "Limited to your trust.\n\r", ch );
	return;
    }

    victim->trust = level;
    return;
}



void do_restore( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Restore whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    victim->hit[HIT_LOC_BODY]  = victim->max_hit;
    victim->mana = victim->max_mana;
    victim->move = victim->max_move;
    victim->stun = victim->max_stun;
    update_pos( victim );
    act( "$n has restored you.", ch, victim, NULL, NULL, NULL, TO_VICT, SENSE_SIXTH );
    send_to_char( "Ok.\n\r", ch );
    return;
}



void do_freeze( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Freeze whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if ( get_trust( victim ) >= get_trust( ch ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if ( IS_SET(victim->act, PLR_FREEZE) )
    {
	REMOVE_BIT(victim->act, PLR_FREEZE);
	send_to_char( "You can play again.\n\r", victim );
	send_to_char( "FREEZE removed.\n\r", ch );
    }
    else
    {
	SET_BIT(victim->act, PLR_FREEZE);
	send_to_char( "You can't do ANYthing!\n\r", victim );
	send_to_char( "FREEZE set.\n\r", ch );
    }

    save_char_obj( victim );

    return;
}



void do_log( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Log whom?\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, "all" ) )
    {
	if ( fLogAll )
	{
	    fLogAll = FALSE;
	    send_to_char( "Log ALL off.\n\r", ch );
	}
	else
	{
	    fLogAll = TRUE;
	    send_to_char( "Log ALL on.\n\r", ch );
	}
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    /*
     * No level check, gods can log anyone.
     */
    if ( IS_SET(victim->act, PLR_LOG) )
    {
	REMOVE_BIT(victim->act, PLR_LOG);
	send_to_char( "LOG removed.\n\r", ch );
    }
    else
    {
	SET_BIT(victim->act, PLR_LOG);
	send_to_char( "LOG set.\n\r", ch );
    }

    return;
}



void do_noemote( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Noemote whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if ( get_trust( victim ) >= get_trust( ch ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if ( IS_SET(victim->act, PLR_NO_EMOTE) )
    {
	REMOVE_BIT(victim->act, PLR_NO_EMOTE);
	send_to_char( "You can emote again.\n\r", victim );
	send_to_char( "NO_EMOTE removed.\n\r", ch );
    }
    else
    {
	SET_BIT(victim->act, PLR_NO_EMOTE);
	send_to_char( "You can't emote!\n\r", victim );
	send_to_char( "NO_EMOTE set.\n\r", ch );
    }

    return;
}



void do_notell( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Nowish whom?", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if ( get_trust( victim ) >= get_trust( ch ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if ( IS_SET(victim->act, PLR_NO_TELL) )
    {
	REMOVE_BIT(victim->act, PLR_NO_TELL);
	send_to_char( "You can wish again.\n\r", victim );
	send_to_char( "NO_WISH removed.\n\r", ch );
    }
    else
    {
	SET_BIT(victim->act, PLR_NO_TELL);
	send_to_char( "You can't wish.\n\r", victim );
	send_to_char( "NO_WISH set.\n\r", ch );
    }

    return;
}



void do_peace( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *rch;

    for ( rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room )
    {
	if ( check_fighting( rch ) )
	    stop_fighting( rch, TRUE );

        /* remove hunting? */
    }

    send_to_char( "Ok.\n\r", ch );
    return;
}



BAN_DATA *		ban_free;
BAN_DATA *		ban_list;

void do_ban( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    BAN_DATA *pban;

    if ( IS_NPC(ch) )
	return;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	strcpy( buf, "Banned sites:\n\r" );
	for ( pban = ban_list; pban != NULL; pban = pban->next )
	{
	    strcat( buf, pban->name );
	    strcat( buf, "\n\r" );
	}
	send_to_char( buf, ch );
	return;
    }

    for ( pban = ban_list; pban != NULL; pban = pban->next )
    {
	if ( !str_cmp( arg, pban->name ) )
	{
	    send_to_char( "That site is already banned!\n\r", ch );
	    return;
	}
    }

    if ( ban_free == NULL )
    {
	pban		= alloc_perm( sizeof(*pban) );
    }
    else
    {
	pban		= ban_free;
	ban_free	= ban_free->next;
    }

    pban->name	= str_dup( arg );
    pban->next	= ban_list;
    ban_list	= pban;
    send_to_char( "Ok.\n\r", ch );
    return;
}



void do_allow( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    BAN_DATA *prev;
    BAN_DATA *curr;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Remove which site from the ban list?\n\r", ch );
	return;
    }

    prev = NULL;
    for ( curr = ban_list; curr != NULL; prev = curr, curr = curr->next )
    {
	if ( !str_cmp( arg, curr->name ) )
	{
	    if ( prev == NULL )
		ban_list   = ban_list->next;
	    else
		prev->next = curr->next;

	    free_string( curr->name, MEM_GENERAL );
	    curr->next	= ban_free;
	    ban_free	= curr;
	    send_to_char( "Ok.\n\r", ch );
	    return;
	}
    }

    send_to_char( "Site is not banned.\n\r", ch );
    return;
}

void do_downtime( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    extern bool downtime;
    downtime = !downtime;

    if ( downtime )
    {
        send_to_char( "Downtime set.\n\r",ch);
        sprintf( buf, "rm -f %sstatus.txt", AREA_DIR  );
    system( buf );
    sprintf( buf, "touch %sstatus.txt", AREA_DIR );
    system( buf );
    append_stat(  STAT_FILE, "Sunday Downtime" );
    sprintf( buf, "cp -fr %sstatus.txt /usr/data/httpd/users/darksun/status.txt", AREA_DIR );
    system( buf );

    return;
    }
    else
    {
        send_to_char( "Downtime un-set.\n\r",ch);
          sprintf( buf, "rm -f %sstatus.txt", AREA_DIR  );
    system( buf );
    sprintf( buf, "touch %sstatus.txt", AREA_DIR );
    system( buf );
    append_stat(  STAT_FILE, "Normal" );
    sprintf( buf, "cp -fr %sstatus.txt /usr/data/httpd/users/darksun/status.txt", AREA_DIR );
    system( buf );

    return;
    }

    return;
}

void do_wizlock( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    extern bool wizlock;
    wizlock = !wizlock;

    


    if ( wizlock )
    {
	send_to_char( "Game wizlocked.\n\r", ch );
       sprintf( buf, "rm -f %sstatus.txt", AREA_DIR  );
    system( buf );
    sprintf( buf, "touch %sstatus.txt", AREA_DIR );
    system( buf );

    append_stat(  STAT_FILE, "Wizlocked" );

    sprintf( buf, "cp -fr %sstatus.txt /usr/data/httpd/users/darksun/status.txt", AREA_DIR );
    system( buf );

    return;
    }
    else
    {
	send_to_char( "Game un-wizlocked.\n\r", ch );
        sprintf( buf, "rm -f %sstatus.txt", AREA_DIR  );
    system( buf );
    sprintf( buf, "touch %sstatus.txt", AREA_DIR );
    system( buf );

    append_stat(  STAT_FILE, "Normal" );

    sprintf( buf, "cp -fr %sstatus.txt /usr/data/httpd/users/darksun/status.txt", AREA_DIR );
    system( buf );

    return;
    }

    return;
}



void do_slookup( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    int sn;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Slookup what?\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, "all" ) )
    {
	for ( sn = 0; sn < MAX_SKILL; sn++ )
	{
	    if ( skill_table[sn].name == NULL )
		break;
	    sprintf( buf, "Sn: %4d Slot: %4d Skill/spell: '%s'\n\r",
		sn, skill_table[sn].slot, skill_table[sn].name );
	    send_to_char( buf, ch );
	}
	}
    else
    {
	if ( ( sn = skill_lookup( arg ) ) < 0 )
	{
	    send_to_char( "No such skill or spell.\n\r", ch );
	    return;
	}

	sprintf( buf, "Sn: %4d Slot: %4d Skill/spell: '%s'\n\r",
	    sn, skill_table[sn].slot, skill_table[sn].name );
	send_to_char( buf, ch );
    }

    return;
}

void do_punload( CHAR_DATA *ch, char *argument )
{

	char arg[MAX_INPUT_LENGTH];
        CHAR_DATA *victim;
        DESCRIPTOR_DATA *d;

	one_argument( argument, arg );

	if (arg[0] == '\0' )
	{
		send_to_char("Punload whom?\n\r",ch);
		return;
	}

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	    send_to_char( "They aren't here.\n\r", ch );
	    return;
    }

    if ( IS_NPC(victim) )
    {
		send_to_char("Not on NPCs\n\r",ch);
		return;
	}
    
    send_to_char( "Thank you For playing, You will now be disconnected.\n\r",victim);
    send_to_char( "Disconnected.\n\r",ch);
    save_char_load( victim );
    d = victim->desc;
    extract_char( victim, TRUE );
    if ( d != NULL )
    close_socket( d );

    return;
}


void do_sset( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
/*    char arg4 [MAX_INPUT_LENGTH]; */
    char arg5 [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int value;
/*    int value2; */
    int value3;
    int sn;
    bool fAll;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    argument = one_argument( argument, arg3 );
/* arg4 was removed */
    argument = one_argument( argument, arg5 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0'
	/* || arg4[0] == '\0' */ || arg5[0] == '\0' )
    {
	send_to_char( "Syntax: sset <victim> <skill> <value> <power>\n\r",	ch );
	send_to_char( "or:     sset <victim> all     <value> <power>\n\r",	ch );
	send_to_char( "Skill being any skill or spell.\n\r",		ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    fAll = !str_cmp( arg2, "all" );
    sn   = 0;
    if ( !fAll && ( sn = skill_lookup( arg2 ) ) < 0 )
    {
	send_to_char( "No such skill or spell.\n\r", ch );
	return;
    }

    /*
     * Snarf the value.
     */
    if ( !is_number( arg3 ) /* || !is_number( arg4 ) */ || !is_number( arg5 ) )
    {
	send_to_char( "Value must be numeric.\n\r", ch );
	return;
    }

    value = atoi( arg3 );
    if ( value < 0 || value > 100 )
    {
	send_to_char( "Value range is 0 to 100.\n\r", ch );
	return;
    }

/*
    value2 = atoi( arg4 );
    if ( value2 < 0 || value2 > 500 )
    {
	send_to_char( "Mana range is 0 to 500.\n\r", ch );
	return;
    }
*/
    value3 = atoi( arg5 );
    if ( value3 < 0 || value3 > 250 )
    {
	send_to_char( "Power range is 0 to 250.\n\r", ch );
	return;
    }

    if ( fAll )
    {
	for ( sn = 0; sn < MAX_SKILL; sn++ )
	{
	    if ( skill_table[sn].name != NULL )
	    {
		victim->pcdata->learned[sn]	= value;
		victim->pcdata->power_level[sn]	= value3;
	    }
	}
    }
    else
    {
	victim->pcdata->learned[sn] = value;
	victim->pcdata->power_level[sn] = value3;
    }

    return;
}

void do_load( CHAR_DATA *ch, char *argument )
{
    int amount;
    int coin;
    char arg[MAX_INPUT_LENGTH];
 

    

    argument = one_argument( argument, arg );
    coin = get_coin_index( argument );
    amount = is_number( arg ) ? atoi( arg ) : -1;

    if ( arg[0] == '\0' || argument[0] == '\0')
    {
        
         send_to_char("Syntax: load <amount> <cointype>\n\r",ch);
         send_to_char("Current types are:\n\r
         Silvers, Gold Coins, Ceramic Coins\n\r",ch);
         return;
    }

   if ( coin == -1 )
        {
            send_to_char( "What type of coin?\n\r", ch );
            send_to_char( "Syntax: load <amount> <cointype>\n\r",ch );
            return;
        }

    
 
    if ( amount <= 0 )
    {
        send_to_char("Amount must be greater than 0.\n\r",ch);
        return;
    }

    send_to_char("Done.\n\r",ch);
    obj_to_char( create_money( amount, coin ), ch);

}

    
    

void do_mset( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    DESCRIPTOR_DATA *d;
    char *pwdnew;
    int value;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    strcpy( arg3, argument );

    if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
    {
	send_to_char( "Syntax: mset <victim> <field>  <value>\n\r",	ch );
	send_to_char( "or:     mset <victim> <string> <value>\n\r",	ch );
	send_to_char( "\n\r",						ch );
	send_to_char( "Field being one of:\n\r",			ch );
	send_to_char( "  str int dex tou off def sex class level\n\r",	ch );
	send_to_char( "  hp mana stun move align aggr\n\r",			ch );
	send_to_char( "  thirst drunk full race speed clan",		ch );
	send_to_char( "  height weight",				ch );
	send_to_char( "\n\r",						ch );
	send_to_char( "String being one of:\n\r",			ch );
	send_to_char( "  name namelist short long description spec\n\r",ch );
	send_to_char( "  dynamic email password dead \n\r",                                  ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    /*
     * Snarf the value (which need not be numeric).
     */
    value = is_number( arg3 ) ? atoi( arg3 ) : -1;

    /*
     * Set something.
     */
    if ( !str_cmp( arg2, "str" ) )
    {
	if ( value < 3 || value > 200 )
	{
	    send_to_char( "Strength range is 3 to 200.\n\r", ch );
	    return;
	}

	victim->perm_stat_phys_str = value;
	return;
   }
    if ( !str_cmp( arg2, "int" ) )
    {
	if ( value < 3 || value > 200 )
	{
	    send_to_char( "Intelligence range is 3 to 200.\n\r", ch );
	    return;
	}

	victim->perm_stat_ment_int = value;
	return;
    }

    if ( !str_cmp( arg2, "dex" ) )
    {
	if ( value < 3 || value > 200 )
	{
	    send_to_char( "Dexterity range is 3 to 200.\n\r", ch );
	    return;
	}

	victim->perm_stat_phys_dex = value;
	return;
    }

    if ( !str_cmp( arg2, "qui" ) )
    {
	if ( value < 3 || value > 200 )
	{
	    send_to_char( "Quickness range is 3 to 200.\n\r", ch );
	    return;
	}

	victim->perm_stat_phys_qui = value;
	return;
    }

    if ( !str_cmp( arg2, "tou" ) )
    {
	if ( value < 3 || value > 200 )
	{
	    send_to_char( "Toughness range is 3 to 200.\n\r", ch );
	    return;
	}

	victim->perm_stat_phys_tou = value;
	return;
    }

    if ( !str_cmp( arg2, "height" ) )
    {
	victim->height = value;
	return;
    }

    if ( !str_cmp( arg2, "weight" ) )
    {
	victim->weight = value;
	return;
    }

    if ( !str_cmp( arg2, "off" ) )
    {
	if ( value < 0 || value > 150 )
	{
	    send_to_char( "Offense range is 0 to 150.\n\r", ch );
	    return;
	}

	victim->perm_offense = value;
	return;
    }

    if ( !str_cmp( arg2, "def" ) )
    {
	if ( value < 0 || value > 150 )
	{
	    send_to_char( "Defense range is 0 to 150.\n\r", ch );
	    return;
	}

	victim->perm_defense = value;
	return;
    }

    if ( !str_cmp( arg2, "race" ) )
    {
	if ( value < 0 || value > MAX_RACE )
	{
	    send_to_char( "Race range is 0 to 5.\n\r", ch );
	    return;
	}

	victim->race = value;
	return;
    }

    if ( !str_cmp( arg2, "speed" ) )
    {
	if ( value < 0 || (value > 128 && get_trust(ch) < 38) )
	{
	    send_to_char( "Speed range is 0 to 128.\n\r", ch );
	    return;
	}

	victim->speed = value;
	return;
    }

    if ( !str_cmp( arg2, "clan" ) )
    {
	if ( value < 0 || value > MAX_CLAN )
	{
	    send_to_char( "Clan range is 0 to 10.\n\r", ch );
	    return;
	}

	if ( IS_NPC( victim ) )
	{
	    victim->clan = p2( value-1 );
	}
	else
	{
	  if ( value > 0 )
	  {
	    if ( IS_SET( p2( value-1 ), victim->clan ) )
		REMOVE_BIT( victim->clan, p2( value-1 ) );
	    else
		SET_BIT( victim->clan, p2( value-1 ) );
	  }
	  else
		victim->clan = 0;
	}
	return;
    }

    if ( !str_cmp( arg2, "sex" ) )
    {
	if ( value < 0 || value > 2 )
	{
	    send_to_char( "Sex range is 0 to 2.\n\r", ch );
	    return;
	}
	victim->sex = value;
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
	victim->class = value;
	return;
    }

    if ( !str_cmp( arg2, "level" ) )
    {
	if (!IS_NPC(victim))
	{
	    send_to_char( "Not on PC's.\n\r", ch );
	    return;
	}

	if ( value < 0 || value > 50 )
	{
	    send_to_char( "Level range is 0 to 50.\n\r", ch );
	    return;
	}
	victim->level = value;
	return;
    }

    if ( !str_cmp( arg2, "hp" ) )
    {
	if ( value < -10 || value > 30000 )
	{
	    send_to_char( "Hp range is -10 to 30,000 hit points.\n\r", ch );
	    return;
	}
	victim->max_hit = value;
	return;
    }

    if ( !str_cmp( arg2, "mana" ) )
    {
	if ( value < 0 || value > 30000 )
	{
	    send_to_char( "Mana range is 0 to 30,000 mana points.\n\r", ch );
	    return;
	}
	victim->max_mana = value;
	return;
    }

     if ( !str_cmp( arg2, "stun" ) )
    {
	if ( value < 0 || value > 30000 )
	{
	    send_to_char( "Stun range is 0 to 30,000 stun points.\n\r", ch );
	    return;
	}
	victim->max_stun = value;
	return;
    }

    if ( !str_cmp( arg2, "move" ) )
    {
	if ( value < 0 || value > 30000 )
	{
	    send_to_char( "Move range is 0 to 30,000 move points.\n\r", ch );
	    return;
	}
	victim->max_move = value;
	return;
    }

    if ( !str_cmp( arg2, "aggr" ) )
    {
	if ( value < -1 )
	{
	    send_to_char( "Aggression must be positive.\n\r", ch );
	    return;
	}
	victim->aggression = value;
	return;
    }

    if ( !str_cmp( arg2, "align" ) )
    {
	if ( value < -1000 || value > 1000 )
	{
	    send_to_char( "Alignment range is -1000 to 1000.\n\r", ch );
	    return;
	}
	victim->alignment = value;
	return;
    }

    if ( !str_cmp( arg2, "thirst" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

	if ( value < 0 || value > 100 )
	{
	    send_to_char( "Thirst range is 0 to 100.\n\r", ch );
	    return;
	}

	victim->pcdata->condition[COND_THIRST] = value;
	return;
    }

    if ( !str_cmp( arg2, "drunk" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

	if ( value < 0 || value > 100 )
	{
	    send_to_char( "Drunk range is 0 to 100.\n\r", ch );
	    return;
	}

	victim->pcdata->condition[COND_DRUNK] = value;
	return;
    }

    if ( !str_cmp( arg2, "full" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

	if ( value < 0 || value > 100 )
	{
	    send_to_char( "Full range is 0 to 100.\n\r", ch );
	    return;
	}

	victim->pcdata->condition[COND_FULL] = value;
	return;
    }

    if ( !str_cmp( arg2, "name" ) )
    {
	if ( IS_NPC( victim )
	&& victim->name != victim->pIndexData->name )
	    free_string( victim->name, MEM_MOB_NAME );
	victim->name = str_dup( arg3 );
	return;
    }


    if ( !str_cmp( arg2, "namelist" ) )
    {
	if ( IS_NPC( victim )
	&& victim->namelist != victim->pIndexData->namelist )
	    free_string( victim->namelist, MEM_MOB_NAMELIST );
	victim->namelist = str_dup( arg3 );
	return;
    }

    if ( !str_cmp( arg2, "short" ) )
    {
	if ( IS_NPC( victim ) && victim->short_descr != victim->pIndexData->short_descr )
	    free_string( victim->short_descr, MEM_MOB_SHORT_DESCR );
	victim->short_descr = str_dup( arg3 );
	return;
    }
   if ( !str_cmp( arg2, "dead" ) )
   {
        if ( IS_NPC(victim) )
        send_to_char("Not on NPCs\n\r",ch);

        if ( !str_cmp( arg3, "dead" ) )
		{
			if ( IS_SET( victim->act, PLR_DEAD) )
			{
            send_to_char("They are already dead. Try using Alive\n\r",ch);
			return;
			}

            stop_fighting( victim, TRUE );
            SET_BIT(victim->act, PLR_DEAD);
            send_to_char("DEAD bit set.\n\r",ch);
            send_to_char("The gods strike you dead!\n\r",victim );
            save_char_obj( victim );
            d = victim->desc;
            extract_char( victim, TRUE );
            if ( d != NULL )
            close_socket( d );
            
            return;
        }


   if ( !str_cmp( arg3, "alive" ) )
   {
			if ( !IS_SET( victim->act, PLR_DEAD) )
			{
            send_to_char("They are already alive. Try using Dead.\n\r",ch);
			return;
			}

            stop_fighting( victim, TRUE );
            REMOVE_BIT(victim->act, PLR_DEAD);
            send_to_char("DEAD bit removied.\n\r",ch);
            send_to_char("The gods grant you back your life.\n\r",victim );
            
            return;
        }
   send_to_char( "Dead or alive?\n\r",ch);
   return;
   }
            

   if ( !str_cmp( arg2, "email" ) )
    {

        if ( IS_NPC(victim) )
        {
            send_to_char("Not on Npc's\n\r",ch);
            return;
        }
        
        victim->pcdata->email_address = str_dup( arg3 );
        return;
    }

    if ( !str_cmp( arg2, "climbing" ) )
    {
        if ( !IS_SET(victim->act, PLR_CLIMBING) )
        {
            SET_BIT(victim->act, PLR_CLIMBING);
            send_to_char("Climbing bit SET\n\r",ch);
            return;
        }
    
    REMOVE_BIT(victim->act, PLR_CLIMBING);
    send_to_char("Climbing bit REMOVED\n\r",ch);
    return;
    }
          
  

    if ( !str_cmp( arg2, "password" ) )
    {
        if ( get_trust( ch ) < MAX_LEVEL )
        {
            send_to_char("You must be level 40 to use this command.\n\r",ch);
            return;
        }


        if (strlen(arg3) < 5 )
        {
            send_to_char("Password must be 5+ Characters.\n\r",ch);
            return;
        }
        pwdnew = crypt( arg3, victim->name );
        victim->pcdata->pwd = str_dup( pwdnew );
        send_to_char("Password reset, forcing save...\n\r",ch);
        save_char_load(victim); // In case you load them.
        send_to_char("Your password has been reset.\n\r",victim);
        send_to_char("Done.\n\r",ch);
        sprintf( log_buf, "Log: %s changed %s's password.",ch->name,victim->name );
        log_string( log_buf );
        return;
    }
        


		

    if ( !str_cmp( arg2, "long" ) )
    {
	char buf[MAX_STRING_LENGTH];

	if ( IS_NPC( victim ) )
	{
	  if ( victim->long_descr != victim->pIndexData->long_descr )
	    free_string( victim->long_descr, MEM_MOB_LONG_DESCR );
        }
	else
	  free_string( victim->long_descr, MEM_GENERAL );

	sprintf( buf, "%s\n\r", arg3 );
	victim->long_descr = str_dup( buf );
	return;
    }



    if ( !str_cmp( arg2, "dynamic" ) )
    {
	char buf[MAX_STRING_LENGTH];

        clear_dynamic( victim );

	if ( !str_cmp( arg3, "clear" ) )
	{
	  send_to_char( "Target's dynamic desc cleared.\n\r", ch );
	  return;
        }

	sprintf( buf, "%s\n\r", arg3 );
	victim->dynamic_descr = str_dup( buf );
	return;
    }

    if ( !str_cmp( arg2, "spec" ) )
    {
	if ( !IS_NPC(victim) )
	{
	    send_to_char( "Not on PC's.\n\r", ch );
	    return;
	}

	free_string( victim->spec_fun_name, MEM_MOB_SPEC_FUN_NAME );

	if ( ( victim->spec_fun = spec_lookup( arg3 ) ) == 0 )
	{
	    send_to_char( "No such spec fun.\n\r", ch );
	    return;
	}

	victim->spec_fun_name = str_dup( arg3 );

	return;
    }

    /*
     * Generate usage message.
     */
    do_mset( ch, "" );
    return;

    if (!str_cmp( arg2, "password" ) )
    {
        sprintf( log_buf, "%s changed %s's password.",ch->name,victim->name );
        log_string( log_buf );
        return;
    }

    sprintf( log_buf, "%s: %s %s %s",ch->name,arg1,arg2,arg3 );
    log_string( log_buf );
    return;
}



void do_oset( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int value;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    strcpy( arg3, argument );

    if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
    {
	send_to_char( "Syntax: oset <object> <field>  <value>\n\r",	ch );
	send_to_char( "or:     oset <object> <string> <value>\n\r",	ch );
	send_to_char( "\n\r",						ch );
	send_to_char( "Field being one of:\n\r",			ch );
	send_to_char( "  value0 value1 value2 value3\n\r",		ch );
	send_to_char( "  extra wear weight size cost timer\n\r",		ch );
	send_to_char( "  d0 d1 toughness attack material color\n\r",	ch );
	send_to_char( "  spec capacity\n\r",				ch );
	send_to_char( "\n\r",						ch );
	send_to_char( "String being one of:\n\r",			ch );
	send_to_char( "  name short long ed\n\r",			ch );
	return;
    }

    if ( ( obj = get_obj_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "Nothing like that in hell, earth, or heaven.\n\r", ch );
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
	obj->dam[0] = UMAX( 1, value );
	return;
    }

    if ( !str_cmp( arg2, "damage1" ) || !str_cmp( arg2, "d1" ) )
    {
	obj->dam[1] = UMAX( 1, value );
	return;
    }

    if ( !str_cmp( arg2, "toughness" ) )
    {
	obj->toughness = value;
	return;
    }

    if ( !str_cmp( arg2, "attack" ) )
    {
	obj->attack_type = value;
	return;
    }

    if ( !str_cmp( arg2, "color" ) )
    {
	obj->color = value;
	return;
    }

    if ( !str_cmp( arg2, "value0" ) || !str_cmp( arg2, "v0" ) )
    {
	obj->value[0] = value;
	return;
    }

    if ( !str_cmp( arg2, "value1" ) || !str_cmp( arg2, "v1" ) )
    {
	obj->value[1] = value;
	return;
    }

    if ( !str_cmp( arg2, "value2" ) || !str_cmp( arg2, "v2" ) )
    {
	obj->value[2] = value;
	return;
    }

    if ( !str_cmp( arg2, "value3" ) || !str_cmp( arg2, "v3" ) )
    {
	obj->value[3] = value;
	return;
    }

    if ( !str_cmp( arg2, "extra" ) )
    {
	obj->extra_flags = value;
	return;
    }

    if ( !str_cmp( arg2, "wear" ) )
    {
	obj->wear_flags = value;
	return;
    }

    if ( !str_cmp( arg2, "capacity" ) )
    {
	send_to_char("Capacity set.  Don't forget is in units of .1 pounds.\n\r", ch );
	obj->capacity = value;
	return;
    }

    if ( !str_cmp( arg2, "weight" ) )
    {
	send_to_char("Weight set.  Don't forget is in units of .1 pounds.\n\r", ch );
	obj->weight = value;
	return;
    }

    if ( !str_cmp( arg2, "size" ) )
    {
	send_to_char("Size set.\n\r", ch );
	obj->size = value;
	return;
    }

    if ( !str_cmp( arg2, "cost" ) )
    {
	obj->cost = value;
	return;
    }

    if ( !str_cmp( arg2, "timer" ) )
    {
	obj->timer = value;
	return;
    }

    if ( !str_cmp( arg2, "material" ) || !str_cmp( arg2, "matl" ) )
    {
	obj->material = material_lookup( argument );
	return;
    }
	
    if ( !str_cmp( arg2, "name" ) )
    {
	if ( obj->name != obj->pIndexData->name )
	    free_string( obj->name, MEM_OBJ_NAME );
	obj->name = str_dup( arg3 );
	return;
    }

    if ( !str_cmp( arg2, "short" ) )
    {
	if ( obj->short_descr != obj->pIndexData->short_descr )
	    free_string( obj->short_descr, MEM_OBJ_SHORT_DESCR );
	obj->short_descr = str_dup( arg3 );
	return;
    }

    if ( !str_cmp( arg2, "long" ) )
    {
	if ( obj->look_descr != obj->pIndexData->look_descr )
	    free_string( obj->look_descr, MEM_OBJ_LOOK_DESCR );
	obj->look_descr = str_dup( arg3 );
	return;
    }

    if ( !str_cmp( arg2, "ed" ) )
    {
	EXTRA_DESCR_DATA *ed;

	argument = one_argument( argument, arg3 );
	if ( argument == NULL )
	{
	    send_to_char( "Syntax: oset <object> ed <keyword> <string>\n\r",
		ch );
	    return;
	}

	if ( extra_descr_free == NULL )
	{
	    ed			= alloc_perm( sizeof(*ed) );
	}
	else
	{
	    ed			= extra_descr_free;
	    extra_descr_free	= extra_descr_free->next;
	}

	ed->keyword		= str_dup( arg3     );
	ed->description		= str_dup( argument );
	ed->next		= obj->extra_descr;
	obj->extra_descr	= ed;
	return;
    }

    if ( !str_cmp( arg2, "spec" ) )
    {
	if ( !str_cmp( arg2, "clear" ) )
	{
	    obj->spec_fun = 0;
	    free_string( obj->spec_fun_name, MEM_OBJ_SPEC_FUN_NAME );
	    send_to_char("Special cleared.\n\r", ch );
	    return;
	}

	if ( ( obj->spec_fun = obj_fun_lookup( arg3 ) ) == 0 )
	{
	    send_to_char("No such special.\n\r", ch );
	    return;
	}

	obj->spec_fun_name = str_dup( arg3 );

	send_to_char( "Special set.\n\r", ch );
	return;
    }

    /*
     * Generate usage message.
     */
    do_oset( ch, "" );
    return;
}



void do_rset( CHAR_DATA *ch, char *argument )
{
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA *location;
    int value;

    if ( IS_NPC( ch ) )
	return;

    if ( !can_build( ch, ch->in_room->area ) )
    {
	send_to_char("You do not have authorization to build in this zone.\n\r", ch );
	return;
    }

    argument = one_argument( argument, arg2 );
    strcpy( arg3, argument );

    if ( arg2[0] == '\0' || arg3[0] == '\0' )
    {
	send_to_char( "Syntax: rset <field> value\n\r",	ch );
	send_to_char( "\n\r",						ch );
	send_to_char( "Field being one of:\n\r",			ch );
	send_to_char( "  flags sector cost delay light\n\r",		ch );
	send_to_char( "  name  filter\n\r",				ch );
        send_to_char( "\n\rFor sectors, please see HELP SECTOR_TYPES\n\r", ch);
        send_to_char( "For room flags, see HELP ROOM_FLAGS\n\r",ch);
	return;
    }

    location = ch->in_room;

    /*
     * Set something.
     */
    if ( !str_cmp( arg2, "flags" ) )
    {
    if ( !is_number( arg3 ) )
    {
	send_to_char( "Value must be numeric.\n\r", ch );
	return;
    }
    value = atoi( arg3 );

	location->room_flags	= value;
	return;
    }

    if ( !str_cmp( arg2, "sector" ) )
    {
      struct sector_type *s;

      /* Need to eliminate quotes for 2-words sectors */
      one_argument( argument, arg3 );

      s = sector_lookup( arg3, NULL ); /*ch->in_room->area );*/
      if ( s == NULL )
      {
        send_to_char( "Sector type not found.\n\r", ch );
	return;
      }


      set_sector( location, s );
      return;
    }

    if ( !str_cmp( arg2, "cost" ) )
    {
    if ( !is_number( arg3 ) )
    {
	send_to_char( "Value must be numeric.\n\r", ch );
	return;
    }
    value = atoi( arg3 );

	location->move_cost	= value;
	return;
    }

    if ( !str_cmp( arg2, "delay" ) )
    {
    if ( !is_number( arg3 ) )
    {
	send_to_char( "Value must be numeric.\n\r", ch );
	return;
    }
    value = atoi( arg3 );

	location->move_delay	= value;
	return;
    }

    if ( !str_cmp( arg2, "light" ) )
    {
    if ( !is_number( arg3 ) )
    {
	send_to_char( "Value must be numeric.\n\r", ch );
	return;
    }
    value = atoi( arg3 );

	location->light	= value;
	return;
    }

    if ( !str_cmp( arg2, "filter" ) )
    {
    if ( !is_number( arg3 ) )
    {
	send_to_char( "Value must be numeric.\n\r", ch );
	return;
    }
    value = atoi( arg3 );

	location->light_filter	= value;
	return;
    }

    if ( !str_cmp( arg2, "size" ) )
    {
    if ( !is_number( arg3 ) )
    {
	send_to_char( "Value must be numeric.\n\r", ch );
	return;
    }
    value = atoi( arg3 );

	location->size	= value;
	return;
    }

    if ( !str_cmp( arg2, "name" ) )
    {
	free_string( location->name, MEM_ROOM_NAME );
	location->name = str_dup( arg3 );
	return;
    }

    if ( !str_cmp( arg2, "spec" ) )
    {
	if ( !str_cmp( arg2, "clear" ) )
	{
	    location->spec_fun = 0;
	    free_string( location->spec_fun_name, MEM_ROOM_SPECIAL_NAME );
	    send_to_char("Special cleared.\n\r", ch );
	    return;
	}

	if ( ( location->spec_fun = room_fun_lookup( arg3 ) ) == 0 )
	{
	    send_to_char("No such special.\n\r", ch );
	    return;
	}

	location->spec_fun_name = str_dup( arg3 );

	send_to_char( "Special set.\n\r", ch );
	return;
    }


    /*
     * Generate usage message.
     */
    do_rset( ch, "" );
    return;
}



void do_users( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
    int count;

    count	= 0;
    buf[0]	= '\0';
    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d->character != NULL && can_see( ch, d->character ) )
	{
	    count++;
	    sprintf( buf + strlen(buf), "[%3d %2d] %s@%s\n\r",
		d->descriptor,
		d->connected,
		d->original  ? d->original->name  :
		d->character ? d->character->name : "(none)",
		d->host
		);
	}
    }

    sprintf( buf2, "%d user%s\n\r", count, count == 1 ? "" : "s" );
    send_to_char( buf2, ch );
    send_to_char( buf, ch );
    return;
}


void do_viewskills( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    int sn;
    int col;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( " Syntax:  skills <player>.\n\r", ch );
	return;
    }

    if ( (victim = get_char_world( ch, argument ) ) == NULL )
    {
	send_to_char("No such person in the game.\n\r", ch );
	return;
    }

    col    = 0;

  if ( !IS_NPC( victim ) )
  {
    for ( sn = 0; sn < MAX_SKILL; sn++ )
    {
	if ( skill_table[sn].name == NULL )
	    break;
	if ( victim->pcdata->learned[sn] == 0
	&& victim->pcdata->power_level[sn] == 0 )
	    continue;

	    sprintf( buf, "%21s  %3d %3d  ", skill_table[sn].name,
		victim->pcdata->learned[sn],
		victim->pcdata->power_level[sn] );
	send_to_char( buf, ch );

	    if ( ++col % 2 == 0 )
		send_to_char( "\n\r", ch );
    }
  }
  else
  {
    if ( victim->pIndexData == NULL )
    {
	send_to_char("Mobile has null prototype.\n\r", ch );
	return;
    }

    for ( sn = 0; sn < MAX_SKILL; sn++ )
    {
	if ( skill_table[sn].name == NULL )
	    break;
	if ( victim->pIndexData->learned[sn] == 0
	&& victim->pIndexData->power_level[sn] == 0 )
	    continue;

	    sprintf( buf, "%21s  %3d %3d  ", skill_table[sn].name,
		victim->pIndexData->learned[sn],
		victim->pIndexData->power_level[sn] );
	send_to_char( buf, ch );

	    if ( ++col % 2 == 0 )
		send_to_char( "\n\r", ch );
    }
  }

	if ( col % 2 != 0 )
	    send_to_char( "\n\r", ch );

    view_skill_delays( ch, victim );

    return;
}



/*
 * Thanks to Grodyn for pointing out bugs in this function.
 */
void do_force( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    int was_afk = 0;

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
	send_to_char( "Force whom to do what?\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, "all" ) )
    {
	CHAR_DATA *vch;
	CHAR_DATA *vch_next;

	for ( vch = char_list; vch != NULL; vch = vch_next )
	{
	    was_afk = 0;
	    vch_next = vch->next;

	    if (!IS_NPC(vch) && get_trust(vch) < get_trust(ch)) {
		if (IS_SET(vch->act, PLR_AFK)) {
		    REMOVE_BIT(vch->act, PLR_AFK);
		    was_afk = 1;
		}

		act( "$n forces you to '$t'.", ch, vch, NULL, argument, 
		     NULL, TO_VICT, SENSE_SIXTH );
		interpret( vch, argument );
		
		if (was_afk) SET_BIT(vch->act, PLR_AFK);
	    }
	}
    } else {
	CHAR_DATA *victim;

	if ((victim = get_char_world(ch, arg)) == NULL) {
	    send_to_char( "They aren't here.\n\r", ch );
	    return;
	}

	if (!IS_NPC(victim) && IS_SET(victim->act, PLR_AFK)) {
	    was_afk = 1;
	    REMOVE_BIT(victim->act, PLR_AFK);
	}

	if (victim == ch) {
	    send_to_char( "Do it yourself!\n\r", ch );
	    return;
	}

        if ( get_trust(ch) <= 37 && !IS_NPC(victim) )
        {
            send_to_char("You do not have proper authority to do this.\n\r",ch);
            return;
        }

	if ( get_trust( victim ) >= get_trust( ch ) ) {
	    send_to_char( "Do it yourself!\n\r", ch );
	    return;
	}

	act( "$n forces you to '$t'.", ch, victim, NULL, argument, NULL, 
	     TO_VICT, SENSE_SIXTH );
	interpret( victim, argument );

	if (was_afk) SET_BIT(victim->act, PLR_AFK);

    }

    send_to_char( "Ok.\n\r", ch );
    return;
}



/*
 * New routines by Dionysos.
 */
void do_invis( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;

    if ( IS_SET(ch->act, PLR_WIZINVIS) )
    {
        if ( ch->level <= 37 ){
        send_to_char("You do not have authorization to become visible to mortals.\n\r",ch);
        return;
        }

	REMOVE_BIT(ch->act, PLR_WIZINVIS);
	act( "$n slowly fades into existence.", ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIXTH );
	send_to_char( "You slowly fade back into existence.\n\r", ch );
    }
    else
    {
	SET_BIT(ch->act, PLR_WIZINVIS);
	act( "$n slowly fades into thin air.", ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIXTH );
	send_to_char( "You slowly vanish into thin air.\n\r", ch );
    }
    

    return;
}


/**************************************************************************/
void do_holylight( CHAR_DATA *ch, char *argument )
{
    if (IS_UNSWITCHED_MOB(ch))
	return;

    if (IS_SET(ch->act, PLR_HOLYLIGHT)) {
        REMOVE_BIT(ch->act, PLR_HOLYLIGHT);
        send_to_char("Holylight mode off.\n\r", ch);
    } else {
        SET_BIT(ch->act, PLR_HOLYLIGHT);
        send_to_char("Holylight mode on.\n\r", ch);
    }
    return;
}

void do_holyspeech( CHAR_DATA *ch, char *argument )
{
    if(IS_UNSWITCHED_MOB(ch))
	return;
    
    if (HAS_HOLYSPEECH(ch)) {
	REMOVE_BIT(ch->act, PLR_HOLYSPEECH);
        send_to_char("Holyspeech mode off.\n\r", ch);

    } else {
	SET_BIT(ch->act, PLR_HOLYSPEECH);
        send_to_char("Holyspeech mode on.\n\r", ch);
    }
    return;
}

void do_holyname( CHAR_DATA *ch, char *argument )
{
    if(IS_UNSWITCHED_MOB(ch)) 
	return;
    
    if (IS_SET(ch->act, PLR_HOLYNAME)) {
        REMOVE_BIT(ch->act, PLR_HOLYNAME);
        send_to_char("Holyname mode off.\n\r", ch);

    } else {
        SET_BIT(ch->act, PLR_HOLYNAME);
        send_to_char("Holyname mode on.\n\r", ch);
    }
    return;
}

void do_holywalk( CHAR_DATA *ch, char *argument )
{
    if(IS_UNSWITCHED_MOB(ch)) 
	return;

    if (IS_SET(ch->act, PLR_HOLYWALK)) {
        REMOVE_BIT(ch->act, PLR_HOLYWALK);
        send_to_char("Holywalk mode off.\n\r", ch);

    } else {
        SET_BIT(ch->act, PLR_HOLYWALK);
        send_to_char("Holywalk mode on.\n\r", ch);
    }
    return;
}


void do_holyvnum( CHAR_DATA *ch, char *argument )
{
    if(IS_UNSWITCHED_MOB(ch)) 
	return;

    if (IS_SET(ch->act, PLR_HOLYVNUM)) {
        REMOVE_BIT(ch->act, PLR_HOLYVNUM);
        send_to_char("Holyvnum mode off.\n\r", ch);

    } else {
        SET_BIT(ch->act, PLR_HOLYVNUM);
        send_to_char("Holyvnum mode on.\n\r", ch);
    }
    return;
}

void do_holylist( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    send_to_char("  action     status\n\r", ch);
    send_to_char("---------------------\n\r", ch);
    sprintf(buf, "holylight      %s\n\r",
	    IS_SET(ch->act, PLR_HOLYLIGHT) ? "ON" : "OFF");
    send_to_char(buf,ch);
    sprintf(buf, "holyname       %s\n\r",
	    IS_SET(ch->act, PLR_HOLYNAME) ? "ON" : "OFF");
    send_to_char(buf,ch);
    sprintf(buf, "holyspeech     %s\n\r",
	    HAS_HOLYSPEECH(ch) ? "ON" : "OFF");
    send_to_char(buf,ch);
    sprintf(buf, "holywalk       %s\n\r",
	    IS_SET(ch->act, PLR_HOLYWALK) ? "ON" : "OFF");
    send_to_char(buf,ch);
    sprintf(buf, "holyvnum       %s\n\r",
	    IS_SET(ch->act, PLR_HOLYVNUM) ? "ON" : "OFF");
    send_to_char(buf,ch);
}
