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



extern char str_empty[1];
extern char *top_string;
extern char *string_space;
extern int nAllocString;
extern int sAllocString;
extern bool fBootDb;
extern FILE *fpArea;
extern char strArea[MAX_INPUT_LENGTH];

char *			string_hash		[MAX_KEY_HASH];


/*
 * Read a letter from a file.
 */
char fread_letter( FILE *fp )
{
    char c;

    do
    {
	c = getc( fp );
    }
    while ( isspace(c) );

    return c;
}



/*
 * Read a number from a file.
 */
int fread_number( FILE *fp )
{
    int number;
    bool sign;
    char c;

    do
    {
	c = getc( fp );
    }
    while ( isspace(c) );

    number = 0;

    sign   = FALSE;
    if ( c == '+' )
    {
	c = getc( fp );
    }
    else if ( c == '-' )
    {
	sign = TRUE;
	c = getc( fp );
    }

    if ( !isdigit(c) )
    {
	bug( "Fread_number: bad format.", 0 );
	exit( 1 );
    }

    while ( isdigit(c) )
    {
	number = number * 10 + c - '0';
	c      = getc( fp );
    }

    if ( sign )
	number = 0 - number;

    if ( c == '|' )
	number += fread_number( fp );
    else if ( c != ' ' )
	ungetc( c, fp );

    return number;
}


/*
 * Read and allocate space for a string from a file.
 * These strings are read-only and shared.
 * Strings are hashed:
 *   each string prepended with hash pointer to prev string,
 *   hash code is simply the string length.
 * This function takes 40% to 50% of boot-up time.
 */
char *fread_string( FILE *fp )
{
    return fread_string_full( fp, '~', TRUE );
}

/*
 * This function allows leading whitespace.
 */
char *fread_description( FILE *fp )
{
    return fread_string_full( fp, '~', FALSE );
}

/*
 * This allows strings terminated by newline and tilde only.
 */
char *fread_string_full( FILE *fp, char terminator, bool fKillSpace )
{
    char *plast;
    char c;

    plast = top_string + sizeof(char *);
    if ( plast > &string_space[MAX_STRING - MAX_STRING_LENGTH] )
    {
	bug( "Fread_string: MAX_STRING %d exceeded.", MAX_STRING );
	exit( 1 );
    }

    /*
     * Skip blanks.
     * Read first char.
     */
    if ( fKillSpace )
    {
      do
      {
	c = getc( fp );
      }
      while ( isspace(c) );
    }
    else
    {
      do
      {
        c = getc( fp );
      }
      while( c == '\n' || c == '\r' );
    }

    if ( ( *plast++ = c ) == '~' )
	return &str_empty[0];

    for ( ;; )
    {
	/*
	 * Back off the char type lookup,
	 *   it was too dirty for portability.
	 *   -- Furey
	 */
	switch ( *plast = getc( fp ) )
	{
	default:
	    plast++;
	    break;

	case EOF:
	    bug( "Fread_string: EOF", 0 );
	    exit( 1 );
	    break;

	case '\\':
	    plast++;
	    *plast = getc( fp );
	    if ( *plast == '~' )
		plast[-1] = '~';
	    else
		plast++;
	    break;

	case '\r':
	    break;

	case '\n':
	    if ( terminator != '\n' )
	    {
		plast++;
		*plast++ = '\r';
		break;
	    }

	case '~':
	    plast++;
	    {
		union
		{
		    char *	pc;
		    char	rgc[sizeof(char *)];
		} u1;
		int ic;
		int iHash;
		char *pHash;
		char *pHashPrev;
		char *pString;

		plast[-1] = '\0';
		iHash     = UMIN( MAX_KEY_HASH - 1, plast - 1 - top_string );
		for ( pHash = string_hash[iHash]; pHash; pHash = pHashPrev )
		{
		    for ( ic = 0; ic < sizeof(char *); ic++ )
			u1.rgc[ic] = pHash[ic];
		    pHashPrev = u1.pc;
		    pHash    += sizeof(char *);

		    if ( top_string[sizeof(char *)] == pHash[0]
		    &&   !strcmp( top_string+sizeof(char *)+1, pHash+1 ) )
			return pHash;
		}

		if ( fBootDb )
		{
		    pString		= top_string;
		    top_string		= plast;
		    u1.pc		= string_hash[iHash];
		    for ( ic = 0; ic < sizeof(char *); ic++ )
			pString[ic] = u1.rgc[ic];
		    string_hash[iHash]	= pString;

		    nAllocString += 1;
		    sAllocString += top_string - pString;
		    return pString + sizeof(char *);
		}
		else
		{
		    return str_dup( top_string + sizeof(char *) );
		}
	    }
	}
    }
}



