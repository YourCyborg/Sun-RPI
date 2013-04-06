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


extern MOB_INDEX_DATA *        mob_index_hash          [MAX_KEY_HASH];
extern OBJ_INDEX_DATA *        obj_index_hash          [MAX_KEY_HASH];
extern int                     top_affect;
extern int                     top_area;
extern int                     top_ed;
extern int                     top_exit;
extern int                     top_help;
extern int                     top_mob_index;
extern int                     top_obj_index;
extern int                     top_reset;
extern int                     top_room;
extern int                     top_shop;

/*
 * Locals.
 */
char *			string_space;
char *			top_string;
char			str_empty	[1];

/*
 * Memory management.
 * Increase MAX_STRING if you have too.  (moved to merc.h)
 * Tune the others only if you understand what you're doing.
 */
#define			MAX_PERM_BLOCK	131072
#define			MAX_MEM_LIST	11

void *			rgFreeList	[MAX_MEM_LIST];
const int		rgSizeList	[MAX_MEM_LIST]	=
{
    16, 32, 64, 128, 256, 1024, 2048, 4096, 8192, 16384, 32768-64
};

int			nAllocString;
int			sAllocString;
int			nAllocPerm;
int			sAllocPerm;


void init_string_space( void )
{
  if ( ( string_space = calloc( 1, MAX_STRING ) ) == NULL )
  {
    bug( "init_string_space: can't alloc %d string space.", MAX_STRING );
    exit( 1 );
  }
  top_string	= string_space;

  return;
}


/*
 * Allocate some ordinary memory,
 *   with the expectation of freeing it someday.
 */
void *alloc_mem( int sMem )
{
    void *pMem;
    int iList;

    for ( iList = 0; iList < MAX_MEM_LIST; iList++ )
    {
	if ( sMem <= rgSizeList[iList] )
	    break;
    }

    if ( iList == MAX_MEM_LIST )
    {
	bug( "Alloc_mem: size %d too large.", sMem );
	exit( 1 );
    }

    if ( rgFreeList[iList] == NULL )
    {
	pMem		  = alloc_perm( rgSizeList[iList] );
    }
    else
    {
	pMem              = rgFreeList[iList];
	rgFreeList[iList] = * ((void **) rgFreeList[iList]);
    }

    return pMem;
}



/*
 * Free some memory.
 * Recycle it back onto the free list for blocks of that size.
 */
void free_mem( void *pMem, int sMem )
{
    int iList;

    for ( iList = 0; iList < MAX_MEM_LIST; iList++ )
    {
	if ( sMem <= rgSizeList[iList] )
	    break;
    }

    if ( iList == MAX_MEM_LIST )
    {
	bug( "Free_mem: size %d too large.", sMem );
	exit( 1 );
    }

    * ((void **) pMem) = rgFreeList[iList];
    rgFreeList[iList]  = pMem;

    return;
}



/*
 * Allocate some permanent memory.
 * Permanent memory is never freed,
 *   pointers into it may be copied safely.
 */
void *alloc_perm( int sMem )
{
    static char *pMemPerm;
    static int iMemPerm;
    void *pMem;

    while ( sMem % sizeof(long) != 0 )
	sMem++;
    if ( sMem > MAX_PERM_BLOCK )
    {
	bug( "Alloc_perm: %d too large.", sMem );
	exit( 1 );
    }

    if ( pMemPerm == NULL || iMemPerm + sMem > MAX_PERM_BLOCK )
    {
	iMemPerm = 0;
	if ( ( pMemPerm = calloc( 1, MAX_PERM_BLOCK ) ) == NULL )
	{
	    perror( "Alloc_perm" );
	    exit( 1 );
	}
    }

    pMem        = pMemPerm + iMemPerm;
    iMemPerm   += sMem;
    nAllocPerm += 1;
    sAllocPerm += sMem;
    return pMem;
}



