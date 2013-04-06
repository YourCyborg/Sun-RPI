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
#include "flora.h"
#include "languages.h"

sh_int		gn_wear_about;
sh_int		gn_wear_arms;
sh_int		gn_wear_body;
sh_int		gn_wear_feet;
sh_int		gn_wear_finger;
sh_int		gn_wear_hands;
sh_int		gn_wear_head;
sh_int		gn_wear_legs;
sh_int		gn_wear_neck;
sh_int		gn_wear_none;
sh_int		gn_wear_on_belt;
sh_int		gn_wear_primary;
sh_int		gn_wear_secondary;
sh_int		gn_wear_waist;
sh_int		gn_wear_leftwrist;
sh_int          gn_wear_rightwrist;

struct wear_data const	wear_info [] =
{
    { ITEM_WEAR_UNWEARABLE, "none", "<worn as a bug>  ",
	"You buggily wear $p",
	"$n buggily wears $p",
	NULL, NULL, FALSE, &gn_wear_none },
    { ITEM_WEAR_HEAD, "head", "<worn on head>      ",
	"You place $p on your head.",
	"$n places $p on $s head.",
	NULL, NULL, TRUE, &gn_wear_head },
    { ITEM_WEAR_NECK, "neck", "<worn around neck>  ",
	"You place $p about your neck.",
	"$n places $p about $s neck.",
	&gn_wear_about, NULL, FALSE, &gn_wear_neck },
    { ITEM_WEAR_ARMS, "arms", "<worn on arms>      ",
	"You wear $p on your arms.",
	"$n wears $p on $s arms.",
	&gn_wear_about, NULL, TRUE, &gn_wear_arms },
    { ITEM_WEAR_BODY, "body", "<worn on body>      ",
	"You wear $p on your body.",
	"$n wears $p on $s body.",
	&gn_wear_about, NULL, TRUE, &gn_wear_body },
    { ITEM_WEAR_LEFT_WRIST, "left wrist", "<worn around wrist> ",
	"You slide $p onto your left wrist.",
	"$n slides $p onto $s left wrist.",
	NULL, NULL, FALSE, &gn_wear_leftwrist },
    { ITEM_WEAR_RIGHT_WRIST, "right wrist", "<worn around wrist> ",
        "You slide $p onto your right wrist.",
        "$n slides $p onto $s right wrist.",
        NULL, NULL, FALSE, &gn_wear_rightwrist },
    { ITEM_WEAR_HANDS, "hands", "<worn on hands>     ",
	"You slip your hands into $p.",
	"$n slips $s hands into $p.",
	NULL, NULL, TRUE, &gn_wear_hands },
    { ITEM_WEAR_FINGER, "finger", "<worn on finger>    ",
	"You slide $p onto your finger.",
	"$n slides $p onto $s finger.",
	&gn_wear_hands, NULL, FALSE, &gn_wear_finger },
    { ITEM_WIELD, "primary", "<primary hand>      ",
	"You hold $p in your primary hand.",
	"$n holds $p in $s primary hand.",
	NULL, NULL, TRUE, &gn_wear_primary },
    { ITEM_HOLD, "secondary", "<secondary hand>    ",
	"You hold $p in your secondary hand.",
	"$n holds $p in $s secondary hand.",
	NULL, NULL, TRUE, &gn_wear_secondary },
    { ITEM_WEAR_ABOUT, "about", "<worn about body>   ",
	"You unfurl $p about your body.",
	"$n unfurls $p about $s body.",
	NULL, NULL, TRUE, &gn_wear_about },
    { ITEM_WEAR_WAIST, "waist", "<worn about waist>  ",
	"You fasten $p about your waist.",
	"$n fastens $p about $s waist.",
	&gn_wear_about, NULL, FALSE, &gn_wear_waist },
    { ITEM_WEAR_ON_BELT, "on belt", "<worn on belt>      ",
	"You fasten $p to your belt.",
	"$n fastens $p to $s belt.",
	&gn_wear_about, &gn_wear_waist, FALSE, &gn_wear_on_belt },
    { ITEM_WEAR_LEGS, "legs", "<worn on legs>      ",
	"You wear $p on your legs.",
	"$n wears $p on $s legs.",
	&gn_wear_about, NULL, TRUE, &gn_wear_legs },
    { ITEM_WEAR_FEET, "feet", "<worn on feet>      ",
	"You wear $p on your feet.",
	"$n wears $p on $s feet.",
	NULL, NULL, TRUE, &gn_wear_feet },
    { 0, "broken", "<worn as a bug>  ",
	"You buggily wear $p",
	"$n buggily wears $p",
	NULL, NULL, FALSE, NULL },
    { 0, "broken", "<worn as a bug>  ",
	"You buggily wear $p",
	"$n buggily wears $p",
	NULL, NULL, FALSE, NULL },
    { 0, "broken", "<worn as a bug>  ",
	"You buggily wear $p",
	"$n buggily wears $p",
	NULL, NULL, FALSE, NULL }
};



/*
 * Local functions.
 */
char *	format_obj_to_char	args( ( OBJ_DATA *obj, CHAR_DATA *ch,
				    bool fShort ) );
bool	show_list_to_char	args( ( OBJ_DATA *list, CHAR_DATA *ch,
				    bool fShort, bool fShowNothing,
				    bool fLargeOnly ) );
void	show_char_to_char_0	args( ( CHAR_DATA *victim, CHAR_DATA *ch ) );
void	show_char_to_char_1	args( ( CHAR_DATA *victim, CHAR_DATA *ch,
				    bool fPeek ) );
void	show_char_eq_to_char	args( ( CHAR_DATA *victim, CHAR_DATA *ch,
				    bool fPeek ) );
bool	show_char_to_char	args( ( CHAR_DATA *list, CHAR_DATA *ch ) );
bool	check_blind		args( ( CHAR_DATA *ch ) );



char *format_obj_to_char( OBJ_DATA *obj, CHAR_DATA *ch, bool fShort )
{
    static char buf[MAX_STRING_LENGTH];

    if (obj == NULL) {
	strcpy(buf, "(bug - format_obj_to_char - please report)\n\r" );
	return buf;
    }

    buf[0] = '\0';

    if (obj->pIndexData && 
	!IS_UNSWITCHED_MOB(ch) && IS_SET(TRUE_CH(ch)->act, PLR_HOLYVNUM)) 
	sprintf(buf,"[%d] ", obj->pIndexData->vnum);

    if (IS_OBJ_STAT(obj, ITEM_INVIS))   
	strcat(buf, " (invis)");
    
    if (fShort) {
	if ( obj->short_descr != NULL )
	    strcat( buf, obj->short_descr );
    } else {
	if ( obj->look_descr != NULL )
	    strcat( buf, obj->look_descr );
    }

    
    if (IS_AFFECTED(ch, AFF_DETECT_MAGIC)
	&& IS_OBJ_STAT(obj, ITEM_MAGIC))   
	strcat(buf, " ...it glows with a blue light!"   );
    if (IS_OBJ_STAT(obj, ITEM_GLOW))   
	strcat(buf, " ...it has a soft glowing aura!"   );
    if (IS_OBJ_STAT(obj, ITEM_DARK))   
	strcat(buf, " ...it seems to absorb light!"   );
    if (IS_OBJ_STAT(obj, ITEM_HUM))   
	strcat(buf, " ...it emits a faint humming sound!"   );

    return buf;
}



/*
 * Show a list to a character.
 * Can coalesce duplicated items.
 */
