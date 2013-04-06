#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"

static char * const he_she  [] = { "it",  "he",  "she" };
static char * const him_her [] = { "it",  "him", "her" };
static char * const his_her [] = { "its", "his", "her" };

bool execute_emote( CHAR_DATA *ch, char *argument, bool fPemote, bool fSemote );
bool emote_group( CHAR_DATA *ach, CHAR_DATA *bch );
bool emote_sniff( CHAR_DATA *ch, CHAR_DATA *victim );

void do_emote( CHAR_DATA *ch, char *argument )
{
    execute_emote( ch, argument, FALSE, FALSE );
}


void do_pemote( CHAR_DATA *ch, char *argument )
{
    execute_emote( ch, argument, TRUE, FALSE );
}

void do_semote( CHAR_DATA *ch, char *argument )
{
    execute_emote( ch, argument, FALSE, TRUE );
}


void do_spemote( CHAR_DATA *ch, char *argument )
{
    execute_emote( ch, argument, TRUE, TRUE );
}

bool emote_sniff( CHAR_DATA *ch, CHAR_DATA *victim )
{
  int eyes = 0;
    
  if ( ch == victim )
    eyes = 101;

  if (HAS_HOLYLIGHT(ch)) {
    eyes = 101;
  }

  return (number_percent() < eyes);
}

bool emote_group( CHAR_DATA *ach, CHAR_DATA *bch )
{
  if ( ach->leader != NULL ) 
    ach = ach->leader;

  if ( bch->leader != NULL ) 
    bch = bch->leader;

  return ach == bch;
}


/* 0 on success */
bool execute_emote( CHAR_DATA *ch, char *argument, bool fPemote, bool fSemote )
{
  CHAR_DATA *vch;
//  static char buf[MAX_STRING_LENGTH];

  if (argument[0] == '\0') {
    send_to_char("Emote what?\n\r", ch);
    return 1;
  }                                                                                     

  if (!emote_processor(ch, argument, fPemote, FALSE, ch))
    return 1;

  for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room) {
    vch->leader=NULL;
    if (fSemote) {
      if (emote_sniff(vch, ch))
	vch->leader=ch;
      else
	emote_processor(ch, argument, fPemote, fSemote, vch);
    }
  }

  for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room) {              
    if (fSemote) {
      if (vch->leader==ch) {
	send_to_char( emote_processor( ch, argument, fPemote, fSemote, vch ), vch );
      }
    } else
      send_to_char( emote_processor( ch, argument, fPemote, fSemote, vch ), vch );
  }
  return 0;
}



char *emote_processor( CHAR_DATA *ch, char *argument, bool fPemote, bool fSemote,
  CHAR_DATA *vch )
{
  static char buf[MAX_STRING_LENGTH];
  CHAR_DATA *wch;
  OBJ_DATA *obj;
  bool fBegin = TRUE, fPoss = FALSE;
  char word[MAX_INPUT_LENGTH];
  char *pin = argument;

  for ( pin = argument; *pin != '\0'; pin++ ) {
    if ( *pin == '@' ) {
	fBegin = FALSE;
	break;
    }
  }

  pin = argument;
  buf[0] = '\0';

  /* If no @, copy emoter's short desc. */
  if (fBegin) {
    //    if (vch == ch) {
    //      strcat(buf, (fPemote ? "your" : "you"));
    //    } else {
      strcpy(buf, person(vch, ch));
      if ( fPemote ) {
	if ( buf[strlen(buf)-1] == 's' )
	  strcat( buf, "'" );
	else
	  strcat( buf, "'s" );
	//      }
	//    }
      }  
      strcat( buf, " " );
  }

  while ( *pin != '\0' ) {
    fPoss = FALSE;
    switch( *pin ) {
    default:
      one_emote_argument( pin, word );
      strcat( buf, word );
      pin = pin + (strlen( word ) - 1);
      break;
      
    case ' ':
      strcat( buf, " " );
      break;
      
    case '@':
      if (vch == ch) {
	strcat(buf, (fPemote ? "your" : "you"));
      } else {
	strcat(buf, person(vch, ch));
	if ( fPemote )
	  strcat( buf, (buf[strlen(buf)-1] == 's') ? "'" : "'s");
      }
      break;
      
    case '%':
      fPoss = TRUE;
    case '~':
      one_emote_argument( &pin[1], word );	      
      if ((obj = get_obj_here(ch, strip_punct(word))) != NULL) {
	if (can_see_obj(vch, obj)) {
	  strcat( buf, obj->short_descr );
	  if ( fPoss ) {
	    if ( word[strlen(word) - 1] == 's' )
	      strcat( buf, "'" );
	    else
	      strcat( buf, "'s" );
	  }
	} else
	  strcat( buf, (fPoss ? "something's" : "something"));
      } else if ((wch = get_char_room(ch, strip_punct(word))) != NULL) {
	if (fSemote) 
	  wch->leader = ch;
	if (vch == wch) {
	  strcat(buf, (fPoss ? "your" : "you"));
	} else {
	  strcat(buf, person(vch, wch));
	  if (fPoss) {
	    if ( buf[strlen(buf)-1] == 's' )
	      strcat( buf, "'" );
	    else
	      strcat( buf, "'s" );
	  }
	}
      } else {
	sprintf( buf, "%s is not here.\n\r", word );
	send_to_char( buf, ch );
	return NULL;
      }
      
      pin = pin + strlen( word );
      fPoss = FALSE;
      break;
      
      /* %m  him/her */
    case '!':
      one_emote_argument( &pin[1], word);
      if ((obj = get_obj_here(ch, strip_punct(word))) != NULL) {
	strcat(buf, ((can_see_obj(vch, obj)) ? "it" : "something"));
      } else if ((wch = get_char_room(ch, strip_punct(word))) != NULL) {
	if (fSemote)
	  wch->leader = ch;
	strcat(buf, ((vch == wch) ? "you" : him_her[wch->sex]));
      } else {
        sprintf( buf, "%s is not here.\n\r", word );
        send_to_char( buf, ch );
        return NULL;
      }

      pin = pin + strlen( word );
      break;
	
	/* %s  his/her */
    case '^':
      one_emote_argument( &pin[1], word);
      if ((obj = get_obj_here(ch, strip_punct(word))) != NULL) {
        strcat(buf, ((can_see_obj(vch, obj)) ? "its" : "something's"));
      } else if ((wch = get_char_room(ch, strip_punct(word))) != NULL) {
        if (fSemote)
          wch->leader = ch;
        strcat(buf, ((vch == wch) ? "your" : his_her[wch->sex]));
      } else {
        sprintf( buf, "%s is not here.\n\r", word );
        send_to_char( buf, ch );
        return NULL;
      }

      pin = pin + strlen( word );
      break;
      
      /* %s  he/she */
    case '#':
      one_emote_argument( &pin[1], word);
      if ((obj = get_obj_here(ch, strip_punct(word))) != NULL) {
        strcat(buf, ((can_see_obj(vch, obj)) ? "it" : "something"));
      } else if ((wch = get_char_room(ch, strip_punct(word))) != NULL) {
        if (fSemote)
          wch->leader = ch;
        strcat(buf, ((vch == wch) ? "you" : he_she[wch->sex]));
      } else {
        sprintf( buf, "%s is not here.\n\r", word );
        send_to_char( buf, ch );
        return NULL;
      }
      
      pin = pin + strlen( word );
      break;
    }
    
    pin++;
  }

  strcat( strip_punct(buf), ".\n\r" );
  buf[0] = UPPER( buf[0] );
  return buf;
}
