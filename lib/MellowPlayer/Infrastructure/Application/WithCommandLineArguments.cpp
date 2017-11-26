#include "WithCommandLineArguments.hpp"
#include <MellowPlayer/Infrastructure/CommandLineArguments/ICommandLineArguments.hpp>

using namespace MellowPlayer::Infrastructure;

WithCommandLineArguments::WithCommandLineArguments(IApplication& application,
                                                   ICommandLineArguments& commandLineArguments)
        : ApplicationDecorator(application), commandLineArguments_(commandLineArguments)
{

}

void WithCommandLineArguments::initialize()
{
    commandLineArguments_.parse();
    ApplicationDecorator::initialize();
}