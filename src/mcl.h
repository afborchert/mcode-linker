/*
 *	MC-Code Linker
 *
 *	(c) Andreas Borchert, 1983
 */

#define	TRUE	1
#define	FALSE	0

typedef	unsigned short word;

/* Typen der Inputfiles */

#define	BASE	2		/* loadfile */
#define	LINK	3		/* objectfile */

/* FrameTypes bei loadfiles */

#define	xBASE  	  0300
#define	xDATATEXT 0301
#define	xCODETEXT 0302
#define	xMODULE	  0303

/* FrameTypes bei objectfiles */

#define	EOFSY		0
#define	CODEKEYSY	0200
#define	MODULESY	0201
#define	IMPORTSY	0202
#define	CTEXT		0203
#define	DTEXT		0204
#define	FIXUP		0205

/* Struktur der Hashtabelle */

typedef	struct	hashtab	{

	char		*ht_keyword;
	char		*ht_file;
	int		ht_module;
	struct	hashtab	*ht_next;

	} HASHTAB;

#define	CALLOC(ptr,n,s)	{ if ( (ptr = calloc(n,s)) == NULL )\
				quit("No space available"); }
#define	MNLEN	16
