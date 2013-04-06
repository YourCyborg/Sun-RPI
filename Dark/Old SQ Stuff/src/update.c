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

extern void  raw_kill        args( ( CHAR_DATA *victim ) );


extern time_t environment_update_time;
extern bool weather_update( void );
extern int environment_interval;



/*
 * Local functions.
 */
int	hit_gain	args( ( CHAR_DATA *ch ) );
int	mana_gain	args( ( CHAR_DATA *ch ) );
int	move_gain	args( ( CHAR_DATA *ch ) );
int     stun_gain   args( ( CHAR_DATA *ch ) );
void	mobile_update	args( ( void ) );
void	char_update	args( ( void ) );
void	pc_update	args( ( void ) );
void	obj_update	args( ( void ) );
void	decay_corpse	args( ( OBJ_DATA *corpse ) );
void   fall_update  args( ( void ) );
void   knockout_update args( ( void ) );
/*
void sundayshutdown ()
{

 //. char * ctime( &current_time )
  
  char *sunday = "Sun";
  char *starttime = "06:00:00";
  char *endtime = "17:00:00";
  char *current = ctime;
    
   
    if ( !strpbrk( current, sunday ) )
    {
        if ( !strstr( ctime, starttime ) )
        {
            sprintf(log_buf, " System downtime mode at: %s set. ", ctime );
            log_string( log_buf );
        }
        else if ( !strchr( ctime, endtime ) )
        {
            sprintf( log_buf, " Normal mode set at: %s:", ctime );
            log_string( log_buf );
        }
        else
        {
            return FALSE;
        }
    return FALSE;   
    }

}
*/
/*
 * Regeneration stuff.
 */
int hit_gain( CHAR_DATA *ch )
{
    int gain = 0;
    int chance = 0;

    if ( IS_NPC(ch) )
    {
	return 0;
    }
    else
    {
	chance = get_curr_tou(ch);

	switch ( ch->position )
	{
	case POS_SLEEPING: chance *= 150; chance /= 100;	break;
	case POS_RESTING:  chance *= 125; chance /= 100;	break;
	case POS_SITTING:  chance *= 110; chance /= 100;	break;
	}

	if ( ch->pcdata->condition[COND_FULL]   == 0 )
	    chance /= 2;

	if ( ch->pcdata->condition[COND_THIRST] == 0 )
	    chance /= 2;

    }

    if ( IS_AFFECTED(ch, AFF_POISON) )
	chance /= 4;

    chance /= 5;

    while ( chance > 0 )
    {
	if ( number_percent() < chance )
	    gain++;
	chance -= 100;
    }

    return UMIN(gain, ch->max_hit - ch->hit[HIT_LOC_BODY]);
}



int mana_gain( CHAR_DATA *ch )
{
    int gain = 0;
    int chance = 0;

    if ( IS_NPC(ch) )
    {
	return 0;
    }
    else
    {
	chance = get_curr_int( ch ); 

	switch ( ch->position )
	{
	case POS_SLEEPING: chance = 0;	break;
	case POS_RESTING:  chance *= 150; chance /= 100;	break;
	case POS_SITTING:  chance *= 120; chance /= 100;	break;
	}

	if ( ch->pcdata->condition[COND_FULL]   == 0 )
	    chance /= 2;

	if ( ch->pcdata->condition[COND_THIRST] == 0 )
	    chance /= 2;

    }

    if ( IS_AFFECTED( ch, AFF_POISON ) )
	chance /= 4;

    chance /= 5;

    while ( chance > 0 )
    {
	if ( number_percent() < chance )
	    gain++;
	chance -= 100;
    }

    return UMIN(gain, ch->max_mana - ch->mana);
}



