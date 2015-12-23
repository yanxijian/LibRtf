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

/** Description :
 *   Implementation of C++ interface classes for librtf.
 *
 *  Autors :
 *      Tomek : Tomasz Hławiczka : at /at/ tru /./ pl
 *
 *  Changes :
 *	28 Sep 2006, Tomek: Implementation
 *
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
extern "C" {
	#include "../c/librtf.h"
}

#include <string>
#include <iostream>
#include <errno.h>
#include <fstream>
#include <list>

#include "librtfcpp.h"

#define impl0(C,R,FUNC) R cpp_##FUNC ( c_def_##C * f ) { \
	return ( reinterpret_cast<format::C *> ( f->object )->FUNC() ); \
}
#define impl1(C,R,FUNC,PA) R cpp_##FUNC ( c_def_##C * f, PA a ) { \
	return ( reinterpret_cast<format::C *> ( f->object )->FUNC(a) ); \
}
#define impl2(C,R,FUNC,PA,PB) R cpp_##FUNC ( c_def_##C * f, PA a, PB b) { \
	return ( reinterpret_cast<format::C *> ( f->object )->FUNC(a,b) ); \
}
#define impl3(C,R,FUNC,PA,PB,PC) R cpp_##FUNC ( c_def_##C * f, PA a, PB b, PC c ) { \
	return reinterpret_cast<format::C *> ( f->object )->FUNC(a, b, c); \
}
#define impl4(C,R,FUNC,PA,PB,PC,PD) R cpp_##FUNC ( c_def_##C * f, PA a, PB b, PC c, PD d ) { \
	return reinterpret_cast<format::C *> ( f->object )->FUNC(a, b, c, d); \
}
#define use(func) cobject->func = & cpp_##func;

#define duse(func) std::cerr << #func "\t= " << (void*) cobject->func << std::endl;

typedef unsigned char uchar;
typedef ::processor	c_def_processor;
typedef ::source	c_def_source;
typedef ::observer	c_def_observer;
typedef ::image_mgr	c_def_image_mgr;

impl2(processor, void,  attr_push			, int, int );
impl0(processor, void,  attrstack_push		);
impl0(processor, void,  attrstack_drop		);
impl1(processor, int,   attr_pop			, int);
impl0(processor, void,  attr_drop_all		);
impl0(processor, void,  attr_pop_all		);
impl0(processor, void,  attr_pop_dump		);
// impl2(processor, void,  attr_push_style	, format::attributes::style::types, int );
impl0(processor, void,  begin			);
impl0(processor, void,  end			);
impl1(processor, void,  title			, const char * );
impl1(processor, void,  keywords		, const char * );
impl1(processor, void,  author		, const char *	);
// impl2(processor, void,  summary            , enum document_summary, int );
impl1(processor, char*, translate_char		,int);
impl1(processor, void,  print			, const char *);
impl1(processor, void,  print_symbol		, const char * );
impl0(processor, void,  print_forced_space		);
impl1(processor, int,   print_unicode		, int );
impl1(processor, void,  hyperlink			, const char * );
impl1(processor, void,  hyperlink_base			, const char * );
//impl1(processor, void,  print_char			, enum special_char );
impl0(processor, void,  font_smaller_begin );
impl0(processor, void,  font_smaller_end   );
impl0(processor, void,  table_begin		);
impl0(processor, void,  table_end			);
impl0(processor, void,  table_cell_begin		);
impl0(processor, void,  table_cell_end		);
impl0(processor, void,  table_row_begin		);
impl0(processor, void,  table_row_end		);
impl2(processor, void,  attr_remove		, int * , int );
impl3(processor, void,  register_font		, int, const char *, int );
impl3(processor, void,  register_color		, uchar,uchar,uchar );
// impl1(processor, void,  register_style		, const ::TextStyle * );
impl1(processor, void,  charset			, int );
impl1(processor, void,  codepage			, int );

impl2(image_mgr, void, put			, const uchar *, unsigned long );
	
impl1(observer, void, debug			, const char *);
impl2(observer, void, warning		, int, const char *);
impl2(observer, void, error			, int, const char *);

impl0(source, void, open			);
impl0(source, void, close			);
impl2(source, size_t, read			, char *, size_t);


::image_mgr * cpp_image_begin( ::processor * proc , const image_descr * desc )
{
	format::image_descr img;
	img.width = desc->width;
	img.height = desc->height;
	img.depth = desc->bits_per_pixel;
	img.type = static_cast< format::image_descr::types > ( desc->type );
	img.wmetafile_type = desc->wmetafile_type;
	
	format::image_mgr * mgr = reinterpret_cast< format::processor * > ( proc->object )->image_begin( img );
	if( mgr == NULL )
		return NULL;

	((format::bridge*)mgr)->internal_init();

	return mgr->c< ::image_mgr > ();
}

void cpp_image_end( ::processor * proc, ::image_mgr * mgr )
{
	// TODO: mgr cannot be NULL !
	format::image_mgr * cpp_mgr = reinterpret_cast< format::image_mgr * > ( mgr->object );
	// TODO: cpp_mgr cannot be NULL !
	reinterpret_cast< format::processor * > ( proc->object )->image_end( *cpp_mgr );
}

void cpp_register_style( ::processor * proc, const TextStyle * style )
{
	format::attributes::style cpp_style;
	cpp_style.bold = style->bold;
	cpp_style.id	= style->id;
	cpp_style.italic = style->italic;
	if( style->stylename )
		cpp_style.name = style->stylename;
	cpp_style.rsid = style->rsid;
	cpp_style.type = static_cast< format::attributes::style::types > ( style->type );
	cpp_style.font.size = style->fontsize;
	cpp_style.font.face = style->fontface;

	
	reinterpret_cast< format::processor * > ( proc->object )->register_style( cpp_style );
}

void cpp_attr_push_style( ::processor * proc, TextStyleType type, int style_id )
{
	reinterpret_cast< format::processor * > ( proc->object )->attr_push_style (
		static_cast < format::attributes::style::types > ( type ), style_id
	);
}

void cpp_summary ( ::processor * proc, ::document_summary category, int param )
{
	reinterpret_cast< format::processor * > ( proc->object )->summary (
		static_cast< format::processor::summary_categories > ( category ), param 
	);
}

void cpp_print_char ( ::processor * proc, ::special_char ch )
{
	reinterpret_cast< format::processor * > ( proc->object )->print_char (
			static_cast< format::processor::special_characters > ( ch ) );
}

format::observer::observer()
	: bridge( )
{
}
		
void format::observer::internal_init()
{
	register_object < ::observer > ( this );
	
	::observer * cobject = c < ::observer > ();
		
	use( debug );
	use( error );
	use( warning );
}

format::source::source()
	: bridge( )
{
}
		
void format::source::internal_init()
{
	register_object < ::source > ( this );
	
	::source * cobject = c < ::source > ();

	use( read );
	use( open );
	use( close );
}

format::image_mgr::image_mgr()
	: bridge()
{
}
		
void format::image_mgr::internal_init()
{
	register_object < ::image_mgr > ( this );
	
	::image_mgr * cobject = c < ::image_mgr > ();
	
	use( put );
}

namespace utils = format::utils;

utils::stream_logger::stream_logger(std::ostream & output, bool log_errors, bool log_warnings, bool log_debug)
	: output(output), 
	log_errors(log_errors), 
	log_warnings(log_warnings), 
	log_debug(log_debug)
{
}

void utils::stream_logger::debug ( const char * str )
{
	output << "DEBUG   : " << str << std::endl;
}

void utils::stream_logger::warning ( int id, const char * str)
{
	output << "WARNING : " << str << std::endl;
}

void utils::stream_logger::error ( int id, const char * str)
{
	output << "ERROR   : " << str << std::endl;
}

utils::stream_source::stream_source( std::istream & input )
	: input(input), buf(NULL)
{
}

size_t utils::stream_source::read(char * buf, size_t chars)
{
	input.read(buf, chars);
	return input.gcount();
}

void utils::stream_source::open()
{	
}

void utils::stream_source::close()
{	
}

format::parsers::rtf::rtf(source & src, processor & proc, observer & log)
{
	exec( src, proc, log );
}

void format::parsers::rtf::exec(source & src, processor & proc, observer & log)
{
	((bridge&)src).internal_init();
	((bridge&)proc).internal_init();
	((bridge&)log).internal_init();
	
	::rtf_process( src.c< ::source >(), proc.c< ::processor >(), log.c < ::observer > () );
}

format::processor::processor()
	: bridge( )
{
}

format::processor::~processor()
{
}

void format::processor::internal_init()
{
	register_object < ::processor > ( this );
	::processor * cobject = c < ::processor > ();

	use( attr_push			);
	use( attrstack_push		);
	use( attrstack_drop		);
	use( attr_pop			);
	use( attr_drop_all		);
	use( attr_pop_all		);
	use( attr_pop_dump		);
	use( attr_remove		);
	use( attr_push_style		);

	use( register_font		);
	use( register_color		);
	use( register_style		);

	use( begin			);
	use( end			);
	use( title			);
	use( keywords			);
	use( author			);
	use( summary   			);
	use( translate_char		);
	use( print			);
	use( print_symbol		);
	use( print_forced_space		);
	use( print_unicode		);
	use( hyperlink			);
	use( hyperlink_base		);
	use( print_char			);
	use( font_smaller_begin		);
	use( font_smaller_end		);
	use( table_begin		);
	use( table_end			);
	use( table_cell_begin		);
	use( table_cell_end		);
	use( table_row_begin		);
	use( table_row_end		);
	use( image_begin		);
	use( image_end			);

	use( charset			);
	use( codepage			);

}

void format::processor::charset( int charset_id )
{
}

void format::processor::codepage( int cp_id )
{
}


char * format::processor::translate_char(int ch )
{
//	short start;
	char *result=NULL;

	/* if we are seeking a character from a symbol font we can
	   be below 0x80
	*/
	/*
	if(ntable == FONTSYMBOL_TABLE)
	{
		start = op->symbol_first_char;

		if(ch >= start && ch <= op->symbol_last_char)
			result = op->symbol_translation_table[ch - start];
	}
	else
	if (ch >= 0x20 && ch < 0x80) {
		result = op->ascii_translation_table [ch - 0x20];
	}
	else
	if (charset != CHARSET_ANSI &&
	    charset != CHARSET_MAC &&
	    charset != CHARSET_CP437 &&
	    charset != CHARSET_CP850)
		error_handler ("invalid character set value, cannot translate character");
	else
	switch (charset)
	{
	case CHARSET_ANSI:
		if (codepage != NULL && op->unisymbol_print != NULL && codepage->cp) {
			if (codepage->chars[ch - 0x80]) {
				result = op->unisymbol_print(codepage->chars[ch - 0x80]);
			}
		} else {
		start = op->ansi_first_char;
			if (ch >= start &&
			    ch <= op->ansi_last_char)
				result = op->ansi_translation_table [ch-start];
		}
		break;
	case CHARSET_MAC:
		start = op->mac_first_char;
		if (ch >= start &&
		    ch <= op->mac_last_char)
			result = op->mac_translation_table [ch-start];
		break;
	case CHARSET_CP437:
		start = op->cp437_first_char;
		if (ch >= start &&
		    ch <= op->cp437_last_char)
			result = op->cp437_translation_table [ch-start];
		break;
	case CHARSET_CP850:
		start = op->cp850_first_char;
		if (ch >= start &&
		    ch <= op->cp850_last_char)
			result = op->cp850_translation_table [ch-start];
		break;
	}
	*/
	return result;
}


