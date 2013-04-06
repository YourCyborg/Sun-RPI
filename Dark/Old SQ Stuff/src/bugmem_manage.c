/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        
*
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   
*
 *                                                                         
*
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          
*
 *  Chastain, Michael Quan, and Mitchell Tse.                              
*
 *                                                                         
*
 *  In order to use any part of this Merc Diku Mud, you must comply with   
*
 *  both the original Diku license in 'license.doc' as well the Merc       
*
 *  license in 'license.txt'.  In particular, you may not remove either of 
*
 *  these copyright notices.                                               
*
 *                                                                         
*
 *  Much time and thought has gone into this software and you are          
*
 *  benefitting.  We hope that you share your changes too.  What goes      
*
 *  around, comes around.                                                  
*
 
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

#define MEM_MAGIC  (0xDCBA1234)

struct mem_hdr
{
  unsigned int magic;
  int references;
};

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

  int size_mem = sMem + sizeof( struct mem_hdr );
  struct mem_hdr *sm_ptr;

  for ( iList = 0; iList < MAX_MEM_LIST; iList++ )
  {
    if ( size_mem <= rgSizeList[iList] )
      break;
  }

  if ( iList == MAX_MEM_LIST )
  {
    bug( "Alloc_mem: size %d too large.", sMem );
    exit( 1 );
  }

  if ( rgFreeList[iList] == NULL )
  {
    pMem                = alloc_perm( rgSizeList[iList] );
  }
  else
  {
    pMem                = rgFreeList[iList];
    rgFreeList[iList]   = * ((void **) rgFreeList[iList]);
  }

  sm_ptr = (struct mem_hdr *) pMem;
  pMem += sizeof( struct mem_hdr );

  sm_ptr->magic = MEM_MAGIC;
  sm_ptr->references = 1;

  return pMem;
}



/*
 * Free some memory.
 * Recycle it back onto the free list for blocks of that size.
 */
void free_mem( void *pMem, int sMem )
{
  int iList;
  int size_mem = sMem + sizeof( struct mem_hdr );
  struct mem_hdr *sm_ptr;

  sm_ptr = pMem - sizeof( struct mem_hdr );

  /* Log error and continue.  Do not free memory. */
  if ( sm_ptr->magic != MEM_MAGIC )
  {
    bug( "Free_mem: Attempting to free memory without MEM_MAGIC set.", 0 );
    bug( pMem, 0 );
    return;
  }

  /* Decrement number of references to this memory. */
  sm_ptr->references--;

  /* If one or more references remain, do not free this memory */
  if ( sm_ptr->references != 0 )
  {
    return;
  }

  for ( iList = 0; iList < MAX_MEM_LIST; iList++ )
  {
    if ( size_mem <= rgSizeList[iList] )
      break;
  }

  if ( iList == MAX_MEM_LIST )
  {
    bug( "Free_mem: size %d too large.", size_mem );
    exit( 1 );
  }

  * ((void **) sm_ptr) = rgFreeList[iList];
  rgFreeList[iList]  = sm_ptr;

  return;
}



/*
 * Allocate some permanent memory.
 * Permanent memory is never freed,
 *   pointers into it may be copied safely.
 */
void *alloc_perm( int sMem )
{
  static char *pMemPerm = NULL;
  static int iMemPerm = 0;
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
  struct mem_hdr *sm_ptr;

  if ( str[0] == '\0' )
    return &str_empty[0];

  if ( str >= string_space && str < top_string )
    return (char *) str;

  sm_ptr = (struct mem_hdr *) ( str - sizeof( struct mem_hdr ) );

  /* Increment references to dynamic memory. */
  if ( sm_ptr->magic == MEM_MAGIC )
  {
    sm_ptr->references++;
    return (char *) str;
  }

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

  /* Free mem now handles reference counts */
  free_mem( pstr, strlen(pstr) + 1 );
  return;
}


void do_memory( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    sprintf( buf, "Affects %5d\n\r", top_affect    ); send_to_char( buf, 
ch );
    sprintf( buf, "Areas   %5d\n\r", top_area      ); send_to_char( buf, 
ch );
    sprintf( buf, "ExDes   %5d\n\r", top_ed        ); send_to_char( buf, 
ch );
    sprintf( buf, "Exits   %5d\n\r", top_exit      ); send_to_char( buf, 
ch );
    sprintf( buf, "Helps   %5d\n\r", top_help      ); send_to_char( buf, 
ch );
    sprintf( buf, "Mobs    %5d\n\r", top_mob_index ); send_to_char( buf, 
ch );
    sprintf( buf, "Objs    %5d\n\r", top_obj_index ); send_to_char( buf, 
ch );
    sprintf( buf, "Resets  %5d\n\r", top_reset     ); send_to_char( buf, 
ch );
    sprintf( buf, "Rooms   %5d\n\r", top_room      ); send_to_char( buf, 
ch );
    sprintf( buf, "Shops   %5d\n\r", top_shop      ); send_to_char( buf, 
ch );

    sprintf( buf, "Strings %5d strings of %7d bytes (max %d).\n\r",
	nAllocString, sAllocString, MAX_STRING );
    send_to_char( buf, ch );

    sprintf( buf, "Perms   %5d blocks  of %7d bytes.\n\r",
	nAllocPerm, sAllocPerm );
    send_to_char( buf, ch );

    return;
}
