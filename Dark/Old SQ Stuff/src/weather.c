/* Advanced weather Code */

#if defined(macintosh)
#include <types.h>
#include <time.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "merc.h"
#include "macros.h"


void small_storm args( ( void ) );
void med_sand_storm  args( ( void ) );         
void big_storm args( ( void ) );
void energy_storm args( ( void ) );
void light_sand  args( ( void ) );
void fog args( ( void ) );

void weather_update   args( ( void ) );
extern char *target_name;
#define LI1		MAX_LEVEL/5
#define LI2		MAX_LEVEL/3
#define LI3		MAX_LEVEL
/*
 * Update the weather.
 */
void weather_update( void )
{
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;

    buf[0] = '\0';

    switch ( ++time_info.hour )
    {
    case  5:
	weather_info.sunlight = SUN_LIGHT;
//	strcat( buf, "The sweltering heat can be felt as the day slowly begins.\n\r" );
	break;

    case  6:
	weather_info.sunlight = SUN_RISE;
	strcat( buf, "The fiery sun rises over the eastern horizon to bath Athas in a broiling heat.\n\r" );
	break;

    case 19:
	weather_info.sunlight = SUN_SET;
	strcat( buf, "The last rays of the sun begin to set beyond the ringing mountains in the west.\n\r" );
	break;

    case 20:
	weather_info.sunlight = SUN_DARK;
	strcat( buf, "The gloomy chill of night has begun.\n\r" );
	break;

    case 24:
	time_info.hour = 0;
	time_info.day++;
	break;

    }

    if ( time_info.day   >= 35 )
    {
	time_info.day = 0;
	time_info.month++;
    }

    if ( time_info.month >= 12 )
    {
	time_info.month = 0;
	time_info.year++;
    }

    /*
     * Weather change.
     */
    switch ( weather_info.sky )
    {
    default: 
	    weather_info.sky = SKY_CLOUDLESS;
	break;

    case SKY_CLOUDLESS:
if ((number_chance(10))&&  (time_info.hour <= 6) )
      {
	    weather_info.sky = SKY_FOGGY;
     }
      else  if (number_chance(15))
      {
	    weather_info.sky = SKY_CLOUDY;
		strcat( buf, "Clouds begin to fill the sky.\n\r" );
     }
      else   if (number_chance(30))
      {
	    weather_info.sky = SKY_LIGHT_SAND;
     }
else
      if (number_chance(45))
      {
            weather_info.sky = SKY_CLOUDLESS;
     }


	break;

    case SKY_CLOUDY:
      if (number_chance(15))
	{
	    weather_info.sky = SKY_SMALL_STORM;
	}
	else
      if (number_chance(15))
	{
	    weather_info.sky = SKY_MED_SAND_STORM;
	}
	else
      if (number_chance(15))
	{
	    weather_info.sky = SKY_ENERGY_STORM;
	}
	else
      if (number_chance(15))
	{
	    weather_info.sky = SKY_BIG_STORM;
	}      
else      if (number_chance(15))
	{
	    weather_info.sky = SKY_CLOUDLESS;
	}
else if (number_chance(25))
	{
	    weather_info.sky = SKY_CLOUDY;
	}
    case SKY_LIGHT_SAND:
      if (number_chance(15))
        {
            weather_info.sky = SKY_SMALL_STORM;
            small_storm       ( );
        }
      else
      if (number_chance(10))
        {
            weather_info.sky = SKY_MED_SAND_STORM;
	    med_sand_storm();
        }
      else
      if (number_chance(10))
        {
            weather_info.sky = SKY_ENERGY_STORM;
            energy_storm();
        }
      else
      if (number_chance(10))
        {
            weather_info.sky = SKY_CLOUDY;
        }
else  if (number_chance(55))
        {
            weather_info.sky = SKY_LIGHT_SAND;
        }
        break;

    case SKY_MED_SAND_STORM:
if (number_chance(15))
	{
	    weather_info.sky = SKY_BIG_STORM;
	}
else if (number_chance(15))
	{
	    weather_info.sky = SKY_CLOUDY;
	}
else if (number_chance(15))
	{
	    weather_info.sky = SKY_SMALL_STORM;
	}
	else 
 if (number_chance(55))
	{
	    weather_info.sky = SKY_MED_SAND_STORM;
            med_sand_storm();
	}

	break;
    case SKY_SMALL_STORM:
if (number_chance(15))
	{
	    weather_info.sky = SKY_ENERGY_STORM;
          }
else if (number_chance(15))
	{
	    weather_info.sky = SKY_SMALL_STORM;
	}
else if (number_chance(15))
	{
	    weather_info.sky = SKY_CLOUDY;
	}
else if (number_chance(15))
	{
	    weather_info.sky = SKY_MED_SAND_STORM;
          }
else if (number_chance(40))
	{
	    weather_info.sky = SKY_LIGHT_SAND;
          }
	break;
    case SKY_FOGGY:
 if (number_chance(45))
	{
 weather_info.sky = SKY_CLOUDY;
	}
else if (number_chance(55))
	{
        fog();
  weather_info.sky = SKY_FOGGY;
}
  break;
	


    case SKY_BIG_STORM:
 if (number_chance(15))
	{
	    weather_info.sky = SKY_CLOUDY;
	}
 else
if (number_chance(30))
	{
	    weather_info.sky = SKY_SMALL_STORM;
          }
      else
 if (number_chance(55))
	{
	    weather_info.sky = SKY_BIG_STORM;
	big_storm();
	}
	break;
	}
if ( buf[0] != '\0' )
    {
	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    if ( d->connected == CON_PLAYING
	    &&   IS_OUTDOORS(d->character)
	    &&   IS_AWAKE(d->character) )
		send_to_char( buf, d->character );
	}
    }

    return;
}
	







