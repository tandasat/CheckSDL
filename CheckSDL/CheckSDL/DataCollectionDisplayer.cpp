//
// This module implements a class responsible for outputting FileStatus
//
#include "stdafx.h"
#include "DataCollectionDisplayer.h"


// C/C++ standard headers
// Other external headers
// Windows headers
// Project headers


namespace fs = std::tr2::sys;


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

namespace {


template <typename OutPutDevice>
void write(
    const std::vector<FileStatus>& Files,
    const DataCollectionDisplayer::Statistics& Stat,
    OutPutDevice& Device,
    char Separator);


tm UintToTm(
    std::uint32_t SecSince1900);


} // End of namespace {unnamed}


////////////////////////////////////////////////////////////////////////////////
//
// variables
//


////////////////////////////////////////////////////////////////////////////////
//
// implementations
//

DataCollectionDisplayer::DataCollectionDisplayer(
    std::vector<FileStatus> Files)
    : m_files(std::move(Files))
    , m_stat({})
{
    // Sorts elements in that priority:
    // numberOfCriticalPriorityViolations > numberOfHighPriorityViolations > path
    std::sort(std::begin(m_files), std::end(m_files), [](
        const decltype(m_files)::value_type& Lhs,
        const decltype(m_files)::value_type& Rhs)
    {
        if (Lhs.sdlStatus.getSeverity().numberOfCriticalPriorityViolations !=
            Rhs.sdlStatus.getSeverity().numberOfCriticalPriorityViolations)
        {
            return
                Lhs.sdlStatus.getSeverity().numberOfCriticalPriorityViolations >
                Rhs.sdlStatus.getSeverity().numberOfCriticalPriorityViolations;
        }
        else if (
            Lhs.sdlStatus.getSeverity().numberOfHighPriorityViolations !=
            Rhs.sdlStatus.getSeverity().numberOfHighPriorityViolations)
        {
            return
                Lhs.sdlStatus.getSeverity().numberOfHighPriorityViolations >
                Rhs.sdlStatus.getSeverity().numberOfHighPriorityViolations;
        }
        else
        {
            return Lhs.path < Rhs.path;
        }
    });

    // Fill data into Statistics 
    for (const auto& file : m_files)
    {
        m_stat.sumOfCriticalPriorityViolations += file.sdlStatus.getSeverity().numberOfCriticalPriorityViolations;
        m_stat.sumOfHighPriorityViolations += file.sdlStatus.getSeverity().numberOfHighPriorityViolations;
        if (!file.sdlStatus.isBuildByGoodCompiler()) { m_stat.sumOfBadCompilerUsages++; }
        if (!file.sdlStatus.isASLREnabled()) { m_stat.sumOfNoASLR++; }
        if (!file.sdlStatus.isDEPEnabled()) { m_stat.sumOfNoDEP++; }
        if (!file.sdlStatus.isSEHProtectionEnabled()) { m_stat.sumOfNoSEHProtection++; }
        if (!file.sdlStatus.isGSEnabled()) { m_stat.sumOfNoGS++; }
        if (file.sdlStatus.isHeapProtectionEnabled() == TriBool::False) { m_stat.sumOfNoHeapProtection++; }
        if (file.peConfig.isExecutable()) { m_stat.numberOfExecutableFiles++; }
        m_stat.sumOfSharedSections += file.peConfig.getSharedSectionNames().size();
        m_stat.sumOfBannedFuncs1 += file.peConfig.getImportedBannedFunctionsRequired().size();
        m_stat.sumOfBannedFuncs2 += file.peConfig.getimportedBannedFunctionsRecommended().size();
    }
}


bool DataCollectionDisplayer::hasData() const
{
    return !m_files.empty();
}


bool DataCollectionDisplayer::csvWrite(
    const fs::path& FilePath) const
{
    std::ofstream file(FilePath);
    if (!file)
    {
        return false;
    }
    write(m_files, m_stat, file, ',');
    return true;
}


