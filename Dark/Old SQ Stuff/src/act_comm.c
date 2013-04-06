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
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "languages.h"



/*
 * Local functions.
 */
bool	is_note_to	args( ( CHAR_DATA *ch, NOTE_DATA *pnote ) );
void	note_attach	args( ( CHAR_DATA *ch ) );
void	note_remove	args( ( CHAR_DATA *ch, NOTE_DATA *pnote ) );
void	talk_channel	args( ( CHAR_DATA *ch, char *argument,
			    int channel, const char *verb ) );

char* ooc1;
char* ooc2;
char* ooc3;
char* ooc4;
char* ooc5;
char* ooc6;
char* ooc7;
char* ooc8;


bool is_note_to( CHAR_DATA *ch, NOTE_DATA *pnote )
{
    if ( !str_cmp( ch->name, pnote->sender ) )
	return TRUE;

    if ( is_name( "all", pnote->to_list ) )
	return TRUE;

    if ( IS_IMMORTAL(ch) && is_name( "immortal", pnote->to_list ) )
	return TRUE;

    if ( is_name( ch->name, pnote->to_list ) )
	return TRUE;

    if ( get_trust(ch) >= 40)
	return TRUE;

    return FALSE;
}



void note_attach( CHAR_DATA *ch )
{
    NOTE_DATA *pnote;

    if ( ch->pnote != NULL )
	return;

    if ( note_free == NULL )
    {
	pnote	  = alloc_perm( sizeof(*ch->pnote) );
    }
    else
    {
	pnote	  = note_free;
	note_free = note_free->next;
    }

    pnote->next		= NULL;
    pnote->sender	= str_dup( ch->name );
    pnote->date		= str_dup( "" );
    pnote->to_list	= str_dup( "" );
    pnote->subject	= str_dup( "" );
    pnote->text		= str_dup( "" );
    ch->pnote		= pnote;
    return;
}


void do_afk ( CHAR_DATA *ch, char *argument)
{
    // unswitched mobs can't be afk
    if (IS_UNSWITCHED_MOB(ch)) {
	send_to_char("No.\n\r", ch);
        return;
    }

    if (!IS_NULLSTR(argument)) {
    // so you can change your afk message without turning afk off
	REMOVE_BIT(ch->act, PLR_AFK);
    }

    if (IS_SET(ch->act, PLR_AFK)) {
       act("$n has returned from being AFK.",ch,NULL,NULL,NULL,NULL,TO_ROOM,SENSE_SIGHT);
       send_to_char("AFK mode removed.\n\r", ch);
       REMOVE_BIT(ch->act, PLR_AFK);
    } else {
	if(IS_NULLSTR(argument)) {
	    act("$n has gone AFK.",ch,NULL,NULL,NULL,NULL,TO_ROOM,SENSE_SIGHT);
	    send_to_char( "You are now in AFK mode.\n\r", ch);
	} else {
	    act("$n has gone AFK ($t).",ch,NULL,NULL,argument,NULL,TO_ROOM,SENSE_SIGHT);
	    send_to_char( "You are now in AFK mode.\n\r", ch);
	}
	SET_BIT(ch->act, PLR_AFK);
    }
}
	    

void note_remove( CHAR_DATA *ch, NOTE_DATA *pnote )
{
    char to_new[MAX_INPUT_LENGTH];
    char to_one[MAX_INPUT_LENGTH];
    FILE *fp;
    NOTE_DATA *prev;
    char *to_list;

    /*
     * Build a new to_list.
     * Strip out this recipient.
     */
    to_new[0]	= '\0';
    to_list	= pnote->to_list;
    while ( *to_list != '\0' )
    {
	to_list	= one_argument( to_list, to_one );
	if ( to_one[0] != '\0' && str_cmp( ch->name, to_one ) )
	{
	    strcat( to_new, " " );
	    strcat( to_new, to_one );
	}
    }

    /*
     * Just a simple recipient removal?
     */
    if ( str_cmp( ch->name, pnote->sender ) && to_new[0] != '\0' 
	&& get_trust(ch) < 40 )
    {
	free_string( pnote->to_list, MEM_GENERAL );
	pnote->to_list = str_dup( to_new + 1 );
	return;
    }

    /*
     * Remove note from linked list.
     */
    if ( pnote == note_list )
    {
	note_list = pnote->next;
    }
    else
    {
	for ( prev = note_list; prev != NULL; prev = prev->next )
	{
	    if ( prev->next == pnote )
		break;
	}

	if ( prev == NULL )
	{
	    bug( "Note_remove: pnote not found.", 0 );
	    return;
	}

	prev->next = pnote->next;
    }

    free_string( pnote->text, MEM_GENERAL );
    free_string( pnote->subject, MEM_GENERAL );
    free_string( pnote->to_list, MEM_GENERAL );
    free_string( pnote->date, MEM_GENERAL );
    free_string( pnote->sender, MEM_GENERAL );
    pnote->next	= note_free;
    note_free	= pnote;

    /*
     * Rewrite entire list.
     */
    fclose( fpReserve );
    if ( ( fp = fopen( NOTE_FILE, "w" ) ) == NULL )
    {
	perror( NOTE_FILE );
    }
    else
    {
	for ( pnote = note_list; pnote != NULL; pnote = pnote->next )
	{
	    fprintf( fp, "Sender  " );
	    write_string( fp, pnote->sender );
	    fprintf( fp, "~\nDate    " );
	    write_string( fp, pnote->date );
	    fprintf( fp, "~\nTo      " );
	    write_string( fp, pnote->to_list );
	    fprintf( fp, "~\nSubject " );
	    write_string( fp, pnote->subject );
	    fprintf( fp, "~\nText\n" );
	    write_string( fp, pnote->text );
	    fprintf( fp, "~\n\n" );
	}
	fclose( fp );
    }
    fpReserve = fopen( NULL_FILE, "r" );
    return;
}


