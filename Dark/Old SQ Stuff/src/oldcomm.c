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
 *  Thanks to abaddon for proof-reading our comm.c and pointing out bugs.  *
 *  Any remaining bugs are, of course, our work, not his.  :)              *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

/*
 * This file contains all of the OS-dependent stuff:
 *   startup, signals, BSD sockets for tcp/ip, i/o, timing.
 *
 * The data flow for input is:
 *    Game_loop ---> Read_from_descriptor ---> Read
 *    Game_loop ---> Read_from_buffer
 *
 * The data flow for output is:
 *    Game_loop ---> Process_Output ---> Write_to_descriptor -> Write
 *
 * The OS-dependent functions are Read_from_descriptor and Write_to_descriptor.
 * -- Furey  26 Jan 1993
 */

#include <sys/types.h>
#include <sys/time.h>

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "merc.h"



/*
 * Malloc debugging stuff.
 */
#if defined(sun)
#undef MALLOC_DEBUG
#endif

#if defined(MALLOC_DEBUG)
#include <malloc.h>
extern	int	malloc_debug	args( ( int  ) );
extern	int	malloc_verify	args( ( void ) );
#endif



/*
 * Signal handling.
 * Apollo has a problem with __attribute(atomic) in signal.h,
 *   I dance around it.
 */
#if defined(apollo)
#define __attribute(x)
#endif

#if defined(unix)
#include <signal.h>
#endif

#if defined(apollo)
#undef __attribute
#endif



/*
 * Socket and TCP/IP stuff.
 */
#if	defined(unix)
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/telnet.h>
const	char	echo_off_str	[] = { IAC, WILL, TELOPT_ECHO, '\0' };
const	char	echo_on_str	[] = { IAC, WONT, TELOPT_ECHO, '\0' };
const	char 	go_ahead_str	[] = { IAC, GA, '\0' };
#endif



/*
 * OS-dependent declarations.
 */
#if	defined(_AIX)
#include <sys/select.h>
int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
int	bind		args( ( int s, struct sockaddr *name, int namelen ) );
void	bzero		args( ( char *b, int length ) );
int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
/*
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
*/
int	listen		args( ( int s, int backlog ) );
int	setsockopt	args( ( int s, int level, int optname, void *optval,
			    int optlen ) );
int	socket		args( ( int domain, int type, int protocol ) );
#endif

#if	defined(apollo)
#include <unistd.h>
void	bzero		args( ( char *b, int length ) );
#endif

#if	defined(__hpux)
int	accept		args( ( int s, void *addr, int *addrlen ) );
int	bind		args( ( int s, const void *addr, int addrlen ) );
void	bzero		args( ( char *b, int length ) );
int	getpeername	args( ( int s, void *addr, int *addrlen ) );
int	getsockname	args( ( int s, void *name, int *addrlen ) );
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
int	listen		args( ( int s, int backlog ) );
int	setsockopt	args( ( int s, int level, int optname,
 				const void *optval, int optlen ) );
int	socket		args( ( int domain, int type, int protocol ) );
#endif

#if	defined(interactive)
#include <net/errno.h>
#include <sys/fcntl.h>
#endif

#if	defined(linux)
#if 0
int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
int	bind		args( ( int s, struct sockaddr *name, int namelen ) );
#endif
int	close		args( ( int fd ) );
/*
int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
int	listen		args( ( int s, int backlog ) );
*/
int	read		args( ( int fd, char *buf, int nbyte ) );
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			    fd_set *exceptfds, struct timeval *timeout ) );
int	socket		args( ( int domain, int type, int protocol ) );
int	write		args( ( int fd, char *buf, int nbyte ) );
#endif

#if	defined(NeXT)
int	close		args( ( int fd ) );
int	fcntl		args( ( int fd, int cmd, int arg ) );
#if	!defined(htons)
u_short	htons		args( ( u_short hostshort ) );
#endif
#if	!defined(ntohl)
u_long	ntohl		args( ( u_long hostlong ) );
#endif
int	read		args( ( int fd, char *buf, int nbyte ) );
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			    fd_set *exceptfds, struct timeval *timeout ) );
int	write		args( ( int fd, char *buf, int nbyte ) );
#endif

#if	defined(sequent)
int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
int	bind		args( ( int s, struct sockaddr *name, int namelen ) );
int	close		args( ( int fd ) );
int	fcntl		args( ( int fd, int cmd, int arg ) );
int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
#if	!defined(htons)
u_short	htons		args( ( u_short hostshort ) );
#endif
int	listen		args( ( int s, int backlog ) );
#if	!defined(ntohl)
u_long	ntohl		args( ( u_long hostlong ) );
#endif
int	read		args( ( int fd, char *buf, int nbyte ) );
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			    fd_set *exceptfds, struct timeval *timeout ) );
int	setsockopt	args( ( int s, int level, int optname, caddr_t optval,
			    int optlen ) );
int	socket		args( ( int domain, int type, int protocol ) );
int	write		args( ( int fd, char *buf, int nbyte ) );
#endif

/*
 * This includes Solaris SYSV as well.
 */
#if defined(sun)
int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
int	bind		args( ( int s, struct sockaddr *name, int namelen ) );
void	bzero		args( ( char *b, int length ) );
int	close		args( ( int fd ) );
int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
int	listen		args( ( int s, int backlog ) );
int	read		args( ( int fd, char *buf, int nbyte ) );
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			    fd_set *exceptfds, struct timeval *timeout ) );
#if defined(SYSV)
int	setsockopt	args( ( int s, int level, int optname, 
                            const char *optval, int optlen ) );
#else 
int	setsockopt	args( ( int s, int level, int optname, void *optval,
			    int optlen ) );
#endif
int	socket		args( ( int domain, int type, int protocol ) );
int	write		args( ( int fd, char *buf, int nbyte ) );
#endif

#if defined(ultrix)
int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
int	bind		args( ( int s, struct sockaddr *name, int namelen ) );
void	bzero		args( ( char *b, int length ) );
int	close		args( ( int fd ) );
int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
int	listen		args( ( int s, int backlog ) );
int	read		args( ( int fd, char *buf, int nbyte ) );
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			    fd_set *exceptfds, struct timeval *timeout ) );
int	setsockopt	args( ( int s, int level, int optname, void *optval,
			    int optlen ) );
int	socket		args( ( int domain, int type, int protocol ) );
int	write		args( ( int fd, char *buf, int nbyte ) );
#endif



/*
 * Global variables.
 */
DESCRIPTOR_DATA *   descriptor_free = NULL; /* Free list for descriptors */
DESCRIPTOR_DATA *   descriptor_list = NULL; /* All open descriptors	*/
DESCRIPTOR_DATA *   d_next = NULL;	/* Next descriptor in loop	*/
FILE *		    fpReserve;		/* Reserved file handle		*/
bool		    god;		/* All new chars are gods!	*/
bool		    merc_down;		/* Shutdown			*/
bool		    wizlock;		/* Game is wizlocked		*/
bool                downtime;           /* For sunday downtime.         */
char		    str_boot_time[MAX_INPUT_LENGTH];
time_t		    current_time;	/* Time of this pulse		*/



/*
 * OS-dependent local functions.
 */
#if defined(unix)
void	game_loop_unix		args( ( int control ) );
int	init_socket		args( ( int port ) );
void	new_descriptor		args( ( int control ) );
bool	read_from_descriptor	args( ( DESCRIPTOR_DATA *d ) );
bool	write_to_descriptor	args( ( int desc, char *txt, int length ) );
#endif




/*
 * Other local functions (OS-independent).
 */
bool	check_parse_name	args( ( char *name ) );
bool	check_reconnect		args( ( DESCRIPTOR_DATA *d, char *name,
				    bool fConn ) );
bool	check_examine		args( ( char *name ) );
void	reconnect		args( ( DESCRIPTOR_DATA *d, CHAR_DATA *ch ) );
void	free_descriptor		args( ( DESCRIPTOR_DATA *d ) );
int	main			args( ( int argc, char **argv ) );
void	nanny			args( ( DESCRIPTOR_DATA *d, char *argument ) );
bool	process_output		args( ( DESCRIPTOR_DATA *d, bool fPrompt ) );
void	read_from_buffer	args( ( DESCRIPTOR_DATA *d ) );
void	stop_idling		args( ( CHAR_DATA *ch ) );
void	write_prompt		args( ( DESCRIPTOR_DATA *d ) );



