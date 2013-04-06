#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "flora.h"


extern int environment_interval;

struct flora_class   *flora_class_list   = NULL;
struct flora_species *flora_species_list = NULL;

struct flora_class    *flora_class_free    = NULL;
struct flora_instance *flora_instance_free = NULL;
struct flora_species  *flora_species_free  = NULL;

static char *flora_type_table [FLORA_TYPE_MAX] =
{
  "tree",
  "shrub",
  "bush",
  "stem",  /* "flowers" */
  "cactus"
};


struct flora_class *new_flora_class( void )
{
  struct flora_class *fc;

  if ( flora_class_free == NULL )
  {
    fc = alloc_perm( sizeof(*fc) );
  }
  else
  {
    fc = flora_class_free;
    flora_class_free = fc->next;
  }

  fc->name = NULL;
  fc->next = NULL;
  fc->type = FLORA_TYPE_TREE;
  fc->health_flower = 500;
  fc->health_max = 1000;
  fc->water_desired = 300;

  return fc;
}

void free_flora_class( struct flora_class *fc )
{
  if ( fc == NULL )
    return;

/*
 * This should be freed, but with short, common words like
 * oak, cactus, etc., it might be a bad idea.  The only time
 * this will leak memory is when a sector type is created
 * then freed during the same boot.  Otherwise it would be
 * in reserved string space and the free would just return.
  free_string( fc->name, MEM_GENERAL );
 */

  fc->next = flora_class_free;
  flora_class_free = fc;

  return;
}

struct flora_species *new_flora_species( void )
{
  struct flora_species *fs;

  if ( flora_species_free == NULL )
  {
    fs = alloc_perm( sizeof(*fs) );
  }
  else
  {
    fs = flora_species_free;
    flora_species_free = fs->next;
  }

  fs->name = NULL;
  fs->next = NULL;
  fs->class = NULL;
  fs->flower_effort = 6;       /* 6 hour respawn on fruit */
  fs->flower_vnum = 0;
  fs->flower_max = 1;
  fs->fruit_vnum = 0;
  fs->fruit_max = 1;
  fs->growth_seed = -168;      /* 1 week to germinate */
  fs->growth_flowering = 720;  /* 1 month to produce fruit */
  fs->growth_max = 16800;      /* 2 years default lifespan */

  return fs;
}

void free_flora_species( struct flora_species *fs )
{
  if ( fs == NULL )
    return;

/*
 * This should be freed, but with short, common words like
 * oak, cactus, etc., it might be a bad idea.  The only time
 * this will leak memory is when a sector type is created
 * then freed during the same boot.  Otherwise it would be
 * in reserved string space and the free would just return.
  free_string( fs->name, MEM_GENERAL );
 */

  fs->next = flora_species_free;
  flora_species_free = fs;

  return;
}

struct flora_instance *new_flora_instance( void )
{
  struct flora_instance *fi;

  if ( flora_instance_free == NULL )
  {
    fi = alloc_perm( sizeof(*fi) );
  }
  else
  {
    fi = flora_instance_free;
    flora_instance_free = fi->next;
  }

  fi->next = NULL;
  fi->species = NULL;
  fi->health = 300;
  fi->water = 300;
  fi->fruit = 0;
  fi->flower = 0;
  fi->growth = 720;
  fi->eiv_fruit = environment_interval;

  return fi;
}

void free_flora_instance( struct flora_instance *fi )
{
  if ( fi == NULL )
    return;

  fi->next = flora_instance_free;
  flora_instance_free = fi;

  return;
}


void destroy_flora( struct flora_instance *fi, ROOM_INDEX_DATA *room )
{
  struct flora_instance *f;

  if ( fi == NULL || room == NULL )
    return;

  if ( room->flora == fi )
  {
    room->flora = fi->next;
    free_flora_instance( fi );
    return;
  }

  for ( f = room->flora; f != NULL; f = f->next )
  {
    if ( f->next == fi )
    {
      f->next = fi->next;
      free_flora_instance( fi );
      return;
    }
  }

  /* flora instance not found if control reaches here */

  return;
}


struct flora_class *flora_class_lookup( char *argument )
{
  struct flora_class *fc;

  for ( fc = flora_class_list; fc != NULL; fc = fc->next )
    if ( !str_cmp( argument, fc->name ) )
      return fc;

