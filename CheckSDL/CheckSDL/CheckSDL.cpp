//
// This module implements the top level procedure of this tool.
//
// Windows ISV Software Security Defenses
// http://msdn.microsoft.com/en-us/library/bb430720.aspx
//
#include "stdafx.h"


// C/C++ standard headers
// Other external headers
// Windows headers
// Project headers
#include "FileStatus.h"
#include "DataCollectionDisplayer.h"
#include "CollectFileStatuses.h"


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


bool AppMain(
    const std::vector<std::string>& Args);


} // End of namespace {unnamed}


////////////////////////////////////////////////////////////////////////////////
//
// variables
//


////////////////////////////////////////////////////////////////////////////////
//
// implementations
//

int main(int argc, char* argv[])
{
    int result = EXIT_FAILURE;
    try
    {
        std::vector<std::string> args;
        for (int i = 0; i < argc; ++i)
        {
            args.emplace_back(argv[i]);
        }
        if (AppMain(args))
        {
            result = EXIT_SUCCESS;
        }
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    catch (...)
    {
        std::cout << "Unhandled exception.\n";
    }
    return result;
}


namespace {


bool AppMain(
    const std::vector<std::string>& Args)
{
    if (Args.size() == 1)
    {
        std::cout
            << "Usage:\n"
            << "    >CheckSDL.exe target_path [csv_out_path]\n"
            << "\n"
            << "    target_path: A path of either a file or a directory to evaluate.\n"
            << "    csv_out_path: A path of a CSV file to save the result if needed.\n"
            << "                  An existing file will be overwritten without warning.\n"
            << "\n"
            << "Example:\n"
            << "    >CheckSDL.exe CheckSDL.exe\n"
            << "    >CheckSDL.exe C:\\Windows\\system32 system32.csv\n"
            << std::endl;
        return false;
    }

    const fs::path targetPath{ Args.at(1) };
    const DataCollectionDisplayer displayer{ CollectFileStatuses(targetPath) };
    if (!displayer.hasData())
    {
        std::cout << "No applicable file was found.\n";
        return false;
    }

    displayer.print();
    if (Args.size() > 2)
    {
        const fs::path csvPath{ Args.at(2) };
        if (!displayer.csvWrite(csvPath))
        {
            std::cout << "Failed to output on " << csvPath << std::endl;
        }
    }
    return true;
}


} // End of namespace {unnamed}