/*
 * Duplicate a string into dynamic memory.
 * Fread_strings are read-only and shared.
 */
char *str_dup( const char *str )
{
    char *str_new;

    if ( str[0] == '\0' )
	return &str_empty[0];

    if ( str >= string_space && str < top_string )
	return (char *) str;

    str_new = alloc_mem( strlen(str) + 1 );
    strcpy( str_new, str );
    return str_new;
}


/*
 * add or strip tabs.
 * fStrip is whether tabs are stripped or placed.
 */
char *str_dup_tab( const char *str, bool fStrip )
{
  char *str_new;
  char buf[MAX_STRING_LENGTH];
  char *pstr, *pbuf;
  char *endstr = (char *) ( str + strlen( str ) );

  if ( str[0] == '\0' )
    return &str_empty[0];

  pstr = (char *) str;
  pbuf = buf;
  while ( pstr < endstr )
  {
    if ( pstr[0] == '\t' && fStrip )
    {
      strncpy( pbuf, "     ", 5 );
      pbuf = &pbuf[5];
      pstr++;
      continue;
    }

    if ( pstr[0] == ' ' && !fStrip && &pstr[4] < endstr
    && pstr[1] == ' ' &&  pstr[2] == ' ' && pstr[3] == ' ' && pstr[4] == ' ' )
    {
      pbuf[0] = '\t';
      pbuf++;
      pstr = &pstr[5];
      continue;
    }

    pbuf[0] = pstr[0];
    pbuf++;
    pstr++;
  }

  pbuf[0] = '\0';

  str_new = alloc_mem( strlen(buf) + 1 );
  strcpy( str_new, buf );
  return str_new;
}



/*
 * Free a string.
 * Null is legal here to simplify callers.
 * Read-only shared strings are not touched.
 */
void free_string( char *pstr, int mem_type )
{
    if ( pstr == NULL
    ||   pstr == &str_empty[0]
    || ( pstr >= string_space && pstr < top_string ) )
	return;


    if ( find_string_references( pstr, mem_type ) )
	return;

    free_mem( pstr, strlen(pstr) + 1 );
    return;
}