void do_nmodify( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char *oldb;
    char arg[MAX_INPUT_LENGTH];
    char word[MAX_INPUT_LENGTH];
    int spaces = 0;
    int ii = 0;
    int inew;
    int iline;
    int iw;
    int l_count;

    if ( IS_NPC( ch ) )
	return;

    if ( ch->pnote == NULL || ch->pnote->text[0] == '\0' )
    {
	send_to_char("Nothing in your note buffer to modify.\n\r", ch );
	return;
    }

    oldb = ch->pnote->text;
    argument = one_argument( argument, arg );

    if ( !str_cmp( arg, "format" ) )
    {
	iline = atoi( argument );

	strcpy( buf, oldb );

	inew = 1;
      if ( iline > 1 )
	while ( 1 )
	{
	    ii++;

	    if ( oldb[ii] == '\0' )
		break;

	    if ( oldb[ii] != '\n' )
		continue;

	    ii+=2;
	    inew++;

	    if ( iline == inew )
		break;
	}

	if ( inew < iline )
	{
	    send_to_char("Not that many lines in buffer.\n\r", ch );
	    return;
	}

	buf[ii] = '\0';
	oldb = &oldb[ii];
	word[0] = '\0';
	l_count = 1;

	while( 1 )
	{
	    /* set word length to 0 */
	    iw = 0;

	    /* find beginning of next word */
	    while ( oldb[0] == ' ' || oldb[0] == '\n' || oldb[0] == '\r' )
	    {
		oldb++;
	    }

	    /* if last word, exit */
	    if ( oldb[0] == '\0' )
	    {
		break;
	    }

	    /* if it's a tab, stick in a tab */
	    if ( oldb[0] == '	' /* tab */ )
	    {
		if ( inew != iline )
		    strcat( buf, "\n\r" );
		strcat( buf, "	" );
		l_count = 8;
		oldb++;
		continue;
	    }

	    /* find end of next word */
	    while ( oldb[0] != ' ' && oldb[0] != '\n' )
	    {
		word[iw] = oldb[0];
		iw++;
		oldb++;
	    }

	    /* terminate word */
	    word[iw] = '\0';

	    /* exit if at end */
	    if ( oldb[0] == '\n' )
	    {
		oldb++;
		if ( oldb[0] == '\0' )
		    break;
	    }

	    /* if length is longer than line, new line, otherwise, add word */
	    if ( (l_count + iw) > 72 )
	    {
		inew++;
		strcat( buf, "\n\r" );
		strcat( buf, word );
		l_count = iw;
	    }
	    else
	    {
		if ( spaces > 0 )
		    strcat( buf, " ");
		if ( spaces > 1 )
		    strcat( buf, " ");
		strcat( buf, word );
		l_count += spaces + iw;
	    }

	    /* set # of spaces.  2 if '.', 1 otherwise */
	    if ( word[iw-1] == '.' )
		spaces = 2;
	    else
		spaces = 1;
	}

	strcat( buf, "\n\r\0" );
	free_string( ch->pnote->text, MEM_GENERAL );
	ch->pnote->text = str_dup( buf );
	send_to_char("Reformatted.\n\r", ch );
	send_to_char( buf, ch );
	return;
    }


    if ( !str_cmp( arg, "sentence" ) || !str_cmp( arg, "sent" ) )
    {
	iline = atoi( argument );

	strcpy( buf, oldb );

	inew = 1;
      if ( iline > 1 )
	while ( 1 )
	{
	    ii++;

	    if ( oldb[ii] == '\0' )
		break;

	    if ( oldb[ii] != '\n' )
		continue;

	    ii+=2;
	    inew++;

	    if ( iline == inew )
		break;
	}

	if ( inew < iline )
	{
	    send_to_char("Not that many lines in buffer.\n\r", ch );
	    return;
	}

	buf[ii] = '\0';
	oldb = &oldb[ii-1];
	ii--;

	while( 1 )
	{
	    oldb++;
	    ii++;

	    /* if last word, exit */
	    if ( oldb[0] == '\0' )
	    {
		buf[ii] = '\0';
		break;
	    }

	    /* dont transfer cr, lf characters */
	    if ( oldb[0] == '\n' || oldb[0] == '\r' )
	    {
		ii--;
		continue;
	    }

	    buf[ii] = oldb[0];
	    if ( oldb[0] == '.' )
	    {
		ii++;  buf[ii] = '\n';
		ii++;  buf[ii] = '\r';
	    }
	}

	free_string( ch->pnote->text, MEM_GENERAL );
	ch->pnote->text = str_dup( buf );
	send_to_char("Text broken into sentences..\n\r", ch );
	send_to_char( buf, ch );
	return;
    }


    if ( !str_cmp( arg, "delete" ) )
    {
	iline = atoi( argument );

	strcpy( buf, oldb );

	inew = 1;
      if ( iline > 1 )
	while ( 1 )
	{
	    ii++;

	    if ( oldb[ii] == '\0' )
		break;

	    if ( oldb[ii] != '\n' )
		continue;

	    ii+=2;
	    inew++;

	    if ( iline == inew )
		break;
	}

	if ( inew < iline || oldb[ii] == '\0' )
	{
	    send_to_char("Not that many lines in buffer.\n\r", ch );
	    return;
	}

	buf[ii] = '\0';
	oldb = &oldb[ii-1];

	/* find end of next line */
	while( oldb[0] != '\n' )
	{
	    oldb++;
	}

	/* skip \n and \r */
	strcat( buf, &oldb[2] );
	free_string( ch->pnote->text, MEM_GENERAL );
	ch->pnote->text = str_dup( buf );
	send_to_char("Line Deleted.\n\r", ch );
	send_to_char( buf, ch );
	return;
    }

    if ( !str_cmp( arg, "insert" ) )
    {
	char arg2[MAX_INPUT_LENGTH];

	argument = one_argument( argument, arg2 );

	iline = atoi( arg2 );

	strcpy( buf, oldb );

	iline += 1;

	inew = 1;
      if ( iline > 1 )
	while ( 1 )
	{
	    ii++;

	    if ( oldb[ii] == '\0' )
		break;

	    if ( oldb[ii] != '\n' )
		continue;

	    ii+=2;
	    inew++;

	    if ( iline == inew )
		break;
	}

	if ( inew < iline )
	{
	    send_to_char("Not that many lines in buffer.\n\r", ch );
	    return;
	}

	buf[ii] = '\0';
	oldb = &oldb[ii];

	strcat( buf, argument );
	strcat( buf, "\n\r" );

	strcat( buf, oldb );
	free_string( ch->pnote->text, MEM_GENERAL );
	ch->pnote->text = str_dup( buf );
	send_to_char("Line Inserted.\n\r", ch );
	send_to_char( buf, ch );
	return;
    }

    if ( !str_cmp( arg, "tab" ) )
    {
	iline = atoi( argument );

	strcpy( buf, oldb );

	inew = 1;
      if ( iline > 1 )
	while ( 1 )
	{
	    ii++;

	    if ( oldb[ii] == '\0' )
		break;

	    if ( oldb[ii] != '\n' )
		continue;

	    ii+=2;
	    inew++;

	    if ( iline == inew )
		break;
	}

	if ( iline < 1 )
	{
	    send_to_char("Must be positve integer.\n\r", ch );
	    return;
	}

	if ( inew < iline )
	{
	    send_to_char("Not that many lines in buffer.\n\r", ch );
	    return;
	}

	buf[ii] = '\0';
	oldb = &oldb[ii];

	strcat( buf, "	" /* tab */ );
	strcat( buf, oldb );

	free_string( ch->pnote->text, MEM_GENERAL );
	ch->pnote->text = str_dup( buf );
	send_to_char("Tab Added.\n\r", ch );
	send_to_char( buf, ch );
	return;
    }

    if ( !str_cmp( arg, "untab" ) )
    {
	iline = atoi( argument );

	strcpy( buf, oldb );

	inew = 1;
      if ( iline > 1 )
	while ( 1 )
	{
	    ii++;

	    if ( oldb[ii] == '\0' )
		break;

	    if ( oldb[ii] != '\n' )
		continue;

	    ii+=2;
	    inew++;

	    if ( iline == inew )
		break;
	}

	if ( iline < 1 )
	{
	    send_to_char("Must be positve integer.\n\r", ch );
	    return;
	}

	if ( inew < iline )
	{
	    send_to_char("Not that many lines in buffer.\n\r", ch );
	    return;
	}

	buf[ii] = '\0';
	oldb = &oldb[ii];

	if ( oldb[0] == '	' /* tab */ )
	{
	    strcat( buf, &oldb[1] );
	    free_string( ch->pnote->text, MEM_GENERAL );
	    ch->pnote->text = str_dup( buf );
	    send_to_char("Tab deleted.\n\r", ch );
	}
	else
	{
	    send_to_char("No tab on that line to remove.\n\r", ch );
	}

	send_to_char( ch->pnote->text, ch );
	return;

    }

    send_to_char("Usage: nmod <format/sent/tab/untab/insert/delete> <line> <text>\n\r", ch );
    return;
}