void do_weather( CHAR_DATA *ch, char *argument )
{
    send_to_char("The weather command is no longer available. If you wish to see the weather, look at the sky.\n\r",ch);
	return;

} 




bool IS_OUTDOORS( CHAR_DATA *ch )
{
    if ( !IS_SET(ch->in_room->sector->flags, SECTOR_OUTDOORS ) )
        return FALSE;
    else
        return TRUE;
}

/*void do_autoweather(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;

    if (IS_SET(ch->act,PLR_AUTOWEATHER))
    {
   send_to_char("You will no longer see weather descriptions in rooms\n\r",ch);
      REMOVE_BIT(ch->act,PLR_AUTOWEATHER);
    }
    else
    {
      send_to_char("You will now see weather descriptions in rooms\n\r",ch);
      SET_BIT(ch->act,PLR_AUTOWEATHER);
    }
}*/

void show_weather(CHAR_DATA *ch)
{
    char buf  [MAX_STRING_LENGTH];


        if (weather_info.sky == SKY_SMALL_STORM)
        {
    if ( number_range( 0, 100 ) <= 15  )
{
        send_to_char("Small, eroding sand sifts through the air.",ch);
}
else
  if ( number_range( 0, 100 ) <= 30  )
{
        send_to_char("Small, eroding sand sifts through the air.",ch);
}
else   if ( number_range( 0, 100 ) <= 45  )
{
        send_to_char("Sands swirl around, causing dust spirals and gritty biting sensations at times.",ch); 
}

else
  if ( number_range( 0, 100 ) <= 60  )
{
        send_to_char("The sand whirls about with a feral tenacity.",ch); 
}
else
  if ( number_range( 0, 100 ) <= 75  )
{
        send_to_char("The wind howls and the sand blows around in gusts",ch); 
}
else   if ( number_range( 0, 100 ) <= 85  )
{
        send_to_char("The wind screams as sand swirls around",ch); 
}
else
  if ( number_range( 0, 100 ) <= 100  )
{
        send_to_char("The wind howls and screams as small grains of sand fly about",ch); 
}
	    	    send_to_char( "\n\r",ch);

        }

       else if (weather_info.sky == SKY_CLOUDY)
        {
        sprintf( buf, "%s.\n\r",
        weather_info.change >= 0
        ? "A warm breeze can be felt about"
        : "A cold breeze can be felt about" );
    send_to_char( buf, ch );
        }
                 else if (weather_info.sky == SKY_CLOUDLESS)
        {
        send_to_char("The olive coloured sky doesn't have a single cloud in it. The Dark Sun shines overhead.\n\r",ch);
        }
                 else if (weather_info.sky == SKY_ENERGY_STORM)
        {
        send_to_char("Arcs of energy sweep across the sky, some striking the ground, crackling through the air.\n\r",ch);
        }
                 else if (weather_info.sky == SKY_BIG_STORM)
        {
        send_to_char("Horrible biting sand flies across the terrain, the winds scream and howl. The sand is so thick that it is impossible to see.\n\r",ch);
        }
                 else if (weather_info.sky == SKY_MED_SAND_STORM)
        {
        send_to_char("A harsh sand flies across the terrain in sheets, making it hard to see.\n\r",ch);
        }
        
                 else if (weather_info.sky == SKY_FOGGY)
        {
        send_to_char("A misty haze covers the horizon.\n\r",ch);
        }

        else if (weather_info.sky == SKY_LIGHT_SAND)
        {
        send_to_char("A light sand lingers in the air, carried by a small gusting breeze. \n\r",ch);
     }
        else 
        {
        send_to_char("BUG :  A descriptor does not exist for this condition  \n\r",ch);
        }
        }


