#pragma once
/* Standart include file */
#include <iostream>
#include <string>
#include <vector>

#ifdef WIN_PATH
	const char cPathSeparator = '\\';
#else
	const char cPathSeparator = '/';
#endif