void do_note( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    NOTE_DATA *pnote;
    int vnum;
    int anum;
    int number, ii;

    if ( IS_NPC(ch) )
	return;

    argument = one_argument( argument, arg );

    if ( !str_cmp( arg, "list" ) )
    {
	vnum = 0;
	for ( pnote = note_list; pnote != NULL; pnote = pnote->next )
	{
	    if ( is_note_to( ch, pnote ) )
	    {
		sprintf( buf, "[%3d] %s: %s\n\r",
		    vnum, pnote->sender, pnote->subject );
		send_to_char( buf, ch );
		vnum++;
	    }
	}
	return;
    }

    if ( !str_cmp( arg, "read" ) )
    {
	bool fAll;

	if ( !str_cmp( argument, "all" ) )
	{
	    fAll = TRUE;
	    anum = 0;
	}
	else if ( is_number( argument ) )
	{
	    fAll = FALSE;
	    anum = atoi( argument );
	}
	else
	{
	    send_to_char( "Note read which number?\n\r", ch );
	    return;
	}

	vnum = 0;
	for ( pnote = note_list; pnote != NULL; pnote = pnote->next )
	{
	    if ( is_note_to( ch, pnote ) && ( vnum++ == anum || fAll ) )
	    {
		sprintf( buf, "[%3d] %s: %s\n\r%s\n\rTo: %s\n\r",
		    vnum - 1,
		    pnote->sender,
		    pnote->subject,
		    pnote->date,
		    pnote->to_list
		    );
		send_to_char( buf, ch );
		send_to_char( pnote->text, ch );
		return;
	    }
	}

	send_to_char( "No such note.\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, "+" ) )
    {
	note_attach( ch );
	strcpy( buf, ch->pnote->text );
	if ( strlen(buf) + strlen(argument) >= MAX_STRING_LENGTH - 4 )
	{
	    send_to_char( "Note too long.\n\r", ch );
	    return;
	}

	strcat( buf, argument );
	strcat( buf, "\n\r" );
	free_string( ch->pnote->text, MEM_GENERAL );
	ch->pnote->text = str_dup( buf );
	send_to_char( "Ok.\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, "subject" ) )
    {
	note_attach( ch );
	free_string( ch->pnote->subject, MEM_GENERAL );
	ch->pnote->subject = str_dup( argument );
	send_to_char( "Ok.\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, "to" ) )
    {
	note_attach( ch );
	free_string( ch->pnote->to_list, MEM_GENERAL );
	ch->pnote->to_list = str_dup( argument );
	send_to_char( "Ok.\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, "clear" ) )
    {
	if ( ch->pnote != NULL )
	{
	    free_string( ch->pnote->text, MEM_GENERAL );
	    free_string( ch->pnote->subject, MEM_GENERAL );
	    free_string( ch->pnote->to_list, MEM_GENERAL );
	    free_string( ch->pnote->date, MEM_GENERAL );
	    free_string( ch->pnote->sender, MEM_GENERAL );
	    ch->pnote->next	= note_free;
	    note_free		= ch->pnote;
	    ch->pnote		= NULL;
	}

	send_to_char( "Ok.\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, "show" ) )
    {
	if ( ch->pnote == NULL )
	{
	    send_to_char( "You have no note in progress.\n\r", ch );
	    return;
	}

        number = 1;
        ii = 1;
	sprintf( buf, "%s: %s\n\rTo: %s\n\r",
	    ch->pnote->sender,
	    ch->pnote->subject,
	    ch->pnote->to_list
	    );
	send_to_char( buf, ch );
        send_to_char( ch->pnote->text, ch );
   
	
	return;
    }

    if ( !str_cmp( arg, "post" ) )
    {
	FILE *fp;
	char *strtime;

	if ( ch->pnote == NULL )
	{
	    send_to_char( "You have no note in progress.\n\r", ch );
	    return;
	}

	ch->pnote->next			= NULL;
	strtime				= ctime( &current_time );
	strtime[strlen(strtime)-1]	= '\0';
	ch->pnote->date			= str_dup( strtime );

	if ( note_list == NULL )
	{
	    note_list	= ch->pnote;
	}
	else
	{
	    for ( pnote = note_list; pnote->next != NULL; pnote = pnote->next )
		;
	    pnote->next	= ch->pnote;
	}
	pnote		= ch->pnote;
	ch->pnote	= NULL;

	fclose( fpReserve );
	if ( ( fp = fopen( NOTE_FILE, "a" ) ) == NULL )
	{
	    perror( NOTE_FILE );
	}
	else
	{
	    fprintf( fp, "Sender  " );
	    write_string( fp, pnote->sender );
	    fprintf( fp, "~\nDate    " );
	    write_string( fp, pnote->date );
	    fprintf( fp, "~\nTo      " );
	    write_string( fp, pnote->to_list );
	    fprintf( fp, "~\nSubject " );
	    write_string( fp, pnote->subject );
	    fprintf( fp, "~\nText\n" );
	    write_string( fp, pnote->text );
	    fprintf( fp, "~\n\n" );
	    fclose( fp );
	}
	fpReserve = fopen( NULL_FILE, "r" );

	send_to_char( "Ok.\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, "remove" ) )
    {
	if ( !is_number( argument ) )
	{
	    send_to_char( "Note remove which number?\n\r", ch );
	    return;
	}

	anum = atoi( argument );
	vnum = 0;
	for ( pnote = note_list; pnote != NULL; pnote = pnote->next )
	{
	    if ( is_note_to( ch, pnote ) && vnum++ == anum )
	    {
		note_remove( ch, pnote );
		send_to_char( "Ok.\n\r", ch );
		return;
	    }
	}

	send_to_char( "No such note.\n\r", ch );
	return;
    }

    send_to_char( "Huh?  Type 'help note' for usage.\n\r", ch );
    return;
}



/*
 * Generic channel function.
 */
void talk_channel( CHAR_DATA *ch, char *argument, int channel, const char *verb )
{
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
    int position;

    if ( argument[0] == '\0' )
    {
	sprintf( buf, "%s what?\n\r", verb );
	buf[0] = UPPER(buf[0]);
	return;
    }

    REMOVE_BIT(ch->deaf, channel);

    switch ( channel )
    {
    default:
	sprintf( buf, "You %s '%s'\n\r", verb, argument );
	send_to_char( buf, ch );
	sprintf( buf, "$n %ss '$t'",     verb );
	break;

    case CHANNEL_IMMTALK:
	sprintf( buf, "$n: \"$t\"" );
	position	= ch->position;
	ch->position	= POS_STANDING;
	act( buf, ch, NULL, NULL, argument, NULL, TO_CHAR, SENSE_SIXTH );
	ch->position	= position;
	break;
    }

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	CHAR_DATA *och;
	CHAR_DATA *vch;

	och = d->original ? d->original : d->character;
	vch = d->character;

	if ( d->connected == CON_PLAYING
	&&   vch != ch
	&&  !IS_SET(och->deaf, channel) )
	{
	    if ( channel == CHANNEL_IMMTALK && !IS_IMMORTAL(och) )
		continue;

	    position		= vch->position;
	    act( buf, ch, vch, NULL, argument, NULL, TO_VICT, SENSE_HEARING );
	    vch->position	= position;
	}
    }

    return;
}



void do_chat( CHAR_DATA *ch, char *argument )
{
    talk_channel( ch, argument, CHANNEL_CHAT, "chat" );
    return;
}


void do_immtalk( CHAR_DATA *ch, char *argument )
{
    talk_channel( ch, argument, CHANNEL_IMMTALK, "immtalk" );
    return;
}


void do_shout( CHAR_DATA *ch, char *argument )
{
  ROOM_INDEX_DATA *room;
  CHAR_DATA *vch;
  int door, gsn;
  
  if ( !str_cmp(ch->in_room->spec_fun_name, "spec_start_room" ) )
  return;

  if ( argument[0] == '\0' )
  {
    send_to_char( "Shout what?\n\r", ch );
    return;
  }

  gsn = *language_table[ch->language].pgsn;

  act( "You shout, in $T:\n\r   \"$t\"",      ch, NULL, NULL, argument, skill_table[gsn].name, TO_CHAR, SENSE_SIXTH );

  for ( vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room )
  {
    if ( ch == vch )
      continue;

    language_to_char( ch, vch, argument, -1, "shouts", TRUE );
  }

  for ( door = 0; door < 6; door++ )
  {
    /* presently, shouts go through walls */
    if ( ch->in_room->exit[door] == NULL
    || ( room = ch->in_room->exit[door]->to_room ) == NULL )
      continue;

    for ( vch = room->people; vch != NULL; vch = vch->next_in_room )
    {
      if ( ch == vch )
        continue;

      language_to_char( ch, vch, argument, door, "shouts", TRUE );
    }
  }

  return;
}


void do_say( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *vch;
  int gsn;

  if ( !str_cmp(ch->in_room->spec_fun_name, "spec_start_room" ) )
  return;

  if ( argument[0] == '\0' )
  {
    send_to_char( "Say what?\n\r", ch );
    return;
  }

  gsn = *language_table[ch->language].pgsn;

  act( "You say, in $T:\n\r   \"$t\"",
      ch, NULL, NULL, argument, skill_table[gsn].name, TO_CHAR, SENSE_SIXTH );

  for ( vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room )
  {
    if ( ch == vch )
      continue;

    language_to_char( ch, vch, argument, -1, "says", TRUE );
  }

  return;
}


void do_tell( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim = NULL;
	CHAR_DATA *vch;
    int gsn;

    argument = one_argument( argument, arg );

    if ( !str_cmp(ch->in_room->spec_fun_name, "spec_start_room" ) )
  return;
   
    if ( argument[0] == '\0' || arg[0] == '\0'
    || ( ( victim = get_char_room( ch, arg ) ) == NULL ) )
    {
	send_to_char("Tell what to whom?\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
        send_to_char("Talking to yourself?\n\r",ch);
        return;
    }

	gsn = *language_table[ch->language].pgsn;

	if ( IS_NPC(victim) )
    {
    sprintf(log_buf, "%s to %s: \"%s\"",ch->name, victim->short_descr, argument );
    log_string( log_buf );
    return;
    }

  act( "You say to $N, in $T:\n\r   \"$t\"",
      ch, victim, NULL, argument, skill_table[gsn].name, TO_CHAR, SENSE_SIXTH );

  for ( vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room )
  {
    if ( ch == vch )
      continue;

    language_tell_to_char( ch, vch, victim, argument,  "says to", TRUE );
  }
    return;

}


void do_ooc( CHAR_DATA *ch, char *argument )
{

    
    char buf[MAX_INPUT_LENGTH];
    
    if ( !str_cmp(ch->in_room->spec_fun_name, "spec_start_room" ) )
  return;
   
 if ( argument[0] == '\0' )
    {
	send_to_char( "OOC what?\n\r", ch );
	return;
    }

     
     sprintf(buf, "%s says, OOC: \"%s\"", ch->name, argument);

        ooc8 = ooc7;
        ooc7 = ooc6;
        ooc6 = ooc5;
        ooc5 = ooc4;
        ooc4 = ooc3;
        ooc3 = ooc2;
        ooc2 = ooc1;
        ooc1 = str_dup(buf);

       act( "$n says, out of character:\n\r   \"$t\"", ch, NULL, NULL, argument, NULL, TO_ROOM, SENSE_SIXTH );
    act( "You say, out of character:\n\r   \"$t\"", ch, NULL, NULL, argument, NULL, TO_CHAR, SENSE_SIXTH );
     
   
   

/* 
    act( "$n says, OOC:\n\r   \"$t\"", ch, NULL, NULL, argument, NULL, TO_ROOM, SENSE_SIXTH );
    act( "You say, OOC:\n\r   \"$t\"", ch, NULL, NULL, argument, NULL, TO_CHAR, SENSE_SIXTH );

 */
    return;

}



void do_wish( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int position;
    extern int interp_doer;
//    DESCRIPTOR_DATA *d;

    argument = one_argument( argument, arg );
   
    if ( IS_SET(ch->act, PLR_NO_TELL) )
    {
        send_to_char("You can't wish.\n\rPlease send all inquiries to staff@darksunkings.com.\n\r",ch);
        return;
    }

    if ( arg[0] == '\0' || argument[0] == '\0')
    {
	send_to_char( "Wish whom what?\n\r", ch );
	return;
    }

    if (!str_cmp(arg, "all" ) )
        {
     /*      if( !str_cmp( argument, "Log me out please.") )
           {
            save_char_obj( ch );
    d = ch->desc;
    extract_char( ch, TRUE );
    if ( d != NULL )
        close_socket( d );

           return;
           }*/
           act("You wish to all \"$t\"",ch,NULL,NULL,argument,NULL,TO_CHAR,SENSE_SIXTH );
           sprintf(log_buf, "%s wishes to all \"%s\"",ch->name,argument );
           log_string( log_buf);
           return;
        }


    /*
     * Can tell to PC's anywhere, but NPC's only in same room.
     * -- Furey
     */
    if ( ( victim = get_char_world( ch, arg ) ) == NULL
    || ( IS_NPC(victim) && victim->in_room != ch->in_room ) )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( !IS_IMMORTAL(ch) && !IS_AWAKE(victim) )
    {
	act( "$E can't hear you.", ch, victim, NULL, NULL, NULL, TO_CHAR, SENSE_SIXTH );
	return;
    }
    if ( (IS_IMMORTAL(ch) && !IS_IMMORTAL(victim) )
	 || IS_SET(ch->act, PLR_WIZINVIS))
    {
        act( "You send to $N \"$t\"",ch,victim,NULL,argument,NULL,TO_CHAR,SENSE_SIXTH );
        position            = victim->position;
        victim->position    = POS_STANDING;
        act( "A staff member sends \"$t\"", ch, victim, NULL, argument, NULL, TO_VICT, SENSE_SIXTH );
        victim->position    = position;
        victim->reply	= ch;
        sprintf( log_buf, "%s sends to %s \"%s\"", ch->name, victim->name, argument );
        interp_doer = MAX_LEVEL;
        log_string ( log_buf );
            

        return;
    }

    if ( !IS_IMMORTAL(victim) )
    {
        send_to_char("You can't wish to mortals.\n\rSee HELP WISH.\n\r",ch);
        return;
    }
       
    if ( !IS_IMMORTAL(ch) && IS_IMMORTAL(victim) )
	{

        act( "You wish to $N \"$t\"", ch, victim, NULL, argument, NULL, TO_CHAR, SENSE_SIXTH );
        position		= victim->position;
        victim->position	= POS_STANDING;
        act( "$n wishes to you \"$t\"", ch, victim, NULL, argument, NULL, TO_VICT, SENSE_HEARING );
        sprintf( log_buf, "%s wishes to %s \"%s\"", ch->name, victim->name, argument );
        interp_doer = MAX_LEVEL;
        log_string( log_buf );
        victim->position	= position;
        victim->reply	= ch;
		return;
	}

	act( "$n wishes to you \"$t\"", ch, victim, NULL, argument, NULL, TO_VICT, SENSE_SIXTH );
        act( "You wish to $N \"$t\"", ch, victim, NULL, argument, NULL, TO_CHAR, SENSE_SIXTH );
}



void do_reply( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    int position;

    if ( ( victim = ch->reply ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( !IS_IMMORTAL(ch) && !IS_AWAKE(victim) )
    {
	act( "$E can't hear you.", ch, victim, NULL, NULL, NULL, TO_CHAR, SENSE_SIXTH );
	return;
    }

    if ( IS_IMMORTAL(ch) )
    {
        act( "You send to $N \"$t\"", ch, victim, NULL, argument, NULL, TO_CHAR, SENSE_SIXTH );
        position            = victim->position;
        victim->position    = POS_STANDING;
        act( "A staff member sends \"$t\"", ch, victim, NULL, argument, NULL, TO_VICT, SENSE_SIXTH );
        victim->position    = position;
        victim->reply       = ch;
        return;
    }


    act( "You wish to $N \"$t\"", ch, victim, NULL, argument, NULL, TO_CHAR, SENSE_SIXTH );
    position		= victim->position;
    victim->position	= POS_STANDING;
    act( "$n wishes to you \"$t\"", ch, victim, NULL, argument, NULL, TO_VICT, SENSE_HEARING );
    victim->position	= position;
    victim->reply	= ch;

    return;
}



#if 0
void do_emote( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char *plast;

    if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_NO_EMOTE) )
    {
	send_to_char( "You can't show your emotions.\n\r", ch );
	return;
    }

    if ( argument[0] == '\0' )
    {
	send_to_char( "Emote what?\n\r", ch );
	return;
    }

    for ( plast = argument; *plast != '\0'; plast++ )
	;

    strcpy( buf, argument );
/*
    if ( isalpha(plast[-1]) )
	strcat( buf, "." );
*/

/* insert special emote commands for hearing etc? or require an argument? */
    act( "$n $t", ch, NULL, NULL, buf, NULL, TO_ROOM, SENSE_SIXTH );
    act( "$n $t", ch, NULL, NULL, buf, NULL, TO_CHAR, SENSE_SIXTH );
    return;
}
#endif


void do_bug( CHAR_DATA *ch, char *argument )
{
    append_file( ch, BUG_FILE, argument );
    send_to_char( "Ok.  Thanks.\n\r", ch );
    return;
}



void do_idea( CHAR_DATA *ch, char *argument )
{
    append_file( ch, IDEA_FILE, argument );
    send_to_char( "Ok.  Thanks.\n\r", ch );
    return;
}



void do_typo( CHAR_DATA *ch, char *argument )
{
    append_file( ch, TYPO_FILE, argument );
    send_to_char( "Ok.  Thanks.\n\r", ch );
    return;
}



void do_qui( CHAR_DATA *ch, char *argument )
{
    send_to_char( "If you want to QUIT, you have to spell it out.\n\r", ch );
    return;
}



void do_quit( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    char buf[MAX_INPUT_LENGTH];

    if ( IS_NPC(ch) )
	return;

    if ( check_fighting( ch ) )
    {
	send_to_char( "No way! You are fighting.\n\r", ch );
	return;
    }

    if ( ch->position  < POS_STUNNED  )
    {
	send_to_char( "You're not DEAD yet.\n\r", ch );
	return;
    }

    if ( is_affected( ch, gsn_noquit ) && !IS_IMMORTAL( ch ) )
    {
	send_to_char("You are a bit too restless to quit just yet.\n\r", ch );
	return;
    }

    if ( ch->in_room && !IS_SET( ch->in_room->room_flags, ROOM_RENT )
    && !IS_IMMORTAL( ch ) )
    {
        /* Urik Tavern special tell. */

        if ( ch->in_room == get_room_index( ROOM_VNUM_TEMPLE ) )
        {
            send_to_char("You can't quit here.\n\rTry going west, then quit.\n\r", ch );
            return;
        }

	send_to_char("You can't quit here.\n\r", ch );
	return;
    }
    sprintf(buf, "%s",(char *) ctime( &current_time ) );
    ch->time = buf;
    send_to_char( 
	"Farewell.\n\r",
	ch );
    if (!IS_SET(ch->act, PLR_WIZINVIS) ){
    act( "$n departs from the area.", ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIXTH );
    
    }
    sprintf( log_buf, "%s has quit.", ch->name );
    log_string( log_buf );

    /*
     * After extract_char the ch is no longer valid!
     */
  //  save_char_obj( ch );
  //  d = ch->desc;
  //  extract_char( ch, TRUE );
    save_char_obj( ch );
    d = ch->desc;
    extract_char( ch, TRUE );
    if ( d != NULL )
        close_socket( d );

    
/* New quit code, needs a lot of working, causing wierd bugs. */

   /* if ( d != NULL )
    {
        extern char *help_greeting;
        write_to_buffer(d, help_greeting, 0 );
        d->connected = CON_MAIN_MENU;
        return;
    }
*/
    return;
}



void do_save( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;

    save_char_obj( ch );
    if (!IS_SET(ch->act, PLR_AFK))
	send_to_char( "Ok.\n\r", ch );
    return;
}



void do_follow( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Follow whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master != NULL )
    {
	act( "You don't wish to leave your beloved master!", ch, ch->master, NULL, NULL, NULL, TO_CHAR, SENSE_SIXTH );
	return;
    }

    if ( victim == ch )
    {
	if ( ch->master == NULL )
	{
	    send_to_char( "You already follow yourself.\n\r", ch );
	    return;
	}
	stop_follower( ch );
	return;
    }
    
    if ( ch->master != NULL )
	stop_follower( ch );

    add_follower( ch, victim );
    return;
}



void add_follower( CHAR_DATA *ch, CHAR_DATA *master )
{
    if ( ch->master != NULL )
    {
	bug( "Add_follower: non-null master.", 0 );
	return;
    }

    ch->master        = master;
    ch->leader        = NULL;

    act( "You now follow $N.",  ch, master, NULL, NULL, NULL, TO_CHAR, SENSE_SIXTH );

    return;
}



void stop_follower( CHAR_DATA *ch )
{
    if ( ch->master == NULL )
    {
	bug( "Stop_follower: null master.", 0 );
	return;
    }

    if ( IS_AFFECTED(ch, AFF_CHARM) )
    {
	REMOVE_BIT( ch->affected_by, AFF_CHARM );
	affect_strip( ch, gsn_charm_person );
    }

    act( "You stop following $N.", ch, ch->master, NULL, NULL, NULL, TO_CHAR, SENSE_SIXTH );

    ch->master = NULL;
    ch->leader = NULL;
    return;
}



void die_follower( CHAR_DATA *ch )
{
    CHAR_DATA *fch;

    if ( ch->master != NULL )
	stop_follower( ch );

    ch->leader = NULL;

    for ( fch = char_list; fch != NULL; fch = fch->next )
    {
	if ( fch->master == ch )
	    stop_follower( fch );
	if ( fch->leader == ch )
	    fch->leader = fch;
    }

    return;
}



void do_order( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *och;
    CHAR_DATA *och_next;
    bool found;
    bool fAll;

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
	send_to_char( "Order whom to do what?\n\r", ch );
	return;
    }

/*
    if ( IS_AFFECTED( ch, AFF_CHARM ) )
    {
	send_to_char( "You feel like taking, not giving, orders.\n\r", ch );
	return;
    }
*/

    act( "$n issues the order '$t'", ch, NULL, NULL, argument, NULL, TO_NOTVICT, SENSE_HEARING );

    if ( !str_cmp( arg, "all" ) )
    {
	fAll   = TRUE;
	victim = NULL;
    }
    else
    {
	fAll   = FALSE;
	if ( ( victim = get_char_room( ch, arg ) ) == NULL )
	{
	    send_to_char( "They aren't here.\n\r", ch );
	    return;
	}

	if ( victim == ch )
	{
	    send_to_char( "Uh, brilliant, ordering yourself?\n\r", ch );
	    return;
	}

	if ( !IS_AFFECTED(victim, AFF_CHARM) || victim->master != ch )
	{
	    send_to_char( "Do it yourself!\n\r", ch );
	    return;
	}
    }

    found = FALSE;
    for ( och = ch->in_room->people; och != NULL; och = och_next )
    {
	och_next = och->next_in_room;

	if ( IS_AFFECTED(och, AFF_CHARM)
	&&   och->master == ch
	&& ( fAll || och == victim ) )
	{
	    found = TRUE;
	    act( "$n orders you to '$t'", ch, och, NULL, argument, NULL, TO_VICT, SENSE_HEARING );
	    interpret( och, argument );
	}
    }

    if ( found )
	send_to_char( "Ok.\n\r", ch );
    else
	send_to_char( "You have no followers here.\n\r", ch );
    return;
}



void do_group( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	CHAR_DATA *gch;
	CHAR_DATA *leader;

	leader = (ch->leader != NULL) ? ch->leader : ch;
	sprintf( buf, "%s's group:\n\r", person( ch, leader ) );
	send_to_char( buf, ch );

	for ( gch = char_list; gch != NULL; gch = gch->next )
	{
	    if ( is_same_group( gch, ch ) )
	    {
		sprintf( buf, "%-16s\n\r", person( ch, gch) );
		send_to_char( buf, ch );
	    }
	}
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( ch->master != NULL || ( ch->leader != NULL && ch->leader != ch ) )
    {
	send_to_char( "But you are following someone else!\n\r", ch );
	return;
    }

    if ( victim->master != ch && ch != victim )
    {
	act( "$N isn't following you.", ch, victim, NULL, NULL, NULL, TO_CHAR, SENSE_SIXTH );
	return;
    }

    if ( is_same_group( victim, ch ) && ch != victim )
    {
	victim->leader = NULL;
	act( "$n removes you from $s group.",  ch, victim, NULL, NULL, NULL, TO_VICT, SENSE_SIXTH );
	act( "You remove $N from your group.", ch, victim, NULL, NULL, NULL, TO_CHAR, SENSE_SIXTH );
	return;
    }

    victim->leader = ch;
    act( "You join $n's group.", ch, victim, NULL, NULL, NULL, TO_VICT, SENSE_SIXTH );
    act( "$N joins your group.", ch, victim, NULL, NULL, NULL, TO_CHAR, SENSE_SIXTH );
    return;
}



void do_gtell( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *gch;

    if ( argument[0] == '\0' )
    {
	send_to_char( "Tell your group what?\n\r", ch );
	return;
    }

    if ( IS_SET( ch->act, PLR_NO_TELL ) )
    {
	send_to_char( "Your message didn't get through!\n\r", ch );
	return;
    }

    /*
     * Note use of send_to_char, so gtell works on sleepers.
     */
    sprintf( buf, "%s tells the group '%s'.\n\r", ch->name, argument );
    for ( gch = char_list; gch != NULL; gch = gch->next )
    {
	if ( is_same_group( gch, ch ) )
	    send_to_char( buf, gch );
    }

    return;
}



/*
 * It is very important that this be an equivalence relation:
 * (1) A ~ A
 * (2) if A ~ B then B ~ A
 * (3) if A ~ B  and B ~ C, then A ~ C
 */
bool is_same_group( CHAR_DATA *ach, CHAR_DATA *bch )
{
    if ( ach->leader != NULL ) ach = ach->leader;
    if ( bch->leader != NULL ) bch = bch->leader;
    return ach == bch;
}
