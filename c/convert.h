/***************************************************************************
 *   Copyright (C) 2006 by Tomasz Hławiczka                                *
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

/*----------------------------------------------------------------------
 * Module name:    convert
 * Author name:    Zachary Smith
 * Create date:    19 Sep 2001
 * Purpose:        Definitions for the conversion module
 *----------------------------------------------------------------------
 * Changes:
 * 31 March 2005 by daved@physiol.usyd.edu.au: changes requested by ZT Smith
 *--------------------------------------------------------------------*/


#ifndef ___CONVERT_H___
#define ___CONVERT_H___

enum {
	CHARSET_ANSI=1,
	CHARSET_MAC,
	CHARSET_CP437,
	CHARSET_CP850,
};


#if 1 /* daved 0.19.6 - support for multiple char number->output tables */
	#define FONTROMAN_TABLE 0
	#define FONTSYMBOL_TABLE 1
#endif

#endif // ___CONVERT_H___

