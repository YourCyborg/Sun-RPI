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
 *    And Rip's Mods (c) 1994 by Robert L. Peckham....                     *
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
 * The following special functions are available for rooms.
 */
DECLARE_ROOM_SPECIAL(	spec_start_room		);
DECLARE_ROOM_SPECIAL(   spec_urik_gate          );


ROOM_SPECIAL *room_fun_lookup( const char *name )
{
    if ( !str_cmp( name, "spec_start_room"       ) ) return spec_start_room;
    

    if ( !str_cmp( name, "spec_urik_gate"        ) ) return spec_urik_gate;
    return 0;

}

bool spec_urik_gate( CHAR_DATA *ch, char *command, DO_FUN *fnptr,  char *argument, ROOM_INDEX_DATA *room )
{
    if ( !str_cmp( command, "open gate" ) )
    {
       if (!IS_IMMORTAL(ch) )
       {
           send_to_char("A human Urikite soldier blocks you from accessing the gate.\n\r", ch);
           return TRUE;
       }
        send_to_char("You call up to the tower.\n\r",ch);
       return TRUE;
    }
    return FALSE;
}



bool spec_start_room( CHAR_DATA *ch, char *command, DO_FUN *fnptr,
  char *argument, ROOM_INDEX_DATA *room )
{
    if (IS_NPC(ch))
	return FALSE;

    if ( !str_cmp( command, "say" ) 
       || !str_cmp( command, "shout")
       || !str_cmp( command, "tell" )
       || !str_cmp( command, "ooc"  ))
    {
        send_to_char("You can't do that here.\n\r",ch);
        return FALSE;
    }


    if ( !str_cmp( command, "city" ) )
    {
	if ( !str_cmp( argument, "urik" ) )
	{
	    send_to_char("You have chosen the city-state of Urik.\n\r", ch );
	    char_from_room( ch );
	    char_to_room( ch, get_room_index( ROOM_VNUM_TEMPLE ), FALSE );
	    do_look( ch, "auto" );
	}
	else if ( !str_cmp( argument, "tyr" ) )
	{
	    send_to_char("You have chosen the city-state of Tyr.\n\r", ch );
	    char_from_room( ch );
	    char_to_room( ch, get_room_index( 20001 ), FALSE );
	    do_look( ch, "auto" );
	}
	else if ( !str_cmp( argument, "draj" ) )
	{
	    send_to_char("You have chosen the city-state of Draj.\n\r", ch );
	    char_from_room( ch );
	    char_to_room( ch, get_room_index( 1042 ), FALSE );
            act( "$n walks in from the north.",ch,NULL,NULL,NULL,NULL,TO_ROOM,SENSE_SIXTH );
	    do_look( ch, "auto" );
	}
	else
	{
	    send_to_char("Usage:   city <city>\n\r", ch );
	    send_to_char("Where <city> is one of the following:\n\r", ch );
	    send_to_char("Urik\n\r", ch );
	    send_to_char("Draj\n\r", ch );
	    send_to_char("Tyr\n\r", ch );
	}

	return TRUE;
    }

    return FALSE;
}

