//
// This module implements a class responsible for extracting interesting
// characteristics from a PE file
//
#include "stdafx.h"
#include "PEConfig.h"


// C/C++ standard headers
// Other external headers
// Windows headers
// Project headers
#include "ImportStatus.h"


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


bool HasDotNetHeader(
    void* BaseAddress);


bool HasSEH(
    const IMAGE_NT_HEADERS* Nt);


bool IsDynamicBaseCompatible(
    const IMAGE_NT_HEADERS* Nt);


bool IsNXCompatible(
    const IMAGE_NT_HEADERS* Nt);


bool HasSafeSEH(
    void* BaseAddress);


bool HasGS(
    void* BaseAddress,
    std::uint64_t AddressGap);


std::vector<std::string> GetSharedSectionNames(
    const IMAGE_NT_HEADERS* Nt);


bool IsHeapProtectionSupportedCompilerVersion(
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

PEConfig::PEConfig(
    const fs::path& Path)
    : m_isExecutalbe(false)
    , m_hasDotNetHeader(false)
    , m_isDynamicBaseCompatible(false)
    , m_isNXCompatible(false)
    , m_hasSEH(false)
    , m_hasSafeSEH(false)
    , m_hasGS(false)
    , m_timestamp(0)
    , m_linkerVersion(0)
{
    // As LoadLibraryEx may display an error dialog
    ::SetErrorMode(SEM_FAILCRITICALERRORS);

    const auto dll = std::experimental::unique_resource(
        ::LoadLibraryExA(Path.string().c_str(), nullptr, DONT_RESOLVE_DLL_REFERENCES),
        ::FreeLibrary);
    if (!dll)
    {
        throw std::runtime_error("LoadLibraryExA failed.");
    }

    // FIXME: Do not trust what a PE header says. Treat it like malware.
    const auto baseAddr =
        reinterpret_cast<std::uintptr_t>(dll.get()) & 0xffffffffffff0000;
    const auto dos = reinterpret_cast<PIMAGE_DOS_HEADER>(baseAddr);
    const auto nt = reinterpret_cast<PIMAGE_NT_HEADERS>(baseAddr + dos->e_lfanew);
    if (nt->OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR32_MAGIC)
    {
        throw std::runtime_error("Not PE32 header was detected.");
    }

    m_importStatus = std::make_shared<ImportStatus>(dos);
    m_isExecutalbe = ::_stricmp(Path.extension().c_str(), ".exe") == 0;
    m_hasDotNetHeader = HasDotNetHeader(dos);
    m_isDynamicBaseCompatible = IsDynamicBaseCompatible(nt);
    m_isNXCompatible = IsNXCompatible(nt);
    m_hasSEH = HasSEH(nt);
    m_hasSafeSEH = HasSafeSEH(dos);
    m_hasGS = HasGS(dos, nt->OptionalHeader.ImageBase - baseAddr);
    m_timestamp = nt->FileHeader.TimeDateStamp;
    m_linkerVersion = nt->OptionalHeader.MajorLinkerVersion;
    m_sharedSectionNames = GetSharedSectionNames(nt);
}


bool PEConfig::isExecutable() const
{
    return m_isExecutalbe;
}


bool PEConfig::isVB6PE() const
{
    return m_importStatus->isVB6PE();
}


bool PEConfig::isManagedPE() const
{
    return m_importStatus->isManagedPE() && m_hasDotNetHeader;
}


bool PEConfig::isDynamicBaseCompatible() const
{
    return m_isDynamicBaseCompatible;
}


bool PEConfig::isNXCompatible() const
{
    return m_isNXCompatible;
}


bool PEConfig::isSEHProtectionEnabled() const
{
    return !m_hasSEH || m_hasSafeSEH;
}


bool PEConfig::isGSEnabled() const
{
    return m_hasGS;
}


std::uint32_t PEConfig::getTimeStamp() const
{
    return m_timestamp;
}


std::uint32_t PEConfig::getLinkerVersion() const
{
    return m_linkerVersion;
}


const std::vector<std::string>& PEConfig::getSharedSectionNames() const
{
    return m_sharedSectionNames;
}


bool PEConfig::isHeapProtectionEnabled() const
{
    return IsHeapProtectionSupportedCompilerVersion(m_linkerVersion)
        || m_importStatus->isHeapProtectionFunctionImported();
}


const std::vector<std::string>& PEConfig::getImportedBannedFunctionsRequired() const
{
    return m_importStatus->getImportedBannedFunctionsRequired();
}


const std::vector<std::string>& PEConfig::getimportedBannedFunctionsRecommended() const
{
    return m_importStatus->getimportedBannedFunctionsRecommended();
}


namespace {


bool HasDotNetHeader(
    void* BaseAddress)
{
    auto bytesOfDirectoryEntry = 0ul;
    return (::ImageDirectoryEntryToData(BaseAddress, TRUE,
            IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR, &bytesOfDirectoryEntry) != nullptr);
}


// SEH Stack Based Windows Buffer Overflow Tutorial
// http://www.thegreycorner.com/2010/01/seh-stack-based-windows-buffer-overflow.html
bool HasSEH(
    const IMAGE_NT_HEADERS* Nt)
{
    return (Nt->OptionalHeader.DllCharacteristics &
        IMAGE_DLLCHARACTERISTICS_NO_SEH) == 0;
}


bool IsDynamicBaseCompatible(
    const IMAGE_NT_HEADERS* Nt)
{
    return (Nt->OptionalHeader.DllCharacteristics &
        IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE) != 0;
}


bool IsNXCompatible(
    const IMAGE_NT_HEADERS* Nt)
{
    return (Nt->OptionalHeader.DllCharacteristics &
        IMAGE_DLLCHARACTERISTICS_NX_COMPAT) != 0;
}


bool HasSafeSEH(
    void* BaseAddress)
{
    auto bytesOfDirectoryEntry = 0ul;
    const auto moduleConfig = static_cast<PIMAGE_LOAD_CONFIG_DIRECTORY>(
        ::ImageDirectoryEntryToData(BaseAddress, TRUE,
            IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG, &bytesOfDirectoryEntry));
    return moduleConfig && moduleConfig->SEHandlerCount;
}


bool HasGS(
    void* BaseAddress,
    std::uint64_t AddressGap)
{
    const auto isValidCookieValue = [](
        DWORD* Address)
    {
        __try { return *Address != 0; }
        __except (1) { return false; }
    };

    auto bytesOfDirectoryEntry = 0ul;
    const auto moduleConfig = static_cast<PIMAGE_LOAD_CONFIG_DIRECTORY>(
        ::ImageDirectoryEntryToData(BaseAddress, TRUE,
            IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG, &bytesOfDirectoryEntry));
    return moduleConfig
        && moduleConfig->SecurityCookie
        && isValidCookieValue(reinterpret_cast<DWORD*>(
                                moduleConfig->SecurityCookie - AddressGap));
}


// Appendix G: SDL Requirement: No Shared Sections
// http://msdn.microsoft.com/en-us/library/windows/desktop/cc307397.aspx
std::vector<std::string> GetSharedSectionNames(
    const IMAGE_NT_HEADERS* Nt)
{
    std::vector<std::string> sharedSectionNames;
    const auto firstSection = IMAGE_FIRST_SECTION(Nt);
    for (std::uint32_t i = 0; i < Nt->FileHeader.NumberOfSections; ++i)
    {
        const auto section = firstSection + i;
        if ((section->Characteristics & IMAGE_SCN_MEM_SHARED) == 0)
        {
            continue;
        }

        std::vector<char> sectionName;
        for (auto c : section->Name)
        {
            const char normalized =
                (c == '\0' || ::isprint(c)) ? c : '?';
            sectionName.push_back(normalized);
        }
        sectionName.push_back('\0');    // null-terminator
        sharedSectionNames.push_back(sectionName.data());
    }
    return sharedSectionNames;
}


// HeapSetInformation in Visual C++ 2010 beta 2
// http://blogs.msdn.com/b/sdl/archive/2010/01/14/heapsetinformation-in-visual-c-2010-beta-2.aspx
bool IsHeapProtectionSupportedCompilerVersion(
    std::uint32_t MajorLinkerVersion)
{
    return MajorLinkerVersion >= 10;   // Microsoft VIsual Studio 2010
}


} // End of namespace {unnamed}