void utils::fonts_stack::register_font( int num, const char * fontname, int charset )
{
	fonts[ num ] = fontname;
	// FIXME: use charset
}

const std::string & utils::fonts_stack::fontname( int id )
{
	return fonts[ id ];
}

void utils::colors_stack::register_color( uchar r, uchar g, uchar b )
{
	colors.push_back( color(r,g,b) );
}

void utils::attributes_stack::attr_push_style( format::attributes::style::types type, int style_id )
{
	style tmp;
		tmp.id = style_id;
		tmp.type = type;
	styles_collection::const_iterator it = styles.find( tmp );
	if( it == styles.end() )
		return;

	const style & ref = (*it);
	if ( ref.bold == 1 )
		attr_push( ATTR_BOLD, 0);
	else if( ref.bold == 0 )
		attr_pop( ATTR_BOLD );

	if ( ref.italic == 1 )
		attr_push( ATTR_ITALIC, 0);
	else if( ref.italic == 0 )
		attr_pop( ATTR_ITALIC );

	if( ref.font.face >= 0 )
		attr_push( ATTR_FONTFACE, ref.font.face );

	if( ref.font.size >= 0 )
		attr_push( ATTR_FONTSIZE, ref.font.size );
}

void utils::attributes_stack::register_style( const format::attributes::style & rtf_style )
{
	styles.insert( rtf_style );
}

