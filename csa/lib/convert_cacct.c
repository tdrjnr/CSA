/*
 * Copyright (c) 2000-2002 Silicon Graphics, Inc and LANL  All Rights Reserved.
 * Copyright (c) 2007 Silicon Graphics, Inc  All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it would be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details. 
 *
 * You should have received a copy of the GNU General Public License along 
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 * Contact information:  Silicon Graphics, Inc., 1140 East Arques Avenue, 
 * Sunnyvale, CA  94085, or:
 * 
 * http://www.sgi.com 
 */

/*
 *  The routines in this file convert cacct records.  Fields which did not
 *  exist previously are zeroed out.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif	/* HAVE_CONFIG_H */

#include <stdio.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/stat.h>

#include "acctdef.h"
#include "acctmsg.h"
#include "cacct.h"
#include "convert.h"
#include "csa.h"

/*
 *  Convert cacct records to current format.
 */
#define	FUNC_NAME     "convert_cacct()"
int
convert_cacct(void *buf, int flag)
{
	struct	achead	*h;

	h = (struct achead *)buf;

	CHECK_REV (h);
	convert_hdr(h, REC_CACCT, flag);
	
	return (sizeof(struct cacct));
}
#undef  FUNC_NAME

/*
 *  Determine if this cacct file has been converted
 *  Return:    -1 - if the file is not a cacct file.
 *	 	0 - if the file HAS NOT been converted.
 *		1 - if the file HAS been converted.
 */
#define	FUNC_NAME     "verify_cacct()"
int
verify_cacct(struct cacct *cbuf, FILE *in)
{
	int	retval = 0;

/*
 *	Read first record in order to check record header.
 */
	if (fread((char *)cbuf, sizeof(struct achead), 1, in) != 1) {
		acct_err(ACCT_ABORT,
		       _("%s: An error was returned from the '%s' routine."),
			FUNC_NAME, "fread()");
	}

/*
 *	Check the record header.
 */
	if (cbuf->ca_hdr.ah_type != ACCT_CACCT) {
		retval = -1;
	} else if ((cbuf->ca_hdr.ah_magic == ACCT_MAGIC) &&
	    (cbuf->ca_hdr.ah_revision == rev_tab[cbuf->ca_hdr.ah_type])) {
		retval = 1;
	}

/*
 *	Rewind the file for conversion processing.
 */
	rewind(in);

	return(retval);
}
#undef  FUNC_NAME
