#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"



struct coin_pile_data
{
    int		amount;
    char	*string;
};

const struct coin_pile_data coins_pile_table [] =
{
    {    1, "a single %s" },
    {    2, "a couple of %s" },
    {    4, "a few %s" },
    {    8, "a handfull of %s" },
    {   16, "a stack of %s" },
    {   32, "a small pile of %s" },
    {   64, "a pile of %s" },
    {   -1, NULL }
};


const char *shopkeeper_messages[SHOP_MAX_MESSAGE] =
{
    "I don't carry that item.",
    "You don't seem to have that.",
    "I don't buy such items.",
    "I think I'll wait until later.",
    "You can't afford that item.",
    "That will be",
    "Take these",
    "I'll give you %d %s for that.",
    "I don't take %s."
};


/*
 * Initial values.  These are modified by a coin value file,
 * if available. The main list for the coins are at the forums.
 */
/*   value      name        plural   density */
const struct coin_data coin_table [MAX_COIN] =
{
    {    1,    "arrow",   "arrows",  500 },
    {  197,    "quiver",    "quivers",  250 },
    {   80,    "blade", "blades",  180 },
    {   50,    "lion",    "lions",  200 },
    {   70,    "bit",    "bits",  250 },
    {   60,    "tenbit",    "tenbits",  200 },
    {   35,    "talent",    "talents",  250 },
    {   50,     "kalak-gold",   "kalak-golds",  200 },
    {   55,    "star",    "stars",  250 },
    {   65,    "crescent",      "crescents",  250 },
    {   70,    "full moon",    "full moons",  250 },
    {   60,    "tectuk",    "tectuks",  200 },
    {   35,    "chip",    "chips",  250 },
    {   50,     "penny",      "pennys",  200 },
    {   55,    "agora",    "agoras",  250 },
    {   70,    "dictator",    "dictators",  250 },
    {   60,    "talon",    "talons",  200 },
    {   35,    "judaga",    "judagas",  250 },
    {   50,     "oba",      "obas",  200 },
    {   55,    "piece",    "pieces",  250 },
    {   70,    "tenpiece",    "tenpieces",  250 },
    {   60,    "mark",    "marks",  200 },
    {   35,    "crown",    "crowns",  250 },
    {   50,     "slave",      "slaves",  200 },
    {   55,    "common",    "commons",  250 },
    {   65,    "vizier",      "viziers",  250 },
    {   70,    "abalach",    "abalachs",  250 }
};



/*
 * Create a 'money' obj.
 */
OBJ_DATA *create_money( int amount, int type )
{
    int i;
    char buf[MAX_STRING_LENGTH];
    char buf_desc[MAX_STRING_LENGTH];
    char buf_name[MAX_STRING_LENGTH];
    OBJ_DATA *obj;

    if ( amount <= 0 )
    {
	bug( "Create_money: zero or negative money %d.", amount );
	amount = 1;
    }

    for ( i = 0; coins_pile_table[i].amount > 0; i++ )
	if ( amount < coins_pile_table[i].amount )
	    break;
    i--;

    obj = create_object( get_obj_index( OBJ_VNUM_MONEY_SOME ) );
    if ( amount == 1 )
    {
      sprintf( buf, coins_pile_table[i].string, coin_table[type].name );
      sprintf( buf_desc, "A %s.", coin_table[type].name );
    }
    else
    {
      sprintf( buf, coins_pile_table[i].string, coin_table[type].name_plural );
      sprintf( buf_desc, "%d %s.", amount, coin_table[type].name );
    }
    sprintf( buf_name, "coins pile %s %s",
	coin_table[type].name, coin_table[type].name_plural );

    free_string( obj->name, MEM_OBJ_NAME );
    obj->name		= str_dup( buf_name );
    free_string( obj->short_descr, MEM_OBJ_SHORT_DESCR );
    obj->short_descr		= str_dup( buf );
    free_string( obj->description, MEM_OBJ_DESCRIPTION );
    obj->description		= str_dup( buf_desc );

    sprintf( buf, "%s lies here.", buf );
    free_string( obj->look_descr, MEM_OBJ_LOOK_DESCR );
    obj->look_descr		= str_dup( buf );

    obj->value[0]		= amount;
    obj->value[1]		= type;
    obj->weight			= get_coin_weight( amount, type );

    return obj;
}


int get_coin_weight( int amount, int type )
{
    return ((amount * 10 / coin_table[type].density) + 5);
}