  return NULL;
}


struct flora_species *flora_species_lookup( char *argument )
{
  struct flora_species *fs;

  for ( fs = flora_species_list; fs != NULL; fs = fs->next )
    if ( !str_cmp( argument, fs->name ) )
      return fs;

  return NULL;
}


int flora_type_lookup( char *argument )
{
  int type;

  for ( type = 0; type < FLORA_TYPE_MAX; type++ )
    if ( !str_cmp( argument, flora_type_table[type] ) )
      return type;

  return FLORA_TYPE_TREE;
}


void write_flora_class( FILE *fp, struct flora_class *fc )
{
  fprintf( fp, "Name '%s'\n", fc->name );
  fprintf( fp, "Type '%s'\n", flora_type_table[fc->type] );
  fprintf( fp, "HealthFlower %d\n", fc->health_flower );
  fprintf( fp, "HealthMax %d\n", fc->health_max );
  fprintf( fp, "WaterDesired %d\n", fc->water_desired );
  fprintf( fp, "EndFloraClass\n" );

  return;
}

void write_flora_species( FILE *fp, struct flora_species *fs )
{
  fprintf( fp, "Name '%s'\n", fs->name );
  fprintf( fp, "Class '%s'\n", fs->class->name );
  fprintf( fp, "FlowerEffort %d\n", fs->flower_effort );
  fprintf( fp, "FlowerVnum %d\n", fs->flower_vnum );
  fprintf( fp, "FlowerMax %d\n", fs->flower_max );
  fprintf( fp, "FruitVnum %d\n", fs->fruit_vnum );
  fprintf( fp, "FruitMax %d\n", fs->fruit_max );
  fprintf( fp, "GrowthSeed %d\n", fs->growth_seed );
  fprintf( fp, "GrowthFlowering %d\n", fs->growth_flowering );
  fprintf( fp, "GrowthMax %d\n", fs->growth_max );
  fprintf( fp, "EndFloraSpecies\n" );

  return;
}

void write_flora_instance( FILE *fp, struct flora_instance *fi )
{
  fprintf( fp, "Species '%s'\n", fi->species->name );
  fprintf( fp, "Quantity %d\n", fi->quantity );
  fprintf( fp, "Health %d\n", fi->health );
  fprintf( fp, "PlantWater %d\n", fi->water );
  fprintf( fp, "Fruit %d\n", fi->fruit );
  fprintf( fp, "Flower %d\n", fi->flower );
  fprintf( fp, "Growth %d\n", fi->growth );
  fprintf( fp, "FruitUpdate %d\n", environment_interval - fi->eiv_fruit );
  fprintf( fp, "EndFloraInstance\n" );

  return;
}

int write_flora_class_file( void )
{
  FILE *fp;
  struct flora_class *f;
  char buf[MAX_STRING_LENGTH];

  if ( ( fp = fopen( "flora_class.tmp", "w" ) ) == NULL )
  {
    bug("Error opening flora class file.\n\r", 0 );
    return 1;
  }

  for ( f = flora_class_list; f != NULL; f = f->next )
  {
    write_flora_class( fp, f );
  }

  fprintf( fp, "Endsection\n" );

  fclose( fp );
  sprintf( buf, "cp flora_class.tmp %s", FLORA_CLASS_FILE );
  system( buf );
  system( "rm flora_class.tmp" );

  return 0;
}


int write_flora_species_file( void )
{
  FILE *fp;
  struct flora_species *fs;
  char buf[MAX_STRING_LENGTH];

  if ( ( fp = fopen( "flora_species.tmp", "w" ) ) == NULL )
  {
    bug("Error opening flora species file.\n\r", 0 );
    return 1;
  }

  for ( fs = flora_species_list; fs != NULL; fs = fs->next )
  {
    write_flora_species( fp, fs );
  }

  fprintf( fp, "Endsection\n" );

  fclose( fp );
  sprintf( buf, "cp flora_species.tmp %s", FLORA_SPECIES_FILE );
  system( buf );
  system( "rm flora_species.tmp" );

  return 0;
}


