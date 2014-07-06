//
// This module implements functions responsible for collecting FileStatus items
//
#include "stdafx.h"
#include "CollectFileStatuses.h"


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


bool AnalyzeFile(
    const fs::path& Path,
    std::vector<FileStatus>& Files);


bool IsSupportedFileType(
    const fs::path& Path);


} // End of namespace {unnamed}


////////////////////////////////////////////////////////////////////////////////
//
// variables
//


////////////////////////////////////////////////////////////////////////////////
//
// implementations
//

std::vector<FileStatus> CollectFileStatuses(
    const fs::path& TargetPath)
{
    if (!fs::exists(TargetPath))
    {
        throw std::runtime_error("The specified file or directory does not exist.");
    }

    std::function<void(const fs::path&, std::vector<FileStatus>&)> traverse;
    traverse = [&traverse](
        const fs::path& Path,
        std::vector<FileStatus>& Files)
    {
        if (fs::is_regular_file(Path))
        {
            if (IsSupportedFileType(Path))
            {
                AnalyzeFile(Path, Files);
            }
        }
        else if (fs::is_directory(Path))
        {
            fs::directory_iterator endit;
            for (fs::directory_iterator it{ Path }; it != endit; ++it)
            {
                traverse(it->path(), Files);
            }
        }
    };

    std::vector<FileStatus> files;
    traverse(TargetPath, files);
    return files;
}


namespace {


bool AnalyzeFile(
    const fs::path& Path,
    std::vector<FileStatus>& Files)
{
    try
    {
        std::cout << Path << std::endl;
        PEConfig pe{ Path };
        SDLStatus sdl{ &pe };
        Files.emplace_back(Path, std::move(pe), std::move(sdl));
        return true;
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    return false;
}


bool IsSupportedFileType(
    const fs::path& Path)
{
    return(::_stricmp(Path.extension().c_str(), ".exe") == 0
        || ::_stricmp(Path.extension().c_str(), ".dll") == 0
        || ::_stricmp(Path.extension().c_str(), ".ocx") == 0);
}


} // End of namespace {unnamed}