int get_coin_index( char *argument )
{
    int i;

    for ( i = 0; i < MAX_COIN; i++ )
	if ( !str_prefix( argument, coin_table[i].name )
	|| !str_prefix( argument, coin_table[i].name_plural ) )
	    return i;

    return -1;
}


void gain_coin( CHAR_DATA *ch, int amount, int coin, bool fShow, char *outbuf )
{
    OBJ_DATA *c_obj;
    char buf[MAX_STRING_LENGTH];

    for ( c_obj = ch->carrying; c_obj != NULL; c_obj = c_obj->next_content )
	if ( c_obj->item_type == ITEM_MONEY && c_obj->value[1] == coin )
	    break;

    if ( fShow )
    {
	sprintf( buf, "%d %s", amount, coin_table[coin].name_plural );
	strcat( outbuf, buf );
    }

    if ( c_obj != NULL )
    {
	amount += c_obj->value[0];
	obj_from_char( c_obj );
	extract_obj( c_obj, TRUE );
    }

    c_obj = create_money( amount, coin );
    obj_to_char( c_obj, ch );

    return;
}


void lose_coin( CHAR_DATA *ch, int amount, int coin, bool fShow, char *outbuf )
{
    OBJ_DATA *c_obj;
    char buf[MAX_STRING_LENGTH];

    for ( c_obj = ch->carrying; c_obj != NULL; c_obj = c_obj->next_content )
	if ( c_obj->item_type == ITEM_MONEY && c_obj->value[1] == coin )
	    break;

    if ( c_obj == NULL || c_obj->value[0] < amount )
	return;

    if ( fShow )
    {
	sprintf( buf, "%d %s", amount, coin_table[coin].name_plural );
	strcat( outbuf, buf );
    }

    amount = c_obj->value[0] - amount;

    obj_from_char( c_obj );
    extract_obj( c_obj, TRUE );

    if ( amount != 0 )
    {
	c_obj = create_money( amount, coin );
	obj_to_char( c_obj, ch );
    }

    return;
}


int get_coin_value( int coin, bool fBuy, SHOP_DATA *pShop )
{
  int cval = coin_table[coin].value;

  if ( pShop != NULL )
  {
    if ( fBuy )
      cval *= pShop->coinage_buy[coin];
    else
      cval *= pShop->coinage_sell[coin];

    if ( cval == 0 )
      return 0;

    cval /= 100;
  }

  cval = UMAX( cval, 1 );

  return cval;
}


/*
 *  Check money should already have verified that they
 * have enough.  Will just take all money otherwise, since
 * we don't want to keep track of what we took.
 *
 *  Order is an arbitrarily long string consisting of
 * optional quantities and coin types.  If a coin type is
 * given, it will attempt to use that many of that type,
 * then proceed to the next argument.  If arguments are used
 * up, it will use the first type in the character's inventory.
 */
void transfer_money( CHAR_DATA *ch, CHAR_DATA *victim, int amount, 
    SHOP_DATA *pShop, bool fBuy, char *order, bool fToRecipient,
    bool fToSource, char *outbuf )
{
  OBJ_DATA *obj;
  OBJ_DATA *obj_next;
  int total = 0;
  int i;
  int cval;
  int coin;
  bool fFirst = TRUE;
  char arg[MAX_INPUT_LENGTH];

  order = one_argument( order, arg );

  while ( total < amount && arg[0] != '\0' )
  {
    i = -1;
    if ( is_number( arg ) )
    {
      i = atoi( arg );
      order = one_argument( order, arg );
      if ( arg[0] == '\0' )
	break;
    }

    coin = get_coin_index( arg );

    if ( coin < 0 )
    {
      order = one_argument( order, arg );
      continue;
    }

    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
      if ( obj->item_type == ITEM_MONEY && obj->value[1] == coin )
	break;

    if ( obj == NULL )
    {
      order = one_argument( order, arg );
      continue;
    }

    if ( i > obj->value[0] || i < 1 )
      i = obj->value[0];

    cval = get_coin_value( coin, fBuy, pShop );

    if ( ( amount - total ) % cval )
    {
      i = UMIN( i, ( !fBuy
	? ( ( amount - total ) / cval )
	: ( ( ( amount - total ) / cval ) + 1 ) ) );
    }
    else
      i = UMIN( i, ( amount - total ) / cval );

    if ( outbuf != NULL )
    {
      if ( fFirst )
      {
	strcat( outbuf, " " );
	fFirst = FALSE;
      }
      else
	strcat( outbuf, ", " );
    }

    lose_coin( ch, i, coin, fToSource, outbuf );
    gain_coin( victim, i, coin, fToRecipient, outbuf );

    total += i * cval;
    if ( total >= amount )
      return;

    order = one_argument( order, arg );
  }

  for ( obj = ch->carrying; obj != NULL; obj = obj_next )
  {
    obj_next = obj->next_content;

    if ( obj->item_type != ITEM_MONEY )
      continue;

    i = obj->value[0];
    coin = obj->value[1];

    cval = get_coin_value( coin, fBuy, pShop );

    if ( cval == 0 )
      continue;

    if ( ( amount - total ) % cval )
    {
      i = UMIN( i, ( !fBuy
	? ( ( amount - total ) / cval )
	: ( ( ( amount - total ) / cval ) + 1 ) ) );
    }
    else
      i = UMIN( i, ( amount - total ) / cval );

    if ( outbuf != NULL )
    {
      if ( fFirst )
      {
	strcat( outbuf, " " );
	fFirst = FALSE;
      }
      else
	strcat( outbuf, ", " );
    }

    lose_coin( ch, i, coin, fToSource, outbuf );
    gain_coin( victim, i, coin, fToRecipient, outbuf );

    total += i * cval;
    if ( total >= amount )
      return;
  }

  return;
}