bool show_list_to_char( OBJ_DATA *list, CHAR_DATA *ch, bool fShort,
  bool fShowNothing, bool fLargeOnly )
{
    char buf[MAX_STRING_LENGTH];
    char **prgpstrShow;
    int *prgnShow;
    char *pstrShow;
    OBJ_DATA *obj;
    int nShow;
    int iShow;
    int count;
    bool fCombine;
    bool found = FALSE;

    if ( ch->desc == NULL )
	return FALSE;

    /*
     * Alloc space for output lines.
     */
    count = 0;
    for ( obj = list; obj != NULL; obj = obj->next_content )
	count++;
    prgpstrShow	= alloc_mem( count * sizeof(char *) );
    prgnShow    = alloc_mem( count * sizeof(int)    );
    nShow	= 0;

    /*
     * Format the list of objects.
     */
    for ( obj = list; obj != NULL; obj = obj->next_content )
    { 
        if ( fLargeOnly && !IS_SET( obj->extra_flags, ITEM_VISIBLE_DISTANCE ) )
	  continue;

	if ( obj->wear_loc == gn_wear_none && can_see_obj( ch, obj ) )
	{
            found = TRUE;

	    pstrShow = format_obj_to_char( obj, ch, fShort );
	    fCombine = FALSE;

	    if ( IS_NPC(ch) || IS_SET(ch->act, PLR_COMBINE) )
	    {
		/*
		 * Look for duplicates, case sensitive.
		 * Matches tend to be near end so run loop backwords.
		 */
		for ( iShow = nShow - 1; iShow >= 0; iShow-- )
		{
		    if ( !strcmp( prgpstrShow[iShow], pstrShow ) )
		    {
			prgnShow[iShow]++;
			fCombine = TRUE;
			break;
		    }
		}
	    }

	    /*
	     * Couldn't combine, or didn't want to.
	     */
	    if ( !fCombine )
	    {
		prgpstrShow [nShow] = str_dup( pstrShow );
		prgnShow    [nShow] = 1;
		nShow++;
	    }
	}
    }

    /*
     * Output the formatted list.
     */
    for ( iShow = 0; iShow < nShow; iShow++ )
    {
	if ( IS_NPC(ch) || IS_SET(ch->act, PLR_COMBINE) )
	{
	    if ( prgnShow[iShow] != 1 )
	    {
		sprintf( buf, "(%2d) ", prgnShow[iShow] );
		send_to_char( buf, ch );
	    }
	    else
	    {
		send_to_char( "     ", ch );
	    }
	}
	send_to_char( prgpstrShow[iShow], ch );
	send_to_char( "\n\r", ch );
	free_string( prgpstrShow[iShow], MEM_GENERAL );
    }

    if ( fShowNothing && nShow == 0 )
    {
	if ( IS_NPC(ch) || IS_SET(ch->act, PLR_COMBINE) )
	    send_to_char( "     ", ch );
	send_to_char( "Nothing.\n\r", ch );
    }

    /*
     * Clean up.
     */
    free_mem( prgpstrShow, count * sizeof(char *) );
    free_mem( prgnShow,    count * sizeof(int)    );

    return found;
}



void show_char_to_char_0( CHAR_DATA *victim, CHAR_DATA *ch )
{
    CHAR_DATA *rch;
    char buf[MAX_STRING_LENGTH], buf2[MAX_STRING_LENGTH];
    int position;

    buf[0] = '\0', buf2[0] = '\0';


    if (IS_NPC(victim)) {
	if (HAS_HOLYVNUM(ch))
	    sprintf(buf,"[%d] ", victim->pIndexData->vnum);
    } else {
	if (HAS_HOLYNAME(ch))
	    sprintf(buf,"[%s] ", victim->name);
    }

    if (IS_SET(victim->act, PLR_AFK) && HAS_HOLYLIGHT(ch))
	strcat(buf, "(AFK) ");
    
    if (IS_AFFECTED(victim, AFF_INVISIBLE))
	strcat(buf, "*");

    if (IS_AFFECTED(victim, AFF_HIDE) && IS_SET( ch->act, PLR_HOLYLIGHT ) 
)
	strcat(buf, "%");
    
    if (check_fighting(victim)) {
	strcat( buf, person( ch, victim ) );
	strcat( buf, " is here, fighting " );
	if ( victim->fighting == NULL )
	    strcat( buf, "for his life." );
	else if ( victim->fighting == ch )
	    strcat( buf, "YOU!" );
	else if ( victim->in_room == victim->fighting->in_room ) {
	    if (IS_NPC(victim->fighting)) {
		if (HAS_HOLYVNUM(ch)) {
		    sprintf(buf2,"[%d] ", victim->fighting->pIndexData->vnum);
		    strcat (buf, buf2);
		}	    
	    } else {
		if (HAS_HOLYNAME(ch)) {
		    sprintf(buf2,"[%s] ", victim->fighting->name);
		    strcat(buf, buf2); 
		}
	    }
	    strcat( buf, person( ch, victim->fighting ) );
	    strcat( buf, "." );
	}
	else
	    strcat( buf, "somone who left??" );
	
	strcat( buf, "\n\r" );
    }
    else if ( IS_NPC( victim ) && IS_SET( victim->act, ACT_MOUNT ) ) {
	for ( rch = victim->in_room->people;
	      rch != NULL; rch = rch->next_in_room ) {
	    if ( rch->riding == victim ) {
		if ( rch == ch ) {
		    act( "$n is here, being ridden by you.",
			 victim, ch, NULL, NULL, NULL, TO_VICT, SENSE_SIXTH );
		    break;
		}
		else
		    return;
            }
	}
    }
    else if ( victim->riding != NULL ) {
	strcat( buf, person( ch, victim ) );
	strcat( buf, " is here riding " );
	strcat( buf, person( ch, victim->riding ) );
	strcat( buf, ".\n\r" );
    }
    else if ( victim->dynamic_descr
	      && can_see( ch, victim ) == VISION_NORMAL ) {
	strcat( buf, victim->dynamic_descr );
    }
    else if ( ( !IS_NPC( victim )
		|| ( victim->pIndexData ?
		     victim->pIndexData->start_pos == victim->position :
		     victim->position == POS_STANDING ) )
	      && victim->long_descr[0] != '\0'
	      && can_see( ch, victim ) == VISION_NORMAL ) {
	strcat( buf, victim->long_descr );
    } else {
	strcat( buf, person( ch, victim ) );
	
	position	= victim->position;
	
	switch ( position ) {
	    case POS_DEAD:     strcat( buf, " is DEAD!!" );              break;
	    case POS_MORTAL:   strcat( buf, " is mortally wounded." );   break;
	    case POS_INCAP:    strcat( buf, " is incapacitated." );      break;
	    case POS_STUNNED:  strcat( buf, " is lying here stunned." ); break;
	    case POS_SLEEPING: strcat( buf, " is sleeping here." );      break;
	    case POS_RESTING:  strcat( buf, " is resting here." );       break;
	    case POS_SITTING:  strcat( buf, " is sitting here." );       break;
	    case POS_STANDING: strcat( buf, " is standing here." );      break;
	}

	strcat( buf, "\n\r" );
    }

    if ( IS_AFFECTED(victim, AFF_PASS_DOOR)   ) {
	strcat( buf, person( ch, victim ) );
	strcat( buf, " appears to be semi-solid.\n\r" );
    }

    if ( IS_AFFECTED(victim, AFF_SANCTUARY)   ) {
	strcat( buf, person( ch, victim ) );
	strcat( buf, " glows with a bright light.\n\r" );
    }

    buf[0] = UPPER(buf[0]);
    send_to_char( buf, ch );
    return;
}



void show_char_to_char_1( CHAR_DATA *victim, CHAR_DATA *ch, bool fPeek )
{
    char buf[MAX_STRING_LENGTH];
    int percent;

    if ( can_see( victim, ch ) == TRUE && ch != victim )
    {
      if ( !fPeek || number_percent() > get_skill( ch, gsn_peek ) )
      {
       if ( fPeek && number_percent() > get_skill( ch, gsn_peek ) )
	act( "$n looks at you closely.", ch, victim, NULL, NULL, NULL, TO_VICT, SENSE_SIGHT );
       else
	act( "$n looks at you.", ch, victim, NULL, NULL, NULL, TO_VICT, SENSE_SIGHT );

	act( "$n looks at $N.",  ch, victim, NULL, NULL, NULL, TO_NOTVICT, SENSE_SIGHT );
      }
    }

    if ( victim->description[0] != '\0' )
    {
	send_to_char( victim->description, ch );
    }
    else
    {
	act( "You see nothing special about $M.", ch, victim, NULL, NULL, NULL, TO_CHAR, SENSE_SIXTH );
    }

    if ( victim->max_hit > 0 )
	percent = ( 100 * victim->hit[HIT_LOC_BODY] ) / victim->max_hit;
    else
	percent = -1;

    strcpy( buf, person(ch, victim) );

         if ( percent >= 100 ) strcat( buf, " is in perfect condition.\n\r");
    else if ( percent >=  90 ) strcat( buf, " has a few scratches.\n\r"    );
    else if ( percent >=  80 ) strcat( buf, " has a small wounds and bruises.\n\r" );
    else if ( percent >=  70 ) strcat( buf, " has several wounds.\n\r"    );
    else if ( percent >=  60 ) strcat( buf, " has several wounds.\n\r"    );
    else if ( percent >=  50 ) strcat( buf, " has big nasty wounds.\n\r"  );
    else if ( percent >=  40 ) strcat( buf, " has big nasty wounds.\n\r"  );
    else if ( percent >=  30 ) strcat( buf, " is pretty hurt.\n\r"        );
    else if ( percent >=  20 ) strcat( buf, " is in bad shape.\n\r"       );
    else if ( percent >=  10 ) strcat( buf, " is in awful condition.\n\r" );
    else                       strcat( buf, " is almost dead.\n\r"        );

    buf[0] = UPPER(buf[0]);
    send_to_char( buf, ch );

    show_char_eq_to_char( ch, victim, fPeek );
}


