/*
 *	MC-Code Linker
 *
 *	(c) Andreas Borchert, 1983
 */

/*
 *	Output Format :
 *
 *	Loadfile =	[ BaseFrame ] { Module } .
 *	Module =	ModuleHeader DataFrame CodeFrame .
 *	ModuleHeader =	MODULE frameSize ModuleNumber .
 *	BaseFrame =	BASE frameSize { ModuleName } .
 *	DataFrame =	DATATEXT frameSize { dataWord } .
 *	CodeFrame =	CODETEXT frameSize { codeWord } .
 *	dataWord =	number .
 *	codeWord =	number .
 *	ModuleNumber =	number .
 *
 *	(ein Wort = 2 Bytes)
 */

#include	<stdio.h>
#include	"mcl.h"

#define	NEXT_ARG	{ ++argv;--argc;if (argc < 0) quit(usage,mcl_name);}

extern	int	mod_cnt;
extern	int	no_output;

char	usage[] = { "Usage: %s [-o outfile] [-b base] [-l] main [ objects ... ]" };
char	*mcl_name;
char	out_file[15];
int	bflag = FALSE;
int	lflag = FALSE;

main ( argc , argv )
int	argc;
char	**argv;
{
	HASHTAB	**modules,**m_ptr;

	mcl_name = rindex(*argv,'/');
	if ( mcl_name )
		++mcl_name;
	else
		mcl_name = *argv;
	strncpy ( out_file , mcl_name , 10 );
	out_file[10] = '\0';
	strncat ( out_file , ".out" , 14 );
	out_file[14] = '\0';

	mod_cnt = 1;

	while ( --argc && **++argv == '-' ) switch ( argv[0][1] ) {
	case 'o' :
		NEXT_ARG;
		strncpy ( out_file , *argv , 14 );
		break;
	case 'b' :
		if ( bflag )
			quit(usage,mcl_name);
		++bflag;
		NEXT_ARG;
		enter ( *argv , BASE );
		break;
	case 'l' :
		++lflag;
		break;
	default :
		quit(usage,mcl_name);
	}

	if ( argc <= 0 )
		quit(usage,mcl_name);
	initoutcode();
	if ( !bflag ) {
		writeword(xBASE);
		writeword( (argc) * 8 );	/* frameSize */
		}
	CALLOC(modules,argc+1,sizeof(HASHTAB *));
	m_ptr = modules;
	*m_ptr = enter(*argv,LINK);	/* main module */
	if ( !bflag )
		writename((*m_ptr)->ht_keyword);
	while ( --argc ) {
		*++m_ptr = enter(*++argv,LINK);
		if ( !bflag )
			writename((*m_ptr)->ht_keyword);
		}
	*++m_ptr = NULL;
	for ( m_ptr = modules ; *m_ptr ; ++m_ptr )
		link(*m_ptr);
	if ( no_output )
		exit(1);
	else
		exit(0);
}
