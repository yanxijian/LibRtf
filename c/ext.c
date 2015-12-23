/***************************************************************************
 *   Copyright (C) 2006 by Tomasz Hławiczka   *
 *   tomek@tru.pl   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

/** This file contanis implementation of extensions for librtf.

Following extensions are declared in librtf.h file.
They might be used with C version of this library. For C++ there are another
functions.

When you don't need it (for example when you use library only via C++ or other
higher languages) you might define LIBRTF_DISABLE_EXT to skip these functions.
*/

#include "librtf.h"
#include <stdio.h>

#ifndef LIBRTF_DISABLE_C_EXT

static size_t
src_stream_read( source * src, char * buf, size_t size)
{
	return fread( buf, 1, size, (FILE*) src->object );
}

static void
src_stream_open( source * src )
{
}

static void
src_stream_close()
{
}

//! Initialize processor source as stream reader.
void
rtf_src_stream_init( source * src, FILE * stream )
{
	src->object = (void*) stream;
	src->read   = & src_stream_read;
	src->open   = & src_stream_open;
	src->close  = & src_stream_close;
}

void
rtf_src_stream_free( source * src )
{
	src->object = NULL;
	src->read   = NULL;
	src->open   = NULL;
	src->close  = NULL;
}

static void
log_stream_debug ( observer * log, const char * msg )
{
	// FIXME;
}

static void
log_stream_warning ( observer * log, int id, const char * msg )
{
	// FIXME;
}

static void
log_stream_error (  observer * log, int id, const char * msg )
{
	// FIXME;
}

//! Initialize observer as stream output.
void
rtf_log_stream_init( observer * log, FILE * stream )
{
	log->object = (void*) stream;
	
	log->debug   = log_stream_debug;
	log->warning = log_stream_warning;
	log->error   = log_stream_error;
}

void
rtf_log_stream_free( observer * log )
{
	log->object   = NULL;
	log->debug    = NULL;
	log->warning  = NULL;
	log->error    = NULL;
}

static void
log_null_debug ( observer * log, const char * msg )
{
}

static void
log_null_warning ( observer * log, int id, const char * msg )
{
}

static void
log_null_error (  observer * log, int id, const char * msg )
{
}

//! Initialize observer to /dev/null
void
rtf_log_null_init( observer * log )
{
	log->object = NULL;
	
	log->debug   = log_null_debug;
	log->warning = log_null_warning;
	log->error   = log_null_error;
}

void
rtf_log_null_free( observer * log )
{
	log->object   = NULL;
	log->debug    = NULL;
	log->warning  = NULL;
	log->error    = NULL;
}

#endif // LIBRTF_DISABLE_C_EXT