/*
 * Read to end of line (for comments).
 */
void fread_to_eol( FILE *fp )
{
    char c;

    do
    {
	c = getc( fp );
    }
    while ( c != '\n' && c != '\r' );

    do
    {
	c = getc( fp );
    }
    while ( c == '\n' || c == '\r' );

    ungetc( c, fp );
    return;
}



/*
 * Read one word (into static buffer).
 */
char *fread_word( FILE *fp )
{
    static char word[MAX_INPUT_LENGTH];
    char *pword;
    char cEnd;

    do
    {
	cEnd = getc( fp );
    }
    while ( isspace( cEnd ) );

    if ( cEnd == '\'' || cEnd == '"' )
    {
	pword   = word;
    }
    else
    {
	word[0] = cEnd;
	pword   = word+1;
	cEnd    = ' ';
    }

    for ( ; pword < word + MAX_INPUT_LENGTH; pword++ )
    {
	*pword = getc( fp );
	if ( cEnd == ' ' ? isspace(*pword) : *pword == cEnd )
	{
	    if ( cEnd == ' ' )
		ungetc( *pword, fp );
	    *pword = '\0';
	    return word;
	}
    }

    bug( "Fread_word: word too long.", 0 );
    bug( word, 0 );
    exit( 1 );
    return NULL;
}



/*
 * Removes the tildes from a string.
 * Used for player-entered strings that go into disk files.
 */
void smash_tilde( char *str )
{
    for ( ; *str != '\0'; str++ )
    {
	if ( *str == '~' )
	    *str = '-';
    }

    return;
}



/*
 * Compare strings, case insensitive.
 * Return TRUE if different
 *   (compatibility with historical functions).
 */
bool str_cmp( const char *astr, const char *bstr )
{
    if ( astr == NULL )
    {
	bug( "Str_cmp: null astr.", 0 );
	return TRUE;
    }

    if ( bstr == NULL )
    {
	bug( "Str_cmp: null bstr.", 0 );
	return TRUE;
    }

    for ( ; *astr || *bstr; astr++, bstr++ )
    {
	if ( LOWER(*astr) != LOWER(*bstr) )
	    return TRUE;
    }

    return FALSE;
}



/*
 * Compare strings, case insensitive, for prefix matching.
 * Return TRUE if astr not a prefix of bstr
 *   (compatibility with historical functions).
 */
bool str_prefix( const char *astr, const char *bstr )
{
    if ( astr == NULL )
    {
	bug( "Strn_cmp: null astr.", 0 );
	return TRUE;
    }

    if ( bstr == NULL )
    {
	bug( "Strn_cmp: null bstr.", 0 );
	return TRUE;
    }

    for ( ; *astr; astr++, bstr++ )
    {
	if ( LOWER(*astr) != LOWER(*bstr) )
	    return TRUE;
    }

    return FALSE;
}



/*
 * Compare strings, case insensitive, for match anywhere.
 * Returns TRUE is astr not part of bstr.
 *   (compatibility with historical functions).
 */
bool str_infix( const char *astr, const char *bstr )
{
    int sstr1;
    int sstr2;
    int ichar;
    char c0;

    if ( ( c0 = LOWER(astr[0]) ) == '\0' )
	return FALSE;

    sstr1 = strlen(astr);
    sstr2 = strlen(bstr);

    for ( ichar = 0; ichar <= sstr2 - sstr1; ichar++ )
    {
	if ( c0 == LOWER(bstr[ichar]) && !str_prefix( astr, bstr + ichar ) )
	    return FALSE;
    }

    return TRUE;
}



/*
 * Compare strings, case insensitive, for suffix matching.
 * Return TRUE if astr not a suffix of bstr
 *   (compatibility with historical functions).
 */
bool str_suffix( const char *astr, const char *bstr )
{
    int sstr1;
    int sstr2;

    sstr1 = strlen(astr);
    sstr2 = strlen(bstr);
    if ( sstr1 <= sstr2 && !str_cmp( astr, bstr + sstr2 - sstr1 ) )
	return FALSE;
    else
	return TRUE;
}


/*
 * Returns an initial-capped string.
 */
char *capitalize( const char *str )
{
    static char strcap[MAX_STRING_LENGTH];
    int i;

    for ( i = 0; str[i] != '\0'; i++ )
	strcap[i] = LOWER(str[i]);
    strcap[i] = '\0';
    strcap[0] = UPPER(strcap[0]);
    return strcap;
}