int move_gain( CHAR_DATA *ch )
{
    int gain = 0;
    int chance = 0;

    if ( IS_NPC(ch) )
    {
	return 0;
    }
    else
    {
	chance = get_curr_dex(ch);

	switch ( ch->position )
	{
	case POS_SLEEPING: chance *= 4;		break;
	case POS_RESTING:  chance *= 3;	break;
	case POS_SITTING:  chance *= 2;	break;
	case POS_STANDING: chance = 0; break;
	}

	if ( ch->pcdata->condition[COND_FULL]   == 0 )
	    chance /= 2;

	if ( ch->pcdata->condition[COND_THIRST] == 0 )
	    chance /= 2;
    }

    if ( IS_AFFECTED(ch, AFF_POISON) )
	chance /= 4;

    chance /= 5;

    while ( chance > 0 )
    {
	if ( number_percent() < chance )
	    gain++;
	chance -= 100;
    }

    return UMIN(gain, ch->max_move - ch->move);
}

int stun_gain( CHAR_DATA *ch )
{
    int gain = 0;
    int chance = 0;

    if ( IS_NPC(ch) )
    {
	return 0;
    }
    else
    {
	chance = get_curr_tou(ch);

	switch ( ch->position )
	{
	case POS_SLEEPING: 
        if ( ch->stun <= 0 )
        {
        chance *= 10; chance /= 50;	
        }
        else
        {
        chance *= 150; chance /= 100; break;
        }

	case POS_RESTING:  chance *= 125; chance /= 100;	break;
	case POS_SITTING:  chance *= 125; chance /= 100;	break;
	}


    }

       chance /= 5;

    while ( chance > 0 )
    {
	if ( number_percent() < chance )
	    gain++;
	chance -= 100;
    }

    return UMIN(gain, ch->max_stun - ch->stun);
}

void gain_condition( CHAR_DATA *ch, int iCond, int value )
{
    int condition;

    if ( value == 0 || IS_NPC(ch) || ch->level >= LEVEL_IMMORTAL )
	return;

    condition				= ch->pcdata->condition[iCond];

    if (condition < 0)
	return;

    ch->pcdata->condition[iCond]	= UMAX( 0, condition + value );

  /*  if ( ch->contact != NULL )
    {
        send_to_char("You suffer from mental strain.\n\r",ch);
        ch->stun -= 9;
        return;
    }*/

    if ( ch->pcdata->condition[iCond] == 0 )
    {
	switch ( iCond )
	{
	case COND_FULL:
	    send_to_char( "You are hungry.\n\r",  ch );
	    break;

	case COND_THIRST:
	    send_to_char( "You are thirsty.\n\r", ch );
	    break;

	case COND_DRUNK:
	    if ( condition != 0 )
		send_to_char( "You are sober.\n\r", ch );
	    break;
	}
    }

    return;
}



/*
 * Mob autonomous action.
 * This function takes 25% to 35% of ALL Merc cpu time.
 * -- Furey
 */