void read_flora_classes( void )
{
  char *word;
  struct flora_class *fc = NULL;
  FILE *fp;

  if ( ( fp = fopen( FLORA_CLASS_FILE, "r" ) ) == NULL )
  {
    bug("Error opening flora class file.\n\r", 0 );
    exit( 1 );
  }

  while ( 1 )
  {
    word = fread_word( fp );

    if ( !str_cmp( word, "Name" ) )
    {
      if ( fc != NULL )
      {
        fc->next = flora_class_list;
	flora_class_list = fc;
      }
      fc = new_flora_class( );
      fc->name = str_dup( fread_word( fp ) );
      continue;
    }

    if ( !str_cmp( word, "Type" ) )
    {
      if ( fc != NULL )
        fc->type = flora_type_lookup( fread_word( fp ) );
      else
        fread_number( fp );

      continue;
    }

    if ( !str_cmp( word, "HealthFlower" ) )
    {
      if ( fc != NULL )
        fc->health_flower = fread_number( fp );
      else
        fread_number( fp );

      continue;
    }

    if ( !str_cmp( word, "HealthMax" ) )
    {
      if ( fc != NULL )
        fc->health_max = fread_number( fp );
      else
        fread_number( fp );

      continue;
    }

    if ( !str_cmp( word, "WaterDesired" ) )
    {
      if ( fc != NULL )
        fc->water_desired = fread_number( fp );
      else
        fread_number( fp );

      continue;
    }

    if ( !str_cmp( word, "EndFloraClass" ) )
    {
      fc->next = flora_class_list;
      flora_class_list = fc;
      fc = NULL;
      continue;
    }

    if ( !str_cmp( word, "Endsection" ) )
      break;
  }

  fclose( fp );

  return;
}


void read_flora_species( void )
{
  char *word;
  struct flora_species *fs = NULL;
  FILE *fp;

  if ( ( fp = fopen( FLORA_SPECIES_FILE, "r" ) ) == NULL )
  {
    bug("Error opening flora species file.\n\r", 0 );
    exit( 1 );
  }

  while ( 1 )
  {
    word = fread_word( fp );

    if ( !str_cmp( word, "Name" ) )
    {
      if ( fs != NULL )
      {
        fs->next = flora_species_list;
	flora_species_list = fs;
      }
      fs = new_flora_species( );
      fs->name = str_dup( fread_word( fp ) );
      continue;
    }

    if ( !str_cmp( word, "Class" ) )
    {
      if ( fs != NULL )
        fs->class = flora_class_lookup( fread_word( fp ) );
      else
        fread_number( fp );

      continue;
    }

    if ( !str_cmp( word, "FlowerEffort" ) )
    {
      if ( fs != NULL )
        fs->flower_effort = fread_number( fp );
      else
        fread_number( fp );

      continue;
    }

    if ( !str_cmp( word, "FlowerVnum" ) )
    {
      if ( fs != NULL )
        fs->flower_vnum = fread_number( fp );
      else
        fread_number( fp );

      continue;
    }

    if ( !str_cmp( word, "FlowerMax" ) )
    {
      if ( fs != NULL )
        fs->flower_max = fread_number( fp );
      else
        fread_number( fp );

      continue;
    }

    if ( !str_cmp( word, "FruitVnum" ) )
    {
      if ( fs != NULL )
        fs->fruit_vnum = fread_number( fp );
      else
        fread_number( fp );

      continue;
    }

    if ( !str_cmp( word, "FruitMax" ) )
    {
      if ( fs != NULL )
        fs->fruit_max = fread_number( fp );
      else
        fread_number( fp );

      continue;
    }

    if ( !str_cmp( word, "GrowthSeed" ) )
    {
      if ( fs != NULL )
        fs->growth_seed = fread_number( fp );
      else
        fread_number( fp );

      continue;
    }

    if ( !str_cmp( word, "GrowthFlowering" ) )
    {
      if ( fs != NULL )
        fs->growth_flowering = fread_number( fp );
      else
        fread_number( fp );

      continue;
    }

    if ( !str_cmp( word, "GrowthMax" ) )
    {
      if ( fs != NULL )
        fs->growth_max = fread_number( fp );
      else
        fread_number( fp );

      continue;
    }

    if ( !str_cmp( word, "EndFloraSpecies" ) )
    {
      fs->next = flora_species_list;
      flora_species_list = fs;
      fs = NULL;
      continue;
    }

    if ( !str_cmp( word, "Endsection" ) )
      break;
  }

  fclose( fp );

  return;
}

