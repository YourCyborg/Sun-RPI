#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "languages.h"


inline void clear_dynamic( CHAR_DATA *ch )
{
//  if ( ch->dynamic_descr )
    free_string( ch->dynamic_descr, MEM_GENERAL );

  ch->dynamic_descr = NULL;

  return;
}


void do_change( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    char *c;
    int i;
    OBJ_DATA *obj;
    OBJ_DATA *obj2;

    argument = one_argument( argument, arg );
 
    if ( arg[0] == '\0' )
    {
        send_to_char( "What do you want to change?\n\r", ch );
        send_to_char( "Current options: hands, ddesc/ldesc.\n\r", ch );
        return;
    }

    if ( !str_cmp( arg, "ddesc" ) || !str_cmp( arg, "ldesc" ) )
    {
      clear_dynamic( ch );

      if ( !str_cmp( argument, "clear" ) )
      {
        send_to_char( "Dynamic desc cleared.\n\r", ch );
	return;
      }

      c = emote_processor( ch, argument, FALSE, FALSE, ch );
      if ( c )
      {
        ch->dynamic_descr = str_dup( c );
        send_to_char( "Dynamic description set to : ", ch);
        send_to_char( ch->dynamic_descr, ch );
        send_to_char( "\n\r", ch );
      }
      return;
    }

    if ( !str_cmp( arg, "hands" ) )
    {
	obj = get_eq_char( ch, gn_wear_primary );
	obj2 = get_eq_char( ch, gn_wear_secondary );

	if ( obj == NULL && obj2 == NULL )
	{
	    send_to_char("You aren't holding anything to change hands.\n\r", ch );
	    return;
	}

	if ( obj != NULL )
	    unequip_char( ch, obj );
	
	if ( obj2 != NULL )
	    unequip_char( ch, obj2 );

	if ( obj != NULL )
	    equip_char( ch, obj, gn_wear_secondary );

	if ( obj2 != NULL )
	    equip_char( ch, obj2, gn_wear_primary );

	if ( obj == NULL || obj2 == NULL )
	{
	    act("$n moves $p to $s other hand.", ch, NULL,
		( obj == NULL? obj2 : obj ), NULL, NULL,
		TO_ROOM, SENSE_SIGHT );
	    act("You move $p to your other hand.", ch, NULL,
		( obj == NULL? obj2 : obj ), NULL, NULL,
		TO_CHAR, SENSE_SIGHT );
	}
	else
	{
	    act("$n swaps $p and $P in $s hands.", ch, NULL, obj, obj2, NULL,
		TO_ROOM, SENSE_SIGHT );
	    act("You swap $p and $P in your hands.", ch, NULL, obj, obj2, NULL,
		TO_CHAR, SENSE_SIGHT );
	}

	return;
    }

    if ( !str_prefix( arg, "language" ) )
    {
      int lmax = gsn_andean + MAX_LANGUAGE;

      i = language_lookup( argument );

      if ( i == -1
      || ( get_skill( ch, *language_table[i].pgsn ) < 1 ) )
      {
        send_to_char( "You don't know that language.  Try:\n\r", ch );
	for ( i = gsn_andean; i < lmax; i++ )
	  if ( get_skill( ch, i ) > 0 )
	  {
	    send_to_char( skill_table[i].name, ch );
	    send_to_char( "\n\r", ch );
          }
        return;
      }

      ch->language = i; 
      sprintf( buf, "Language set to %s.\n\r",
        skill_table[*language_table[i].pgsn].name );
      send_to_char( buf, ch );
      return;
    }

    send_to_char( "What do you want to change?\n\r", ch );
    send_to_char( "Current options: hands, ddesc/ldesc.\n\r", ch );

    return;
}