void mobile_update( void )
{
  CHAR_DATA *ch;
  CHAR_DATA *ch_next;
  CHAR_DATA *vch;
  ROOM_INDEX_DATA *in_room;
  AFFECT_DATA *paf;
  AFFECT_DATA *paf_next;
  EXIT_DATA *pexit;
  HUNTING_DATA *h;
  int door = 0;

  /* Examine all mobs. */
  for ( ch = char_list; ch != NULL; ch = ch_next )
  {
    ch_next = ch->next;

    for ( paf = ch->affected; paf != NULL; paf = paf_next )
    {
      paf_next	= paf->next;
      if ( paf->duration > 0 )
        paf->duration--;
      else if ( paf->duration < 0 )
        ;
      else
      {
	if ( paf_next == NULL
	||   paf_next->type != paf->type
	||   paf_next->duration > 0 )
	{
	  if ( paf->type > 0 && skill_table[paf->type].msg_off )
	  {
	    send_to_char( skill_table[paf->type].msg_off, ch );
	    send_to_char( "\n\r", ch );
	  }
	}
	  
	affect_remove( ch, paf );
      }
    }

    /* examine delayed functions */
    if ( ch->predelay_time > 0 )
    {
      ch->predelay_time--;

      if ( ch->predelay_time == 0 )
      {
	if ( ch->predelay_info != NULL )
	{
          (*ch->predelay_info->fnptr) ( ch, ch->predelay_info->argument );

          free_predelay( ch->predelay_info );
	  ch->predelay_info = NULL;

	  continue;
	}
      }
    }

    if ( ch->desc != NULL )
      continue;

    if ( !IS_NPC(ch) || ch->in_room == NULL || IS_AFFECTED(ch, AFF_CHARM)
    || ch->position <= POS_STUNNED )
      continue;

    if ( ch->position == POS_SLEEPING
    && ch->pIndexData->start_pos != POS_SLEEPING
    && !is_affected( ch, gsn_sleep ) )
    {
      act( "$n wakes up.", ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIGHT );
      ch->position = POS_RESTING;
      WAIT_STATE( ch, 8 );
      continue;
    }

    if ( ch->wait > 0 )
    {
      ch->wait--;
      continue;
    }

    if ( number_bits( 9 ) > ch->speed )
      continue;

    /* Examine call for special procedure */
    if ( ch->spec_fun != 0 )
    {
      if ( (*ch->spec_fun) ( NULL, NULL, NULL, NULL, ch ) )
      continue;
    }

    if ( ch->fighting != NULL )
    {
      if ( ch->position != POS_STANDING )
      {
        if ( ch->position >= POS_STUNNED )
	{
          act("$n quickly gets to $s feet.",
	    ch, NULL, NULL, NULL, NULL, TO_ROOM, TRUE );
	  ch->position = POS_STANDING;
	  WAIT_STATE( ch, 8 );
	}
	continue;
      }

      /* combat stuff */
    }

    if ( check_fighting( ch ) )
      continue;

    in_room = ch->in_room;

/**** grudges ****/
    /* memory */
    for ( h = ch->hunting; h != NULL; h = h->next )
    {
      if ( h->victim
      && h->victim->in_room == in_room
      && can_see( ch, h->victim ) )
        break;
    }

    if ( h )
    {
      CH_STAND_FIRST( one_hit( ch, h->victim, TYPE_UNDEFINED ) );
      continue;
    }

    /* memory to next room */
    if ( !IS_SET(ch->act, ACT_SENTINEL) )
    {
      for ( h = ch->hunting; h != NULL; h = h->next )
      {
        if ( h->victim == NULL )
          continue;

        for ( door = 0; door < 6; door++ )
        {
          if ( ( pexit = in_room->exit[door] ) != NULL
	  && h->victim->in_room == pexit->to_room )
	    break;
        }

	if ( door < 6 )
	  break;
      }

      if ( h )
      {
	CH_STAND_FIRST( move_char( ch, door ) );
	continue;
      }
    }

/**** end grudges ****/


/****  aggression  ****/
    if ( ch->aggression != 0 )
    {
      int agg = 0;
      int a;
      CHAR_DATA *ach = NULL;

      for ( vch = in_room->people; vch != NULL; vch = vch->next_in_room )
      {
        if ( vch == ch )
	  continue;

	a = get_aggr_factor( ch, vch );

	if ( a > agg )
	{
	  agg = a;
	  ach = vch;
	}
      }

      if ( agg != 0 )
      {
        CH_STAND_FIRST( one_hit( ch, ach, TYPE_UNDEFINED ) );
        continue;
      }
    }

    if ( IS_SET( ch->aggression, AGGRESSIVE_LONG_DISTANCE ) )
    {
      CHAR_DATA *vch = NULL;
      int agg = 0;
      int a;
      CHAR_DATA *ach = NULL;

      door = number_range(0,5);

      if ( (pexit = in_room->exit[door]) != NULL && pexit->to_room != NULL)
      {
	for ( vch = pexit->to_room->people; vch != NULL; vch = vch->next )
	{
	  if ( get_aggr_factor( ch, vch ) )
	    break;
	}

	if ( vch )
	{
	  CH_STAND_FIRST( move_char( ch, door ) );
         
       for ( vch = in_room->people; vch != NULL; vch = vch->next_in_room )
       {
        if ( vch == ch )
	  continue;

	a = get_aggr_factor( ch, vch );

	if ( a > agg )
	{
	  agg = a;
	  ach = vch;
	}
      }

      if ( agg != 0 )
      {
        CH_STAND_FIRST( one_hit( ch, ach, TYPE_UNDEFINED ) );
        continue;
      }
    }

	  continue;
	}
      }
    
/****  end aggression  ****/

    if ( ch->position != ch->pIndexData->start_pos )
    {
      switch( ch->pIndexData->start_pos )
      {
        case POS_SLEEPING:
          if ( ch->hit[HIT_LOC_BODY] != ch->max_hit )
          {
            if ( ch->position != POS_RESTING )
            {
              act("$n lies down on the ground to rest.",
                ch, NULL, NULL, NULL, NULL, TO_ROOM, TRUE );
              ch->position = POS_RESTING;
            }
          }
          else
          {
            act("$n lies down and falls to sleep.",
              ch, NULL, NULL, NULL, NULL, TO_ROOM, TRUE );
            ch->position = POS_SLEEPING;
          }
          break;
        case POS_RESTING:
          act("$n lies down on the ground to rest.",
            ch, NULL, NULL, NULL, NULL, TO_ROOM, TRUE );
          ch->position = POS_RESTING;
          break;
        case POS_SITTING:
          act("$n sits down.",
            ch, NULL, NULL, NULL, NULL, TO_ROOM, TRUE );
          ch->position = POS_SITTING;
          break;
        case POS_STANDING:
          act("$n stands up.", ch, NULL, NULL, NULL, NULL, TO_ROOM, TRUE );
          ch->position = POS_STANDING;
          break;
        default:
          act("$n resumes $s activies.",
            ch, NULL, NULL, NULL, NULL, TO_ROOM, TRUE );
          ch->position = ch->pIndexData->start_pos;
          break;
      }
    }

    /* Scavenge */
    if ( IS_SET(ch->act, ACT_SCAVENGER)
    &&   ch->in_room->contents != NULL
    &&   number_bits( 5 ) == 0 )
    {
      OBJ_DATA *obj;
      OBJ_DATA *obj_best = NULL;
      int max = 0;

      for ( obj = ch->in_room->contents; obj; obj = obj->next_content )
      {
	if ( CAN_WEAR(obj, ITEM_TAKE) && obj->cost > max )
	{
          obj_best    = obj;
          max         = obj->cost;
	}
      }

      if ( obj_best )
      {
	obj_from_room( obj_best );
	obj_to_char( obj_best, ch );
	act( "$n gets $p.",
	  ch, NULL, obj_best, NULL, NULL, TO_ROOM, SENSE_SIGHT );
	WAIT_STATE( ch, 20 );
	continue;
      }
    }

    /* Wander */
    if ( !IS_SET(ch->act, ACT_SENTINEL)
/* raised from 7 to 8 after lowering pulse to .25 */
    && ( door = number_bits( 8 ) ) <= 5
    && ( pexit = ch->in_room->exit[door] ) != NULL
    &&   pexit->to_room != NULL
    &&   !IS_SET(pexit->exit_flags, EX_CLOSED)
    &&   !IS_SET(pexit->to_room->room_flags, ROOM_NO_MOB)
    && ( !IS_SET(ch->act, ACT_STAY_AREA)
    ||   pexit->to_room->area == ch->in_room->area ) )
    {
      move_char( ch, door );
      WAIT_STATE( ch, 16 );
    }

  }

  return;
}



