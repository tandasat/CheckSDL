//
// This module defines all banned function names defined by SDL
//
#pragma once


////////////////////////////////////////////////////////////////////////////////
//
// macro utilities
//


////////////////////////////////////////////////////////////////////////////////
//
// constants and macros
//

//
// Security Development Lifecycle (SDL) Banned Function Calls
// http://msdn.microsoft.com/en-us/library/bb288454.aspx
//
static const char* BANNED_FUNCTIONS_REQUIRED[] =
{
    "strcpy",
    "strcpyA",
    "strcpyW",
    "wcscpy",
    "_tcscpy",
    "_mbscpy",
    "StrCpy",
    "StrCpyA",
    "StrCpyW",
    "lstrcpy",
    "lstrcpyA",
    "lstrcpyW",
    "_tccpy",
    "_mbccpy",
    "_ftcscpy",
    "strncpy",
    "wcsncpy",
    "_tcsncpy",
    "_mbsncpy",
    "_mbsnbcpy",
    "StrCpyN",
    "StrCpyNA",
    "StrCpyNW",
    "StrNCpy",
    "strcpynA",
    "StrNCpyA",
    "StrNCpyW",
    "lstrcpyn",
    "lstrcpynA",
    "lstrcpynW",
    "strcat",
    "strcatA",
    "strcatW",
    "wcscat",
    "_tcscat",
    "_mbscat",
    "StrCat",
    "StrCatA",
    "StrCatW",
    "lstrcat",
    "lstrcatA",
    "lstrcatW",
    "StrCatBuff",
    "StrCatBuffA",
    "StrCatBuffW",
    "StrCatChainW",
    "_tccat",
    "_mbccat",
    "_ftcscat",
    "strncat",
    "wcsncat",
    "_tcsncat",
    "_mbsncat",
    "_mbsnbcat",
    "StrCatN",
    "StrCatNA",
    "StrCatNW",
    "StrNCat",
    "StrNCatA",
    "StrNCatW",
    "lstrncat",
    "lstrcatnA",
    "lstrcatnW",
    "lstrcatn",
    "sprintfW",
    "sprintfA",
    "wsprintf",
    "wsprintfW",
    "wsprintfA",
    "sprintf",
    "swprintf",
    "_stprintf",
    "wvsprintf",
    "wvsprintfA",
    "wvsprintfW",
    "vsprintf",
    "_vstprintf",
    "vswprintf",
    "wvsprintf",
    "wvsprintfA",
    "wvsprintfW",
    "vsprintf",
    "_vstprintf",
    "vswprintf",
    "strncpy",
    "wcsncpy",
    "_tcsncpy",
    "_mbsncpy",
    "_mbsnbcpy",
    "StrCpyN",
    "StrCpyNA",
    "StrCpyNW",
    "StrNCpy",
    "strcpynA",
    "StrNCpyA",
    "StrNCpyW",
    "lstrcpyn",
    "lstrcpynA",
    "lstrcpynW",
    "_fstrncpy",
    "strncat",
    "wcsncat",
    "_tcsncat",
    "_mbsncat",
    "_mbsnbcat",
    "StrCatN",
    "StrCatNA",
    "StrCatNW",
    "StrNCat",
    "StrNCatA",
    "StrNCatW",
    "lstrncat",
    "lstrcatnA",
    "lstrcatnW",
    "lstrcatn",
    "_fstrncat",
    "gets",
    "_getts",
    "_gettws",
    "IsBadWritePtr",
    "IsBadHugeWritePtr",
    "IsBadReadPtr",
    "IsBadHugeReadPtr",
    "IsBadCodePtr",
    "IsBadStringPtr",
    "memcpy",
    "RtlCopyMemory",
    "CopyMemory",
    "wmemcpy",
};


static const char* BANNED_FUNCTIONS_RECOMMENDED[] =
{
    "wnsprintf",
    "wnsprintfA",
    "wnsprintfW",
    "_snwprintf",
    "snprintf",
    "sntprintf",
    "_vsnprintf",
    "vsnprintf",
    "_vsnwprintf",
    "_vsntprintf",
    "wvnsprintf",
    "wvnsprintfA",
    "wvnsprintfW",
    "_snwprintf",
    "_snprintf",
    "_sntprintf",
    "nsprintf",
    "_vsnprintf",
    "_vsnwprintf",
    "_vsntprintf",
    "wvnsprintf",
    "wvnsprintfA",
    "wvnsprintfW",
    "strtok",
    "_tcstok",
    "wcstok",
    "_mbstok",
    "makepath",
    "_tmakepath",
    "_makepath",
    "_wmakepath",
    "_splitpath",
    "_tsplitpath",
    "_wsplitpath",
    "scanf",
    "wscanf",
    "_tscanf",
    "sscanf",
    "swscanf",
    "_stscanf",
    "_itoa",
    "_itow",
    "_i64toa",
    "_i64tow",
    "_ui64toa",
    "_ui64tot",
    "_ui64tow",
    "_ultoa",
    "_ultot",
    "_ultow",
    "CharToOem",
    "CharToOemA",
    "CharToOemW",
    "OemToChar",
    "OemToCharA",
    "OemToCharW",
    "CharToOemBuffA",
    "CharToOemBuffW",
    "alloca",
    "_alloca",
    "strlen",
    "wcslen",
    "_mbslen",
    "_mbstrlen",
    "StrLen",
    "lstrlen",
    "ChangeWindowMessageFilter",
};


////////////////////////////////////////////////////////////////////////////////
//
// types
//


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