/*
 * total the amount of money a character has with respect to a shopkeeper,
 * and return the total in silvers. (or shopkeeper with respect to his
 * own money).
 */
int check_money( CHAR_DATA *ch, SHOP_DATA *pShop, bool fBuy )
{
    OBJ_DATA *obj;
    int total = 0;
    int i;

    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
    {
	if ( obj->item_type != ITEM_MONEY )
	    continue;

	i = obj->value[0];
	i *= coin_table[obj->value[1]].value;

	if ( pShop != NULL )
	{
	    if ( fBuy )
		i *= pShop->coinage_buy[obj->value[1]];
	    else
		i *= pShop->coinage_sell[obj->value[1]];

	    i /= 100;
	}

	total += i;
    }

    return total;
}



CHAR_DATA *find_keeper( CHAR_DATA *ch )
{
    CHAR_DATA *keeper;
    SHOP_DATA *pShop;

    pShop = NULL;
    for ( keeper = ch->in_room->people; keeper; keeper = keeper->next_in_room )
    {
	if ( IS_NPC(keeper) && (pShop = keeper->pIndexData->pShop) != NULL )
	    break;
    }

    if ( pShop == NULL )
    {
	send_to_char( "You can't do that here.\n\r", ch );
	return NULL;
    }

    /*
     * Shop hours.
     */
    if ( time_info.hour < pShop->open_hour )
    {
	do_say( keeper, "Sorry, come back later." );
	return NULL;
    }
    
    if ( time_info.hour > pShop->close_hour )
    {
	do_say( keeper, "Sorry, come back tomorrow." );
	return NULL;
    }

    /*
     * Invisible or hidden people.
     */
    if ( !can_see( keeper, ch ) )
    {
	do_say( keeper, "I don't trade with folks I can't see." );
	return NULL;
    }

    return keeper;
}



int get_cost( CHAR_DATA *keeper, OBJ_DATA *obj, bool fBuy )
{
    SHOP_DATA *pShop;
    int cost;

    if ( obj == NULL || ( pShop = keeper->pIndexData->pShop ) == NULL )
	return 0;

    if ( fBuy )
    {
	cost = obj->cost * pShop->profit_buy  / 100;
    }
    else
    {
	int itype;

	cost = 0;
	for ( itype = 0; itype < MAX_TRADE; itype++ )
	{
	    if ( obj->item_type == pShop->buy_type[itype] )
	    {
		cost = obj->cost * pShop->profit_sell / 100;
		break;
	    }
	}
    }

    if ( obj->item_type == ITEM_STAFF || obj->item_type == ITEM_WAND )
	cost = cost * obj->value[2] / obj->value[1];

    return cost;
}