bool find_string_references( char *pstr, int mem_type )
{
    int iHash;
    int count = 0;
    int i;
    SHOP_DATA *p;
    MOB_INDEX_DATA *pMobIndex;
    CHAR_DATA *ch;
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    AREA_DATA *pArea;
    extern AREA_DATA *area_first;

    switch( mem_type )
    {
	case MEM_MOB_NAME_FREE_CHAR:
	    count++;
	case MEM_MOB_NAME:
	    for ( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
		for ( pMobIndex = mob_index_hash[iHash];
		pMobIndex != NULL; pMobIndex = pMobIndex->next )
		    if ( pMobIndex->name == pstr ) {
			if ( count == 1 )
			    return TRUE;
			else
			    count++;               }
	    for ( ch = char_list; ch != NULL; ch = ch->next )
		    if ( ch->name == pstr ) {
			if ( count == 1 )
			    return TRUE;
			else
			    count++;        }
	    break;

	case MEM_MOB_NAMELIST_FREE_CHAR:
	    count++;
	case MEM_MOB_NAMELIST:
	    for ( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
		for ( pMobIndex = mob_index_hash[iHash];
		pMobIndex != NULL; pMobIndex = pMobIndex->next )
		    if ( pMobIndex->namelist == pstr ) {
			if ( count == 1 )
			    return TRUE;
			else
			    count++;                   }
	    for ( ch = char_list; ch != NULL; ch = ch->next )
		    if ( ch->namelist == pstr ) {
			if ( count == 1 )
			    return TRUE;
			else
			    count++;            }
	    break;

	case MEM_MOB_SHORT_DESCR_FREE_CHAR:
	    count++;
	case MEM_MOB_SHORT_DESCR:
	    for ( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
		for ( pMobIndex = mob_index_hash[iHash];
		pMobIndex != NULL; pMobIndex = pMobIndex->next )
		    if ( pMobIndex->short_descr == pstr ) {
			if ( count == 1 )
			    return TRUE;
			else
			    count++;                      }
	    for ( ch = char_list; ch != NULL; ch = ch->next )
		    if ( ch->short_descr == pstr ) {
			if ( count == 1 )
			    return TRUE;
			else
			    count++;               }
	    break;

	case MEM_MOB_LONG_DESCR_FREE_CHAR:
	    count++;
	case MEM_MOB_LONG_DESCR:
	    for ( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
		for ( pMobIndex = mob_index_hash[iHash];
		pMobIndex != NULL; pMobIndex = pMobIndex->next )
		    if ( pMobIndex->long_descr == pstr ) {
			if ( count == 1 )
			    return TRUE;
			else
			    count++;                     }
	    for ( ch = char_list; ch != NULL; ch = ch->next )
		    if ( ch->long_descr == pstr ) {
			if ( count == 1 )
			    return TRUE;
			else
			    count++;              }
	    break;

	case MEM_MOB_DESCRIPTION_FREE_CHAR:
	    count++;
	case MEM_MOB_DESCRIPTION:
	    for ( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
		for ( pMobIndex = mob_index_hash[iHash];
		pMobIndex != NULL; pMobIndex = pMobIndex->next )
		    if ( pMobIndex->description == pstr ) {
			if ( count == 1 )
			    return TRUE;
			else
			    count++;                      }
	    for ( ch = char_list; ch != NULL; ch = ch->next )
		    if ( ch->description == pstr ) {
			if ( count == 1 )
			    return TRUE;
			else
			    count++;               }
	    break;

	case MEM_MOB_SPEC_FUN_NAME_FREE_CHAR:
	    count++;
	case MEM_MOB_SPEC_FUN_NAME:
	    for ( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
		for ( pMobIndex = mob_index_hash[iHash];
		pMobIndex != NULL; pMobIndex = pMobIndex->next )
		    if ( pMobIndex->spec_fun_name == pstr )  {
			if ( count == 1 )
			    return TRUE;
			else
			    count++;                         }
	    for ( ch = char_list; ch != NULL; ch = ch->next )
		    if ( ch->spec_fun_name == pstr ) {
			if ( count == 1 )
			    return TRUE;
			else
			    count++;                 }
	    break;

	case MEM_MOB_SHOP_MESSAGE:
	    for ( p = shop_first; p != NULL; p = p->next )
		for ( i = 0; i < SHOP_MAX_MESSAGE; i++ )
		    if ( p->shop_messages[i] == pstr ) {
			if ( count == 1 )
			    return TRUE;
			else
			    count++;                   }
	    break;

	case MEM_OBJ_NAME:
	    for ( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
		for ( pObjIndex = obj_index_hash[iHash];
		pObjIndex != NULL; pObjIndex = pObjIndex->next )
		    if ( pObjIndex->name == pstr ) {
			if ( count == 1 )
			    return TRUE;
			else
			    count++;               }
	    for ( obj = object_list; obj != NULL; obj = obj->next )
		    if ( obj->name == pstr ) {
			if ( count == 1 )
			    return TRUE;
			else
			    count++;         }
	    break;

	case MEM_OBJ_SHORT_DESCR:
	    for ( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
		for ( pObjIndex = obj_index_hash[iHash];
		pObjIndex != NULL; pObjIndex = pObjIndex->next )
		    if ( pObjIndex->short_descr == pstr ) {
			if ( count == 1 )
			    return TRUE;
			else
			    count++;                      }
	    for ( obj = object_list; obj != NULL; obj = obj->next )
		    if ( obj->short_descr == pstr ) {
			if ( count == 1 )
			    return TRUE;
			else
			    count++;                }
	    break;

	case MEM_OBJ_LOOK_DESCR:
	    for ( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
		for ( pObjIndex = obj_index_hash[iHash];
		pObjIndex != NULL; pObjIndex = pObjIndex->next )
		    if ( pObjIndex->look_descr == pstr ) {
			if ( count == 1 )
			    return TRUE;
			else
			    count++;                     }
	    for ( obj = object_list; obj != NULL; obj = obj->next )
		    if ( obj->look_descr == pstr ) {
			if ( count == 1 )
			    return TRUE;
			else
			    count++;               }
	    break;

	case MEM_OBJ_DESCRIPTION:
	    for ( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
		for ( pObjIndex = obj_index_hash[iHash];
		pObjIndex != NULL; pObjIndex = pObjIndex->next )
		    if ( pObjIndex->description == pstr ) {
			if ( count == 1 )
			    return TRUE;
			else
			    count++;                      }
	    for ( obj = object_list; obj != NULL; obj = obj->next )
		    if ( obj->description == pstr ) {
			if ( count == 1 )
			    return TRUE;
			else
			    count++;                }
	    break;

	case MEM_OBJ_SPEC_FUN_NAME:
	    for ( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
		for ( pObjIndex = obj_index_hash[iHash];
		pObjIndex != NULL; pObjIndex = pObjIndex->next )
		    if ( pObjIndex->spec_fun_name == pstr ) {
			if ( count == 1 )
			    return TRUE;
			else
			    count++;                        }
	    for ( obj = object_list; obj != NULL; obj = obj->next )
		    if ( obj->spec_fun_name == pstr ) {
			if ( count == 1 )
			    return TRUE;
			else
			    count++;                  }
	    break;

	case MEM_AREA_NAME:
	    for ( pArea = area_first; pArea != NULL; pArea = pArea->next ) {
	      if ( pArea->name == pstr ) {
	        if ( count == 1 )
		  return TRUE;
                else
		  count++;               }
	    }
	    break;

	case MEM_AREA_FILENAME:
	    for ( pArea = area_first; pArea != NULL; pArea = pArea->next ) {
	      if ( pArea->filename == pstr ) {
	        if ( count == 1 )
		  return TRUE;
                else
		  count++;               }
	    }
	    break;

	case MEM_AREA_BUILDERS:
	    for ( pArea = area_first; pArea != NULL; pArea = pArea->next ) {
	      if ( pArea->builders == pstr ) {
	        if ( count == 1 )
		  return TRUE;
                else
		  count++;               }
	    }
	    break;

	default:
	    return FALSE;
    }

    return FALSE;
}


void do_memory( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    sprintf( buf, "Affects %5d\n\r", top_affect    ); send_to_char( buf, ch );
    sprintf( buf, "Areas   %5d\n\r", top_area      ); send_to_char( buf, ch );
    sprintf( buf, "ExDes   %5d\n\r", top_ed        ); send_to_char( buf, ch );
    sprintf( buf, "Exits   %5d\n\r", top_exit      ); send_to_char( buf, ch );
    sprintf( buf, "Helps   %5d\n\r", top_help      ); send_to_char( buf, ch );
    sprintf( buf, "Mobs    %5d\n\r", top_mob_index ); send_to_char( buf, ch );
    sprintf( buf, "Objs    %5d\n\r", top_obj_index ); send_to_char( buf, ch );
    sprintf( buf, "Resets  %5d\n\r", top_reset     ); send_to_char( buf, ch );
    sprintf( buf, "Rooms   %5d\n\r", top_room      ); send_to_char( buf, ch );
    sprintf( buf, "Shops   %5d\n\r", top_shop      ); send_to_char( buf, ch );

    sprintf( buf, "Strings %5d strings of %7d bytes (max %d).\n\r",
	nAllocString, sAllocString, MAX_STRING );
    send_to_char( buf, ch );

    sprintf( buf, "Perms   %5d blocks  of %7d bytes.\n\r",
	nAllocPerm, sAllocPerm );
    send_to_char( buf, ch );

    return;
}
