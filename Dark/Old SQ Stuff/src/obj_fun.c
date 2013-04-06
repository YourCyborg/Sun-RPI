#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"



/*
 * The following special functions are available for rooms.
 */
DECLARE_OBJ_FUN(	spec_armsheath		);
DECLARE_OBJ_FUN(	spec_flame_blade	);


OBJ_FUN *obj_fun_lookup( const char *name )
{
    if ( !str_cmp( name, "spec_armsheath"          ) ) return spec_armsheath;
    if ( !str_cmp( name, "spec_flame_blade"        ) ) return spec_flame_blade;
    return 0;
}


bool spec_armsheath( CHAR_DATA *ch, char *command, DO_FUN *fnptr,
  char *argument, OBJ_DATA *obj )
{
    char arg[MAX_STRING_LENGTH];
    char arg2[MAX_STRING_LENGTH];
    OBJ_DATA *weapon;

    if ( obj->wear_loc == gn_wear_none )
	return FALSE;

    if ( fnptr != do_draw && fnptr != do_sheath )
	return FALSE;

    argument = one_argument( argument, arg );
    argument = one_argument( argument, arg2 );

    if ( arg2 == NULL || arg2[0] == '\0' )
	return FALSE;

    if ( obj != get_obj_wear( ch, arg2, ch ) )
	return FALSE;

    if ( fnptr == do_draw )
    {
	int loc = gn_wear_primary;

	if ( get_eq_char( ch, gn_wear_primary ) )
	{
	    if ( get_eq_char( ch, gn_wear_secondary ) )
	    {
		send_to_char("Your hands are full.\n\r", ch );
		return TRUE;
	    }
	    loc = gn_wear_secondary;
	}

	for ( weapon = obj->contains; weapon != NULL; weapon = weapon->next_content )
	{
	    if ( is_name( arg, weapon->name ) )
		break;
	}

	if ( weapon == NULL )
	{
	    act( "$p doesn't seem to contain that weapon.", ch, NULL, obj,
		NULL, NULL, TO_CHAR, SENSE_SIXTH );
	    return TRUE;
	}

	if ( get_obj_weight( weapon ) > (can_carry_w(ch)/15) )
	{
	    send_to_char( "It is too heavy for you to wield.\n\r", ch );
	    return TRUE;
	}

	act( "$n draws $p from $P.", ch, NULL, weapon, obj, NULL,
	    TO_ROOM, SENSE_SIGHT );
	act( "You draw $p from $P.", ch, NULL, weapon, obj, NULL,
	    TO_CHAR, SENSE_SIXTH );

	obj_from_obj( weapon );
	obj_to_char( weapon, ch );
	equip_char( ch, weapon, loc );

	return TRUE;
    }

    if ( fnptr == do_sheath )
    {
	weapon = get_eq_char( ch, gn_wear_primary );

	if ( weapon == NULL || !is_name( arg, weapon->name ) )
	{
	    weapon = get_eq_char( ch, gn_wear_secondary );

	    if ( weapon == NULL || !is_name( arg, weapon->name ) )
	    {
		send_to_char("You don't wield that weapon.\n\r", ch );
		return TRUE;
	    }
	}

	if ( weapon->item_type != ITEM_WEAPON )
	{
	    send_to_char("You can't sheath that.\n\r", ch );
	    return TRUE;
	}

	if ( ( get_obj_weight( weapon ) + get_obj_weight( obj ) )
	    > obj->capacity )
	{
	    send_to_char("There isn't enough room to sheath that.\n\r", ch );
	    return TRUE;
	}

	act( "You sheath $p in $P.", ch, NULL, weapon, obj, NULL,
	    TO_CHAR, SENSE_SIXTH );
	act( "$n sheathes $p in $P.", ch, NULL, weapon, obj, NULL,
	    TO_ROOM, SENSE_SIGHT );

	unequip_char( ch, weapon );
	obj_from_char( weapon );
	obj_to_obj( weapon, obj );

	return TRUE;
    }


    return FALSE;
}


bool spec_flame_blade( CHAR_DATA *ch, char *command, DO_FUN *fnptr,
  char *argument, OBJ_DATA *obj )
{
    if ( fnptr != (DO_FUN *) violence_update )
	return FALSE;

    if ( obj->wear_loc != gn_wear_primary
    && obj->wear_loc != gn_wear_secondary )
	return FALSE;

    if ( number_bits( 5 ) )
	return FALSE;

    spell_fireball( gsn_fireball, 200, ch, ch->fighting );

    return TRUE;
}