void show_char_eq_to_char( CHAR_DATA *ch, CHAR_DATA *victim, bool fPeek )
{
    bool found = FALSE;
    OBJ_DATA *obj;
    int iWear;

    for ( iWear = 0; iWear < MAX_WEAR; iWear++ )
    {
	for ( obj = victim->wearing[iWear]; obj != NULL; obj = obj->next_worn )
	{
	    if ( obj == NULL )
		continue;

	    if ( obj->wear_loc != iWear )
		continue;

	    if ( !can_see_obj( ch, obj ) )
		continue;

	    if ( wear_info[iWear].covered_by != NULL
	    && victim->wearing[*wear_info[iWear].covered_by] != NULL
	    && ( !fPeek || number_percent() > get_skill( ch, gsn_peek ) ) )
		continue;

	    if ( !found )
	    {
		send_to_char( "\n\r", ch );
		act( "$N is using:", ch, victim, NULL, NULL, NULL, TO_CHAR, SENSE_SIXTH );
		found = TRUE;
	    }
	    send_to_char( wear_info[iWear].worn, ch );
	    send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
	    send_to_char( "\n\r", ch );

	    /* only show top piece. */
	    if ( wear_info[iWear].covers == TRUE )
		while( obj->next_worn != NULL )
		    obj = obj->next_worn;
	}
    }

    if ( victim != ch && fPeek )
    {
	OBJ_DATA *obj_peek;
	OBJ_DATA *obj_peek_next;

	send_to_char( "\n\rYou attempt to peek at their inventory:\n\r", ch );

	for ( obj_peek = victim->carrying ; obj_peek != NULL ; obj_peek = obj_peek_next )
	{
	    obj_peek_next = obj_peek->next_content;

	    if ( obj_peek->wear_loc == gn_wear_none && can_see_obj( ch, obj_peek )
		&& number_percent() < get_skill( ch, gsn_peek ) )
	    {
		send_to_char(format_obj_to_char( obj_peek, ch, TRUE ), ch );
		send_to_char("\n\r", ch );
	    }
	    else
	    {
		skill_improve(ch,gsn_peek);
	    }
	}
    }

    return;
}



bool show_char_to_char( CHAR_DATA *list, CHAR_DATA *ch )
{
    CHAR_DATA *rch;
    bool found = FALSE;

    for ( rch = list; rch != NULL; rch = rch->next_in_room )
    {
	if ( rch == ch )
	    continue;

	if ( !IS_NPC(rch)
	&&   IS_SET(rch->act, PLR_WIZINVIS)
	&&   !IS_IMMORTAL(ch) )
	    continue;

	if ( can_see( ch, rch ) != FALSE )
	{
	    show_char_to_char_0( rch, ch );
	    found = TRUE;
        }
    }

    return found;
} 



bool check_blind( CHAR_DATA *ch )
{
    if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_HOLYLIGHT) )
	return TRUE;

    if ( IS_AFFECTED(ch, AFF_BLIND) )
    {
	send_to_char( "You can't see a thing!\n\r", ch );
	return FALSE;
    }

    return TRUE;
}



