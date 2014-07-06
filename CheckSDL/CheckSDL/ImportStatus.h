//
// This module declares a class responsible for extracting interesting
// characteristics from an import section
//
#pragma once


// C/C++ standard headers
#include <cstdint>
#include <vector>
#include <string>

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

class ImportStatus
{
public:
    explicit ImportStatus(
        void* BaseAddress);
    bool isVB6PE() const;
    bool isManagedPE() const;
    bool isHeapProtectionFunctionImported() const;
    const std::vector<std::string>& getImportedBannedFunctionsRequired() const;
    const std::vector<std::string>& getimportedBannedFunctionsRecommended() const;

private:
    void ProcessImportedModule(
        std::uintptr_t BaseAddress,
        const PIMAGE_IMPORT_DESCRIPTOR ImportDesc);
    void ProcessImportedFunction(
        std::uintptr_t BaseAddress,
        const PIMAGE_THUNK_DATA ThunkData);

    bool m_isVB6PE;
    bool m_isManagedPE;
    bool m_isMSVBVM60Imported;
    bool m_isMSCOREEImported;
    bool m_isHeapProtectionFunctionImported;
    std::vector<std::string> m_importedBannedFunctionsRequired;
    std::vector<std::string> m_importedBannedFunctionsRecommended;
};


////////////////////////////////////////////////////////////////////////////////
//
// variables
//


////////////////////////////////////////////////////////////////////////////////
//
// implementations
//