void load_flora( void )
{
  read_flora_classes( );
  read_flora_species( );

  return;
}

char *get_plant_sdesc( struct flora_instance *fi )
{
  static char sdesc[MAX_STRING_LENGTH];
  char buf[MAX_STRING_LENGTH];

  sprintf( buf, "%s %s", fi->species->name,
    ( fi->quantity > 1 )
    ? plural( flora_type_table[fi->species->class->type] )
    : flora_type_table[fi->species->class->type] );

  if ( fi->health < 0 )
    sprintf( sdesc, "rotting %s", buf );
  else if ( fi->health < ( fi->species->class->health_flower / 8 ) )
    sprintf( sdesc, "dying %s", buf );
  else if ( fi->health < ( fi->species->class->health_flower / 2 ) )
    sprintf( sdesc, "unhealthy %s", buf );
  else if ( fi->health  <
   ( ( fi->species->class->health_max
     - fi->species->class->health_flower ) / 2 )
   + fi->species->class->health_flower )
    sprintf( sdesc, "%s", buf );
  else
    sprintf( sdesc, "flourishing %s", buf );

  return sdesc;
}


void show_plant( CHAR_DATA *ch, struct flora_instance *fi )
{
  char buf[MAX_STRING_LENGTH];
  char *sdesc;

  if ( fi->species == NULL )
    return;

  sdesc = get_plant_sdesc( fi );

  if ( fi->quantity <= 0 )
    sprintf( buf, "BUG: %d %s seem to be here...\n\r", fi->quantity,
      sdesc );
  else if ( fi->quantity == 1 )
    sprintf( buf, "%s %s grows here.\n\r",
      is_vowel(sdesc[0]) ? "an" : "a",
      sdesc );
  else if ( fi->quantity < 4 )
    sprintf( buf, "A few %s grow here.\n\r", sdesc );
  else if ( fi->quantity < 10 )
    sprintf( buf, "%s dot the terrain.\n\r", sdesc );
  else
    sprintf( buf, "A forest of %s covers the land.\n\r", sdesc );

  send_to_char( buf, ch );

  return;
}


bool display_flora( CHAR_DATA *ch, FLORA_DATA *flora, int idist )
{
  struct flora_instance *fi;
  bool found = FALSE;

  for ( fi = flora; fi != NULL; fi = fi->next )
  {
    found = TRUE;
    show_plant( ch, fi );
  }

  return found;
}


void view_flora_class( CHAR_DATA *ch, struct flora_class *fc )
{
  char buf[MAX_STRING_LENGTH];

  sprintf( buf, "Flora Class: %s.\n", fc->name );
  send_to_char( buf, ch );

  sprintf( buf, "Type: %s (%d).\n\r", flora_type_table[fc->type], fc->type );
  send_to_char( buf, ch );

  sprintf( buf, "Health:  Flowering: %d, Max: %d.\n\r",
    fc->health_flower, fc->health_max );
  send_to_char( buf, ch );

  sprintf( buf, "Water Desired: %d.\n\r",
    fc->water_desired );
  send_to_char( buf, ch );

  return;
}


void view_flora_species( CHAR_DATA *ch, struct flora_species *fs )
{
  char buf[MAX_STRING_LENGTH];
  OBJ_INDEX_DATA *pObjIndex;

  sprintf( buf, "Flora Species: %s.  Class: %s.\n",
    fs->name, fs->class->name );
  send_to_char( buf, ch );

  sprintf( buf, "Flower Effort: %d.\n\r", fs->flower_effort );
  send_to_char( buf, ch );

  pObjIndex = get_obj_index( fs->flower_vnum );
  sprintf( buf, "Flower: %s (%d), Max: %d.\n\r",
    pObjIndex ? pObjIndex->short_descr : "(none)",
    fs->flower_vnum, fs->flower_max );
  send_to_char( buf, ch );

  pObjIndex = get_obj_index( fs->fruit_vnum );
  sprintf( buf, "Fruit: %s (%d), Max: %d.\n\r",
    pObjIndex ? pObjIndex->short_descr : "(none)",
    fs->fruit_vnum, fs->fruit_max );
  send_to_char( buf, ch );

  sprintf( buf, "Growth: Seed: %d, Flowering: %d, Max: %d.\n\r",
    fs->growth_seed, fs->growth_flowering, fs->growth_max );
  send_to_char( buf, ch );

  return;
}


