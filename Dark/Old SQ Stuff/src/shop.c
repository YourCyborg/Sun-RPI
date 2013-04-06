#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"



void do_shopset( CHAR_DATA *ch, char *argument )
{
    SHOP_DATA *pShop;
    MOB_INDEX_DATA *pMobIndex = NULL;
    CHAR_DATA *mob;
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char arg3[MAX_INPUT_LENGTH];
    char arg4[MAX_INPUT_LENGTH];
    char arg5[MAX_INPUT_LENGTH];
    int i, num;
    extern const char *shopkeeper_messages[SHOP_MAX_MESSAGE];

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( is_number( arg1 ) )
    {
	pMobIndex = get_mob_index( atoi( arg1 ) );
    }
    else
    {
	if ( (mob = get_char_room( ch, arg1 )) != NULL )
	{
	    if ( !IS_NPC( mob ) )
	    {
		send_to_char("Target must be a non-player.\n\r", ch );
		return;
	    }

	    pMobIndex = mob->pIndexData;
	}
    }

    if ( pMobIndex == NULL )
    {
	send_to_char( "No such mobile.\n\r", ch );
	return;
    }

    if ( !can_build( ch, pMobIndex->area ) )
    {
	send_to_char(
      "You don't have authorization to build in that mobile's zone.\n\r", ch );
	return;
    }

    if ( pMobIndex->pShop == NULL
    && str_prefix( arg2, "create" ) )
    {
	send_to_char("That mobile isn't a shopkeeper.\n\r", ch );
	send_to_char("Use 'shopset <mob> create' to make a shopkeeper.\n\r", ch );
	return;
    }

    if ( !str_prefix( arg2, "create" ) )
    {
	if ( pMobIndex->pShop != NULL )
	{
	    send_to_char("That mobile is already a shopkeeper.\n\r", ch );
	    return;
	}
	pMobIndex->pShop = new_shop( );
	send_to_char("Shop created.\n\r", ch );
	return;
    }

    pShop = pMobIndex->pShop;

    if ( !str_prefix( arg2, "delete" ) )
    {
	free_shop( pShop );
	pMobIndex->pShop = NULL;
	send_to_char("Shop removed from mobile.\n\r", ch );
	return;
    }

    /*
     * Everything past here uses 3.
     * message uses rest of string, so don't parse 4 until after.
     */
    argument = one_argument( argument, arg3 );

    if ( !str_prefix( arg2, "message" ) )
    {
	int message = -1;
	if ( !str_cmp( arg3, "buy1" )
	|| !str_cmp( arg3, "list2" )
	|| !str_cmp( arg3, "doesnthave" ) )
	    message = SHOP_DOESNT_HAVE;

	else if ( !str_cmp( arg3, "buy2" )
	|| !str_cmp( arg3, "youarebroke" ) )
	    message = SHOP_YOU_ARE_BROKE;

	else if ( !str_cmp( arg3, "buy3" )
	|| !str_cmp( arg3, "thatwillbe" ) )
	    message = SHOP_THAT_WILL_BE;

	else if ( !str_cmp( arg3, "list1" )
	|| !str_cmp( arg3, "value1" )
	|| !str_cmp( arg3, "thatisntmoney" ) )
	    message = SHOP_THAT_ISNT_MONEY;

	else if ( !str_cmp( arg3, "sell1" )
	|| !str_cmp( arg3, "value2" )
	|| !str_cmp( arg3, "youdonthave" ) )
	    message = SHOP_YOU_DONT_HAVE;

	else if ( !str_cmp( arg3, "sell2" )
	|| !str_cmp( arg3, "value3" )
	|| !str_cmp( arg3, "wontbuy" ) )
	    message = SHOP_WONT_BUY;

	else if ( !str_cmp( arg3, "sell3" )
	|| !str_cmp( arg3, "isbroke" ) )
	    message = SHOP_IS_BROKE;

	else if ( !str_cmp( arg3, "sell4" )
	|| !str_cmp( arg3, "takethismoney" ) )
	    message = SHOP_TAKE_THIS_MONEY;

	else if ( !str_cmp( arg3, "value4" )
	|| !str_cmp( arg3, "thatisworth" ) )
	    message = SHOP_THAT_IS_WORTH;

	else
	{
	    send_to_char( "Bad message.  Options:\n\r", ch );
	    send_to_char( "buy1 (list2) doesnthave\n\r", ch );
	    send_to_char( "buy2 youarebroke\n\r", ch );
	    send_to_char( "buy3 thatwillbe\n\r", ch );
	    send_to_char( "list1 (value1) thatisntmoney\n\r", ch );
	    send_to_char( "sell1 (value2) youdonthave\n\r", ch );
	    send_to_char( "sell2 (value3) wontbuy\n\r", ch );
	    send_to_char( "sell3 isbroke\n\r", ch );
	    send_to_char( "sell4 takethismoney\n\r", ch );
	    send_to_char( "value4 thatisworth\n\r", ch );
	    return;
	}

	if ( pShop->shop_messages[message]
	!= shopkeeper_messages[message] )
	    free_string( pShop->shop_messages[message],
		MEM_MOB_SHOP_MESSAGE );

	pShop->shop_messages[message] = str_dup( argument );

	send_to_char( "Shop message set.\n\r", ch );
	return;
    }


    argument = one_argument( argument, arg4 );

    if ( !str_prefix( arg2, "buy" ) )
    {
	switch( arg3[0] )
	{
	    default:
		send_to_char("Options:\n\r", ch );
		send_to_char("  buy add <type>\n\r", ch );
		send_to_char("  buy remove <type>\n\r", ch );
		send_to_char("  buy clear\n\r", ch );
		return;
		break;

	    case 'a':
		for ( i = 0; i < MAX_TRADE; i++ )
		    if ( pShop->buy_type[i] <= 0 )
			break;

		if ( i == MAX_TRADE )
		{
		    send_to_char( "You must remove a buy type first.\n\r", ch );
		    return;
		}

		if ( arg4[0] == '\0' || !is_number( arg4 ) )
		{
		    send_to_char( "You must supply a type to set.\n\r", ch );
		    return;
		}

		pShop->buy_type[i] = atoi( arg4 );
		break;

	    case 'r':
		if ( arg4[0] == '\0' || !is_number( arg4 ) )
		{
		    send_to_char( "You must supply a type to remove.\n\r", ch );
		    return;
		}

		num = atoi( arg4 );

		for ( i = 0; i < MAX_TRADE; i++ )
		    if ( pShop->buy_type[i] == num )
			pShop->buy_type[i] = 0;
		break;

	    case 'c':
		for ( i = 0; i < MAX_TRADE; i++ )
		    pShop->buy_type[i] = 0;
		break;
	}
	send_to_char("Ok.\n\r", ch );
	return;
    }

    if ( !str_prefix( arg2, "coinage" ) )
    {
	one_argument( argument, arg5 );
	if ( arg5[0] == '\0'
	|| !is_number( arg4 ) || !is_number( arg5 ) )
	{
	    send_to_char("Syntax: coinage <type> <buy> <sell>\n\r", ch );
	    return;
	}

	num = get_coin_index( arg3 );
	if ( num == -1 )
	{
	    send_to_char("Invalid coin type.\n\r", ch );
	    return;
	}

	pShop->coinage_buy[num] = atoi( arg4 );
	pShop->coinage_sell[num] = atoi( arg5 );

	send_to_char( "Ok.\n\r", ch );
	return;
    }

    if ( !str_prefix( arg2, "gain" ) )
    {
	send_to_char("Gain is presently unused.\n\r", ch );
	return;
    }

    if ( !str_prefix( arg2, "hours" ) )
    {
	if ( arg4[0] == '\0'
	|| !is_number( arg3 ) || !is_number( arg4 ) )
	{
	    send_to_char("Syntax: hours <open: 0-22> <close: 1-23>\n\r", ch );
	    return;
	}

	pShop->open_hour = atoi( arg3 );
	pShop->close_hour = atoi( arg4 );
	return;
    }

    if ( !str_prefix( arg2, "profits" ) )
    {
	if ( arg4[0] == '\0'
	|| !is_number( arg3 ) || !is_number( arg4 ) )
	{
	    send_to_char("Syntax: profits <buy: high> <sell: low>\n\r", ch );
	    send_to_char("Profits need not be higher and lower than 100\n\r", ch );
	    send_to_char("respectively but buy should always be higher.\n\r", ch );
	    return;
	}

	pShop->profit_buy = atoi( arg3 );
	pShop->profit_sell = atoi( arg4 );
	return;
    }
}