void utils::attributes_stack::attr_push( int attr, int param )
{
	if(attributes.empty() )
		return;
	attributes.top().push( attribute(attr, param ) );
	enable_attr( attr, param, true );
}

void utils::attributes_stack::attrstack_push()
{
	attributes.push ( block() );
}

void utils::attributes_stack::attrstack_drop()
{
	if(attributes.empty() ) 
		return;

	block & b = attributes.top();
	while( b.empty() == false )
	{
		attribute & attr = b.top();
		enable_attr( attr.first, attr.second, false );
		b.pop();
	}
	attributes.pop();
}

int utils::attributes_stack::attr_pop(int attr )
{
	if(attributes.empty() ) 
		return 0;
	block & blk = attributes.top();
	if(blk.empty() ) 
		return 0;

	if( blk.top().first == attr )
	{
		enable_attr( blk.top().first, blk.top().second, false );
		blk.pop();
	}
	else
	{
		std::list< attribute > backup;
		while ( blk.empty() == false )
			if ( blk.top().first == attr )
			{
				enable_attr( blk.top().first, blk.top().second, false );
				blk.pop();
				break;
			}
			else
			{
				backup.push_front( blk.top() );
				blk.pop();
			}
		for( std::list<attribute >::const_iterator it= backup.begin(), et=backup.end(); it != et; ++it)
		{
				blk.push( *it );
				enable_attr( it->first, it->second, true );
		}
	}

	return 1;
}

void utils::attributes_stack::attr_drop_all()
{
	if(attributes.empty() ) 
		return;

	attributes.pop();
}

void utils::attributes_stack::attr_pop_all()
{
	if(attributes.empty() ) 
		return;

	block & b = attributes.top();
	while( b.empty() == false )
	{
		attribute & attr = b.top();
		enable_attr( attr.first, attr.second, false );
		b.pop();
	}
}

void utils::attributes_stack::attr_pop_dump()
{
	if(attributes.empty() ) 
		return;

	block & b = attributes.top();
	while( b.empty() == false )
	{
		attribute & attr = b.top();
		enable_attr( attr.first, attr.second, false );
		b.pop(); // FIXME this version should not remove attributes from stack.
	}
}

void utils::attributes_stack::attr_remove(int * tab, int size )
{
	if(attributes.empty() ) 
		return;

	block & b = attributes.top();
	while( b.empty() == false )
	{
		attribute & attr = b.top();
		for( int i=size; i--; )
			if( attr.first == tab[i] )
			{
				enable_attr( attr.first, attr.second, false );
				b.pop();
				break; // FIXME not only last agrument should be checked, another container is needed.
			}
	}
}