void energy_storm( void )
{
 DESCRIPTOR_DATA *d;
  for ( d = descriptor_list; d != NULL; d = d->next )
        {
            if ( d->connected == CON_PLAYING
                && IS_OUTDOORS( d->character )
                && IS_AWAKE  ( d->character )
                && number_chance(10)
                && !IS_IMMORTAL(d->character)
                && weather_info.sky == SKY_ENERGY_STORM ) {

 send_to_char("You see a brilliant flash come down from the sky and then black out!\n\r",d->character);
 act( "A bolt of blue energy flashes down from the sky and strikes $n!", d->character, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIXTH);
 if (d->character->level > 0 ) {
 if(d->character->fighting) {
stop_fighting(d->character,TRUE); }


d->character->hit[HIT_LOC_BODY] -= 25;
d->character->position = POS_INCAP;
WAIT_STATE(d->character,40); 
 }
			} 
  }
}

void big_storm( void )
{
 DESCRIPTOR_DATA *d;
  for ( d = descriptor_list; d != NULL; d = d->next )
        {
            if ( d->connected == CON_PLAYING
                && IS_OUTDOORS( d->character )
                && weather_info.sky == SKY_BIG_STORM )
 {
  if ( number_range( 0, 2000 ) <= 1000  )
      {
send_to_char("The winds howl and push you around, making you stumble \n\r",d->character);
d->character->move -= 5;
     }
else
  if ( number_range( 0, 2000 ) <= 1250  )
      {
send_to_char("You loose your balance in a large gust and collapse to the ground. \n\r",d->character );
 act( "$n looses $s footing and falls to the ground.", d->character, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIXTH);
d->character->hit[HIT_LOC_BODY] -= 5;
d->character->move -= 10;
d->character->position = POS_RESTING;
     }
else
  if ( number_range( 0, 2000 ) <= 1500  )
      {
send_to_char("You tumble over a pile of sand and go careening forward, smashing into something. \n\r",d->character);
 act( "$n tumbles over a small pile of sand and goes careening forward, smashing into something hard.", d->character, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIXTH);
d->character->hit[HIT_LOC_BODY] -= 15;
d->character->move -= 15;
}
else
  if ( number_range( 0, 2000 ) <= 1950  )
      {
send_to_char("The winds howl and rage, forcing you back. \n\r",d->character);
     }
else
  if ( number_range( 0, 2000 ) <= 2000  )
      {
send_to_char("The sand finally takes it's affect on you, sapping you of your strength as you collapse to the ground. \n\r", d->character);
 act( "$n collapses to the ground in a heap.", d->character, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIXTH);
d->character->hit[HIT_LOC_BODY] -= 40;
d->character->move -= 50;
     }
} }
}

