/**************************************************************************/
// macros.h - most of the compiler macros will be defined in here
/**************************************************************************/
#ifndef MACROS_H
#define MACROS_H

/**************************************************************************/
// disable minor printf optimisation macro used in versions of GCC >= 2.97
// this macro creates problems when you have member functions called 
// printf() - such as in the char_data class
#ifdef __GNUC__
#  ifdef printf
#    if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 97) 
#      undef printf
       // macro is defined normally as below
       //#ifdef __GNUC_PREREQ (2,97)
       //#  define printf(fmt, args...) fprintf (stdout, fmt, ##args)
#    endif
#  endif
#endif

/**************************************************************************
 * #define alloc_mem(sz) malloc((size_t)sz)
 * #define free_mem(mem, sz) free(mem)
 **************************************************************************/

/*
* Utility macros.
*/
#define IS_NULLSTR(str)   ((str)==NULL || (str)[0]=='\0') 



/**************************************************************************/
/*
* Character macros.
*/

#define IS_SWITCHED(ch)			((ch)->desc && (ch)->desc->original)
#define IS_CONTROLLED(victim)	(IS_NPC(victim) && (victim)->desc != NULL)
#define HAS_DESC(ch)			((ch)->desc)

#define SAFE_DESC_CHARACTER(ch)	(ch->desc->character? ch->desc->character: ch)

#define TRUE_CH(ch)				(ch->desc ? \
	(ch->desc->original ? ch->desc->original : SAFE_DESC_CHARACTER(ch)):ch) 
#define TCH(ch) TRUE_CH(ch)

#define TRUE_CH_PCDATA(ch)		(TRUE_CH(ch)?TRUE_CH(ch)->pcdata:NULL)

#define CH(descriptor)			((descriptor)->original ? \
	(descriptor)->original : (descriptor)->character)

// #define IS_NPC(ch)              ((ch)->pcdata==NULL)
#define IS_UNSWITCHED_MOB(ch)	(IS_NPC(ch) && !IS_CONTROLLED(ch))
// #define IS_NPC(ch)		(IS_SET((ch)->act, ACT_IS_NPC))
#define IS_IMMORTAL(ch)		(get_trust(ch) >= LEVEL_IMMORTAL)
// #define IS_AFFECTED(ch, bit)    (IS_SET((ch)->affected_by, (bit)))
// #define IS_AFFECTED2(ch, bit)   (IS_SET((ch)->affected_by2, (bit)))
// #define IS_ADMIN(ch)			(TRUE_CH(ch)->level>=ADMIN)

#define HAS_HOLYNAME(ch)       (TRUE_CH(ch)->pcdata \
				&& IS_SET(ch->act,PLR_HOLYNAME))
#define HAS_HOLYLIGHT(ch)      (TRUE_CH(ch)->pcdata \
				&& IS_SET(ch->act,PLR_HOLYLIGHT))
#define HAS_HOLYSPEECH(ch)     (TRUE_CH(ch)->pcdata \
                                && IS_SET(ch->act,PLR_HOLYSPEECH))
#define HAS_HOLYWALK(ch)       (TRUE_CH(ch)->pcdata \
                                && IS_SET(ch->act,PLR_HOLYWALK))
#define HAS_HOLYVNUM(ch)       (TRUE_CH(ch)->pcdata \
                                && IS_SET(ch->act,PLR_HOLYVNUM))

#define IS_GOOD(ch)		(ch->alignment >= 350)
#define IS_EVIL(ch)		(ch->alignment <= -350)
#define IS_NEUTRAL(ch)		(!IS_GOOD(ch) && !IS_EVIL(ch))

#define IS_AWAKE(ch)		(ch->position > POS_SLEEPING)
#define GET_HITROLL(ch)		((ch)->hitroll)
#define GET_DAMROLL(ch)		((ch)->damroll)

#define IS_OUTSIDE(ch)		(!IS_SET(				    \
				    (ch)->in_room->sector->flags,	    \
				    SECTOR_OUTDOORS))
#define VIEW_OUTSIDE(ch)	(IS_SET(				    \
				      (ch)->in_room->sector->flags,	    \
				      SECTOR_OUTDOORS)                      \
				  || IS_SET( (ch)->in_room->sector->flags,  \
				       SECTOR_WINDOW ) )

#define CH_STAND_FIRST(action)  if ( ch->position != POS_STANDING )         \
  { do_stand( ch, "" );  WAIT_STATE( ch, 4 );  continue; }                  \
  else                                                                      \
    (action)



#define WAIT_STATE(ch, npulse)	((ch)->wait = UMAX((ch)->wait, (npulse)))
/*
#define IS_AWAKE(ch)            (ch->position > POS_SLEEPING)
#define IS_MOUNTED(ch)          ((ch)->ridden_by)
#define IS_RIDING(ch)			((ch)->mounted_on)
#define IS_TETHERED(ch)			(IS_NPC((ch)) && ((ch)->tethered))

#define DAMTYPE(sn)				(skill_table[sn].damtype)
*/


#define CAN_SEE_EXIT(ch, exit) (!IS_SET(exit->exit_info, EX_CLOSED) \
				|| HAS_HOLYLIGHT(ch) \
				|| IS_SET(ch->act, PLR_HOLYWALK)) \
				

// capitalized person
#define CPERS(ch, looker)		(capitalize(PERS(ch,looker)))

#define HSHR(ch) ((ch->sex == SEX_MALE) ? "his" : \
                  (ch->sex == SEX_FEMALE) ? "her" : "its")

#define HSSH(ch) ((ch->sex == SEX_MALE) ? "he" : \
                  (ch->sex == SEX_FEMALE) ? "she" : "it")

#define HMHR(ch) ((ch->sex == SEX_MALE) ? "him" : \
                  (ch->sex == SEX_FEMALE) ? "her" : "it")

/*
* Object macros.
*/


#define CAN_WEAR(obj, part)     (IS_SET((obj)->wear_flags,  (part)))
#define IS_OBJ_STAT(obj, stat)  (IS_SET((obj)->extra_flags, (stat)))

/**************************************************************************/
/**************************************************************************/
// reading in a a defined datatype macro
#if defined(KEY)
#undef KEY
#endif
#define KEY( literal, field, value )             \
             if ( !str_cmp( word, literal ) )    \
             {                                   \
                  field  = value;                \
                  fMatch = TRUE;                 \
                  break;                         \
             }
/**************************************************************************/
// reading in a string from a file macro
#if defined(SKEY)
#undef SKEY
#endif
#define SKEY( string, field )                       \
		if ( !str_cmp( word, string ) )     \
	        {                                   \
	            free_string( field );           \
       		    field = fread_string( fp );     \
		    fMatch = TRUE;                  \
		    break;                          \
      	        }

#endif // MACROS_H

