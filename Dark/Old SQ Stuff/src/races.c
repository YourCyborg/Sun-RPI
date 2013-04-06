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

#if !defined(macintosh)
extern	int	_filbuf		args( (FILE *) );
#endif


struct race_data *race_array[MAX_RACE];
int top_race;


/*
 *  Load race data.
 */
void load_races( void )
{
    FILE *fpList;
    FILE *fpRace;
    RACE_DATA *pRace;
    char strRace[MAX_INPUT_LENGTH];
    char filename[MAX_INPUT_LENGTH];
    int i, j;

    top_race = 0;

    sprintf( filename, "%s%s", RACE_DIR, RACE_FILE );

    if ( ( fpList = fopen( filename, "r" ) ) == NULL )
    {
	perror( filename );
	exit( 1 );
    }

    for ( ; ; )
    {
	strcpy( filename, fread_word( fpList ) );
	if ( filename[0] == '$' )
	    break;
	sprintf( strRace, "%s%s", RACE_DIR, filename );

	if ( ( fpRace = fopen( strRace, "r" ) ) == NULL )
	{
	    perror( strRace );
	    exit( 1 );
	}

	pRace = alloc_perm( sizeof(*pRace) );

	pRace->race_name = fread_string( fpRace );
	pRace->hazy_desc = fread_string( fpRace );
	pRace->skin = fread_string( fpRace );
	pRace->prace = top_race;
	pRace->race_bits = fread_number( fpRace );

	pRace->stat_phys_str_mod = fread_number( fpRace );
	pRace->stat_phys_dex_mod = fread_number( fpRace );
	pRace->stat_phys_tou_mod = fread_number( fpRace );
	pRace->stat_phys_qui_mod = fread_number( fpRace );
	pRace->stat_ment_int_mod = fread_number( fpRace );

	pRace->hitnodice = fread_number( fpRace );
	/* d */		fread_letter( fpRace );
	pRace->hitsizedice = fread_number( fpRace );
	/* + */		fread_letter( fpRace );
	pRace->hits_base = fread_number( fpRace );

       pRace->mananodice = fread_number( fpRace );
	/* d */		fread_letter( fpRace );
	pRace->manasizedice = fread_number( fpRace );
	/* + */		fread_letter( fpRace );
	pRace->mana_base = fread_number( fpRace );

       pRace->move_nodice = fread_number( fpRace );
	/* d */		fread_letter( fpRace );
	pRace->movesizedice = fread_number( fpRace );
	/* + */		fread_letter( fpRace );
	pRace->move_base = fread_number( fpRace );

       pRace->stun_nodice = fread_number( fpRace );
        /* d */         fread_letter( fpRace );
        pRace->stunsizedice = fread_number( fpRace );
        /* + */         fread_letter( fpRace );
        pRace->stun_base = fread_number( fpRace );


	

	pRace->vision_min = fread_number( fpRace );
	pRace->vision_max = fread_number( fpRace );
	pRace->vision_range = fread_number( fpRace );

	pRace->heightnodice = fread_number( fpRace );
	/* d */		fread_letter( fpRace );
	pRace->heightsizedice = fread_number( fpRace );
	/* + */		fread_letter( fpRace );
	pRace->height_base = fread_number( fpRace );

	pRace->weightnodice = fread_number( fpRace );
	/* d */		fread_letter( fpRace );
	pRace->weightsizedice = fread_number( fpRace );
	/* + */		fread_letter( fpRace );
	pRace->weight_base = fread_number( fpRace );

	for ( i = 0; i < 10; i++ )
	{
	    pRace->hit_loc_damage[i] = fread_number( fpRace );
	    pRace->hit_loc_location[i] = parse_location( fread_word( fpRace ) );
	    pRace->hit_loc_bits[i] = fread_number( fpRace );
	    pRace->hit_loc_string[i] = fread_string( fpRace );
	}

	for ( i = 0; i < 10; i++ )
	{
	    for ( j = 0; j < 10; j++ )
		pRace->hit_loc_slash[i][j] = fread_number( fpRace );
	}

	for ( i = 0; i < 10; i++ )
	{
	    for ( j = 0; j < 10; j++ )
		pRace->hit_loc_bludgeon[i][j] = fread_number( fpRace );
	}

	for ( i = 0; i < 10; i++ )
	{
	    for ( j = 0; j < 10; j++ )
		pRace->hit_loc_pierce[i][j] = fread_number( fpRace );
	}

	fclose( fpRace );

	race_array[top_race] = pRace;
	top_race++;
    }
    fclose( fpList );
}



int race_lookup( char *name )
{
    int i;

    for ( i = 0; i < top_race; i++ )
    {
	if ( !strcmp( name, race_array[i]->race_name ) )
	    break;
    }

    /*
     * default to standard humanoid
     */
    if ( i == top_race )
	return 0;

    return i;
}


int relative_height( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int r;

    r = ch->height * 100;

    r /= ( victim->height ? victim->height : 1 );

    if ( r < 40 )
	return 0;
    else if ( r < 50 )
	return 1;
    else if ( r < 60 )
	return 2;
    else if ( r < 70 )
	return 3;
    else if ( r < 80 )
	return 4;
    else if ( r < 90 )
	return 5;
    else if ( r < 110 )
	return 6;
    else if ( r < 130 )
	return 7;
    else if ( r < 180 )
	return 8;

    return 9;
}