void do_look( CHAR_DATA *ch, char *argument )
{
    char buf  [MAX_STRING_LENGTH], buf2 [20];
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    EXIT_DATA *pexit;
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    ROOM_INDEX_DATA *in_room;
    ROOM_INDEX_DATA *look_room;
    char *pdesc;
    int door;
    
    int dist, idist;

    if ( ch->desc == NULL )
	return;

    if ( ch->position < POS_SLEEPING )
    {
	send_to_char( "You can't see anything but stars!\n\r", ch );
	return;
    }

    if ( ch->position == POS_SLEEPING )
    {
	send_to_char( "You can't see anything, you're sleeping!\n\r", ch );
	return;
    }

    if ( !check_blind( ch ) )
	return;

    if ( !IS_NPC(ch)
    &&   !IS_SET(ch->act, PLR_HOLYLIGHT)
    &&   room_is_dark( ch->in_room, ch ) == VISION_TOO_DARK )
    {
	send_to_char( "It is pitch black ... \n\r", ch );
	show_char_to_char( ch->in_room->people, ch );
	return;
    }

    if ( !IS_NPC(ch)
    &&   !IS_SET(ch->act, PLR_HOLYLIGHT)
    &&   room_is_dark( ch->in_room, ch ) == VISION_TOO_BRIGHT )
    {
	send_to_char( "You are blinded in the light!\n\r", ch );
	show_char_to_char( ch->in_room->people, ch );
	return;
    }

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || !str_cmp( arg1, "auto" ) )
    {
	/* 'look' or 'look auto' */


     if (HAS_HOLYLIGHT(ch)) {
	 sprintf(buf2,"[%d] ", ch->in_room->vnum);
	 send_to_char(buf2, ch);
     }
     send_to_char( ch->in_room->name, ch );
     send_to_char( " ", ch );
     
     do_exits( ch, "auto" );
     
/* npcs are always in brief */
     if ( arg1[0] == '\0' ||
	  ( /*!IS_NPC( ch ) &&*/ !IS_SET( ch->act, PLR_BRIEF ) ) )
	 send_to_char( ch->in_room->description, ch );

     

     update_flora( ch->in_room );
     display_flora( ch, ch->in_room->flora, 0 );
     
     show_list_to_char( ch->in_room->contents, ch, FALSE, FALSE, FALSE );
     show_char_to_char( ch->in_room->people,   ch );
     return;
    }
    
    if ( !str_cmp( arg1, "i" ) || !str_cmp( arg1, "in" ) ) {
	/* 'look in' */
	if ( arg2[0] == '\0' )
	{
	    send_to_char( "Look in what?\n\r", ch );
	    return;
	}

	if ( ( obj = get_obj_here( ch, arg2 ) ) == NULL )
	{
	    send_to_char( "You do not see that here.\n\r", ch );
	    return;
	}

	switch ( obj->item_type )
	{
	default:
	    if ( obj->capacity <= 0 )
	    {
		send_to_char( "That is not a container.\n\r", ch );
		break;
	    }
	    act( "$p contains:",
	      ch, NULL, obj, NULL, NULL, TO_CHAR, SENSE_SIXTH );
	    show_list_to_char( obj->contains, ch, TRUE, TRUE, FALSE );
	    break;

	case ITEM_DRINK_CON:
	    if ( obj->value[1] <= 0 )
	    {
		send_to_char( "It is empty.\n\r", ch );
		break;
	    }

	    sprintf( buf, "It's %s full of a %s liquid.\n\r",
		obj->value[1] <     obj->value[0] / 4
		    ? "less than" :
		obj->value[1] < 3 * obj->value[0] / 4
		    ? "about"     : "more than",
		liq_table[obj->value[2]].liq_color
		);

	    send_to_char( buf, ch );
	    break;

	case ITEM_CONTAINER:
	case ITEM_CORPSE:
	    if ( IS_SET(obj->value[1], CONT_CLOSED) )
	    {
		send_to_char( "It is closed.\n\r", ch );
		break;
	    }

	    act( "$p contains:", ch, NULL, obj, NULL, NULL, TO_CHAR, SENSE_SIXTH );
	    show_list_to_char( obj->contains, ch, TRUE, TRUE, FALSE );
	    break;
	}
	return;
    }

	if ( !str_cmp( arg1, "sky" ) )
	{
	char *suf;
    int day;	

    day     = time_info.day + 1;

         if ( day > 4 && day <  20 ) suf = "th";
    else if ( day % 10 ==  1       ) suf = "st";
    else if ( day % 10 ==  2       ) suf = "nd";
    else if ( day % 10 ==  3       ) suf = "rd";
    else                             suf = "th";


    if ( IS_OUTSIDE(ch) )
    {
	send_to_char( "You can't see the sky indoors.\n\r", ch );
	return;
    }
        send_to_char("Looking at the sky you see:\n\r\n\r",ch); 

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
        send_to_char("The sand whirls freely about in the air, pelting things.",ch); 
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

        send_to_char("\n\r",ch); 

        }

        else if (weather_info.sky == SKY_CLOUDY)
        {
        sprintf( buf, "%s.\n\r",
        weather_info.change >= 0
        ? "A soft rumble rolls overhead through the cloud-covered sky."
        : "The skies are covered with calm looking clouds." );
        send_to_char( buf, ch );
        }
                 
        				 else if (weather_info.sky == SKY_CLOUDLESS )
				 {
					 send_to_char("The olive colored sky doesn't have a single cloud in it. \n\r",ch);
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
/*    
sprintf( buf2,
	"It is %d o'clock %s\n\r",
	(time_info.hour % 12 == 0) ? 12 : time_info.hour %12,
	time_info.hour >= 12 ? "pm" : "am");
    send_to_char( buf2, ch );*/
    
        
        if ( time_info.hour == 12 )
        {
        send_to_char("It is mid-day as the sun sets high in the sky.\n\r",ch) ;
        }
        else if ( time_info.hour >= 13 && time_info.hour <= 15 )
        {
        send_to_char("It is early afternoon.\n\r",ch);
        }
        else if ( time_info.hour >= 16 && time_info.hour <= 18 )
        {
        send_to_char("It is late afternoon.\n\r",ch);
        }
        else if ( time_info.hour >= 19 && time_info.hour <= 21 )
        {
        send_to_char("It is early evening.\n\r",ch);
        }
        else if ( time_info.hour >= 22 || time_info.hour <= 3  )
        {
        send_to_char("It is late night.\n\r",ch);
        }
        else if ( time_info.hour >= 4 && time_info.hour <= 6 )
        {
        send_to_char("It is early morning\n\r",ch);
        }
        else if ( time_info.hour >= 7 && time_info.hour <= 9 )
        {
        send_to_char("It is morning.\n\r",ch);
        }
        else if ( time_info.hour >= 10 && time_info.hour <= 11 )
        {
        send_to_char("It is late morning.\n\r",ch);
        }
        else
        {
    send_to_char("Bug in time command.\n\r",ch);
    sprintf(buf2, "It's supposed to be %d.\n\r", time_info.hour );
    send_to_char( buf2, ch );
        }
    
    return;
	}

    if ( !str_cmp( arg1, "out" ) )
    {
	bool fArg = TRUE;

	if ( arg2[0] == '\0' )
	    fArg = FALSE;

	for ( obj = ch->in_room->contents; obj != NULL; obj = obj->next_content )
	{
	    if ( obj->item_type != ITEM_BUILDING_WINDOW )
		continue;

	    if ( fArg && !is_name( arg2, obj->name ) )
		continue;
	}

	if ( obj == NULL )
	{
	    if ( fArg )
		send_to_char("Nothing like that to look out through.\n\r", ch );
	    else
		send_to_char("Nothing to look out through.\n\r", ch );
	    return;
	}
/* implement windows here */
	return;
    }

    if ( ( victim = get_char_room( ch, arg1 ) ) != NULL )
    {
	show_char_to_char_1( victim, ch,
	    ( !str_cmp( "peek", arg2 ) ? TRUE : FALSE ) );
	return;
    }

    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
    {
	if ( can_see_obj( ch, obj ) )
	{
	    pdesc = get_extra_descr( arg1, obj->extra_descr );
	    if ( pdesc != NULL )
	    {
		send_to_char( pdesc, ch );
		return;
	    }

	    pdesc = get_extra_descr( arg1, obj->pIndexData->extra_descr );
	    if ( pdesc != NULL )
	    {
		send_to_char( pdesc, ch );
		return;
	    }
	}

	if ( is_name( arg1, obj->name ) )
	{
	    send_to_char( obj->description, ch );
	    send_to_char("\n\r", ch );
	    return;
	}
    }

    for ( obj = ch->in_room->contents; obj != NULL; obj = obj->next_content )
    {
	if ( can_see_obj( ch, obj ) )
	{
	    pdesc = get_extra_descr( arg1, obj->extra_descr );
	    if ( pdesc != NULL )
	    {
		send_to_char( pdesc, ch );
		return;
	    }

	    pdesc = get_extra_descr( arg1, obj->pIndexData->extra_descr );
	    if ( pdesc != NULL )
	    {
		send_to_char( pdesc, ch );
		return;
	    }
	}

	if ( is_name( arg1, obj->name ) )
	{
	    send_to_char( obj->description, ch );
	    return;
	}
    }

    pdesc = get_extra_descr( arg1, ch->in_room->extra_descr );
    if ( pdesc != NULL )
    {
	send_to_char( pdesc, ch );
	return;
    }

    for ( door = 0; door < 6; door++ )
    {
	if ( (pexit = ch->in_room->exit[door]) == NULL )
	    continue;

	if ( pexit->keyword != NULL && !str_cmp( pexit->keyword, arg1 ) )
	    break;
    }

    if ( door == 6 && ((door = get_direction(arg1)) == -1) )
    {
	send_to_char( "You do not see that here.\n\r", ch );
	return;
    }

    /* 'look direction' */
    if ( ( pexit = ch->in_room->exit[door] ) == NULL )
    {
	send_to_char( "Nothing special there.\n\r", ch );
	return;
    }
/*    if ( IS_SET(pexit->exit_flags, EX_CLOSED) )
    {
        if ( pexit->cdescription != NULL && pexit->description[0] != '\0' )
        {
            send_to_char( pexit->cdescription, ch);
            return;
        }
        send_to_char("You see nothing special there.\n\r",ch);
        return;
    }*/

    if ( pexit->description != NULL && pexit->description[0] != '\0' )
	send_to_char( pexit->description, ch );
    else
	send_to_char( "Nothing special there.\n\r", ch );

    if ( IS_SET(pexit->exit_flags, EX_WALL) )
    {
	send_to_char("You can't see through walls.\n\r", ch );
	return;
    }

    dist = URANGE( 1, ch->vision_range, 400 ); // Was 200

    in_room = ch->in_room;
    look_room = in_room;
    idist = 0;
    for ( /* dist set above */ ; dist > 0; dist -= 40)
    {
	if ( (pexit = look_room->exit[door]) == NULL )
	    break;

	if ( pexit->to_room == NULL )
	    break;

	if ( IS_SET( pexit->exit_flags, EX_CLOSED ) )
	{
          if ( pexit->keyword    != NULL
            && pexit->keyword[0] != '\0' &&   pexit->keyword[0] != ' ' )
	    act( "The $d is closed.", ch, NULL, NULL,
	      pexit->keyword, NULL, TO_CHAR, SENSE_SIXTH );
          else
	    send_to_char("The door is closed.\n\r", ch );
	    break;
	}

	if ( IS_SET( pexit->exit_flags, EX_WALL ) )
	{
	    send_to_char("A wall blocks your view.\n\r", ch );
	    break;
	}

	if ( IS_SET( pexit->exit_flags, EX_ISDOOR ) )
	{
          if ( pexit->keyword    != NULL
            && pexit->keyword[0] != '\0' &&   pexit->keyword[0] != ' ' )
	    act( "The $d is open.", ch, NULL, NULL,
	      pexit->keyword, NULL, TO_CHAR, SENSE_SIXTH );
          else
	    send_to_char("The door is open.\n\r", ch );
	}

	if ( IS_SET( pexit->exit_flags, EX_FENCE ) )
	{
	    send_to_char("You look through the fence.\n\r", ch );
	    dist /= 2;
	}

	look_room = pexit->to_room;

	idist++;
	switch(idist)
	{
	    default:
	    case 1:
		send_to_char("[Near]\n\r", ch );
		break;
	    case 2:
		send_to_char("[Far]\n\r", ch );
		break;
	    case 3:
		send_to_char("[Very Far]\n\r", ch );
		break;
	    case 4:
		send_to_char("[Distant]\n\r", ch );
		break;
	    case 5:
		send_to_char("[Very Distant]\n\r", ch );
		break;
            case 6:
               send_to_char("[Extremely Far]\n\r",ch );
               break;
            case 7:
               send_to_char("[Extremely Distant]\n\r",ch );
               break;
            case 8:
               send_to_char("[Impossibly Far]\n\r",ch );
               break;
            case 9:
               send_to_char("[Impossibly Distant]\n\r",ch );
               break;
           case 10:
               send_to_char("[Ungodly Far]\n\r",ch );
               break;
	}

        {
	  bool found = FALSE;

          update_flora( look_room );
          found = display_flora( ch, look_room->flora, idist );

	  if ( look_room->people != NULL )
            found |= show_char_to_char( look_room->people, ch );

	  if ( look_room->contents != NULL )
            found |= show_list_to_char( look_room->contents, ch,
	      FALSE, FALSE, TRUE );

          if ( !found )
            send_to_char("Nothing.\n\r", ch );
        }
    }

    return;
}


