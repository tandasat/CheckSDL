//
// This module implements a class responsible for extracting interesting
// characteristics from an import section
//
#include "stdafx.h"
#include "ImportStatus.h"


// C/C++ standard headers
// Other external headers
// Windows headers
// Project headers
#include "BannedFunctions.h"


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


bool IsMSVB(
    const char* ModuleName);


bool IsMSCOREE(
    const char* ModuleName);


bool IsBannedRequiredFunction(
    const char* FunctionName);


bool IsBannedRecommendedFunction(
    const char* FunctionName);


bool IsHeapProtectionFunction(
    const char* FunctionName);


} // End of namespace {unnamed}


////////////////////////////////////////////////////////////////////////////////
//
// variables
//


////////////////////////////////////////////////////////////////////////////////
//
// implementations
//

ImportStatus::ImportStatus(
    void* BaseAddress)
    : m_isVB6PE(false)
    , m_isManagedPE(false)
    , m_isMSVBVM60Imported(false)
    , m_isMSCOREEImported(false)
    , m_isHeapProtectionFunctionImported(false)
{
    auto bytesOfDirectoryEntry = 0ul;
    auto importDesc = static_cast<PIMAGE_IMPORT_DESCRIPTOR>(
        ::ImageDirectoryEntryToData(BaseAddress, TRUE,
        IMAGE_DIRECTORY_ENTRY_IMPORT, &bytesOfDirectoryEntry));
    if (!importDesc)
    {
        throw std::runtime_error("No Import Directory.");
    }

    const auto baseAddr = reinterpret_cast<std::uintptr_t>(BaseAddress);
    auto numberOfImportedModules = 0ul;
    for (; importDesc->OriginalFirstThunk; ++importDesc, ++numberOfImportedModules)
    {
        ProcessImportedModule(baseAddr, importDesc);
    }
    m_isVB6PE = (m_isMSVBVM60Imported && numberOfImportedModules == 1);
    m_isManagedPE = (m_isMSCOREEImported && numberOfImportedModules == 1);
}


bool ImportStatus::isVB6PE() const
{
    return m_isVB6PE;
}


bool ImportStatus::isManagedPE() const
{
    return m_isManagedPE;
}


bool ImportStatus::isHeapProtectionFunctionImported() const
{
    return m_isHeapProtectionFunctionImported;
}


const std::vector<std::string>& ImportStatus::getImportedBannedFunctionsRequired() const
{
    return m_importedBannedFunctionsRequired;
}


const std::vector<std::string>& ImportStatus::getimportedBannedFunctionsRecommended() const
{
    return m_importedBannedFunctionsRecommended;
}


void ImportStatus::ProcessImportedModule(
    std::uintptr_t BaseAddress,
    const PIMAGE_IMPORT_DESCRIPTOR ImportDesc)
{
    const auto moduleName = reinterpret_cast<char*>(BaseAddress + ImportDesc->Name);
    m_isMSVBVM60Imported = m_isMSVBVM60Imported || IsMSVB(moduleName);
    m_isMSCOREEImported = m_isMSCOREEImported || IsMSCOREE(moduleName);

    auto thunkData = reinterpret_cast<PIMAGE_THUNK_DATA>(
        BaseAddress + ImportDesc->OriginalFirstThunk);
    for (; thunkData->u1.Ordinal; ++thunkData)
    {
        ProcessImportedFunction(BaseAddress, thunkData);
    }
}


void ImportStatus::ProcessImportedFunction(
    std::uintptr_t BaseAddress,
    const PIMAGE_THUNK_DATA ThunkData)
{
    if (IMAGE_SNAP_BY_ORDINAL(ThunkData->u1.Ordinal))
    {
        return;
    }
    const auto importByName = reinterpret_cast<PIMAGE_IMPORT_BY_NAME>(
        BaseAddress + ThunkData->u1.AddressOfData);
    const auto functionName = reinterpret_cast<char*>(
        importByName->Name);

    if (IsBannedRequiredFunction(functionName))
    {
        m_importedBannedFunctionsRequired.push_back(functionName);
    }
    if (IsBannedRecommendedFunction(functionName))
    {
        m_importedBannedFunctionsRecommended.push_back(functionName);
    }
    m_isHeapProtectionFunctionImported = m_isHeapProtectionFunctionImported
        || IsHeapProtectionFunction(functionName);
}


namespace {


bool IsMSVB(
    const char* ModuleName)
{
    return ::_stricmp(ModuleName, "MSVBVM60.DLL") == 0;
}


bool IsMSCOREE(
    const char* ModuleName)
{
    return ::_stricmp(ModuleName, "MSCOREE.DLL") == 0;
}


bool IsBannedRequiredFunction(
    const char* FunctionName)
{
    return std::find_if(
        std::begin(BANNED_FUNCTIONS_REQUIRED),
        std::end(BANNED_FUNCTIONS_REQUIRED),
        [=](const char* Banned)
    {
        return strcmp(Banned, FunctionName) == 0;
    }) != std::end(BANNED_FUNCTIONS_REQUIRED);
}


bool IsBannedRecommendedFunction(
    const char* FunctionName)
{
    return std::find_if(
        std::begin(BANNED_FUNCTIONS_RECOMMENDED),
        std::end(BANNED_FUNCTIONS_RECOMMENDED),
        [=](const char* Banned)
    {
        return strcmp(Banned, FunctionName) == 0;
    }) != std::end(BANNED_FUNCTIONS_RECOMMENDED);
}


bool IsHeapProtectionFunction(
    const char* FunctionName)
{
    return ::strcmp("HeapSetInformation", FunctionName) == 0;
}


} // End of namespace {unnamed}
