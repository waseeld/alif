// ==================================
// (C)2016 IGS TECHNOLOGIES, SARL AU.
//             www.igs.ma
// ----------------------------------
// Alif Programming Language
// Alif compiler.
// ==================================
#pragma once
#define UNICODE
#define _UNICODE
using namespace std;
// ----------------------------------
#include <sstream> // stringstream
// ----------------------------------

/*
template <typename T>
T CONVERT_NUM_TO_STRING(T NUM)
{
    stringstream STRING_STREAM_BUFFER;
	STRING_STREAM_BUFFER << NUM;
	string STRING_BUFFER = STRING_STREAM_BUFFER.str();
	return STRING_BUFFER;
}
*/

string CONVERT_INT_TO_STRING(int INT_VAL)
{
	// Int --> String
	stringstream STRING_STREAM_BUFFER;
	STRING_STREAM_BUFFER << INT_VAL;
	string STRING_BUFFER = STRING_STREAM_BUFFER.str();
	return STRING_BUFFER;
}

string CONVERT_DOUBLE_TO_STRING(double INT_VAL)
{
	// Int --> String
	stringstream STRING_STREAM_BUFFER;
	STRING_STREAM_BUFFER << INT_VAL;
	string STRING_BUFFER = STRING_STREAM_BUFFER.str();
	return STRING_BUFFER;
}

string CONVERT_STRING_ARRAY_TO_STRING(string STRING_ARRAY_VAL[2048], unsigned LONG)
{
	// string[123] --> String
	stringstream STRING_STREAM_BUFFER;
	
	for (unsigned p = 0; p <= LONG; p++)
	{
		STRING_STREAM_BUFFER << STRING_ARRAY_VAL[p];
	}
	
	string STRING_BUFFER = STRING_STREAM_BUFFER.str();
	return STRING_BUFFER;
}

int CONVERT_STRING_TO_INT(wxString STRING)
{
	// string --> int
	
	return wxAtoi(STRING);
}

string CONVERT_WCHAT_T_TO_STRING(wchar_t* WCHART_T_VAL)
{
	// wchar_t --> String
	wstring W_STRING_BUFFER (WCHART_T_VAL);
	string STRING_BUFFER (W_STRING_BUFFER.begin(), W_STRING_BUFFER.end());
	return STRING_BUFFER;
}

string CONVERT_CHAR_TO_STRING(char* CHART_VAL)
{
	// Char --> String
	string STRING_BUFFER(CHART_VAL);
	return STRING_BUFFER;
}
