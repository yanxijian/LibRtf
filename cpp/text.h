#ifndef __VXCOMMONLIB_LIBRTF_TEXT_H__
#define __VXCOMMONLIB_LIBRTF_TEXT_H__

#include <iostream>
#include <ostream>
#include <string>
#include "librtfcpp.h"

using namespace std;
using namespace format;

namespace RtfReader
{
	class text :
		public format::processor,
		public format::utils::attributes_stack,
		public format::utils::fonts_stack
	{
	protected:
		ostream &stream;

	public:
		text(ostream &stream)
			: stream(stream)
		{
		}

		// format::processor
		virtual void begin();
		virtual void end();
		virtual void title(const string &title);
		virtual void keywords(const string &keywords);
		virtual void author(const string &author);
		virtual void summary(summary_categories cat, int param);

		virtual void print(const string &);
		virtual void print_symbol(const string &);
		virtual void print_forced_space();
		virtual int  print_unicode(int);
		virtual void hyperlink(const string &);
		virtual void hyperlink_base(const string &);

		virtual void print_char(special_characters ch);
		virtual void font_smaller_begin();
		virtual void font_smaller_end();
		virtual void table_begin();
		virtual void table_end();
		virtual void table_cell_begin();
		virtual void table_cell_end();
		virtual void table_row_begin();
		virtual void table_row_end();

		virtual format::image_mgr *image_begin(const format::image_descr & image);
		virtual void image_end(format::image_mgr &);


		// format::utils::attributes_stack
		virtual void enable_attr(int attr, int param, bool enable);

		virtual void attr_push(int attr, int param);
		virtual void attrstack_push();
		virtual void attrstack_drop();
		virtual int  attr_pop(int);
		virtual void attr_drop_all();
		virtual void attr_pop_all();
		virtual void attr_pop_dump();
		virtual void attr_remove(int * tab, int size);
		virtual void attr_push_style(style::types type, int id);

		virtual void register_style(const style & style);


		// format::utils::fonts_stack
		virtual void register_font(int num, const char * fontname, int charset);

		// format::colors
		virtual void register_color(uchar r, uchar g, uchar b);
	};
}
#endif