void small_storm( void )
{
 DESCRIPTOR_DATA *d;
  for ( d = descriptor_list; d != NULL; d = d->next )
        {

	  if ( d->connected == CON_PLAYING && IS_OUTDOORS( d->character )
                && weather_info.sky == SKY_BIG_STORM )
      {
	  send_to_char("A gust of wind hits you but dies away quickly.\n\r",d->character );
	  d->character->move -=1;
	  return;
	  }
	  
  }
}

void fog(void )
{
 DESCRIPTOR_DATA *d;
 for ( d = descriptor_list; d != NULL; d = d->next )
     {

	 if ( d->connected == CON_PLAYING && IS_OUTDOORS( d->character )
                && weather_info.sky == SKY_BIG_STORM )

      {
       send_to_char("This is you getting lost in fog, but it's not in just yet\n\r",d->character );
	   return;
	 }
	 
  }
}

void light_sand( void )
{
 DESCRIPTOR_DATA *d;
  for ( d = descriptor_list; d != NULL; d = d->next )
        {

	  if ( d->connected == CON_PLAYING && IS_OUTDOORS( d->character )
                && weather_info.sky == SKY_BIG_STORM )

      {
      d->character->move -= 2;
	  send_to_char("This is you loosing some movement points, but it's not in yet.. oh wait, you lost movement!\n\r",d->character );
	  }
  }
}

void med_sand_storm( void )
{
 DESCRIPTOR_DATA *d;
  for ( d = descriptor_list; d != NULL; d = d->next )
        {

	  if ( d->connected == CON_PLAYING && IS_OUTDOORS( d->character )
                && weather_info.sky == SKY_BIG_STORM )

      {
 
	  send_to_char("This is you getting lost or maybe even hurt in the sandstorm, but it's not in yet, sorry.\n\r",d->character );

	  }
  }
}



bool number_chance(int num)
{
    if (number_range(1,100) <= num) return TRUE;
    else return FALSE;
}

