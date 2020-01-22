/* (c) Andreas Borchert 1983, University of Ulm */

#include	<stdio.h>
#include	<ctype.h>
#include	"mcl.h"

#define	HASHCONST	271

static	HASHTAB	*table[HASHCONST];

/*
 * die erste aufzurufende Routine, die die Hashtabelle anlegt
 */

match_init ()
{
	int	index;
	int	hashindex;

	for ( index = 0 ; index < HASHCONST ; ++index )
		table[index] = NULL;
}

/*
 * Einfuegung eines einzelnen Schluesselwortes bei bekanntem
 * Index der Hashtabelle
 */

static
insert ( hashindex , keyword )
int	hashindex;
char	*keyword;
{

	HASHTAB	*ptr;


	ptr = table[hashindex];
	if ( (table[hashindex] = (HASHTAB *) calloc ( (unsigned) 1 , sizeof(HASHTAB) )) == NULL ) {
		perror("ebnf_hash.c");
		exit(1);
		}
	table[hashindex]->ht_next = ptr;
	table[hashindex]->ht_keyword = keyword;
}

/*
 * Bestimmung des Index der Hashtabelle eines bestimmten Wortes.
 * Die Schluesselwoerter sollten ziemlich breit auf der Tabelle
 * verteilt sein fuer eine effiziente Suche.
 */

static	int
hash ( word )
char	*word;
{
	int	length;

	length = strlen(word);
	return ( (word[0]<<1+word[length-1]>>2+length) % HASHCONST );
}

/*
 * match() liefert, falls `word' in der Tabelle eingetragen worden ist
 * sein Symbol, ansonsten 0
 */

int
match ( word )
char	*word;
{
	int	hashindex;
	HASHTAB	*ptr;
	char	*buf;

	hashindex = hash(word);
	for ( ptr = table[hashindex] ; ptr != NULL ; ptr = ptr->ht_next )
		if ( strcmp(ptr->ht_keyword,word) == 0 )
			return(ptr);
	if ( (buf = calloc((unsigned) strlen(word)+1,sizeof(char))) == NULL )
		quit("No space available");
	strcpy ( buf , word );
	insert ( hashindex , buf , 0 );
	return ( table[hashindex] );
}
