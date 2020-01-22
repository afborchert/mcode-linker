/*
 *	MC-Code Linker
 *
 *	(c) Andreas Borchert, 1983
 */

#include	<stdio.h>
#include	"mcl.h"

extern	char	*out_name;

extern	unsigned int	sy;
extern	int		fs;

extern	int	no_output;
extern	int	lflag;
extern	FILE	*out;
extern	FILE	*inout;

int	mod_cnt;

#define	MAXIMP	30
static	HASHTAB	*imports[MAXIMP];

static	long	Fixup;		/* hier ist die FrameSize des CodeFrames */
static	int	FrameSize;
static	long	DataFrame;	/* G --> */
static	long	CodeFrame;	/* F --> */
static	int	probablyprocnum;
static	int	procnum = -1;
static	int	offset;

link ( module )
HASHTAB	*module;
{
	int	index;
	char	mn[MNLEN+1];
	int	n;
	FILE	*fp;
	unsigned int datasize;

	if ( (fp = fopen(module->ht_file,"r")) == NULL )
		pquit(module->ht_file);
	initincode(fp);
	getsy();
	if ( sy == CODEKEYSY ) {
		skip();
		getsy();
		}
	/* module header */
	get_modulename ( mn );	/* dummy Aufruf */
	if ( lflag )
		printf("\nModule %s :\nNAME             REL ABS\n",mn);
	getnum ( &n ); getnum ( &n ); getnum ( &n );	/* module key */
	getnum ( &datasize );
	/* import list */
	getsy();
	imports[0] = module;
	if ( sy == IMPORTSY ) {
		index = 1;
		while ( fs >= 11 ) {
			if ( index >= MAXIMP )
				quit("Overflow of import list");
			get_modulename(mn);
			getnum ( &n ); getnum ( &n ); getnum ( &n );
			imports[index] = match(mn);
			if ( imports[index]->ht_module == 0 ) {
				fprintf(stderr,"%s: undefined.\n",mn);
				imports[index]->ht_module = -1;
				no_output = TRUE;
				}
			else if ( lflag )
				printf("%-16s %3o %3o\n",mn,index,imports[index]->ht_module);
			++index;
			}
		getsy();
		}
	writeword ( xMODULE );
	writeword ( 1 );
	writeword ( module->ht_module );
	writeword ( xDATATEXT );
	writeword ( datasize );
	DataFrame = ftell(out);
	for ( index = 0 ; index < datasize ; ++index )
		writeword ( 0 );
	writeword ( xCODETEXT );
	Fixup = ftell(out);
	writeword ( 0 );	/* frameSize bis jetzt noch unbekannt */
	FrameSize = 0;
	CodeFrame = ftell(out);
	probablyprocnum = TRUE;
	while ( sy == CTEXT || sy == DTEXT ) {
		if ( sy == CTEXT ) {
			loadcode();
			probablyprocnum = !probablyprocnum;
			}
		else
			loaddata();
		}
	fseek ( out , Fixup , 0 );
	writeword ( FrameSize );
	fseek ( out , FrameSize * 2 , 1 );
	if ( sy != EOFSY )
		fprintf(stderr,"%s: only one module per object allowed !",module->ht_file);

	fclose(fp);
}

loadcode ()
{
	unsigned int	n;		/* aux var */
	unsigned int	floc;		/* fixup location */
	unsigned int	log_nr;		/* logische Modulenumber */
	unsigned int	act_nr;		/* tatsaechliche Modulenumber */

	if ( probablyprocnum && fs == 2 ) {
		getnum ( &offset );
		procnum = offset;
		getnum ( &offset );
		fseek ( out , CodeFrame + procnum*2 , 0 );
		writeword ( offset );
		FrameSize = offset/2 > FrameSize ? offset/2 : FrameSize;
		getsy();
		}
	else {
		getnum ( &n );	/* dummy Aufruf */
		fseek ( out , CodeFrame + offset , 0 );
		while ( fs ) {
			getnum ( &n );
			writeword ( n );
			++FrameSize;
			}
		fflush ( out );
		getsy();
		if ( sy == FIXUP ) {
			while ( fs ) {
				getnum ( &floc );
				if ( fseek ( inout , CodeFrame + floc , 0 ) )
					perror("fseek ( inout )");
				log_nr = getc ( inout );
				act_nr = imports[log_nr]->ht_module;
				fseek ( out , CodeFrame + floc , 0 );
				putc ( act_nr , out );
				}
			getsy();
			}
		}
}

loaddata ()
{
	int	offset,n;

	getnum ( &offset );
	fseek ( out , DataFrame + offset*2 , 0 );
	while ( fs ) {
		getnum ( &n );
		writeword ( n );
		}
	getsy ();
}
