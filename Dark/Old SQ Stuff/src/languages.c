/*
 * For reference.
 * English Letter frequency:  (per 1000 letters)
 * A 81.51  B 14.40  C 27.58  D 37.88  E 131.05  F 29.24  G 19.94  H 52.59 
 * I 63.45  J 1.32   K 4.20   L 33.89  M 25.36   N 70.98  O 79.95  P 19.82 
 * Q 1.21   R 68.32  S 61.01  T 104.68 U 24.59   V 9.19   W 15.39  X 1.66 
 * Y 19.82  Z .77 
 *
 * German Letter frequency: (per 1000 letters)
 * A 65.06  B 25.66  C 28.37  D 54.14  E 166.93  F 20.44  G 36.47  H 40.64 
 * I 78.12  J 1.91   K 18.79  L 28.25  M 30.05   N 99.05  O 22.85  P 9.44 
 * Q .55    R 65.39  S 67.65  T 67.42  U 37.03   V 10.69  W 13.96  X .22 
 * Y .32    Z 10.02 
*/

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "merc.h"
#include "languages.h"

/*  "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890" */

struct language_info language_table[MAX_LANGUAGE] =
{
    /* english frequencies */
  { &gsn_andean,
    "aaaaaaabcccddddeeeeeeeeeeeefffgghhhiiiiiiijkllllmmmnnnnnnnooooooopppqrrrrrrrssssssttttttttuuuvwwxyyz" },
    /* more vowely than english, less hard letters */
  { &gsn_luderwen,
    "aaaaaaaaaabbccdddeeeeeeeeeeeeeeeeeefffgghhiiiiiiiiiiijjklllllmmmnnnnnoooppqrrrrssssttttuuuvwwxyyyyyz" },
    /* roughchoppy more or less has a german profile, with some tweaks */
  { &gsn_vedican,
    "aaaaaabbbcccccdddddeeeeeeeeeeffggggghhhhhiiiijkkkklllmmmnnnnnooppqqrrrrrrsssssssttttttttuuuvwxxyzzzz" },
    /* lots of hard letters, few vowels, few soft consonants */
  { &gsn_jikkean,
    "aaabcccccccddeeeeeefggghhhhhhhiiiiiijjkkkkkkkkkklllmmnoopppqqqqrssstttttttttttuuuvwwxxxxxxxxyyzzzzzz" },
  { &gsn_tongues,
    "aaaaaaabcccddddeeeeeeeeeeeefffgghhhiiiiiiijkllllmmmnnnnnnnooooooopppqrrrrrrrssssssttttttttuuuvwwxyyz" },
  { &gsn_elekun,
    "abcccccccccccddggghhhhhhhiiikkkkkkkkkkkkkklllooqqqqsssssssssssttttttttttttttttuuuvwwxxxxxxxxyyzzzzzz" },
  
};


char *scramble_language( int skill, int language, char *argument )
{
  static char buf[MAX_STRING_LENGTH];
  int collect = 0;
  int size;
  char *p;
  int i;

  size = strlen( argument );

  collect = (int) argument[0];
  for ( p = argument; p[0] != '\0'; p++ )
    collect += (int) p[0];

  lcg_next( collect, 100 );

  for ( i = 0; i < size; i++ )
  {
    if ( lcg_next( -1, 100 ) > skill )
    {
      if ( isupper( argument[i] ) )
        buf[i] = UPPER( language_table[language].replace[lcg_next( -1, 100 )] );
      else if ( islower( argument[i] ) )
        buf[i] = language_table[language].replace[lcg_next( -1, 100 )];
      else
      {
        buf[i] = argument[i];
	lcg_next( -1, 100 );
      }
    }
    else
    {
      buf[i] = argument[i];
      lcg_next( -1, 100 );
    }
  }

  buf[i] = '\0';

  return buf;
}


int language_lookup( char *argument )
{
  int i;

  for ( i = 0; i < MAX_LANGUAGE; i++ )
    if ( !str_prefix( argument, skill_table[*language_table[i].pgsn].name ) )
      return i;

  return -1;
}