void knockout_update (void )
{
    DESCRIPTOR_DATA *d;
    CHAR_DATA *vch;
    AFFECT_DATA *paf;
    AFFECT_DATA *paf_next;

    for ( d = descriptor_list; d != NULL; d = d->next )
     {
     
     
     if ( d->connected != CON_PLAYING )
     return;

     vch = d->character;

     if ( vch->stun < 0 )
     {
         if ( IS_AFFECTED(vch, AFF_FLYING) )
         {
          
             for ( paf = vch->affected ; paf != NULL ; paf = paf_next )
             {
               paf_next = paf->next;

                if ( paf->type > 0 && paf->type < MAX_SKILL
                && ( skill_table[paf->type].target == TAR_CHAR_SELF
                || skill_table[paf->type].target == TAR_CHAR_DEFENSIVE ) )
                affect_remove( vch, paf );
              }
               send_to_char("You feel your magick fade\n\r",vch);
               return;
         }


         send_to_char("You black out.\n\r",vch);
         vch->stun = 0;
         clear_dynamic(vch);
         vch->position = POS_SLEEPING;
     }
}
}

/*
void fall_update( void )
{
    DESCRIPTOR_DATA *d;
    ROOM_INDEX_DATA *in_room;
    ROOM_INDEX_DATA *to_room;
    EXIT_DATA *pexit;
    CHAR_DATA *vch;
    

     for ( d = descriptor_list; d; d = d->next )
     {
     
     
     if ( d->connected != CON_PLAYING )
     return;

     in_room = d->character->in_room;

     vch = d->character;

     if (!IS_SET(vch->in_room->sector->flags, SECTOR_AIR) )
     return;

    

    if ((IS_SET(vch->in_room->sector->flags, SECTOR_AIR)
      ||   IS_SET( to_room->sector->flags, SECTOR_AIR))
      &&  !IS_SET(vch->act, PLR_HOLYWALK)) {

    if ( IS_AFFECTED( vch, AFF_FLYING ) )
    return;

    if ( IS_SET(vch->act, PLR_CLIMBING ) )
    return;

    
   
    if ( ( pexit   = in_room->exit[DIR_DOWN] ) != NULL
    ||   ( to_room = pexit->to_room ) == NULL )
    {
          to_room = in_room->exit[DIR_DOWN]->to_room;
          send_to_char( "You plummet to the ground below...\n\r", vch );
          act( "$n plummets to the ground below.", vch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIGHT );
          char_from_room( vch );
          char_to_room( vch, to_room, TRUE );
          do_look(vch, "auto" );
          act("$n falls in from above.", vch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIGHT );
          vch->hit[HIT_LOC_BODY] -= 35;
          
          vch->position = POS_RESTING;
          
          if ( vch->stun >= 40 )
          {
          vch->stun -= 40;
          continue;
          }
          

          if ( vch->hit[HIT_LOC_BODY] <= -15 )
          {
          raw_kill(vch);
          SET_BIT(vch->act, PLR_DEAD);
          //save_char_obj(vch);    
          extract_char( vch, TRUE );
          return;
          }
          return;
 }

}
}
}

*/