int main( int argc, char **argv )
{
    struct timeval now_time;
    int port;

#if defined(unix)
    int control;
#endif

  /*
     * Memory debugging if needed.
     */

#if defined(MALLOC_DEBUG)
    malloc_debug( 2 );
#endif

    /*
     * Init time.

     */
    

    gettimeofday( &now_time, NULL );
    current_time = (time_t) now_time.tv_sec;
    strcpy( str_boot_time, ctime( &current_time ) );

    /*
     * Reserve one channel for our use.
     */
    if ( ( fpReserve = fopen( NULL_FILE, "r" ) ) == NULL )
    {
	perror( NULL_FILE );
	exit( 1 );
    }

    /*
     * Get the port number.
     */
    port = 5040;
    if ( argc > 1 )
    {
	if ( !is_number( argv[1] ) )
	{
	    fprintf( stderr, "Usage: %s [port #]\n", argv[0] );
	    exit( 1 );
	}
	else if ( ( port = atoi( argv[1] ) ) <= 1024 )
	{
	    fprintf( stderr, "Port number must be above 1024.\n" );
	    exit( 1 );
	}
    }

    /*
     * Run the game.
     */
#if defined(unix)
    control = init_socket( port );
    boot_db( );
    sprintf( log_buf, "Merc is ready to rock on port %d.", port );
    log_string( log_buf );
    game_loop_unix( control );
    close( control );
#endif

    /*
     * That's all, folks.
     */
    log_string( "Normal termination of game." );
    exit( 0 );
    return 0;
}



