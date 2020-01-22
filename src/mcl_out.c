/*
 *	MC-Code Linker
 *
 *	(c) Andreas Borchert, 1983
 */

#include	<stdio.h>
#include	"mcl.h"

extern	char	out_file[];

FILE	*out;
FILE	*inout;
int	no_output = FALSE;

writeword ( i )
int	i;
{
	word	w;

	w = (word) i;
	fwrite ( &w , sizeof(word) , 1 , out );
}

initoutcode ()
{
	close(creat(out_file,0600));
	if ( (out = fopen(out_file,"w")) == NULL )
		pquit(out_file);
	if ( (inout = fopen(out_file,"r")) == NULL )
		pquit(out_file);
}

writename ( name )
char	*name;
{
	fwrite ( name , sizeof(char) , MNLEN , out );
}
