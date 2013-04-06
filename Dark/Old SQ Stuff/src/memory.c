#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "merc.h"


HUNTING_DATA *		hunting_free;

void set_hunting( CHAR_DATA *ch, CHAR_DATA *victim, int reason, int time )
{
  HUNTING_DATA *h;

  if ( !IS_NPC( ch ) )
    return;

  /* already hunting victim? */
  for ( h = ch->hunting; h != NULL; h = h->next )
  {
    if ( h->victim == victim )
      return;
  }

  if ( hunting_free == NULL )
  {
    h = alloc_perm( sizeof( *h ) );
  }
  else
  {
    h = hunting_free;
    hunting_free = h->next;
  }

  h->next = ch->hunting;
  h->reason = reason;
  h->time = time;
  h->victim = victim;
  if ( !IS_NPC( victim ) )
    strcpy( h->name, victim->name );
  else
    h->name[0] = '\0';
  ch->hunting = h;

  return;
}

void remove_hunting( CHAR_DATA *ch, CHAR_DATA *victim, bool fDead )
{
  HUNTING_DATA *h;
  HUNTING_DATA *h_last;

  for ( h = ch->hunting; h != NULL; h = h->next )
  {
    if ( h->victim == victim )
      break;
  }

  if ( h == NULL )
    return;

  /*
   * If it's a PC, just remove the pointer.
   * We'll reset the pointer if they log back in.
   */
  if ( h->name[0] != '\0'
  && fDead == FALSE )
  {
    h->victim = NULL;
    return;
  }

  if ( ch->hunting == h )
  {
    ch->hunting = h->next;
  }
  else
  {
    for ( h_last = ch->hunting; h_last != NULL; h_last = h_last->next )
    {
      if ( h_last->next == h )
        break;
    }

    h_last->next = h->next;
  }

  h->victim = NULL;
  h->next = hunting_free;
  hunting_free = h;

  return;
}


bool is_hunting( CHAR_DATA *ch, CHAR_DATA *victim )
{
  HUNTING_DATA *h;

  /*
   * Be careful.  PCs can have null victims.
   */
  for ( h = ch->hunting; h != NULL; h = h->next )
  {
    if ( h->victim == victim )
      return TRUE;
  }

  return FALSE;
}



void restore_hunting( CHAR_DATA *ch )
{
  CHAR_DATA *vch;
  HUNTING_DATA *h;

  for ( vch = char_list; vch != NULL; vch = vch->next )
  {
    if ( ( h = vch->hunting ) == NULL )
      continue;

    for ( h = vch->hunting; h != NULL; h = h->next )
      if ( h->victim == NULL && !str_cmp( h->name, ch->name ) )
        break;

    if ( h )
      h->victim = ch;
  }
}