#if defined(unix)
int init_socket( int port )
{
    static struct sockaddr_in sa_zero;
    struct sockaddr_in sa;
    int x = 1;
    int fd;

    if ( ( fd = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 )
    {
	perror( "Init_socket: socket" );
	exit( 1 );
    }

    if ( setsockopt( fd, SOL_SOCKET, SO_REUSEADDR,
    (char *) &x, sizeof(x) ) < 0 )
    {
	perror( "Init_socket: SO_REUSEADDR" );
	close( fd );
	exit( 1 );
    }

#if defined(SO_DONTLINGER) && !defined(SYSV)
    {
	struct	linger	ld;

	ld.l_onoff  = 1;
	ld.l_linger = 1000;

	if ( setsockopt( fd, SOL_SOCKET, SO_DONTLINGER,
	(char *) &ld, sizeof(ld) ) < 0 )
	{
	    perror( "Init_socket: SO_DONTLINGER" );
	    close( fd );
	    exit( 1 );
	}
    }
#endif

    sa		    = sa_zero;
    sa.sin_family   = AF_INET;
    sa.sin_port	    = htons( port );

    if ( bind( fd, (struct sockaddr *) &sa, sizeof(sa) ) < 0 )
    {
	perror( "Init_socket: bind" );
	close( fd );
	exit( 1 );
    }

    if ( listen( fd, 3 ) < 0 )
    {
	perror( "Init_socket: listen" );
	close( fd );
	exit( 1 );
    }

    return fd;
}
#endif



#if defined(unix)
void game_loop_unix( int control )
{
    static struct timeval null_time;
    struct timeval last_time;

    signal( SIGPIPE, SIG_IGN );
    gettimeofday( &last_time, NULL );
    current_time = (time_t) last_time.tv_sec;

    /* Main loop */
    while ( !merc_down )
    {
	fd_set in_set;
	fd_set out_set;
	fd_set exc_set;
	DESCRIPTOR_DATA *d;
	int maxdesc;

#if defined(MALLOC_DEBUG)
	if ( malloc_verify( ) != 1 )
	    abort( );
#endif

	/*
	 * Poll all active descriptors.
	 */
	FD_ZERO( &in_set  );
	FD_ZERO( &out_set );
	FD_ZERO( &exc_set );
	FD_SET( control, &in_set );
	maxdesc	= control;
	for ( d = descriptor_list; d; d = d->next )
	{
	    maxdesc = UMAX( maxdesc, d->descriptor );
	    FD_SET( d->descriptor, &in_set  );
	    FD_SET( d->descriptor, &out_set );
	    FD_SET( d->descriptor, &exc_set );
	}

	if ( select( maxdesc+1, &in_set, &out_set, &exc_set, &null_time ) < 0 )
	{
	    perror( "Game_loop: select: poll" );
	    exit( 1 );
	}

	/*
	 * New connection?
	 */
	if ( FD_ISSET( control, &in_set ) )
	    new_descriptor( control );

	/*
	 * Kick out the freaky folks.
	 */
	for ( d = descriptor_list; d != NULL; d = d_next )
	{
	    d_next = d->next;   
	    if ( FD_ISSET( d->descriptor, &exc_set ) )
	    {
		FD_CLR( d->descriptor, &in_set  );
		FD_CLR( d->descriptor, &out_set );
		if ( d->character
		&& !IS_SET( d->character->act, PLR_UNAPPROVED )
		&& !IS_SET( d->character->act, PLR_REJECTED ) )
		    save_char_obj( d->character );
		d->outtop	= 0;
		close_socket( d );
	    }
	}

	/*
	 * Process input.
	 */
	for ( d = descriptor_list; d != NULL; d = d_next )
	{
	    d_next	= d->next;
	    d->fcommand	= FALSE;

	    if ( FD_ISSET( d->descriptor, &in_set ) )
	    {
		if ( d->character != NULL )
		    d->character->timer = 0;
		if ( !read_from_descriptor( d ) )
		{
		    FD_CLR( d->descriptor, &out_set );
		    if ( d->character != NULL
		    && !IS_SET( d->character->act, PLR_UNAPPROVED )
		    && !IS_SET( d->character->act, PLR_REJECTED ) )
			save_char_obj( d->character );
		    d->outtop	= 0;
		    close_socket( d );
		    continue;
		}
	    }

	    if ( d->character != NULL && d->character->wait > 0 )
	    {
		--d->character->wait;
		continue;
	    }

	    read_from_buffer( d );
	    if ( d->incomm[0] != '\0' )
	    {
		d->fcommand	= TRUE;
		stop_idling( d->character );

		if ( d->connected == CON_PLAYING )
		    if (d->showstr_point)
			show_string( d, d->incomm );
		    else
			interpret( d->character, d->incomm );
		else
		    nanny( d, d->incomm );

		d->incomm[0]	= '\0';
	    }
	}



	/*
	 * Autonomous game motion.
	 */
	update_handler( );



	/*
	 * Output.
	 */
	for ( d = descriptor_list; d != NULL; d = d_next )
	{
	    d_next = d->next;

	    if ( ( d->fcommand || d->outtop > 0 )
	    &&   FD_ISSET(d->descriptor, &out_set) )
	    {
		if ( !process_output( d, TRUE ) )
		{
		    if ( d->character != NULL
		    && !IS_SET( d->character->act, PLR_UNAPPROVED )
		    && !IS_SET( d->character->act, PLR_REJECTED ) )
			save_char_obj( d->character );
		    d->outtop	= 0;
		    close_socket( d );
		}
	    }
	}



	/*
	 * Synchronize to a clock.
	 * Sleep( last_time + 1/PULSE_PER_SECOND - now ).
	 * Careful here of signed versus unsigned arithmetic.
	 */
	{
	    struct timeval now_time;
	    long secDelta;
	    long usecDelta;

	    gettimeofday( &now_time, NULL );
	    usecDelta	= ((int) last_time.tv_usec) - ((int) now_time.tv_usec)
			+ 1000000 / PULSE_PER_SECOND;
	    secDelta	= ((int) last_time.tv_sec ) - ((int) now_time.tv_sec );
	    while ( usecDelta < 0 )
	    {
		usecDelta += 1000000;
		secDelta  -= 1;
	    }

	    while ( usecDelta >= 1000000 )
	    {
		usecDelta -= 1000000;
		secDelta  += 1;
	    }

	    if ( secDelta > 0 || ( secDelta == 0 && usecDelta > 0 ) )
	    {
		struct timeval stall_time;

		stall_time.tv_usec = usecDelta;
		stall_time.tv_sec  = secDelta;
		if ( select( 0, NULL, NULL, NULL, &stall_time ) < 0 )
		{
		    perror( "Game_loop: select: stall" );
		    exit( 1 );
		}
	    }
	}

	gettimeofday( &last_time, NULL );
	current_time = (time_t) last_time.tv_sec;
    }

    return;
}
#endif



#if defined(unix)
void new_descriptor( int control )
{
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *dnew;
    BAN_DATA *pban;
    struct sockaddr_in sock;
    struct hostent *from;
    int desc;
    int size;
//    extern int interp_doer;

    size = sizeof(sock);
    getsockname( control, (struct sockaddr *) &sock, &size );
    if ( ( desc = accept( control, (struct sockaddr *) &sock, &size) ) < 0 )
    {
	perror( "New_descriptor: accept" );
	return;
    }

#if !defined(FNDELAY)
#define FNDELAY O_NDELAY
#endif

    if ( fcntl( desc, F_SETFL, FNDELAY ) == -1 )
    {
	perror( "New_descriptor: fcntl: FNDELAY" );
	return;
    }

    /*
     * Cons a new descriptor.
     */
    if ( descriptor_free == NULL )
    {
	dnew		= alloc_perm( sizeof(*dnew) );
    }
    else
    {
	dnew		= descriptor_free;
	descriptor_free	= descriptor_free->next;
    }

    memset( dnew, 0x00, sizeof( *dnew ) );
    dnew->next = NULL;
    dnew->snoop_by = NULL;
    dnew->character = NULL;
    dnew->original = NULL;
    dnew->host = NULL;
    dnew->descriptor = 0;
    dnew->fcommand = 0;
    dnew->inbuf[0] = '\0';
    dnew->incomm[0] = '\0';
    dnew->inlast[0] = '\0';
    dnew->repeat = 0;
    dnew->outtop = 0;
    dnew->ansi_enable = 0;

    dnew->outsize	= 2000;
    dnew->outbuf	= alloc_mem( dnew->outsize );
    dnew->descriptor	= desc;
    dnew->connected	= CON_ANSI_ENABLE;
    dnew->showstr_head	= NULL;
    dnew->showstr_point	= NULL;

    size = sizeof(sock);
    if ( getpeername( desc, (struct sockaddr *) &sock, &size ) < 0 )
    {
	perror( "New_descriptor: getpeername" );
	dnew->host = str_dup( "(unknown)" );
    }
    else
    {
	/*
	 * Would be nice to use inet_ntoa here but it takes a struct arg,
	 * which ain't very compatible between gcc and system libraries.
	 */
	int addr;

	addr = ntohl( sock.sin_addr.s_addr );
	sprintf( buf, "%d.%d.%d.%d",
	    ( addr >> 24 ) & 0xFF, ( addr >> 16 ) & 0xFF,
	    ( addr >>  8 ) & 0xFF, ( addr       ) & 0xFF
	    );
      /*  
	sprintf( log_buf, "Sock.sinaddr:  %s", buf );
	interp_doer = MAX_LEVEL;
	log_string( log_buf );*/
	from = gethostbyaddr( (char *) &sock.sin_addr,
	    sizeof(sock.sin_addr), AF_INET );
	dnew->host = str_dup( from ? from->h_name : buf );
    }
	
    /*
     * Swiftest: I added the following to ban sites.  I don't
     * endorse banning of sites, but Copper has few descriptors now
     * and some people from certain sites keep abusing access by
     * using automated 'autodialers' and leaving connections hanging.
     *
     * Furey: added suffix check by request of Nickel of HiddenWorlds.
     */
    for ( pban = ban_list; pban != NULL; pban = pban->next )
    {
	if ( !str_suffix( pban->name, dnew->host ) )
	{
	    write_to_descriptor( desc,
		"Your site has been banned from this Mud.\n\r", 0 );
	    close( desc );
	    free_string( dnew->host, MEM_GENERAL );
	    free_mem( dnew->outbuf, dnew->outsize );
	    dnew->next		= descriptor_free;
	    descriptor_free	= dnew;
	    return;
	}
    }

    /*
     * Init descriptor data.
     */
    dnew->next			= descriptor_list;
    descriptor_list		= dnew;

    /*
     * Send the greeting.
     */
    {
/*
	extern char * help_greeting;
	if ( help_greeting[0] == '.' )
	    write_to_buffer( dnew, help_greeting+1, 0 );
	else
	    write_to_buffer( dnew, help_greeting  , 0 );
*/      
            if ( wizlock && downtime )
            {

                write_to_buffer( dnew, "Game status: WIZLOCKED & DOWNTIME\n\r", 0 );
                write_to_buffer( dnew, "Enable VT100 text (n) ? ", 0 );
                return;
            }
            else if ( wizlock )
            {
    sprintf( buf, "rm -f %sstatus.txt", AREA_DIR  );
    system( buf );
    sprintf( buf, "mv -f %snewstat %sstatus.txt", AREA_DIR, AREA_DIR );
    system( buf );

    append_stat(  STAT_FILE, "Wizlocked" );

    sprintf( buf, "cp -fr %sstatus.txt /usr/data/httpd/users/darksun/status.txt", AREA_DIR );
    system( buf );

                write_to_buffer( dnew, "Game status: WIZLOCKED\n\r", 0 );
                write_to_buffer( dnew, "Enable VT100 text (n) ? ", 0 );
                return;
            }
            else if ( downtime )
            {

            sprintf( buf, "rm -f %sstatus.txt", AREA_DIR  );
    system( buf );
    sprintf( buf, "mv -f %snewstat %sstatus.txt", AREA_DIR, AREA_DIR );
    system( buf );

    append_stat(  STAT_FILE, "Sunday Downtime" );

    sprintf( buf, "cp -fr %sstatus.txt /usr/data/httpd/users/darksun/status.txt", AREA_DIR );
    system( buf );
                write_to_buffer( dnew, "Game status: DOWNTIME\n\r", 0 );
                write_to_buffer( dnew, "Enable VT100 text (n) ? ", 0 );
                return;
            }
            else
            {

    sprintf( buf, "rm -f %sstatus.txt", AREA_DIR  );
    system( buf );
    sprintf( buf, "mv -f %snewstat %sstatus.txt", AREA_DIR, AREA_DIR );
    system( buf );

    append_stat(  STAT_FILE, "Normal" );

    sprintf( buf, "cp -fr %sstatus.txt /usr/data/httpd/users/darksun/status.txt", AREA_DIR );
    system( buf );
                write_to_buffer( dnew, "Game status: NORMAL\n\r", 0 );
                write_to_buffer( dnew, "Enable VT100 text (n) ? ", 0 );
                return;
            }
	write_to_buffer( dnew, "Enable VT100 text (n) ? ", 0 );
    }

    return;
}
#endif



void close_socket( DESCRIPTOR_DATA *dclose )
{
    CHAR_DATA *ch;
    extern int interp_doer;

    if ( dclose->outtop > 0 )
	process_output( dclose, FALSE );

    if ( dclose->snoop_by != NULL )
    {
	write_to_buffer( dclose->snoop_by,
	    "Your victim has left the game.\n\r", 0 );
    }

    {
	DESCRIPTOR_DATA *d;

	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    if ( d->snoop_by == dclose )
		d->snoop_by = NULL;
	}
    }

    if ( ( ch = dclose->character ) != NULL )
    {
        interp_doer = get_trust( ch );
	sprintf( log_buf, "Closing link to %s.", ch->name );
	log_string( log_buf );
	if ( dclose->connected == CON_PLAYING )
	{
	    act( "$n has lost $s link.", ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIXTH );
	    ch->desc = NULL;
	}
	else
	{
	    free_char( dclose->character );
	    dclose->character = NULL;
	}
    }

    if ( d_next == dclose )
	d_next = d_next->next;   

    if ( dclose == descriptor_list )
    {
	descriptor_list = descriptor_list->next;
    }
    else
    {
	DESCRIPTOR_DATA *d;

	for ( d = descriptor_list; d && d->next != dclose; d = d->next )
	    ;
	if ( d != NULL )
	    d->next = dclose->next;
	else
	    bug( "Close_socket: dclose not found.", 0 );
    }

    close( dclose->descriptor );
    free_string( dclose->host, MEM_GENERAL );
    free_mem( dclose->outbuf, dclose->outsize );
    dclose->next	= descriptor_free;
    descriptor_free	= dclose;
    return;
}



bool read_from_descriptor( DESCRIPTOR_DATA *d )
{
    int iStart;
    extern int interp_doer;

    interp_doer = MAX_LEVEL;

    /* Hold horses if pending command already. */
    if ( d->incomm[0] != '\0' )
	return TRUE;

    /* Check for overflow. */
    iStart = strlen(d->inbuf);
    if ( iStart >= sizeof(d->inbuf) - 10 )
    {
	sprintf( log_buf, "%s input overflow!", d->host );
	log_string( log_buf );
	write_to_descriptor( d->descriptor,
	    "\n\r*** PUT A LID ON IT!!! ***\n\r", 0 );
	return FALSE;
    }

    /* Snarf input. */
#if defined(unix)
    for ( ; ; )
    {
	int nRead;

	nRead = read( d->descriptor, d->inbuf + iStart,
	    sizeof(d->inbuf) - 10 - iStart );
	if ( nRead > 0 )
	{
	    iStart += nRead;
	    if ( d->inbuf[iStart-1] == '\n' || d->inbuf[iStart-1] == '\r' )
		break;
	}
	else if ( nRead == 0 )
	{
//	    log_string( "EOF encountered on read." );
	    return FALSE;
	}
	else if ( errno == EWOULDBLOCK )
	    break;
	else
	{
	    perror( "Read_from_descriptor" );
	    return FALSE;
	}
    }
#endif

    d->inbuf[iStart] = '\0';
    return TRUE;
}