void do_examine( CHAR_DATA *ch, char *argument )
{
  char buf[MAX_STRING_LENGTH];
  char arg[MAX_INPUT_LENGTH];
  OBJ_DATA *obj;
  OBJ_INDEX_DATA *pObjIndex;
  FLORA_DATA *fi;

  one_argument( argument, arg );

  if ( arg[0] == '\0' )
  {
    send_to_char( "Examine what?\n\r", ch );
    return;
  }

  do_look( ch, arg );

  if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
  {
    int percent;

    switch ( obj->item_type )
    {
      default:
        if ( obj->capacity <= 0 )
        break;

      case ITEM_DRINK_CON:
      case ITEM_CONTAINER:
      case ITEM_CORPSE:
        send_to_char( "When you look inside, you see:\n\r", ch );
        sprintf( buf, "in %s", arg );
        do_look( ch, buf );
    }

    if ( obj->pIndexData != NULL )
    {
      if ( obj->pIndexData->toughness <= 0 )
        percent = 100;
      else
        percent = obj->toughness * 100 / obj->pIndexData->toughness;
    }
    else
      percent = obj->toughness * 100 / 10000;

    if ( percent >= 100 )
      send_to_char("It is in perfect condition.\n\r", ch );
    else if ( percent >= 80 )
      send_to_char("It appears to be a little damaged.\n\r", ch );
    else if ( percent >= 60 )
      send_to_char("It is fairly beat up.\n\r", ch );
    else if ( percent >= 40 )
      send_to_char("It is heavily worn.\n\r", ch );
    else if ( percent >= 20 )
      send_to_char("It is poor condition.\n\r", ch );
    else if ( percent >= 5 )
      send_to_char("It is in terrible condition.\n\r", ch );
    else
      send_to_char("You'd best start looking for a new one.\n\r", ch );

    return;
  }

  fi = get_flora_here( ch, arg );
  if ( fi )
  {
    update_flora( ch->in_room );

    show_plant( ch, fi );

    if ( fi->flower > 0
    && ( pObjIndex = get_obj_index( fi->species->flower_vnum ) ) != NULL )
    {
      sprintf( buf, "%d %s decorate%s the plant.\n\r",
        fi->flower,
	( fi->flower > 1 )
	 ? plural( pObjIndex->short_descr ) : pObjIndex->short_descr,
        ( fi->flower > 1 ) ? "" : "s" );
      send_to_char( buf, ch );
    }

    if ( fi->fruit > 0
    && ( pObjIndex = get_obj_index( fi->species->fruit_vnum ) ) != NULL )
    {
      sprintf( buf, "%d %s hang%s to be picked.\n\r",
        fi->fruit,
	( fi->fruit > 1 )
	 ? plural( pObjIndex->short_descr ) : pObjIndex->short_descr,
        ( fi->fruit > 1 ) ? "" : "s" );
      send_to_char( buf, ch );
    }

    soil_condition( ch, fi->water );
  }

  if ( !str_cmp( arg, "soil" ) )
  {
    if ( !IS_SET( ch->in_room->sector->flags, SECTOR_OUTDOORS ) )
    {
      send_to_char( "You are indoors, there is no soil here.\n\r", ch );
      return;
    }

    soil_condition( ch, get_water_value( ch->in_room ) );

    return;
  }

  return;
}



void do_exits( CHAR_DATA *ch, char *argument )
{
    extern char * const dir_name[];
    extern char * const dir_letter[];
    char buf[MAX_STRING_LENGTH];
    EXIT_DATA *pexit;
    bool found;
    bool fAuto;
    int door;

    buf[0] = '\0';
    fAuto  = !str_cmp( argument, "auto" );

    if ( !check_blind( ch ) )
	return;

    strcpy( buf, fAuto ? "[" : "Obvious exits:\n\r" );

    found = FALSE;
    for ( door = 0; door <= 5; door++ )
    {
	if ( ( pexit = ch->in_room->exit[door] ) != NULL
	&&   pexit->to_room != NULL
	&&   !(  // IS_SET( pexit->exit_flags, EX_CLOSED )
	        IS_SET( pexit->exit_flags, EX_SECRET ))
	&& !IS_SET( pexit->exit_flags, EX_WALL ) ) 
       // && IS_SET( ch->act, PLR_HOLYLIGHT ))
	{
	    found = TRUE;
	    if ( fAuto )
	    {
              bool closed = FALSE;

              if ( IS_SET( pexit->exit_flags, EX_CLOSED ) )
              {
                strcat( buf, "(" );
                strcat( buf, dir_letter[door] );
		closed = TRUE;
              }
              else
	      {
	        if ( closed )
		  strcat( buf, ")" );
                strcat( buf, dir_letter[door] );
              }
	      if ( closed )
                strcat( buf, ")" );
	    }
	    else
	    {
		if ( IS_SET( pexit->exit_flags, EX_CLOSED ) )
		{
		    char dname[MAX_STRING_LENGTH];
		    const char *s;

		    if ( pexit->keyword != NULL && pexit->keyword[0] != '\0' )
		    {
			one_argument( (char *) pexit->keyword, dname );
			s = dname;
		    }
		    else
			s = "door";

		    sprintf( buf + strlen(buf), "%-5s - a closed %s.\n\r",
			capitalize( dir_name[door] ),
			    s );
		}
		else
		{
		  sprintf( buf + strlen(buf), "%-5s - %s\n\r",
		    capitalize( dir_name[door] ),
		    (room_is_dark( pexit->to_room, ch ) != VISION_NORMAL)
			?  "Hard to tell"
			: pexit->to_room->name
		    );
		}
	    }
	}
    }

    if ( !found )
	strcat( buf, fAuto ? "none" : "None.\n\r" );

    if ( fAuto )
	strcat( buf, "]\n\r" );

    send_to_char( buf, ch );
    return;
}



void do_score( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
/*
    AFFECT_DATA *paf;
*/
send_to_char("------=*  Darksun: Clash of the Sorceror Kings  *=------\n",ch);
send_to_char("*Character Name and Account*\n\n",ch);
sprintf( buf,	"Character: %s[Account not available]\n\n", 	ch->name );
send_to_char( buf, ch );

send_to_char("------=* Short Description and Long Description *=------         \n\n",ch);
sprintf(buf, "Sdesc: %s\nLdesc: %s\n\r", IS_NPC(ch) ? "" : ch->short_descr, IS_NPC(ch) ? "" : ch->long_descr );
send_to_char(buf, ch);

send_to_char("------=*             Race and Age               *=------\n\n",ch);
    sprintf( buf, "You are %s %s at %d years of age.\nClass: %s\n\n", 
    (is_vowel(race_array[ch->race]->race_name[0]) ? "an" : "a" ),
    race_array[ch->race]->race_name,
   get_age( ch ), class_table[ch->class].class_name );
    send_to_char( buf, ch );

    if ( get_trust( ch ) != ch->level || IS_IMMORTAL(ch) )
    {
send_to_char("------=*        Staff Information Only          *=------\n\n",ch);
	sprintf( buf, "You are staff trusted at level %d[%d].\n\r",
	    get_trust( ch ), ch->level );
	send_to_char( buf, ch );
    }
    
//   sprintf( buf, "||   Your strength is %s, your intelligence is %s,\n
    sprintf( buf,
	"You have %d/%d hit, %d/%d mana, %d/%d movement and %d/%d stun.\n\r",
	ch->hit[HIT_LOC_BODY],  ch->max_hit,
	ch->mana, ch->max_mana,
	ch->move, ch->max_move,
        ch->stun, ch->max_stun );
        
    send_to_char( buf, ch );

/* something about how heavy a load */

    sprintf( buf, "You have played for %d days and %d hours.\n\r",
	get_hours_played( ch ) / 24, (get_hours_played( ch ) % 24) );
    send_to_char( buf, ch );

    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK]   > 10 )
	send_to_char( "You are drunk.\n\r",   ch );
    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_THIRST] ==  0 )
	send_to_char( "You are thirsty.\n\r", ch );
    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_FULL]   ==  0 )
	send_to_char( "You are hungry.\n\r",  ch );

    switch ( ch->position )
    {
    case POS_DEAD:     
	send_to_char( "You are dead!!\n\r",		ch );
	break;
    case POS_MORTAL:
	send_to_char( "You are mortally wounded.\n\r",	ch );
	break;
    case POS_INCAP:
	send_to_char( "You are incapacitated.\n\r",	ch );
	break;
    case POS_STUNNED:
	send_to_char( "You are stunned.\n\r",		ch );
	break;
    case POS_SLEEPING:
	send_to_char( "You are sleeping.\n\r",		ch );
	break;
    case POS_RESTING:
	send_to_char( "You are resting.\n\r",		ch );
	break;
    case POS_SITTING:
	send_to_char( "You are sitting.\n\r",		ch );
	break;
    case POS_STANDING:
	send_to_char( "You are standing.\n\r",		ch );
	break;
    case POS_FIGHTING:
	send_to_char( "You are fighting.\n\r",		ch );
	break;
    }

    switch( ch->move_speed )
    {
      case MOVE_SNEAK:
        send_to_char( "You are sneaking.\n\r", ch );
	break;
      case MOVE_RUN:
        send_to_char( "You are running.\n\r", ch );
	break;
      case MOVE_WALK:
      default:
        send_to_char( "You are walking.\n\r", ch );
	break;
    }

    sprintf( buf, "You are speaking %s.\n\r",
      skill_table[*language_table[ch->language].pgsn].name );
    send_to_char( buf, ch );

