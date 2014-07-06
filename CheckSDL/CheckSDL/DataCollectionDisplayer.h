//
// This module declares a class responsible for outputting FileStatus
//
#pragma once


// C/C++ standard headers
// Other external headers
// Windows headers
// Project headers
#include "FileStatus.h"


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


////////////////////////////////////////////////////////////////////////////////
//
// prototypes
//

class DataCollectionDisplayer
{
public:
    explicit DataCollectionDisplayer(
        std::vector<FileStatus> Files);
    bool hasData() const;
    bool csvWrite(
        const std::tr2::sys::path& FilePath) const;
    void print() const;

    struct Statistics
    {
        std::uint32_t sumOfCriticalPriorityViolations;
        std::uint32_t sumOfHighPriorityViolations;
        std::uint32_t sumOfBadCompilerUsages;
        std::uint32_t sumOfNoGS;
        std::uint32_t sumOfNoSEHProtection;
        std::uint32_t sumOfNoASLR;
        std::uint32_t sumOfNoDEP;
        std::uint32_t sumOfNoHeapProtection;
        std::uint32_t numberOfExecutableFiles;
        std::uint32_t sumOfBadSections;
        std::uint32_t sumOfBad1Funcs;
        std::uint32_t sumOfBad2Funcs;
    };

private:
    std::vector<FileStatus> m_files;
    Statistics m_stat;
};


////////////////////////////////////////////////////////////////////////////////
//
// variables
//


////////////////////////////////////////////////////////////////////////////////
//
// implementations
//