/*
 * Transfer one line from input buffer to input line.
 */
void read_from_buffer( DESCRIPTOR_DATA *d )
{
    int i, j, k;

    /*
     * Hold horses if pending command already.
     */
    if ( d->incomm[0] != '\0' )
	return;

    /*
     * Look for at least one new line.
     */
    for ( i = 0; d->inbuf[i] != '\n' && d->inbuf[i] != '\r'; i++ )
    {
	if ( d->inbuf[i] == '\0' )
	    return;
    }

    /*
     * Canonical input processing.
     */
    for ( i = 0, k = 0; d->inbuf[i] != '\n' && d->inbuf[i] != '\r'; i++ )
    {
	if ( k >= MAX_INPUT_LENGTH - 2 )
	{
	    write_to_descriptor( d->descriptor, "Line too long.\n\r", 0 );

	    /* skip the rest of the line */
	    for ( ; d->inbuf[i] != '\0'; i++ )
	    {
		if ( d->inbuf[i] == '\n' || d->inbuf[i] == '\r' )
		    break;
	    }
	    d->inbuf[i]   = '\n';
	    d->inbuf[i+1] = '\0';
	    break;
	}

	if ( d->inbuf[i] == '\b' && k > 0 )
	    --k;
	else if ( isascii(d->inbuf[i]) && isprint(d->inbuf[i]) )
	    d->incomm[k++] = d->inbuf[i];
    }

    /*
     * Finish off the line.
     */
    if ( k == 0 )
	d->incomm[k++] = ' ';
    d->incomm[k] = '\0';

    /*
     * Deal with bozos with #repeat 1000 ...
     */
    if ( k > 1 || d->incomm[0] == '!' )
    {
    	if ( d->incomm[0] != '!' && strcmp( d->incomm, d->inlast ) )
	{
	    d->repeat = 0;
	}
	else
	{
	    if ( ++d->repeat >= 20 && d->character->level < 40 )
	    {
		sprintf( log_buf, "%s input spamming!", d->host );
		log_string( log_buf );
		write_to_descriptor( d->descriptor,
		    "\n\rWe thank you for not spamming.\n\r", 0 );
		strcpy( d->incomm, "quit" );
	    }
	}
    }

    /*
     * Do '!' substitution.
     */
    if ( d->incomm[0] == '!' )
	strcpy( d->incomm, d->inlast );
    else
	strcpy( d->inlast, d->incomm );

    /*
     * Shift the input buffer.
     */
    while ( d->inbuf[i] == '\n' || d->inbuf[i] == '\r' )
	i++;
    for ( j = 0; ( d->inbuf[j] = d->inbuf[i+j] ) != '\0'; j++ )
	;
    return;
}



/*
 * Low level output function.
 */
bool process_output( DESCRIPTOR_DATA *d, bool fPrompt )
{
    extern bool merc_down;

    /*
     * Bust a prompt.
     */
    if ( fPrompt && !merc_down && d->connected == CON_PLAYING )
    {
	CHAR_DATA *ch;

	if ( d->showstr_point )
	{
	    write_to_buffer( d,
	"****press return****", 0 );
	}
	else
	{
	ch = d->original ? d->original : d->character;
	if ( IS_SET(ch->act, PLR_BLANK) )
	    write_to_buffer( d, "\n\r", 2 );

	write_prompt( d );

	if ( IS_SET(ch->act, PLR_TELNET_GA) )
	    write_to_buffer( d, go_ahead_str, 0 );
	}
    }

    /*
     * Short-circuit if nothing to write.
     */
    if ( d->outtop == 0 )
	return TRUE;

    /*
     * Snoop-o-rama.
     */
    if ( d->snoop_by != NULL )
    {
	write_to_buffer( d->snoop_by, "% ", 2 );
	write_to_buffer( d->snoop_by, d->outbuf, d->outtop );
    }

    /*
     * OS-dependent output.
     */
    if ( !write_to_descriptor( d->descriptor, d->outbuf, d->outtop ) )
    {
	d->outtop = 0;
	return FALSE;
    }
    else
    {
	d->outtop = 0;
	return TRUE;
    }
}



void write_prompt( DESCRIPTOR_DATA *d )
{
   char buf[MAX_STRING_LENGTH];
   char buf2[MAX_STRING_LENGTH];
   const char *str;
   const char *i;
   char *point;
   char *stdprompt = "%h %v > ";
   CHAR_DATA *ch;

   ch = d->character;

   if( ch->prompt == NULL )
	str = stdprompt;
   else if ( ch->prompt[0] == '\0' )
   {
      send_to_char( "\n\r\n\r", ch );
      return;
   }
   else
      str = d->original != NULL ? d->original->prompt : d->character->prompt;

   point = buf;
   while( *str != '\0' )
   {
      if( *str != '%' )
      {
         *point++ = *str++;
         continue;
      }
      ++str;
      switch( *str )
      {
         default :
            i = " "; break;
         case 'h' :
            sprintf( buf2, "%d", ch->hit[HIT_LOC_BODY] );
            i = buf2; break;
         case 'H' :
            sprintf( buf2, "%d", ch->max_hit );
            i = buf2; break;
         case 'i' :
	    if ( IS_AFFECTED( ch, AFF_INVISIBLE ) )
         	sprintf( buf2, "*" );
            i = buf2; break;
         case 'I' :
            if ( IS_IMMORTAL(ch) && IS_SET(ch->act, PLR_WIZINVIS) )
                sprintf( buf2, "-Invis-" );
	    else
		buf2[0] = '\0';
/*
		sprintf( buf2, "" );
*/
            i = buf2; break;
         case 'm' :
            sprintf( buf2, "%d", ch->mana );
            i = buf2; break;
         case 'M' :
            sprintf( buf2, "%d", ch->max_mana );
            i = buf2; break;
         case 'v' :
            sprintf( buf2, "%d", ch->move );
            i = buf2; break;
         case 'V' :
            sprintf( buf2, "%d", ch->max_move );
            i = buf2; break;
         case 'r' :
            if( ch->in_room != NULL )
               sprintf( buf2, "%s", 
		((!IS_NPC(ch) && IS_SET(ch->act,PLR_HOLYLIGHT)) ||
		 (!IS_AFFECTED(ch,AFF_BLIND) && !room_is_dark( ch->in_room, ch ) != VISION_NORMAL))
		? ch->in_room->name : "darkness");
            else
               sprintf( buf2, " " );
            i = buf2; break;
			
			
			/*  case 'r' :
            if( ch->in_room != NULL )
               sprintf( buf2, "%s", ch->in_room->name );
            else
               sprintf( buf2, " " );
            i = buf2; break; */
         case 'R' :
            if( IS_IMMORTAL( ch ) && ch->in_room != NULL )
               sprintf( buf2, "%d", ch->in_room->vnum );
            else
               sprintf( buf2, " " );
            i = buf2; break;
         case 'z' :
            if( IS_IMMORTAL( ch ) && ch->in_room != NULL )
               sprintf( buf2, "%s", ch->in_room->area->name );
            else
               sprintf( buf2, " " );
            i = buf2; break;
         case '%' :
            sprintf( buf2, "%%" );
            i = buf2; break;
      } 
      ++str;
      while( (*point = *i) != '\0' )
         ++point, ++i;      
   }
   *point = '\0';
   write_to_buffer( ch->desc, buf, point - buf );
   return;
}


/*
 * Append onto an output buffer.
 */
void write_to_buffer( DESCRIPTOR_DATA *d, const char *txt, int length )
{
    /*
     * Find length in case caller didn't.
     */
    if ( length <= 0 )
	length = strlen(txt);

    /*
     * Initial \n\r if needed.
     */
    if ( d->outtop == 0 && !d->fcommand )
    {
	d->outbuf[0]	= '\n';
	d->outbuf[1]	= '\r';
	d->outtop	= 2;
    }

    /*
     * Expand the buffer as needed.
     */
    while ( d->outtop + length >= d->outsize )
    {
	char *outbuf;

	outbuf      = alloc_mem( 2 * d->outsize );
	strncpy( outbuf, d->outbuf, d->outtop );
	free_mem( d->outbuf, d->outsize );
	d->outbuf   = outbuf;
	d->outsize *= 2;
    }

    /*
     * Copy.
     */
    strcpy( d->outbuf + d->outtop, txt );
    d->outtop += length;
    return;
}



