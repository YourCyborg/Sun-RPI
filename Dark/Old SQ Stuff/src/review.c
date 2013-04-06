/*
 *  review.c  Online character review functions.
 *
 * (C) 1997, Robert L. Peckham (Rip)
 */

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"

void do_rlist( CHAR_DATA *ch, char *arg )
{
    char buf[MAX_STRING_LENGTH];
    char *word;
    int i = 0;
    FILE *fp;

    system( "rm -f rlist.tmp" );
    sprintf( buf, "ls %s > rlist.tmp", UNAPPROVED_DIR );
    system( buf );
    system( "echo 'END' | cat >> rlist.tmp" );

    if ( ( fp = fopen( "rlist.tmp", "r" ) ) == NULL ) 
    {
	send_to_char("Unable to open list of unapproved characters.\n\r", ch );
	return;
    }

    while( (word = fread_word( fp )) != NULL && strcmp( word, "END" ) )
    {
	sprintf( buf, "%15s", word );
	send_to_char( buf, ch );

	if ( i++ == 3 )
	{
	    i = 0;
	    send_to_char( "\n\r", ch );
	}
    }

    if ( i != 3 )
	send_to_char( "\n\r", ch );

    fclose( fp );

    return;
}



/*
 * Loads a character for review.
 */
void do_rload( CHAR_DATA *ch, char *arg )
{
    char *name;
    CHAR_DATA *rch;
    DESCRIPTOR_DATA *d;
    char strsave[MAX_INPUT_LENGTH];
    FILE *fp;

    if ( arg == NULL || arg[0] == '\0' )
    {
	send_to_char("Syntax: rload <character>\n\r", ch );
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

    rch->desc = NULL;

    load_char_from_file( rch, fp );
    fclose( fp );

    char_to_room( rch, get_room_index( ROOM_VNUM_REVIEW_ROOM ), FALSE );

    SET_BIT( rch->act, PLR_REVIEWING );

    rch->next = char_list;
    char_list = rch;

    send_to_char( "Character loaded to review room.\n\r", ch );
    return;
}


void do_approve( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *ach;
    char buf[MAX_STRING_LENGTH];

    ach = get_char_world( ch, argument );
    if ( ach == NULL || IS_NPC( ach ) )
    {
	send_to_char( "No such character to approve.\n\r", ch );
	return;
    }

    if ( !IS_SET( ach->act, PLR_REVIEWING ) )
    {
	send_to_char( "That character is not being reviewed.\n\r", ch );
	return;
    }

    REMOVE_BIT( ach->act, PLR_REJECTED );
    REMOVE_BIT( ach->act, PLR_UNAPPROVED );

    sprintf( buf, "rm -f %s%s", REJECTED_DIR, capitalize( ach->name ) );
    system( buf );
    sprintf( buf, "rm -f %s%s", UNAPPROVED_DIR, capitalize( ach->name ) );
    system( buf );

    save_char_obj( ach );
    do_quit( ach, "" );

    send_to_char( "Character approved.\n\r", ch );
    return;
}

void do_reject( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *ach;
    char buf[MAX_STRING_LENGTH];

    ach = get_char_world( ch, argument );
    if ( ach == NULL || IS_NPC( ach ) )
    {
	send_to_char( "No such character to reject.\n\r", ch );
	return;
    }

    if ( !IS_SET( ach->act, PLR_REVIEWING ) )
    {
	send_to_char( "That character is not being reviewed.\n\r", ch );
	return;
    }

    SET_BIT( ach->act, PLR_REJECTED );
    REMOVE_BIT( ach->act, PLR_UNAPPROVED );

    sprintf( buf, "rm -f %s%s", PLAYER_DIR, capitalize( ach->name ) );
    system( buf );
    sprintf( buf, "rm -f %s%s", UNAPPROVED_DIR, capitalize( ach->name ) );
    system( buf );

    free_string( ach->pcdata->rejected, MEM_PCDATA );
    if ( ch->pnote && ch->pnote->text )
	ach->pcdata->rejected = str_dup( ch->pnote->text );
    else
	ach->pcdata->rejected = str_dup( "No specific reject message.\n\r" );

    save_char_obj( ach );
    do_quit( ach, "" );

    send_to_char( "Character rejected.\n\r", ch );
    return;
}
