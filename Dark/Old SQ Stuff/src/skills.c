
#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"



void do_spells( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int sn;
    int col;

    send_to_char("    You have knowledge of the following spells:\n\n\r", ch );

    col    = 0;
    for ( sn = 0; sn < MAX_SKILL; sn++ )
    {
	if ( skill_table[sn].name == NULL )
	    break;
	if ( !IS_SET( skill_table[sn].skill_flags, SKILL_SPELL ) )
	    continue;
	if ( get_skill( ch, sn ) < 5 )
	    continue;

	sprintf( buf, "%24s  ",
	    skill_table[sn].name );
	    send_to_char( buf, ch );

	    if ( ++col % 3 == 0 )
		send_to_char( "\n\r", ch );
    }

    if ( col % 3 != 0 )
      send_to_char( "\n\r", ch );

    return;
}


void do_skills( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    int sn;
    int col;
    int type;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	do_skills( ch, "all" );
	return;
    }
    else if ( !str_prefix( arg, "psionic" ) )
    {
        send_to_char(
          "   You have the following psionic abilities:\n\n\r", ch);
          type = SKILL_PSIONIST;
    }
    else if ( !str_prefix( arg, "combat" ) )
    {
	send_to_char(
	  "   You have the following fighting abilities:\n\n\r",  ch );
	type = SKILL_COMBAT;
    }
    else if ( !str_prefix( arg, "stealth" ) )
    {
	send_to_char( "   You have the following stealth skills:\n\n\r", ch );
	type = SKILL_STEALTH;
    }
    else if ( !str_prefix( arg, "handling" ) )
    {
	send_to_char(
	  "   You have the following handling abilities:\n\n\r", ch );
	type = SKILL_HANDLING;
    }
    else if ( !str_prefix( arg, "languages" ) )
    {
	send_to_char(
	  "   You have the following language skills:\n\n\r", ch );
	type = SKILL_LANGUAGE;
    }
    else if ( !str_prefix( arg, "all" ) )
    {
	do_skills( ch, "psionic" );
        send_to_char("\n\r",ch);
        do_skills( ch, "combat" );
	send_to_char("\n\r",ch);
	do_skills( ch, "stealth" );
	send_to_char("\n\r",ch);
	do_skills( ch, "handling" );
	send_to_char("\n\r",ch);
	do_skills( ch, "languages" );
	return;
    }
    else
    {
	do_skills( ch, "all" );
	return;
    }

    col    = 0;
    for ( sn = 0; sn < MAX_SKILL; sn++ )
    {
	if ( skill_table[sn].name == NULL )
	    break;
	if ( !IS_SET( skill_table[sn].skill_flags, type ) )
	    continue;
	if ( get_skill( ch, sn ) < 5 )
	    continue;

	sprintf( buf, "%24s ", skill_table[sn].name );
	send_to_char( buf, ch );

	    if ( ++col % 3 == 0 )
		send_to_char( "\n\r", ch );
	}

	if ( col % 3 != 0 )
	    send_to_char( "\n\r", ch );

    return;
}


void skill_improve( CHAR_DATA *ch, int sn )
{
    SKILL_DELAY_DATA delay;
    int iSkill;
    int iMax;

    iMax = 100;

    if (IS_NPC(ch))
	return;

    iSkill = ch->pcdata->learned[sn];

    if ( iSkill >= skill_table[sn].percent_max )
      return;

    if ( is_delayed( ch, sn ) )
	return;

/* implement delay between improves */

    if (ch->pcdata->learned[sn] < 5
    && IS_SET(skill_table[sn].skill_flags,SKILL_SPELL) )
	return;

    if (skill_table[sn].prerequisite != NULL)
    {
	iMax = ch->pcdata->learned[*skill_table[sn].prerequisite];
    }

    if (skill_table[sn].skill_group != NULL)
    {
	iMax = UMIN( iMax, ch->pcdata->learned[*skill_table[sn].skill_group] );
    }

    iMax = UMIN( iMax, skill_table[sn].percent_max );

    if (dice(1,iMax) < ch->pcdata->learned[sn])
    {
/* learned nothing */
	delay.skill = sn;
	delay.expire = current_time
	  + ( skill_table[sn].learn_delay * 60 * 100 / get_curr_int( ch ) );
	delay_to_char( ch, &delay );
	return;
    }

    ch->pcdata->learned[sn] = UMIN(skill_table[sn].percent_max,
		ch->pcdata->learned[sn] + (skill_table[sn].percent_prac / 10));

    ch->pcdata->learned[sn] = UMIN(ch->pcdata->learned[sn], iMax);

    ch->pcdata->learned[sn] = UMAX( ch->pcdata->learned[sn], iSkill );

/* delay to next possible learn */
    delay.skill = sn;
    delay.expire = current_time
	  + ( skill_table[sn].learn_delay * 60 * 100 / get_curr_int( ch ) );
    delay_to_char( ch, &delay );
    return;
}


void power_improve( CHAR_DATA *ch, int sn )
{
    SKILL_DELAY_DATA delay;
    int iPower;
    int iMax;

    iMax = 100;

    if (IS_NPC(ch))
	return;

    iPower = ch->pcdata->power_level[sn];

    if ( iPower >= ch->pcdata->learned[sn] )
      return;

    if ( is_delayed( ch, sn ) )
	return;

    if (ch->pcdata->learned[sn] < 5
    && IS_SET(skill_table[sn].skill_flags,SKILL_SPELL) )
	return;

    if (skill_table[sn].prerequisite != NULL)
    {
	if ( IS_SET( skill_table[sn].skill_flags, SKILL_SPELL ) )
	    iMax = ch->pcdata->power_level[*skill_table[sn].prerequisite];
	else
	    iMax = ch->pcdata->learned[sn];
    }

    if (skill_table[sn].skill_group != NULL)
    {
	iMax = UMIN( iMax, ch->pcdata->learned[*skill_table[sn].skill_group]);
    }

/*
    iMax = UMIN( iMax, skill_table[sn].percent_max );
*/

    if ( dice(1,iMax) < ch->pcdata->power_level[sn] )
    {
/* learned nothing */
	delay.skill = sn;
        delay.expire = current_time
	  + ( skill_table[sn].learn_delay * 60 * 100 / get_curr_int( ch ) );
	delay_to_char( ch, &delay );
	return;
    }

    ch->pcdata->power_level[sn] = ch->pcdata->power_level[sn] + 1;

    ch->pcdata->learned[sn] = UMAX( ch->pcdata->learned[sn], iPower );

/* delay to next possible learn */
    delay.skill = sn;
    delay.expire = current_time
	  + ( skill_table[sn].learn_delay * 60 * 100 / get_curr_int( ch ) );
    delay_to_char( ch, &delay );
    return;
}


void view_skill_delays( CHAR_DATA *ch, CHAR_DATA *vch )
{
  char buf[MAX_STRING_LENGTH];
  SKILL_DELAY_DATA *d;
  SKILL_DELAY_DATA *d_next;

  if ( IS_NPC(vch) || vch->pcdata->skill_delays == NULL )
  {
    sprintf( buf, "%s has no delayed skills.\n\r", vch->name );
    send_to_char( buf, ch );
    return;
  }

  for ( d = vch->pcdata->skill_delays; d != NULL; d = d_next )
  {
    d_next = d->next;

    if ( current_time >= d->expire )
    {
      delay_remove( vch, d );
      continue;
    }

    sprintf( buf, "Skill: %s, delayed until: %s",
      skill_table[d->skill].name, (char *) ctime( &d->expire ) );

    send_to_char( buf, ch );
  }

  return;
}