/*void spell_control_weather(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    if ( !str_cmp( target_name, "better" ) )
{

if (weather_info.sky == SKY_CLOUDLESS)
{
send_to_char("But it's so beautiful outside already\n\r",ch);
return;
}
else
if (weather_info.sky == SKY_CLOUDY)
{
send_to_char("You recite the ancient spell and the clouds part in obedience\n\r",ch);
act( "$n makes a strange movement with their hands and the clouds part.", ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIXTH);
weather_info.sky = SKY_CLOUDLESS;
return;
}
else
if (weather_info.sky == SKY_RAINING)
{
send_to_char("You recite the ancient spell and the  rain stops in obedience\n\r",ch);
act( "$n makes a strange movement with their hands and the rain stops.", ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIXTH);
weather_info.sky = SKY_CLOUDY;
return;
}
else
if (weather_info.sky == SKY_SMALL_STORM)
{
send_to_char("You recite the ancient spell and the small_storm ceases in obedience\n\r",ch);
act( "$n makes a strange movement with their hands and the small_storm stops.", ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIXTH);
weather_info.sky = SKY_THUNDERSTORM;
return;
}
else
if (weather_info.sky == SKY_THUNDERSTORM)
{
send_to_char("You recite the ancient spell and the storm ceases\n\r",ch);
act( "$n makes a strange movement with their hands and the  storm ceases.", ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIXTH);
weather_info.sky = SKY_RAINING;
return;
}
else
if (weather_info.sky == SKY_SNOWING)
{
send_to_char("You recite the ancient spell and the snow ceases in obedience\n\r",ch);
act( "$n makes a strange movement with their hands and the snow ceases.", ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIXTH);
weather_info.sky = SKY_CLOUDY;
return;
}
else
if (weather_info.sky == SKY_BLIZZARD)
{
send_to_char("You recite the ancient spell and the  horizon clears\n\r",ch);
act( "$n makes a strange movement with their hands and the horizon clears.", ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIXTH);
weather_info.sky = SKY_SNOWING;
return;
}
else
if (weather_info.sky == SKY_ICESTORM)
{
send_to_char("You recite the ancient spell and the  horizon clears\n\r",ch);
act( "$n makes a strange movement with their hands and the horizon clears.", ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIXTH);
weather_info.sky = SKY_RAINING;
return;
}
else
if (weather_info.sky == SKY_HAILSTORM)
{
send_to_char("You recite the ancient spell and the  horizon clears\n\r",ch);
act( "$n makes a strange movement with their hands and the horizon clears.", ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIXTH);
weather_info.sky = SKY_RAINING;
return;
}
else
if (weather_info.sky == SKY_FOGGY)
{
send_to_char("You recite the ancient spell and the  horizon clears\n\r",ch);
act( "$n makes a strange movement with their hands and the horizon clears.", ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIXTH);
weather_info.sky = SKY_CLOUDY;
return;
}
else 
{
            send_to_char("Bad Weather Call. Please notify the imms.\n\r",ch);
}

}
    else if ( !str_cmp( target_name, "worse" ) )
{

if (weather_info.sky == SKY_CLOUDLESS)
{
send_to_char("You recite the ancient spell and the clouds  come at your command.\n\r",ch);
act( "$n makes a strange movement with their hands and the clouds  darken the sky.", ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIXTH);
weather_info.sky = SKY_CLOUDY;
return;
}
else
if (weather_info.sky == SKY_CLOUDY)
{
send_to_char("You recite the ancient spell and the clouds trickle down rain\n\r",ch);
act( "$n makes a strange movement with their hands and the clouds open up to rain.", ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIXTH);
weather_info.sky = SKY_RAINING;
return;
}
else
if (weather_info.sky == SKY_RAINING)
{
send_to_char("You recite the ancient spell and the  rain  turns to hail\n\r",ch);
act( "$n makes a strange movement with their hands and the rain  turns to hail.", ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIXTH);
weather_info.sky = SKY_HAILSTORM;
return;
}
else
if (weather_info.sky == SKY_SMALL_STORM)
{
send_to_char("You recite the ancient spell and the  clouds send down sheets of ice\n\r",ch);
act( "$n makes a strange movement with their hands and the small_storm turns to ice.", ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIXTH);
weather_info.sky = SKY_ICESTORM;
return;
}
else
if (weather_info.sky == SKY_THUNDERSTORM)
{
send_to_char("You recite the ancient spell and the clouds clap in thunderous approval\n\r",ch);
act( "$n makes a strange movement with their hands and the  clouds clap in thunder.", ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIXTH);
weather_info.sky = SKY_SMALL_STORM;
return;
}
else
if (weather_info.sky == SKY_SNOWING)
{
send_to_char("You recite the ancient spell and the snow increases in obedience\n\r",ch);
act( "$n makes a strange movement with their hands and the snow turns to a blizzard.", ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIXTH);
weather_info.sky = SKY_BLIZZARD;
return;
}
else
if (weather_info.sky == SKY_BLIZZARD)
{
send_to_char("It's already as bad as it can get\n\r",ch);
return;
}
else
if (weather_info.sky == SKY_ICESTORM)
{
send_to_char("It's already as bad as it can get\n\r",ch);
return;
}
else
if (weather_info.sky == SKY_HAILSTORM)
{
send_to_char("It's already as bad as it can get\n\r",ch);
return;
}
else
if (weather_info.sky == SKY_FOGGY)
{
send_to_char("You recite the ancient spell and the  horizon clears\n\r",ch);
act( "$n makes a strange movement with their hands and the horizon clears.", ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIXTH);
weather_info.sky = SKY_CLOUDY;
return;

}
else 
{
            send_to_char("Bad Weather Call. Please notify the imms.\n\r",ch);
}

}
    else
        send_to_char ("Do you want it to get better or worse?\n\r", ch );

    send_to_char( "Ok.\n\r", ch );
    return;
}*/


