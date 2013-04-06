#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"


extern struct sector_type *sector_type_list;


int flag_lookup( char *arg, const struct flag_info *f_list )
{
  int i;

  for ( i = 0; f_list[i].flag != 0; i++ )
    if ( !str_cmp( arg, f_list[i].name ) )
      return f_list[i].flag;

  return 0;
}

int fread_flags( FILE *fp, const struct flag_info *f_list )
{
  char *word;
  int flagval = 0;

  word = fread_word( fp );

  while ( word != NULL && str_cmp( word, "Endflags" ) )
  {
    flagval += flag_lookup( word, f_list );
    word = fread_word( fp );
  }

  return flagval;
}

struct sector_type *sector_lookup( char *argument, AREA_DATA *pArea )
{
  struct sector_type *s;

  /*
   * search sectors owned by the area first.
   */
  if ( pArea )
    for ( s = pArea->sector_list; s != NULL; s = s->next )
      if ( !str_cmp( argument, s->name ) )
        return s;

  for ( s = sector_type_list; s != NULL; s = s->next )
    if ( !str_cmp( argument, s->name ) )
      return s;

  return NULL;
}


const struct flag_info sector_flag_table[] =
{
  { "outdoors", SECTOR_OUTDOORS },
  { "water", SECTOR_WATER },
  { "noswim", SECTOR_NOSWIM },
  { "underground", SECTOR_UNDERGROUND },
  { "building", SECTOR_BUILDING },
  { "air", SECTOR_AIR },
  { "window", SECTOR_WINDOW },
  { "last", 0 }
};



void output_text_flags( FILE *fp, CHAR_DATA *ch, int flags,
  const struct flag_info *f_table )
{
  char buf[MAX_STRING_LENGTH] = "/0";
  struct flag_info *f;
  bool fStart = FALSE;

  for ( f = (struct flag_info *) f_table; f->flag != 0; f++ )
  {
    if ( IS_SET( flags, f->flag ) )
    {
      if ( !fStart )
      {
        fStart = TRUE;
	sprintf( buf, "%s", f->name );
      }
      else
      {
        strcat( buf, " " );
	strcat( buf, f->name );
      }
    }
  }

  if ( fp )
    fprintf( fp, buf );
  else if ( ch )
    send_to_char( buf, ch );

  return;
}


void write_sector( FILE *fp, struct sector_type *s )
{
  fprintf( fp, "Name '%s'\n", s->name );
  fprintf( fp, "Flags " );
  output_text_flags( fp, NULL, s->flags, sector_flag_table );
  fprintf( fp, " Endflags\nDifficulty %d\n", s->difficulty );
  fprintf( fp, "Water %d %d\n",
    s->water_min, s->water_degrade );
  fprintf( fp, "Endsector\n" );

  return;
}

int write_sector_file( void )
{
  FILE *fp;
  struct sector_type *s;
  char buf[MAX_STRING_LENGTH];

  if ( ( fp = fopen( "sectors.tmp", "w" ) ) == NULL )
  {
    bug("Error opening sector file.\n\r", 0 );
    return 1;
  }

  for ( s = sector_type_list; s != NULL; s = s->next )
  {
    write_sector( fp, s );
  }

  fprintf( fp, "Endsection\n" );

  fclose( fp );
  sprintf( buf, "cp sectors.tmp %s", SECTOR_FILE );
  system( buf );
  system( "rm sectors.tmp" );

  return 0;
}


struct sector_type *read_sectors( FILE *fp )
{
  char *word;
  struct sector_type *slist = NULL;
  struct sector_type *s = NULL;

  while ( 1 )
  {
    word = fread_word( fp );

    if ( !str_cmp( word, "Name" ) )
    {
      if ( s != NULL )
      {
        s->next = slist;
	slist = s;
      }
      s = new_sector_type( );
      s->name = str_dup( fread_word( fp ) );
      continue;
    }

    if ( !str_cmp( word, "Flags" ) )
    {
      if ( s != NULL )
        s->flags = fread_flags( fp, sector_flag_table );
      else
        fread_number( fp );
      continue;
    }

    if ( !str_cmp( word, "Difficulty" ) )
    {
      if ( s != NULL )
        s->difficulty = fread_number( fp );
      else
        fread_number( fp );
      continue;
    }

    if ( !str_cmp( word, "Water" ) )
    {
      if ( s != NULL )
      {
        s->water_min = fread_number( fp );
        s->water_degrade = fread_number( fp );
      }
      else
      {
        fread_number( fp );
        fread_number( fp );
      }
      continue;
    }

    if ( !str_cmp( word, "Endsector" ) )
    {
      s->next = slist;
      slist = s;
      s = NULL;
      continue;
    }

    if ( !str_cmp( word, "Endsection" ) )
      break;
  }

  fclose( fp );

  return slist;
}


void load_main_sectors( void )
{
  FILE *fp;

  if ( ( fp = fopen( SECTOR_FILE, "r" ) ) == NULL )
  {
    bug("Error opening sector file.\n\r", 0 );
    exit( 1 );
  }

  sector_type_list = read_sectors( fp );

  return;
}


void view_sector( CHAR_DATA *ch, struct sector_type *s )
{
  char buf[MAX_STRING_LENGTH];

  sprintf( buf, "Sector: %s\n", s->name );
  send_to_char( buf, ch );

  send_to_char( "Flags: ", ch );
  output_text_flags( NULL, ch, s->flags, sector_flag_table );

  sprintf( buf, "\n\rDifficulty: %d\n\r", s->difficulty );
  send_to_char( buf, ch );

  sprintf( buf, "Water:  Min: %d, Degrade: %d\n\r",
    s->water_min, s->water_degrade );
  send_to_char( buf, ch );
}


void do_sector( CHAR_DATA *ch, char *argument )
{
  struct sector_type *s = NULL;
  char arg2[MAX_INPUT_LENGTH];
  char arg3[MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg2 );
  argument = one_argument( argument, arg3 );

  if ( arg2[0] == '\0' )
  {
    send_to_char( "Usage: sector <command> [args]\n\r", ch );
    send_to_char( "Commands:\n\r", ch );
    send_to_char( "  view <sector>  (views specified sector info)\n\r", ch );
    send_to_char( "  write          (writes global sector table)\n\r", ch );
    return;
  }

  if ( !str_cmp( arg2, "write" ) )
  {
    if ( get_trust( ch ) < MAX_LEVEL )
      return;

    write_sector_file( );

    send_to_char( "Sector file written.\n\r", ch );
    return;
  }

  if ( !str_cmp( arg2, "view" ) )
  {
    s = sector_lookup( arg3, ch->in_room->area );
    if ( s == NULL )
      send_to_char( "Could not find that sector to view.\n\r", ch );
    else
      view_sector( ch, s );
    return;
  }

  do_sector( ch, "" );
  return;
}


void set_sector( ROOM_INDEX_DATA *room, struct sector_type *s )
{
  room->sector = s;
  room->water_min = s->water_min;
  room->water_degrade = s->water_degrade;

  return;
}
