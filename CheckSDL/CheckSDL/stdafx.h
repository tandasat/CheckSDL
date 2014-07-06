// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
#pragma once
#include "targetver.h"


// C/C++ standard headers
#include <cassert>
#include <cstdint>
#include <string>
#include <vector>
#include <filesystem>
#include <iostream>
#include <iomanip>
#include <functional>
#include <algorithm>
#include <fstream>
#include <sstream>

// Other external headers
// Windows headers
#include <tchar.h>
#include <Windows.h>
#include <DbgHelp.h>
#pragma comment(lib, "Dbghelp.lib")

// Project headers
#include "N3949/scope_guard.h"
#include "N3949/unique_resource.h"
#define _SDL_BANNED_RECOMMENDED
#include "banned.h"


////////////////////////////////////////////////////////////////////////////////
//
// macro utilities
//


////////////////////////////////////////////////////////////////////////////////
//
// constants and macros
//


////////////////////////////////////////////////////////////////////////////////
//
// types
//

enum class TriBool : std::uint8_t
{
    False = 0,
    True,
    NotApplicable,
};


////////////////////////////////////////////////////////////////////////////////
//
// prototypes
//


////////////////////////////////////////////////////////////////////////////////
//
// variables
//


////////////////////////////////////////////////////////////////////////////////
//
// implementations
//