void fall_update( void )
{
    
    ROOM_INDEX_DATA *in_room;
    ROOM_INDEX_DATA *to_room;
    CHAR_DATA *vch;
    EXIT_DATA *pexit;
    CHAR_DATA *ch;
    CHAR_DATA *ch_next;
    CHAR_DATA *ch_save;
    CHAR_DATA *ch_quit;
    time_t save_time;

    save_time	= current_time;
    ch_save	= NULL;
    ch_quit	= NULL;
    for ( ch = char_list; ch != NULL; ch = ch_next )
    {
	

	ch_next = ch->next;

	/*
	 * Find dude with oldest save time.
	 */
	if ( !IS_NPC(ch)
	&& ( ch->desc == NULL || ch->desc->connected == CON_PLAYING )
	&&   ch->save_time < save_time )
	{
	    ch_save	= ch;
	    save_time	= ch->save_time;
	}

     
     
     

     in_room = ch->in_room;

     vch = ch;

     if (!IS_SET(vch->in_room->sector->flags, SECTOR_AIR) )
     return;

    

    if ((IS_SET(vch->in_room->sector->flags, SECTOR_AIR)
      ||   IS_SET( to_room->sector->flags, SECTOR_AIR))
      &&  !IS_SET(vch->act, PLR_HOLYWALK)) {

    if ( IS_AFFECTED( vch, AFF_FLYING ) )
    return;

    /*if ( IS_SET(vch->act, PLR_CLIMBING ) )
    return;*/

/*    ||   ( to_room = pexit->to_room ) == NULL )*/
   
    if ( ( pexit   = in_room->exit[DIR_DOWN] ) != NULL )
    
    {
          to_room = in_room->exit[DIR_DOWN]->to_room;
          send_to_char( "You plummet to the ground below...\n\r", vch );
          act( "$n plummets to the ground below.", vch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIGHT );
          char_from_room( vch );
          char_to_room( vch, to_room, TRUE );
          do_look(vch, "auto" );
          act("$n falls in from above.", vch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIGHT );
          vch->hit[HIT_LOC_BODY] -= 35;
          
          vch->position = POS_RESTING;
          
          if ( vch->stun >= 40 )
          {
          vch->stun -= 40;
          continue;
          }
          

          if ( vch->hit[HIT_LOC_BODY] <= -15 )
          {
          raw_kill(vch);
          SET_BIT(vch->act, PLR_DEAD);
          //save_char_obj(vch);    
          //extract_char( vch, TRUE );
          return;
          }
          return;
 }

}
}
}


