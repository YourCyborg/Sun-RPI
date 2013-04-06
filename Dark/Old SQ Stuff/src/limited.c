/*
 *  limited.c
 *
 *  implements limited equipment for Merc 2.1.
 *
 *  this limited equipment code assumes that mobiles are not able
 * to save.  This is easily correctable by changing the Vnum code
 * for objects to something like OVnum.
 *
 *  This file will have _slow_run time, but only once at boot.
 * grep on entire player directory for Vnum is slow...
 *
 * (c) 1996 by Robert L. Peckham
 */

#include <stdio.h>
#include <time.h>
#include "merc.h"

void load_equipment_totals( void )
{
    FILE *fp;
    OBJ_INDEX_DATA *pObjIndex;
    char file[MAX_STRING_LENGTH];
    int vnum;

/* moving this to the startup script. */
/*
    sprintf( buf, "rm -f %s/0object.list", PLAYER_DIR );
    system( buf );

*** change - to a * in line below

    sprintf( buf, "grep Vnum %s/- > %s/0object.list", PLAYER_DIR, PLAYER_DIR );
    system( buf );
*/

    /*
     * Player file directory.
     */
    sprintf( file, "%s/0object.list", PLAYER_DIR );
    if ( ( fp = fopen( file, "r" ) ) == NULL )
    {
	log_string("Error opening restricted object file in player directory.\n\r");
	exit(1);
    }

    while ( fread_word( fp ) != "#" )
    {
	vnum = fread_number( fp );
	fread_to_eol( fp );

	pObjIndex = get_obj_index( vnum );

	if ( pObjIndex == NULL )
	    continue;

	pObjIndex->number++;
    }

    fclose( fp );

    /*
     * Player file directory.
     */
    sprintf( file, "%s/0object.list", ROOM_SAVE_DIR );
    if ( ( fp = fopen( file, "r" ) ) == NULL )
    {
	log_string("Error opening restricted object file in room directory.\n\r");
	exit(1);
    }

    while ( fread_word( fp ) != "#" )
    {
	vnum = fread_number( fp );
	fread_to_eol( fp );

	pObjIndex = get_obj_index( vnum );

	if ( pObjIndex == NULL )
	    continue;

	pObjIndex->number++;
    }

    fclose( fp );

    return;
}
