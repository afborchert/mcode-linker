/*
 *	MC-Code Linker
 *
 *	(c) Andreas Borchert, 1983
 */

#include	<stdio.h>
#include	"mcl.h"

extern	HASHTAB	*main;
extern	int	mod_cnt;
extern	unsigned	int	sy;
extern	int	fs;
extern	int	lflag;

enter ( filename , type )
char	*filename;
int	type;
{
	char	mn[MNLEN+1];
	HASHTAB	*ptr;
	FILE	*fp;

	if ( (fp = fopen(filename,"r")) == NULL )
		pquit(filename);
	initincode(fp);
	if ( type == BASE ) {
		getsy();
		if ( sy != xBASE ) quit("%s: No base format.",filename);
		while ( fs ) {
			get_modulename ( mn );
			ptr = match ( mn );
			if ( ptr->ht_module )
				quit("%s: Multiply defined.",mn);
			ptr->ht_module = mod_cnt++;
			ptr->ht_file = NULL;
			}
		}
	else {
		getsy();
		if ( sy == CODEKEYSY ) {
			skip();
			getsy();
			}
		if ( sy == MODULESY ) {
			get_modulename ( mn );
			ptr = match ( mn );
			if ( ptr->ht_module )
				quit("%s: Multiply defined.",mn);
			ptr->ht_module = mod_cnt++;
			if ( lflag )
				printf("%3o %s\n",ptr->ht_module,ptr->ht_keyword);
			ptr->ht_file = filename;
			}
		else
			quit("%s: Bad format.",filename);
		}
	fclose(fp);
	return ( ptr );
}

get_modulename ( mn )
char	*mn;
{
	int	c,n;
	char	ch;
	int	oddpos;

	oddpos = FALSE;
	for ( c = 0 ; c < MNLEN ; ++c ) {
		if ( oddpos ) {
			ch = n % 0400;
			oddpos = FALSE;
			}
		else {
			getnum ( &n );
			ch = n / 0400;
			oddpos = TRUE;
			}
		if ( ch == ' ' )
			ch = '\0';
		mn[c] = ch;
		}
	mn[MNLEN] = '\0';
}
