//
// This module declares a class responsible for extracting interesting
// characteristics from a PE file
//
#pragma once


// C/C++ standard headers
#include <cstdint>
#include <vector>
#include <string>
#include <memory>
#include <filesystem>

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


////////////////////////////////////////////////////////////////////////////////
//
// prototypes
//

class ImportStatus;

class PEConfig
{
public:
    explicit PEConfig(
        const std::tr2::sys::path& Path);
    bool isExecutable() const;
    bool isVB6PE() const;
    bool isManagedPE() const;
    bool isDynamicBaseCompatible() const;
    bool isNXCompatible() const;
    bool isSEHProtectionEnabled() const;
    bool isGSEnabled() const;
    std::uint32_t getTimeStamp() const;
    std::uint32_t getLinkerVersion() const;
    const std::vector<std::string>& getSharedSectionNames() const;
    bool isHeapProtectionEnabled() const;
    const std::vector<std::string>& getImportedBannedFunctionsRequired() const;
    const std::vector<std::string>& getimportedBannedFunctionsRecommended() const;

private:
    bool m_isExecutalbe;
    bool m_hasDotNetHeader;
    bool m_isDynamicBaseCompatible;
    bool m_isNXCompatible;
    bool m_hasSEH;
    bool m_hasSafeSEH;
    bool m_hasGS;
    std::uint32_t m_timestamp;
    std::uint32_t m_linkerVersion;
    std::vector<std::string> m_sharedSectionNames;
    std::shared_ptr<ImportStatus> m_importStatus;
};


////////////////////////////////////////////////////////////////////////////////
//
// variables
//


////////////////////////////////////////////////////////////////////////////////
//
// implementations
//
