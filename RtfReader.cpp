#include <sstream>
#include "RtfReader.h"
#include "cpp/text.h"
using namespace std;

bool RtfReader::RtfStream2TextString(string &outstring, istream &instream)
{
	using namespace format;
	try
	{
		// "{\r" at the beginning, the RTF file is considered
		char head[3] = { 0 };
		instream.read(head, sizeof(head));
		if (head[0] != '{' || head[1] != '\\' || head[2] != 'r')
		{
			return false;
		}

		instream.seekg(0, ios::beg);
		utils::stream_logger log(cerr, true, true, true);
		utils::stream_source src(instream);
		std::stringstream stream;
		RtfReader::text proc(stream);

		parsers::rtf parser(src, proc, log);
		outstring = stream.str();
	}
	catch (...)
	{
		return false;
	}
	return true;
}

bool RtfReader::RtfString2TextString(string &outstring, const string &instring)
{
	std::stringstream instream(instring);
	return RtfStream2TextString(outstring, instream);
}

bool RtfReader::RtfFile2TextString(string &outstring, const char * filename)
{
	ifstream file(filename);
	return RtfStream2TextString(outstring, file);
}
