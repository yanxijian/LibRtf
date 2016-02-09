#ifndef __RTFREADER_H__
#define __RTFREADER_H__

#ifdef _WIN32
#ifdef RTFREADER_EXPORTS
#define RTFREADER_EXT  _declspec(dllexport)
#else
#define RTFREADER_EXT  _declspec(dllimport)
#endif
#else
#define RTFREADER_EXT
#endif

#include <string>
namespace RtfReader
{
	RTFREADER_EXT bool RtfStream2TextString(std::string &outstring, std::istream &instream);
	RTFREADER_EXT bool RtfString2TextString(std::string &outstring, const std::string &instring);
	RTFREADER_EXT bool RtfFile2TextString(std::string &outstring, const char *filename);
}

#endif
