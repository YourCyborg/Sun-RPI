
#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"


void do_classupdate (CHAR_DATA *ch)
{

/* For now, set everyone to fighters */

ch->class = 2;



    if ( ch->class == 0 )
    {
        return;
    }

    if (ch->class == 1 )
    {

        ch->pcdata->learned[gsn_invis] = 60;
        ch->pcdata->power_level[gsn_invis] = 50;
        return;
    }

    if ( ch->class == 2 )
    {
        /* BASH SKILL */
        ch->pcdata->learned[gsn_bash] = 10;
        ch->pcdata->power_level[gsn_bash] = 20;

        /* DISARM SKILL */
        ch->pcdata->learned[gsn_disarm] = 35;
        ch->pcdata->power_level[gsn_disarm] = 30;

       /* SLASH PROFICIENCY */
        ch->pcdata->learned[gsn_slash_proficiency] = 15;
        ch->pcdata->power_level[gsn_slash_proficiency] = 40;

       /* BLUDGEON PROFICIENCY */
        ch->pcdata->learned[gsn_blud_proficiency] = 15;
        ch->pcdata->power_level[gsn_blud_proficiency] = 50;
 
       /* RESCUE SKILL */
        ch->pcdata->learned[gsn_rescue] = 10;
        ch->pcdata->power_level[gsn_rescue] = 35;

       /* MARTIAL ARTS */
        ch->pcdata->learned[gsn_martial_arts] = 40;
        ch->pcdata->power_level[gsn_martial_arts] = 50;
 
       /* PIERCE PROFICIENCY */
        ch->pcdata->learned[gsn_pierce_proficiency] = 15;
        ch->pcdata->power_level[gsn_pierce_proficiency] = 35;

       /* KICK SKILL */
        ch->pcdata->learned[gsn_kick] = 5;
        ch->pcdata->power_level[gsn_kick] = 40;
       
        return;
    }
if ( ch->class == 3 )
    {
        return;
    }
if ( ch->class == 4 )
    {
        return;
    }
if ( ch->class == 5 )
    {
        return;
    }
if ( ch->class == 6 )
    {
        return;
    }
if ( ch->class == 7 )
    {
        return;
    }
if ( ch->class == 8 )
    {
        return;
    }
if ( ch->class == 9 )
    {
        return;
    }
if ( ch->class == 10 )
    {
        return;
    }
if ( ch->class == 11 )
    {
        return;
    }
if ( ch->class == 12 )
    {
        return;
    }
if ( ch->class == 13 )
    {
        return;
    }
if ( ch->class == 14 )
    {
        return;
    }
if ( ch->class == 15 )
    {
        return;
    }

    
}



