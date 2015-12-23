#include <sstream>
#include "text.h"

#ifdef _WIN32
string newLine = "\r\n";
#else
string newLine = "\n";
#endif

// format::processor
void RtfReader::text::begin()
{
	// Nothing to do
}

void RtfReader::text::end()
{
	// Nothing to do
}

void RtfReader::text::title(const string & title)
{
	// Nothing to do
}

void RtfReader::text::keywords(const string & keywords)
{
	// Nothing to do
}

void RtfReader::text::author(const string & author)
{
	// Nothing to do
}

void RtfReader::text::summary(summary_categories cat, int param)
{
	// Nothing to do
}

void RtfReader::text::print(const string &txt)
{
	stream << txt;
}

void RtfReader::text::print_symbol(const string &)
{
	// Nothing to do
}
void RtfReader::text::print_forced_space()
{
	stream << " ";
}

int  RtfReader::text::print_unicode(int)
{
	return 1;
}

void RtfReader::text::hyperlink(const string &url)
{
	stream << url;
}

void RtfReader::text::hyperlink_base(const string &)
{
	// Nothing to do
}

void RtfReader::text::print_char(special_characters ch)
{
	switch (ch)
	{
		case bullet:
		case left_quote:
			stream << "\'"; break;
		case right_quote:
			stream << "\'"; break;
		case left_dbl_quote:
			stream << "\""; break;
		case right_dbl_quote:
			stream << "\""; break;
		case nonbreaking_space:
			stream << " "; break;
		case emdash:
			stream << ""; break;
		case endash:
			stream << ""; break;
		case lessthan:
			stream << "<"; break;
		case greaterthan:
			stream << ">;"; break;
		case amp:
			stream << "&"; break;
		case copyright:
			stream << "(C)"; break;
		case trademark:
			stream << "TM"; break;
		case nonbreaking_hyphen:
			stream << "-"; break;
		case optional_hyphen:
			stream << "-"; break;
		case dbl_quote:
			stream << "\""; break;
		case line_break:
			stream << newLine; break;
		case page_break:
			stream << newLine; break;
			break;
		case paragraph_begin:
			stream << newLine << "\t"; break;
		case center_begin:
			stream << ""; break;
		case right_begin:
			stream << ""; break;
		case justify_begin:
			stream << ""; break;
		case center_end:
			stream << ""; break;
		case right_end:
			stream << ""; break;
		case justify_end:
			stream << ""; break;
		case tab:
			stream << "\t"; break;
		default:
			break;
	}
}

void RtfReader::text::font_smaller_begin()
{
	// Nothing to do
}

void RtfReader::text::font_smaller_end()
{
	// Nothing to do
}

void RtfReader::text::table_begin()
{
	// Nothing to do
}

void RtfReader::text::table_end()
{
	// Nothing to do
}

void RtfReader::text::table_cell_begin()
{
	// Nothing to do
}

void RtfReader::text::table_cell_end()
{
	// Nothing to do
}

void RtfReader::text::table_row_begin()
{
	// Nothing to do
}

void RtfReader::text::table_row_end()
{
	// Nothing to do
}

format::image_mgr * RtfReader::text::image_begin(const format::image_descr & image)
{
	// Nothing to do
	return NULL;
}

void RtfReader::text::image_end(format::image_mgr &)
{
	// Nothing to do
}

// format::utils::attributes_stack
void RtfReader::text::enable_attr(int attr, int param, bool enable)
{
	// Nothing to do
}

void RtfReader::text::attr_push(int attr, int param)
{
	// Nothing to do
}

void RtfReader::text::attrstack_push()
{
	// Nothing to do
}

void RtfReader::text::attrstack_drop()
{
	// Nothing to do
}

int  RtfReader::text::attr_pop(int)
{
	return 1;
}

void RtfReader::text::attr_drop_all()
{
	// Nothing to do
}

void RtfReader::text::attr_pop_all()
{
	// Nothing to do
}

void RtfReader::text::attr_pop_dump()
{
	// Nothing to do
}

void RtfReader::text::attr_remove(int * tab, int size)
{
	// Nothing to do
}
void RtfReader::text::attr_push_style(style::types type, int id)
{
	// Nothing to do
}

void RtfReader::text::register_style(const style & style)
{
	// Nothing to do
}

// format::utils::fonts_stack
void RtfReader::text::register_font(int num, const char * fontname, int charset)
{
	// Nothing to do
}

// format::colors
void RtfReader::text::register_color(uchar r, uchar g, uchar b)
{
	// Nothing to do
}