void view_flora_instance( CHAR_DATA *ch, struct flora_instance *fi )
{
  char buf[MAX_STRING_LENGTH];

  sprintf( buf, "Flora Species: %s.\n",
    fi->species->name );
  send_to_char( buf, ch );

  sprintf( buf, "Health: %d.  Water: %d.  Growth: %d.  Quantity: %d.\n\r",
    fi->health, fi->water, fi->growth, fi->quantity );
  send_to_char( buf, ch );

  sprintf( buf, "Flowers: %d.  Fruit: %d.\n\r",
    fi->flower, fi->fruit );
  send_to_char( buf, ch );

  return;
}

struct flora_instance *get_flora_here( CHAR_DATA *ch, char *arg )
{
  struct flora_instance *fi;

  for ( fi = ch->in_room->flora; fi != NULL; fi = fi->next )
  {
    if ( is_name( arg, fi->species->name ) )
      return fi;

    if ( is_name( arg, fi->species->class->name ) )
      return fi;

    if ( !str_cmp( arg, flora_type_table[fi->species->class->type] ) )
      return fi;
  }

  return NULL;
}

void update_fruit( struct flora_instance *fi )
{
  if ( fi->fruit == fi->species->fruit_max )
    fi->eiv_fruit = environment_interval;

  fi->fruit--;

  return;
}

void update_flower( struct flora_instance *fi )
{
  if ( fi->flower == fi->species->flower_max )
    fi->eiv_fruit = environment_interval;

  fi->flower--;

  return;
}


void update_flora( ROOM_INDEX_DATA *room )
{
  struct flora_instance *fi;
  int interval;

  if ( room->water_update >= environment_interval )
    return;

  interval = environment_interval - room->water_update;

  update_soil( room );

  for ( fi = room->flora; fi != NULL; fi = fi->next )
  {
    if ( fi->water < room->water )
      fi->water = room->water;
    else if ( fi->water > room->water )
      fi->water = UMAX( fi->water - interval, room->water );

    if ( fi->water >= fi->species->class->water_desired )
      fi->health += interval;
    else
      fi->health -= interval;

    if ( fi->health >= fi->species->class->health_flower / 2 )
      fi->growth += interval;

    if ( fi->health < fi->species->class->health_flower )
      fi->eiv_fruit = environment_interval;
    else
    {
      if ( ( fi->species->fruit_max != 0 || fi->species->flower_max != 0 )
      && fi->health >= fi->species->class->health_flower )
      {
        while ( ( environment_interval - fi->eiv_fruit )
              > fi->species->flower_effort )
        {
          if ( fi->fruit >= fi->species->fruit_max
	  && fi->species->fruit_max != -1 )
	    break;

          fi->fruit++;
          fi->eiv_fruit += fi->species->flower_effort;
        }

        while ( ( environment_interval - fi->eiv_fruit )
              > fi->species->flower_effort )
        {
          if ( fi->flower >= fi->species->flower_max
	  && fi->species->flower_max != -1 )
            break;

          fi->flower++;
          fi->eiv_fruit += fi->species->flower_effort;
        }
      }
    }
  }

  return;
}