/*
 * Lowest level output function.
 * Write a block of text to the file descriptor.
 * If this gives errors on very long blocks (like 'ofind all'),
 *   try lowering the max block size.
 */
bool write_to_descriptor( int desc, char *txt, int length )
{
    int iStart;
    int nWrite;
    int nBlock;

    if ( length <= 0 )
	length = strlen(txt);

    for ( iStart = 0; iStart < length; iStart += nWrite )
    {
	nBlock = UMIN( length - iStart, 4096 );
	if ( ( nWrite = write( desc, txt + iStart, nBlock ) ) < 0 )
	    { perror( "Write_to_descriptor" ); return FALSE; }
    } 

    return TRUE;
}



/*
 * Deal with sockets that haven't logged in yet.
 */
void nanny( DESCRIPTOR_DATA *d, char *argument )
{
    char buf[MAX_STRING_LENGTH];
//    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *ch;
    char *pwdnew;
    char *p;
/*    int iClass; */
    int iRace;
/*    int col, sn; */
    extern char * help_greeting;
    FILE *fp;
    char strsave[MAX_INPUT_LENGTH];
    extern int interp_doer;


    while ( isspace(*argument) )
	argument++;

    ch = d->character;

    switch ( d->connected )
    {

    default:
	bug( "Nanny: bad d->connected %d.", d->connected );
	close_socket( d );
	return;

    case CON_LOGIN_PASSWORD:
	write_to_buffer( d, "\n\r", 2 );

	if ( strcmp( crypt( argument, ch->pcdata->pwd ), ch->pcdata->pwd ) )
	{
	    write_to_buffer( d, "Wrong password.\n\r", 0 );
	    close_socket( d );
	    return;
	}

	write_to_buffer( d, echo_on_str, 0 );

	if ( check_reconnect( d, ch->name, TRUE ) )
	    return;

	if ( IS_SET( ch->act, PLR_REJECTED ) )
	{
	    sprintf( buf, "Your character was rejected because:\n\n\r%s\n\n",
		ch->pcdata->rejected );
	    write_to_buffer( d, buf, 0 );
	    sprintf( buf, "rm -f %s%s",
		REJECTED_DIR, capitalize( ch->name ) );
	    system( buf );
	    free_char( d->character );
	    d->character = NULL;
	    d->connected = CON_MAIN_MENU;
	    return;
	}

	if (IS_SET(ch->act, PLR_AFK))
	    REMOVE_BIT(ch->act, PLR_AFK);

	sprintf( log_buf, "%s@%s has connected.", ch->name, d->host );
	interp_doer = get_trust( ch );
	log_string( log_buf );

	sprintf( buf, "\n\rWelcome back, %s.\n\r", ch->name );
	write_to_buffer( d, buf, 0 );

	if ( IS_SET( ch->act, PLR_UNAPPROVED ) )
	write_to_buffer( d, "Note that your character has not yet been reviewed.\n\r", 0 );

	write_to_buffer( d, help_greeting, 0 );
	d->connected = CON_MAIN_MENU;
	break;

    case CON_READ_MOTD:
	write_to_buffer( d, 
    "\n\rWelcome to Darksun: Clash of the Sorceror Kings!\n\r", 0);
        
	    
	ch->next	= char_list;
	char_list	= ch;
	d->connected	= CON_PLAYING;

	if ( ch->level == 0 )
	{
	    ch->perm_stat_phys_str
	      = number_stat() + race_array[ch->race]->stat_phys_str_mod;
	    ch->perm_stat_phys_dex
	      = number_stat() + race_array[ch->race]->stat_phys_dex_mod;
	    ch->perm_stat_phys_tou
	      = number_stat() + race_array[ch->race]->stat_phys_tou_mod;
	    ch->perm_stat_phys_qui
	      = number_stat() + race_array[ch->race]->stat_phys_qui_mod;
	    ch->perm_stat_ment_int
	      = number_stat() + race_array[ch->race]->stat_ment_int_mod;
	    ch->perm_offense = 10;

	    ch->level	= 1;
	    ch->max_hit = race_array[ch->race]->hits_base
    + dice( race_array[ch->race]->hitnodice, race_array[ch->race]->hitsizedice );
	    ch->hit[HIT_LOC_BODY]	= ch->max_hit;
	    ch->max_mana = race_array[ch->race]->mana;
	    ch->mana	= ch->max_mana;
	    ch->max_move = race_array[ch->race]->move;
	    ch->move	= ch->max_move;
	    ch->vision_min = race_array[ch->race]->vision_min;
	    ch->vision_max = race_array[ch->race]->vision_max;
	    ch->vision_range = race_array[ch->race]->vision_range;
	    ch->prompt = NULL;

	    ch->height = race_array[ch->race]->height_base
                         + dice( race_array[ch->race]->heightnodice,
                                 race_array[ch->race]->heightsizedice );

	    ch->weight = race_array[ch->race]->weight_base
                         + dice( race_array[ch->race]->weightnodice,
                                 race_array[ch->race]->weightsizedice );

            /* for now, give all characters common */
            ch->pcdata->learned[gsn_common] = 100;

	    char_to_room( ch,
	      get_room_index( ROOM_VNUM_CHARACTER_GENERATION ), FALSE );
	}
	else if ( ch->in_room != NULL )
	{
	    char_to_room( ch, ch->in_room, FALSE );
	}
	else if ( IS_IMMORTAL(ch) )
	{
	    char_to_room( ch, get_room_index( ROOM_VNUM_CHAT ), FALSE );
	}
	else
	{
	    char_to_room( ch, get_room_index( ROOM_VNUM_TEMPLE ), FALSE );
	}

	save_char_obj( ch );

	restore_hunting( ch );

	act( "$n has entered the game.",
	  ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIXTH );
	do_look( ch, "auto" );
	sprintf( log_buf, "%s@%s has entered the game.", ch->name, d->host );
	log_string( log_buf );
	break;

    case CON_ANSI_ENABLE:
	if ( argument[0] == 'y' || argument[0] == 'Y' )
	    d->ansi_enable = TRUE;
	else
	    d->ansi_enable = FALSE;
	write_to_buffer( d, help_greeting, 0 );
	d->connected = CON_MAIN_MENU;
	break;

    case CON_MAIN_MENU:
	/*
	 * Connect character.
	 */
	if ( argument[0] == 'C' || argument[0] == 'c' )
	{
	    if ( d->character != NULL )
	    {
		free_char( d->character );
		d->character = NULL;
	    }
	    write_to_buffer( d, "\n\rEnter your character's name:  ", 0 );
	    d->connected = CON_LOGIN_GET_NAME;
	    return;
	}

	/*
	 * Enter game with current character.
	 */
	if ( argument[0] == 'E' || argument[0] == 'e' )
	{
	    if ( d->character == NULL || IS_NPC( d->character ) )
	    {
		write_to_buffer( d, "\n\rYou need to connect a character first.\n\r", 0 );
		write_to_buffer( d, help_greeting, 0 );
		d->connected = CON_MAIN_MENU;
		return;
	    }

	    if ( IS_SET( d->character->act, PLR_UNAPPROVED ) )
	    {
		write_to_buffer( d, "\n\rYour character has not yet been approved.\n\r", 0 );
		write_to_buffer( d, help_greeting, 0 );
		d->connected = CON_MAIN_MENU;
		return;
	    }

	    if ( IS_IMMORTAL(ch) )
		do_help( ch, "imotd" );
	    do_help( ch, "motd" );
	    d->connected = CON_READ_MOTD;
            
	    return;
	}

	/*
	 * Create new character.
	 */
	if ( argument[0] == 'R' || argument[0] == 'r' )
	{
	    if ( d->character != NULL )
	    {
		free_char( d->character );
		d->character = NULL;
	    }
	    write_to_buffer( d, "\n\rEnter new character name>  ", 0 );
	    d->connected = CON_CREATE_NEW_NAME;
	    return;
	}

	/*
	 * Exit the game.
	 */
	if ( argument[0] == 'X' || argument[0] == 'x' )
	{
	    write_to_buffer( d, "\n\rThank you for playing Darksun: Clash of the Sorceror Kings.\n\r", 0 );
	    if ( d->character != NULL )
	    {
		free_char( d->character );
		d->character = NULL;
	    }
	    close_socket( d );
	}
        
        

	write_to_buffer( d, help_greeting, 0 );
	break;

    case CON_LOGIN_GET_NAME:
	if ( argument[0] == '\0' )
	{
	    write_to_buffer( d, "\n\rNo name input.\n\r", 0 );
	    write_to_buffer( d, help_greeting, 0 );
	    d->connected = CON_MAIN_MENU;
	    return;
	}

	argument[0] = UPPER(argument[0]);
	if ( !check_parse_name( argument ) )
	{
	    write_to_buffer( d, "Illegal name, try another.\n\rName: ", 0 );
	    return;
	}

	if ( load_char_obj( d, argument ) == FALSE )
	{
	    write_to_buffer( d, "\n\rThat character does not exist.\n\r", 0 );
	    write_to_buffer( d, help_greeting, 0 );
	    free_char( d->character );
	    d->character = NULL;
	    d->connected = CON_MAIN_MENU;
	    return;
	}
	ch   = d->character;

	if ( IS_SET(ch->act, PLR_DENY) )
	{
	    sprintf( log_buf, "Denying access to %s@%s.", argument, d->host );
	    interp_doer = MAX_LEVEL - 3;
	    log_string( log_buf );
	    write_to_buffer( d, "You are denied access.\n\r", 0 );
	    close_socket( d );
	    return;
	}

        if (IS_SET(ch->act, PLR_DEAD) )
        {
            sprintf( log_buf, "Denying access to dead character, %s@%s.",argument, d->host );
            log_string ( log_buf );
            write_to_buffer( d, "That character is dead.\n\r", 0 );
            close_socket ( d );
            return;
        }

	if ( check_examine( argument ) )
	{
	    write_to_buffer( d, "Your character is currently being examined.\n\r", 0 );
	    free_char( d->character );
	    d->character = NULL;
	    d->connected = CON_MAIN_MENU;
	    return;
	}

	if ( !check_reconnect( d, argument, FALSE ) )
	{
	    if ( wizlock && !IS_IMMORTAL(ch) )
	    {
		write_to_buffer( d, "The game is wizlocked.\n\r", 0 );
		close_socket( d );
		return;
	    }
            if ( downtime && !IS_IMMORTAL(ch) )
            {
                write_to_buffer( d, "We are currently down for maintinence. The game will be back\n", 0 );
                write_to_buffer( d, "back up at approxamently 5 p.m. CST.\n\r", 0 );
	        close_socket( d );
                return;
            }
}

	write_to_buffer( d, "Password: ", 0 );
	write_to_buffer( d, echo_off_str, 0 );
	d->connected = CON_LOGIN_PASSWORD;
	return;
	break;

    case CON_CREATE_NEW_NAME:
	if ( argument[0] == '\0' )
	{
	    write_to_buffer( d, "\n\rNo name input.\n\r", 0 );
	    write_to_buffer( d, help_greeting, 0 );
	    d->connected = CON_MAIN_MENU;
	    return;
	}

	argument[0] = UPPER(argument[0]);
	if ( !check_parse_name( argument ) )
	{
	    write_to_buffer( d, "\n\rIllegal name.\n\r", 0 );
	    write_to_buffer( d, help_greeting, 0 );
	    d->connected = CON_MAIN_MENU;
	    return;
	}

	fclose( fpReserve );
	sprintf( strsave, "%s%s", PLAYER_DIR, capitalize( argument ) );
	if ( ( fp = fopen( strsave, "r" ) ) != NULL )
	{
	    write_to_buffer( d, "\n\rThat character already exists.\n\r", 0 );
	    write_to_buffer( d, help_greeting, 0 );
	    d->connected = CON_MAIN_MENU;
	    fclose( fp );
	    fpReserve = fopen( NULL_FILE, "r" );
	    return;
	}

	sprintf( strsave, "%s%s", UNAPPROVED_DIR, capitalize( argument ) );
	if ( ( fp = fopen( strsave, "r" ) ) != NULL )
	{
	    write_to_buffer( d, "\n\rThat character is being reviewed.\n\r", 0 );
	    write_to_buffer( d, help_greeting, 0 );
	    d->connected = CON_MAIN_MENU;
	    fclose( fp );
	    fpReserve = fopen( NULL_FILE, "r" );
	    return;
	}
	fpReserve = fopen( NULL_FILE, "r" );

	load_char_obj( d, argument );
	SET_BIT( d->character->act, PLR_UNAPPROVED );
      //  SET_BIT( d->character->act, PLR_HOLYLIGHT );
	write_to_buffer( d, "\n\rEnter password:  ", 0 );
	write_to_buffer( d, echo_off_str, 0 );
	d->connected = CON_CREATE_NEW_PASSWORD;
	break;

	case CON_CREATE_NEW_PASSWORD:
	#if defined(unix)
	    write_to_buffer( d, "\n\r", 2 );
	#endif

	if ( strlen(argument) < 5 )
	{
	    write_to_buffer( d,
		"Password must be at least five characters long.\n\rPassword: ",
		0 );
	    return;
	}

	pwdnew = crypt( argument, ch->name );
	for ( p = pwdnew; *p != '\0'; p++ )
	{
	    if ( *p == '~' )
	    {
		write_to_buffer( d,
		    "New password not acceptable, try again.\n\rPassword: ",
		    0 );
		return;
	    }
	}

	free_string( ch->pcdata->pwd, MEM_PCDATA );
	ch->pcdata->pwd	= str_dup( pwdnew );
	write_to_buffer( d, "Please retype password: ", 0 );
	d->connected = CON_CREATE_CONFIRM_PASSWORD;
	break;

	case CON_CREATE_CONFIRM_PASSWORD:
	#if defined(unix)
	    write_to_buffer( d, "\n\r", 2 );
	#endif

	if ( strcmp( crypt( argument, ch->pcdata->pwd ), ch->pcdata->pwd ) )
	{
	    write_to_buffer( d, "Passwords don't match.\n\rRetype password: ",	0 );
	    d->connected = CON_CREATE_NEW_PASSWORD;
	    return;
	}

	write_to_buffer( d, echo_on_str, 0 );
	write_to_buffer( d, "What is your sex (M/F)? ", 0 );
	d->connected = CON_CREATE_NEW_SEX;
	break;

	case CON_CREATE_NEW_SEX:
	switch ( argument[0] )
	{
	case 'm': case 'M': ch->sex = SEX_MALE;    break;
	case 'f': case 'F': ch->sex = SEX_FEMALE;  break;
	default:
	    write_to_buffer( d, "Invalid gender.\n\rWhat is your character's gender? ", 0 );
	    return;
	}

	write_to_buffer( d, "Select a race:  human, elf, dwarf > ", 0 );
	d->connected = CON_CREATE_NEW_RACE;
	break;

    case CON_CREATE_NEW_RACE:
	    /* KLUDGE */
	for ( iRace = 1; iRace <= 3; iRace++ )
	{
	    if ( !str_cmp( argument, race_array[iRace]->race_name ) )
	    {
		ch->race = iRace;
		break;
	    }
	}

	if ( iRace > 3 )
	{
	    write_to_buffer( d, "One of the following only:\n\r"
				"  human, elf, dwarf > ", 0 );
	    break;
	}


/*
	col = 1;
	sprintf( buf, "Classes:\n\r" );
	for ( iClass = 0; iClass < MAX_CLASS; iClass++ )
	{
	    if ( race_array[ch->race]->classes[iClass] == TRUE )
	    {
		strcat( buf, class_table[iClass].spaces );
		strcat( buf, class_table[iClass].class_name );
		col++;
		if ( col == 5)
		{
		    strcat( buf, "\n\r" );
		    col = 1;
		}
	    }
	}
	if ( col != 1 )
	    strcat( buf, "\n\r" );
	strcat( buf, "Choose a class > " );
	write_to_buffer( d, buf, 0 );
	d->connected = CON_CREATE_NEW_CLASS;
	break;

    case CON_CREATE_NEW_CLASS:
	for ( iClass = 0; iClass < MAX_CLASS; iClass++ )
	{
	    if ( !str_cmp( argument, class_table[iClass].class_name ) )
	    {
		ch->class = iClass;
		break;
	    }
	}

	if ( iClass == MAX_CLASS || race_array[ch->race]->classes[iClass] == FALSE )
	{
	    write_to_buffer( d,
		"Invalid class.\n\rChoose your class: ", 0 );
	    return;
	}
*/


	do_help( ch, "long_desc" );
	write_to_buffer( d, "\n\rEnter long description on line below:\n\r>", 0 );
	d->connected = CON_CREATE_LONG_DESC;
	break;

    case CON_CREATE_LONG_DESC:
        if ( argument[0] == '\0' )
        {
            write_to_buffer( d, "\n\rPlease try again.\n\r> ", 0);
            d->connected = CON_CREATE_LONG_DESC;
            return;
        }
	strcat( argument, "\n\r" );
	ch->long_descr = str_dup( argument );

	do_help( ch, "short_desc" );
	write_to_buffer( d, "\n\rEnter short description on line below:\n\r>", 0 );
	d->connected = CON_CREATE_SHORT_DESC;
	break;

    case CON_CREATE_SHORT_DESC:
        if ( argument[0] == '\0' )
        {
            do_help( ch, "short_desc" );
            write_to_buffer( d, "\n\rEnter short description on line below:\n\r> ", 0);
            d->connected = CON_CREATE_SHORT_DESC;
            return;
        }
	ch->short_descr = str_dup( argument );

	do_help( ch, "namelist" );
	write_to_buffer( d, "\n\rEnter your namelist on the line below:\n\r>", 0 );
	d->connected = CON_CREATE_NAMELIST;
	break;

    case CON_CREATE_NAMELIST:
        if ( argument[0] == '\0' )
        {
            do_help( ch, "namelist" );
            write_to_buffer( d, "\n\rEnter your namelist on the line below:\n\r> ", 0);
            d->connected = CON_CREATE_NAMELIST;
            return;
        }

	ch->namelist = str_dup( argument );

	do_help( ch, "description" );
	write_to_buffer( d, "\n\rEnter your description below.  End with a ~\n\r>", 0 );
	d->connected = CON_CREATE_DESCRIPTION;
	break;

    case CON_CREATE_DESCRIPTION:
	if ( argument[0] == '~' )
	{
	    do_help( ch, "email" );
	    write_to_buffer( d, "\n\rEnter your email address on the line below:\n\r>", 0 );
	    d->connected = CON_CREATE_EMAIL;
	    return;
	}
	else if ( argument[0] == '\0' )
        {
            write_to_buffer( d, "\n\rYou must write a description.\n\r> ", 0  );
            d->connected = CON_CREATE_DESCRIPTION;
        }
        else
	{
	    strcpy( buf, ch->description );
	    if ( buf[0] == '\0' )
		strcpy( buf, argument );
	    else
		strcat( buf, argument );
	    strcat( buf, "\n\r" );
	    free_string( ch->description, MEM_PCDATA );
	    ch->description = str_dup( buf );

	    if ( strlen( buf ) > 900 )
	    {
		write_to_buffer( d, "\n\rMaximum description length exceeded.\n\r", 0 );
		do_help( ch, "email" );
		write_to_buffer( d, "\n\rEnter your email address on the line below:\n\r>", 0 );
		d->connected = CON_CREATE_EMAIL;
		return;
	    }
	    else
	    {
		write_to_buffer( d, ">", 1 );
		return;
	    }
	}
	break;

    case CON_CREATE_EMAIL:
       
        if ( argument[0] == '\0' )
        {
            write_to_buffer( d, "You must supply an email address.\n\r
            What is your Email Address? ", 0 );
            d->connected = CON_CREATE_EMAIL;
            return;
        }

	ch->pcdata->email_address = str_dup( argument );

	ch->pcdata->rejected = str_dup( "" );
	SET_BIT( ch->act, PLR_UNAPPROVED );
        SET_BIT( ch->act, PLR_HOLYLIGHT );
	save_char_obj( ch );

	write_to_buffer( d, "\n\n\rYour character has been saved, and should be approved or disapproved\n\rin less than a day or two.\n\n\r", 0 );

        sprintf( log_buf, "%s@%s new player application.",
	  ch->name, d->host );
        interp_doer = MAX_LEVEL - 3;
	log_string( log_buf );

	write_to_buffer( d, help_greeting, 0 );
	free_char( ch );
	d->character = NULL;
	d->connected = CON_MAIN_MENU;
	break;
    }

    return;
}



/*
 * Parse a name for acceptability.
 */
bool check_parse_name( char *name )
{
    /*
     * Reserved words.
     */
    if ( is_name( name, "all auto immortal me self someone" ) )
	return FALSE;

    /*
     * Length restrictions.
     */
    if ( strlen(name) <  3 )
	return FALSE;

    if ( strlen(name) > MAX_NAME_LENGTH )
	return FALSE;

    /*
     * Alphanumerics only.
     * Lock out IllIll twits.
     */
    {
	char *pc;
	bool fIll;

	fIll = TRUE;
	for ( pc = name; *pc != '\0'; pc++ )
	{
	    if ( !isalpha(*pc) )
		return FALSE;
	    if ( LOWER(*pc) != 'i' && LOWER(*pc) != 'l' )
		fIll = FALSE;
	}

	if ( fIll )
	    return FALSE;
    }

    /*
     * Prevent players from naming themselves after mobs.
     */
    {
	extern MOB_INDEX_DATA *mob_index_hash[MAX_KEY_HASH];
	MOB_INDEX_DATA *pMobIndex;
	int iHash;

	for ( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
	{
	    for ( pMobIndex  = mob_index_hash[iHash];
		  pMobIndex != NULL;
		  pMobIndex  = pMobIndex->next )
	    {
		if ( is_name( name, pMobIndex->name ) )
		    return FALSE;
	    }
	}
    }

    return TRUE;
}


/*
 * Make sure the character isn't loaded for modification.
 */
bool check_examine( char *name )
{
    CHAR_DATA *ch;

    for ( ch = char_list; ch != NULL; ch = ch->next )
    {
	if ( !IS_NPC(ch)
	&& ( ch->name && !str_cmp( ch->name, name ) )
	&& IS_SET( ch->act, PLR_REVIEWING ) )
	    return TRUE;
    }

    return FALSE;
}

/*
 * Look for link-dead player to reconnect.
 */
bool check_reconnect( DESCRIPTOR_DATA *d, char *name, bool fConn )
{
    CHAR_DATA *ch;
    DESCRIPTOR_DATA *dclose;

    for ( ch = char_list; ch != NULL; ch = ch->next )
    {
	if ( !IS_NPC(ch)
	&& ( !fConn || ch->desc == NULL )
	&& !str_cmp( d->character->name, ch->name ) )
	{
	    if ( fConn == FALSE )
	    {
		free_string( d->character->pcdata->pwd, MEM_PCDATA );
		d->character->pcdata->pwd = str_dup( ch->pcdata->pwd );
	    }
	    else
	    {
		if ( ch->switched_into != NULL )
		{
		    do_return( ch->switched_into, "" );
		    reconnect( d, ch );
		}
		else
		    reconnect( d, ch );
	    }
	    return TRUE;
	}
    }

    for ( dclose = descriptor_list; dclose != NULL; dclose = dclose->next )
    {
        if ( dclose == d )
            continue;

        if ( d->character == NULL || dclose->character == NULL
        || str_cmp( d->character->name, dclose->character->name ) )
            continue;

        if ( dclose->connected == CON_PLAYING )
	{
            reconnect( d, dclose->character );
	    return TRUE;
	}

        break;
    }

    if ( dclose == NULL )
        return FALSE;

/* free the original desc */
    free_char( dclose->character );
    free_descriptor( dclose );

    return FALSE;
}


void free_descriptor( DESCRIPTOR_DATA *dclose )
{
    if ( dclose == descriptor_list )
    {
        descriptor_list = descriptor_list->next;
    }
    else
    {
        DESCRIPTOR_DATA *d2;

        for ( d2 = descriptor_list; d2 && d2->next != dclose; d2 = d2->next )
            ;
        if ( d2 != NULL )
            d2->next = dclose->next;
        else
            bug( "Free descriptor: Close_socket: dclose not found.", 0 );
    }

    close( dclose->descriptor );
    free_string( dclose->host, MEM_GENERAL );
    dclose->next        = descriptor_free;
    descriptor_free     = dclose;

    return;
}


void reconnect( DESCRIPTOR_DATA *d, CHAR_DATA *ch )
{
    OBJ_DATA *obj;
    char log_buf[MAX_STRING_LENGTH];
    extern int interp_doer;


    if ( ch->desc != NULL )
	free_descriptor( ch->desc );

/* really cheesy problem here, cutting link and reconnecting
   will screw up room light modifier.  so, we will blast all
   equipment on the duplicate character before freeing */
    for ( obj = d->character->carrying; obj != NULL; obj = obj->next_content )
    {
	if ( obj->wear_loc != gn_wear_none )
	{
	    obj->value[0] = 0;
	    obj->extra_flags = 0;
	}
    }
    free_char( d->character );
    d->character = ch;
    ch->desc         = d;
    ch->timer        = 0;
    send_to_char( "Reconnecting.\n\r", ch );
    act( "$n has reconnected.", ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIXTH );
    sprintf( log_buf, "%s@%s reconnected.", ch->name, d->host );
    interp_doer = get_trust( ch );
    log_string( log_buf );
    d->connected = CON_PLAYING;

    if (IS_SET(ch->act, PLR_AFK)) 
	REMOVE_BIT(ch->act, PLR_AFK);

    return;
}


void stop_idling( CHAR_DATA *ch )
{
    if ( ch == NULL
    ||   ch->desc == NULL
    ||   ch->desc->connected != CON_PLAYING
    ||   ch->was_in_room == NULL 
    ||   ch->in_room != get_room_index( ROOM_VNUM_LIMBO ) )
	return;

    ch->timer = 0;
    char_from_room( ch );
    char_to_room( ch, ch->was_in_room, FALSE );
    ch->was_in_room	= NULL;
    act( "$n has returned from the void.", ch, NULL, NULL, NULL, NULL, TO_ROOM, SENSE_SIXTH );
    return;
}



/*
 * Write to one char.
 */
void send_to_char( const char *txt, CHAR_DATA *ch )
{
    if ( txt == NULL || ch->desc == NULL )
	return;
    if ( ch->desc->showstr_point )
    {
	write_to_buffer( ch->desc, txt, strlen( txt ) );
	return;
    }
    ch->desc->showstr_head = alloc_mem( strlen( txt ) + 1 );
    strcpy( ch->desc->showstr_head, txt );
    ch->desc->showstr_point = ch->desc->showstr_head;
    show_string( ch->desc, "" );
    return;
}


void show_string(struct descriptor_data *d, char *input)
{
  char buffer[ MAX_STRING_LENGTH ];
  char buf[ MAX_INPUT_LENGTH ];
  register char *scan, *chk;
  int lines = 0, toggle=1;
  int pagelen;

  one_argument(input, buf);

  if ( d->original )
    pagelen = d->original->pcdata->pagelen;
  else
    pagelen = d->character->pcdata->pagelen;

  switch( UPPER( buf[0] ) )
  {
  case '\0':
  case 'C': /* show next page of text */
    lines = 0;
    break;

  case 'R': /* refresh current page of text */
    lines = - 1 - (pagelen);
    break;

  case 'B': /* scroll back a page of text */
    lines = -(2*pagelen);
    break;

  case 'H': /* Show some help */
    write_to_buffer( d,
        "C, or Return = continue, R = redraw this page,\n\r", 0 );
    write_to_buffer( d,
        "B = back one page, H = this help, Q or other keys = exit.\n\r\n\r",
		    0 );
    lines = - 1 - pagelen;
    break;

  default: /*otherwise, stop the text viewing */
    if ( d->showstr_head )
    {
      free_string( d->showstr_head, MEM_GENERAL );
      d->showstr_head  = 0;
    }
    d->showstr_point = 0;
    return;

  }

  /* do any backing up necessary */
  if (lines < 0)
  {
    for ( scan = d->showstr_point; scan > d->showstr_head; scan-- )
         if ( ( *scan == '\n' ) || ( *scan == '\r' ) )
	 {
	     toggle = -toggle;
	     if ( toggle < 0 )
	         if ( !( ++lines ) )
		     break;
	 }
    d->showstr_point = scan;
  }

  /* show a chunk */
  lines  = 0;
  toggle = 1;
  for ( scan = buffer; ; scan++, d->showstr_point++ )
       if ( ( ( *scan = *d->showstr_point ) == '\n' || *scan == '\r' )
	   && ( toggle = -toggle ) < 0 )
	   lines++;
       else
	   if ( !*scan || ( d->character && !IS_NPC( d->character )
			  && lines >= pagelen ) )
	   {

	       *scan = '\0';
	       write_to_buffer( d, buffer, strlen( buffer ) );

	     /* See if this is the end (or near the end) of the string */
	       for ( chk = d->showstr_point; isspace( *chk ); chk++ );
	       if ( !*chk )
	       {
		   if ( d->showstr_head )
		   {
		      free_string( d->showstr_head, MEM_GENERAL );
		      d->showstr_head  = 0;
		   }
		   d->showstr_point = 0;
	       }
	       return;
	   }

  return;
}

/*
 * The primary output interface for formatted output.
 */
void act( const char *format, CHAR_DATA *ch, CHAR_DATA *vch, OBJ_DATA *obj, const void *arg1, const void *arg2, int type, int sense )
{
    static char * const he_she	[] = { "it",  "he",  "she" };
    static char * const him_her	[] = { "it",  "him", "her" };
    static char * const his_her	[] = { "its", "his", "her" };

    char buf[MAX_STRING_LENGTH];
    char fname[MAX_INPUT_LENGTH];
    CHAR_DATA *to;
    OBJ_DATA *obj2 = (OBJ_DATA  *) arg1;
    const char *str;
    const char *i;
    char *point;

    /*
     * Discard null and zero-length messages.
     */
    if ( format == NULL || format[0] == '\0' )
	return;

    to = ch->in_room->people;
    if ( type == TO_VICT )
    {
	if ( vch == NULL )
	{
	    bug( "Act: null vch with TO_VICT.", 0 );
	    return;
	}
	to = vch->in_room->people;
    }
    
    for ( ; to != NULL; to = to->next_in_room )
    {
	if ( to->desc == NULL || !IS_AWAKE(to) )
	    continue;

	if ( type == TO_CHAR && to != ch )
	    continue;
	if ( type == TO_VICT && ( to != vch || to == ch ) )
	    continue;
	if ( type == TO_ROOM && to == ch )
	    continue;
	if ( type == TO_NOTVICT && (to == ch || to == vch) )
	    continue;
	if ( sense == SENSE_SIGHT && (!can_see(to, ch)))
	     continue;
/* XOR deprived senses with act sense */

	point	= buf;
	str	= format;
	while ( *str != '\0' )
	{
	    if ( *str != '$' )
	    {
		*point++ = *str++;
		continue;
	    }
	    ++str;

	    if ( strchr( "nems", *str ) && ch == NULL )
	    {
		bug( "Act: missing ch for code %d.", *str );
		i = " a character ";
	    }
	    else if ( strchr( "NEMS", *str ) && vch == NULL )
	    {
		bug( "Act: missing vch for code %d.", *str );
		i = " a character ";
	    }
	    else if ( strchr( "Pdt", *str ) && arg1 == NULL )
	    {
		bug( "Act: missing arg1 for code %d.", *str );
		i = " <@@@> ";
	    }
	    else if ( 'T' == *str && arg2 == NULL )
	    {
		bug( "Act: missing arg2 for code %d.", *str );
		i = " <@@@> ";
	    }
	    else if ( 'p' == *str && obj == NULL )
	    {
		bug( "Act: missing obj for code %d.", *str );
		i = " an object ";
	    }
	    else
	    {
		switch ( *str )
		{
		default:  bug( "Act: bad code %d.", *str );
			  i = " <@@@> ";				break;
		case 'B':
		    if ( to->desc != NULL && to->desc->ansi_enable == TRUE )
			    i = TEXT_BOLD_ON;
		    else
		    {
			++str;
			continue;
		    }
									break;
		case 'b':
		    if ( to->desc != NULL && to->desc->ansi_enable == TRUE )
			i = TEXT_NORMAL;
		    else
		    {
			++str;
			continue;
		    }
									break;
		case 't': i = (char *) arg1;				break;
		case 'T': i = (char *) arg2;          			break;
		case 'n': i = person( to,  ch  );			break;
		case 'N': i = person( to, vch  );			break;
		case 'e': i = he_she  [URANGE(0, ch  ->sex, 2)];	break;
		case 'E': i = he_she  [URANGE(0, vch ->sex, 2)];	break;
		case 'm': i = him_her [URANGE(0, ch  ->sex, 2)];	break;
		case 'M': i = him_her [URANGE(0, vch ->sex, 2)];	break;
		case 's': i = his_her [URANGE(0, ch  ->sex, 2)];	break;
		case 'S': i = his_her [URANGE(0, vch ->sex, 2)];	break;

		case 'p':
		    i = can_see_obj( to, obj )
			    ? obj->short_descr
			    : "something";
		    break;

		case 'P':
		    i = can_see_obj( to, obj2 )
			    ? obj2->short_descr
			    : "something";
		    break;

		case 'd':
		    if ( arg1 == NULL || ((char *) arg1)[0] == '\0' )
		    {
			i = "door";
		    }
		    else
		    {
			one_argument( (char *) arg1, fname );
			i = fname;
		    }
		    break;
		}
	    }
		
	    ++str;
	    while ( ( *point = *i ) != '\0' )
		++point, ++i;
	}

	*point++ = '\n';
	*point++ = '\r';
	buf[0]   = UPPER(buf[0]);
	write_to_buffer( to->desc, buf, point - buf );
    }

    return;
}
