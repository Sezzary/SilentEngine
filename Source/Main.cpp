#include "Framework.h"

#include "Application.h"
#include "Platform.h"

int main()
{
    InitializeSignals();

    try
    {
        g_App.Initialize();
        g_App.Run();
        g_App.Deinitialize();
    }
    catch (const std::exception& ex)
    {
        CriticalErrorHandler(ex.what());
        return EXIT_FAILURE;
    }

    Debug::Deinitialize();
    return EXIT_SUCCESS;
}
