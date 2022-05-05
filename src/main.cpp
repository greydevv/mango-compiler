#include <iostream>
#include <string>
#include <vector>
#include "path.h"
#include "Exception.h"
#include "ContextManager.h"
#include "compile.h"
#include "checks.h"
#include "io.h"

int main(int argc, char const *argv[])
{
    CompileArgs args = parseArgs(std::vector<std::string>(argv+1, argc+argv));
    
    if (args.paths.size() == 0)
    {
        fmt::print(fmt::emphasis::bold | fmt::fg(fmt::color::orange_red), "Error");
        fmt::print(fmt::emphasis::bold, ": no file names specified\n");
        return 1;
    }

    if (args.debug)
        // validate enums
        runChecks();

    for (const auto& p : args.paths)
    {
        ContextManager ctx;

        try
        {
            std::ostringstream outs;

            std::shared_ptr<ModuleAST> prog = compile(p, ctx, outs);

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
        ctx.clear();
    }

    return 0;
}