void DataCollectionDisplayer::print() const
{
    write(m_files, m_stat, std::cout, '\t');
}


namespace {


template <typename OutPutDevice>
void write(
    const std::vector<FileStatus>& Files,
    const DataCollectionDisplayer::Statistics& Stat,
    OutPutDevice& Device,
    char Separator)
{
    Device
        << "#ofCV" << Separator
        << "#ofHV" << Separator
        << "(Built)" << Separator
        << "NewComp" << Separator
        << "(Ver)" << Separator
        << "GS" << Separator
        << "SEHP" << Separator
        << "DEP" << Separator
        << "ASLR" << Separator
        << "HeapP" << Separator
        << "#ofSS" << Separator
        << "#ofBF1" << Separator
        << "#ofBF2" << Separator
        << "(Path)" << Separator
        << "\n";

    for (const auto& file : Files)
    {
        const auto tm = UintToTm(file.peConfig.getTimeStamp());
        const auto note = file.peConfig.isVB6PE() ? "(VB6)"
            : file.peConfig.isManagedPE() ? "(NET)" : "";
        const auto heap = file.sdlStatus.isHeapProtectionEnabled() == TriBool::True ? "true"
            : file.sdlStatus.isHeapProtectionEnabled() == TriBool::False ? "false" : "N/A";

        Device
            << std::boolalpha << std::dec
            << file.sdlStatus.getSeverity().numberOfCriticalPriorityViolations << Separator
            << file.sdlStatus.getSeverity().numberOfHighPriorityViolations << Separator
            << tm.tm_year + 1900 << "/" << std::setw(2) << tm.tm_mon + 1 << Separator
            << file.sdlStatus.isBuildByGoodCompiler() << Separator
            << file.peConfig.getLinkerVersion() << note << Separator
            << file.sdlStatus.isGSEnabled() << Separator
            << file.sdlStatus.isSEHProtectionEnabled() << Separator
            << file.sdlStatus.isDEPEnabled() << Separator
            << file.sdlStatus.isASLREnabled() << Separator
            << heap << Separator
            << file.peConfig.getSharedSectionNames().size() << Separator
            << file.peConfig.getImportedBannedFunctionsRequired().size() << Separator
            << file.peConfig.getimportedBannedFunctionsRecommended().size() << Separator
            << file.path << Separator
            << "\n";
    }
    Device
        << std::setprecision(2) << std::fixed
        << static_cast<double>(Stat.sumOfCriticalPriorityViolations) / Files.size() << Separator
        << static_cast<double>(Stat.sumOfHighPriorityViolations) / Files.size() << Separator
        << Separator
        << static_cast<double>(Stat.sumOfBadCompilerUsages) / Files.size() * 100 << "%" << Separator
        << Separator
        << static_cast<double>(Stat.sumOfNoGS) / Files.size() * 100 << "%" << Separator
        << static_cast<double>(Stat.sumOfNoSEHProtection) / Files.size() * 100 << "%" << Separator
        << static_cast<double>(Stat.sumOfNoDEP) / Files.size() * 100 << "%" << Separator
        << static_cast<double>(Stat.sumOfNoASLR) / Files.size() * 100 << "%" << Separator
        << static_cast<double>(Stat.sumOfNoHeapProtection) / Stat.numberOfExecutableFiles * 100 << "%" << Separator
        << static_cast<double>(Stat.sumOfSharedSections) / Files.size() << Separator
        << static_cast<double>(Stat.sumOfBannedFuncs1) / Files.size() << Separator
        << static_cast<double>(Stat.sumOfBannedFuncs2) / Files.size() << Separator
        << "\n";
}


tm UintToTm(
    std::uint32_t SecSince1900)
{
    time_t t = SecSince1900;
    tm time{};
    ::gmtime_s(&time, &t);
    return time;
}


} // End of namespace {unnamed}
