#include <iostream>
#include <string>
#include <vector>
#include "Exception.h"
#include "ContextManager.h"
#include "compile.h"
#include "checks.h"
#include "io.h"

int main(int argc, char const *argv[])
{
    CompileArgs args = parseArgs(std::vector<std::string>(argv+1, argc+argv));
    if (args.fnames.size() == 0)
    {
        fmt::print(fmt::emphasis::bold | fmt::fg(fmt::color::orange_red), "Error");
        fmt::print(fmt::emphasis::bold, ": no file names specified\n");
        return 1;
    }

    if (args.debug)
        // validate enums
        runChecks();

    for (auto& fname : args.fnames)
    {
        if (fname.find_first_of('-') == 0)
        {
            fmt::print(fmt::emphasis::bold | fmt::fg(fmt::color::orange_red), "Error");
            fmt::print(fmt::emphasis::bold, ": unknown argument: '{}'\n", fname);
            return 1;
        }

        ContextManager ctx;

        try
        {
            std::ostringstream outs;

            // the 'canonical' methods resolve things like '../' and './' in
            // the given paths
            // for example, a/b/c/../d is resolved to the literal a/b/d
            //
            // using 'weakly_canonical' does not throw an error if the file is
            // not found
            std::filesystem::path absPath = std::filesystem::weakly_canonical(fname);
            std::shared_ptr<ModuleAST> prog = compile(absPath, ctx, outs);

            if (args.debug)
            {
                prog->print();
                std::cout << '\n' << outs.str();
            }
        }
        catch (const BaseException& e)
        {
            std::cout << e.getMsg(ctx) << std::flush;
            return 1;
        }
    }

    return 0;
}
