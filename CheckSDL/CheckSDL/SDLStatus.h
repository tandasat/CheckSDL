//
// This module declares a class responsible for evaluating PE characteristics
// based on the SDL criteria
//
#pragma once


// C/C++ standard headers
// Other external headers
// Windows headers
// Project headers


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

struct Severity
{
    std::uint32_t numberOfCriticalPriorityViolations;
    std::uint32_t numberOfHighPriorityViolations;
};


////////////////////////////////////////////////////////////////////////////////
//
// prototypes
//

class PEConfig;

class SDLStatus
{
public:
    explicit SDLStatus(
        const PEConfig* pe);
    bool isBuildByGoodCompiler() const;
    bool isASLREnabled() const;
    bool isDEPEnabled() const;
    bool isSEHProtectionEnabled() const;
    bool isGSEnabled() const;
    TriBool isHeapProtectionEnabled() const;
    const Severity& getSeverity() const;

private:
    bool m_isBuildByGoodCompiler;
    bool m_isASLREnabled;
    bool m_isDEPEnabled;
    bool m_isSEHProtectionEnabled;
    bool m_isGSEnabled;
    TriBool m_isHeapProtectionEnabled;
    Severity m_severity;
};


////////////////////////////////////////////////////////////////////////////////
//
// variables
//


////////////////////////////////////////////////////////////////////////////////
//
// implementations
//