void append_stat( char *file, char *str )
{
//    DESCRIPTOR_DATA *d;
    FILE *fp;
    char buf[MAX_STRING_LENGTH];
//    extern int interp_doer;


sprintf( buf, "rm -f %s%s", AREA_DIR, file  );
system( buf );
sprintf( buf, "touch %s%s", AREA_DIR, file );
system( buf );

    sprintf( buf, "%s\n", str );

    fclose( fpReserve );
    if ( ( fp = fopen( file, "a" ) ) == NULL )
    {
	perror( file );
//	send_to_char( "Could not open the file!\n\r", ch );
    }
    else
    {
	fprintf( fp, buf );
	fclose( fp );
        sprintf( buf, "cp -fr %s%s /usr/data/httpd/users/darksun/%s", AREA_DIR, file, file );
        system( buf );
    }

    fpReserve = fopen( NULL_FILE, "r" );

 /*   for ( d = descriptor_list; d != NULL; d = d->next )
    {
        if ( d->character == NULL || get_trust( d->character ) < 37
        || interp_doer > get_trust( d->character )
        || IS_SET( d->character->deaf, CHANNEL_LOG )
        || d->connected != CON_PLAYING )
            continue;

        send_to_char( buf, d->character );
    }*/

    return;
}



/*
 * Append a string to a file.
 */
void append_file( CHAR_DATA *ch, char *file, char *str )
{
    DESCRIPTOR_DATA *d;
    FILE *fp;
    char buf[MAX_STRING_LENGTH];
    extern int interp_doer;

    if ( IS_NPC(ch) || str[0] == '\0' )
	return;

    sprintf( buf, "[%5d] %s: %s\n",
            ch->in_room ? ch->in_room->vnum : 0, ch->name, str );

    fclose( fpReserve );
    if ( ( fp = fopen( file, "a" ) ) == NULL )
    {
	perror( file );
	send_to_char( "Could not open the file!\n\r", ch );
    }
    else
    {
	fprintf( fp, buf );
	fclose( fp );
    }

    fpReserve = fopen( NULL_FILE, "r" );

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
        if ( d->character == NULL || get_trust( d->character ) < 34
        || interp_doer > get_trust( d->character )
        || IS_SET( d->character->deaf, CHANNEL_LOG )
        || d->connected != CON_PLAYING )
            continue;

        send_to_char( buf, d->character );
    }

    return;
}



/*
 * Reports a bug.
 */
void bug( const char *str, int param )
{
    char buf[MAX_STRING_LENGTH];
    FILE *fp;

    if ( fpArea != NULL )
    {
	int iLine;
	int iChar;

	if ( fpArea == stdin )
	{
	    iLine = 0;
	}
	else
	{
	    iChar = ftell( fpArea );
	    fseek( fpArea, 0, 0 );
	    for ( iLine = 0; ftell( fpArea ) < iChar; iLine++ )
	    {
		while ( getc( fpArea ) != '\n' )
		    ;
	    }
	    fseek( fpArea, iChar, 0 );
	}

	sprintf( buf, "[*****] FILE: %s LINE: %d", strArea, iLine );
	log_string( buf );

	if ( ( fp = fopen( "shutdown.txt", "a" ) ) != NULL )
	{
	    fprintf( fp, "[*****] %s\n", buf );
	    fclose( fp );
	}
    }

    strcpy( buf, "[*****] BUG: " );
    sprintf( buf + strlen(buf), str, param );
    log_string( buf );

    fclose( fpReserve );
    if ( ( fp = fopen( BUG_FILE, "a" ) ) != NULL )
    {
	fprintf( fp, "%s\n", buf );
	fclose( fp );
    }
    fpReserve = fopen( NULL_FILE, "r" );

    return;
}



/*
 * Writes a string to the log.
 */
void log_string( const char *str )
{
    DESCRIPTOR_DATA *d;
    char *strtime;
    extern int interp_doer;

    strtime                    = ctime( &current_time );
    strtime[strlen(strtime)-1] = '\0';
    fprintf( stderr, "%s :: %s\n", strtime, str );

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
        if ( d->character == NULL || get_trust( d->character ) < 37
        || interp_doer > get_trust( d->character )
        || IS_SET( d->character->deaf, CHANNEL_LOG )
        || d->connected != CON_PLAYING )
            continue;

        send_to_char( str, d->character );
        send_to_char( "\n\r", d->character );
    }

    return;
}