/* method = "says", "shouts" */
void language_to_char( CHAR_DATA *ch, CHAR_DATA *vch, char *argument, int door,
  char *method, bool fLearn )
{
  char buf[MAX_STRING_LENGTH];
  int cskill, vskill, eskill, gsn;
  extern char *const dir_name_2[];
  extern sh_int rev_dir[];

  if ( vch->desc == NULL )
    return;

  gsn = *language_table[ch->language].pgsn;
  cskill = get_skill( ch, gsn );
  vskill = get_skill( vch, gsn );

  if ( door >= 0 && door <= 5 )
  {
    sprintf( buf, "Someone %s from %s, in ",
      method, dir_name_2[rev_dir[door]] );
  }
  else
  {
    sprintf( buf, "%s %s, in ", person( vch, ch ), method );
  }

  

  if ( vskill > 10 )
    strcat( buf, skill_table[gsn].name );
  else if ( is_name( "tongues", skill_table[*language_table[ch->language].pgsn].name ) )
    strcat( buf, skill_table[gsn].name );
  else
    strcat( buf, "an unknown tongue" );
 
  strcat( buf, ":\n\r   \"" );

  eskill = cskill * vskill / 100;

  if ( eskill == 100 )
    strcat( buf, argument );
   else if ( is_name( "tongues", skill_table[*language_table[ch->language].pgsn].name ) )
    strcat( buf, argument );
  else
  {
    strcat( buf, scramble_language( eskill, ch->language, argument ) );
    if ( fLearn && ( cskill == 100 || ( vskill + 19 ) < cskill ) )
      skill_improve( vch, gsn );
  }

  strcat( buf, "\"." );

  act( buf, ch, vch, NULL, NULL, NULL, TO_VICT, SENSE_HEARING );

  return;
}

void    language_tell_to_char( CHAR_DATA *ch, CHAR_DATA *vch, CHAR_DATA *victim, char *argument, char *method, bool fLearn )
{
  char buf[MAX_STRING_LENGTH];
  char buf2[MAX_STRING_LENGTH];
  int cskill, vskill, eskill, gsn;
  
  if ( vch->desc == NULL )
    return;

  gsn = *language_table[ch->language].pgsn;
  cskill = get_skill( ch, gsn );
  vskill = get_skill( vch, gsn );

  
      sprintf( buf, "%s %s %s, in ", person( vch, ch ), method, person( vch, victim ) );
	  sprintf( buf2,"%s %s you, in ", person( vch, ch ), method );
  

  

  if ( vskill > 10 ){
    strcat( buf, skill_table[gsn].name );
	strcat( buf2, skill_table[gsn].name );
  }
	else if ( is_name( "tongues", skill_table[*language_table[ch->language].pgsn].name ) ){
    strcat( buf, skill_table[gsn].name );
    strcat( buf2, skill_table[gsn].name );
	}
	else {
    strcat( buf, "an unknown tongue" );
	strcat( buf2, "an unknown tongue" );
	}
 
  strcat( buf, ":\n\r   \"" );
  strcat( buf2, ":\n\r  \"" );

  eskill = cskill * vskill / 100;

  if ( eskill == 100 ){
    strcat( buf, argument );
  strcat( buf2, argument );
	}
	else if ( is_name( "tongues", skill_table[*language_table[ch->language].pgsn].name ) ) {
    strcat( buf, argument );
	strcat( buf2, argument );
	}
  else
  {
    strcat( buf, scramble_language( eskill, ch->language, argument ) );
	strcat( buf2, scramble_language( eskill, ch->language, argument ) );
    if ( fLearn && ( cskill == 100 || ( vskill + 19 ) < cskill ) )
      skill_improve( vch, gsn );
  }

  strcat( buf, "\"" );
  strcat( buf2, "\"" );

  act( buf, ch, vch, NULL, victim, NULL, TO_NOTVICT, SENSE_HEARING );
  act( buf2, ch, victim, NULL, NULL, NULL, TO_VICT, SENSE_HEARING );

  return;
}