void do_buy( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Buy what?\n\r", ch );
	return;
    }

    if ( IS_SET(ch->in_room->room_flags, ROOM_PET_SHOP) )
    {
	CHAR_DATA *pet;
	ROOM_INDEX_DATA *pRoomIndexNext;
	ROOM_INDEX_DATA *in_room;

	if ( IS_NPC(ch) )
	    return;

	pRoomIndexNext = get_room_index( ch->in_room->vnum + 1 );
	if ( pRoomIndexNext == NULL )
	{
	    bug( "Do_buy: bad pet shop at vnum %d.", ch->in_room->vnum );
	    send_to_char( "Sorry, you can't buy that here.\n\r", ch );
	    return;
	}

	in_room     = ch->in_room;
	ch->in_room = pRoomIndexNext;
	pet         = get_char_room( ch, arg );
	ch->in_room = in_room;

	if ( pet == NULL )
	{
	    send_to_char( "Sorry, you can't buy that here.\n\r", ch );
	    return;
	}

/*
	if ( check_money( ch ) < 100 * pet->level * pet->level )
	{
	    send_to_char( "You can't afford it.\n\r", ch );
	    return;
	}
*/

/*	ch->gold		-= 100 * pet->level * pet->level; */
	pet			= create_mobile( pet->pIndexData );

	argument = one_argument( argument, arg );
	if ( arg[0] != '\0' )
	{
	    sprintf( buf, "%s %s", pet->name, arg );
	    free_string( pet->name, MEM_MOB_NAME );
	    pet->name = str_dup( buf );
	}

	char_to_room( pet, ch->in_room, TRUE );

	if ( IS_SET( pet->act, ACT_MOUNT ) )
	{
	    act( "$N is led to you.", ch, pet, NULL, NULL, NULL, TO_CHAR, SENSE_SIXTH );
	    act( "$n purchases $N.", ch, pet, NULL, NULL, NULL, TO_ROOM, SENSE_SIGHT );
	}
	else
	{
	    SET_BIT(pet->affected_by, AFF_CHARM);
	    add_follower( pet, ch );
	    act( "$N is led to you.", ch, pet, NULL, NULL, NULL, TO_CHAR, SENSE_SIXTH );
	    act( "$n purchases $N.", ch, pet, NULL, NULL, NULL, TO_ROOM, SENSE_SIGHT );
	}

	return;
    }
    else
    {
	CHAR_DATA *keeper;
	OBJ_DATA *obj;
	int cost;

	if ( ( keeper = find_keeper( ch ) ) == NULL )
	    return;

	obj  = get_obj_carry( keeper, arg, ch );
	cost = get_cost( keeper, obj, TRUE );

	if ( cost <= 0 || !can_see_obj( ch, obj ) )
	{
	    sprintf( buf, "%s tells you\n\r    '%s'",
		person( ch, keeper ),
		keeper->pIndexData->pShop->shop_messages[SHOP_DOESNT_HAVE] );
	    act( buf, keeper, ch, NULL, NULL, NULL, TO_VICT, SENSE_SIXTH );
	    return;
	}

	if ( check_money( ch, keeper->pIndexData->pShop, TRUE ) < cost )
	{
	    sprintf( buf, "%s tells you\n\r    '%s'",
		person( ch, keeper ),
		keeper->pIndexData->pShop->shop_messages[SHOP_YOU_ARE_BROKE] );
	    act( buf, keeper, ch, NULL, NULL, NULL, TO_VICT, SENSE_SIXTH );
	    return;
	}
	
	if ( ch->carry_number + 1 > can_carry_n( ch ) )
	{
	    send_to_char( "You can't carry that many items.\n\r", ch );
	    return;
	}

	if ( ch->carry_weight + get_obj_weight( obj ) > can_carry_w( ch ) )
	{
	    send_to_char( "You can't carry that much weight.\n\r", ch );
	    return;
	}

	act( "$n buys $p.", ch, NULL, obj, NULL, NULL, TO_ROOM, SENSE_SIGHT );
	sprintf( buf, "%s tells you\n\r    '%s",
	    person( ch, keeper ),
	    keeper->pIndexData->pShop->shop_messages[SHOP_THAT_WILL_BE] );
	transfer_money( ch, keeper, cost, keeper->pIndexData->pShop,
	    TRUE, argument, TRUE, FALSE, buf );
	strcat( buf, ".'\n\r" );
	send_to_char( buf, ch );

/* item shuffling because of renaming */
	if ( IS_SET( obj->extra_flags, ITEM_INVENTORY ) )
	{
	    obj_from_char( obj );
	    obj_to_char( obj, ch );
	    REMOVE_BIT( obj->extra_flags, ITEM_INVENTORY );

	    obj = create_object( obj->pIndexData );
	    obj_to_char( obj, keeper );
	    SET_BIT( obj->extra_flags, ITEM_INVENTORY );
	}
	else
	{
	    obj_from_char( obj );
	    obj_to_char( obj, ch );
	}

	return;
    }

    return;
}