/* alignment */

/* spell affects that are obvious */
    if ( IS_AFFECTED(ch, AFF_SANCTUARY) )
	send_to_char("You are surrounded by a bright aura.\n\r", ch );

    return;
}



char *	const	day_name	[] =
{
    "Eltanin", "Challenger", "Armageddon", "the Outpost", "Genocide",
    "Copper", "the Mercenary"
};

char *	const	month_name	[] =
{
    "Winter", "the Winter Wolf", "the Frost Giant", "the Old Forces",
    "the Grand Struggle", "the Spring", "Nature", "Futility", "the Dragon",
    "the Sun", "the Heat", "the Battle", "the Dark Shades", "the Shadows",
    "the Long Shadows", "the Ancient Darkness", "the Great Evil"
};

void do_time( CHAR_DATA *ch, char *argument )
{
    extern char str_boot_time[];
    char buf[MAX_STRING_LENGTH];
    char *suf;
    int day;

    day     = time_info.day + 1;

         if ( day > 4 && day <  20 ) suf = "th";
    else if ( day % 10 ==  1       ) suf = "st";
    else if ( day % 10 ==  2       ) suf = "nd";
    else if ( day % 10 ==  3       ) suf = "rd";
    else                             suf = "th";

	if (!IS_IMMORTAL(ch) )
	{
		send_to_char("The time command is no longer available, to see what time it is.. look at the sky.\n\r",ch);
		return;
	}

	send_to_char("*Time is disabled for chars*\n\r",ch);
    sprintf( buf,
	"It is %d o'clock %s, Day of %s, %d%s the Month of %s.\n\rDarksun MUD started up at %s\rThe system time is %s\r",

	(time_info.hour % 12 == 0) ? 12 : time_info.hour % 12,
	time_info.hour >= 12 ? "pm" : "am",
	day_name[day % 7],
	day, suf,
	month_name[time_info.month],
	str_boot_time,
	(char *) ctime( &current_time )
	);

    send_to_char( buf, ch );
    return;
}







void do_help( CHAR_DATA *ch, char *argument )
{
    char argall[MAX_INPUT_LENGTH];
    char argone[MAX_INPUT_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    HELP_DATA *pHelp;
    char buf[MAX_STRING_LENGTH];
    int col;

    if ( argument[0] == '\0' )
	argument = "summary";

    one_argument( argument, arg );

    /*
     * Tricky argument handling so 'help a b' doesn't match a.
     */
    argall[0] = '\0';
    while ( argument[0] != '\0' )
    {
	argument = one_argument( argument, argone );
	if ( argall[0] != '\0' )
	    strcat( argall, " " );
	strcat( argall, argone );
    }

    if ( !str_cmp( argall, "index" ) )
    {
	send_to_char("Index of helps:\n\r", ch );

	col = 0;
	for ( pHelp = help_first ; pHelp != NULL ; pHelp = pHelp->next )
	{
	    if ( pHelp->level <= get_trust(ch) )
	    {
		sprintf( buf, "%-25s", pHelp->keyword );
		send_to_char( buf, ch );
		if (++col % 3 == 0)
		    send_to_char("\n\r", ch );
	    }
	}

	if (++col % 3 != 0)
	    send_to_char("\n\r", ch );

	return;
    }

    for ( pHelp = help_first; pHelp != NULL; pHelp = pHelp->next )
    {
	if ( pHelp->level > get_trust( ch ) )
	    continue;

/*
	if ( is_name( argall, pHelp->keyword ) )
*/
	if ( !str_prefix( arg, pHelp->keyword ) )
	{
	    if ( pHelp->level >= 0 && str_cmp( argall, "imotd" ) )
	    {
		send_to_char( pHelp->keyword, ch );
		send_to_char( "\n\r", ch );
	    }

	    /*
	     * Strip leading '.' to allow initial blanks.
	     */
	    if ( pHelp->text[0] == '.' )
		send_to_char( pHelp->text+1, ch );
	    else
		send_to_char( pHelp->text  , ch );
	    return;
	}
    }

    send_to_char( "No help on that word.\n\r", ch );
    return;
}



void do_prompt( CHAR_DATA *ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];

   if ( argument[0] == '\0' )
   {
       return;
   }

   if( !strcmp( argument, "all" ) )
      strcpy( buf, "%h %m %v> ");
   else
   {
      if ( strlen(argument) > 50 )
         argument[50] = '\0';
      strcpy( buf, argument );
   }

   free_string( ch->prompt, MEM_GENERAL );
   ch->prompt = str_dup( buf );
   send_to_char( "Ok.\n\r", ch );
   return;
}
 

/*
 * New 'who' command originally by Alander of Rivers of Mud.
 */
void do_who( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
    int nNumber;
    int nMatch;
    bool fImmortalOnly;
 
    /*
     * Set default arguments.
     */
    fImmortalOnly  = FALSE;

    /*
     * Parse arguments.
     */
    nNumber = 0;
    for ( ;; )
    {
	char arg[MAX_STRING_LENGTH];

	argument = one_argument( argument, arg );
	if ( arg[0] == '\0' )
	    break;

	{
	    /*
	     * Look for flags to turn on.
	     */
	    if ( !str_cmp( arg, "imm" ) )
	    {
		fImmortalOnly = TRUE;
	    }
	}
    }

    /*
     * Now show matching chars.
     */
    nMatch = 0;
    buf[0] = '\0';
    for ( d = descriptor_list; d != NULL; d = d->next ) {
	CHAR_DATA *wch;
	char const *godly;
	
	/*
	 * Check for match against restrictions.
	 * Don't use trust as that exposes trusted mortals.
	 */
	if ( d->connected != CON_PLAYING || !can_see( ch, d->character ) )
	    continue;
	
	wch   = ( d->original != NULL ) ? d->original : d->character;
	if ( ( fImmortalOnly  && wch->level < LEVEL_IMMORTAL ) )
	    continue;
	
	nMatch++;
	
	/*
	 * Figure out what to print for god level.
	 */
	godly = "(GOD)";
/*	switch ( wch->level )
	{
	    default: godly = ""; break;
	    case MAX_LEVEL - 0: godly = "(GOD) ";     break;
	    case MAX_LEVEL - 1: godly = "(SUP) ";     break;
	    case MAX_LEVEL - 2: godly = "(LESSER) ";  break;
	    case MAX_LEVEL - 3: godly = "(BUILDER) "; break;
	    case MAX_LEVEL - 4: godly = "(WEK) ";     break;
	}
*/	
	/*
	 * Format it up.
	 */
	if ( IS_IMMORTAL( ch ) || wch->level > LEVEL_IMMORTAL )
	    sprintf( buf + strlen(buf), " %5s%5s%12s (%s)    <%d>\n\r",
		     (IS_SET(wch->act, PLR_AFK) ? "[AFK]" : " "),
                     (IS_SET(wch->act, PLR_WIZINVIS) ? "[INVIS]" : " "),
		     wch->name,
		     wch->short_descr, (IS_IMMORTAL(ch) ? wch->in_room->vnum : wch->level));                 


    }
    if (IS_IMMORTAL(ch) && (argument == '\0'))
    {
        sprintf( buf2, "%d player%s online.\n\r", nMatch, nMatch == 1 ? "" : "s" );
    send_to_char( buf2, ch );
    send_to_char( buf, ch );
    return;
    }

    sprintf( buf2, "There are %d player%s online.\n\rImmortals:\n\r", nMatch, nMatch == 1 ? "" : "s" );
    send_to_char( buf2, ch );
    send_to_char( buf, ch );
    return;
}