/*
 * Update all chars, including mobs.
 * This function is performance sensitive.
 */

void char_update( void )
{   
    CHAR_DATA *ch;
    CHAR_DATA *ch_next;
    CHAR_DATA *ch_save;
    CHAR_DATA *ch_quit;
    time_t save_time;

    save_time	= current_time;
    ch_save	= NULL;
    ch_quit	= NULL;
    for ( ch = char_list; ch != NULL; ch = ch_next )
    {
	sh_int light, vision;

	ch_next = ch->next;

	/*
	 * Find dude with oldest save time.
	 */
	if ( !IS_NPC(ch)
	&& ( ch->desc == NULL || ch->desc->connected == CON_PLAYING )
	&&   ch->save_time < save_time )
	{
	    ch_save	= ch;
	    save_time	= ch->save_time;
	}

       

	if ( ch->position >= POS_STUNNED && IS_NPC( ch ) )
	{
	    if ( ch->hit[HIT_LOC_BODY]  < ch->max_hit )
		ch->hit[HIT_LOC_BODY] = UMIN( ch->max_hit, ch->hit[HIT_LOC_BODY] + get_curr_tou( ch )/5 );

	    if ( ch->mana < ch->max_mana )
		ch->mana = UMIN( ch->max_mana, ch->mana + get_curr_int( ch )/3);

	    if ( ch->move < ch->max_move )
		ch->move = UMIN( ch->max_move, ch->move + get_curr_dex( ch )/5);

            if ( ch->stun < ch->max_stun )
                ch->stun = UMIN( ch->max_stun, ch->stun + get_curr_tou( ch)/3);
	}

	if ( ch->position == POS_STUNNED )
	    update_pos( ch );

	if ( !IS_NPC(ch) && ch->level < LEVEL_IMMORTAL )
	{
	    OBJ_DATA *obj;

	    if ( ( obj = get_eq_char( ch, gn_wear_secondary ) ) != NULL
	    &&   obj->item_type == ITEM_LIGHT
	    &&   obj->value[2] >= 0
	    &&   obj->value[0] != 0 )
	    {
		if ( obj->value[2] == 0 && ch->in_room != NULL )
		{
		    ch->in_room->light_mod -= (obj->value[0]/2);
		    ch->light -= (obj->value[0]/2);
		    obj->value[0] -= (obj->value[0]/2);
		    if ( obj->value[0] <= 1 && obj->value[0] >= -1 )
		    {
			ch->in_room->light_mod -= obj->value[0];
			ch->light -= obj->value[0];
			obj->value[0] = 0;
			act( "$p goes out.", ch, NULL, obj, NULL, NULL, TO_ROOM, SENSE_SIGHT );
			act( "$p goes out.", ch, NULL, obj, NULL, NULL, TO_CHAR, SENSE_SIXTH );
		    }
		    else
		    {
			act( "$p fades slowly.", ch, NULL, obj, NULL, NULL, TO_ROOM, SENSE_SIGHT );
			act( "$p fades slowly.", ch, NULL, obj, NULL, NULL, TO_CHAR, SENSE_SIXTH );
		    }
		}
		else
		    obj->value[2]--;
	    }
	    ch->timer++;
/* no void for now */
/*
	    if ( ++ch->timer >= 12 )
	    {
		if ( ch->was_in_room == NULL && ch->in_room != NULL )
		{
		    ch->was_in_room = ch->in_room;
		    if ( ch->fighting != NULL )
			stop_fighting( ch, TRUE );
		    act( "$n disappears into the void.",
			ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIGHT );
		    send_to_char( "You disappear into the void.\n\r", ch );
		    save_char_obj( ch );
		    char_from_room( ch );
		    char_to_room( ch, get_room_index( ROOM_VNUM_LIMBO ), FALSE );
		}
	    }
*/

/* no auto-quit for now */
/*
	    if ( ch->timer > 30 )
		ch_quit = ch;
*/

	    gain_condition( ch, COND_DRUNK,  -1 );
	    gain_condition( ch, COND_FULL,   -1 );
	    gain_condition( ch, COND_THIRST, -1 );
	}

	/* adjust vision */
      if ( IS_NPC(ch) )
      {
	int mod;

	vision = ch->vision_current;
	light = room_light( ch->in_room );
	if ( vision != light
	&& ((vision > ch->vision_min && vision < ch->vision_max )
	|| ( ch->vision_min == ch->vision_max ) ) )
	{
	    mod = light - vision;

	    if ( vision < ch->vision_min )
		mod = URANGE( 0, mod, 50 );
	    else
		mod = URANGE( -50, mod, 0 );

	    ch->vision_current = vision + mod;
	}
      }

	/*
	 * Careful with the damages here,
	 *   MUST NOT refer to ch after damage taken,
	 *   as it may be lethal damage (on NPC).
	 */

        if ( ch->contact != NULL )
        {
           send_to_char("You suffer from mental strain.\n\r",ch);
           
           ch->stun -= 9;

           if ( ch->stun <= 0 )
           {
           
           ch->position = POS_SLEEPING;
           clear_dynamic(ch);
           ch->dynamic_descr = str_dup("lies here, passed out cold.");
           return;
           }
        }

	if ( IS_AFFECTED(ch, AFF_POISON) )
	{
	    damage( ch, ch, 2, gsn_poison, NULL, gn_wear_none );
	}
	else if ( ch->position == POS_INCAP )
	{
	    damage( ch, ch, 1, gsn_bleeding, NULL, gn_wear_none );
	}
	else if ( ch->position == POS_MORTAL )
	{
	    damage( ch, ch, 2, gsn_bleeding, NULL, gn_wear_none );
	}
    }

    /*
     * Autosave and autoquit.
     * Check that these chars still exist.
     */
    if ( ch_save != NULL || ch_quit != NULL )
    {
	for ( ch = char_list; ch != NULL; ch = ch_next )
	{
	    ch_next = ch->next;
	    if ( ch == ch_save )
		save_char_obj( ch );
	    if ( ch == ch_quit )
		do_quit( ch, "" );
	}
    }

    return;
}


