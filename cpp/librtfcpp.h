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
 *   Definitions of C++ interface classes for librtf.
 *
 *  Autors :
 *      Tomek : Tomasz Hławiczka : at /at/ tru /./ pl
 *      
 *  Changes :
 *	28 Sep 2006, Tomek: Implementation.
 *	3  Oct 2006, Tomek: StyleSheet support.
 *
 */

#ifndef ___LIBRTF_CPP_INTERFACE_H___
#define ___LIBRTF_CPP_INTERFACE_H___

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <stack>
#include <vector>
#include <utility>
#include <map>
#include <fstream>
#include <set>

namespace format
{
	class processor;
	class source;
	class observer;

	namespace parsers
	{
		class rtf
		{
		public:
			explicit rtf( source & src, processor & proc, observer & log );
	
		protected:
			void exec ( source & src, processor & proc, observer & log );
		};
	}

	/** @brief Proxy class for C++ interfaces.
	 *
	 */

	class bridge
	{
	public:
		explicit bridge()
			: internal_object ( NULL )
		{
			// memset( internal_object, 0, sizeof( T ) );
		}
		
// 		template<typename T>
// 		explicit bridge()
// 			: internal_object ( reinterpret_cast< void * > ( new T() ) )
// 		{
// 			memset( internal_object, 0, sizeof( T ) );
// 		}
		
		/** Returns C structure object embeded in C++ class.
		 * 
		 * @return C
		 */
		template<typename C>
		inline C * c()
		{
			return reinterpret_cast<C *>(internal_object);
		}
		virtual ~bridge()
		{
			free( internal_object );
		}
			
		virtual void internal_init() = 0;

	protected:
		
		template<typename C, typename T>
		inline void register_object ( T * object ) {
			if( internal_object == NULL)
				internal_object = (C*) malloc( sizeof(C) );
			reinterpret_cast<C *>(internal_object)->object =
					reinterpret_cast< void * > ( object );
		}

		void * internal_object;
		
		friend class parsers::rtf;
	};

	struct image_descr
	{
		enum types {
			unknown = 0, //PICT_UNKNOWN,
			wm      = 1, //PICT_WM,
			mac     = 2, //PICT_MAC,
			pm      = 3, //PICT_PM,
			di      = 4, //PICT_DI,
			wb      = 5, //PICT_WB,
			jpeg    = 6, //PICT_JPEG,
			png     = 7, //PICT_PNG,
		};
		int width;
		int height;
		int depth;
		types type;
		int wmetafile_type;
	};
	
	class image_mgr : public bridge
	{
	public:
		explicit image_mgr();
		virtual void put ( const unsigned char * ch, unsigned long size ) =0;

	private:
		virtual void internal_init();
	};

	class observer  : public bridge
	{
	public:
		explicit observer();
		
		virtual void debug ( const char * )		=0;
		virtual void warning ( int id, const char * )	=0;
		virtual void error ( int id, const char *)	=0;
		
	private:
		virtual void internal_init();
	};
			
	class source : public bridge
	{
	public:
		explicit source();

		virtual size_t read(char * buf, size_t chars)	=0;
		virtual void open()				=0;
		virtual void close()				=0;
		
	private:
		virtual void internal_init();
	};

	class fonts
	{
	public:
		virtual ~ fonts() { }
		virtual void register_font( int num, const char * fontname, int charset ) 	=0;
		// virtual const std::string & fontname( int id )			=0;
	};

	class colors
	{
	public:
		typedef unsigned char uchar;
		
		virtual ~ colors() { }
		virtual void register_color( uchar r, uchar g, uchar b ) =0;
	};

	class attributes
	{
	public:
		enum attr_codes
		{
			none=0,
			bold, italic,

			underline, double_ul, word_ul,

			thick_ul, wave_ul,

			dot_ul, dash_ul, dot_dash_ul, dbl_dot_dash_ul,

			fontsize, std_fontsize, fontface,
			foreground, background,
			caps,
			smallcaps,

			shadow, outline, emboss, engrave, 

			super, sub, 
			strike, 
			dbl_strike, 

			expand,
		};
		struct style 
		{
			inline bool operator == (const style & another) const
			{
				return type == another.type && id == another.id;
			}
			inline bool operator < ( const style & another ) const
			{
				return type < another.type || ( type == another.type && id < another.id );
			}
			inline bool operator > ( const style & another ) const 
			{
				return type > another.type || ( type == another.type && id > another.id );
			}

			enum types
			{
				paragraph, character, section, table, table_row,
			};
			
			int id;
			int rsid;
			int bold;
			int italic;

			struct
			{
				int size;
				int face;
			} font;
			std::string name;
			types type;
		};

		virtual ~attributes() { }
		
		virtual void enable_attr( int attr, int param, bool enable ) =0;
		
		virtual void attr_push( int attr, int param)			=0;
		virtual void attrstack_push()					=0;
		virtual void attrstack_drop()					=0;
		virtual int attr_pop(int)					=0;
		virtual void attr_drop_all()					=0;
		virtual void attr_pop_all()					=0;
		virtual void attr_pop_dump()					=0;
		virtual void attr_remove(int * tab, int size )			=0;
		virtual void attr_push_style( style::types type, int id )	=0;