void do_inventory( CHAR_DATA *ch, char *argument )
{
    send_to_char( "You are carrying:\n\r", ch );
    show_list_to_char( ch->carrying, ch, TRUE, TRUE, FALSE );
    return;
}



void do_equipment( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    int iWear;
    bool found;

    send_to_char( "You are using:\n\r", ch );
    found = FALSE;
    for ( iWear = 0; iWear < MAX_WEAR; iWear++ )
    {
	for ( obj = ch->wearing[iWear]; obj != NULL; obj = obj->next_worn )
	{
	    if ( obj->wear_loc != iWear )
		continue;

	    send_to_char( wear_info[iWear].worn, ch );
	    if ( can_see_obj( ch, obj ) )
	    {
		send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
		send_to_char( "\n\r", ch );
	    }
	    else
	    {
		send_to_char( "something.\n\r", ch );
	    }
	    found = TRUE;
	}
    }

    if ( !found )
	send_to_char( "Nothing.\n\r", ch );

    return;
}



void do_compare( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *obj1;
    OBJ_DATA *obj2;
    int value1;
    int value2;
    char *msg;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    if ( arg1[0] == '\0' )
    {
	send_to_char( "Compare what to what?\n\r", ch );
	return;
    }

    if ( ( obj1 = get_obj_carry( ch, arg1, ch ) ) == NULL )
    {
	send_to_char( "You do not have that item.\n\r", ch );
	return;
    }

    if ( arg2[0] == '\0' )
    {
	for ( obj2 = ch->carrying; obj2 != NULL; obj2 = obj2->next_content )
	{
	    if ( obj2->wear_loc != gn_wear_none
	    &&   can_see_obj( ch, obj2 )
	    &&   obj1->item_type == obj2->item_type
	    && ( obj1->wear_flags & obj2->wear_flags & ~ITEM_TAKE) != 0 )
		break;
	}

	if ( obj2 == NULL )
	{
	    send_to_char( "You aren't wearing anything comparable.\n\r", ch );
	    return;
	}
    }
    else
    {
	if ( ( obj2 = get_obj_carry( ch, arg2, ch ) ) == NULL )
	{
	    send_to_char( "You do not have that item.\n\r", ch );
	    return;
	}
    }
	    
    msg		= NULL;
    value1	= 0;
    value2	= 0;

    if ( obj1 == obj2 )
    {
	msg = "You compare $p to itself.  It looks about the same.";
    }
    else if ( obj1->item_type != obj2->item_type )
    {
	msg = "You can't compare $p and $P.";
    }
    else
    {
	switch ( obj1->item_type )
	{
	default:
	    msg = "You can't compare $p and $P.";
	    break;

	case ITEM_ARMOR:
	    value1 = obj1->value[0];
	    value2 = obj2->value[0];
	    break;

	case ITEM_WEAPON:
	    value1 = obj1->value[1] * (obj1->value[2] + 1);
	    value2 = obj2->value[1] * (obj2->value[2] + 1);
	    break;
	}
    }

    if ( msg == NULL )
    {
	     if ( value1 == value2 ) msg = "$p and $P look about the same.";
	else if ( value1  > value2 ) msg = "$p looks better than $P.";
	else                         msg = "$p looks worse than $P.";
    }

    act( msg, ch, NULL, obj1, obj2, NULL, TO_CHAR, SENSE_SIXTH );
    return;
}



void do_credits( CHAR_DATA *ch, char *argument )
{
    do_help( ch, "diku" );
    return;
}



void do_where( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    DESCRIPTOR_DATA *d;
    bool found;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Players near you:\n\r", ch );
	found = FALSE;
	for ( d = descriptor_list; d; d = d->next )
	{
	    if ( d->connected == CON_PLAYING
	    && ( victim = d->character ) != NULL
	    &&   !IS_NPC(victim)
	    &&   victim->in_room != NULL
	    &&   victim->in_room->area == ch->in_room->area
	    &&   can_see( ch, victim ) )
	    {
		found = TRUE;
		sprintf( buf, "%-28s %s\n\r",
		    victim->name, victim->in_room->name );
		send_to_char( buf, ch );
	    }
	}
	if ( !found )
	    send_to_char( "None\n\r", ch );
    }
    else
    {
	found = FALSE;
	for ( victim = char_list; victim != NULL; victim = victim->next )
	{
	    if ( victim->in_room != NULL
	    &&   victim->in_room->area == ch->in_room->area
	    &&   !IS_AFFECTED(victim, AFF_HIDE)
	    &&   !IS_AFFECTED(victim, AFF_SNEAK)
	    &&   can_see( ch, victim )
	    &&   is_name( arg, victim->name ) )
	    {
		found = TRUE;
		sprintf( buf, "%-28s %s\n\r",
		    person(ch, victim), victim->in_room->name );
		send_to_char( buf, ch );
		break;
	    }
	}
	if ( !found )
	    act( "You didn't find any $t.", ch, NULL, NULL, arg, NULL, TO_CHAR, SENSE_SIXTH );
    }

    return;
}




void do_consider( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    send_to_char("Consider is currently disabled.\n\r", ch );
    return;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Consider killing whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They're not here.\n\r", ch );
	return;
    }

    return;
}



void do_description( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    if ( argument[0] != '\0' )
    {
	buf[0] = '\0';
	if ( argument[0] == '+' )
	{
	    if ( ch->description != NULL )
		strcat( buf, ch->description );
	    argument++;
	    while ( isspace(*argument) )
		argument++;
	}

	if ( strlen(buf) + strlen(argument) >= MAX_STRING_LENGTH - 2 )
	{
	    send_to_char( "Description too long.\n\r", ch );
	    return;
	}

	strcat( buf, argument );
	strcat( buf, "\n\r" );
	free_string( ch->description, MEM_GENERAL );
	ch->description = str_dup( buf );
    }

    send_to_char( "Your description is:\n\r", ch );
    send_to_char( ch->description ? ch->description : "(None).\n\r", ch );
    return;
}



void do_password( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char *pArg;
    char *pwdnew;
    char *p;
    char cEnd;

    if ( IS_NPC(ch) )
	return;

    /*
     * Can't use one_argument here because it smashes case.
     * So we just steal all its code.  Bleagh.
     */
    pArg = arg1;
    while ( isspace(*argument) )
	argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
	cEnd = *argument++;

    while ( *argument != '\0' )
    {
	if ( *argument == cEnd )
	{
	    argument++;
	    break;
	}
	*pArg++ = *argument++;
    }
    *pArg = '\0';

    pArg = arg2;
    while ( isspace(*argument) )
	argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
	cEnd = *argument++;

    while ( *argument != '\0' )
    {
	if ( *argument == cEnd )
	{
	    argument++;
	    break;
	}
	*pArg++ = *argument++;
    }
    *pArg = '\0';

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Syntax: password <old> <new>.\n\r", ch );
	return;
    }
    
         
    if ( strcmp( crypt( arg1, ch->pcdata->pwd ), ch->pcdata->pwd ) )
    {
	WAIT_STATE( ch, 40 );
	send_to_char( "Wrong password.  Wait 10 seconds.\n\r", ch );
	return;
    }

    if ( strlen(arg2) < 5 )
    {
	send_to_char(
	    "New password must be at least five characters long.\n\r", ch );
	return;
    }

    /*
     * No tilde allowed because of player file format.
     */
    pwdnew = crypt( arg2, ch->name );
    for ( p = pwdnew; *p != '\0'; p++ )
    {
	if ( *p == '~' )
	{
	    send_to_char(
		"New password not acceptable, try again.\n\r", ch );
	    return;
	}
    }

    free_string( ch->pcdata->pwd, MEM_PCDATA );
    ch->pcdata->pwd = str_dup( pwdnew );
    save_char_obj( ch );
    send_to_char( "Ok.\n\r", ch );
    return;
}



