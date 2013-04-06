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

extern int environment_interval;


void update_soil( ROOM_INDEX_DATA *room )
{
  int interval = environment_interval - room->water_update;

  room->water = UMAX( room->water_min,
    room->water - ( interval * room->water_degrade ) );

  room->water_update = environment_interval;

  return;
}


int get_water_value( ROOM_INDEX_DATA *room )
{
  update_flora( room );

  return room->water;
}


void soil_condition( CHAR_DATA *ch, int condition )
{
  if ( condition > 1000 )
    send_to_char( "The ground is flooded!\n\r", ch );
  else if ( condition >= 800 )
    send_to_char( "The soil is saturated with water.\n\r", ch );
  else if ( condition >= 600 )
    send_to_char( "The soil is very wet.\n\r", ch );
  else if ( condition >= 400 )
    send_to_char( "The soil is moist and damp.\n\r", ch );
  else if ( condition >= 200 )
    send_to_char( "The ground seems to be healthy.\n\r", ch );
  else if ( condition >= 100 )
    send_to_char( "The earth is rather dry.\n\r", ch );
  else
    send_to_char( "The earth is dry and caked.\n\r", ch );

  return;
}


void write_environment_area( AREA_DATA *area )
{
  ROOM_INDEX_DATA *room;
  struct flora_instance *fi;
  char filename[MAX_STRING_LENGTH];
  char buf[MAX_STRING_LENGTH];
  FILE *fp;

  sprintf( filename, "%s%s.tmp", ENVIRONMENT_DIR, area->filename );

  fclose( fpReserve );
  fp = fopen( filename, "w" );
  if ( fp == NULL )
  {
    bug( "Error opening environment save file.\n\r", 0 );
    fpReserve = fopen( NULL_FILE, "r" );
    return;
  }

  for ( room = area->room_first; room != NULL; room = room->zone_next )
  {
    fprintf( fp, "Room %d\n", room->vnum );
    fprintf( fp, "RoomWater %d\n", room->water );

    for ( fi = room->flora; fi != NULL; fi = fi->next )
      write_flora_instance( fp, fi );
  }

  fprintf( fp, "EndEnvironmentAreaFile\n" );

  fclose( fp );
  fpReserve = fopen( NULL_FILE, "r" );

  sprintf( buf, "cp %s%s.tmp %s%s.env",
    ENVIRONMENT_DIR, area->filename,
    ENVIRONMENT_DIR, area->filename );
  system( buf );

  return;
}

void read_environment_area( AREA_DATA *area )
{
  ROOM_INDEX_DATA *room = NULL;
  FILE *fp;
  char filename[MAX_STRING_LENGTH];
  char *word;
  struct flora_instance *tmp_list = NULL;
  struct flora_instance *fi = NULL;

  sprintf( filename, "%s%s.env", ENVIRONMENT_DIR, area->filename );

  fp = fopen( filename, "r" );
  if ( fp == NULL )
  {
    /* No error message.  Some zones may have no enviro data. */
    return;
  }

  while ( 1 )
  {
    word = fread_word( fp );

    if ( !str_cmp( word, "Room" ) )
    {
      /* reverse read order */
      while ( room && tmp_list )
      {
        struct flora_instance *tmp;

	tmp = tmp_list->next;
        tmp_list->next = room->flora;
	room->flora = tmp_list;
	tmp_list = tmp;
      }

      room = get_room_index( fread_number( fp ) );
      continue;
    }

    if ( !str_cmp( word, "EndEnvironmentAreaFile" ) )
    {
      if ( fi )
      {
        fi->next = tmp_list;
        tmp_list = fi;
      }

      if ( room )
      {
        while ( tmp_list != NULL )
        {
          struct flora_instance *tmp;

	  tmp = tmp_list->next;
          tmp_list->next = room->flora;
	  room->flora = tmp_list;
	  tmp_list = tmp;
        }
      }

      while ( tmp_list )
      {
        fi = tmp_list->next;
        free_flora_instance( tmp_list );
	tmp_list = fi;
      }

      break;
    }

    if ( room == NULL )
      continue;

    if ( !str_cmp( word, "RoomWater" ) )
    {
      room->water = fread_number( fp );
      continue;
    }

    if ( !str_cmp( word, "Species" ) )
    {
      if ( fi )
      {
        fi->next = tmp_list;
        tmp_list = fi;
      }

      fi = new_flora_instance( );
      fi->species = flora_species_lookup( fread_word( fp ) );
      continue;
    }

    if ( !str_cmp( word, "Quantity" ) )
    {
      if ( fi != NULL )
        fi->quantity = fread_number( fp );
      else
        fread_number( fp );

      continue;
    }

    if ( !str_cmp( word, "Health" ) )
    {
      if ( fi != NULL )
        fi->health = fread_number( fp );
      else
        fread_number( fp );

      continue;
    }

    if ( !str_cmp( word, "PlantWater" ) )
    {
      if ( fi != NULL )
        fi->water = fread_number( fp );
      else
        fread_number( fp );

      continue;
    }

    if ( !str_cmp( word, "Fruit" ) )
    {
      if ( fi != NULL )
        fi->fruit = fread_number( fp );
      else
        fread_number( fp );

      continue;
    }

    if ( !str_cmp( word, "Flower" ) )
    {
      if ( fi != NULL )
        fi->flower = fread_number( fp );
      else
        fread_number( fp );

      continue;
    }

    if ( !str_cmp( word, "Growth" ) )
    {
      if ( fi != NULL )
        fi->growth = fread_number( fp );
      else
        fread_number( fp );

      continue;
    }

    if ( !str_cmp( word, "FruitUpdate" ) )
    {
      if ( fi != NULL )
        fi->eiv_fruit = environment_interval - fread_number( fp );
      else
        fread_number( fp );

      continue;
    }

    if ( !str_cmp( word, "EndFloraInstance" ) )
    {
      if ( !fi )
        continue;
      fi->next = tmp_list;
      tmp_list = fi;
      fi = NULL;
      continue;
    }
  }

  fclose( fp );
  return;
}

void load_environment( void )
{
  AREA_DATA *area;
  extern AREA_DATA *area_first;

  for ( area = area_first; area != NULL; area = area->next )
    read_environment_area( area );

  return;
}

void write_environment( bool fAll )
{
  AREA_DATA *area;
  extern AREA_DATA *area_first;

  for ( area = area_first; area != NULL; area = area->next )
  {
    if ( fAll || --area->env_save_interval <= 0 )
    {
      write_environment_area( area );
      if ( fAll )
        area->env_save_interval = number_range( 0, 24 );
      else
        area->env_save_interval += 24;
    }
  }

  return;
}
