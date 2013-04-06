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


NAME_DATA *names_list;


/*
 *  Load name data.
 */
void load_names( void )
{
    FILE *fpList;
    FILE *fpName;
    NAME_DATA *pName;
    char strName[MAX_INPUT_LENGTH];
    char filename[MAX_INPUT_LENGTH];
    int i, total;

    sprintf( filename, "%s%s", NAME_DIR, NAME_FILE );

    if ( ( fpList = fopen( filename, "r" ) ) == NULL )
    {
	perror( filename );
	exit( 1 );
    }

    for ( ; ; )
    {
        strcpy( filename, fread_word( fpList ) );
	if ( filename[0] == '$' )
	    break;

	sprintf( strName, "%s%s.male", NAME_DIR, filename );

	if ( ( fpName = fopen( strName, "r" ) ) == NULL )
	{
	    perror( strName );
	    exit( 1 );
	}

	pName = alloc_perm( sizeof(*pName) );

	pName->filename = str_dup( filename );

	total = fread_number( fpName );
	pName->n_male_names = total;
	pName->male_names = (char **)
		malloc( sizeof(char *) * pName->n_male_names );

	for ( i = 0; i < total; i++ )
	    pName->male_names[i] = fread_string_full( fpName, '\n', TRUE );

	fclose( fpName );


	sprintf( strName, "%s%s.female", NAME_DIR, filename );

	if ( ( fpName = fopen( strName, "r" ) ) == NULL )
	{
	    perror( strName );
	    exit( 1 );
	}

	total = fread_number( fpName );
	pName->n_female_names = total;
	pName->female_names = (char **)
		malloc( sizeof(char *) * pName->n_female_names );

	for ( i = 0; i < total; i++ )
	    pName->female_names[i] = fread_string_full( fpName, '\n', TRUE );

	fclose( fpName );

	pName->next = names_list;
	names_list = pName;
    }
    fclose( fpList );
}



void assign_name( CHAR_DATA *ch )
{
    NAME_DATA *pName;
    int n;

    if ( !IS_NPC( ch ) )
	return;

    for ( pName = names_list; pName != NULL; pName = pName->next )
	if ( !str_cmp( ch->name, pName->filename ) )
	    break;

    if ( pName == NULL )
	return;

    if ( ch->sex == SEX_MALE
    || ( ch->sex == SEX_NEUTRAL && number_bits( 1 ) ) )
    {
	n = number_range( 1, pName->n_male_names ) - 1;
	ch->name = str_dup( pName->male_names[n] );
    }
    else
    {
	n = number_range( 1, pName->n_female_names ) - 1;
	ch->name = str_dup( pName->female_names[n] );
    }

    return;
}