/*
 * Update all player characters.
 */
void pc_update( void )
{
    DESCRIPTOR_DATA *d;
    CHAR_DATA *ch;
    int vision;

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d->connected != CON_PLAYING )
	    continue;

	if ( ( ch = d->character ) == NULL )
	    continue;

	if ( IS_NPC( ch ) )
	    continue;

	if ( ch->position >= POS_STUNNED && !check_fighting( ch ) )
	{
	    if ( ch->hit[HIT_LOC_BODY]  < ch->max_hit )
		ch->hit[HIT_LOC_BODY]  += hit_gain(ch);

		if ( ch->stun < ch->max_stun )
			ch->stun += stun_gain(ch);

	    if ( ch->mana < ch->max_mana )
		ch->mana += mana_gain(ch);

	    if ( ch->move < ch->max_move )
		ch->move += move_gain(ch);
	}

	if ( ch->position == POS_STUNNED )
	    update_pos( ch );


	vision = ch->vision_current;
	if ( room_light( ch->in_room ) > vision && vision < ch->vision_max )
	    ch->vision_current+=2;

	if ( room_light( ch->in_room ) < vision && vision > ch->vision_min )
	    ch->vision_current-=2;
    }

    return;
}


/*
 * Update all objs.
 * This function is performance sensitive.
 */