void do_wset( CHAR_DATA *ch, char *argument )
{


char arg1 [MAX_INPUT_LENGTH];
argument = one_argument( argument, arg1 );
    if ( arg1[0] == '\0' )
    {
        send_to_char( "Syntax:\n\r",ch);
        send_to_char( "  set weather <condition> \n\r", ch);
        send_to_char( "  Condition can be :\n\r",ch);
        send_to_char("   cloudless cloudy smallstorm sandstorm\n\r",ch);
        send_to_char("   badsandstorm energystorm fog lightsand  \n\r",ch);
       return;

    }
 if(!str_cmp(arg1,"cloudless"))
{
send_to_char( "You wave your hands and in reverence to you, the clouds dissapear \n\r", ch);
          weather_info.sky = SKY_CLOUDLESS;
act( "$n makes a strange movement with their hands and the clouds part.", ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIXTH);

}
else
 if(!str_cmp(arg1,"fog"))
{
send_to_char( "You wave your hands and in reverence to you, a mist vapors the horizon \n\r", ch);
          weather_info.sky = SKY_FOGGY;
act( "$n makes a strange movement with $s hands and a mist vapors the horizon.", ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIXTH);
}
else
 if(!str_cmp(arg1,"energystorm"))
{
send_to_char( "You wave your hands and in reverence to you, energy begins to dance across the sky \n\r", ch);
          weather_info.sky = SKY_ENERGY_STORM;
act( "$n makes a strange movement with $s hands and energy begins to dance across the sky.", ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIXTH);
}
else
 if(!str_cmp(arg1,"smallstorm"))
{
send_to_char( "You wave your hands and in reverence to you, the winds pick up and blow gritty sand around. \n\r", ch);
          weather_info.sky = SKY_SMALL_STORM;
act( "$n makes a strange movement with $s hands, causing the winds to pick up and blow gritty sand around.", ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIXTH);
}
else
 if(!str_cmp(arg1,"sandstorm"))
{
send_to_char( "You wave your hands and in reverence to you, a good size sandstorm blows across the land\n\r", ch);
          weather_info.sky = SKY_MED_SAND_STORM;
act( "$n makes a strange movement with $s hands and a sandstorm passes over the land.", ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIXTH);
}
else
 if(!str_cmp(arg1,"badsandstorm"))
{
send_to_char( "You wave your hands and in reverence to you, a massive sandstorm rips across the lands.\n\r", ch);
          weather_info.sky = SKY_BIG_STORM;
act( "$n makes a strange movement with $s hands and a massive sandstorm rips across the lands.", ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIXTH);
}
else
 if(!str_cmp(arg1,"lightsand"))
{
send_to_char( "You wave your hands and in reverence to you, a breeze picks up some sand and carries it on the wind\n\r", ch);
          weather_info.sky = SKY_LIGHT_SAND;
act( "$n makes a strange movement with $s hands, and a breeze picks up some sand and carries it on the wind.", ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIXTH);
}
else
 if(!str_cmp(arg1,"cloudy"))
{
send_to_char( "You wave your hands and in reverence to you, clouds cover the horizon, threatening an energy storm\n\r", ch);
          weather_info.sky = SKY_CLOUDY;
act( "$n makes a strange movement with their hands and clouds cover the horizon, threatening an energy storm", ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIXTH);
}
/* okay, we haven't met any conditions so far. Did they forget? */
else
    {
        send_to_char( "Syntax:\n\r",ch);
        send_to_char( "  set weather <condition> \n\r", ch);
        send_to_char( "  Condition can be :\n\r",ch);
        send_to_char("   cloudless cloudy smallstorm sandstorm\n\r",ch);
        send_to_char("   badsandstorm energystorm fog lightsand  \n\r",ch);
        return;
    }

}
