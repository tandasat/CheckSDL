//
// This module implements a class responsible for evaluating PE characteristics
// based on the SDL criteria
//
#include "stdafx.h"
#include "SDLStatus.h"


// C/C++ standard headers
// Other external headers
// Windows headers
// Project headers
#include "PEConfig.h"


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


bool IsOldCompiler(
    std::uint32_t MajorLinkerVersion);


} // End of namespace {unnamed}


////////////////////////////////////////////////////////////////////////////////
//
// variables
//


////////////////////////////////////////////////////////////////////////////////
//
// implementations
//

SDLStatus::SDLStatus(
    const PEConfig* pe)
    : m_isBuildByGoodCompiler(!pe->isVB6PE() && !IsOldCompiler(pe->getLinkerVersion()))
    , m_isASLREnabled(pe->isManagedPE() || pe->isDynamicBaseCompatible())
    , m_isDEPEnabled(pe->isManagedPE() || pe->isNXCompatible())
    , m_isSEHProtectionEnabled(pe->isSEHProtectionEnabled())
    , m_isGSEnabled(pe->isManagedPE() || pe->isGSEnabled())
    , m_isHeapProtectionEnabled(!pe->isExecutable() ? TriBool::NotApplicable
                               : pe->isManagedPE() ? TriBool::True
                               : pe->isHeapProtectionEnabled() ? TriBool::True
                               : TriBool::False)
    , m_severity({})
{
    //
    // CriticalPriority
    //
    if (!m_isBuildByGoodCompiler)
    {
        ++m_severity.numberOfCriticalPriorityViolations;
    }
    if (!m_isASLREnabled)
    {
        ++m_severity.numberOfCriticalPriorityViolations;
    }
    if (!m_isDEPEnabled)
    {
        ++m_severity.numberOfCriticalPriorityViolations;
    }
    if (!m_isSEHProtectionEnabled)
    {
        ++m_severity.numberOfCriticalPriorityViolations;
    }
    if (!m_isGSEnabled)
    {
        ++m_severity.numberOfCriticalPriorityViolations;
    }

    //
    // HighPriority
    //
    if (m_isHeapProtectionEnabled == TriBool::False)
    {
        ++m_severity.numberOfHighPriorityViolations;
    }
    if (!pe->getSharedSectionNames().empty())
    {
        ++m_severity.numberOfHighPriorityViolations;
    }
    if (!pe->getImportedBannedFunctionsRequired().empty())
    {
        ++m_severity.numberOfHighPriorityViolations;
    }
    if (!pe->getimportedBannedFunctionsRecommended().empty())
    {
        ++m_severity.numberOfHighPriorityViolations;
    }
}


bool SDLStatus::isBuildByGoodCompiler() const
{
    return m_isBuildByGoodCompiler;
}


bool SDLStatus::isASLREnabled() const
{
    return m_isASLREnabled;
}


bool SDLStatus::isDEPEnabled() const
{
    return m_isDEPEnabled;
}


bool SDLStatus::isSEHProtectionEnabled() const
{
    return m_isSEHProtectionEnabled;
}


bool SDLStatus::isGSEnabled() const
{
    return m_isGSEnabled;
}


TriBool SDLStatus::isHeapProtectionEnabled() const
{
    return m_isHeapProtectionEnabled;
}


const Severity& SDLStatus::getSeverity() const
{
    return m_severity;
}


namespace {


// What hints in machine code can point me to the compiler which was used to generate it?
// http://reverseengineering.stackexchange.com/questions/11/what-hints-in-machine-code-can-point-me-to-the-compiler-which-was-used-to-genera
bool IsOldCompiler(
    std::uint32_t MajorLinkerVersion)
{
    if (MajorLinkerVersion == 3 ||     // Microsoft Visual C++ v3.x
        MajorLinkerVersion == 4 ||     // Microsoft Visual C++ v4.x
        MajorLinkerVersion == 5 ||     // Microsoft Visual C++ v5.x / Borland C++
        MajorLinkerVersion == 6 ||     // Microsoft VIsual Studio 6
        MajorLinkerVersion == 7)       // Microsoft VIsual Studio .NET
    {
        return true;
    }
    return false;
}


} // End of namespace {unnamed}