void do_socials( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int iSocial;
    int col;
 
    col = 0;
    for ( iSocial = 0; social_table[iSocial].name[0] != '\0'; iSocial++ )
    {
	sprintf( buf, "%-12s", social_table[iSocial].name );
	send_to_char( buf, ch );
	if ( ++col % 6 == 0 )
	    send_to_char( "\n\r", ch );
    }
 
    if ( col % 6 != 0 )
	send_to_char( "\n\r", ch );
    return;
}



/*
 * Contributed by Alander.
 */
void do_commands( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int cmd;
    int col;
 
    col = 0;
    for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
    {
        if ( cmd_table[cmd].level <  LEVEL_IMMORTAL
        &&   cmd_table[cmd].level <= get_trust( ch ) )
	{
	    sprintf( buf, "%-12s", cmd_table[cmd].name );
	    send_to_char( buf, ch );
	    if ( ++col % 6 == 0 )
		send_to_char( "\n\r", ch );
	}
    }
 
    if ( col % 6 != 0 )
	send_to_char( "\n\r", ch );
    return;
}



void do_channels( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {

	send_to_char( "Channels:", ch );

	send_to_char( !IS_SET(ch->deaf, CHANNEL_CHAT)
	    ? " +CHAT"
	    : " -chat",
	    ch );

	if ( IS_IMMORTAL(ch) )
	{
	    send_to_char( !IS_SET(ch->deaf, CHANNEL_IMMTALK)
		? " +IMMTALK"
		: " -immtalk",
		ch );
	}

	if ( IS_IMMORTAL(ch) )
	{
	    send_to_char( !IS_SET(ch->deaf, CHANNEL_LOG)
		? " +LOG"
		: " -log",
		ch );
	}

	send_to_char( ".\n\r", ch );
    }
    else
    {
	bool fClear;
	int bit;

	     if ( arg[0] == '+' ) fClear = TRUE;
	else if ( arg[0] == '-' ) fClear = FALSE;
	else
	{
	    send_to_char( "Channels -channel or +channel?\n\r", ch );
	    return;
	}

             if ( !str_cmp( arg+1, "chat"     ) ) bit = CHANNEL_CHAT;
	else if ( !str_cmp( arg+1, "immtalk"  ) ) bit = CHANNEL_IMMTALK;
	else if ( !str_cmp( arg+1, "log"      ) ) bit = CHANNEL_LOG;
	else
	{
	    send_to_char( "Set or clear which channel?\n\r", ch );
	    return;
	}

	if ( fClear )
	    REMOVE_BIT (ch->deaf, bit);
	else
	    SET_BIT    (ch->deaf, bit);

	send_to_char( "Ok.\n\r", ch );
    }

    return;
}


/*
 * Rip, Dec 10, 1995
 */
void do_brief( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
    {
    }
    else
    {
	if ( IS_SET(ch->act,PLR_BRIEF))
	{
	    REMOVE_BIT(ch->act,PLR_BRIEF);
	    send_to_char("Brief mode off.\n\r", ch );
	}
	else
	{
	    SET_BIT(ch->act,PLR_BRIEF);
	    send_to_char("Brief mode on.\n\r", ch );
	}
    }

    return;
}


/*
 * Rip, Dec 10, 1995
 */
void do_compact( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
    {
    }
    else
    {
	if ( IS_SET(ch->act,PLR_BLANK))
	{
	    REMOVE_BIT(ch->act,PLR_BLANK);
	    send_to_char("Compact mode on.\n\r", ch );
	}
	else
	{
	    SET_BIT(ch->act,PLR_BLANK);
	    send_to_char("Compact mode off.\n\r", ch );
	}
    }

    return;
}


void do_peaceful( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
    {
    }
    else
    {
	if ( IS_SET(ch->act,PLR_PEACEFUL))
	{
	    REMOVE_BIT(ch->act,PLR_PEACEFUL);
	    send_to_char("You now attack when attacked.\n\r", ch );
	}
	else
	{
	    SET_BIT(ch->act,PLR_PEACEFUL);
	    send_to_char("You are now passive when attacked.\n\r", ch );
	}
    }

    return;
}


void do_config( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];

    if ( IS_NPC(ch) )
	return;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
        send_to_char( "[ Keyword  ] Option\n\r", ch );

	send_to_char(  IS_SET(ch->act, PLR_BLANK)
	    ? "[+BLANK    ] You have a blank line before your prompt.\n\r"
	    : "[-blank    ] You have no blank line before your prompt.\n\r"
	    , ch );

	send_to_char(  IS_SET(ch->act, PLR_BRIEF)
	    ? "[+BRIEF    ] You see brief descriptions.\n\r"
	    : "[-brief    ] You see long descriptions.\n\r"
	    , ch );
         
	send_to_char(  IS_SET(ch->act, PLR_COMBINE)
	    ? "[+COMBINE  ] You see object lists in combined format.\n\r"
	    : "[-combine  ] You see object lists in single format.\n\r"
	    , ch );

	send_to_char(  IS_SET(ch->act, PLR_PEACEFUL)
	    ? "[+PEACEFUL ] You do not attack when attacked.\n\r"
	    : "[-peaceful ] You attack when attacked.\n\r"
	    , ch );

	send_to_char(  IS_SET(ch->act, PLR_PROMPT)
	    ? "[+PROMPT   ] You have a prompt.\n\r"
	    : "[-prompt   ] You don't have a prompt.\n\r"
	    , ch );

	send_to_char(  IS_SET(ch->act, PLR_TELNET_GA)
	    ? "[+TELNETGA ] You receive a telnet GA sequence.\n\r"
	    : "[-telnetga ] You don't receive a telnet GA sequence.\n\r"
	    , ch );

	send_to_char( !IS_SET(ch->act, PLR_NO_EMOTE)
	    ? ""
	    : "[-emote    ] You can't emote.\n\r"
	    , ch );

	send_to_char( !IS_SET(ch->act, PLR_NO_TELL)
	    ? ""
	    : "[-tell     ] You can't use 'tell'.\n\r"
	    , ch );
    }
    else
    {
	bool fSet;
	int bit;

	     if ( arg[0] == '+' ) fSet = TRUE;
	else if ( arg[0] == '-' ) fSet = FALSE;
	else
	{
	    send_to_char( "Config -option or +option?\n\r", ch );
	    return;
	}

	     if ( !str_cmp( arg+1, "blank"    ) ) bit = PLR_BLANK;
	else if ( !str_cmp( arg+1, "brief"    ) ) bit = PLR_BRIEF;
	else if ( !str_cmp( arg+1, "combine"  ) ) bit = PLR_COMBINE;
        else if ( !str_cmp( arg+1, "peaceful" ) ) bit = PLR_PEACEFUL;
        else if ( !str_cmp( arg+1, "prompt"   ) ) bit = PLR_PROMPT;
	else if ( !str_cmp( arg+1, "telnetga" ) ) bit = PLR_TELNET_GA;
	else
	{
	    send_to_char( "Config which option?\n\r", ch );
	    return;
	}

	if ( fSet )
	    SET_BIT    (ch->act, bit);
	else
	    REMOVE_BIT (ch->act, bit);

	send_to_char( "Ok.\n\r", ch );
    }

    return;
}


void do_set( CHAR_DATA *ch, char *argument )
{
  char buf[MAX_STRING_LENGTH];
  char arg[MAX_INPUT_LENGTH];
  char arg2[MAX_INPUT_LENGTH];
  int i;

  if ( IS_NPC(ch) )
    return;

  argument = one_argument( argument, arg );
  one_argument( argument, arg2 );

  if ( !str_cmp( arg, "pagelen" ) )
  {
    i = UMAX( atoi( arg2 ), 20 );
    sprintf( buf, "Page Length set to %d\n\r", i );
    send_to_char( buf, ch );
    ch->pcdata->pagelen = i;
    return;
  }

  send_to_char( "Syntax:  set <option> <value>\n\r", ch );
  sprintf( buf, "Page Length (pagelen):  %d\n\r", ch->pcdata->pagelen );
  send_to_char( buf, ch );

  return;
}