		virtual void register_style( const style & style )		=0;
	};

	class processor : public bridge,
			virtual public fonts,
			virtual public colors,
			virtual public attributes
	{
	protected:
		virtual void internal_init();

		typedef std::string string;
		// typedef const char * string;
	public:
	
		enum summary_categories
		{
			nubmer_of_pages,
			number_of_words,
			number_of_chars,
		};
		enum special_characters 
		{
			bullet,
			left_quote,
			right_quote,
			left_dbl_quote,
			right_dbl_quote,
			nonbreaking_space,
			emdash,
			endash,
			lessthan,
			greaterthan,
			amp,
			copyright,
			trademark,
			nonbreaking_hyphen,
			optional_hyphen,
			dbl_quote,
			line_break,
			page_break,
			paragraph_begin,
			center_begin,	// move to attr_code
			right_begin,	// -- || --
			justify_begin,	// -- || --
			center_end,	// -- || --
			right_end,		// -- || --
			justify_end,	// -- || --
			tab,
		};
		
		explicit processor();
		virtual ~processor();

		virtual void charset( int charset_id );
		virtual void codepage( int cp_id );

		virtual void begin()                            =0;
		virtual void end()                              =0;
		virtual void title( const string & title )              =0;
		virtual void keywords( const string & keywords )         =0;
		virtual void author( const string & author )             =0;
		virtual void summary( summary_categories cat, int param)   =0;

		virtual char* translate_char(int);

		virtual void print( const string & )				=0;
		virtual void print_symbol( const string & )			=0;
		virtual void print_forced_space( )					=0;
		virtual int  print_unicode( int )					=0;
		virtual void hyperlink( const string & )			=0;
		virtual void hyperlink_base ( const string & )		=0;

		virtual void print_char( special_characters ch )	=0;
		virtual void font_smaller_begin()					=0;
		virtual void font_smaller_end()						=0;
		virtual void table_begin ()							=0;
		virtual void table_end ()							=0;
		virtual void table_cell_begin ()					=0;
		virtual void table_cell_end ()						=0;
		virtual void table_row_begin ()						=0;
		virtual void table_row_end ()						=0;

		virtual format::image_mgr * image_begin(const format::image_descr & image)	=0;
		virtual void image_end(format::image_mgr &)					=0;
		
	};

	namespace utils 
	{
		class stream_logger : public observer
		{
		public:
			explicit stream_logger(std::ostream & output, 
						  bool log_errors = true,
						  bool log_warnings = true,
						  bool log_debug = false );
		protected:	
			
			virtual void debug ( const char * );
			virtual void warning ( int id, const char * );
			virtual void error ( int id, const char *);

		protected:
			std::ostream & output;
			bool log_errors, log_warnings, log_debug;
		};

		class stream_source : public source
		{
		public:
			explicit stream_source( std::istream & input );
		protected:
			virtual size_t read(char * buf, size_t chars);
			virtual void open();
			virtual void close();
		protected:
			std::istream & input;
			char * buf;
		};

		class file_image_mgr : public image_mgr
		{
		public:
			explicit file_image_mgr(const std::string & f)
			{
				output.open( f.c_str() );
			}
			virtual ~file_image_mgr()
			{
				output.close();
			}
			
			virtual void put( const unsigned char * ch, unsigned long size )
			{
				output.write( (const char*) ch, size );
			}

			std::ofstream output;
		};

		class attributes_stack :
			virtual protected format::attributes
		{
		protected:
			typedef std::pair<int, int> attribute;
			typedef std::stack< attribute > block;
			typedef std::stack< block > stack;

		public:

		protected:
			typedef std::set< style > styles_collection;

			styles_collection styles;
			stack attributes;
			
			virtual void attr_push( int attr, int param);
			virtual void attrstack_push();
			virtual void attrstack_drop();
			virtual int attr_pop(int);
			virtual void attr_drop_all();
			virtual void attr_pop_all();
			virtual void attr_pop_dump();
			virtual void attr_remove(int * tab, int size );

			virtual void attr_push_style(format::attributes::style::types type, int style_id);

			virtual void register_style(const format::attributes::style & rtf_style );
		};

		class fonts_stack : virtual protected format::fonts
		{
		protected:
			typedef int oid;
			typedef std::map< oid, std::string > fonts_container;
			fonts_container   fonts;
			
			virtual void register_font( int id, const char * fontname, int charset );
			virtual const std::string & fontname( int id );
		};

		class colors_stack : virtual protected format::colors
		{
		protected:
			
			struct color {
				color(uchar r, uchar g, uchar b ) : r(r), g(g), b(b) { }
				uchar r,g,b;
			};
			
			typedef std::vector< color > colors_container;
			colors_container   colors;

			virtual void register_color( uchar, uchar, uchar );
		};
	}

}

#endif /// ___LIBRTF_CPP_INTERFACE_H___