void do_list( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    if ( IS_SET(ch->in_room->room_flags, ROOM_PET_SHOP) )
    {
	ROOM_INDEX_DATA *pRoomIndexNext;
	CHAR_DATA *pet;
	bool found;

	pRoomIndexNext = get_room_index( ch->in_room->vnum + 1 );
	if ( pRoomIndexNext == NULL )
	{
	    bug( "Do_list: bad pet shop at vnum %d.", ch->in_room->vnum );
	    send_to_char( "You can't do that here.\n\r", ch );
	    return;
	}

	found = FALSE;
	for ( pet = pRoomIndexNext->people; pet; pet = pet->next_in_room )
	{
		if ( !found )
		{
		    found = TRUE;
		    send_to_char( "Pets for sale:\n\r", ch );
		}
		sprintf( buf, "%8d - %s\n\r",
		    100 * pet->level * pet->level,
		    pet->short_descr );
		send_to_char( buf, ch );
	}
	if ( !found )
	    send_to_char( "Sorry, we're out of pets right now.\n\r", ch );
	return;
    }
    else
    {
	char arg[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];
	CHAR_DATA *keeper;
	OBJ_DATA *obj;
	int cost;
	int modifier;
	int coin = -1;
	bool found;

	argument = one_argument( argument, arg );
	one_argument( argument, arg2 );

	if ( ( keeper = find_keeper( ch ) ) == NULL )
	    return;

	if ( arg2[0] != '\0' )
	    coin = get_coin_index( arg2 );
	else if ( arg[0] != '\0' )
	{
	    coin = get_coin_index( arg );
	    if ( coin != -1 )
		arg[0] = '\0';
	}

	if ( coin == -1 )
	{
	    for ( obj = ch->carrying; obj; obj = obj->next_content )
		if ( obj->item_type == ITEM_MONEY )
		    break;

	    if ( obj != NULL )
		coin = obj->value[1];
	}

	if (  coin == -1 )
	{
	    send_to_char("List in what currency?\n\r", ch );
	    return;
	}

	modifier = get_coin_value( coin, TRUE, keeper->pIndexData->pShop );

	if ( modifier == 0 )
	{
	    sprintf( buf,
		keeper->pIndexData->pShop->shop_messages[SHOP_THAT_ISNT_MONEY],
		coin_table[coin].name_plural );
	    act( "$n tells you\n\r    '$t'", keeper, ch, NULL, buf, NULL,
		TO_VICT, SENSE_SIXTH );
	    return;
	}

	found = FALSE;
	for ( obj = keeper->carrying; obj; obj = obj->next_content )
	{
	    if ( obj->wear_loc == gn_wear_none
	    &&   can_see_obj( ch, obj )
	    && ( cost = get_cost( keeper, obj, TRUE ) ) > 0
	    && ( arg[0] == '\0' || is_name( arg, obj->name ) ) )
	    {
		if ( !found )
		{
		    found = TRUE;
		    sprintf( buf, "Prices in %s.\n\r[Price] Item\n\r",
			coin_table[coin].name_plural );
		    send_to_char( buf, ch );
		}

		sprintf( buf, "[%5d] %s.\n\r",
		    ( (cost % modifier)
			? (cost / modifier) + 1 : (cost / modifier) ),
		    capitalize( obj->short_descr ) );
		send_to_char( buf, ch );
	    }
	}

	if ( !found )
	{
	    if ( arg[0] == '\0' )
		send_to_char( "You can't buy anything here.\n\r", ch );
	    else
	      act( "$n tells you\n\r    '$t'", keeper, ch, NULL,
		keeper->pIndexData->pShop->shop_messages[SHOP_DOESNT_HAVE],
		NULL, TO_VICT, SENSE_SIXTH );
	}
	return;
    }
}