void do_flora( CHAR_DATA *ch, char *argument )
{
  struct flora_class *fc = NULL;
  struct flora_species *fs = NULL;
  struct flora_instance *fi = NULL;
  char arg2[MAX_INPUT_LENGTH];
  char arg3[MAX_INPUT_LENGTH];
  char arg4[MAX_INPUT_LENGTH];
  char arg5[MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg2 );

  if ( arg2[0] == '\0' )
  {
    send_to_char( "Usage: flora <command> [args]\n\r", ch );
    send_to_char( "Commands:\n\r", ch );
    send_to_char( "  cedit <class> [..]  (edits flora class info)\n\r", ch );
    send_to_char( "  destroy <flora>     (destroys flora instance)\n\r", ch );
    send_to_char( "  new   <species>     (creates new tree in room)\n\r", ch );
    send_to_char( "  save                (save current zone state)\n\r", ch );
    send_to_char( "  sedit <species> [..]  (edits flora species info)\n\r",
      ch );
    send_to_char( "  set   <flora> [..]  (edits flora instance info)\n\r", ch );
    send_to_char( "  stat                (reviews flora in room)\n\r", ch );
    send_to_char( "  view  <flora>   (views specified flora info)\n\r", ch );
    send_to_char( "  write <class/species>  (writes global flora table)\n\r",
      ch );
    return;
  }

  if ( !str_cmp( arg2, "save" ) )
  {
    write_environment_area( ch->in_room->area );
    send_to_char( "Flora state saved.\n\r", ch );
    return;
  }

  if ( !str_cmp( arg2, "stat" ) )
  {
    if ( ch->in_room->flora == NULL )
    {
      send_to_char( "No flora in this room.\n\r", ch );
      return;
    }

    update_flora( ch->in_room );

    for ( fi = ch->in_room->flora; fi != NULL; fi = fi->next )
      view_flora_instance( ch, fi );

    return;
  }

  argument = one_argument( argument, arg3 );

  if ( arg3[0] == '\0' )
  {
    do_flora( ch, "" );
    return;
  }

  if ( !str_cmp( arg2, "new" ) )
  {
    fs = flora_species_lookup( arg3 );
    if ( !fs )
    {
      send_to_char( "Species of plant does not exist.\n\r", ch );
      return;
    }

    fi = new_flora_instance( );
    fi->species = fs;
    fi->quantity = 1;

    fi->next = ch->in_room->flora;
    ch->in_room->flora = fi;

    send_to_char( "Plant created.\n\r", ch );
    return;
  }

  if ( !str_cmp( arg2, "write" ) )
  {
    if ( get_trust( ch ) < MAX_LEVEL )
      return;

    if ( !str_cmp( arg3, "class" ) )
    {
      write_flora_class_file( );
      send_to_char( "Flora class file written.\n\r", ch );
    }
    else if ( !str_cmp( arg3, "species" ) )
    {
      write_flora_species_file( );
      send_to_char( "Flora species file written.\n\r", ch );
    }
    else
      send_to_char( "You must specify class or species.\n\r", ch );

    return;
  }

  if ( !str_cmp( arg2, "view" ) )
  {
    fc = flora_class_lookup( arg3 );
    if ( fc == NULL )
      send_to_char( "No flora class by that name.\n\r", ch );
    else
      view_flora_class( ch, fc );

    fs = flora_species_lookup( arg3 );
    if ( fs == NULL )
      send_to_char( "No flora species by that name.\n\r", ch );
    else
      view_flora_species( ch, fs );

    if ( fs && fs->class != fc )
    {
      send_to_char( "Displaying associated flora class.\n\r", ch );
      view_flora_class( ch, fs->class );
    }

    return;
  }

  if ( !str_cmp( arg2, "destroy" ) )
  {
    fi = get_flora_here( ch, arg3 );

    if ( fi == NULL )
    {
      send_to_char( "Flora instance not found.\n\r", ch );
      return;
    }

    destroy_flora( fi, ch->in_room );

    send_to_char( "Flora destroyed.\n\r", ch );
    return;
  }

  argument = one_argument( argument, arg4 );
  argument = one_argument( argument, arg5 );

  if ( arg4[0] == '\0' || arg5[0] == '\0' )
  {
    do_flora( ch, "" );
    return;
  }

  if ( !str_cmp( arg2, "set" ) )
  {
    int n = atoi( arg5 );

    fi = get_flora_here( ch, arg3 );

    if ( fi == NULL )
    {
      send_to_char( "Flora instance not found.\n\r", ch );
      return;
    }

    if ( !str_prefix( arg4, "flower" ) )
      fi->flower = n;
    else if ( !str_prefix( arg4, "fruit" ) )
      fi->fruit = n;
    else if ( !str_prefix( arg4, "growth" ) )
      fi->growth = n;
    else if ( !str_prefix( arg4, "health" ) )
      fi->health = n;
    else if ( !str_prefix( arg4, "quantity" ) )
      fi->quantity = n;
    else if ( !str_prefix( arg4, "water" ) )
      fi->water = n;
    else
      send_to_char( "Syntax: flora set <plant> <attribute> <number>\n\r"
     "  Attribute is:  flower, fruit, growth, health, quantity, or water.\n\r",
      ch );

    return;
  }

  do_flora( ch, "" );
  return;
}