void obj_update( void )
{   
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;

    for ( obj = object_list; obj != NULL; obj = obj_next )
    {
	CHAR_DATA *rch;
	char *message;

	obj_next = obj->next;

	if ( obj->timer <= 0 || --obj->timer > 0 )
	    continue;

	switch ( obj->item_type )
	{
	default:              message = "$p vanishes.";         break;
	case ITEM_FOUNTAIN:   message = "$p dries up.";         break;
	case ITEM_CORPSE:     message = "$p is eaten by a quivering hoard of maggots."; break;
	case ITEM_FOOD:       message = "$p decomposes.";	break;
	}

	if ( obj->carried_by != NULL )
	{
	    act( message, obj->carried_by, NULL, obj, NULL, NULL, TO_CHAR, SENSE_SIXTH );
	}
	else if ( obj->in_room != NULL
	&&      ( rch = obj->in_room->people ) != NULL )
	{
	    act( message, rch, NULL, obj, NULL, NULL, TO_ROOM, SENSE_SIGHT );
	    act( message, rch, NULL, obj, NULL, NULL, TO_CHAR, SENSE_SIGHT );
	}

	if ( obj->item_type == ITEM_CORPSE )
	    decay_corpse( obj );
	else
	    extract_obj( obj, TRUE );
    }

    return;
}


void decay_corpse( OBJ_DATA *corpse )
{
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    ROOM_INDEX_DATA *in_room = NULL;
    bool fAll = FALSE;

    if ( corpse->carried_by == NULL && corpse->in_room == NULL )
    {
	fAll = TRUE;
        extract_obj( corpse, TRUE );
	return;
    }
    else if ( corpse->carried_by != NULL )
    {
	if ( corpse->carried_by->in_room == NULL )
	    fAll = TRUE;
	else
	    in_room = corpse->carried_by->in_room;
    }
    else
    {
	in_room = corpse->in_room;
    }

    for ( obj = corpse->contains ; obj != NULL ; obj = obj_next )
    {
	obj_next = obj->next_content;

	obj_from_obj( obj );
	obj_to_room( obj, in_room, TRUE );
    }

    extract_obj( corpse, TRUE );

    return;
}



/*
 * Handle all kinds of updates.
 * Called once per pulse from game loop.
 * Random times to defeat tick-timing clients and players.
 */
void update_handler( void )
{
    static  int     pulse_area;
    static  int     pulse_mobile;
    static  int     pulse_violence;
    static  int     pulse_weather;
    static  int     pulse_point;
    static  int     pulse_pc_update;
    static  int     pulse_environment;
    static  int     pulse_fall;
    static  int     pulse_knockout;

    if ( --pulse_area     <= 0 )
    {
	pulse_area	= PULSE_AREA;
	area_update	( );
    }

    if ( --pulse_mobile   <= 0 )
    {
	pulse_mobile	= PULSE_MOBILE;
	mobile_update	( );
    }

    if ( --pulse_violence <= 0 )
    {
	pulse_violence	= PULSE_VIOLENCE;
	violence_update	( );
    }

    if ( --pulse_weather <= 0 )
    {
	pulse_weather = PULSE_WEATHER;
	weather_update ( ); // Was commented out TEST
	obj_update	( );
    }

    if ( --pulse_environment <= 0 )
    {
      pulse_environment = PULSE_ENVIRONMENT;
      environment_interval++;
      write_environment( FALSE );
    }

    if ( --pulse_point    <= 0 )
    {
	pulse_point     = PULSE_TICK;
	char_update	( );
    }
  
    if ( --pulse_fall <= 0 )
    {
    pulse_fall = PULSE_FALL;
    fall_update     ();
    }

    if ( --pulse_knockout <= 0 )
    {
    pulse_knockout = PULSE_KNOCKOUT;
    knockout_update      ();
    }

    if ( --pulse_pc_update <= 0 )
    {
	pulse_pc_update = PULSE_PC_UPDATE;
	pc_update ( );
    }

    tail_chain( );
    return;
}