void do_sell( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *keeper;
    OBJ_DATA *obj;
    int cost;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Sell what?\n\r", ch );
	return;
    }

    if ( ( keeper = find_keeper( ch ) ) == NULL )
	return;

    if ( ( obj = get_obj_carry( ch, arg, ch ) ) == NULL )
    {
	act( "$n tells you\n\r    '$t'.", keeper, ch, NULL,
	    keeper->pIndexData->pShop->shop_messages[SHOP_YOU_DONT_HAVE],
	    NULL, TO_VICT, SENSE_SIXTH );
	return;
    }

    if ( !can_drop_obj( ch, obj ) )
    {
	send_to_char( "You can't let go of it.\n\r", ch );
	return;
    }

    if ( ( cost = get_cost( keeper, obj, FALSE ) ) <= 0 )
    {
	act( "$n tells you\n\r    '$t'.", keeper, ch, NULL,
	    keeper->pIndexData->pShop->shop_messages[SHOP_WONT_BUY],
	    NULL, TO_VICT, SENSE_SIXTH );
	return;
    }

    if ( check_money( keeper, keeper->pIndexData->pShop, FALSE ) < cost )
    {
	act( "$n tells you\n\r    '$t'.", keeper, ch, NULL,
	    keeper->pIndexData->pShop->shop_messages[SHOP_IS_BROKE],
	    NULL, TO_VICT, SENSE_SIXTH );
	return;
    }

    act( "$n sells $p.", ch, NULL, obj, NULL, NULL, TO_ROOM, SENSE_SIGHT );
    sprintf( buf, "%s tells you\n\r    '%s",
      person( ch, keeper ),
      keeper->pIndexData->pShop->shop_messages[SHOP_TAKE_THIS_MONEY] );
    transfer_money( keeper, ch, cost, keeper->pIndexData->pShop,
      FALSE, argument, FALSE, TRUE, buf );
    strcat( buf, ".'\n\r" );
    send_to_char( buf, ch );


    obj_from_char( obj );
    obj_to_char( obj, keeper );

    return;
}


void do_value( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *keeper;
    OBJ_DATA *obj = NULL;
    int coin = -1;
    int cost;
    int modifier;

    argument = one_argument( argument, arg );
    one_argument( argument, arg2 );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Value what?\n\r", ch );
	return;
    }

    if ( ( keeper = find_keeper( ch ) ) == NULL )
	return;

    if ( arg2[0] != '\0' )
	coin = get_coin_index( arg2 );
    else
    {
	for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
	    if ( obj->item_type == ITEM_MONEY )
		break;

	if ( obj == NULL )
	  for ( obj = keeper->carrying; obj != NULL; obj = obj->next_content )
	    if ( obj->item_type == ITEM_MONEY )
		break;

	if ( obj != NULL )
	    coin = obj->value[1];
    }

    if ( coin == -1 )
    {
	send_to_char( "Value in what currency?\n\r", ch );
	return;
    }

    if ( keeper->pIndexData->pShop->coinage_sell[coin] == 0 )
    {
	sprintf( buf,
	    keeper->pIndexData->pShop->shop_messages[SHOP_THAT_ISNT_MONEY],
	    coin_table[coin].name_plural );
	act( "$n tells you\n\r    '$t'", keeper, ch, NULL, buf, NULL,
	    TO_VICT, SENSE_SIXTH );
	return;
    }

    if ( ( obj = get_obj_carry( ch, arg, ch ) ) == NULL )
    {
	act( "$n tells you\n\r    '$t'.", keeper, ch, NULL,
	    keeper->pIndexData->pShop->shop_messages[SHOP_YOU_DONT_HAVE],
	    NULL, TO_VICT, SENSE_SIXTH );
	return;
    }

    if ( !can_drop_obj( ch, obj ) )
    {
	send_to_char( "You can't let go of it.\n\r", ch );
	return;
    }

    if ( ( cost = get_cost( keeper, obj, FALSE ) ) <= 0 )
    {
	act( "$n tells you\n\r    '$t'.", keeper, ch, NULL,
	    keeper->pIndexData->pShop->shop_messages[SHOP_WONT_BUY],
	    NULL, TO_VICT, SENSE_SIXTH );
	return;
    }

    modifier = get_coin_value( coin, FALSE, keeper->pIndexData->pShop );

    if ( cost % modifier )
	cost = (cost / modifier) + 1;
    else
	cost = cost / modifier;

    sprintf( buf,
	keeper->pIndexData->pShop->shop_messages[SHOP_THAT_IS_WORTH],
	cost,
	(cost > 1 ? coin_table[coin].name_plural : coin_table[coin].name ) );
    act( buf, keeper, ch, obj, NULL, NULL, TO_VICT, SENSE_SIXTH );

    return;
}
